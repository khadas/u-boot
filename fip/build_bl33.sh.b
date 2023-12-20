#!/bin/bash

function select_uboot() {
	local cfg_name=$1

	cd ${MAIN_FOLDER}

	bl33_path=$2
	BL33_BUILD_FOLDER=$2/build/
	SOURCE_FILE=("${BL33_BUILD_FOLDER}.config")
	CONFIG_FILE=("${BL33_BUILD_FOLDER}include/autoconf.mk")
	UBOOT_SRC_FOLDER=${bl33_path}

	export BL33_BUILD_FOLDER SOURCE_FILE CONFIG_FILE UBOOT_SRC_FOLDER
}

function pre_build_uboot() {
	select_uboot $1 $2
	cd ${UBOOT_SRC_FOLDER}
	echo -n "Compile config: "
	echo "$1"
	SOCNAME=$1
	echo "SOCNAME:${SOCNAME}"
	pwd
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
	echo "Build uboot...Please Wait...$1...$2...$3...$4...$5...$6"
	mkdir -p ${FIP_BUILD_FOLDER}
	echo UBOOT_SRC_FOLDER:$UBOOT_SRC_FOLDER
	cd ${UBOOT_SRC_FOLDER}
	if [[ "${SCRIPT_ARG_CHIPSET_VARIANT}" =~ "nocs" ]] || [[ "${CONFIG_CHIPSET_VARIANT}" =~ "nocs" ]]; then
		CONFIG_CHIP_NOCS=1
		echo "### ${CONFIG_CHIP_NOCS} ###"
	fi
	if [ "${CONFIG_MDUMP_COMPRESS}" = "1" ]; then
		CONFIG_MDUMP_COMPRESS=1
		echo "### BL33 CONFIG_MDUMP_COMPRESS = 1 ###"
		make -j SYSTEMMODE=$1 AVBMODE=$2 BOOTCTRLMODE=$3 FASTBOOTMODE=$4 AVB2RECOVERY=$5 TESTKEY=$6 CHIPMODE=${CONFIG_CHIP_NOCS} \
			CONFIG_MDUMP_COMPRESS=${CONFIG_MDUMP_COMPRESS} # &> /dev/null
	else
		echo "### BL33 CONFIG_MDUMP_COMPRESS = 0 ###"
		make -j SYSTEMMODE=$1 AVBMODE=$2 BOOTCTRLMODE=$3 FASTBOOTMODE=$4 AVB2RECOVERY=$5 TESTKEY=$6 CHIPMODE=${CONFIG_CHIP_NOCS} # &> /dev/null
	fi
	set +e

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
			make -C bl33z/ PLAT=${SOCNAME} AARCH=aarch64 distclean
			make -C bl33z/ PLAT=${SOCNAME} AARCH=aarch64
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
	cp ${BL33_BUILD_FOLDER}/u-boot.bin ${FIP_BUILD_FOLDER}bl33.bin -f

	# remove src link to prevent android "File system loop detected" issue
	#cd ${UBOOT_SRC_FOLDER}/build/
	#rm source
	cd ${MAIN_FOLDER}
}
