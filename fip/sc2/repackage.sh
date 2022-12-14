
key_dir=$1
projects=$2
input_dir=$3
board=$4
chipset_variant=$5
BL33_BIN_SIZE="1572864"
DDR_FIP_NAME="blob-ddr-fip.bin.signed"

if [ ! -f ${input_dir}/bl33-payload-nocs.bin ]; then
	echo NO bl33-payload-nocs.bin
	exit -1
fi

bl33_size=`stat -c %s ${input_dir}/bl33-payload-nocs.bin`

if [[ ${bl33_size} -gt ${BL33_BIN_SIZE} ]]; then
	echo size of bl33-payload-nocs.bin is ${bl33_size}, large than ${BL33_BIN_SIZE}
	exit -1
fi

if [ ! -f ${input_dir}/${board}-u-boot.aml.zip ]; then
	echo No ${board}-u-boot.aml.zip
	exit -1
fi

work_dir=${input_dir}/tmp

rm -rf ${input_dir}/tmp/
mkdir -p ${input_dir}/tmp/
cp ${input_dir}/${board}-u-boot.aml.zip ${input_dir}/tmp/
unzip ${input_dir}/tmp/${board}-u-boot.aml.zip -d ${input_dir}/tmp/

ls -l ${input_dir}/tmp/


dd if=/dev/zero of=${input_dir}/tmp.bin bs=${BL33_BIN_SIZE} count=1 &> /dev/null
dd if=${input_dir}/bl33-payload-nocs.bin of=${input_dir}/tmp.bin conv=notrunc &> /dev/null
mv ${input_dir}/tmp.bin ${input_dir}/bl33-payload-nocs.bin

cp ${input_dir}/bl33-payload-nocs.bin ${input_dir}/tmp/bl33-payload.bin
rm ${input_dir}/tmp/${board}-u-boot.aml.zip
zip -D -j ${input_dir}/${board}-uboot-nocs.zip ${input_dir}/tmp/*

function mk_uboot() {
	output_images=$1
	input_payloads=$2
	postfix=$3
	storage_type_suffix=$4
	chipset_variant_suffix=$5

	device_fip="${input_payloads}/device-fip.bin${postfix}"
	bb1st="${input_payloads}/bb1st${storage_type_suffix}.${chipset_variant_suffix}.bin${postfix}"
	bl2e="${input_payloads}/blob-bl2e${storage_type_suffix}.${chipset_variant_suffix}.bin${postfix}"
	bl2x="${input_payloads}/blob-bl2x.bin${postfix}"

	if [ ! -f ${device_fip} ] || \
	   [ ! -f ${bb1st} ] || \
	   [ ! -f ${bl2e} ] || \
	   [ ! -f ${bl2x} ]; then
		echo fip:${device_fip}
		echo bb1st:${bb1st}
		echo bl2e:${bl2e}
		echo bl2x:${bl2x}
		echo "Error: ${input_payloads}/ bootblob does not all exist... abort"
		ls -la ${input_payloads}/
		exit -1
	fi

	file_info_cfg="${output_images}/aml-payload.cfg"
	file_info_cfg_temp=${temp_cfg}.temp

	bootloader="${output_images}/u-boot.bin${storage_type_suffix}${postfix}"
	sdcard_image="${output_images}/u-boot.bin.sd.bin${postfix}"

	#fake ddr fip 256KB
	ddr_fip="${input_payloads}/${DDR_FIP_NAME}"
	if [ ! -f ${ddr_fip} ]; then
		echo "==== WARNING: use empty ddr-fip ===="
		dd if=/dev/zero of=${ddr_fip} bs=1024 count=256 status=none
	fi

	#cat those together with 4K upper aligned for sdcard
	align_base=4096
	total_size=0
	for file in ${bb1st} ${bl2e} ${bl2x} ${ddr_fip} ${device_fip}; do
		size=`stat -c "%s" ${file}`
		upper=$[(size+align_base-1)/align_base*align_base]
		total_size=$[total_size+upper]
		echo ${file} ${size} ${upper}
	done

	echo ${total_size}
	rm -f ${bootloader}
	dd if=/dev/zero of=${bootloader} bs=${total_size} count=1 status=none

	sector=512
	seek=0
	seek_sector=0
	dateStamp=SC2-`date +%Y%m%d%H%M%S`

	echo @AMLBOOT > ${file_info_cfg_temp}
	dd if=${file_info_cfg_temp} of=${file_info_cfg} bs=1 count=8 conv=notrunc &> /dev/null
	nItemNum=5
	nSizeHDR=$[64+nItemNum*16]
	printf "01 %02x %02x %02x 00 00 00 00" $[(nItemNum)&0xFF] $[(nSizeHDR)&0xFF] $[((nSizeHDR)>>8)&0xFF] \
		| xxd -r -ps > ${file_info_cfg_temp}
	cat ${file_info_cfg_temp} >> ${file_info_cfg}

	echo ${dateStamp} > ${file_info_cfg_temp}
	dd if=${file_info_cfg_temp} of=${file_info_cfg} bs=1 count=16 oflag=append conv=notrunc &> /dev/null

	index=0
	arrPayload=("BBST" "BL2E" "BL2X" "DDRF" "DEVF");
	nPayloadOffset=0
	nPayloadSize=0
	for file in ${bb1st} ${bl2e} ${bl2x} ${ddr_fip} ${device_fip}; do
		size=`stat -c "%s" ${file}`
		size_sector=$[(size+align_base-1)/align_base*align_base]
		nPayloadSize=$[size_sector]
		size_sector=$[size_sector/sector]
		seek_sector=$[seek/sector+seek_sector]
		#nPayloadOffset=$[sector*(seek_sector+1)]
		nPayloadOffset=$[sector*(seek_sector)]
		echo ${file} ${seek_sector} ${size_sector}
		dd if=${file} of=${bootloader} bs=${sector} seek=${seek_sector} conv=notrunc status=none

		echo ${arrPayload[$index]} > ${file_info_cfg_temp}.x
		index=$((index+1))
		dd if=${file_info_cfg_temp}.x of=${file_info_cfg_temp} bs=1 count=4 &> /dev/null
		rm -f ${file_info_cfg_temp}.x
		printf "%02x %02x %02x %02x %02x %02x %02x %02x 00 00 00 00" $[(nPayloadOffset)&0xFF] $[((nPayloadOffset)>>8)&0xFF] $[((nPayloadOffset)>>16)&0xFF] $[((nPayloadOffset)>>24)&0xFF] \
		$[(nPayloadSize)&0xFF] $[((nPayloadSize)>>8)&0xFF] $[((nPayloadSize)>>16)&0xFF] $[((nPayloadSize)>>24)&0xFF] | xxd -r -ps >> ${file_info_cfg_temp}
		dd if=${file_info_cfg_temp} of=${file_info_cfg} oflag=append conv=notrunc &> /dev/null
		rm -f ${file_info_cfg_temp}
		seek=$[(size+align_base-1)/align_base*align_base]
	done

	openssl dgst -sha256 -binary ${file_info_cfg} > ${file_info_cfg}.sha256
	cat ${file_info_cfg} >> ${file_info_cfg}.sha256
	#cat ${file_info_cfg}.sha256 >> ${file_info_cfg}
	rm -f ${file_info_cfg}
	mv -f ${file_info_cfg}.sha256 ${file_info_cfg}

	dd if=${file_info_cfg} of=${bootloader} bs=512 seek=508 conv=notrunc status=none

	if [ ${storage_type_suffix} == ".sto" ]; then
		echo "Image SDCARD"
		total_size=$[total_size+512]
		rm -f ${sdcard_image}
		dd if=/dev/zero of=${sdcard_image} bs=${total_size} count=1 status=none
		dd if=${file_info_cfg}   of=${sdcard_image} conv=notrunc status=none
		dd if=${bootloader} of=${sdcard_image} bs=512 seek=1 conv=notrunc status=none

		mv ${bootloader} ${output_images}/u-boot.bin${postfix}
	fi

	rm -f ${file_info_cfg}
}

echo ---- repackage devices signed uboot ----
if [ -n ${chipset_variant} ]; then
	./fip/sc2/bin/device-vendor-scs-signing.sh --key-dir ${key_dir} --project ${projects} --input-package ${input_dir}/${board}-uboot-nocs.zip --out-dir ${input_dir} --chipset-variant ${chipset_variant}
else
	./fip/sc2/bin/device-vendor-scs-signing.sh --key-dir ${key_dir} --project ${projects} --input-package ${input_dir}/${board}-uboot-nocs.zip --out-dir ${input_dir}
fi

echo ---- repackage chipset signed uboot ----

if [ ! -f ${BUILD_PATH}/device-fip-header.bin ]; then
	echo "Warning: local device fip header templates"
	cp soc/templates/sc2/${projects}/device-fip-header.bin ${work_dir}
fi

./fip/sc2/bin/gen-bl.sh ${work_dir} ${work_dir} ${work_dir} ${work_dir} ${chipset_variant}
postfix=.signed
mk_uboot ${work_dir} ${work_dir} ${postfix} .sto ${chipset_variant}
mk_uboot ${work_dir} ${work_dir} ${postfix} .usb ${chipset_variant}

mv ${work_dir}/u-boot.bin.sd.bin.signed ${input_dir}/u-boot.bin.sd.bin_repackage.signed
mv ${work_dir}/u-boot.bin.signed ${input_dir}/u-boot.bin_repackage.signed
mv ${work_dir}/u-boot.bin.usb.signed ${input_dir}/u-boot.bin.usb_repackage.signed
