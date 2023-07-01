#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#
BLOB_NAME=$1
BASEDIR_PAYLOAD=$2
BASEDIR_OUTPUT_BLOB=$3
CHIPSET_NAME=$4
KEY_TYPE=$5
SOC=$6

BASEDIR_AUCPU_CERT="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/cert-template/${CHIPSET_NAME}"


#
# Arguments
#

dd if=${BASEDIR_AUCPU_CERT}/fw-aucpu-cert.bin of=${BASEDIR_PAYLOAD}/bl${BLOB_NAME}-payload.bin bs=1 seek=1024 conv=notrunc >& /dev/null

# vim: set tabstop=2 expandtab shiftwidth=2:
