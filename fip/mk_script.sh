#!/bin/bash
#
#  author: xiaobo.gu@amlogic.com
#  2016.09.28
#  2016.12.01-2016.12.20 Update for bootloader repo
#  2017.05.20-2017.05.26 Update for txlx and ATF1.3

# include compile check script
source fip/check_compile.sh
source fip/variables.sh
source fip/lib.sh
source fip/build_bl2.sh
source fip/build_bl30.sh
source fip/build_bl31.sh
source fip/build_bl32.sh
source fip/build_bl33.sh
source fip/build_bl40.sh


function init_variable_early() {
	# source uboot pre-build configs
	if [ ! -e ${SOURCE_FILE} ]; then
		echo "${SOURCE_FILE} doesn't exist!"
		cd ${MAIN_FOLDER}
		exit -1
	else
		source ${SOURCE_FILE} &> /dev/null # ignore warning/error
	fi

	CUR_SOC=${CONFIG_SYS_SOC}

	if [ "y" == "${CONFIG_NEED_BL32}" ]; then
		BLX_NEEDFUL[3]="true"
	fi

	# export variables
	#export FIP_BUILD_FOLDER
	#export CUR_SOC

	export_variables
}

function init_variable_late() {
	# after uboot build, source configs
	local CONFIG_FILE_TMP="${MAIN_FOLDER}/autoconf"
	local STR_INVALID="option"
	if [ ! -e ${CONFIG_FILE} ]; then
		echo "${CONFIG_FILE} doesn't exist!"
		cd ${MAIN_FOLDER}
		exit -1
	else
		# workaround for source file error
		while read LINE
		do
			#echo $LINE
			# ignore "*(option)*" lines
			if [[ ${LINE} =~ ${STR_INVALID} ]]; then
				echo "ignore: $LINE"
			else
				#echo "LINE: ${LINE}"
				echo "$LINE" >> "${CONFIG_FILE_TMP}"
			fi
		done < ${CONFIG_FILE}
		source "${CONFIG_FILE_TMP}" &> /dev/null
		rm ${CONFIG_FILE_TMP}
	fi
	if [ "y" == "${CONFIG_SUPPORT_CUSOTMER_BOARD}" ]; then
		BOARD_DIR="customer/board/${CONFIG_SYS_BOARD}"
	else
		BOARD_DIR="${CONFIG_BOARDDIR}"
	fi
	export BOARD_DIR
}

function build_blx_src() {
	# compile $name $src_folder $bin_folder $soc
	local name=$1
	local src_folder=$2
	local bin_folder=$3
	local soc=$4
	#dbg "compile - name: ${name}, src_folder: ${src_folder}, bin_folder: ${bin_folder}, soc: ${soc}"
	if [ $name == ${BLX_NAME_GLB[0]} ]; then
		# bl2
		build_bl2 $src_folder $bin_folder $soc
	elif [ $name == ${BLX_NAME_GLB[1]} ]; then
		# bl30
		build_bl30 $src_folder $bin_folder $soc
	elif [ $name == ${BLX_NAME_GLB[2]} ]; then
		# bl31
		# some soc use v1.3
		check_bl31_ver $soc
	if [ $? != 0 ]; then
		echo "check bl31 ver: use v1.3"
		build_bl31_v1_3 $src_folder $bin_folder $soc
	else
		echo "check bl31 ver: use v1.0"
		build_bl31 $src_folder $bin_folder $soc
	fi
	elif [ $name == ${BLX_NAME_GLB[3]} ]; then
		# control flow for jenkins patchbuild
		if [ "$BUILD_TYPE" != "AOSP" ]; then
			# bl32
			if [ "y" == "${CONFIG_NEED_BL32}" ]; then
				build_bl32 $src_folder $bin_folder $soc
			fi
		fi
	fi
}

function build_blx() {
	# build each blx

	# switch bl31 version
	switch_bl31 ${CUR_SOC}

	# get version of each blx
	get_versions

	# build loop
	for loop in ${!BLX_NAME[@]}; do
		dbg "BIN_PATH[${loop}]: ${BIN_PATH[loop]}"
		if [ "null" == ${BIN_PATH[loop]} ]; then
			get_blx_bin ${loop}
		elif [ "source" == ${BIN_PATH[loop]} ]; then
			dbg "Build blx source code..."
			build_blx_src ${BLX_NAME[loop]} ${BLX_SRC_FOLDER[loop]} ${FIP_BUILD_FOLDER} ${CUR_SOC}
		else
			if [ ! -e ${BIN_PATH[loop]} ]; then
				echo "Error: ${BIN_PATH[loop]} doesn't exist... abort"
				exit -1
			else
				cp ${BIN_PATH[loop]} ${FIP_BUILD_FOLDER} -f
				echo "Get ${BLX_NAME[$loop]} from ${BIN_PATH[loop]}... done"
			fi
		fi
	done
	return
}

copy_bootloader() {
	mkdir -p ${BUILD_FOLDER}
	cp ${FIP_BUILD_FOLDER}u-boot.bin* ${BUILD_FOLDER}
	if [ "y" == "${CONFIG_AML_CRYPTO_IMG}" ]; then
		cp ${FIP_BUILD_FOLDER}boot.img.encrypt ${BUILD_FOLDER}boot.img.encrypt
	fi
}

function update_bin_path() {
	for loop in ${!BLX_NAME[@]}; do
		if [ "${BLX_NAME[$loop]}" == "${BLX_NAME_GLB[$1]}" ]; then
			dbg "Update BIN_PATH[$1]=$2"
			BIN_PATH[$loop]=$2
		fi
	done
}

function clean() {
	echo "Clean up"
	if [ -e ${BL33_PATH1} ]; then
		cd ${MAIN_FOLDER}
		cd ${BL33_PATH1}
		make distclean
	fi
	if [ -e ${BL33_PATH2} ]; then
		cd ${MAIN_FOLDER}
		cd ${BL33_PATH2}
		make distclean
	fi
	cd ${MAIN_FOLDER}
	rm ${FIP_BUILD_FOLDER} -rf
	rm ${BUILD_FOLDER}/* -rf
	mkdir -p ${BUILD_FOLDER}
	return
}

function build() {
	# *************************************************
	# IMPORTANT!!!!
	# don't change sequence of following function call
	# *************************************************
	clean

	# pre-build, get .config defines
	pre_build_uboot $@

	# variable init depends on uboot .config
	init_variable_early $@

	# must source under main function, all sub function can use these variables
	# but if source in sub-function, only sub-function(or sub-sub..) can use them
	source ${FIP_FOLDER}${CUR_SOC}/variable_soc.sh

	# compile fip tools for ddr_parse and map_tool
	prepare_tools

	# source soc package script
	source ${FIP_FOLDER}${CUR_SOC}/build.sh

	# update bin path, use bin.git or user defined or source code
	bin_path_parser $@
	#bin_path_update $@

	# build bl33/bl301..etc
	if [ ! $CONFIG_SYSTEM_AS_ROOT ]; then
		CONFIG_SYSTEM_AS_ROOT=null
	fi
	if [ ! $CONFIG_AVB2 ]; then
		CONFIG_AVB2=null
	fi
	build_uboot ${CONFIG_SYSTEM_AS_ROOT} ${CONFIG_AVB2}

	# source other configs after uboot compile
	init_variable_late

	# bl2/bl30/bl31..etc, build or copy from bin.git
	build_blx $@

	# cp bl33(uboot)
	copy_bl33

	# cp other firmwares(soc related)
	copy_other_soc

	# make build directory
	mkdir -p ${BUILD_FOLDER}

	# package final bootloader
	package

	if [ "y" != "${CONFIG_AML_SIGNED_UBOOT}" ]; then
		# copy bootloader to main folder
		copy_bootloader
	fi
}

function usage() {
  cat << EOF
  Usage:
    $(basename $0) --help

    Bootloader build script.

    command list:
    1. build default uboot:
        ./$(basename $0) [config_name]

    2. build uboot with specified bl[x].bin
        ./$(basename $0) [config_name] --bl[x] [path]

    3. build uboot with bl[x]/src source code
        ./$(basename $0) [config_name] --update-bl[x]

    4. print config list
        ./$(basename $0) --config

    5. check compile status with filter(optional)
        ./$(basename $0) --check-compile [filter]

    6. update aml ddr fw by source code (for g12a and later use)
        ./$(basename $0) [config_name] --update-bl2 --ddrfw


    Example:
    1) ./$(basename $0) gxb_p200_v1
      build gxb_p200_v1 config

    2) ./$(basename $0) gxb_p200_v1 --bl2 fip/bl2.bin --bl30 fip/bl30.bin
      build gxb_p200_v1 with specified bl2.bin and bl30.bin

    3) ./$(basename $0) gxb_p200_v1 --update-bl31 --update-bl2
      build gxb_p200_v1 with bl31/bl2 source code

    4) ./$(basename $0) --config
      print config list

    5) ./$(basename $0) --check-compile skt
      check all skt board compile status

    Remark:
    bl[x].bin/img priority:
    1. uboot will use binaries under bl[x]/bin/ folder by default, bl[x] version specified in .repo/manifest.xml
    2. if you want use your own bl[x].bin, specify path by add "--bl[x] [path]" parameter
    3. if you want use bl[x].bin by compile source code, please add "--update-bl[x]" parameter


EOF
  exit 1
}

function print_config() {
	cat << EOF
	Usable configs:
`uboot_config_list`
EOF
	exit 1
}

function parser() {
	local i=0
	local argv=()
	for arg in "$@" ; do
		argv[$i]="$arg"
		i=$((i + 1))
	done
	i=0
	while [ $i -lt $# ]; do
		arg="${argv[$i]}"
		i=$((i + 1)) # must place here
		case "$arg" in
			-h|--help|help)
				usage
				exit ;;
			--config)
				print_config
				return ;;
			--check-compile)
				check_compile "${argv[@]:$((i))}"
				exit ;;
			clean|distclean|-distclean|--distclean)
				clean
				exit ;;
			*)
		esac
	done
}

function bin_path_update() {
	# overwrite path in case some git repository doesn't exist
	for loop in ${!BLX_BIN_FOLDER[@]}; do
		if [ -ne ${BLX_BIN_FOLDER[$loop]} ]; then
			dbg "BLX_BIN_FOLDER[$loop] doesn't exist, use src code"
			update_bin_path $loop "source"
		fi
	done

	for loop in ${!BLX_SRC_FOLDER[@]}; do
		echo ""
	done
}

function bin_path_parser() {
	local i=0
	local argv=()
	for arg in "$@" ; do
		argv[$i]="$arg"
		i=$((i + 1))
	done
	i=0
	# ddr fw define, co-work with bl2 build script
	export CONFIG_DDR_FW
	while [ $i -lt $# ]; do
		arg="${argv[$i]}"
		i=$((i + 1)) # must pleace here
		case "$arg" in
			--bl2)
				update_bin_path 0 "${argv[@]:$((i))}"
				continue ;;
			--bl30)
				update_bin_path 1 "${argv[@]:$((i))}"
				continue ;;
			--bl31)
				update_bin_path 2 "${argv[@]:$((i))}"
				continue ;;
			--bl32)
				update_bin_path 3 "${argv[@]:$((i))}"
				continue ;;
			--update-bl2)
				update_bin_path 0 "source"
				continue ;;
			--update-bl30)
				update_bin_path 1 "source"
				continue ;;
			--update-bl31)
				update_bin_path 2 "source"
				continue ;;
			--update-bl32)
				update_bin_path 3 "source"
				continue ;;
			--ddrfw)
				CONFIG_DDR_FW=1
				export CONFIG_DDR_FW
				continue ;;
			--systemroot)
				CONFIG_SYSTEM_AS_ROOT=systemroot
				echo "export CONFIG_SYSTEM_AS_ROOT"
				export CONFIG_SYSTEM_AS_ROOT=systemroot
				continue ;;
			--avb2)
				CONFIG_AVB2=avb2
				echo "export CONFIG_AVB2"
				export CONFIG_AVB2=avb2
				continue ;;
				*)
		esac
	done
}

function main() {
	if [ -z $1 ]
	then
		usage
		return
	fi

	MAIN_FOLDER=`pwd`
	parser $@
	build $@
}

main $@ # parse all paras to function
