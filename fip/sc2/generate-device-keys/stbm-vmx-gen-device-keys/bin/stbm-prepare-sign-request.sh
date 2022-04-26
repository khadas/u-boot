#!/bin/bash
#
# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

set -e

# Uncomment follow line for debugging
#set -x

copy_files() {
  src_dir=$1
  dst_dir=$2
  list=$3

  for f in $list; do
    d="$(dirname $f)"
    mkdir -p ${dst_dir}/${d}
    cp ${src_dir}/$f ${dst_dir}/${d}/.
  done
}

#
# Variables
#

STBM_DATA_BASEDIR=$1
TO_VMX_BASEDIR=$2

LIST_KEYS=""
LIST_KEYS="${LIST_KEYS} boot-blobs/rsa/s905x4/rootrsa-0/key/level-1-rsa-pub.pem"
LIST_KEYS="${LIST_KEYS} boot-blobs/rsa/s905x4/rootrsa-0/key/level-2-rsa-pub.pem"
LIST_KEYS="${LIST_KEYS} boot-blobs/rsa/s905x4/rootrsa-0/epk/lvl1cert-epks.bin"
LIST_KEYS="${LIST_KEYS} boot-blobs/rsa/s905x4/rootrsa-0/epk/lvl2cert-epks.bin"

LIST_TEMPLATES=""
LIST_TEMPLATES="${LIST_TEMPLATES} boot-blobs/template/s905x4/rootrsa-0/bb1st.bin"
LIST_TEMPLATES="${LIST_TEMPLATES} fip/template/s905x4/rootrsa-0/device-fip-header.bin"

#
# Main
#

### Copy files ###

copy_files \
  "${STBM_DATA_BASEDIR}/keydir" \
  "${TO_VMX_BASEDIR}" \
  "${LIST_KEYS}"

copy_files \
  "${STBM_DATA_BASEDIR}/outdir" \
  "${TO_VMX_BASEDIR}" \
  "${LIST_TEMPLATES}"

mv ${TO_VMX_BASEDIR}/boot-blobs/template/s905x4/rootrsa-0/bb1st.bin \
   ${TO_VMX_BASEDIR}/boot-blobs/template/s905x4/rootrsa-0/bb1st.bin.signed

ln -fs device-fip-header.bin \
   ${TO_VMX_BASEDIR}/fip/template/s905x4/rootrsa-0/device-fip-header.ree-vers.0.tee-vers.0.bin

### Show prepared files ###
find ${TO_VMX_BASEDIR} -type f

# vim: set filetype=sh tabstop=2 expandtab shiftwidth=2:
