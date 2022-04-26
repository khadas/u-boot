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

#
# Main
#

copy_files ${SRC_DIR} ${DST_DIR} "${LIST_FILES}"

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
