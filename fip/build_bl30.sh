#!/bin/bash

function build_bl30() {
	# $1: src_folder, $2: bin_folder, $3: soc
	# co-work with bl30 build script under bl30.git
	echo -n "Build $3 bl30...Please wait..."
	local target="$1/bl30.bin"
	cd $1
	#export CROSS_COMPILE=${AARCH32_TOOL_CHAIN}
	local soc=$3
	if [ "$soc" == "gxtvbb" ]; then
		soc="gxtvb"
	fi
	#make clean BOARD=$soc &> /dev/null
	#make BOARD=$soc &> /dev/null
	/bin/bash mk $soc
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