#!/bin/bash

#set -eu -o pipefail
set -x

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
FIN_BB1ST_BIN="intermediate/bb1st.bin"
FIN_BLOB_BL2E="intermediate/blob-bl2e.bin"

# Output filename according to size of BL2E
FOUT_BB1ST_BIN="output/bb1st.bin"
FOUT_BLOB_BL2E="output/blob-bl2e.bin"

#
# Functions
#

#
# Main
#

### Preparation ###
mkdir -p output/

### 1st Boot Blob ###
${ACPU_IMAGETOOL} create-boot-blobs \
    --scs-family=sc2 \
    --infile-template-bb1st=${FIN_BB1ST_BIN} \
    --infile-aes256-device-rootkey-2=${AESKEY_ROOT_BASEDIR}/aes256-device-rootkey-bootstage-2.bin \
    --switch-device-sign-blob=0 \
    --feature-enable-device-lvl1-pubrsa-prot \
    --feature-enable-device-lvlx-pubrsa-prot \
    --outfile-bb1st=${FOUT_BB1ST_BIN}

dd status=none iflag=skip_bytes,count_bytes \
        if=${FIN_BB1ST_BIN} \
        skip=$(( 0x2000 )) count=$(( 0x1dff0 )) \
    | dd status=none oflag=seek_bytes conv=notrunc \
        of=${FOUT_BB1ST_BIN} \
        seek=$(( 0x2000 ))

dd status=none iflag=skip_bytes,count_bytes \
        if=${DEVICE_KEYS_BASEDIR}/boot-blobs/template/${PRJ}/rootrsa-0/bb1st.bin \
        skip=$(( 0x2080 )) count=$(( 0x500 )) \
    | dd status=none oflag=seek_bytes conv=notrunc \
        of=${FOUT_BB1ST_BIN} \
        seek=$(( 0x2080 ))

### Blob BL2E ###
cp -adv ${FIN_BLOB_BL2E} ${FOUT_BLOB_BL2E}

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
