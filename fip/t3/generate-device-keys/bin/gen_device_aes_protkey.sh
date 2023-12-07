#!/bin/bash

set -e
#set -x

version=1.2

#
# Utilities
#

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

       Generate Amlogic SC2 chipset Device Vendor Secure Chipset Startup (SCS) root hash and template

       $(basename $0)
		--key-dir <root-key-dir> \\
		--template-dir <template-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		{--output-dir <output-dir>} \\
		{--project <project-name>}
EOF
    exit 1
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
            --project)
                project="${argv[$i]}"
		;;
            --key-dir)
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --template-dir)
                template_dir="${argv[$i]}"
		check_dir "${template_dir}"
		;;
            --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value $rootkey_index 0 3
		;;
            --device-vendor-segid)
                device_vendor_segid="${argv[$i]}"
		;;
            --device-tee-vers)
                device_tee_vers="${argv[$i]}"
		;;
            --device-ree-vers)
                device_ree_vers="${argv[$i]}"
		;;
            --output-dir)
                output_dir="${argv[$i]}"
		;;
            *)
                echo "Unknown option $arg";
		usage
                ;;
        esac
        i=$((i + 1))
    done
}

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../../binary-tool/acpu-imagetool
key_dir=""
template_dir=""
rootkey_index=0
output_dir=""
project=""
device_vendor_segid=0x0
device_tee_vers=0x0
device_ree_vers=0x0

parse_main "$@"

if [ -z "$key_dir" ]; then
	key_dir="."
fi

#
# Settings
#

BASEDIR_ROOT=${key_dir}
BASEDIR_TEMPLATE="${template_dir}"
BASEDIR_OUTPUT="${output_dir}"

DEVICE_ROOTRSA_INDEX=${rootkey_index}

DEVICE_VENDOR_SEGID=${device_vendor_segid}
DEVICE_TEE_VERS=${device_tee_vers}
DEVICE_REE_VERS=${device_ree_vers}

echo DEVICE_VENDOR_SEGID=${DEVICE_VENDOR_SEGID}
echo DEVICE_TEE_VERS=${DEVICE_TEE_VERS}
echo DEVICE_REE_VERS=${DEVICE_REE_VERS}
if [ -z "$project" ]; then
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/protkey"

	BASEDIR_ROOTHASH_OUTPUT="${BASEDIR_RSAKEY_ROOT}/roothash"
	BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/boot-blobs/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/fip/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
else
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/${project}/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/${project}"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/${project}/protkey"

	BASEDIR_ROOTHASH_OUTPUT="${BASEDIR_RSAKEY_ROOT}/roothash"
	BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/boot-blobs/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/fip/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
fi

if [ -z "$output_dir" ]; then
	BASEDIR_OUTPUT_HASH="${BASEDIR_ROOTHASH_OUTPUT}"
	BASEDIR_OUTPUT_BLOB="${BASEDIR_FIP_TEMPLATE_OUTPUT}"
	BASEDIR_OUTPUT_PROTKEY="${BASEDIR_FIP_AESKEY_ROOT}"
	mkdir -p ${BASEDIR_OUTPUT_HASH}
	mkdir -p ${BASEDIR_OUTPUT_BLOB}
	mkdir -p ${BASEDIR_OUTPUT_PROTKEY}
else
	check_dir "${output_dir}"
	BASEDIR_OUTPUT_HASH="${output_dir}"
	BASEDIR_OUTPUT_BLOB="${output_dir}"
	BASEDIR_OUTPUT_PROTKEY="${output_dir}"
fi

#BASEDIR_OUTPUT_BLOB="./output/blob"
#BASEDIR_OUTPUT_HASH="./output/hash"
#BASEDIR_OUTPUT_PROTKEY="./output/protkey"

#
# Check inputs
#

check_dir "${BASEDIR_ROOT}"
check_dir "${BASEDIR_AESKEY_ROOT}"
check_dir "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}"
check_dir "${BASEDIR_FIP_RSAKEY_ROOT}"
check_dir "${BASEDIR_TEMPLATE}"

check_file "${BASEDIR_TEMPLATE}/${project}/device-fip-header.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/bl30-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl30-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl30-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/bl40-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl40-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl40-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/bl31-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl31-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl31-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/bl32-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl32-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl32-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/bl33-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl33-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl33-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/key/krnl-level-3-rsa-pub.pem"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/epk/krnl-lvl3cert-epks.bin"
check_file "${BASEDIR_FIP_RSAKEY_ROOT}/nonce/krnl-dvlvl3cert-nonce.bin"
check_file "${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-1.bin"

#
# Arguments
#

BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template
BB1ST_ARGS="${BB1ST_ARGS} --infile-template-chipset-fip-header=${BASEDIR_TEMPLATE}/${project}/device-fip-header.bin"

### Input: Device Level-2 private RSA Key ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl2=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"

### Input: Device Level-3 Certs  ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-bl30-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl30-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-bl30-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl30-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-bl30-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl30-dvlvl3cert-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-bl40-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl40-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-bl40-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl40-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-bl40-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl40-dvlvl3cert-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-bl31-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl31-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-bl31-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl31-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-bl31-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl31-dvlvl3cert-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-bl32-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl32-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-bl32-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl32-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-bl32-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl32-dvlvl3cert-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-bl33-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl33-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-bl33-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/bl33-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-bl33-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/bl33-dvlvl3cert-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-krnl-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/key/krnl-level-3-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-krnl-device-lvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/epk/krnl-lvl3cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-krnl-dvlvl3cert=${BASEDIR_FIP_RSAKEY_ROOT}/nonce/krnl-dvlvl3cert-nonce.bin"

### Input: Device RootKey (generated from DVGK) ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-device-rootkey-1=${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-1.bin"

### Features, flags and switches ###
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-lvlx-pubrsa-prot"

# arb info
BB1ST_ARGS="${BB1ST_ARGS} --val-device-vendor-segid=${DEVICE_VENDOR_SEGID}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-tee-vers=${DEVICE_TEE_VERS}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-ree-vers=${DEVICE_REE_VERS}"

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-device-fip-header=${BASEDIR_OUTPUT_BLOB}/device-fip-header.bin"

### Output: generated protection keys ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-protkey-bl30=${BASEDIR_OUTPUT_PROTKEY}/genkey-prot-bl30.bin"
BB1ST_ARGS="${BB1ST_ARGS} --outfile-protkey-bl33=${BASEDIR_OUTPUT_PROTKEY}/genkey-prot-bl33.bin"
BB1ST_ARGS="${BB1ST_ARGS} --outfile-protkey-krnl=${BASEDIR_OUTPUT_PROTKEY}/genkey-prot-krnl.bin"

echo ${TOOLS_ARGS}

#
# Main
#

${ACPU_IMAGETOOL} \
        create-device-fip \
        ${BB1ST_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
