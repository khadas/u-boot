#!/bin/bash

function build_bl30() {
	# $1: src_folder, $2: bin_folder, $3: soc
	echo -n "Build $3 bl30 of ${BL30_SELECT}...Please wait..."
	local tartget
	if [ "${CONFIG_RTOS_SDK_ENABLE}" == "1" ]; then
		echo "Build bl30 from new RTOS SDK."
		local current_dir=$(cd $(dirname $0); pwd)
		local work_dir=${current_dir}/bl30/rtos_sdk
		local output_dir
		cd $work_dir && source scripts/env.sh riscv $CONFIG_SOC_NAME $CONFIG_BOARD_PACKAGE_NAME aocpu && make distclean &&  make REPO_DIR=$REPO_DIR \
			&& if [ -d ${work_dir}/docs ]; then make docs REPO_DIR=$REPO_DIR ; else echo "No docs repo found. Do not generate docs"; fi  &&    \
			output_dir=$work_dir/output/riscv-$CONFIG_BOARD_PACKAGE_NAME-$PRODUCT/freertos
		target=$output_dir/bl30.bin
		cp -arf $output_dir/freertos.bin $output_dir/bl30.bin
	else
		# co-work with bl30 build script under bl30.git
		target="$1/bl30.bin"
		cd $1
		#export CROSS_COMPILE=${AARCH32_TOOL_CHAIN}
		local soc=$3
		if [ "$soc" == "gxtvbb" ]; then
			soc="gxtvb"
		fi
		#make clean BOARD=$soc &> /dev/null
		#make BOARD=$soc &> /dev/null
		if [[ "${BL30_SELECT}" != "" ]]; then
			/bin/bash mk ${BL30_SELECT}
		else
			/bin/bash mk $soc
		fi
	fi
	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl30 failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	echo "done"
	return
}
