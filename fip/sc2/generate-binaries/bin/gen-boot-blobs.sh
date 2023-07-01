#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../../binary-tool/acpu-imagetool

#
# Settings
#

#BASEDIR_DEVICE_TEMPLATE="${BASEDIR_ROOTRSA_X}/data/template/device"
BASEDIR_CHIPSET_TEMPLATE=$1

BASEDIR_ROOT=${BASEDIR_ROOT:-$BASEDIR_TEMPLATE}

#BASEDIR_RSAKEY_LVLX="${BASEDIR_ROOTRSA_X}/data/key/lvlxrsa"

BASEDIR_OUTPUT_BLOB=$2

input_postfix=.signed
output_postfix=.device.signed

echo "============ ROOTRSA_INDEX ${DEVICE_ROOTRSA_INDEX}"
echo "============       KEY_DIR ${BASEDIR_ROOT}"
echo "============       PROJECT ${PROJECT}"

if [ -z "$PROJECT" ]; then
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_BOOTBLOBS_TEMPLATE_ROOT="${BASEDIR_ROOT}/boot-blobs/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/protkey"
	BASEDIR_FIP_TEMPLATE_ROOT="${BASEDIR_ROOT}/fip/template/rootrsa-${DEVICE_ROOTRSA_INDEX}"
else
	BASEDIR_AESKEY_ROOT="${BASEDIR_ROOT}/root/aes/${PROJECT}/rootkey"
	BASEDIR_RSAKEY_ROOT="${BASEDIR_ROOT}/root/rsa/${PROJECT}"
	BASEDIR_BOOTBLOBS_RSAKEY_ROOT="${BASEDIR_ROOT}/boot-blobs/rsa/${PROJECT}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_BOOTBLOBS_TEMPLATE_ROOT="${BASEDIR_ROOT}/boot-blobs/template/${PROJECT}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_RSAKEY_ROOT="${BASEDIR_ROOT}/fip/rsa/${PROJECT}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
	BASEDIR_FIP_AESKEY_ROOT="${BASEDIR_ROOT}/fip/aes/${PROJECT}/protkey"
	BASEDIR_FIP_TEMPLATE_ROOT="${BASEDIR_ROOT}/fip/template/${PROJECT}/rootrsa-${DEVICE_ROOTRSA_INDEX}"
fi

BASEDIR_DEVICE_TEMPLATE="${BASEDIR_BOOTBLOBS_TEMPLATE_ROOT}"

#
# Arguments
#

#
# Arguments
#

BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-template-bb1st=${BASEDIR_DEVICE_TEMPLATE}/bb1st.bin"

### Input: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-dvinit-params=${BASEDIR_CHIPSET_TEMPLATE}/dvinit-params.bin"
BB1ST_ARGS="${BB1ST_ARGS} --infile-blob-bl2e=${BASEDIR_CHIPSET_TEMPLATE}/blob-bl2e${DEVICE_STORAGE_SUFFIX}${DEVICE_VARIANT_SUFFIX}.bin${input_postfix}"
BB1ST_ARGS="${BB1ST_ARGS} --infile-blob-bl2x=${BASEDIR_CHIPSET_TEMPLATE}/blob-bl2x.bin${input_postfix}"
BB1ST_ARGS="${BB1ST_ARGS} --infile-blob-bb1st-ref=${BASEDIR_CHIPSET_TEMPLATE}/bb1st${DEVICE_STORAGE_SUFFIX}${DEVICE_VARIANT_SUFFIX}.bin${input_postfix}"

### Input: Device Level-1/2 Private RSA keys 
BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl1=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-device-lvl2=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-priv.pem"

### Input: Device Level-2 Public RSA key
BB1ST_ARGS="${BB1ST_ARGS} --infile-pubkey-device-lvl2cert=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-2-rsa-pub.pem"

### Features, flags and switches ###
BB1ST_ARGS="${BB1ST_ARGS} --switch-keep-device-lvl2-pubrsa=1"

# arb info
BB1ST_ARGS="${BB1ST_ARGS} --val-device-scs-segid=${DEVICE_SCS_SEGID}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-vendor-segid=${DEVICE_VENDOR_SEGID}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-scs-vers=${DEVICE_SCS_VERS}"
BB1ST_ARGS="${BB1ST_ARGS} --val-device-tee-vers=${DEVICE_TEE_VERS}"

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-bb1st=${BASEDIR_OUTPUT_BLOB}/bb1st${DEVICE_STORAGE_SUFFIX}${DEVICE_VARIANT_SUFFIX}.bin${output_postfix}"
BB1ST_ARGS="${BB1ST_ARGS} --outfile-blob-bl2e=${BASEDIR_OUTPUT_BLOB}/blob-bl2e${DEVICE_STORAGE_SUFFIX}${DEVICE_VARIANT_SUFFIX}.bin${output_postfix}"
BB1ST_ARGS="${BB1ST_ARGS} --outfile-blob-bl2x=${BASEDIR_OUTPUT_BLOB}/blob-bl2x.bin${output_postfix}"

BB1ST_ARGS="${BB1ST_ARGS} --flag-enable-ddrfw-fip"

echo ${TOOLS_ARGS}

#
# Main
#

set -x

${ACPU_IMAGETOOL} \
        create-boot-blobs \
        ${BB1ST_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
