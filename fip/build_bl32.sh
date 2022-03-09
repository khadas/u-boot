#!/bin/bash

function build_bl32() {
	echo -n "Build bl32...Please wait... "
	local target="$1/bl32.img"
	# $1: src_folder, $2: bin_folder, $3: soc
	cd $1
	/bin/bash build.sh $3
	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl32 failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	echo "done"
	return
}
