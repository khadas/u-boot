#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../../binary-tool/acpu-imagetool

BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#

#BASEDIR_DEVICE_TEMPLATE="${BASEDIR_ROOTRSA_X}/data/template/device"
BASEDIR_CHIPSET_TEMPLATE=$1

BASEDIR_ROOT=${BASEDIR_ROOT:-$BASEDIR_TEMPLATE}

#BASEDIR_AESKEY_PROT="${BASEDIR_ROOTRSA_X}/data/key/aesroot"
#BASEDIR_RSAKEY_LVLX="${BASEDIR_ROOTRSA_X}/data/key/lvlxrsa"

BASEDIR_PAYLOAD=$2

#BASEDIR_EPK="${BASEDIR_ROOTRSA_X}/data/epk"

BASEDIR_OUTPUT=$3

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

BASEDIR_DEVICE_TEMPLATE="${BASEDIR_FIP_TEMPLATE_ROOT}"

#
# Arguments
#

BLOB_NAME=$1

EXEC_ARGS="${EXEC_ARGS}"

### Input: template ###
EXEC_ARGS="${EXEC_ARGS} --infile-template-device-fip-header=${BASEDIR_DEVICE_TEMPLATE}/device-fip-header.bin"

### Input: payload ###
EXEC_ARGS="${EXEC_ARGS} --infile-bl30-payload=${BASEDIR_PAYLOAD}/bl30-payload.bin"
EXEC_ARGS="${EXEC_ARGS} --infile-bl33-payload=${BASEDIR_PAYLOAD}/bl33-payload.bin"

### Input: Device Level-3 private RSA keys and EPKs ###

# Device Vendor binaries
EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl30-device-lvl3=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl30-level-3-rsa-priv.pem"
EXEC_ARGS="${EXEC_ARGS} --infile-aes256-bl30-payload=${BASEDIR_FIP_AESKEY_ROOT}/genkey-prot-bl30.bin"

EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl33-device-lvl3=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl33-level-3-rsa-priv.pem"
EXEC_ARGS="${EXEC_ARGS} --infile-aes256-bl33-payload=${BASEDIR_FIP_AESKEY_ROOT}/genkey-prot-bl33.bin"

# Chipset Manufacturer binaries
EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl40-device-lvl3=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl40-level-3-rsa-priv.pem"
EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl31-device-lvl3=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl31-level-3-rsa-priv.pem"
EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl32-device-lvl3=${BASEDIR_FIP_RSAKEY_ROOT}/key/bl32-level-3-rsa-priv.pem"

### Input: chipset blobs ###
EXEC_ARGS="${EXEC_ARGS} --infile-blob-bl40=${BASEDIR_CHIPSET_TEMPLATE}/blob-bl40.bin${input_postfix}"
EXEC_ARGS="${EXEC_ARGS} --infile-blob-bl31=${BASEDIR_CHIPSET_TEMPLATE}/blob-bl31.bin${input_postfix}"
EXEC_ARGS="${EXEC_ARGS} --infile-blob-bl32=${BASEDIR_CHIPSET_TEMPLATE}/blob-bl32.bin${input_postfix}"

### Features, flags and switches ###

# arb info
EXEC_ARGS="${EXEC_ARGS} --val-device-vendor-segid=${DEVICE_VENDOR_SEGID}"
EXEC_ARGS="${EXEC_ARGS} --val-device-tee-vers=${DEVICE_TEE_VERS}"
EXEC_ARGS="${EXEC_ARGS} --val-device-ree-vers=${DEVICE_REE_VERS}"

### Output: Device FIP ###
EXEC_ARGS="${EXEC_ARGS} --outfile-device-fip=${BASEDIR_OUTPUT}/device-fip.bin${output_postfix}"

#echo ${EXEC_ARGS}

#
# Main
#

set -x

${ACPU_IMAGETOOL} \
        create-device-fip \
        ${EXEC_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
