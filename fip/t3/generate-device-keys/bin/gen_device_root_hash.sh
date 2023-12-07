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
		--template-dir <template-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		--key-dir <key-dir-prefix> \\
		{--project <project-name>} \\
		{--output-dir <output-dir>}
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
            --device-scs-segid)
                device_scs_segid="${argv[$i]}"
		;;
            --device-vendor-segid)
                device_vendor_segid="${argv[$i]}"
		;;
            --device-scs-vers)
                device_scs_vers="${argv[$i]}"
		;;
            --device-tee-vers)
                device_tee_vers="${argv[$i]}"
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
device_scs_segid=0x0
device_vendor_segid=0x0
device_scs_vers=0x0
device_tee_vers=0x0

parse_main "$@"

if [ -z "$key_dir" ]; then
	key_dir="."
fi

#
# Settings
#
BASEDIR_ROOT=${key_dir}
BASEDIR_TEMPLATE="${template_dir}"

DEVICE_ROOTRSA_INDEX=${rootkey_index}

DEVICE_SCS_SEGID=${device_scs_segid}
DEVICE_VENDOR_SEGID=${device_vendor_segid}
DEVICE_SCS_VERS=${device_scs_vers}
DEVICE_TEE_VERS=${device_tee_vers}

echo DEVICE_SCS_SEGID=${DEVICE_SCS_SEGID}
echo DEVICE_VENDOR_SEGID=${DEVICE_VENDOR_SEGID}
echo DEVICE_SCS_VERS=${DEVICE_SCS_VERS}
echo DEVICE_TEE_VERS=${DEVICE_TEE_VERS}

if [ -z "$project" ]; then
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"

	BASEDIR_ROOTHASH_OUTPUT="${BASEDIR_RSAKEY_ROOT}/roothash"
	BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/boot-blobs/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	#BASEDIR_FIP_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/fip/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
else
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/${project}/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/${project}"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"

	BASEDIR_ROOTHASH_OUTPUT="${BASEDIR_RSAKEY_ROOT}/roothash"
	BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/boot-blobs/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	#BASEDIR_FIP_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/fip/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
fi

if [ -z "$output_dir" ]; then
	BASEDIR_OUTPUT_HASH="${BASEDIR_ROOTHASH_OUTPUT}"
	BASEDIR_OUTPUT_BLOB="${BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT}"
	mkdir -p ${BASEDIR_OUTPUT_HASH}
	mkdir -p ${BASEDIR_OUTPUT_BLOB}
else
	check_dir "${output_dir}"
	BASEDIR_OUTPUT_HASH="${output_dir}"
	BASEDIR_OUTPUT_BLOB="${output_dir}"
fi

#BASEDIR_OUTPUT_BLOB="./output/blob"
#BASEDIR_OUTPUT_HASH="./output/hash"
#BASEDIR_OUTPUT_PROTKEY="./output/protkey"

#
# Check inputs
#

check_dir "${BASEDIR_ROOT}"
check_dir "${BASEDIR_AESKEY_ROOT}"
check_dir "${BASEDIR_RSAKEY_ROOT}"
check_dir "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}"
check_dir "${BASEDIR_FIP_RSAKEY_ROOT}"
check_dir "${BASEDIR_TEMPLATE}"

check_file "${BASEDIR_TEMPLATE}/${project}/bb1st.bin"
check_file "${BASEDIR_RSAKEY_ROOT}/key/rootrsa-0-pub.pem"
check_file "${BASEDIR_RSAKEY_ROOT}/key/rootrsa-1-pub.pem"
check_file "${BASEDIR_RSAKEY_ROOT}/key/rootrsa-2-pub.pem"
check_file "${BASEDIR_RSAKEY_ROOT}/key/rootrsa-3-pub.pem"
check_file "${BASEDIR_RSAKEY_ROOT}/epk/rootcert-epks.bin"
check_file "${BASEDIR_RSAKEY_ROOT}/nonce/rootrsa-${DEVICE_ROOTRSA_INDEX}-nonce.bin"
check_file "${BASEDIR_RSAKEY_ROOT}/key/rootrsa-${DEVICE_ROOTRSA_INDEX}-priv.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-pub.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl1cert-epks.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl1rsa-nonce.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl2rsa-nonce.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem"
check_file "${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-2.bin"
check_file "${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-3.bin"

#
# Arguments
#

BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-template-bb1st=${BASEDIR_TEMPLATE}/${project}/bb1st.bin"

### Input: Device RootCert ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-rootrsa-0=${BASEDIR_RSAKEY_ROOT}/key/rootrsa-0-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-rootrsa-1=${BASEDIR_RSAKEY_ROOT}/key/rootrsa-1-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-rootrsa-2=${BASEDIR_RSAKEY_ROOT}/key/rootrsa-2-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-rootrsa-3=${BASEDIR_RSAKEY_ROOT}/key/rootrsa-3-pub.pem"

### RootCert EK and NONCE
# EK is common for all root RSA
# NONCE is per root RSA
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-device-rootcert=${BASEDIR_RSAKEY_ROOT}/epk/rootcert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-rootrsa=${BASEDIR_RSAKEY_ROOT}/nonce/rootrsa-${DEVICE_ROOTRSA_INDEX}-nonce.bin"

# Select root RSA to use
BB1ST_ARGS="${BB1ST_ARGS} --device-rootrsa-index=${DEVICE_ROOTRSA_INDEX}"
BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-root=${BASEDIR_RSAKEY_ROOT}/key/rootrsa-${DEVICE_ROOTRSA_INDEX}-priv.pem"

### Input: Device Level-1 Cert ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-lvl1cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-device-lvl1cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl1cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-lvl1rsa=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl1rsa-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl1=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem"

### Input: Device Level-2 Cert ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-lvl2rsa=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl2rsa-nonce.bin"

#BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl2=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem"

### Input: Device RootKey (generated from DVGK) ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-device-rootkey-2=${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-2.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-device-rootkey-3=${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-3.bin"

### Features, flags and switches ###
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-root-pubrsa-prot"
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-lvl1-pubrsa-prot"
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-lvlx-pubrsa-prot"

BB1ST_ARGS="${BB1ST_ARGS} --feature-device-root-pubrsa-prot-mrk"

BB1ST_ARGS="${BB1ST_ARGS} --switch-device-sign-blob=0"

# arb info
BB1ST_ARGS="${BB1ST_ARGS} --val-device-scs-segid=${DEVICE_SCS_SEGID}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-vendor-segid=${DEVICE_VENDOR_SEGID}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-scs-vers=${DEVICE_SCS_VERS}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-tee-vers=${DEVICE_TEE_VERS}"

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-bb1st=${BASEDIR_OUTPUT_BLOB}/bb1st.bin"

### Output: hash of root cert ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-hash-device-rootcert=${BASEDIR_OUTPUT_HASH}/hash-device-rootcert.bin"

echo ${TOOLS_ARGS}

#
# Main
#

${ACPU_IMAGETOOL} \
        create-boot-blobs \
        ${BB1ST_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
