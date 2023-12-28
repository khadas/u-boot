#!/bin/bash

function build_bl40() {
	echo -n "Build bl40...Please wait... "
	# $1: src_folder, $2: bin_folder, $3: soc
	cd $1
	# todo
	cd ${MAIN_FOLDER}
	echo "done"
	return
}
