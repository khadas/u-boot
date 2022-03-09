#!/bin/bash

function select_uboot() {
	local cfg_name=$1

	cd ${MAIN_FOLDER}

	for file in `ls -d ${BL33_DEFCFG1}/* ${BL33_DEFCFG2}/*`; do
		temp_file=`basename $file`
		#echo $temp_file
		temp_file=${temp_file%_*}
		if [ "$cfg_name" == "$temp_file" ]; then
			if [ "${BL33_DEFCFG1}" == "$(dirname $file)" ]; then
				bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION1}
				export CROSS_COMPILE=${BL33_TOOLCHAIN1}
			else
				bl33_path=${UBOOT_FOLDER}/${UBOOT_VERSION2}
				export CROSS_COMPILE=${BL33_TOOLCHAIN2}
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
	echo "Build uboot...Please Wait...$1...$2..."
	mkdir -p ${FIP_BUILD_FOLDER}
	cd ${UBOOT_SRC_FOLDER}
	make -j SYSTEMMODE=$1 AVBMODE=$2 # &> /dev/null
	ret=$?
	cd ${MAIN_FOLDER}
	if [ 0 -ne $ret ]; then
		echo "Error: U-boot build failed... abort"
		exit -1
	fi
}

function uboot_config_list() {
	echo "      ******Amlogic Configs******"
	for file in `ls -d ${BL33_DEFCFG1}/* ${BL33_DEFCFG2}/*`; do
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
