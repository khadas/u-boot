#!/bin/bash
BL33_BIN_SIZE="1572864"
if [[ $1 =~ "sc2" ]];
then
PROJECT=`cat ./bl33/v2019/board/amlogic/defconfigs/$1_defconfig | grep 'CONFIG_CHIPSET_NAME' | awk -F '=' '{print $2}' `
PRJ=`echo $PROJECT | tr -d "\""`
echo $PRJ
export PRJ
fi;

if [[ $1 == "ta_seg_id" ]];		## generate dfu&device keys
then
	mkdir -p ./fip/sc2/nocs/efuse/input
	mkdir -p ./fip/sc2/nocs/efuse/output
	./fip/sc2/bin/efuse-gen.sh --ta-segid $2 -o ./fip/sc2/nocs/efuse/input/ta-segid.efuse 

elif [[ $2 == "generate_keys" ]];		## generate dfu&device keys
then
	echo "Start to build Uboot";
		if [ ! -f "dv_scs_keys/root/rsa/$PRJ/roothash/hash-device-rootcert.bin"  ];
		then	echo "generate dfu keys";
			./fip/sc2/generate-device-keys/gen_all_device_key.sh --key-dir ./dv_scs_keys --rsa-size 4096 --project $PRJ --rootkey-index 0 --template-dir ./soc/templates/sc2/ --out-dir ./bl33/v2019/board/amlogic/$1/device-keys
			./fip/sc2/nocs/createtemplate.sh $1
			./fip/sc2/generate-device-keys/bin/dvuk_gen.sh fip/sc2/nocs/dvuk
			mkdir -p ./fip/sc2/nocs/efuse/input
			mkdir -p ./fip/sc2/nocs/efuse/output
			./fip/sc2/bin/efuse-gen.sh --dfu-device-roothash ./dv_scs_keys/root/rsa/$PRJ/roothash/hash-device-rootcert.bin -o ./fip/sc2/nocs/efuse/input/dfu.efuse
			./fip/sc2/bin/efuse-gen.sh --dvgk ./dv_scs_keys/root/dvgk/$PRJ/dvgk.bin -o ./fip/sc2/nocs/efuse/input/dvgk.efuse
			./fip/sc2/bin/efuse-gen.sh --dvuk ./fip/sc2/nocs/dvuk.bin -o ./fip/sc2/nocs/efuse/input/dvuk.efuse
		else
			./fip/sc2/nocs/createtemplate.sh $1
			echo "no need generate dfu keys"; 
		fi;
## generate device keys
	cp ./dv_scs_keys/root/dvgk/$PRJ/dvgk.bin ./fip/sc2/nocs
	cd ./fip/sc2/nocs

	if [ ! -f "./stage-3a-stbm-generate-keysets/output/data-stbm/keydir/boot-blobs/rsa/$PRJ/rootrsa-0/key/level-2-rsa-pub.pem"  ];
	then echo "generate device key";
		./run-generate-key.sh
	else
		echo "no need geneate device key";
	fi;
elif [[ $2 == "production" ]];
then
	if [ ! -s ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin ];
        then
                echo "!!!PLEASE PUT NAGRA SIGNED FILE to ./fip/sc2/nocs/stage-4-nagra-signing/output/refImgSigned.bin";
	exit;fi;
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
	./fip/sc2/nocs/createtemplate.sh $1
	cd ./fip/sc2/nocs
	./package-uboot.sh usb
	cd -
	./fip/sc2/bin/device-vendor-scs-signing.sh --key-dir bl33/v2019/board/amlogic/$1/device-keys/ --project $PRJ --input-dir ./fip/sc2/nocs/usb/ --rootkey-index 0 --chipset-variant nocs-prod --arb-config ./bl33/v2019/board/amlogic/$1/fw_arb.cfg --out-dir ./fip/sc2/nocs/usb/
	cd ./fip/sc2/nocs
	./run-post.sh
	./package-uboot.sh sto
	./installkeys.sh $1 stage-3a-stbm-generate-keysets/
	if [[  $3 =~ "nasc_nagra_tier_1" ]];
	then
		cp stage-4-nagra-signing/output/bl33-payload.bin sto/bl33-payload.bin
		bl33_size=`stat -c %s sto/bl33-payload.bin`
		if [[ ${bl33_size} -gt ${BL33_BIN_SIZE} ]]; then
			echo size of bl33-payload.bin is ${bl33_size}, large than ${BL33_BIN_SIZE}
			exit -1
		fi

		dd if=/dev/zero of=sto/tmp.bin bs=${BL33_BIN_SIZE} count=1 &> /dev/null
		dd if=sto/bl33-payload.bin of=sto/tmp.bin conv=notrunc &> /dev/null
		mv sto/tmp.bin sto/bl33-payload.bin
	fi;
	cd -
	./fip/sc2/bin/device-vendor-scs-signing.sh --key-dir bl33/v2019/board/amlogic/$1/device-keys/ --project $PRJ --input-dir ./fip/sc2/nocs/sto/ --rootkey-index 0 --chipset-variant nocs-prod --arb-config ./bl33/v2019/board/amlogic/$1/fw_arb.cfg --out-dir ./fip/sc2/nocs/sto/
	cd -
	cp stage-5-stbm-process-signed/output/bb1st.bin sto/bb1st.sto.nocs-prod.bin.device.signed
	cp stage-5-stbm-process-signed/output/blob-bl2e.bin sto/blob-bl2e.sto.nocs-prod.bin.device.signed
	cd -
	cp ./fip/_tmp/blob-ddr-fip.bin.signed ./fip/sc2/nocs/sto/
	source ./fip/sc2/build.sh
	export DDR_FIP_NAME=blob-ddr-fip.bin
	mk_uboot ./fip/sc2/nocs/sto/ ./fip/sc2/nocs/sto/ .device.signed .sto .nocs-prod
	cd -
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
	rm -f ./build/u-boot.bin.sd.*
	cp fip/sc2/nocs/usb/u-boot.bin.usb.* ./build
	cp fip/sc2/nocs/sto/u-boot.bin.device.* ./build/u-boot.bin.device.signed
	echo "please use ./build/u-boot.bin.usb.device.signed for dfu mode";
	echo "please use ./build/u-boot.bin.device.signed for normal mode uboot";
	echo "please program ./fip/sc2/nocs/efuse/out/all.efuse into chipset OTP space";
else		#//build usb and sto mode u-boot
	sed -i 's/\/\/#define\ CONFIG_AML_SIGNED_UBOOT.*/'#define\ CONFIG_AML_SIGNED_UBOOT\ \ \ 1'/' bl33/v2019/board/amlogic/configs/$1.h
	sed -i 's/#define\ CONFIG_AML_SIGNED_UBOOT /\/\/#define\ CONFIG_AML_SIGNED_UBOOT /' bl33/v2019/board/amlogic/configs/$1.h
	./mk $1 --chip-varient nocs-prod --former-sign $2
	cd ./fip/sc2/nocs
	./run-pre.sh
	cd -
	echo "NOW PLEASE RUN <./fip/sc2/nocs/run-all.sh sc2_ahxxx production> to generate complete U-boot.bin under fip/sc2/nocs/sto";
fi;
