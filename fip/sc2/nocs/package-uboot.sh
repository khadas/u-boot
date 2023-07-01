#!/bin/bash
#export CONFIG_DDR_FULL_FW=y
if [[ $1 =~ "sto" ]];
then
mkdir -p sto
cp ../../_tmp/b*.signed sto/
cp ../../_tmp/dvinit-params.bin sto/
cp ../../_tmp/bl30-payload.bin sto/
cp ../../_tmp/bl33-payload.bin sto/

else
mkdir -p usb
cp ../../_tmp/b*.signed usb/
cp ../../_tmp/dvinit-params.bin usb/
cp ../../_tmp/bl30-payload.bin usb/
cp ../../_tmp/bl33-payload.bin usb/

fi;