#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x

SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}

# Temporary files directory
if [ "$TMP" == "/tmp" ] || [ -z "$TMP" ]; then
    TMP=${SCRIPT_PATH}/tmp
fi

trace ()
{
    echo ">>> $@" > /dev/null
}

usage() {
    cat << EOF
Usage: $(basename $0) --help

       Amlogic Device Vendor Secure Chipset Startup (SCS) Signing

       $(basename $0) --sign-kernel	\\
		--key-dir <key-dir> \\
		--project <project-name> \\
		--input  <input img> \\
		{--rootkey-index [0 | 1 | 2 | 3]} \\
		--output <output img>
EOF
    exit 1
}

check_file() {
    if [ ! -f "$2" ]; then echo Error: Unable to open $1: \""$2"\"; exit 1 ; fi
}

check_dir() {
    if [ ! -d "$1" ]; then echo "Error: directory \""$1"\" does NOT exist"; usage ; fi
}

check_value() {
	local val=$1
	local begin=$2
	local end=$3

	if [ $val -lt $begin ] || [ $val -gt $end ]; then
		echo "Error: Value $val is not in range [$begin, $end]"
		exit 1
	fi
}

# Calculate aligned file size
# $1: file
# $2: alignment requirement in bytes
aligned_size() {
    local file=$1
    local skip=$2
    local alignment=$3
    local alignedsize=0

    local filesize=$(wc -c < ${file})
    #echo "Input $file filesize $filesize"
    if [ $skip -ne 0 ]; then
        filesize=$(( $filesize - $skip ))
    fi
    local rem=$(( $filesize % $alignment ))
    if [ $rem -ne 0 ]; then
        #echo "Input $file not $alignment byte aligned"
        local padsize=$(( $alignment - $rem ))
        alignedsize=$(( $filesize + $padsize ))
    else
        alignedsize=$filesize
    fi
    #echo "Aligned size $alignedsize"
    echo $alignedsize
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

# Check input is android format or not
is_android_img() {
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
      echo True
    else
      echo False
    fi
}

# Check input is android R format or not
# 1: input
# returns True or False
# android R file format: 4KB header + kernel/ramdisk/dtb
# file header as following
#define ANDROID_R_IMG_VER  (3)
#typedef struct {
#	char 	magic[ANDR_BOOT_MAGIC_SIZE]; /*"ANDROID!"*/
#
#	u32 	kernel_size;	/* size in bytes */
#	u32 	ramdisk_size;	/* size in bytes */
#
#   /* Operating system version and security patch level.
#       For version "A.B.C" and patch level "Y-M-D":
#      (7 bits for each of A, B, C; 7 bits for (Y-2000), 4 bits for M)
#     os_version = A[31:25] B[24:18] C[17:11] (Y-2000)[10:4] M[3:0]
#       */
#
#	uint32_t os_version;
#	uint32_t header_size;
#	uint32_t reserved[4];
#
#	uint32_t header_version;   /* Version of the boot image header */
#	char     cmdline[BOOT_ARGS_SIZE + BOOT_EXTRA_ARGS_SIZE];
#	unsigned char szReserved[BOOT_IMG_V3_HDR_SIZE - 1580];       /*align to 4KB header,1580 is size before this*/
#}boot_img_hdr_v3_t, * p_boot_img_hdr_v3_t;
is_androidR_img() {
    local input=$1
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\""
        exit 1
    fi
    local insize=$(wc -c < $input)
    if [ $insize -le 4096 ]; then
        # less than size of img header
        echo False
        return
    fi

    local inmagic=$(xxd -p -l 8 $input)

    if [ "$inmagic" == "414e44524f494421" ]; then
      inversion=$(xxd -p -seek 40 -l 4 $input)
      if [ "$inversion" == "03000000" ]; then
        echo True
      else
        echo False
      fi
    elif [ "$inmagic" == "564e4452424f4f54" ]; then
      echo True
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
#	uint8_t aesblk_sig[512];
#	uint8_t rsa_sig[512];
#} aml_boot_header_t;
#
#CASSERT(sizeof(aml_boot_header_t) == 1280, assert_sizeof_aml_boot_header_t);
sign_kernel() {
    local input=""
    local key_dir=""
    local part=""
    local rootkey_index=0
    local output=""
    local rsakey=""
    local aeskey=""
    local aesiv=$TMP/aesiv.bin
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --key-dir)
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --project)
                part="${argv[$i]}"
		;;
            --input)
                input="${argv[$i]}"
		;;
            --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value "${rootkey_index}" 0 3
		;;
            --output)
                output="${argv[$i]}"
		;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "${key_dir}" ]; then
        usage
    fi

    if [ -z "${part}" ]; then
        echo "Error: project cannot be empty"
        usage
    fi

    if [ -z "${rootkey_index}" ]; then
        rootkey_index=0
    fi

    check_file "input" "${input}"
    if [ -z "${output}" ]; then
        echo Error: Missing output file option --output; exit 1;
    fi

    rsakey=$(readlink -f ${key_dir})/fip/rsa/${part}/rootrsa-${rootkey_index}/key/krnl-level-3-rsa-priv.pem
    check_file "RSA key" "${rsakey}"
    aeskey=$(readlink -f ${key_dir})/fip/aes/${part}/protkey/genkey-prot-krnl.bin
    check_file "AES key" "${aeskey}"
    dd if=/dev/zero of=${aesiv} bs=1 count=16 >& /dev/null
    check_file "AES IV" "${aesiv}"

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
    internal_encrypt $input $TMP/kern-pl.bin $aeskey $aesiv

    # Create header
    # magic, version, flags, img_version
    echo -n '@AML' > $TMP/kern.hdr
    append_uint32_le 1 $TMP/kern.hdr
    append_uint32_le 0 $TMP/kern.hdr
    append_uint32_le 0 $TMP/kern.hdr
    # img_size, img_offset, img_hash, reserved
    append_uint32_le $imagesize $TMP/kern.hdr
    append_uint32_le 768 $TMP/kern.hdr
    cat $TMP/kern-pl.sha >> $TMP/kern.hdr
    pad_file $TMP/kern.hdr 256

    # Sign header
    openssl dgst -sha256 -sign $rsakey -out $TMP/kern.hdr.sig $TMP/kern.hdr

    # Combine header + signature
    cat $TMP/kern.hdr.sig >> $TMP/kern.hdr

    # Pad to 768 in case key/sig is smaller than maximum
    pad_file $TMP/kern.hdr 768

    # Combine hdr + payload
    cat $TMP/kern.hdr $TMP/kern-pl.bin > $output

    #......
    #android boot/vendorboot special process
    if [ "$(is_androidR_img ${input})" == "True" ]; then
            local tempfile=${output}.`date +%Y%m%d%H%M%S`
            dd if=${input} of=${tempfile} bs=512 count=8 &> /dev/null
            cat ${output} >> ${tempfile}
            mv -f ${tempfile} ${output}
    elif [ "$(is_android_img ${input})" == "True" ]; then
            local tempfile=${output}.`date +%Y%m%d%H%M%S`
            dd if=${input} of=${tempfile} bs=512 count=4 &> /dev/null
            dd if=/dev/zero of=${tempfile} bs=512 count=4  oflag=append conv=notrunc &> /dev/null
            cat ${output} >> ${tempfile}
            mv -f ${tempfile} ${output}
    fi

    echo
    echo Created signed kernel $output successfully
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
            --sign-kernel)
                sign_kernel "${argv[@]:$((i + 1))}"
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
    local tmpfiles="kernpad.bin  kern.hdr  kern.hdr.sig  kern-pl.sha kern-pl.bin  aesiv.bin"
    for i in $tmpfiles ; do
        rm -f $TMP/$i
    done
    rm -fr $TMP
}

trap cleanup EXIT

cleanup
if [ ! -d "$TMP" ]; then mkdir "$TMP" ; fi
parse_main "$@"
