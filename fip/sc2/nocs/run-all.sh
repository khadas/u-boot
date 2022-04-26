#!/bin/bash
if [ -s ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin ];
then
	echo "update device vendor segid"
	c1=$(xxd -ps -s 7664 -c4 -g4 -l1 fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin)
	c2=$(xxd -ps -s 7665 -c4 -g4 -l1 fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin)
	c3=$(xxd -ps -s 7666 -c4 -g4 -l1 fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin)
	c4=$(xxd -ps -s 7667 -c4 -g4 -l1 fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin)

	vendsg="DEVICE_VENDOR_SEGID=0x$c4$c3$c2$c1"
	scssg="DEVICE_SCS_SEGID=0x$c4$c3$c2$c1"
	echo $vendsg
	echo $scssg
	sed -i 's/DEVICE_VENDOR_SEGID.*/'$vendsg'/' bl33/v2019/board/amlogic/$1/fw_arb.cfg
	sed -i 's/DEVICE_SCS_SEGID.*/'$scssg'/' bl33/v2019/board/amlogic/$1/fw_arb.cfg
	mkdir -p ./fip/sc2/nocs/efuse/input/
	mkdir -p ./fip/sc2/nocs/efuse/output/
	./fip/sc2/bin/efuse-gen.sh --device-vendor-segid 0x$c4$c3$c2$c1 -o ./fip/sc2/nocs/efuse/input/device-vendor.efuse
fi

if [ -z $3 ];		## without --prod
then
	echo "Start to build Uboot";
		if [ ! -f "dv_scs_keys/root/rsa/s905c2/roothash/hash-device-rootcert.bin"  ];
		then	echo "generate dfu keys";
			./fip/sc2/generate-device-keys/gen_all_device_key.sh --key-dir ./dv_scs_keys --rsa-size 4096 --project s905c2 --rootkey-index 0 --template-dir ./soc/templates/sc2/ --out-dir ./bl33/v2019/board/amlogic/$1/device-keys
			./fip/sc2/nocs/createtemplate.sh $1
			mkdir -p ./fip/sc2/nocs/efuse/input
			mkdir -p ./fip/sc2/nocs/efuse/output
			./fip/sc2/bin/efuse-gen.sh --dfu-device-roothash ./dv_scs_keys/root/rsa/s905c2/roothash/hash-device-rootcert.bin -o ./fip/sc2/nocs/efuse/input/dfu.efuse
			./fip/sc2/bin/efuse-gen.sh --dvgk ./dv_scs_keys/root/dvgk/s905c2/dvgk.bin -o ./fip/sc2/nocs/efuse/input/dvgk.efuse 
		else
			./fip/sc2/nocs/createtemplate.sh $1
			echo "no need generate dfu keys";
		fi;
	sed -i 's/\/\/#define\ CONFIG_AML_SIGNED_UBOOT.*/'#define\ CONFIG_AML_SIGNED_UBOOT\ \ \ 1'/' bl33/v2019/board/amlogic/configs/$1.h
	./mk $1 --chip-varient nocs-prod
	mkdir -p ./fip/sc2/nocs/usb
	cp build/* ./fip/sc2/nocs/usb
## generate device keys
	cp ./dv_scs_keys/root/dvgk/s905c2/dvgk.bin ./fip/sc2/nocs
	cd ./fip/sc2/nocs

	if [ ! -f "./stage-3a-stbm-generate-keysets/output/data-stbm/keydir/boot-blobs/rsa/s905c2/rootrsa-0/key/level-2-rsa-pub.pem"  ];
	then echo "generate device key";
		./run-generate-key.sh
	else
		echo "no need geneate device key";
	fi;

	./installkeys.sh $1 stage-3a-stbm-generate-keysets/
	cd -
	./mk $1 --update-bl2e sto --chip-varient nocs-prod --former-sign $2
	cd -
	./run-pre.sh
	cd -
	if [ ! -s ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin ];
	then
		echo "!!!PLEASE PUT NAGRA SIGNED FILE to ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin";
		echo "THEN PLEASE RE-RUN <./fip/sc2/nocs/run-all.sh sc2_ahxxx '--bl2e-size ...'>";
	else
		echo "NOW PLEASE RUN <./fip/sc2/nocs/run-all.sh sc2_ahxxx '--bl2e-size ...' prod> to generate complete U-boot.bin under fip/sc2/nocs/sto";
	fi;

else		# no variable just package uboot
	if [ ! -s ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin ];
        then
                echo "!!!PLEASE PUT NAGRA SIGNED FILE to ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin";
	exit;fi;
	cd fip/sc2/nocs
	./run-post.sh
	./package-uboot.sh sto

#merge efuse files.
	if [ -f "efuse/input/*.bin"  ];
	then	echo "generate all.bin based on input .bin and .dec";
	./efuse/combine-efuse output/all.bin ./efuse/input/*.bin ./efuse/input/*.dec
	else
		echo "generate all.bin based on input .dec";
		./efuse/combine-efuse ./efuse/output/all.bin ./efuse/input/*.dec
		../aml_encrypt_sc2 --efsproc --input ./efuse/output/all.bin --output ./efuse/output/all.efuse --option=debug
	fi;
	cd -
	echo "please use ./fip/sc2/nocs/usb/u-boot.bin.usb.device.signed for dfu mode";
	echo "please use ./fip/sc2/nocs/sto/u-boot.bin for normal mode uboot";
	echo "please prodgram ./fip/sc2/nocs/efuse/out/all.efuse into chipset OTP space";
fi;
