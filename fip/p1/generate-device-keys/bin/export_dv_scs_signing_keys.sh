#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.1

trace ()
{
	echo ">>> $@" > /dev/null
	echo ">>> $@"
}

check_file() {
    if [ ! -f "$1" ]; then echo "Error: file \""$1"\" does NOT exist"; usage ; fi
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

usage() {
    cat << EOF
Usage: $(basename $0) --help | --version

       Export Amlogic SC2 Device Vendor Secure Chipset Startup (SCS) key release for image signing

       $(basename $0)
		--key-dir <key-dir> \\
		--out-dir <key-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		{--project <project-name>}
EOF
    exit 1
}

key_dir=""
project=""
rootkey_index=0
output_dir=""

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
        i=$((i + 1))
        case "$arg" in
            -h|--help)
                usage
                break
		;;
            -v|--version)
                echo "Version $version";
		exit 0
		;;
            --key-dir)
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --out-dir)
                output_dir="${argv[$i]}"
		;;
            --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value $rootkey_index 0 3
		;;
            --project)
                project="${argv[$i]}"
		;;
            *)
                echo "Unknown option $arg";
		usage
                ;;
        esac
        i=$((i + 1))
    done
}

parse_main "$@"

trace "       key-dir $key_dir"
trace "       project $project"
trace "       out-dir $output_dir"
trace " rootkey-index $rootkey_index"

if [ -z "$key_dir" ]; then
	usage
fi

if [ -z "$output_dir" ]; then
	usage
fi

BASEDIR_ROOT=$key_dir
BASEDIR_OUT_ROOT=$output_dir
DEVICE_ROOTRSA_INDEX=$rootkey_index

if [ -z "$project" ]; then
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/protkey"
	BASEDIR_BOOTBLOBS_TEMPLATE_ROOT="${BASEDIR_ROOT}/boot-blobs/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_ROOT="${BASEDIR_ROOT}/fip/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"

	BASEDIR_AESKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/root/aes/rootkey"
	BASEDIR_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/root/rsa/"
	BASEDIR_BOOTBLOBS_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/boot-blobs/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/aes/protkey"

	BASEDIR_BOOTBLOBS_TEMPLATE_OUT_ROOT="${BASEDIR_OUT_ROOT}/boot-blobs/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
else
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/${project}/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/${project}"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/${project}/protkey"
	BASEDIR_BOOTBLOBS_TEMPLATE_ROOT="${BASEDIR_ROOT}/boot-blobs/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_ROOT="${BASEDIR_ROOT}/fip/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"

	BASEDIR_AESKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/root/aes/${project}/rootkey"
	BASEDIR_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/root/rsa/${project}"
	BASEDIR_BOOTBLOBS_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/boot-blobs/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/aes/${project}/protkey"
	BASEDIR_BOOTBLOBS_TEMPLATE_OUT_ROOT="${BASEDIR_OUT_ROOT}/boot-blobs/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_OUT_ROOT="${BASEDIR_OUT_ROOT}/fip/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
fi

### Input: Root Cert ###
### Required to generate Device Vendor SCS Cert  ###
### Required to generate Device Vendor root hash ###
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-0-pub.pem "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-1-pub.pem "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-2-pub.pem "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-3-pub.pem "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-3-pub.pem "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/epk/rootcert-epks.bin "
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/nonce/rootrsa-${DEVICE_ROOTRSA_INDEX}-nonce.bin "
EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/roothash/hash-device-rootcert.bin "

### Input: Selected Root RSA index (0 - 3)
### Required to generate Device Vendor Lvl-1 Cert ###
#EXPORT_FILES+="${BASEDIR_RSAKEY_ROOT}/key/rootrsa-${DEVICE_ROOTRSA_INDEX}-priv.pem "

### Input: Device Level-1/2 Cert ###
### Required to generate Device Vendor Lvl-1/2 Cert ###
#EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl1cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl1rsa-nonce.bin "
EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl2rsa-nonce.bin "

### Input: Device Level-1/2 Private Key ###
### Required to generate Device Vendor Lvl-2 and Lvl-3 Cert ###
EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem "

### Input: Device Level-3 Cert ###
### Required to generate Device Vendor Lvl-3 Cert ###
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl30-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl30-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl30-dvlvl3cert-nonce.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl31-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl31-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl31-dvlvl3cert-nonce.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl32-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl32-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl32-dvlvl3cert-nonce.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl33-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl33-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl33-dvlvl3cert-nonce.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl40-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl40-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl40-dvlvl3cert-nonce.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/krnl-level-3-rsa-pub.pem "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/epk/krnl-lvl3cert-epks.bin "
#EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/nonce/krnl-dvlvl3cert-nonce.bin "

### Input: Device Level-3 Private Key ###
### Required to sign Device Vendor bootloader ###
### Required to sign BL30 and BL33
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl30-level-3-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl31-level-3-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl32-level-3-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl33-level-3-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/bl40-level-3-rsa-priv.pem "
EXPORT_FILES+="${BASEDIR_FIP_RSAKEY_ROOT}/key/krnl-level-3-rsa-priv.pem "
### Input: DVGK derived AES-256 root key ###
### Required to generate Device Root cert and Lvl-1/2 cert ###
#EXPORT_FILES+="${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-2.bin "
#EXPORT_FILES+="${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-3.bin "

### Input: DVGK derived AES-256 root key ###
### Required to derive protkey for Device Root Lvl-3 cert and BL30/BL33/Kernel ###
#EXPORT_FILES+="${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-1.bin "

### Input (Required for Device Bootloader Encryption) ###
### Required to protect BL30/BL33 and kernel protection ###
EXPORT_FILES+="${BASEDIR_FIP_AESKEY_ROOT}/genkey-prot-bl30.bin "
EXPORT_FILES+="${BASEDIR_FIP_AESKEY_ROOT}/genkey-prot-bl33.bin "
EXPORT_FILES+="${BASEDIR_FIP_AESKEY_ROOT}/genkey-prot-krnl.bin "

EXPORT_FILES+="${BASEDIR_BOOTBLOBS_TEMPLATE_ROOT}/bb1st.bin "
EXPORT_FILES+="${BASEDIR_FIP_TEMPLATE_ROOT}/device-fip-header.bin "

for f in $EXPORT_FILES
do
	check_file $f
done

#mkdir -p ${BASEDIR_RSAKEY_OUT_ROOT}/key
#mkdir -p ${BASEDIR_RSAKEY_OUT_ROOT}/epk
#mkdir -p ${BASEDIR_RSAKEY_OUT_ROOT}/nonce
mkdir -p ${BASEDIR_RSAKEY_OUT_ROOT}/roothash
#mkdir -p ${BASEDIR_AESKEY_OUT_ROOT}
mkdir -p ${BASEDIR_BOOTBLOBS_RSAKEY_OUT_ROOT}/key
#mkdir -p ${BASEDIR_BOOTBLOBS_RSAKEY_OUT_ROOT}/epk
#mkdir -p ${BASEDIR_BOOTBLOBS_RSAKEY_OUT_ROOT}/nonce
mkdir -p ${BASEDIR_FIP_RSAKEY_OUT_ROOT}/key
#mkdir -p ${BASEDIR_FIP_RSAKEY_OUT_ROOT}/epk
#mkdir -p ${BASEDIR_FIP_RSAKEY_OUT_ROOT}/nonce
mkdir -p ${BASEDIR_FIP_AESKEY_OUT_ROOT}
mkdir -p ${BASEDIR_BOOTBLOBS_TEMPLATE_OUT_ROOT}
mkdir -p ${BASEDIR_FIP_TEMPLATE_OUT_ROOT}

for f in $EXPORT_FILES
do
	out=${f/#$key_dir/$output_dir}
	echo cp $f $out
	cp $f $out
done
