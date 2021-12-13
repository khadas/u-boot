#!/bin/bash
#export CONFIG_DDR_FULL_FW=y
mkdir -p sto
export DDR_FIP_NAME=blob-ddr-fip.bin
cp stage-5-stbm-process-signed/output/bb1st.bin sto/bb1st.sto.bin
cp stage-5-stbm-process-signed/output/blob-bl2e.bin sto/blob-bl2e.sto.bin
cp ../../_tmp/blob-bl2x.bin.device.signed sto/blob-bl2x.bin
cp ../../_tmp/blob-ddr-fip.bin.signed sto/blob-ddr-fip.bin
cp ../../_tmp/device-fip.bin.device.signed sto/device-fip.bin
if [ -z "$1" ]; then echo "Please input usb or sto mode"; exit 1 ; fi
USBSTO=$1
source ../build.sh
mk_uboot ${USBSTO} sto "" .sto  ""
