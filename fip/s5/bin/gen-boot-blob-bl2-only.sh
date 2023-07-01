#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../binary-tool/acpu-imagetool

BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#

BASEDIR_PAYLOAD=$1

BASEDIR_NONCE="./nonce"

CHIPSET_NAME=$3
KEY_TYPE=$4
SOC_FAMILY=$5
CHIPSET_VARIANT_SUFFIX=$6

BASEDIR_AESKEY_PROT_BL2="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl2/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL2="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl2/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL31="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl31/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL31="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl31/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL32="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl32/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL32="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl32/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL40="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl40/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL40="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/bl40/rsa/${CHIPSET_NAME}"

BASEDIR_TEMPLATE="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC_FAMILY}/chipset/cert-template/${CHIPSET_NAME}"

BASEDIR_OUTPUT_BLOB=$2
postfix=.signed
#
# Arguments
#
#stage 1
BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template ###

BB1ST_ARGS="${BB1ST_ARGS} --infile-template-bb1st=${BASEDIR_TEMPLATE}/bb1st${FEAT_BL2_TEMPLATE_TYPE}${CHIPSET_VARIANT_SUFFIX}.bin"

### Input: payloads ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-bl2-payload=${BASEDIR_PAYLOAD}/bl2-payload.bin"

### Input: Chipset Level-1/2 Private RSA keys

BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-chipset-lvl1=${BASEDIR_RSAKEY_LVLX_BL2}/level-1-rsa-priv.pem"
BB1ST_ARGS="${BB1ST_ARGS} --infile-signkey-chipset-lvl2=${BASEDIR_RSAKEY_LVLX_BL2}/level-2-rsa-priv.pem"

### Input: pre-generated ProtKey for payloads
BB1ST_ARGS="${BB1ST_ARGS} --infile-aes256-bl2-payload=${BASEDIR_AESKEY_PROT_BL2}/genkey-prot-bl2.bin"

BB1ST_ARGS="${BB1ST_ARGS} --scs-family=${SOC_FAMILY}"

### Features, flags and switches ###

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-bb1st=${BASEDIR_OUTPUT_BLOB}/bb1st${FEAT_BL2_TEMPLATE_TYPE}${CHIPSET_VARIANT_SUFFIX}.bin.bl2-only"

#
# Main
#

set -x

${ACPU_IMAGETOOL} \
        create-boot-blobs \
        ${BB1ST_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
