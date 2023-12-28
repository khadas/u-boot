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

BASEDIR_TEMPLATE="${BASEDIR_TOP}/templates"

BASEDIR_PAYLOAD=$2

BASEDIR_NONCE="./nonce"

CHIPSET_NAME=$4
KEY_TYPE=$5
SOC=$6

BASEDIR_AESKEY_PROT_BL2="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl2/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL2="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl2/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL31="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl31/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL31="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl31/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL32="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl32/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL32="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl32/rsa/${CHIPSET_NAME}"

BASEDIR_AESKEY_PROT_BL40="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl40/aes/${CHIPSET_NAME}"
BASEDIR_RSAKEY_LVLX_BL40="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/bl40/rsa/${CHIPSET_NAME}"

BASEDIR_OUTPUT_BLOB=$3
postfix=.signed
#
# Arguments
#

BLOB_NAME=$1
_BASEDIR_AESKEY_PROT_DIR="BASEDIR_AESKEY_PROT_BL${BLOB_NAME}"
BASEDIR_AESKEY_PROT_DIR=${!_BASEDIR_AESKEY_PROT_DIR}
_BASEDIR_RSAKEY_LVLX_DIR="BASEDIR_RSAKEY_LVLX_BL${BLOB_NAME}"
BASEDIR_RSAKEY_LVLX_DIR=${!_BASEDIR_RSAKEY_LVLX_DIR}

EXEC_ARGS="${EXEC_ARGS}"

### Input: payload ###
EXEC_ARGS="${EXEC_ARGS} --infile-bl${BLOB_NAME}-payload=${BASEDIR_PAYLOAD}/bl${BLOB_NAME}-payload.bin"

### Input: Chipset Level-1/2 Private RSA keys

EXEC_ARGS="${EXEC_ARGS} --infile-signkey-bl${BLOB_NAME}-chipset-lvl3=${BASEDIR_RSAKEY_LVLX_DIR}/bl${BLOB_NAME}-level-3-rsa-priv.pem"

### Input: nonce for binary protection ###
#EXEC_ARGS="${EXEC_ARGS} --infile-nonce-blob-bl${BLOB_NAME}=${BASEDIR_NONCE}/chipset/blob/blob-bl${BLOB_NAME}-nonce.bin"

### Input: pre-generated ProtKey for payload
EXEC_ARGS="${EXEC_ARGS} --infile-aes256-bl${BLOB_NAME}-payload=${BASEDIR_AESKEY_PROT_DIR}/genkey-prot-bl${BLOB_NAME}.bin"

### Features, flags and switches ###

### Output: blobs ###
EXEC_ARGS="${EXEC_ARGS} --outfile-blob-bl${BLOB_NAME}=${BASEDIR_OUTPUT_BLOB}/blob-bl${BLOB_NAME}.bin${postfix}"

#echo ${EXEC_ARGS}

#
# Main
#

set -x

${ACPU_IMAGETOOL} \
        create-device-fip \
        ${EXEC_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
