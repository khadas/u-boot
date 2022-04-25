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
#( cd stage-3a-* && ./run-it.sh )
( cd stage-3b-* && ./run-it.sh )
( cd stage-4-* && ./run-it.sh )

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
