#!/bin/bash
echo "./bl33/v2019/board/amlogic/$1/fw_arb.cfg"
source ./bl33/v2019/board/amlogic/$1/fw_arb.cfg
export DEVICE_SCS_VERS
export DEVICE_TEE_VERS
export DEVICE_REE_VERS
export DEVICE_SCS_SEGID
export DEVICE_VENDOR_SEGID
#cp ./generate-binaries/data/template/chipset/bb1st*.bin ./create-template/data/template
device_fip_arb_args="--device-vendor-segid ${DEVICE_VENDOR_SEGID} --device-tee-vers ${DEVICE_TEE_VERS} --device-ree-vers ${DEVICE_REE_VERS}"
boot_blobs_arb_args="--device-scs-segid ${DEVICE_SCS_SEGID} --device-vendor-segid ${DEVICE_VENDOR_SEGID} --device-scs-vers ${DEVICE_SCS_VERS} --device-tee-vers ${DEVICE_TEE_VERS}"

./fip/sc2/generate-device-keys/bin/gen_device_aes_protkey.sh --rootkey-index 0  --project ${PRJ} --key-dir ./dv_scs_keys --template-dir ./soc/templates/sc2/ ${device_fip_arb_args}
./fip/sc2/generate-device-keys/bin/gen_device_root_hash.sh --rootkey-index 0 --key-dir ./dv_scs_keys --project ${PRJ} --template-dir ./soc/templates/sc2/ ${boot_blobs_arb_args}
./fip/sc2/generate-device-keys/bin/export_dv_scs_signing_keys.sh --key-dir ./dv_scs_keys --out-dir ./bl33/v2019/board/amlogic/$1/device-keys --rootkey-index 0 --project ${PRJ}
