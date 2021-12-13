#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.0

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

trace ()
{
	echo ">>> $@" > /dev/null
	#echo ">>> $@"
}

check_dir() {
    if [ ! -d "$1" ]; then echo "Error: directory \""$1"\" does NOT exist"; usage ; fi
}

check_value() {
	local val=$1
	local begin=$2
	local end=$3

	if [ $val -lt $begin ] || [ $val -gt $end ]; then
		echo "Error: Value $val is not in range [$begin, $end]"
		exit 1
	fi
}

function mk_uboot() {
	output_images=$1
	input_payloads=$2
	postfix=$3
	storage_type_suffix=$4
	chipset_variant_suffix=$5

	device_fip="${input_payloads}/device-fip.bin${postfix}"
	bb1st="${input_payloads}/bb1st${storage_type_suffix}${chipset_variant_suffix}.bin${postfix}"
	bl2e="${input_payloads}/blob-bl2e${storage_type_suffix}${chipset_variant_suffix}.bin${postfix}"
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
	ddr_fip="${input_payloads}/blob-ddr-fip.bin.signed "
	if [ ! -f ${ddr_fip} ]; then
		dd if=/dev/zero of=${ddr_fip} bs=1024 count=256 status=none
	fi

	#cat those together with 4K upper aligned for sdcard
	align_base=4096
	total_size=0
	for file in ${bb1st} ${bl2e} ${bl2x} ${ddr_fip} ${device_fip}; do
		size=`stat -c "%s" ${file}`
		upper=$[(size+align_base-1)/align_base*align_base]
		total_size=$[total_size+upper]
		#echo ${file} ${size} ${upper}
	done

	echo ${bootloader} ${total_size}
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
		#echo ${file} ${seek_sector} ${size_sector}
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
		total_size=$[total_size+512]
		echo ${sdcard_image} ${total_size}
		rm -f ${sdcard_image}
		dd if=/dev/zero of=${sdcard_image} bs=${total_size} count=1 status=none
		dd if=${file_info_cfg}   of=${sdcard_image} conv=notrunc status=none
		dd if=${bootloader} of=${sdcard_image} bs=512 seek=1 conv=notrunc status=none

		mv ${bootloader} ${output_images}/u-boot.bin${postfix}
	fi

	rm -f ${file_info_cfg}
}

usage() {
    cat << EOF
Usage: $(basename $0) --help | --version

       Amlogic SC2 Device Vendor Secure Chipset Startup (SCS) Signing

       $(basename $0)
		--key-dir <key-dir> \\
		--project <project-name> \\
		--input-dir  <input-dir> \\
		{--input-package  <input-package>} \\
		{--rootkey-index [0 | 1 | 2 | 3]} \\
		{--chipset-variant <chipset-variant>} \\
		{--arb-config <arb-config-file>} \\
		--out-dir <output-dir>
EOF
    exit 1
}

key_dir=""
part=""
input_dir=""
input_package=""
rootkey_index=0
chipset_variant=""
arb_config=""
output_dir=""

parse_main() {
    local i=0
    local argv=()
    for arg in "$@" ; do
        argv[$i]="$arg"
        i=$((i + 1))
    done

    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -h|--help)
                usage
                break
		;;
            -v|--version)
                echo "Version $version";
		exit 0
		;;
            --key-dir)
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --project)
                part="${argv[$i]}"
		;;
            --input-dir)
                input_dir="${argv[$i]}"
		check_dir "${input_dir}"
		;;
            --input-package)
                input_package="${argv[$i]}"
		;;
            --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value "${rootkey_index}" 0 3
		;;
            --chipset-variant)
                chipset_variant="${argv[$i]}"
		;;
            --arb-config)
                arb_config="${argv[$i]}"
		;;
            --out-dir)
                output_dir="${argv[$i]}"
                check_dir "${output_dir}"
		;;
            *)
                echo "Unknown option $arg";
		usage
                ;;
        esac
        i=$((i + 1))
    done
}

parse_main "$@"

trace "  key-dir ${key_dir}"
trace "  project ${part}"
trace "  input_dir ${input_dir}"
trace "  input_package ${input_package}"
trace "  rootkey-index ${rootkey_index}"
trace "  chipset-variant ${chipset_variant}"
trace "  arb-config ${arb_config}"
trace "  out-dir ${output_dir}"

if [ -z "${key_dir}" ]; then
	usage
fi

if [ -z "${part}" ]; then
	echo "Error: project cannot be empty"
	usage
fi

if [ -z "${input_dir}" ] && [[ ! -f ${input_package} ]]; then
    echo "Error: input package \""${input_package}"\" does NOT exist"
	usage
fi

if [[ -f ${input_package} ]]; then
	temp_dir="$input_package"-`date +%Y%m%d-%H%M%S`
	if [[ -d ${input_dir} ]]; then
		echo "error!input package and input dir conflicts! Only one set is legal!"
		exit 1;
	else
		input_dir=${temp_dir}
	fi
	mkdir -p ${temp_dir}
	if [[ -d ${temp_dir} ]]; then
		unzip ${input_package} -d ${temp_dir} >& /dev/null
	fi
fi

if [ -z "${rootkey_index}" ]; then
	rootkey_index=0
fi

if [ -z "${chipset_variant}" ] || [ "${chipset_variant}" == "no_variant" ]; then
	chipset_variant_suffix=""
else
	chipset_variant_suffix=".${chipset_variant}"
fi

if [ -z "${output_dir}" ]; then
	usage
fi

fw_arb_cfg=${arb_config}
if [ -s "${fw_arb_cfg}" ]; then
	source ${fw_arb_cfg}
	export DEVICE_SCS_SEGID=${DEVICE_SCS_SEGID}
	export DEVICE_VENDOR_SEGID=${DEVICE_VENDOR_SEGID}
	export DEVICE_SCS_VERS=${DEVICE_SCS_VERS}
	export DEVICE_TEE_VERS=${DEVICE_TEE_VERS}
	export DEVICE_REE_VERS=${DEVICE_REE_VERS}
fi

#export DEVICE_SCS_KEY_TOP=$(pwd)/${key_dir}
#export DEVICE_INPUT_PATH=$(pwd)/${input_dir}
#export DEVICE_OUTPUT_PATH=$(pwd)/${input_dir}
export DEVICE_SCS_KEY_TOP=$(readlink -f ${key_dir})
export DEVICE_INPUT_PATH=$(readlink -f ${input_dir})
export DEVICE_OUTPUT_PATH=$(readlink -f ${input_dir})
export PROJECT=${part}
export DEVICE_ROOTRSA_INDEX=${rootkey_index}

export DEVICE_VARIANT_SUFFIX=${chipset_variant_suffix}

echo sign for blob-ddr-fip.bin.signed
make -C ${BASEDIR_TOP} dv-ddr-fip
export DEVICE_STORAGE_SUFFIX=.sto
make -C ${BASEDIR_TOP} dv-boot-blobs
export DEVICE_STORAGE_SUFFIX=.usb
make -C ${BASEDIR_TOP} dv-boot-blobs

make -C ${BASEDIR_TOP} dv-device-fip
postfix=.device.signed

# build final bootloader
mk_uboot ${output_dir} ${input_dir} ${postfix} .sto ${chipset_variant_suffix}
mk_uboot ${output_dir} ${input_dir} ${postfix} .usb ${chipset_variant_suffix}

if [ -d ${temp_dir} ]; then
	rm -rf ${temp_dir}
fi
