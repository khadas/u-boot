#!/bin/bash

DEBUG_PRINT=0

declare GIT_OPERATE_INFO=""

function dbg() {
	if [ 0 != ${DEBUG_PRINT} ]; then
		echo "$1"
	fi
}

declare str_use=""
# filter means get useful information
function string_filter() {
	# #1 origin str, #2 filter str, #3 split char, #4 which section
	local str_origin=$1
	local str_filter=$2
	local str_split=$3
	str_origin=${str_origin#*${str_filter}} # filter
	IFS=${str_split} read -ra DATA <<< "$str_origin"
	str_use=${DATA[$4]}
}

function get_versions() {
	echo "Get version info"
	declare -a SRC_REV
	declare -a BIN_REV
	# read manifest, get each blx information
	if [ -f $MANIFEST ]; then
		while read -r line || [[ -n $line ]]; do
			string_filter "${line}" "dest-branch=" '"' 1
			GIT_INFO[0]=${str_use}
			string_filter "${line}" "path=" '"' 1
			GIT_INFO[1]=${str_use}
			string_filter "${line}" "revision=" '"' 1
			GIT_INFO[2]=${str_use}
			string_filter "${line}" "name=" '"' 1
			GIT_INFO[3]=${str_use}
			string_filter "${line}" "remote=" '"' 1
			GIT_INFO[4]=${str_use}
			# if this line doesn't contain any info, skip it
			if [ "${GIT_INFO[2]}" == "" ]; then
				continue
			fi
			#echo "${GIT_INFO[0]} ${GIT_INFO[1]} ${GIT_INFO[2]} ${GIT_INFO[3]} ${GIT_INFO[4]}"
			#echo ${BLX_NAME[@]}
			#echo ${BLX_SRC_FOLDER[@]}
			for loop in ${!BLX_NAME[@]}; do
				if [ "${GIT_INFO[1]}" == "${BLX_SRC_FOLDER[$loop]}" ]; then
					SRC_REV[$loop]=${GIT_INFO[2]}
					#CUR_BIN_BRANCH[$loop]=${GIT_INFO[0]}
					echo -n "name:${BLX_NAME[$loop]}, path:${BLX_SRC_FOLDER[$loop]}, "
					if [ "${SRC_REV[$loop]}" == "${GIT_INFO[0]}" ]; then
						# if only specify branch name, not version, use latest binaries under bin/ folders
						# use bin.git revision, in case src code have local commits
						if [ -d ${BLX_BIN_FOLDER[loop]} ]; then
							git_operate ${BLX_BIN_FOLDER[loop]} log --pretty=oneline -1
							git_msg=${GIT_OPERATE_INFO}
						else
							git_msg=""
						fi
						IFS=' ' read -ra DATA <<< "$git_msg"
						SRC_REV[$loop]=${DATA[2]}
						echo -n "revL:${SRC_REV[$loop]} "
					else
						SRC_REV[$loop]=${GIT_INFO[2]}
						echo -n "rev:${SRC_REV[$loop]} "
					fi
					echo "@ ${GIT_INFO[0]}"
				fi
				if [ "${GIT_INFO[1]}" == "${BLX_BIN_FOLDER[$loop]}" ]; then
					BIN_REV[$loop]=${GIT_INFO[2]}
					#CUR_BIN_BRANCH[$loop]=${GIT_INFO[0]}
					echo -n "name:${BLX_NAME[$loop]}, path:${BLX_BIN_FOLDER[$loop]}, "
					if [ "${BIN_REV[$loop]}" == "${GIT_INFO[0]}" ]; then
						# if only specify branch name, not version, use latest binaries under bin/ folders
						git_operate ${BLX_BIN_FOLDER[loop]} log --pretty=oneline -1
					else
						# else get bin->src version
						git_operate ${BLX_BIN_FOLDER[loop]} log ${BIN_REV[$loop]} --pretty=oneline -1
					fi
					git_msg=${GIT_OPERATE_INFO}
					IFS=' ' read -ra DATA <<< "$git_msg"
					BIN_REV[$loop]=${DATA[2]}
					echo -n "revL:${BIN_REV[$loop]} "
					echo "@ ${GIT_INFO[0]}"
				fi
			done
		done < "$MANIFEST"
		# SRC_REV="" means this is a no-src repo, use bin.git
		if [ "" == "${SRC_REV[0]}" ]; then
			dbg "src_rev NULL"
			for loop in ${!BIN_REV[@]}; do
				echo "Manifest: Use bin.git version. ${BLX_BIN_FOLDER[$loop]} - ${BIN_REV[$loop]}"
				CUR_REV[$loop]=${BIN_REV[$loop]}
			done
		else
			dbg "src_rev not NULL"
			for loop in ${!SRC_REV[@]}; do
				dbg "Manifest: src.git version. ${BLX_SRC_FOLDER[$loop]} - ${SRC_REV[$loop]}"
				CUR_REV[$loop]=${SRC_REV[$loop]}
			done
		fi

		# BIN_REV="" means this is a no-bin repo, use src.git
		if [ "" == "${BIN_REV[0]}" ]; then
			for loop in ${!SRC_REV[@]}; do
				echo "Manifest: Src code only. build with --update-${BLX_NAME[$loop]}"
				#CUR_REV[$loop]=${SRC_REV[$loop]}
				update_bin_path $loop "source"
				CONFIG_DDR_FW=1
				export CONFIG_DDR_FW
			done
		fi
	else
		for loop in ${!BLX_NAME[@]}; do
			# merge into android/buildroot, can not get manifest.xml, get version by folder
			# loop src folder
			if [ -d ${BLX_SRC_FOLDER[$loop]} ]; then
				echo "No-Manifest: Src code only. build with --update-${BLX_NAME[$loop]}"
				update_bin_path $loop "source"
				CONFIG_DDR_FW=1
				export CONFIG_DDR_FW
			fi
		done
		# loop bin folder. (this will overwrite src version if both exist)
		for loop in ${!BLX_NAME[@]}; do
			if [ -d ${BLX_BIN_FOLDER[$loop]} ]; then
				git_operate ${BLX_BIN_FOLDER[loop]} log --pretty=oneline -1
				git_msg=${GIT_OPERATE_INFO}
				IFS=' ' read -ra DATA <<< "$git_msg"
				CUR_REV[$loop]=${DATA[2]}
				echo -n "revL:${CUR_REV[$loop]} "
				echo "@ ${BLX_BIN_FOLDER[$loop]}"
			fi
		done
	fi
}

function git_operate() {
	# $1: path, $2: other parameters
	GIT_OPERATE_INFO=`git --git-dir $1/.git --work-tree=$1 ${@:2}`
	dbg "${GIT_OPERATE_INFO}"
}

function git_operate2() {
	# use -C. for pull use. don't know why [git_operate pull] doesn't work, server git update?
	# $1: path, $2: other parameters
	GIT_OPERATE_INFO="`git -C \"$1\" ${@:2}`"
	#echo "${GIT_OPERATE_INFO}"
}

function get_blx_bin() {
	# $1: current blx index
	index=$1
	git_operate ${BLX_BIN_FOLDER[index]} log --pretty=oneline

	git_msg=${GIT_OPERATE_INFO}
	BLX_READY[${index}]="false"
	mkdir -p ${FIP_BUILD_FOLDER}

	# get version log line by line, compare with target version
	line_num=0
	while read line;
	do
		IFS=' ' read -ra DATA <<< "$line"
		# v1-fix support short-id
		dbg "${CUR_REV[$index]:0:7} - ${DATA[2]:0:7}"
		if [ "${CUR_REV[$index]:0:7}" == "${DATA[2]:0:7}" ]; then
			BLX_READY[${index}]="true"
			dbg "blxbin:${DATA[0]} blxsrc:  ${DATA[2]}"
			dbg "blxbin:${DATA[0]} blxsrc-s:${DATA[2]:0:7}"
			# reset to history version
			#git --git-dir ${BLX_BIN_FOLDER[index]}/.git --work-tree=${BLX_BIN_FOLDER[index]} reset ${DATA[0]} --hard
			#git_operate2 ${BLX_BIN_FOLDER[index]} reset ${DATA[0]} --hard
			# copy binary file
			if [ "bl32" == "${BLX_NAME[$index]}" ]; then
				# bl32 is optional
				if [ "y" == "${CONFIG_NEED_BL32}" ]; then
					cp ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/${BLX_BIN_NAME[index]} ${FIP_BUILD_FOLDER} -f
					if [ "y" == "${CONFIG_FIP_IMG_SUPPORT}" ]; then
						cp ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/${BLX_IMG_NAME[index]} ${FIP_BUILD_FOLDER} 2>/dev/null
					fi
				fi
			else
				cp ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/${BLX_BIN_NAME[index]} ${FIP_BUILD_FOLDER} -f
				if [ "y" == "${CONFIG_FIP_IMG_SUPPORT}" ]; then
					cp ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/${BLX_IMG_NAME[index]} ${FIP_BUILD_FOLDER} 2>/dev/null
				fi
				if [ -e ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/bl2.v3.bin ]; then
					cp ${BLX_BIN_FOLDER[index]}/${CUR_SOC}/bl2.v3.bin ${FIP_BUILD_FOLDER} -f
				fi
			fi
			# undo reset
			if [ 0 -ne ${line_num} ]; then
				# this is not latest version, can do reset. latest version doesn't have 'git reflog'
				#git --git-dir ${BLX_BIN_FOLDER[index]}/.git --work-tree=${BLX_BIN_FOLDER[index]} reset 'HEAD@{1}' --hard
				git_operate2 ${BLX_BIN_FOLDER[index]} reset 'HEAD@{1}' --hard
			fi
			break
		fi
		line_num=$((line_num+1))
	done <<< "${git_msg}"
	if [ "true" == ${BLX_READY[${index}]} ]; then
		echo "Get ${BLX_NAME[$index]} from ${BLX_BIN_FOLDER[$index]}... done"
	else
		echo -n "Get ${BLX_NAME[$index]} from ${BLX_BIN_FOLDER[$index]}... failed"
		if [ "true" == ${BLX_NEEDFUL[$index]} ]; then
			echo "... abort"
			exit -1
		else
			echo ""
		fi
	fi
	return 0;
}

function prepare_tools() {
	echo "*****Compile tools*****"

	mkdir -p ${FIP_BUILD_FOLDER}

	if [ "${CONFIG_DDR_PARSE}" == "1" ]; then
		if [ -d ${FIP_DDR_PARSE} ]; then
			cd ${FIP_DDR_PARSE}
			make clean; make
			cd ${MAIN_FOLDER}

			mv -f ${FIP_DDR_PARSE}/parse ${FIP_BUILD_FOLDER}
		fi

		if [ ! -x ${FIP_BUILD_FOLDER}/parse ]; then
			echo "Error: no ddr_parse... abort"
			exit -1
		fi
	fi

}
