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

SRC_DIR="../stage-3b-stbm-prepare-request/output/"
DST_DIR="./input/"

LIST_FILES="sha256partial-bl2.bin stbmRefImg.bin"

#
# Functions
#
function create_padded_refimg() {
	f_orig="${DST_DIR}/stbmRefImg.bin"
	f_padded="${f_orig}-padded"

	cp ${f_orig} ${f_padded}
	truncate -s 1318000 ${f_padded}
}

#
# Main
#

copy_files ${SRC_DIR} ${DST_DIR} "${LIST_FILES}"

create_padded_refimg

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
