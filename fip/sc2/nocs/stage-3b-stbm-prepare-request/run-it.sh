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

#
# Main
#

./stbm-prepare-blobs.sh
./stbm-gen-nocs-refimg.sh
echo

# vim: set filetype=sh tabstop=2 expandtab shiftwidth=2:
