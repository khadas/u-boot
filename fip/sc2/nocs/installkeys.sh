#!/bin/bash

set -eu -o pipefail

trap 'echo "ERROR: ${BASH_SOURCE[0]}: line: $LINENO";' ERR

# Uncomment following line for debugging
# set -x

#
# Main
#
echo "../../../bl33/v2019/board/amlogic/$1/fw_arb.cfg"
source ../../../bl33/v2019/board/amlogic/$1/fw_arb.cfg
export DEVICE_SCS_VERS
export DEVICE_TEE_VERS
export DEVICE_REE_VERS
export DEVICE_SCS_SEGID
export DEVICE_VENDOR_SEGID
#cp ./generate-binaries/data/template/chipset/bb1st*.bin ./create-template/data/template
device_fip_arb_args="--device-vendor-segid ${DEVICE_VENDOR_SEGID} --device-tee-vers ${DEVICE_TEE_VERS} --device-ree-vers ${DEVICE_REE_VERS}"

../generate-device-keys/bin/gen_device_aes_protkey.sh --rootkey-index 0  --project ${PRJ} --key-dir ./$2/output/data-stbm/keydir --template-dir ../../../soc/templates/sc2/ ${device_fip_arb_args}
../generate-device-keys/bin/export_dv_scs_signing_keys.sh --key-dir ./$2/output/data-stbm/keydir --out-dir ./$2/output/data-stbm/outdir --rootkey-index 0 --project ${PRJ}

set -x
INSTALLDIR=$1
DEVICE_KEYS=../../../bl33/v2019/board/amlogic/$1/device-keys
if [ -d ${DEVICE_KEYS} ]; then
	rm -rf ${DEVICE_KEYS}
fi
mkdir -p ${DEVICE_KEYS}/
cp -r ./stage-3a-stbm-generate-keysets/output/data-stbm/outdir/* ../../../bl33/v2019/board/amlogic/$1/device-keys

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
