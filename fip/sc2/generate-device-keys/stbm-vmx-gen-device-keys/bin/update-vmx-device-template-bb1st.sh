#!/bin/bash

set -e
#set -x

version=1.0

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

       Update VMX Device template bb1st

       $(basename $0)
                --vmx-cert-path <VMX cert path> \\
                {--rootkey-index [0 | 1 | 2 | 3]} \\
                --key-dir <key-dir-prefix> \\
                --project <project-name> \\
                --output-dir <output-dir>
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
            --vmx-cert-path)
                vmx_cert_path="${argv[$i]}"
                check_dir "${vmx_cert_path}"
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
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../../../binary-tool/acpu-imagetool
key_dir=""
vmx_cert_path=""
rootkey_index=0
output_dir=""
project=""
device_scs_segid=0x0
device_vendor_segid=0x0
device_scs_vers=0x0
device_tee_vers=0x0

parse_main "$@"

if [ -z "$key_dir" ]; then
        usage
fi

if [ -z "$project" ]; then
        usage
fi

if [ -z "$vmx_cert_path" ]; then
        usage
fi

if [ -z "$output_dir" ]; then
        usage
fi

#
# Settings
#
BASEDIR_ROOT=${key_dir}
BASEDIR_TEMPLATE="${vmx_cert_path}"

DEVICE_ROOTRSA_INDEX=${rootkey_index}

DEVICE_SCS_SEGID=${device_scs_segid}
DEVICE_VENDOR_SEGID=${device_vendor_segid}
DEVICE_SCS_VERS=${device_scs_vers}
DEVICE_TEE_VERS=${device_tee_vers}

echo DEVICE_SCS_SEGID=${DEVICE_SCS_SEGID}
echo DEVICE_VENDOR_SEGID=${DEVICE_VENDOR_SEGID}
echo DEVICE_SCS_VERS=${DEVICE_SCS_VERS}
echo DEVICE_TEE_VERS=${DEVICE_TEE_VERS}

BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/${project}/rootkey"
BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/${project}"
BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"

BASEDIR_ROOTHASH_OUTPUT="${BASEDIR_RSAKEY_ROOT}/roothash"
BASEDIR_BOOTBLOBS_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/boot-blobs/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
#BASEDIR_FIP_TEMPLATE_OUTPUT="${BASEDIR_ROOT}/fip/template/${project}/rootrsa-${DEVICE_ROOTRSA_INDEX}"

check_dir "${output_dir}"
BASEDIR_OUTPUT_HASH="${output_dir}"
BASEDIR_OUTPUT_BLOB="${output_dir}"

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

check_file "${BASEDIR_TEMPLATE}/${project}/bb1st.bin.device.cert.segid.${DEVICE_SCS_SEGID}"
check_file "${BASEDIR_RSAKEY_ROOT}/nonce/rootrsa-${DEVICE_ROOTRSA_INDEX}-nonce.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl1rsa-nonce.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"
check_file "${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl2rsa-nonce.bin"
check_file "${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-2.bin"
check_file "${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-3.bin"

#
# Arguments
#

BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-template-bb1st=${BASEDIR_TEMPLATE}/${project}/bb1st.bin.device.cert.segid.${DEVICE_SCS_SEGID}"

### RootCert EK and NONCE
# EK is common for all root RSA
# NONCE is per root RSA
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-rootrsa=${BASEDIR_RSAKEY_ROOT}/nonce/rootrsa-${DEVICE_ROOTRSA_INDEX}-nonce.bin"

### Input: Device Level-1 Cert ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-lvl1rsa=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl1rsa-nonce.bin"

BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl1=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem"

### Input: Device Level-2 Cert ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-epks-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/epk/lvl2cert-epks.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-nonce-device-lvl2rsa=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/nonce/device-lvl2rsa-nonce.bin"

### Input: Device RootKey (generated from DVGK) ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-device-rootkey-2=${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-2.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-device-rootkey-3=${BASEDIR_AESKEY_ROOT}/aes256-device-rootkey-bootstage-3.bin"

### Features, flags and switches ###
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-root-pubrsa-prot"
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-lvl1-pubrsa-prot"
BB1ST_ARGS="${BB1ST_ARGS} --feature-enable-device-lvlx-pubrsa-prot"

BB1ST_ARGS="${BB1ST_ARGS} --feature-device-root-pubrsa-prot-mrk"

BB1ST_ARGS="${BB1ST_ARGS} --switch-device-sign-blob=0"

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-bb1st=${BASEDIR_OUTPUT_BLOB}/bb1st.bin.device"

echo ${TOOLS_ARGS}

#
# Main
#

${ACPU_IMAGETOOL} \
        create-boot-blobs \
        ${BB1ST_ARGS}

# vim: set tabstop=4 expandtab shiftwidth=4:
