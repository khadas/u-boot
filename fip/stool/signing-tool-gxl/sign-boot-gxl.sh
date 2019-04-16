#!/bin/bash -e

# Copyright (c) 2018 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x

SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}

# Path to sign-gxl-boot util
TOOL_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}

# Temporary files directory
if [ -z "$TMP" ]; then
    TMP=${SCRIPT_PATH}/tmp
fi

usage() {
    cat << EOF
Usage: $(basename $0) --help

       All-in-one command to create a signed bootloader:

       $(basename $0) --create-signed-bl	\\
                [--key-hash-ver	[1 | 2]]	\\
                --bl2		bl2.bin		\\
                --bl30		bl30.bin	\\
                --bl30-key	bl30key.pem	\\
                [--bl30-aes-key	bl30aeskey	\\
                 --bl30-aes-iv	bl30aesiv]	\\
                --bl31		bl31.img	\\
                --bl31-key	bl31key.pem	\\
                [--bl31-aes-key	bl31aeskey	\\
                 --bl31-aes-iv	bl31aesiv]	\\
                [--bl32		bl32.img	\\
                 --bl32-key	bl32key.pem]	\\
                [--bl32-aes-key	bl32aeskey	\\
                 --bl32-aes-iv	bl32aesiv]	\\
                --bl33		bl33.bin	\\
                --bl33-key	bl33key.pem	\\
                [--bl33-aes-key	bl33aeskey	\\
                 --bl33-aes-iv	bl33aesiv]	\\
                [--kernel-key	kernelkey.pem	\\
                 --kernel-aes-key	kernelaeskey	\\
                 --kernel-aes-iv	kernelaesiv]	\\
                [--fip-key     fipkey.pem]     \\
                [--bl2-arb-cvn <version>]      \\
                [--fip-arb-cvn <version>]      \\
                [--bl30-arb-cvn        <version>]      \\
                [--bl31-arb-cvn        <version>]      \\
                [--bl32-arb-cvn        <version>]      \\
                [--bl33-arb-cvn        <version>]      \\
                [--e            none]          \\
                [--root-key-idx <index>]       \\
                [--marketid     <marketid>]    \\
                -o bl.bin

       Environment Variables:

       TMP:          path to a temporary directory. Defaults to <this script's path>/tmp
EOF
    exit 1
}

check_file() {
    if [ ! -f "$2" ]; then echo Error: Unable to open $1: \""$2"\"; exit 1 ; fi
}

# Hash root/bl2 RSA keys'.  sha256(n[keylen] + e)
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

    openssl dgst -sha256 -out $output -binary $TMP/keydata
}

# Sign bl2
sign_bl2() {
    local input=""
    local rootkey=""
    local rootkeyidx=""
    local rootkey0=""
    local rootkey1=""
    local rootkey2=""
    local rootkey3=""
    local bl2key0=""
    local bl2key1=""
    local bl2key2=""
    local bl2key3=""
    local bl2key=""
    local aesiv=""
    local output=""
    local arb_cvn=""
    local marketid="0"
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
            -i)
                input="${argv[$i]}" ;;
            --root-key)
                rootkey="${argv[$i]}" ;;
            --root-key-idx)
                rootkeyidx="${argv[$i]}" ;;
            --root-key-0)
                rootkey0="${argv[$i]}" ;;
            --root-key-1)
                rootkey1="${argv[$i]}" ;;
            --root-key-2)
                rootkey2="${argv[$i]}" ;;
            --root-key-3)
                rootkey3="${argv[$i]}" ;;
            --bl2-key-0)
                bl2key0="${argv[$i]}" ;;
            --bl2-key-1)
                bl2key1="${argv[$i]}" ;;
            --bl2-key-2)
                bl2key2="${argv[$i]}" ;;
            --bl2-key-3)
                bl2key3="${argv[$i]}" ;;
            --bl2-key)
                bl2key="${argv[$i]}" ;;
            --iv)
                aesiv="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            --arb-cvn)
                arb_cvn="${argv[$i]}" ;;
            --marketid)
                marketid="${argv[$i]}" ;;
            --sig-ver)
                sigver="${argv[$i]}" ;;
            --key-hash-ver)
                keyhashver="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    check_file bl2  "$input"
    check_file rootkey "$rootkey"
    check_file rootkey0 "$rootkey0"
    check_file rootkey1 "$rootkey1"
    check_file rootkey2 "$rootkey2"
    check_file rootkey3 "$rootkey3"
    check_file bl2key0 "$bl2key0"
    check_file bl2key1 "$bl2key1"
    check_file bl2key2 "$bl2key2"
    check_file bl2key3 "$bl2key3"
    check_file bl2key "$bl2key"
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi
    if [ ! -z "$aesiv"  ]; then
        check_file "AES IV" "$aesiv"
    fi
    if [ -z "$arb_cvn" ]; then
        arb_cvn="0"
    else
        if [ $(( $arb_cvn )) -lt 0 ] || [ $(( $arb_cvn )) -gt 255 ]; then
            echo Error: invalid bl2 arb cvn \"$arb_cvn\"
            exit 1
        fi
    fi
    if [ -z "$sigver" ]; then
        sigver=1
    fi
    if [ -z "$keyhashver" ]; then
        keyhashver=1
    fi

    local keylenbytes=$(get_pem_key_len $rootkey)
    local keylen=$(( $keylenbytes * 8 ))

    # Convert to public PEM using openssl in case of password protected keys
    pem_to_pub $rootkey  $TMP/rootkey.pub
    pem_to_pub $rootkey0 $TMP/rootkey0.pub
    pem_to_pub $rootkey1 $TMP/rootkey1.pub
    pem_to_pub $rootkey2 $TMP/rootkey2.pub
    pem_to_pub $rootkey3 $TMP/rootkey3.pub
    pem_to_pub $bl2key0 $TMP/bl2key0.pub
    pem_to_pub $bl2key1 $TMP/bl2key1.pub
    pem_to_pub $bl2key2 $TMP/bl2key2.pub
    pem_to_pub $bl2key3 $TMP/bl2key3.pub
    pem_to_pub $bl2key  $TMP/bl2key.pub

    # Convert PEM key to rsa_public_key_t (precomputed RSA public key)
    pem_to_bin $TMP/rootkey.pub $TMP/rootkey.bin
    pem_to_bin $TMP/rootkey0.pub $TMP/rootkey0.bin
    pem_to_bin $TMP/rootkey1.pub $TMP/rootkey1.bin
    pem_to_bin $TMP/rootkey2.pub $TMP/rootkey2.bin
    pem_to_bin $TMP/rootkey3.pub $TMP/rootkey3.bin
    pem_to_bin $TMP/bl2key0.pub $TMP/bl2key0.bin
    pem_to_bin $TMP/bl2key1.pub $TMP/bl2key1.bin
    pem_to_bin $TMP/bl2key2.pub $TMP/bl2key2.bin
    pem_to_bin $TMP/bl2key3.pub $TMP/bl2key3.bin
    pem_to_bin $TMP/bl2key.pub $TMP/bl2key.bin

    # hash of keys
    hash_rsa_bin $keyhashver $TMP/rootkey.bin $keylen $TMP/rootkey.sha
    hash_rsa_bin $keyhashver $TMP/rootkey0.bin $keylen $TMP/rootkey0.sha
    hash_rsa_bin $keyhashver $TMP/rootkey1.bin $keylen $TMP/rootkey1.sha
    hash_rsa_bin $keyhashver $TMP/rootkey2.bin $keylen $TMP/rootkey2.sha
    hash_rsa_bin $keyhashver $TMP/rootkey3.bin $keylen $TMP/rootkey3.sha
    hash_rsa_bin $keyhashver $TMP/bl2key0.bin $keylen $TMP/bl2key0.sha
    hash_rsa_bin $keyhashver $TMP/bl2key1.bin $keylen $TMP/bl2key1.sha
    hash_rsa_bin $keyhashver $TMP/bl2key2.bin $keylen $TMP/bl2key2.sha
    hash_rsa_bin $keyhashver $TMP/bl2key3.bin $keylen $TMP/bl2key3.sha
    hash_rsa_bin $keyhashver $TMP/bl2key.bin $keylen $TMP/bl2key.sha

    # Create bl2 header
    ${TOOL_PATH}/sign-boot-gxl --create-bl2-header \
            -r $keylen \
            --root-key $TMP/rootkey.bin \
            --root-key-idx $rootkeyidx \
            --root-key-sha-0 $TMP/rootkey0.sha \
            --root-key-sha-1 $TMP/rootkey1.sha \
            --root-key-sha-2 $TMP/rootkey2.sha \
            --root-key-sha-3 $TMP/rootkey3.sha \
            --bl2-key-sha-0 $TMP/bl2key0.sha \
            --bl2-key-sha-1 $TMP/bl2key1.sha \
            --bl2-key-sha-2 $TMP/bl2key2.sha \
            --bl2-key-sha-3 $TMP/bl2key3.sha \
            --bl2-key-idx 0 \
            --bl2-key $TMP/bl2key.bin \
            --marketid $marketid \
            -o $TMP/bl2.hdr

    # Add bl2
    cat $TMP/bl2.hdr $input > $TMP/bl2.img

    cat $TMP/rootkey0.sha $TMP/rootkey1.sha $TMP/rootkey2.sha $TMP/rootkey3.sha > $TMP/r-key.4
    openssl dgst -sha256 -binary $TMP/r-key.4 > $TMP/r-key.e
    rm -f $TMP/r-key.4

    # Truncate to correct length
    # TODO should check that end of output is all zeroes before truncating
    truncate -s 49152 $TMP/bl2.img

    # Add arb cvn
    if [ ! -z "$arb_cvn" ]; then
        printf %02x $arb_cvn | xxd -r -p > $TMP/bl2cvn
        local filesize=$(wc -c < ${TMP}/bl2cvn)
        if [ $filesize -ne 1 ]; then echo Error: cvn error ; exit 1; fi
        # to byteoffset 16+12 - bl2VerMajor
        dd if=$TMP/bl2cvn of=$TMP/bl2.img bs=1 seek=28 count=1 \
            conv=notrunc >& /dev/null
        # to 2nd aml block header, 0x8d8 + 12
        dd if=$TMP/bl2cvn of=$TMP/bl2.img bs=1 seek=2276 count=1 \
            conv=notrunc >& /dev/null
    fi

    # Sign first part with root key
    # aml block header - first 64:
    dd if=$TMP/bl2.img of=$TMP/sigdata bs=1 skip=16 count=64 >& /dev/null
    local chkstart=$(( 16 + 64 + keylenbytes ))
    local chksize=$(( 2248 - 64 - keylenbytes ))
    # - from nChkStart to end of bl2_key_array
    dd if=$TMP/bl2.img of=$TMP/sigdata bs=1 skip=$chkstart count=$chksize \
        oflag=append conv=notrunc >& /dev/null
    openssl dgst -sha256 -sign $rootkey -out $TMP/sigdata.sig $TMP/sigdata

    # Replace signature placeholder
    dd if=$TMP/sigdata.sig of=$TMP/bl2.img bs=1 seek=80 count=$keylenbytes \
        conv=notrunc >& /dev/null

    # Sign second part with bl2 key
    dd if=$TMP/bl2.img of=$TMP/sigdata bs=1 skip=2264 count=64 >& /dev/null
    chkstart=$(( 2264 + 64 + keylenbytes ))
    chksize=$(( (44*1024) + 1832 - 64 - keylenbytes ))
    # - from nChkStart to end of bl2
    dd if=$TMP/bl2.img of=$TMP/sigdata bs=1 skip=$chkstart count=$chksize \
        oflag=append conv=notrunc >& /dev/null
    openssl dgst -sha256 -sign $bl2key -out $TMP/sigdata.sig $TMP/sigdata

    # Replace signature placeholder
    dd if=$TMP/sigdata.sig of=$TMP/bl2.img bs=1 seek=2328 count=$keylenbytes \
        conv=notrunc >& /dev/null

    # Replace nonce
    if [ ! -z "$aesiv" ]; then
        dd if=$aesiv of=$TMP/bl2.img bs=16 count=1 \
            conv=notrunc >& /dev/null
    else
        dd if=/dev/urandom of=$TMP/bl2.img bs=16 count=1 \
            conv=notrunc >& /dev/null
    fi

    cp $TMP/bl2.img $output
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
    if [ "$bits" == "" ]; then
       echo "Unexpected key size  $bits"
       exit 1
    fi
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

# $1: input
# $2: output
# $3: aes key file
# $4: aes iv file
internal_encrypt() {
    local input=$1
    local output=$2
    local keyfile=$3
    local ivfile=$4
    if [ ! -f "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]; then
        echo "Argument error"
        exit 1
    fi
    local key=$(xxd -p -c64 $keyfile)
    local iv=$(xxd -p -c64 $ivfile)
    local imagesize=$(wc -c < ${input})
    local rem=$(( $imagesize % 16 ))
    if [ $rem -ne 0 ]; then
        echo "Input $input not 16 byte aligned?"
        exit 1
    fi
    openssl enc -aes-256-cbc -K $key -iv $iv -e -in $input -out $output -nopad
}

# sign bl3x
sign_bl3x() {
    local input=""
    local key=""
    local output=""
    local arb_cvn=""
    local aeskey=""
    local aesiv=""
    local argv=("$@")
    local encrypt=false
    local stage=""
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -i)
                input="${argv[$i]}" ;;
            -k)
                key="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            -a)
                aeskey="${argv[$i]}" ;;
            --iv)
                aesiv="${argv[$i]}" ;;
            --stage)
                stage="${argv[$i]}" ;;
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
    if [ ! -z "$key" ]; then
        check_file "RSA key" "$key"
    else
        echo Error missing key
        exit 1
    fi
    check_file "RSA key" "$key"
    if [ ! -z "$aeskey" ]; then
        if [ -z "$aesiv"  ]; then echo Error: missing AES IV; exit 1; fi
        check_file "AES key" "$aeskey"
        check_file "AES IV" "$aesiv"
        encrypt=true
    fi
    if [ "$stage" != "bl30" ] &&
            [ "$stage" != "bl31" ] &&
            [ "$stage" != "bl32" ] &&
            [ "$stage" != "bl33" ]; then
        echo Error invalid bl3x stage \"$stage\"
        exit 1
    fi

    # Pad so that hdr + payload is 512 byte aligned
    # because of bl2 driver limitation
    local imagesize=$(wc -c < ${input})
    local rem=$(( (656 + $imagesize) % 512 ))
    if [ $rem -ne 0 ]; then
        #echo "Input $input not 512 byte aligned?"
        local topad=$(( 512 - $rem ))
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
            --stage $stage \
            -o $TMP/bl.hdr

    # Pad header to push signature to end
    # pad to (sizeof(sbImageHeader_t) - signature[keylen] - nonce[16])
    local keylen=$(get_pem_key_len $key)
    pad_file $TMP/bl.hdr $(( 656 - $keylen - 16 ))

    # Sign header
    openssl dgst -sha256 -sign $key -out $TMP/bl.hdr.sig $TMP/bl.hdr

    # Create nonce
    dd if=/dev/urandom of=$TMP/nonce.bin bs=16 count=1 >& /dev/null

    # Combine nonce + hdr + sig + payload
    cat $TMP/nonce.bin $TMP/bl.hdr $TMP/bl.hdr.sig $input > $TMP/bl.bin

    # For debugging, backup signed, unecrypted bl
    if [[ $output =~ bl30.bin.sig$ ]]; then
        cp $TMP/bl.bin $TMP/bl30.bin.img
    elif [[ $output =~ bl31.bin.sig$ ]]; then
        cp $TMP/bl.bin $TMP/bl31.bin.img
    elif [[ $output =~ bl32.bin.sig$ ]]; then
        cp $TMP/bl.bin $TMP/bl32.bin.img
    elif [[ $output =~ bl33.bin.sig$ ]]; then
        cp $TMP/bl.bin $TMP/bl33.bin.img
    fi

    if $encrypt; then
        internal_encrypt $TMP/bl.bin $output $aeskey $aesiv
        #external_encrypt $TMP/bl.bin $output
    else
        cp $TMP/bl.bin $output
    fi
}

append_uint32_le() {
    local input=$1
    local output=$2
    local v=
    local vrev=
    v=$(printf %08x $input)
    # 00010001
    vrev=${v:6:2}${v:4:2}${v:2:2}${v:0:2}

    echo $vrev | xxd -r -p >> $output
}

# function: add control block for upgrade check/defnedkey
# $1: input/output
# $2: rsakey
# $3: aeskey
# $4: rsakeysha
# typedef struct{
#		unsigned char szPad0[16]; //fixed to 0
#		unsigned char szIMGCHK[32];
#		unsigned char szRTKCHK[32];
#		unsigned char szAESCHK[32];
#		unsigned char szPad1[16];
#
#		unsigned char szPad2[4];  //fixed to 0
#		unsigned char szPad3[12]; //random is perfect for RSA
#		unsigned int  nARBMagic; //magic for ARB magic, ARBU, ARBI...
#		unsigned int  nBL2CVN;
#		unsigned int  nFIPCVN;
#		unsigned int  nBL30CVN;
#		unsigned int  nBL31CVN;
#		unsigned int  nBL32CVN;
#		unsigned int  nBL33CVN;
#		unsigned int  nIMGCVN;
#		unsigned char szPad31[80];
#
#		unsigned char szPad4[4];  //fixed to 0
#		unsigned char szPad5[124];//random is perfect for RSA
#
#		unsigned char szPad6[4];  //fixed to 0
#		unsigned char szPad7[124];//random is perfect for RSA
#	}st_upg_chk_blk;//128x4=512Bytes
add_upgrade_check() {
	local arb_magic="0000"	#arb_magic: "ARBU", "ARBI"
	local bl2_cvn="0"
	local fip_cvn="0"
	local bl30_cvn="0"
	local bl31_cvn="0"
	local bl32_cvn="0"
	local bl33_cvn="0"
	local img_cvn="0"
	local argv=("$@")
	local i=0

    if [ $# -lt 4 ]; then
        echo Invalid args for $FUNCNAME
        echo "args: input/output rsakey aeskey"
        exit 1
    fi

	# Parse args
	i=0
	while [ $i -lt $# ]; do
		arg="${argv[$i]}"
		i=$((i + 1))
		case "$arg" in
			--arb-magic)
				arb_magic="${argv[$i]}" ;;
			--bl2-cvn)
				bl2_cvn="${argv[$i]}" ;;
			--fip-cvn)
				fip_cvn="${argv[$i]}" ;;
			--bl30-cvn)
				bl30_cvn="${argv[$i]}" ;;
			--bl31-cvn)
				bl31_cvn="${argv[$i]}" ;;
			--bl32-cvn)
				bl32_cvn="${argv[$i]}" ;;
			--bl33-cvn)
				bl33_cvn="${argv[$i]}" ;;
			--img-cvn)
				img_cvn="${argv[$i]}" ;;
		esac
		i=$((i + 1))
	done

	local sig="$TMP/sig"
		dd if=/dev/zero of=${sig} bs=1 count=12 &> /dev/null         #szPad0[12]
		echo -n 'SCPT' >> ${sig}                                     #szPad0[4]
		openssl dgst -sha256 -binary -out $TMP/$(basename $1).sha $1
		cat $TMP/$(basename $1).sha >> ${sig}                        #szIMGCHK[32]
		rm -f $TMP/$(basename $1).sha
		if [ -e $4 ]; then
		  cat $4 >> ${sig}                                           #szRTKCHK[32]
		else
		  echo "Warning... Invalid RSA key SHA -- $4"
		  dd if=/dev/zero of=${sig} bs=1 count=32 oflag=append conv=notrunc &> /dev/null  #szRTKCHK[32]
		fi

		if [ -e $3 ]; then
		  cat "$3" >> ${sig}                                          #szAESCHK[32]
		else
		  echo "Warning... Invalid AES key -- $3"
		  dd if=/dev/zero of=${sig} bs=1 count=32 oflag=append conv=notrunc &> /dev/null  #szAESCHK[32]
		fi

		dd if=/dev/zero of=${sig} bs=1 count=20 oflag=append conv=notrunc &> /dev/null		 #szPad1[16],szPad2[4]
		dd if=/dev/urandom of=${sig} bs=1 count=12 oflag=append conv=notrunc &> /dev/null	 #szPad3[12]

		# Add arb cvn
		echo $arb_magic > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nARBMagic
		printf "%02x %02x %02x %02x" $bl2_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nBL2CVN
		printf "%02x %02x %02x %02x" $fip_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nFIPCVN
		printf "%02x %02x %02x %02x" $bl30_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nBL30CVN
		printf "%02x %02x %02x %02x" $bl31_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nBL31CVN
		printf "%02x %02x %02x %02x" $bl32_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nBL32CVN
		printf "%02x %02x %02x %02x" $bl33_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nBL33CVN
		printf "%02x %02x %02x %02x" $img_cvn 0 0 0 | xxd -r -ps > $TMP/upg_chk_cvn
		dd if=$TMP/upg_chk_cvn of=${sig} bs=1 count=4 oflag=append conv=notrunc &> /dev/null	#nIMGCVN

		dd if=/dev/zero of=${sig} bs=1 count=84 oflag=append conv=notrunc &> /dev/null     #szPad31[80],szPad4[4]
		dd if=/dev/urandom of=${sig} bs=1 count=12 oflag=append conv=notrunc &> /dev/null  #szPad5[12]
		dd if=/dev/zero of=${sig} bs=1 count=116 oflag=append conv=notrunc &> /dev/null    #szPad5[112],szPad6[4]
		dd if=/dev/urandom of=${sig} bs=1 count=12 oflag=append conv=notrunc &> /dev/null  #szPad7[12]
		dd if=/dev/zero of=${sig} bs=1 count=112 oflag=append conv=notrunc &> /dev/null    #szPad7[112]

		modulus=`openssl rsa -in $2  -modulus -noout`
		declare -i ikeylen=${#modulus}
	  ikeylen=$[ (ikeylen -8) / 2]
		declare -i iIndex
		declare -i iTotal
		iIndex=$[512%ikeylen]
		if [ $iIndex -ne 0 ]; then
			echo "invalid RSA key len $2\n"
			exit 1
		fi

		iTotal=$[512/ikeylen]
		#dd if=/dev/zero of=${sig}.dec bs=1 count=0 &> /dev/null
		while [ $iIndex -lt $iTotal ]; do
			local step=${sig}.$iIndex
			dd if=${sig} of=${step} bs=1 skip=$[ikeylen*iIndex] count=${ikeylen} &> /dev/null
			openssl rsautl -decrypt -raw -in ${step}     -inkey $2 -out ${step}.enc &> /dev/null
			cat ${step}.enc >> $1
			#openssl rsautl -encrypt -raw -in ${step}.enc -inkey $2 -out ${step}.enc.dec &> /dev/null
			#cat ${step}.enc.dec >> ${sig}.dec
			iIndex+=1
    done

    rm -f ${sig}*
}

# Convert RSA private PEM key to public key
# $1: input RSA private .PEM
# $2: output public key file
pem_to_pub() {
    local input=$1
    local output=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi

    openssl 2>/dev/null rsa -in $input -pubout -RSAPublicKey_out -out $output

    if [ $? -ne 0 ]; then
        echo "Error converting key"
        exit 1
    fi
}

# Convert RSA PEM key to precomputed binary key file
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

# Check input is android 9.0 format or not
# 1: input
# returns True or False
# android 9.0 file format: 2KB header + kernel/ramdisk/dtb
# file header as following
# 9.0 will use the unused[2] field like unused[0] = 1
# define ANDR_BOOT_MAGIC "ANDROID!"  //414e44524f494421
# define ANDR_BOOT_MAGIC_SIZE 8
# define ANDR_BOOT_NAME_SIZE 16
# define ANDR_BOOT_ARGS_SIZE 512
# struct andr_img_hdr {
#   char magic[ANDR_BOOT_MAGIC_SIZE];
#   unsigned int kernel_size;	/* size in bytes */
#   unsigned int kernel_addr;	/* physical load addr */
#   unsigned int ramdisk_size;	/* size in bytes */
#   unsigned int ramdisk_addr;	/* physical load addr */
#   unsigned int second_size;	/* size in bytes */
#   unsigned int second_addr;	/* physical load addr */
#   unsigned int tags_addr;		/* physical addr for kernel tags */
#   unsigned int page_size;		/* flash page size we assume */
#   unsigned int unused[2];		/* future expansion: should be 0 */
#   char name[ANDR_BOOT_NAME_SIZE]; /* asciiz product name */
#   char cmdline[ANDR_BOOT_ARGS_SIZE];
#   unsigned int id[8]; /* timestamp / checksum / sha1 / etc */
#   };


is_android9_img() {
    local input=$1
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\""
        exit 1
    fi
    local insize=$(wc -c < $input)
    if [ $insize -le 2048 ]; then
        # less than size of img header
        echo False
        return
    fi

    local inmagic=$(xxd -p -l 8 $input)

    if [ "$inmagic" == "414e44524f494421" ]; then
      inmagic=$(xxd -p -seek 40 -l 4 $input)
      if [ "$inmagic" == "01000000" ]; then
        echo True
      else
        echo False
      fi
    else
      echo False
    fi
}

# Encrypt/sign kernel
#typedef struct {
#	uint32_t magic;
#	uint32_t version;
#	uint32_t flags;
#	uint32_t img_version;
#	uint32_t img_size;
#	uint32_t img_offset;
#	uint8_t img_hash[32];
#	uint8_t reserved[200];
#	uint8_t rsa_sig[256];
#} aml_boot_header_t;
#
#CASSERT(sizeof(aml_boot_header_t) == 512, assert_sizeof_aml_boot_header_t);
sign_kernel() {
    local input=""
    local key=""
    local output=""
    local arb_cvn=""
    local aeskey=""
    local aesiv=""
    local argv=("$@")
    local encrypt=false
    local stage="kernel"
    local i=0
    local keyhashver=1

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -i)
                input="${argv[$i]}" ;;
            -k)
                key="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            -a)
                aeskey="${argv[$i]}" ;;
            --iv)
                aesiv="${argv[$i]}" ;;
            -v)
                arb_cvn="${argv[$i]}" ;;
            -h)
                keyhashver="${argv[$i]}" ;;
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
    if [ ! -z "$key" ]; then
        check_file "RSA key" "$key"
    else
        echo Error missing key
        exit 1
    fi
    check_file "RSA key" "$key"
    if [ ! -z "$aeskey" ]; then
        if [ -z "$aesiv"  ]; then echo Error: missing AES IV; exit 1; fi
        check_file "AES key" "$aeskey"
        check_file "AES IV" "$aesiv"
        encrypt=true
    fi

    local imagesize=$(wc -c < ${input})
    local rem=$(( $imagesize % 512 ))
    if [ $rem -ne 0 ]; then
        #echo "Input $input not 512 byte aligned?"
        local topad=$(( 512 - $rem ))
        imagesize=$(( $imagesize + $topad ))
        cp $input $TMP/kernpad.bin
        pad_file $TMP/kernpad.bin $imagesize
        input=$TMP/kernpad.bin
    fi

    # Hash payload
    openssl dgst -sha256 -binary $input > $TMP/kern-pl.sha

    # Encrypt payload
    if $encrypt; then
        internal_encrypt $input $TMP/kern-pl.bin $aeskey $aesiv
    else
        cp $input $TMP/kern-pl.bin
    fi

    # Create header
    # magic, version, flags, img_version
    echo -n '@AML' > $TMP/kern.hdr
    append_uint32_le 4 $TMP/kern.hdr
    append_uint32_le 0 $TMP/kern.hdr
    append_uint32_le $arb_cvn $TMP/kern.hdr
    # img_size, img_offset, img_hash, reserved
    append_uint32_le $imagesize $TMP/kern.hdr
    append_uint32_le 512 $TMP/kern.hdr
    cat $TMP/kern-pl.sha >> $TMP/kern.hdr
    pad_file $TMP/kern.hdr 256

    # Sign header
    openssl dgst -sha256 -sign $key -out $TMP/kern.hdr.sig $TMP/kern.hdr

    # Combine header + signature
    cat $TMP/kern.hdr.sig >> $TMP/kern.hdr

    # Pad to 512 in case key/sig is smaller than maximum
    pad_file $TMP/kern.hdr 512

    # Combine hdr + payload
    cat $TMP/kern.hdr $TMP/kern-pl.bin > $output

    echo
    echo Created signed kernel $output successfully

    #......
    #android 9.0 special process
    #file format: 2KB android 9.0 file header + AML block header + Image
    if [ "$(is_android9_img ${input})" == "True" ]; then
			local tempfile=${output}.`date +%Y%m%d%H%M%S`
			dd if=${input} of=${tempfile} bs=512 count=4 &> /dev/null
			cat ${output} >> ${tempfile}
			mv -f ${tempfile} ${output}
    fi

    #add_upgrade_check ${output} $bl2key ${bl2aeskey} $TMP/r-key.e
    local keypath=$(dirname $key)
    local temp_folder=$SCRIPT_PATH/`date +%Y%m%d%H%M%S`

    local rootkey0=$keypath/root0.pem
    local rootkey1=$keypath/root1.pem
    local rootkey2=$keypath/root2.pem
    local rootkey3=$keypath/root3.pem
    local bl2key=$keypath/bl2.pem
    local bl2aeskey=$keypath/bl2aeskey

    check_file bl2key   "$bl2key"
    check_file rootkey0 "$rootkey0"
    check_file rootkey1 "$rootkey1"
    check_file rootkey2 "$rootkey2"
    check_file rootkey3 "$rootkey3"

		mkdir -p $temp_folder

		if [ -d $temp_folder ]; then
	    pem_to_pub $rootkey0 $temp_folder/rootkey0.pub
	    pem_to_pub $rootkey1 $temp_folder/rootkey1.pub
	    pem_to_pub $rootkey2 $temp_folder/rootkey2.pub
	    pem_to_pub $rootkey3 $temp_folder/rootkey3.pub

	    # Convert PEM key to rsa_public_key_t (precomputed RSA public key)
	    pem_to_bin $temp_folder/rootkey0.pub $temp_folder/rootkey0.bin
	    pem_to_bin $temp_folder/rootkey1.pub $temp_folder/rootkey1.bin
	    pem_to_bin $temp_folder/rootkey2.pub $temp_folder/rootkey2.bin
	    pem_to_bin $temp_folder/rootkey3.pub $temp_folder/rootkey3.bin

	    # hash of keys
	    declare -i keylen=$(get_pem_key_len $rootkey0)
	    keylen=$[keylen*8]
	    hash_rsa_bin $keyhashver $temp_folder/rootkey0.bin $keylen $temp_folder/rootkey0.sha
	    keylen=$(get_pem_key_len $rootkey1)
	    keylen=$[keylen*8]
	    hash_rsa_bin $keyhashver $temp_folder/rootkey1.bin $keylen $temp_folder/rootkey1.sha
	    keylen=$(get_pem_key_len $rootkey2)
	    keylen=$[keylen*8]
	    hash_rsa_bin $keyhashver $temp_folder/rootkey2.bin $keylen $temp_folder/rootkey2.sha
	    keylen=$(get_pem_key_len $rootkey3)
	    keylen=$[keylen*8]
	    hash_rsa_bin $keyhashver $temp_folder/rootkey3.bin $keylen $temp_folder/rootkey3.sha

	    cat $temp_folder/rootkey0.sha $temp_folder/rootkey1.sha $temp_folder/rootkey2.sha $temp_folder/rootkey3.sha > $temp_folder/r-key.4
	    openssl dgst -sha256 -binary $temp_folder/r-key.4 > $temp_folder/r-key.e
	    add_upgrade_check ${output} $bl2key ${bl2aeskey} $temp_folder/r-key.e \
			--arb-magic "ARBI" \
			--img-cvn $arb_cvn

	    rm -fr $temp_folder
    fi
}

# Create and sign FIP
#./sign-boot-gxl.sh --create-fip \
# ... \
#    -o fip.bin
create_fip() {
    local bl30=""
    local bl31=""
    local bl32=""
    local bl31_info=""
    local bl32_info=""
    local bl33=""
    local bl30key=""
    local bl31key=""
    local bl32key=""
    local bl33key=""
    local bl30aeskey=""
    local bl31aeskey=""
    local bl32aeskey=""
    local bl33aeskey=""
    local bl30aesiv=""
    local bl31aesiv=""
    local bl32aesiv=""
    local bl33aesiv=""
    local kernelkey=""
    local kernelaeskey=""
    local kernelaesiv=""
    local fipkey=""
    local fip_arb_cvn="0"
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
            --bl30-key)
                bl30key="${argv[$i]}" ;;
            --bl31-key)
                bl31key="${argv[$i]}" ;;
            --bl32-key)
                bl32key="${argv[$i]}" ;;
            --bl33-key)
                bl33key="${argv[$i]}" ;;
            --bl30-aes-key)
                bl30aeskey="${argv[$i]}" ;;
            --bl31-aes-key)
                bl31aeskey="${argv[$i]}" ;;
            --bl32-aes-key)
                bl32aeskey="${argv[$i]}" ;;
            --bl33-aes-key)
                bl33aeskey="${argv[$i]}" ;;
            --bl30-aes-iv)
                bl30aesiv="${argv[$i]}" ;;
            --bl31-aes-iv)
                bl31aesiv="${argv[$i]}" ;;
            --bl32-aes-iv)
                bl32aesiv="${argv[$i]}" ;;
            --bl33-aes-iv)
                bl33aesiv="${argv[$i]}" ;;
            --kernel-key)
                kernelkey="${argv[$i]}" ;;
            --kernel-aes-key)
                kernelaeskey="${argv[$i]}" ;;
            --kernel-aes-iv)
                kernelaesiv="${argv[$i]}" ;;
            --fip-key)
                fipkey="${argv[$i]}" ;;
            --fip-arb-cvn)
                fip_arb_cvn="${argv[$i]}" ;;
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
    check_file bl30key "$bl30key"
    check_file bl31key "$bl31key"
    check_file bl32key "$bl32key"
    check_file bl33key "$bl33key"
    check_file bl30aeskey "$bl30aeskey"
    check_file bl31aeskey "$bl31aeskey"
    check_file bl32aeskey "$bl32aeskey"
    check_file bl33aeskey "$bl33aeskey"
    check_file bl30aesiv "$bl30aesiv"
    check_file bl31aesiv "$bl31aesiv"
    check_file bl32aesiv "$bl32aesiv"
    check_file bl33aesiv "$bl33aesiv"
    if [ ! -z "$kernelkey" ]; then
        check_file kernelkey "$kernelkey"
    fi
    if [ ! -z "$kernelaeskey" ]; then
        check_file kernelaeskey "$kernelaeskey"
        check_file kernelaesiv "$kernelaesiv"
    fi
    check_file fipkey "$fipkey"

    if [ -z "$output" ]; then echo Error: Missing output file option -o ; exit 1; fi

    pem_to_pub $bl30key $TMP/bl30key.pub
    pem_to_pub $bl31key $TMP/bl31key.pub
    pem_to_pub $bl32key $TMP/bl32key.pub
    pem_to_pub $bl33key $TMP/bl33key.pub

    # Convert PEM key to rsa_public_key_t (precomputed RSA public key)
    pem_to_bin $TMP/bl30key.pub $TMP/bl30key.bin
    pem_to_bin $TMP/bl31key.pub $TMP/bl31key.bin
    pem_to_bin $TMP/bl32key.pub $TMP/bl32key.bin
    pem_to_bin $TMP/bl33key.pub $TMP/bl33key.bin
    if [ ! -z "$kernelkey" ]; then
        pem_to_pub $kernelkey $TMP/kernelkey.pub
        pem_to_bin $TMP/kernelkey.pub $TMP/kernelkey.bin
    else
        # dummy kernelkey placeholder
        dd if=/dev/zero of=$TMP/kernelkey.bin bs=1036 count=1 &> /dev/null
    fi
    if [ -z "$kernelaeskey" ]; then
        # dummy key/iv placeholders
        dd if=/dev/zero of=$TMP/zeroaeskey bs=1 count=32 >& /dev/null
        dd if=/dev/zero of=$TMP/zeroaesiv bs=1 count=16 >& /dev/null
        kernelaeskey=$TMP/zeroaeskey
        kernelaesiv=$TMP/zeroaesiv
    fi

    # Overencrypt bl3x keys

    # Create header (without nonce) and add keys
    local argv=("$@")
    ${TOOL_PATH}/sign-boot-gxl --create-fip-header \
        --bl30 $bl30 --bl30-key $TMP/bl30key.bin \
        --bl30-aes-key $bl30aeskey --bl30-aes-iv $bl30aesiv \
        --bl31 $bl31 --bl31-key $TMP/bl31key.bin \
        --bl31-aes-key $bl31aeskey --bl31-aes-iv $bl31aesiv \
        --bl32 $bl32 --bl32-key $TMP/bl32key.bin \
        --bl32-aes-key $bl32aeskey --bl32-aes-iv $bl32aesiv \
        --bl33 $bl33 --bl33-key $TMP/bl33key.bin \
        --bl33-aes-key $bl33aeskey --bl33-aes-iv $bl33aesiv \
        --bl31-info $bl31_info --bl32-info $bl32_info \
        --kernel-key $TMP/kernelkey.bin \
        --kernel-aes-key $kernelaeskey --kernel-aes-iv $kernelaesiv \
        --arb-cvn $fip_arb_cvn \
        -o $TMP/fip.hdr

    # Pad header to size
    # pad to (sizeof(fip hdr) - signature[keylen] - nonce[16])
    local keylen=$(get_pem_key_len $fipkey)
    pad_file $TMP/fip.hdr $(( (16*1024) - $keylen - 16 ))

    # Sign header
    openssl dgst -sha256 -sign $fipkey -out $TMP/fip.hdr.sig $TMP/fip.hdr

    # Create nonce
    dd if=/dev/urandom of=$TMP/nonce.bin bs=16 count=1 >& /dev/null

    # Combine nonce + hdr + sig
    cat $TMP/nonce.bin $TMP/fip.hdr $TMP/fip.hdr.sig > $output
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

# input bl2, bl30/31/32/33/kernel {.bin, rsa key, aes key, aes iv}
create_signed_bl() {
    local bl2=""
    local bl30=""
    local bl31=""
    local bl32=""
    local bl33=""
    local rootkey=""
    local rootkeyidx="0"
    local rootkey0=""
    local rootkey1=""
    local rootkey2=""
    local rootkey3=""
    local bl2key0=""
    local bl2key1=""
    local bl2key2=""
    local bl2key3=""
    local bl2key=""
    local bl30key=""
    local bl31key=""
    local bl32key=""
    local bl33key=""
    local bl2aeskey=""
    local bl30aeskey=""
    local bl31aeskey=""
    local bl32aeskey=""
    local bl33aeskey=""
    local bl2aesiv=""
    local bl30aesiv=""
    local bl31aesiv=""
    local bl32aesiv=""
    local bl33aesiv=""
    local kernel=""
    local kernelkey=""
    local kernelaeskey=""
    local kernelaesiv=""
    local fipkey=""
    local encryptbl2=true
    local encryptFIPbl3x=false
    local bl2_arb_cvn="0"
    local fip_arb_cvn="0"
    local bl30_arb_cvn="0"
    local bl31_arb_cvn="0"
    local bl32_arb_cvn="0"
    local bl33_arb_cvn="0"
    local output=""
    local sigver=""
    local keyhashver=""
    local marketid="0"
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
            --root-key)
                rootkey="${argv[$i]}" ;;
            --root-key-idx)
                rootkeyidx="${argv[$i]}" ;;
            --root-key-0)
                rootkey0="${argv[$i]}" ;;
            --root-key-1)
                rootkey1="${argv[$i]}" ;;
            --root-key-2)
                rootkey2="${argv[$i]}" ;;
            --root-key-3)
                rootkey3="${argv[$i]}" ;;
            --bl2-key-0)
                bl2key0="${argv[$i]}" ;;
            --bl2-key-1)
                bl2key1="${argv[$i]}" ;;
            --bl2-key-2)
                bl2key2="${argv[$i]}" ;;
            --bl2-key-3)
                bl2key3="${argv[$i]}" ;;
            --bl2-key)
                bl2key="${argv[$i]}" ;;
            --bl30-key)
                bl30key="${argv[$i]}" ;;
            --bl31-key)
                bl31key="${argv[$i]}" ;;
            --bl32-key)
                bl32key="${argv[$i]}" ;;
            --bl33-key)
                bl33key="${argv[$i]}" ;;
            --bl2-aes-key)
                bl2aeskey="${argv[$i]}" ;;
            --bl30-aes-key)
                bl30aeskey="${argv[$i]}" ;;
            --bl31-aes-key)
                bl31aeskey="${argv[$i]}" ;;
            --bl32-aes-key)
                bl32aeskey="${argv[$i]}" ;;
            --bl33-aes-key)
                bl33aeskey="${argv[$i]}" ;;
            --bl2-aes-iv)
                bl2aesiv="${argv[$i]}" ;;
            --bl30-aes-iv)
                bl30aesiv="${argv[$i]}" ;;
            --bl31-aes-iv)
                bl31aesiv="${argv[$i]}" ;;
            --bl32-aes-iv)
                bl32aesiv="${argv[$i]}" ;;
            --bl33-aes-iv)
                bl33aesiv="${argv[$i]}" ;;
            --kernel-key)
                kernelkey="${argv[$i]}" ;;
            --kernel-aes-key)
                kernelaeskey="${argv[$i]}" ;;
            --kernel-aes-iv)
                kernelaesiv="${argv[$i]}" ;;
            --fip-key)
                fipkey="${argv[$i]}" ;;
            -e)
                if [ "${argv[$i]}" == "FIPbl3x" ]; then
                    encryptFIPbl3x=true
                fi
                if [ "${argv[$i]}" == "none" ]; then
                    encryptFIPbl3x=false
                    encryptbl2=false
                fi
                ;;
            --bl2-arb-cvn)
                bl2_arb_cvn="${argv[$i]}" ;;
            --fip-arb-cvn)
                fip_arb_cvn="${argv[$i]}" ;;
            --bl30-arb-cvn)
                bl30_arb_cvn="${argv[$i]}" ;;
            --bl31-arb-cvn)
                bl31_arb_cvn="${argv[$i]}" ;;
            --bl32-arb-cvn)
                bl32_arb_cvn="${argv[$i]}" ;;
            --bl33-arb-cvn)
                bl33_arb_cvn="${argv[$i]}" ;;
            --sig-ver)
                sigver="${argv[$i]}" ;;
            --key-hash-ver)
                keyhashver="${argv[$i]}" ;;
            --marketid)
                marketid="${argv[$i]}" ;;
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

    check_file rootkey "$rootkey"
    if [ $rootkeyidx -gt 3 ]; then
        echo "Error invalid root-key-idx: $rootkeyidx.";
        exit 1;
    fi
    check_file rootkey0 "$rootkey0"
    check_file rootkey1 "$rootkey1"
    check_file rootkey2 "$rootkey2"
    check_file rootkey3 "$rootkey3"
    check_file bl2key "$bl2key"
    if [ "$bl2key0" == "" ]; then
        bl2key0=$bl2key
    fi
    if [ "$bl2key1" == "" ]; then
        bl2key1=$bl2key
    fi
    if [ "$bl2key2" == "" ]; then
        bl2key2=$bl2key
    fi
    if [ "$bl2key3" == "" ]; then
        bl2key3=$bl2key
    fi
    check_file bl2key0 "$bl2key0"
    check_file bl2key1 "$bl2key1"
    check_file bl2key2 "$bl2key2"
    check_file bl2key3 "$bl2key3"
    check_file bl30key "$bl30key"
    check_file bl31key "$bl31key"
    check_file bl32key "$bl32key"
    check_file bl33key "$bl33key"
    if $encryptbl2; then
        check_file bl2aeskey "$bl2aeskey"
        if [ -n "$bl2aesiv" ]; then
            check_file bl2aesiv "$bl2aesiv"
        else
            dd if=/dev/urandom of=$TMP/bl2aesiv bs=16 count=1 \
                conv=notrunc >& /dev/null
            bl2aesiv=$TMP/bl2aesiv
        fi
    else
        if [ -n "$bl2aeskey" ]; then
            echo "Error bl2 not encrypted but bl2 AES key given"
            exit 1;
        fi
    fi
    if $encryptFIPbl3x; then
        check_file bl30aeskey "$bl30aeskey"
        check_file bl31aeskey "$bl31aeskey"
        check_file bl32aeskey "$bl32aeskey"
        check_file bl33aeskey "$bl33aeskey"
        check_file bl30aesiv "$bl30aesiv"
        check_file bl31aesiv "$bl31aesiv"
        check_file bl32aesiv "$bl32aesiv"
        check_file bl33aesiv "$bl33aesiv"
        check_file kernelaeskey "$kernelaeskey"
        check_file kernelaesiv "$kernelaesiv"
    fi
    if [ ! -z "$kernelkey" ]; then
        check_file kernelkey "$kernelkey"
    fi
    if [ ! -z "$kernelaeskey" ]; then
        check_file kernelaeskey "$kernelaeskey"
        check_file kernelaesiv "$kernelaesiv"
    fi

    check_file fipkey "$fipkey"
    if [ -z "$bl2_arb_cvn" ]; then
        bl2_arb_cvn="0"
    fi
    if [ -z "$sigver" ]; then
        sigver=1
    fi
    if [ -z "$keyhashver" ]; then
        keyhashver=1
    fi

    if [ -z "$output" ]; then echo Error: Missing output file option -o ; exit 1; fi

    sign_bl2 -i $bl2 -o $TMP/bl2.bin.out \
        --root-key $rootkey --root-key-idx $rootkeyidx \
        --root-key-0 $rootkey0 --root-key-1 $rootkey1 \
        --root-key-2 $rootkey2 --root-key-3 $rootkey3 \
        --bl2-key-0 $bl2key0 --bl2-key-1 $bl2key1 \
        --bl2-key-2 $bl2key2 --bl2-key-3 $bl2key3 \
        --bl2-key $bl2key \
        --arb-cvn $bl2_arb_cvn \
        --marketid $marketid \
        $( [ -n "$bl2aesiv" ] && echo -n "--iv $bl2aesiv" ) \
        --sig-ver $sigver --key-hash-ver $keyhashver

    if $encryptbl2; then
        dd if=/dev/zero of=$TMP/zeroiv bs=16 count=1 >& /dev/null
        internal_encrypt $TMP/bl2.bin.out $TMP/bl2.bin.sig $bl2aeskey $TMP/zeroiv
    else
        cp $TMP/bl2.bin.out $TMP/bl2.bin.sig
    fi

    convert_img_to_bin $bl31 $TMP/bl31.bin
    if [ "$bl32" != "" ]; then
        convert_img_to_bin $bl32 $TMP/bl32.bin
    fi

    sign_bl3x -i $bl30         -o $TMP/bl30.bin.sig -k $bl30key --stage bl30 -v $bl30_arb_cvn \
        $( $encryptFIPbl3x && echo -n "-a $bl30aeskey --iv $bl30aesiv" )
    sign_bl3x -i $TMP/bl31.bin -o $TMP/bl31.bin.sig -k $bl31key --stage bl31 -v $bl31_arb_cvn \
        $( $encryptFIPbl3x && echo -n "-a $bl31aeskey --iv $bl31aesiv" )
    if [ "$bl32" != "" ]; then
        sign_bl3x -i $TMP/bl32.bin -o $TMP/bl32.bin.sig -k $bl32key --stage bl32 -v $bl32_arb_cvn \
            $( $encryptFIPbl3x && echo -n "-a $bl32aeskey --iv $bl32aesiv" )
    fi
    sign_bl3x -i $bl33         -o $TMP/bl33.bin.sig -k $bl33key --stage bl33 -v $bl33_arb_cvn \
        $( $encryptFIPbl3x && echo -n "-a $bl33aeskey --iv $bl33aesiv" )

    if ! $encryptFIPbl3x; then
        # create_fip currently requires bl3x aes key/iv to be inserted into fip
        # header. Create dummy data for it
        dd if=/dev/zero of=$TMP/zerokey bs=32 count=1 >& /dev/null
        dd if=/dev/zero of=$TMP/zeroiv bs=16 count=1 >& /dev/null
        bl30aeskey=$TMP/zerokey
        bl31aeskey=$TMP/zerokey
        bl32aeskey=$TMP/zerokey
        bl33aeskey=$TMP/zerokey
        kernelaeskey=$TMP/zerokey
        bl30aesiv=$TMP/zeroiv
        bl31aesiv=$TMP/zeroiv
        bl32aesiv=$TMP/zeroiv
        bl33aesiv=$TMP/zeroiv
        kernelaesiv=$TMP/zeroiv
    fi

    if [ "$bl32" == "" ]; then
        rm -f $TMP/bl32.bin.sig
        touch $TMP/bl32.bin.sig
        # use bl31 as dummy bl32 img info
        cp $bl31 $TMP/bl32.img-info
        bl32info=$TMP/bl32.img-info
    else
        bl32info=$bl32
    fi

    create_fip \
        --bl30     $TMP/bl30.bin.sig \
        --bl30-key     $bl30key \
        --bl30-aes-key $bl30aeskey \
        --bl30-aes-iv  $bl30aesiv \
        --bl31     $TMP/bl31.bin.sig \
        --bl31-info $bl31 \
        --bl31-key     $bl31key \
        --bl31-aes-key $bl31aeskey \
        --bl31-aes-iv  $bl31aesiv \
        --bl32     $TMP/bl32.bin.sig \
        --bl32-info $bl32info \
        --bl32-key     $bl32key \
        --bl32-aes-key $bl32aeskey \
        --bl32-aes-iv  $bl32aesiv \
        --bl33     $TMP/bl33.bin.sig \
        --bl33-key     $bl33key \
        --bl33-aes-key $bl33aeskey \
        --bl33-aes-iv  $bl33aesiv \
        $( [ ! -z "$kernelkey" ] && \
            echo -n "--kernel-key $kernelkey " ) \
        $( [ ! -z "$kernelaeskey" ] && \
            echo -n "--kernel-aes-key $kernelaeskey \
                     --kernel-aes-iv  $kernelaesiv " ) \
        --fip-key $fipkey \
        --fip-arb-cvn $fip_arb_cvn \
        -o $TMP/fip.hdr.out

    if $encryptFIPbl3x; then
        internal_encrypt $TMP/fip.hdr.out $TMP/fip.hdr.sig $bl2aeskey $bl2aesiv
    else
        cp $TMP/fip.hdr.out $TMP/fip.hdr.sig
    fi

    cat $TMP/bl2.bin.sig $TMP/fip.hdr.sig $TMP/bl30.bin.sig \
        $TMP/bl31.bin.sig $TMP/bl32.bin.sig \
        $TMP/bl33.bin.sig > $output

    echo
    echo Created signed bootloader $output successfully

    add_upgrade_check ${output} $bl2key ${bl2aeskey} $TMP/r-key.e \
		--arb-magic "ARBU" \
		--bl2-cvn $bl2_arb_cvn \
		--fip-cvn $fip_arb_cvn \
		--bl30-cvn $bl30_arb_cvn \
		--bl31-cvn $bl31_arb_cvn \
		--bl32-cvn $bl32_arb_cvn \
		--bl33-cvn $bl33_arb_cvn

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
            --create-signed-bl)
                create_signed_bl "${argv[@]:$((i + 1))}"
                break ;;
            --sign-kernel)
                sign_kernel "${argv[@]:$((i + 1))}"
                break ;;
            --add-upgrade-check)
                add_upgrade_check "${argv[@]:$((i + 1))}"
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
    local tmpfiles="bl2.bin.out bl2.bin.sig bl2cvn bl2.hdr bl2.img bl2key0.bin
    bl2key0.sha bl2key1.bin bl2key1.sha bl2key2.bin bl2key2.sha bl2key3.bin
    bl2key3.sha bl2key.bin bl2key.sha bl30.bin.img bl30.bin.sig bl30key.bin
    bl31.bin bl31.bin.img bl31.bin.sig bl31key.bin bl32.bin bl32.bin.img
    bl32.bin.sig bl32key.bin bl33.bin.img bl33.bin.sig bl33key.bin
    bl2aesiv
    bl.bin bl.hdr bl.hdr.sig blpad.bin bl-pl.sha
    bl32.img-info
    fip.hdr fip.hdr.out fip.hdr.sig kernelkey.bin keydata
    nonce.bin rootkey0.bin rootkey0.sha rootkey1.bin rootkey1.sha
    rootkey2.bin rootkey2.sha rootkey3.bin rootkey3.sha rootkey.bin
    rootkey.sha sigdata sigdata.sig zeroiv
    kernel zeroaesiv zeroaeskey zerokey
    bl2key0.pub bl2key1.pub  bl2key2.pub  bl2key3.pub  bl2key.pub  mod  modhex
    bl30key.pub  bl31key.pub  bl32key.pub  bl33key.pub  kernelkey.pub
    rootkey0.pub  rootkey1.pub  rootkey2.pub  rootkey3.pub  rootkey.pub
    kern.hdr  kern.hdr.sig  kern-pl.bin  kern-pl.sha"
    for i in $tmpfiles ; do
        rm -f $TMP/$i
    done
    rm -fr $TMP
}

trap cleanup EXIT

cleanup
if [ ! -d "$TMP" ]; then mkdir "$TMP" ; fi
parse_main "$@"
