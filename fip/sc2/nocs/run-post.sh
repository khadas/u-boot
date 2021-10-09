#!/bin/bash

set -eu -o pipefail

trap 'echo "ERROR: ${BASH_SOURCE[0]}: line: $LINENO";' ERR

# Uncomment following line for debugging
# set -x

#
# Main
#

set -x
export AMLOGIC_BOOTLOADER_BUILDDIR=$(pwd)/../../../
export AESKEY_ROOT_BASEDIR==$(pwd)/stage-3a-stbm-generate-keysets/output/data-stbm/outdir/root/aes/$PRJ/rootkey
( cd stage-5-* && ./run-it.sh )

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
