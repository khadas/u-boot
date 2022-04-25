#!/bin/bash -e

# Copyright (c) 2018 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x

SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}

# Path to sign-gxl-boot util
TOOL_PATH=${TOOL_PATH:-$(dirname $(readlink -f $0))/../signing-tool-gxl/}

# Temporary files directory
if [ -z "$TMP" ]; then
    TMP=${SCRIPT_PATH}/tmp
fi

usage() {
    cat << EOF
Usage: $(basename $0) --help

       Combine binaries into an unsigned bootloader:

       $(basename $0) --create-unsigned-bl \\
                      --bl2 <bl2_new.bin> \\
                      --bl30 <bl30_new.bin> \\
                      --bl31 <bl31.img> \\
                      [--bl32 <bl32.img>] \\
                      --bl33 <bl33.bin> \\
                      -o <bl.bin>

       Create a root hash for OTP:

       $(basename $0) --create-root-hash \\
                      [--key-hash-ver [1 | 2]] \\
                      --root-key-0 <work/root.pem> \\
                      --root-key-1 <work/root.pem> \\
                      --root-key-2 <work/root.pem> \\
                      --root-key-3 <work/root.pem> \\
                      -o work/rootkeys-hash.bin

       Create a root hash for OTP from public key only:

       $(basename $0) --create-root-hash-from-pub \\
                      [--key-hash-ver [1 | 2]] \\
                      --root-key-0 work/root.pub \\
                      --root-key-1 work/root.pub \\
                      --root-key-2 work/root.pub \\
                      --root-key-3 work/root.pub \\
                      -o work/rootkeys-hash.bin

      Environment Variables:

      TMP:          path to a temporary directory. Defaults to <this script's path>/tmp
EOF
    exit 1
}

check_file() {
    if [ ! -f "$2" ]; then echo Error: Unable to open $1: \""$2"\"; exit 1 ; fi
}

# Hash root/bl2 RSA keys'.
#
#   v1 only sha256(n[keylen] + e)
#   v2 sha256 the entire mincrypt keyblob including length
#
# $1: key hash version
# $2: precomputed binary key file
# $3: keylen
# $4: output hash file
hash_rsa_bin() {
    local keyhashver=$1
    local key=$2
    local keylen=$3
    local output=$4
    if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]; then
        echo "Argument error"
        exit 1
    fi
    local insize=$(wc -c < $key)
    if [ $insize -ne 1036 ]; then
        echo "Keyfile wrong size"
        exit 1
    fi
    if [ $keylen -ne 1024 ] && [ $keylen -ne 2048 ] && [ $keylen -ne 4096 ]; then
        echo "Invalid keylen"
        exit 1
    fi
    local keylenbytes=$(( $keylen / 8 ))

    if [ $keyhashver -eq 2 ]; then
        cp $key $TMP/keydata
    else
        # modulus - rsa_public_key_t.n[key length]
        dd if=$key of=$TMP/keydata bs=$keylenbytes count=1 >& /dev/null
        # exponent - rsa_public_key_t.e
        dd if=$key of=$TMP/keydata bs=1 skip=512 count=4 \
            oflag=append conv=notrunc >& /dev/null
    fi

    openssl dgst -sha256 -binary $TMP/keydata > $output
}

# --pack-bl2  -i bl2.bin  -o bl2.bin.img
pack_bl2() {
    local input=""
    local output=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        case "$arg" in
            -i)
                i=$((i + 1))
                input="${argv[$i]}"
                ;;
            -o)
                i=$((i + 1))
                output="${argv[$i]}"
                ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file bl2  "$input"
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    # Add header
    ${TOOL_PATH}/sign-boot-gxl --add-aml-block-header \
            -i $input \
            -o $TMP/bl2.img-noiv

    # Add nonce
    dd if=/dev/urandom of=$TMP/nonce.bin bs=16 count=1 >& /dev/null
    cat $TMP/nonce.bin $TMP/bl2.img-noiv > $TMP/bl2.img

    # Truncate to correct length
    # TODO should check that end of output is all zeroes before truncating
    truncate -s 49152 $TMP/bl2.img

    # Add sha256 hash into header at byte offset 80
    dd if=$TMP/bl2.img of=$TMP/chkdata bs=1 skip=16 count=64 >& /dev/null
    dd if=$TMP/bl2.img of=$TMP/chkdata bs=1 skip=112 \
        oflag=append conv=notrunc  >& /dev/null
    openssl dgst -sha256 -binary $TMP/chkdata > $TMP/bl2.sha

    dd if=$TMP/bl2.sha of=$TMP/bl2.img bs=1 seek=80 \
        conv=notrunc  >& /dev/null

    cp $TMP/bl2.img $output
}

# Convert RSA public PEM key [from pkcs#8] to pkcs#1
# $1: input RSA private .PEM
# $2: output public key file
pub_to_pkcs1() {
    local input=$1
    local output=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi

    openssl 2>/dev/null rsa -in $input -pubin -pubout -RSAPublicKey_out -out $output

    if [ $? -ne 0 ]; then
        echo "Error converting key"
        exit 1
    fi
}

create_root_hash() {
    local rootkey0=""
    local rootkey1=""
    local rootkey2=""
    local rootkey3=""
    local output=""
    local sigver=""
    local keyhashver=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --root-key-0)
                rootkey0="${argv[$i]}" ;;
            --root-key-1)
                rootkey1="${argv[$i]}" ;;
            --root-key-2)
                rootkey2="${argv[$i]}" ;;
            --root-key-3)
                rootkey3="${argv[$i]}" ;;
            --sig-ver)
                sigver="${argv[$i]}" ;;
            --key-hash-ver)
                keyhashver="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file rootkey0 "$rootkey0"
    check_file rootkey1 "$rootkey1"
    check_file rootkey2 "$rootkey2"
    check_file rootkey3 "$rootkey3"
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi
    if [ -z "$sigver" ]; then
        sigver=1
    fi
    if [ -z "$keyhashver" ]; then
        keyhashver=1
    fi

    echo "Creating root hash using v"$keyhashver" key hash version"
    local keylenbytes=$(get_pem_key_len $rootkey0)
    local keylen=$(( $keylenbytes * 8 ))

    # Convert PEM key to rsa_public_key_t (precomputed RSA public key)
    pem_to_bin $rootkey0 $TMP/rootkey0.bin
    pem_to_bin $rootkey1 $TMP/rootkey1.bin
    pem_to_bin $rootkey2 $TMP/rootkey2.bin
    pem_to_bin $rootkey3 $TMP/rootkey3.bin

    # hash of keys
    hash_rsa_bin $keyhashver $TMP/rootkey0.bin $keylen $TMP/rootkey0.sha
    hash_rsa_bin $keyhashver $TMP/rootkey1.bin $keylen $TMP/rootkey1.sha
    hash_rsa_bin $keyhashver $TMP/rootkey2.bin $keylen $TMP/rootkey2.sha
    hash_rsa_bin $keyhashver $TMP/rootkey3.bin $keylen $TMP/rootkey3.sha

    cat $TMP/rootkey0.sha $TMP/rootkey1.sha $TMP/rootkey2.sha $TMP/rootkey3.sha > $TMP/rootkeys.sha
    openssl dgst -sha256 -binary $TMP/rootkeys.sha > $output
}

create_root_hash_from_pub() {
    local rootkey0=""
    local rootkey1=""
    local rootkey2=""
    local rootkey3=""
    local output=""
    local sigver=""
    local keyhashver=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --root-key-0)
                rootkey0="${argv[$i]}" ;;
            --root-key-1)
                rootkey1="${argv[$i]}" ;;
            --root-key-2)
                rootkey2="${argv[$i]}" ;;
            --root-key-3)
                rootkey3="${argv[$i]}" ;;
            --sig-ver)
                sigver="${argv[$i]}" ;;
            --key-hash-ver)
                keyhashver="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file rootkey0 "$rootkey0"
    check_file rootkey1 "$rootkey1"
    check_file rootkey2 "$rootkey2"
    check_file rootkey3 "$rootkey3"
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi
    if [ -z "$sigver" ]; then
        sigver=1
    fi
    if [ -z "$keyhashver" ]; then
        keyhashver=1
    fi

    local keylenbytes=$(get_pub_pem_key_len $rootkey0)
    local keylen=$(( $keylenbytes * 8 ))

    pub_to_pkcs1 $rootkey0 $TMP/rootkey0.pub
    pub_to_pkcs1 $rootkey1 $TMP/rootkey1.pub
    pub_to_pkcs1 $rootkey2 $TMP/rootkey2.pub
    pub_to_pkcs1 $rootkey3 $TMP/rootkey3.pub

    # Convert PEM key to rsa_public_key_t (precomputed RSA public key)
    pem_to_bin $TMP/rootkey0.pub $TMP/rootkey0.bin
    pem_to_bin $TMP/rootkey1.pub $TMP/rootkey1.bin
    pem_to_bin $TMP/rootkey2.pub $TMP/rootkey2.bin
    pem_to_bin $TMP/rootkey3.pub $TMP/rootkey3.bin

    # hash of keys
    hash_rsa_bin $keyhashver $TMP/rootkey0.bin $keylen $TMP/rootkey0.sha
    hash_rsa_bin $keyhashver $TMP/rootkey1.bin $keylen $TMP/rootkey1.sha
    hash_rsa_bin $keyhashver $TMP/rootkey2.bin $keylen $TMP/rootkey2.sha
    hash_rsa_bin $keyhashver $TMP/rootkey3.bin $keylen $TMP/rootkey3.sha

    cat $TMP/rootkey0.sha $TMP/rootkey1.sha $TMP/rootkey2.sha $TMP/rootkey3.sha > $TMP/rootkeys.sha
    openssl dgst -sha256 -binary $TMP/rootkeys.sha > $output
}

# Get key len in bytes of public PEM RSA key
# $1: PEM file
get_pub_pem_key_len() {
    local pem=$1
    local bits=0
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\""
        exit 1
    fi
    bits=$(openssl rsa -in $pem -pubin -text -noout | \
        grep '^Public-Key: (' | \
        sed 's/Public-Key: (//' | \
        sed 's/ bit)//')
    if [ "$bits" -ne 1024 ] && [ "$bits" -ne 2048 ] &&
       [ "$bits" -ne 4096 ] && [ "$bits" -ne 8192]; then
       echo "Unexpected key size  $bits"
       exit 1
    fi
    echo $(( $bits / 8 ))
}

# Get key len in bytes of private PEM RSA key
# $1: PEM file
get_pem_key_len() {
    local pem=$1
    local bits=0
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\""
        exit 1
    fi
    bits=$(openssl rsa -in $pem -text -noout | \
        grep 'Private-Key: (' | \
        sed -r 's/(RSA )?Private-Key: \(//'| \
        sed -r 's/ bit(,.?[0-9].?primes)?\)//')
    if [ "$bits" -ne 1024 ] && [ "$bits" -ne 2048 ] &&
       [ "$bits" -ne 4096 ] && [ "$bits" -ne 8192]; then
       echo "Unexpected key size  $bits"
       exit 1
    fi
    echo $(( $bits / 8 ))
}


# Pad file to len by adding 0's to end of file
# $1: file
# $2: len
pad_file() {
    local file=$1
    local len=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi
    local filesize=$(wc -c < ${file})
    local padlen=$(( $len - $filesize ))
    if [ $len -lt $filesize ]; then
        echo "File larger than expected.  $filesize, $len"
        exit 1
    fi
    dd if=/dev/zero of=$file oflag=append conv=notrunc bs=1 \
        count=$padlen >& /dev/null
}

# add header to bl3x
pack_bl3x() {
    local input=""
    local output=""
    local arb_cvn=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -i)
                input="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            -v)
                arb_cvn="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file "input" "$input"
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi
    if [ -z "$arb_cvn" ]; then
        arb_cvn="0"
    fi

    # Check padding
    local imagesize=$(wc -c < ${input})
    local rem=$(( $imagesize % 16 ))
    if [ $rem -ne 0 ]; then
        #echo "Input $input not 16 byte aligned?"
        local topad=$(( 16 - $rem ))
        imagesize=$(( $imagesize + $topad ))
        cp $input $TMP/blpad.bin
        pad_file $TMP/blpad.bin $imagesize
        input=$TMP/blpad.bin
    fi

    # Hash payload
    openssl dgst -sha256 -binary $input > $TMP/bl-pl.sha

    # Add hash to header
    ${TOOL_PATH}/sign-boot-gxl --create-sbImage-header \
            --hash $TMP/bl-pl.sha \
            --arb-cvn $arb_cvn \
            -o $TMP/bl.hdr

    # Pad header
    # pad to (sizeof(sbImageHeader_t) - nonce[16] - checksum[32])
    pad_file $TMP/bl.hdr $(( 656 - 16 - 32))

    openssl dgst -sha256 -binary $TMP/bl.hdr > $TMP/bl.hdr.sha

    # Create nonce
    dd if=/dev/urandom of=$TMP/nonce.bin bs=16 count=1 >& /dev/null

    # Combine nonce + hdr + sha + payload
    cat $TMP/nonce.bin $TMP/bl.hdr $TMP/bl.hdr.sha $input > $TMP/bl.bin

    cp $TMP/bl.bin $output
}

# Convert RSA private PEM key to precomputed binary key file
# If input is already the precomputed binary key file, then it is simply copied
# to the output
# $1: input RSA private .PEM
# $2: output precomputed binary key file
pem_to_bin() {
    local input=$1
    local output=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi

    local insize=$(wc -c < $input)
    if [ $insize -eq 1036 ]; then
        # input is already precomputed binary key file
        cp $input $output
    fi

    local pycmd="import sys; \
                 sys.path.append(\"${TOOL_PATH}\"); \
                 import pem_extract_pubkey; \
                 sys.stdout.write(pem_extract_pubkey.extract_pubkey( \
                    \"$input\", headerMode=False));"
    /usr/bin/env python -c "$pycmd" > $output
}

# Create FIP
create_fip_unsigned() {
    local bl30=""
    local bl31=""
    local bl32=""
    local bl31_info=""
    local bl32_info=""
    local bl33=""
    local output=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --bl30)
                bl30="${argv[$i]}" ;;
            --bl31)
                bl31="${argv[$i]}" ;;
            --bl32)
                bl32="${argv[$i]}" ;;
            --bl31-info)
                bl31_info="${argv[$i]}" ;;
            --bl32-info)
                bl32_info="${argv[$i]}" ;;
            --bl33)
                bl33="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1 ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file bl30 "$bl30"
    check_file bl31 "$bl31"
    check_file bl32 "$bl32"
    check_file bl31_info "$bl31_info"
    check_file bl32_info "$bl32_info"
    check_file bl33 "$bl33"

    if [ -z "$output" ]; then echo Error: Missing output file option -o ; exit 1; fi

    #dummy rsa/key/iv because utility can't handle unsigned yet
    dd if=/dev/zero of=$TMP/zerorsakey bs=1 count=1036 >& /dev/null
    dd if=/dev/zero of=$TMP/zeroaeskey bs=1 count=32 >& /dev/null
    dd if=/dev/zero of=$TMP/zeroaesiv bs=1 count=16 >& /dev/null

    # Create header and add keys
    local argv=("$@")
    ${TOOL_PATH}/sign-boot-gxl --create-fip-header \
        --bl30 $bl30 --bl30-key $TMP/zerorsakey \
        --bl30-aes-key $TMP/zeroaeskey --bl30-aes-iv $TMP/zeroaesiv \
        --bl31 $bl31 --bl31-key $TMP/zerorsakey \
        --bl31-aes-key $TMP/zeroaeskey --bl31-aes-iv $TMP/zeroaesiv \
        --bl32 $bl32 --bl32-key $TMP/zerorsakey  \
        --bl32-aes-key $TMP/zeroaeskey --bl32-aes-iv $TMP/zeroaesiv \
        --bl33 $bl33 --bl33-key $TMP/zerorsakey  \
        --bl33-aes-key $TMP/zeroaeskey --bl33-aes-iv $TMP/zeroaesiv \
        --bl31-info $bl31_info --bl32-info $bl32_info \
        --kernel-key $TMP/zerorsakey    \
        --kernel-aes-key $TMP/zeroaeskey --kernel-aes-iv $TMP/zeroaesiv \
        -o $TMP/fip.hdr

    # Pad header to size
    # pad to (sizeof(fip hdr) - nonce[16] - checksum[32])
    pad_file $TMP/fip.hdr $(( (16*1024) - 16 - 32))

    openssl dgst -sha256 -binary $TMP/fip.hdr > $TMP/fip.hdr.sha

    # Create nonce
    dd if=/dev/urandom of=$TMP/nonce.bin bs=16 count=1 >& /dev/null

    # Combine nonce + hdr + sha
    cat $TMP/nonce.bin $TMP/fip.hdr $TMP/fip.hdr.sha > $TMP/fip.bin

    cp $TMP/fip.bin $output
}

# Check (bl31/bl32) input is .img
# 1: input
# returns True or False
is_img_format() {
    local input=$1
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\""
        exit 1
    fi
    local insize=$(wc -c < $input)
    if [ $insize -le 512 ]; then
        # less than size of img header
        echo False
        return
    fi

    local inmagic=$(xxd -p -l 4 $input)
    if [ "$inmagic" == "65873412" ]; then
        # Input has FIP_TOC_ENTRY_EXT_FLAG, so it is in .img format.
        # Strip off 0x200 byte header.
        echo True
    else
        echo False
    fi
}

# Convert from .img to .bin.  If not .img format, copy to output.
# 1: input
# 2: output
convert_img_to_bin() {
    local input=$1
    local output=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi
    local insize=$(wc -c < $input)
    if [ $insize -le 512 ]; then
        # less than size of img header
        cp $input $output
        return
    fi

    local inmagic=$(xxd -p -l 4 $input)
    if [ "$inmagic" == "65873412" ]; then
        # Input has FIP_TOC_ENTRY_EXT_FLAG, so it is in .img format.
        # Strip off 0x200 byte header.
        tail -c +513 $input > $output
    else
        cp $input $output
    fi
}


# input bl2, bl30/31/32/33kernel .bin
create_unsigned_bl() {
    local bl2=""
    local bl30=""
    local bl31=""
    local bl32=""
    local bl33=""
    local output=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --bl2)
                bl2="${argv[$i]}" ;;
            --bl30)
                bl30="${argv[$i]}" ;;
            --bl31)
                bl31="${argv[$i]}" ;;
            --bl32)
                bl32="${argv[$i]}" ;;
            --bl33)
                bl33="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1 ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file bl2 "$bl2"
    check_file bl30 "$bl30"
    check_file bl31 "$bl31"
    if [ "$bl32" != "" ]; then
        check_file bl32 "$bl32"
    fi
    check_file bl33 "$bl33"

    if [ -z "$output" ]; then echo Error: Missing output file option -o ; exit 1; fi

    if [ "$(is_img_format $bl31)" != "True" ]; then
        echo Error. Expected .img format for \"$bl31\"
        exit 1
    fi
    if [ "$bl32" != "" ]; then
        if [ "$(is_img_format $bl32)" != "True" ]; then
            echo Error. Expected .img format for \"$bl32\"
            exit 1
        fi
    fi

    convert_img_to_bin $bl31 $TMP/bl31.bin
    if [ "$bl32" != "" ]; then
        convert_img_to_bin $bl32 $TMP/bl32.bin
    fi

    pack_bl2 -i $bl2 -o $TMP/bl2.bin.img

    pack_bl3x -i $bl30 -o $TMP/bl30.bin.img
    pack_bl3x -i $TMP/bl31.bin -o $TMP/bl31.bin.img
    if [ "$bl32" != "" ]; then
        pack_bl3x -i $TMP/bl32.bin -o $TMP/bl32.bin.img
    else
        rm -f $TMP/bl32.bin.img
        touch $TMP/bl32.bin.img
        # use bl31 as dummy bl32 img info
        cp $bl31 $TMP/bl32.img-info
        bl32=$TMP/bl32.img-info
    fi
    pack_bl3x -i $bl33 -o $TMP/bl33.bin.img

    create_fip_unsigned \
        --bl30     $TMP/bl30.bin.img \
        --bl31     $TMP/bl31.bin.img \
        --bl32     $TMP/bl32.bin.img \
        --bl31-info     $bl31 \
        --bl32-info     $bl32 \
        --bl33     $TMP/bl33.bin.img \
        -o $TMP/fip.hdr.out

    cat $TMP/bl2.bin.img $TMP/fip.hdr.out $TMP/bl30.bin.img $TMP/bl31.bin.img \
        $TMP/bl32.bin.img $TMP/bl33.bin.img > $output

    echo
    echo Created unsigned bootloader $output successfully
}

parse_main() {
    local i=0
    local argv=()
    for arg in "$@" ; do
        argv[$i]="$arg"
        i=$((i + 1))
    done

    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        case "$arg" in
            -h|--help)
                usage
                break ;;
            --create-unsigned-bl)
                create_unsigned_bl "${argv[@]:$((i + 1))}"
                break ;;
            --create-root-hash)
                create_root_hash "${argv[@]:$((i + 1))}"
                break ;;
            --create-root-hash-from-pub)
                create_root_hash_from_pub "${argv[@]:$((i + 1))}"
                break ;;
            *)
                echo "Unknown first option $1"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
}

cleanup() {
    if [ ! -d "$TMP" ]; then return; fi
    local tmpfiles="bl2.bin.img bl2.img bl2.img-noiv bl2.sha
    bl30.bin.img bl31.bin bl31.bin.img bl32.bin bl32.bin.img
    bl33.bin.img bl.bin bl.hdr bl.hdr.sha blpad.bin
    bl-pl.sha chkdata fip.bin fip.hdr fip.hdr.out
    fip.hdr.sha kernel keydata mod modhex nonce.bin
    rootkey0.bin rootkey0.sha rootkey1.bin rootkey1.sha
    rootkey0.pub rootkey1.pub rootkey2.pub rootkey3.pub
    rootkey2.bin rootkey2.sha rootkey3.bin rootkey3.sha rootkeys.sha
    zeroaesiv zeroaeskey zerorsakey
    bl32.img-info"
    for i in $tmpfiles ; do
        rm -f $TMP/$i
    done
    rmdir $TMP || true
}

trap cleanup EXIT

cleanup
if [ ! -d "$TMP" ]; then mkdir "$TMP" ; fi
parse_main "$@"
