#!/bin/bash

#  =============================================================================
#  @module:     BootLoader stable branch tools: checkout, pull, push ...
#  @author:     Li Dongqing  (dongqing.li@amlogic.com)
#  @License:    Copyright (c) 2019 Amlogic, Inc. All rights reserved.
#  @Changes:
#               1. 2022.05.25 v0.1 - Creat stable sub version update tools.
#               2. 2022.07.13 v0.2 - Add option for s4 branch in bl2/core.
#               3. 2022.09.18 v0.3 - Support submitting uboot.bin to Android dir.
#  =============================================================================

#
# Settings
#

#set -e

# Set these ENV externally, cmd: export ENV=${ENV}
if [[ -z "${STABLE_MAJOR_VERSION}" ]]; then
	STABLE_MAJOR_VERSION="3.4.0"
else
	echo "External settings env: STABLE_MAJOR_VERSION=${STABLE_MAJOR_VERSION}"
fi

if [[ -z "${ANDROID_ROOT_DIR}" ]]; then
	ANDROID_ROOT_DIR="/mnt/fileroot/${LOGNAME}/workspace/3_android/2_R_5.4"
else
	echo "External settings env: ANDROID_ROOT_DIR=${ANDROID_ROOT_DIR}"
fi

if [[ -z "${ANDROID_TARGET_BRANCH}" ]]; then
	ANDROID_TARGET_BRANCH="s-tv-dev"
else
	echo "External settings env: ANDROID_TARGET_BRANCH=${ANDROID_TARGET_BRANCH}"
fi

# update, show. 1/s:stable;  2/t:trunk
SWITCH_TARGET_BRANCH=0
SHOW_STABLE_VERSION=0
SHOW_LAST_COMMIT=0
STABLE_OPTION_TYPE=0

# modify, push
MODIFY_STABLE_VERSION=0
ADD_COMMIT_MESSAGE=0
PUSH_TOPIC_COMMON=0

# openlinux stable branch
BL2_SC2_BRANCH="projects/openlinux/bl-sc2-${STABLE_MAJOR_VERSION}"
BL2_S4_BRANCH="projects/openlinux/bl-s4-${STABLE_MAJOR_VERSION}"

# push uboot.bin to android dir
declare -a OPENLINUX_BOARD_TYPE=("ohm" "oppen")
declare -a OPENLINUX_BOARD_BRANCH=("${BL2_SC2_BRANCH}" "${BL2_S4_BRANCH}")
declare -a OPENLINUX_BOARD_CONFIG=("sc2_ah212" "s4_ap222")
ANDROID_DEV_DIR="device/amlogic"
ANDROID_DIR_OHM="${ANDROID_ROOT_DIR}/${ANDROID_DEV_DIR}/${OPENLINUX_BOARD_TYPE[0]}"
ANDROID_DIR_OPPEN="${ANDROID_ROOT_DIR}/${ANDROID_DEV_DIR}/${OPENLINUX_BOARD_TYPE[1]}"

declare -a ANDROID_DIR_LIST=("${ANDROID_DIR_OHM}" "${ANDROID_DIR_OPPEN}")

#
# Array
#
declare -a BLX_NAME=(		\
	"bl2       "			\
	"bl2e      "			\
	"bl2x      "			\
	"bl30      "			\
	"bl31_1.3  "			\
	"bl32_3.8  "			\
	"bl33_v2019"    		\
	"fip       ")

declare -a BLX_PATH=(		\
	"bl2/core"				\
	"bl2/ree"				\
	"bl2/tee"				\
	"bl30/src_ao"			\
	"bl31_1.3/src"			\
	"bl32_3.8/src"			\
	"bl33/v2019"			\
	"fip")

declare -a BLX_COMMIT=(		\
	" "						\
	" "						\
	" "						\
	" "						\
	" "						\
	" "						\
	" "						\
	" ")

declare -a BLX_REMOTE=(		\
	"firmware"				\
	"firmware"				\
	"firmware"				\
	"firmware"				\
	"firmware"				\
	"firmware"				\
	"uboot"					\
	"fip")

declare -a BLX_STABLE_BRANCH=(			        \
	"projects/openlinux/bl-sc2-$STABLE_MAJOR_VERSION"	\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION"		\
	"projects/openlinux/bl-$STABLE_MAJOR_VERSION")

declare -a BLX_TRUNK_BRANCH=(	\
	"projects/sc2"				\
	"projects/amlogic-dev"		\
	"projects/amlogic-dev"		\
	"projects/amlogic-dev"		\
	"amlogic-dev-1.3"			\
	"amlogic-dev-3.8.0"			\
	"amlogic-dev-2019"			\
	"amlogic-dev")

declare -a STABLE_VER_FILE=(			\
	"Makefile.gcc"						\
	"Makefile.gcc"						\
	"Makefile.gcc"						\
	"demos/amlogic/makedefs"			\
	"Makefile"							\
	"core/arch/arm/plat-meson/conf.mk"	\
	"Makefile"							\
	"NULL")

declare -a STABLE_VER_INFO=(	\
	" "							\
	" "							\
	" "							\
	" "							\
	" "							\
	" "							\
	" "							\
	" ")

declare -a STABLE_VER_TOPIC=(	\
	"bl2-sc2"					\
	"bl2e"						\
	"bl2x"						\
	"bl30"						\
	"bl31"						\
	"bl32"						\
	"bl33"						\
	"NULL")

#
# function
#
function go_proj() {
	cd $rootdir

	output="./pxp_output_$1/"

	if [ -d ${output} ] ; then
		rm -fr ${output}
	fi
	mkdir ${output}

	echo "go proj [ $1 ] !!!!!!!!!!!"
	echo "./mk $1_pxp --update-bl2 --update-bl2e --update-bl2x --update-bl31"
	echo ""
	sleep 2

	./mk $1_pxp --update-bl2 --update-bl2e --update-bl2x --update-bl31

	if [ -f "./build/u-boot.bin" ]; then
		echo "run mk ok, found ./build/u-boot.bin"
		cp -f ./build/u-boot.bin  ${output}
	else
		echo "run mk error, not find ./build/u-boot.bin"
	fi

	cp -f ./bl2/core/bl2.bin.sto ${output}bl2.bin
	cp -f ./bl2/ree/bl2e.bin ${output}bl2e.bin
	cp -f ./bl2/tee/bl2x.bin ${output}bl2x.bin

	cp -f ./bl31_1.3/src/bl31.bin ${output}bl31.bin
	cp -f ./fip/_tmp/device-fip.bin ${output}device-fip.bin
	cp -f ./bl33/v2019/build/u-boot.bin ${output}bl33.bin

	dd if=${output}bl2.bin of=${output}bl2-u1.bin bs=1 count=86016
	dd if=${output}bl2.bin of=${output}bl2-u2.bin bs=1 skip=86016

	./fip/c1/aml_encrypt_c1 --bin2hex --input ${output}bl2-u1.bin --usize 32 --bsize 262144 --output ${output}bl2-sram-u1.hex
	./fip/c1/aml_encrypt_c1 --bin2hex --input ${output}bl2-u2.bin --usize 32 --bsize 262144 --output ${output}bl2-sram-u2.hex

	rm -f ${output}bl2-u*.bin
	tar -czf pxp_output_$1.tar.gz ${output}

	echo "-------------------------------------------------------------------------"
	echo "all patterns for [$1] Z1 are ready in ${output}"
	echo "-------------------------------------------------------------------------"
	ls -la ${output}
	echo
}

function  choose_whether_to_continue() {
	read -r -p "$1 [y/n] " input

	case $input in
		[yY][eE][sS]|[yY])
			echo "ok, run continue ..."
			;;
		[nN][oO]|[nN])
			echo "ok, exit."
			exit 1
			;;
		*)
			echo "Invalid input..."
			exit 1
			;;
	esac
}

function err_exit()
{
	echo "Error: $1"
	exit 1
}

function sync_code() {
	#echo "begin sync branch: $1/$2"
	if [ -z $2 ]; then
		err_exit "branch($2) error !"
	fi
	git reset --hard
    cnt=`git branch |grep test1 -c`
    if [ $cnt -eq 0 ]; then
        git checkout -b test1
    else
        git checkout test1
    fi
    cnt=`git branch |grep $2 -c`
    if [ ! $cnt -eq 0 ]; then
        git branch -D $2 > /dev/null
    fi
	git checkout -t $1/$2 || err_exit "git checkout -t $1/$2 faild !"
	git fetch --all
	git reset --hard $1/$2
	git pull
	git branch -D test1
	echo
}

function show_setting_commit() {
	echo
	echo "git_show_setting_commit:"
	echo "==============================="
	for((i=0;i<${#BLX_NAME[@]};i++)); do
		echo "# ${BLX_NAME[$i]} : ${BLX_COMMIT[$i]}"
	done
	echo "==============================="
	echo
}

function show_setting_branch() {
	echo
	echo "show_setting_branch:"
	echo "==============================="
	for((i=0;i<${#BLX_NAME[@]};i++)); do
		echo "# ${BLX_NAME[$i]} : ${BLX_BRANCH[$i]}"
	done
	echo "==============================="
	echo
}

function show_stable_branch_version() {
	echo
	echo "show_stable_branch_version:"
	echo "==============================="

	#for i in $(seq 0 ${BLX_NAME[@]}); do
	for((i=0;i<${#BLX_NAME[@]}-1;i++)); do
		if [ -d $rootdir/${BLX_PATH[$i]} ]; then
			cd $rootdir/${BLX_PATH[$i]}
			# stable branch version format: bl-x.y.z;
			# x is major version, y is minor version, z is patch version
			if [ "${STABLE_VER_FILE[$i]}" != "NULL" ]; then
				VER_MAJOR=`grep 'MAJOR_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				VER_MINOR=`grep 'MINOR_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				VER_PATCH=`grep 'PATCH_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				STABLE_VER_INFO[$i]="bl-${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}"
			fi

			CURRENT_BRANCH=`git branch |grep "*"| awk '{print $2}'`
			if [ $i -eq 0 -a "${CURRENT_BRANCH}" = "${BLX_STABLE_BRANCH[0]}" ];then
				git checkout ${BL2_S4_BRANCH} > /dev/null 2>&1
				VER_MAJOR=`grep 'MAJOR_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				VER_MINOR=`grep 'MINOR_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				VER_PATCH=`grep 'PATCH_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				STABLE_VER_EXT="bl-${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}"
				echo "# bl2-s4     : 	${STABLE_VER_EXT}"
				git checkout ${BLX_STABLE_BRANCH[0]} > /dev/null 2>&1

			elif [ $i -eq 0 -a "${CURRENT_BRANCH}" = "${BL2_S4_BRANCH}" ];then
				git checkout ${BLX_STABLE_BRANCH[0]} > /dev/null 2>&1
				VER_MAJOR=`grep 'MAJOR_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				VER_MINOR=`grep 'MINOR_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				VER_PATCH=`grep 'PATCH_VERSION ' ./${STABLE_VER_FILE[0]} |grep '=' |awk '{print $3}'`
				STABLE_VER_EXT="bl-${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}"
				echo "# bl2-sc2    : 	${STABLE_VER_EXT}"
				git checkout ${BL2_S4_BRANCH} > /dev/null 2>&1
			fi
		fi
	done

	for((i=0;i<${#BLX_NAME[@]}-1;i++)); do
		echo "# ${BLX_NAME[$i]} : 	${STABLE_VER_INFO[$i]}"
	done
	echo "==============================="
	echo
}

function modify_stable_branch_version() {
	echo
	echo "modify_stable_branch_version to: bl-$MODIFY_STABLE_VERSION"
	#for i in $(seq 0 ${BLX_NAME[@]}); do
	for((i=0;i<${#BLX_NAME[@]}-1;i++)); do
		if [ -d $rootdir/${BLX_PATH[$i]} ]; then
			cd $rootdir/${BLX_PATH[$i]}
			# stable branch version format: bl-x.y.z;
			# x is major version, y is minor version, z is patch version
			if [ "${STABLE_VER_FILE[$i]}" != "NULL" ]; then
				######################### 1. modify version ############################
				# major
				CURRET_VER_MAJOG=`grep 'MAJOR_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				MODIFY_VER_MAJOR=`echo $MODIFY_STABLE_VERSION | cut  -d  '.'  -f  1`
				sed -i '/MAJOR_VERSION /s/'$CURRET_VER_MAJOG'/'$MODIFY_VER_MAJOR'/g' ${STABLE_VER_FILE[$i]}

				# minor
				CURRET_VER_MINOR=`grep 'MINOR_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				MODIFY_VER_MINOR=`echo $MODIFY_STABLE_VERSION | cut  -d  '.'  -f  2`
				sed -i '/MINOR_VERSION /s/'$CURRET_VER_MINOR'/'$MODIFY_VER_MINOR'/g' ${STABLE_VER_FILE[$i]}

				# patch
				CURRET_VER_PATCH=`grep 'PATCH_VERSION ' ./${STABLE_VER_FILE[$i]} |grep '=' |awk '{print $3}'`
				MODIFY_VER_PATCH=`echo $MODIFY_STABLE_VERSION | cut  -d  '.'  -f  3`
				sed -i '/PATCH_VERSION /s/'$CURRET_VER_PATCH'/'$MODIFY_VER_PATCH'/g' ${STABLE_VER_FILE[$i]}

				######################### 2. git add file ############################
				git add ${STABLE_VER_FILE[$i]}

				CHECK_COMMIT_FORMAT=`echo $ADD_COMMIT_MESSAGE | grep -c '-'`
				if [ $CHECK_COMMIT_FORMAT -eq 1 ]; then
				######################### 3. git commit -m ############################
git commit --sign -m "
${BLX_PATH[$i]}: stable branch update version: bl-${MODIFY_VER_MAJOR}.${MODIFY_VER_MINOR}.${MODIFY_VER_PATCH} [1/1]

PD#${ADD_COMMIT_MESSAGE}

Problem:
bootloader stable branch(openlinux/bl-${STABLE_MAJOR_VERSION}) update

Solution:
add version to build message: bl-${MODIFY_VER_MAJOR}.${MODIFY_VER_MINOR}.${MODIFY_VER_PATCH}

Verify:
ohm/oppen"
				fi
			fi
		fi
	done

}

function push_stable_branch_version() {
	echo
	echo "push_stable_branch_version:"
	#for i in $(seq 0 ${BLX_NAME[@]}); do
	for((i=0;i<${#BLX_NAME[@]}-1;i++)); do
		if [ -d $rootdir/${BLX_PATH[$i]} ]; then
			cd $rootdir/${BLX_PATH[$i]}

			#check bl2/core is at sc2 branch
			CURRENT_BRANCH=`git branch |grep "*"| awk '{print $2}'`
			if [ $i -eq 0 -a "${CURRENT_BRANCH}" != "${BLX_STABLE_BRANCH[0]}" ];then
				echo "bl2 core is not at sc2 branch. please switch branch first !"
				echo ""
				continue
			fi

			# stable branch version format: bl-x.y.z;
			# x is major version, y is minor version, z is patch version
			if [ "${STABLE_VER_TOPIC[$i]}" != "NULL" ]; then
				echo ""
				echo "PUSH ${STABLE_VER_TOPIC[$i]} ==>>"
				git push review HEAD:refs/for/${BLX_STABLE_BRANCH[$i]}%topic=${PUSH_TOPIC_COMMON}-${STABLE_VER_TOPIC[$i]}
				echo ""
				if [ "${STABLE_VER_TOPIC[$i]}" = "bl2-sc2" ];then
					echo ""
					echo "PUSH bl2-s4 ==>>"
					BL2_SC2_LAST_COMMIT=`git log --pretty=format:"%h" | head -1  | awk '{print $1}'`
					git checkout ${BL2_S4_BRANCH} > /dev/null
					git cherry-pick ${BL2_SC2_LAST_COMMIT}
					git push review HEAD:refs/for/${BL2_S4_BRANCH}%topic=${PUSH_TOPIC_COMMON}-bl2-s4
					git checkout ${BLX_STABLE_BRANCH[$i]} > /dev/null
					echo ""
				fi
			fi
		fi
	done

	echo
}

function show_current_branch_info() {
	echo
	echo "show_current_branch_commit_info:"
	echo "==============================="
	#for i in $(seq 0 ${BLX_NAME[@]}); do
	for((i=0;i<${#BLX_NAME[@]};i++)); do
		if [ -d $rootdir/${BLX_PATH[$i]} ]; then
			cd $rootdir/${BLX_PATH[$i]}
			commit=`git rev-parse HEAD`
			# compare setting commit with current commit
			if [ ${#BLX_COMMIT[$i]} -gt 20 ]; then
				if [ "${BLX_COMMIT[$i]}" == "$commit" ]; then
					echo "[eq] ${BLX_NAME[$i]}: [`git branch --show-current`]  `git log --oneline -1`"
				else
					echo "[no] ${BLX_NAME[$i]}: [`git branch --show-current`]  `git log --oneline -1`"
				fi
			else
				#echo "= ${BLX_NAME[$i]}: `git branch --show-current && git log --oneline -1 | tr -d '\n'`"
				echo "[--] ${BLX_NAME[$i]}: [`git branch --show-current`]  `git log --abbrev=8 --oneline -1 `"
				CURRENT_BRANCH=`git branch |grep "*"| awk '{print $2}'`
				if [ $i -eq 0 -a "${CURRENT_BRANCH}" = "${BLX_STABLE_BRANCH[0]}" ];then
					git checkout ${BL2_S4_BRANCH} > /dev/null 2>&1
					echo "[--] ${BLX_NAME[$i]}: [`git branch --show-current`]  `git log --abbrev=8 --oneline -1 `"
					git checkout ${BLX_STABLE_BRANCH[0]} > /dev/null 2>&1
				elif [ $i -eq 0 -a "${CURRENT_BRANCH}" = "${BL2_S4_BRANCH}" ];then
					git checkout ${BLX_STABLE_BRANCH[0]} > /dev/null 2>&1
					echo "[--] ${BLX_NAME[$i]}: [`git branch --show-current`]  `git log --abbrev=8 --oneline -1 `"
					git checkout ${BL2_S4_BRANCH} > /dev/null 2>&1
				fi
			fi
		else
			echo "[--] ${BLX_NAME[$i]}: ------"
		fi
	done
	echo "==============================="
	echo
}

function switch_to_target_branch() {
	echo
	if [ $SWITCH_TARGET_BRANCH -eq 1 ] ; then
		echo "branch: trunk"
		BLX_BRANCH=(${BLX_TRUNK_BRANCH[*]})
	elif [ $SWITCH_TARGET_BRANCH -eq 2 ]; then
		echo "branch: stable"
		BLX_BRANCH=(${BLX_STABLE_BRANCH[*]})
	else
		exit
	fi
	show_setting_branch

	echo "switch_to_target_branch:"
	for((i=0;i<${#BLX_NAME[@]}-1;i++)); do
		if [ -d $rootdir/${BLX_PATH[$i]} ]; then
			echo "path: ${BLX_PATH[$i]}"
			cd $rootdir/${BLX_PATH[$i]}
			sync_code ${BLX_REMOTE[$i]} ${BLX_BRANCH[$i]}
			if [ ${#BLX_COMMIT[$i]} -gt 20 ]; then
				git reset --hard ${BLX_COMMIT[$i]}
			fi
		else
			echo "error, path($rootdir/${BLX_PATH[$i]}) not exist!"
		fi
	done

	echo
}

function modify_stable_sub_version() {
	echo
	echo "# push_uboot_img_to_android_repo"
	# 1. switch to target branch
	if [ $SWITCH_TARGET_BRANCH -ne 0 ]; then
		switch_to_target_branch
	fi

	# 2. show current branch branch and commit
	if [ $SHOW_LAST_COMMIT -eq 1 ]; then
		show_current_branch_info
	fi

	# 3. show stable branch last version
	if [ $SHOW_STABLE_VERSION -eq 1 ]; then
		show_stable_branch_version
	fi

	# 4. modify stable branch last version
	CHECK_VERSION_FORMAT=`echo $MODIFY_STABLE_VERSION | awk -F'.' '{print NF-1}'`
	if [ $CHECK_VERSION_FORMAT -eq 2 ]; then
		modify_stable_branch_version
		show_stable_branch_version
	fi

	# 5. push stable branch last version
	if [ "$PUSH_TOPIC_COMMON" != "0" ]; then
		push_stable_branch_version
		show_current_branch_info
	fi
}

function compile_stable_uboot_bin() {

	for((i=0;i<${#OPENLINUX_BOARD_TYPE[@]};i++)); do
		echo
		echo "<$i.${OPENLINUX_BOARD_TYPE[$i]}>############################### push_uboot_img_to_android_repo."

		echo
		echo "#1. check android dev path"	
		if [ -d ${ANDROID_DIR_LIST[$i]} ];then
			cd ${ANDROID_DIR_LIST[$i]}
			git reset --hard HEAD^
			git clean -df
			git fetch --all && git reset --hard && git pull
			cd -
		else
			err_exit "Android dir is not exist! (${ANDROID_DIR_LIST[$i]})"
		fi

		if [ -f "./build-${OPENLINUX_BOARD_TYPE[i]}-${PUSH_TOPIC_COMMON}/u-boot.bin.signed" ]; then

			echo
			echo "# 2/3. build bootloader manual already, copy it directly"
			cp -fv ./build-${OPENLINUX_BOARD_TYPE[i]}-${PUSH_TOPIC_COMMON}/u-boot.bin.signed 		${ANDROID_DIR_LIST[$i]}/bootloader.img
			cp -fv ./build-${OPENLINUX_BOARD_TYPE[i]}-${PUSH_TOPIC_COMMON}/u-boot.bin.usb.signed 	${ANDROID_DIR_LIST[$i]}/upgrade/
			cp -fv ./build-${OPENLINUX_BOARD_TYPE[i]}-${PUSH_TOPIC_COMMON}/u-boot.bin.sd.bin.signed  ${ANDROID_DIR_LIST[$i]}/upgrade

		else
			echo
			echo "# 2. build booloader image"
			cd bl2/core && git checkout ${OPENLINUX_BOARD_BRANCH[$i]} || err_exit "cd bl2/core faild !"
			cd -
			runcmd="./mk ${OPENLINUX_BOARD_CONFIG[i]} --update-bl2 --update-bl2e --update-bl2x --update-bl31 --update-bl32 --avb2 --vab"
			echo "[run cmd]: ${runcmd}"
			${runcmd} || err_exit "./mk ${OPENLINUX_BOARD_CONFIG[i]} faild !"
			if [ -f "./build/u-boot.bin.signed" ];then
				echo "\n build booloader(${OPENLINUX_BOARD_TYPE[$i]}) SUCCESS."
			else
				err_exit "build booloader(${OPENLINUX_BOARD_TYPE[$i]}) FAILED."
			fi

			echo
			echo "# 3. copy booloader img to android dir"
			cp -fv ./build/u-boot.bin.signed 		 ${ANDROID_DIR_LIST[$i]}/bootloader.img
			cp -fv ./build/u-boot.bin.usb.signed 	 ${ANDROID_DIR_LIST[$i]}/upgrade/
			cp -fv ./build/u-boot.bin.sd.bin.signed  ${ANDROID_DIR_LIST[$i]}/upgrade/
		fi

		echo
		echo "# 4. add commit message"
		cd ${ANDROID_DIR_LIST[$i]}
		if [ ! -f "${ANDROID_DIR_LIST[$i]}/bootloader.img" ]; then
			err_exit " copy bootloader img error, exit."
		fi
		git add -u
git commit --sign -m "
${OPENLINUX_BOARD_TYPE[$i]}: bootloader stable branch bin for ${PUSH_TOPIC_COMMON} [1/1]

PD#${ADD_COMMIT_MESSAGE}

Problem:
bootloader stable branch(openlinux/bl-${STABLE_MAJOR_VERSION}) update

Solution:
add version to build message: ${PUSH_TOPIC_COMMON}

Verify:
local ${OPENLINUX_BOARD_TYPE[$i]}"

		echo
		echo "# 5. push booloader img to device/amlogic"
		git push review HEAD:refs/for/${ANDROID_TARGET_BRANCH}%topic=BL${STABLE_MAJOR_VERSION} || err_exit " push bootloader img error."
		cd -
		echo "<$i.${OPENLINUX_BOARD_TYPE[$i]}># done."
		echo
	done
}

show_help()
{
	echo -e "\e[1;35m [usage]  V0.3  2022.09.18\e[0m"
	echo "    $0 -[o:/s/t/c/v/m:/a:/p/j:/h]"
	echo ""
	echo -e "\e[1;35m [option] \e[0m"
	echo "    -o : option type. 1:modify stable sub version; 2:compile stable uboot.bin"
	echo "    -s : switch to stable branch, sync code."
	echo "    -t : switch to trunk branch, sync code."
	echo "    -c : print current branch last commit."
	echo "    -v : print stable branch last version."
	echo "    -m : modify stable branch last version x.y.z, eg: -m 3.4.5"
	echo "    -a : add version change and commit message, eg: -a SWPL-83088"
	echo "    -p : push stable branch with topic info. eg: -p BL3.4.4-20220620"
	echo "    -j : set stable branch major version. eg: -j 3.4.0"
	echo "    -h : show help"
	echo ""
	echo -e "\e[1;35m [external ENV] \e[0m"
	echo "    Set these ENV externally(not necessary), cmd: export ENV=\${ENV}"
	echo "        STABLE_MAJOR_VERSION  (default:${STABLE_MAJOR_VERSION})"
	echo "        ANDROID_ROOT_DIR      (default:${ANDROID_ROOT_DIR})"
	echo "        ANDROID_TARGET_BRANCH (default:${ANDROID_TARGET_BRANCH})"
	echo ""
	echo -e "\e[1;35m [example]\e[0m"
	echo "    (1) modify stable sub version, and push to stable"
	echo "        $0 -o 1 -m 3.4.9 -a SWPL-85641 -p BL3.4.9-20220912"
	echo ""
	echo "    (2) compile stable uboot.bin, and push to android"
	echo "        $0 -o 2 -m 3.4.9 -a SWPL-85641 -p BL3.4.9-20220912"
	echo ""
	echo "    (3) manual compile stable uboot.bin (sc2_ah212 / s4_ap222)"
	echo "        ./mk sc2_ah212 --update-bl2 --update-bl2e --update-bl2x --update-bl31 --update-bl32 --avb2 --vab"
	echo ""
	exit 1
}

function parser() {

    if [ -z $1 ]; then
        show_help
        return
    fi

	while getopts sStThcCvVm:M:o:O:p:P:a:A:j:J:hH opt; do
			case ${opt} in
			o|O)
				STABLE_OPTION_TYPE=${OPTARG}
				;;
			s|S)
				SWITCH_TARGET_BRANCH=2
				;;
			t|T)
				SWITCH_TARGET_BRANCH=1
				;;
			c|C)
				SHOW_LAST_COMMIT=1
				;;
			v|V)
				SHOW_STABLE_VERSION=1
				;;
			m|M)
				MODIFY_STABLE_VERSION=${OPTARG}
				;;
			a|A)
				ADD_COMMIT_MESSAGE=${OPTARG}
				;;
			j|J)
				STABLE_MAJOR_VERSION=${OPTARG}
				;;
			p|P)
				PUSH_TOPIC_COMMON=${OPTARG}
				;;
			h|H)
				show_help
				;;
			*)
				show_help
				;;
			esac
	done
}


function main() {
	if [ -f "build_bl2.sh" ]; then
		rootdir=`pwd -P`/../
	fi
	if [ -d "fip" ]; then
		rootdir=`pwd -P`
	fi

	echo
	echo "Run start: $rootdir "
	echo

    parser $@

	# check option type
	if [ "$STABLE_OPTION_TYPE" = "0" ]; then
		show_help
	elif [ "$STABLE_OPTION_TYPE" = "1" ]; then
		modify_stable_sub_version
	elif [ "$STABLE_OPTION_TYPE" = "2" ]; then
		compile_stable_uboot_bin
	fi

	echo
	echo "Run finish."
	echo
}

#
# start here.
#
main $@
