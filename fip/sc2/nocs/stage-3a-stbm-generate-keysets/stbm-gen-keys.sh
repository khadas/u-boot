#!/bin/bash
#
# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

set -eu -o pipefail

trap 'echo "ERROR: ${BASH_SOURCE[0]}: line: $LINENO";' ERR

# Uncomment follow line for debugging
# set -x

if [ -f "common.bashrc" ]; then
    source common.bashrc
fi

check_bootloader_builddir

#
# Variables
#

STBM_DATA_BASEDIR=./output/data-stbm

#
# Main
#

mkdir -p ${STBM_DATA_BASEDIR}

${AMLOGIC_BOOTLOADER_BUILDDIR}/fip/sc2/generate-device-keys/gen_all_device_key.sh \
    --template-dir "${AMLOGIC_BOOTLOADER_BUILDDIR}/soc/templates/sc2" \
    --project ${PRJ} \
    --external-dvgk ${DVGK_EXTERNAL}	\
    --external_lvl1cert_epks ${LVL1CERT_EPKS} \
    --rsa-size 4096 \
    --out-dir "${STBM_DATA_BASEDIR}/outdir" \
    --key-dir "${STBM_DATA_BASEDIR}/keydir"

mkdir -p ${STBM_DATA_BASEDIR}/outdir/boot-blobs/rsa/${PRJ}/rootrsa-0/epk/
cp -adv \
    ${STBM_DATA_BASEDIR}/keydir/boot-blobs/rsa/${PRJ}/rootrsa-0/epk/lvl2cert-epks.bin \
    ${STBM_DATA_BASEDIR}/outdir/boot-blobs/rsa/${PRJ}/rootrsa-0/epk/lvl2cert-epks.bin

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
