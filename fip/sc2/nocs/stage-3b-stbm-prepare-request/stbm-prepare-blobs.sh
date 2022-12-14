#!/bin/bash

set -eu -o pipefail

trap 'echo "ERROR: ${BASH_SOURCE[0]}: line: $LINENO";' ERR

# Uncomment following line for debugging
# set -x

if [ -f "common.bashrc" ]; then
    source common.bashrc
fi

check_bootloader_builddir

#
# Variables and settings
#

# Input files
FIN_BB1ST_BIN="${AMLOGIC_BOOTLOADER_BUILDDIR}/fip/_tmp/bb1st.sto.nocs-prod.bin.signed"
FIN_BLOB_BL2E="${AMLOGIC_BOOTLOADER_BUILDDIR}/fip/_tmp/blob-bl2e.sto.nocs-prod.bin.signed"

# Output filename according to size of BL2E
FOUT_BB1ST_BIN="intermediate/bb1st.bin"
FOUT_BLOB_BL2E="intermediate/blob-bl2e.bin"

BLOBS_ARGS=""

if [ "x${CONFIG_FLASHPROT_ENABLE}" == "xy" ]; then
	BLOBS_ARGS="${BLOBS_ARGS} --flag-enable-flashprot"
fi


#
# Functions
#

#
# Main
#

### Preparation ###
mkdir -p "./intermediate/"

### Prepare 1st Boot Blob ###
${ACPU_IMAGETOOL} create-boot-blobs \
    --scs-family=sc2 ${BLOBS_ARGS}	\
    --infile-template-bb1st=${FIN_BB1ST_BIN} \
    --infile-pubkey-device-lvl2cert=${DEVICE_KEYS_BASEDIR}/boot-blobs/rsa/${PRJ}/rootrsa-0/key/level-2-rsa-pub.pem \
    --infile-epks-device-lvl2cert=${DEVICE_KEYS_BASEDIR}/boot-blobs/rsa/${PRJ}/rootrsa-0/epk/lvl2cert-epks.bin \
    --outfile-bb1st=${FOUT_BB1ST_BIN}

### Copy BLOB BL2E ###
cp -adv ${FIN_BLOB_BL2E} ${FOUT_BLOB_BL2E}

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
