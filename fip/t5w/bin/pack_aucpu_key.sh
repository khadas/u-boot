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
CHIPSET_NAME=$2
KEY_TYPE=$3
SOC=$4

BASEDIR_AUCPU_CERT="${BASEDIR_TOP}/keys/${KEY_TYPE}/${SOC}/chipset/cert-template/${CHIPSET_NAME}"


#
# Arguments
#

dd if=${BASEDIR_AUCPU_CERT}/fw-aucpu-cert.bin of=${BLOB_NAME} bs=1 seek=1536 conv=notrunc >& /dev/null

# vim: set tabstop=2 expandtab shiftwidth=2:
