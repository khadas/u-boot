#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../../binary-tool/acpu-imagetool
BASEDIR_CHIPSET_TEMPLATE=$1

#
# Settings
#

echo "====DDR===== ROOTRSA_INDEX ${DEVICE_ROOTRSA_INDEX}"
echo "====DDR=====       KEY_DIR ${BASEDIR_ROOT}"
echo "====DDR=====       PROJECT ${PROJECT}"

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

DDRFIP_ARGS="${DDRFIP_ARGS}"

DDRFIP_ARGS="${DDRFIP_ARGS} --infile-template-ddrfw-fip=${BASEDIR_CHIPSET_TEMPLATE}/blob-ddr-fip.bin.signed"
DDRFIP_ARGS="${DDRFIP_ARGS} --infile-signkey-device-lvl1=${BASEDIR_BOOTBLOBS_RSAKEY_ROOT}/key/level-1-rsa-priv.pem"
DDRFIP_ARGS="${DDRFIP_ARGS} --val-device-scs-segid=${DEVICE_SCS_SEGID}"
DDRFIP_ARGS="${DDRFIP_ARGS} --val-device-scs-vers=${DEVICE_SCS_VERS}"
DDRFIP_ARGS="${DDRFIP_ARGS} --outfile-ddrfw-fip=${BASEDIR_CHIPSET_TEMPLATE}/blob-ddr-fip.bin.signed"

echo ==== DDRFIP_ARGS ${DDRFIP_ARGS}

${ACPU_IMAGETOOL} \
        create-ddrfw-fip \
        ${DDRFIP_ARGS}
