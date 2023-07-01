#!/bin/bash

function select_uboot() {
	local cfg_name=$1

	cd ${MAIN_FOLDER}

	for file in `ls -d ${BL33_DEFCFG1}/* ${BL33_DEFCFG2}/* ${BL33_DEFCFG3}/*`; do
		temp_file=`basename $file`
		#echo $temp_file
		temp_file=${temp_file%_*}
		if [ "$cfg_name" == "$temp_file" ]; then
			if [ "1" == "${CONFIG_CHOICE_BUILD}" ]; then
				if [ "2015" == "${CONFIG_BUILD_VERSION}" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION1}
				elif [ "2019" == "${CONFIG_BUILD_VERSION}" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION2}
				elif [ "2023" == "${CONFIG_BUILD_VERSION}" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION3}
				else
					echo "input parameter error"
					exit 1
				fi
			else
				if [ "${BL33_DEFCFG1}" == "$(dirname $file)" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION1}
				elif [ "${BL33_DEFCFG2}" == "$(dirname $file)" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION2}
				elif [ "${BL33_DEFCFG3}" == "$(dirname $file)" ]; then
					bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION3}
				else
					echo "not found defconfig"
					exit 1
				fi
			fi
			echo "select bl33: ${bl33_path}"
			BL33_BUILD_FOLDER=${bl33_path}/build/
			SOURCE_FILE=("${BL33_BUILD_FOLDER}.config")
			CONFIG_FILE=("${BL33_BUILD_FOLDER}include/autoconf.mk")
			UBOOT_SRC_FOLDER=${bl33_path}
			break
		fi
	done
	export BL33_BUILD_FOLDER SOURCE_FILE CONFIG_FILE UBOOT_SRC_FOLDER
}

function pre_build_uboot() {
	select_uboot $1
	cd ${UBOOT_SRC_FOLDER}
	echo -n "Compile config: "
	echo "$1"
	SOCNAME=$1
	echo "SOCNAME:${SOCNAME}"
	make distclean # &> /dev/null
	make $1'_config' # &> /dev/null
	if [ $? != 0 ]
	then
		echo "Pre-build failed! exit!"
		cd ${MAIN_FOLDER}
		exit -1
	fi
	cd ${MAIN_FOLDER}
}

function build_uboot() {
	echo "Build uboot...Please Wait...$1...$2...$3...$4...$5...$6...$7"
	mkdir -p ${FIP_BUILD_FOLDER}
	cd ${UBOOT_SRC_FOLDER}
	if [[ "${SCRIPT_ARG_CHIPSET_VARIANT}" =~ "nocs" ]] || [[ "${CONFIG_CHIPSET_VARIANT}" =~ "nocs" ]]; then
		CONFIG_CHIP_NOCS=1
		echo "### ${CONFIG_CHIP_NOCS} ###"
	fi
	set -e
	if [ "${CONFIG_MDUMP_COMPRESS}" = "1" ]; then
		CONFIG_MDUMP_COMPRESS=1
		echo "### BL33 CONFIG_MDUMP_COMPRESS = 1 ###"
		make -j SYSTEMMODE=$1 AVBMODE=$2 BOOTCTRLMODE=$3 FASTBOOTMODE=$4 AVB2RECOVERY=$5 TESTKEY=$6 GPTMODE=$7 CHIPMODE=${CONFIG_CHIP_NOCS} \
			CONFIG_MDUMP_COMPRESS=${CONFIG_MDUMP_COMPRESS} # &> /dev/null
	else
		echo "### BL33 CONFIG_MDUMP_COMPRESS = 0 ###"
		make -j SYSTEMMODE=$1 AVBMODE=$2 BOOTCTRLMODE=$3 FASTBOOTMODE=$4 AVB2RECOVERY=$5 TESTKEY=$6 GPTMODE=$7 CHIPMODE=${CONFIG_CHIP_NOCS} # &> /dev/null
	fi
	set +e

	if [ "y" == "${CONFIG_AB_UPDATE}" ]; then
		cp ./build/board/amlogic/${SOCNAME}/firmware/acs.bin ../../${BUILD_PATH}/acs.bin -f
		fip_bl33_size=`stat -c %s ./build/u-boot.bin`
		dd if=/dev/zero of=./build/fip.tmp bs=2097120 count=1
		dd if=./build/u-boot.bin of=./build/fip.tmp bs=${fip_bl33_size} count=1 conv=notrunc

		# sha fip ab
		openssl dgst -sha256 -binary ./build/fip.tmp > ./build/u-boot.bin.sha256
		dd if=./build/u-boot.bin.sha256 of=./build/fip.tmp bs=1 count=32 oflag=append conv=notrunc
		cp ./build/fip.tmp ../../build/fip-bl33.bin -f

		# sign fip ab
		../../fip/stool/signing-tool-c1/sign-boot-c1.sh --sign-kernel -i ./build/fip.tmp \
		-k ./board/amlogic/common/ab_mode/aml-key/kernelkey.pem -a ./board/amlogic/common/ab_mode/aml-key/kernelaeskey \
		--iv ./board/amlogic/common/ab_mode/aml-key/kernelaesiv -o ./build/fip-bl33.bin.encrypt
		cp ./build/fip-bl33.bin.encrypt ../../build/ -f
		#cp ./build/fip-bl33.bin.encrypt ../../${BUILD_PATH}/fip-bl33.bin -f

		# build pre bl33
		set -e
		make distclean
		make ${SOCNAME}_defconfig
		make -j SYSTEMMODE=$1 AVBMODE=$2 BOOTCTRLMODE=$3 FASTBOOTMODE=$4 AVB2RECOVERY=$5 TESTKEY=$6 CHIPMODE=${CONFIG_CHIP_NOCS} \
			ABUPDATE=${CONFIG_AB_UPDATE} # &> /dev/null
		set +e
		if [ $? != 0 ]; then
			echo "Pre bl33 failed! exit!"
			cd ${MAIN_FOLDER}
			exit 1
		fi
		cp ./build/u-boot.bin ../../${FIP_BUILD_FOLDER}bl33.bin
	fi

	SOC_GROUP=`echo ${SOCNAME} | cut -d '_' -f 1`
	skiped=("a1" "c1" "c2" "c3" "g12a" "g12b" "sm1" "t5w")
	if [[ "${skiped[@]}"  =~ "${SOC_GROUP}" ]]; then
		echo ""
		echo "The soc(${SOC_GROUP}) does not support bl33z, skip."
		echo ""
	elif [ "${CONFIG_SUPPORT_BL33Z}" = "1" ]; then
		echo ""
		set -e
		echo "ramdump enable, build bl33z.bin for soc [${SOCNAME}] ..."
		if [ -f "./bl33z/Makefile" ]; then
			set -e
			make -C bl33z/ PLAT=${SOCNAME} AARCH=aarch64 distclean
			make -C bl33z/ PLAT=${SOCNAME} AARCH=aarch64
			set +e
			if [ -f "./bl33z/build/bl33z.bin" -a -f "./build/u-boot.bin" ]; then
				# place bl33z at end of u-boot.bin, _end align(4096)
				END_LENS=`ls -l ./build/u-boot.bin | awk '{print $5}'`
				END_ALIGN=4096
				BL33Z_LOAD=`echo "((($END_LENS-1) / $END_ALIGN * $END_ALIGN) + $END_ALIGN)" | bc`
				echo "uboot.bin size:$END_LENS, align:$END_ALIGN, new uboot size:$BL33Z_LOAD"

				dd if=/dev/zero of=u-boot.tmp bs=$BL33Z_LOAD count=1
				dd if=./build/u-boot.bin of=u-boot.tmp  conv=notrunc  &> /dev/null
				cat ./bl33z/build/bl33z.bin >> u-boot.tmp
				cp -rf u-boot.tmp build/u-boot.bin
				rm -rf u-boot.tmp
				echo "Append bl33z.bin to the end of uboot.bin OK."
			else
				echo "Error: build bl33z.bin failed... abort"
			fi
		else
			echo "Error: bl33z proj is not exist... abort"
		fi
		set +e
		echo ""
	fi
	ret=$?
	cd ${MAIN_FOLDER}
	if [ 0 -ne $ret ]; then
		echo "Error: U-boot build failed... abort"
		exit -1
	fi
}

function uboot_config_list() {
	echo "      ******Amlogic Configs******"
	for file in `ls -d ${BL33_DEFCFG1}/* ${BL33_DEFCFG2}/* ${BL33_DEFCFG3}/*`; do
		temp_file=`basename $file`
		#echo "$temp_file"
		temp_file=${temp_file%_*}
		echo "          ${temp_file}"
	done

	customer_folder1="${BL33_PATH1}/customer/board/defconfigs"
	customer_folder2="${BL33_PATH2}/customer/board/defconfigs"
	if [ -e ${customer_folder1} ]; then
		echo "      ******Customer Configs******"
		for file in ${customer_folder1}/*; do
			temp_file=`basename $file`
			temp_file=${temp_file%_*}
			echo "          ${temp_file}"
		done
	fi
	if [ -e ${customer_folder2} ]; then
		for file in ${customer_folder2}/*; do
			temp_file=`basename $file`
			temp_file=${temp_file%_*}
			echo "          ${temp_file}"
		done
	fi

	echo "      ***************************"
}

function copy_bl33() {
	if [ "y" != "${CONFIG_AB_UPDATE}" ]; then
		cp ${BL33_BUILD_FOLDER}/u-boot.bin ${FIP_BUILD_FOLDER}bl33.bin -f
	fi

	# remove src link to prevent android "File system loop detected" issue
	#cd ${UBOOT_SRC_FOLDER}/build/
	#rm source
	cd ${MAIN_FOLDER}
}
