#!/bin/bash

function get_branch() {
	local oldifs="$IFS"
	local find_base='0'
	local base_branch=""
	local tmp=""
	IFS=$'\n'

	tmp=`git branch`
	# Eg: * (HEAD detached at firmware/projects/sc2)
	if [[ "${tmp}" =~ "HEAD detached " ]]; then
		base_branch=${tmp%)*}
		base_branch=${base_branch##* }
		CURRENT_BL2_BRANCH=${base_branch}
		export CURRENT_BL2_BRANCH
		return
	fi

	# No valid information from 'git branch' command
	for line1 in `git show-branch --current --more=5`; do
		logmsg=${line1#* }
		for line2 in `git branch -l -v -a`; do
			tmp=${line2# }
			# Eg: * (HEAD detached from 65a3a6e)
			if [[ "${tmp}" =~ "HEAD detached from" ]]; then
				continue
			fi
			# Eg:   remotes/firmware/master  e1d4a6f Inital commit
			if [[ "${tmp}" =~ "${logmsg}" ]]; then
				base_branch=${tmp#* }
				base_branch=${base_branch%% *}
				find_base=1
				break
			fi
		done
		if [ ${find_base} == 1 ]; then
			break
		fi
	done
	IFS="$oldifs"
	CURRENT_BL2_BRANCH=${base_branch}
	export CURRENT_BL2_BRANCH
	return
}

function check_branch() {
	local dest_branch="projects/$1"
	local str=`git branch --remote | grep ${dest_branch}`

	if [ "" != "${BL2_BRANCH_ARG}" ]; then
		echo ==== BL2 BRANCH ${BL2_BRANCH_ARG} SPECIFIED ====
		dest_branch="projects/${BL2_BRANCH_ARG}"
	fi
	# 1, check if existed amlogic git branch name format
	if [ "${str}" == "" ]; then
		echo "can't find ${dest_branch}"
	else
		local cur_branch=''
		local diff=`git diff`

		# 2, check current branch is based on target soc?
		get_branch
		echo ==== current BL2 branch:${CURRENT_BL2_BRANCH} ====
		if [[ "${CURRENT_BL2_BRANCH}" == *"${dest_branch}" ]]; then
			echo ==== NO NEED TO SWITCH BRANCH ====
			return
		fi

		# 3, if contains uncommitted local changes, stop compile
		#    before branch switch, user must save their local
		#    change.
		if [ "${diff}" == "" ]; then
			echo ==== switch branch to ${dest_branch} ====
			git checkout "remotes/firmware/$dest_branch"
		else
			echo ==== bl2/core/ code is not clean!!! ====
			echo ==== Please save your change first! ====
			exit -1
		fi
	fi
}

function build_bl2() {
	echo -n "Build bl2...Please wait..."
	local target="$1/bl2.bin"
	local targetv3="$1/bl2.v3.bin"
	# $1: src_folder, $2: bin_folder, $3: soc

	cd $1
	if [ "$ADVANCED_BOOTLOADER" == "1" ]; then
		check_branch $3
		/bin/bash mk $3 --ddrtype ${CONFIG_DDRFW_TYPE} --dsto
		/bin/bash mk $3 --ddrtype ${CONFIG_DDRFW_TYPE} --dusb
		target="$1/bl2.bin*"
		targetv3="$1/chip_acs.bin"
		targetvd="$1/ddr_param.bin"
	else
		/bin/bash mk $3
	fi
	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl2 failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	if [ -e ${targetv3} ]; then
		cp ${targetv3} $2 -f
	fi
	if [ -e ${targetvd} ]; then
		cp ${targetvd} $2 -f
	fi
	echo "...done"
	return
}

function build_bl2e() {
	echo -n "Build bl2e...Please wait..."
	local target="$1/bl2e.bin*"

	# $1: src_folder, $2: bin_folder, $3: soc
	cd $1
	/bin/bash mk $3

	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl2 failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	echo "...done"
	return
}

function build_bl2x() {
	echo -n "Build bl2...Please wait..."
	local target="$1/bl2x.bin"

	# $1: src_folder, $2: bin_folder, $3: soc
	cd $1
	/bin/bash mk $3

	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl2x failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	echo "...done"
	return
}
