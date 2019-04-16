#!/bin/bash


function pre_build_uboot() {
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
	folder_board="${UBOOT_SRC_FOLDER}/board/amlogic"
	echo "      ******Amlogic Configs******"
	for file in ${folder_board}/*; do
		temp_file=`basename $file`
		#echo "$temp_file"
		if [ -d ${folder_board}/${temp_file} ] && [ "$temp_file" != "defconfigs" ] && [ "$temp_file" != "configs" ];then
			echo "          ${temp_file}"
		fi
	done

	customer_folder="${UBOOT_SRC_FOLDER}/customer/board"
	if [ -e ${customer_folder} ]; then
		echo "      ******Customer Configs******"
		for file in ${customer_folder}/*; do
			temp_file=`basename $file`
			if [ -d ${customer_folder}/${temp_file} ] && [ "$temp_file" != "defconfigs" ] && [ "$temp_file" != "configs" ];then
				echo "          ${temp_file}"
			fi
		done
	fi
	echo "      ***************************"
}

function copy_bl33() {
	cp ${UBOOT_SRC_FOLDER}/build/u-boot.bin ${FIP_BUILD_FOLDER}bl33.bin -f

	# remove src link to prevent android "File system loop detected" issue
	cd ${UBOOT_SRC_FOLDER}/build/
	rm source
	cd ${MAIN_FOLDER}
}