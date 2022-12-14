#!/bin/bash
#
#  =============================================================================
#  @Module:     BootLoader coverity check
#  @Author:     Li Dongqing  (dongqing.li@amlogic.com)
#  @License:    Copyright (c) 2019 Amlogic, Inc. All rights reserved.
#  @Changes:
#               1. 2021.11.22 v0.1 - Create bash check_coverity.
#               2. 2022.09.29 v0.2 - Support run long command
#  =============================================================================

#############
# function
#############
MK_ARGV=""
NEW_ARGV=""
#COVERITY_PATH="/proj/coverity/cov-analysis-linux64-2020.12/bin"
COV_IM_DIR="./cov-imdir"
COV_RESULT_HTML="./result-html"
HIGH_LEVEL="0"
DIR_FILTER=""
PATTERN_ENABLE="0"
IS_CMD_STRING=0
RUN_BASH=""
MANUAL_RUN_FLAG=0

#############
# function
#############
function err_exit()
{
	echo "Error: $1"
	exit 1
}

function check_cov_path() {
	echo "[run cov path] : ${MKPATH}"
	if [ -d "${COV_IM_DIR}" ]; then
		rm -rf ${COV_IM_DIR}
	fi
	mkdir -p ${COV_IM_DIR}
	if [ -d "${COV_RESULT_HTML}" ]; then
		rm -rf ${COV_RESULT_HTML}
	fi
}

function check_cov_build_version() {
	COV_CONFIG=`cov-build --help | grep 'Task Reference' -c`
	if [ "${COV_CONFIG}" = "0" ]; then
		echo
		echo -e "\e[1;35m Run cov-build error! Please check: \e[0m"
		echo "  1.is installed ? if not, call IT. (/proj/coverity/cov-analysis-linux64-2021.12.0/bin)"
		echo "  2.in PATH env ? if not, export it.(export PATH=/proj/coverity/cov-analysis-linux64-2021.12.0/bin/:$PATH)"
		echo
		exit 0
	else
		COV_VERSION=`cov-build --help | grep 'Task Reference' | awk '{print $2}'`
		echo "check_cov_version:   ${COV_VERSION}"
	fi
}

function run_coverity() {
	echo ""
	echo -e "\e[1;35m[1] run cov-build: $@ \e[0m"
	check_cov_build_version
	cov-build --dir ${COV_IM_DIR} $@ || err_exit "cov-build error."
	echo -e "\e[1;35m[1] run cov-build OK. \e[0m"
}

function analysis_coverity() {
	echo ""
	echo -e "\e[1;35m[2] run cov-analyze ... \e[0m"
	if [ ${HIGH_LEVEL} = "1" ]; then
        if [ "${PATTERN_ENABLE}" = "1" ];then
			cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all --aggressiveness-level high --fb-max-mem 3072 --tu-pattern "file('/${DIR_FILTER}')" || err_exit "cov-analyze high level error."
        else
			cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all --aggressiveness-level high --fb-max-mem 3072 || err_exit "cov-analyze high level error."
		fi
	else
		if [ "${PATTERN_ENABLE}" = "1" ];then
			cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all  --tu-pattern "file('/${DIR_FILTER}')" || err_exit "cov-analyze normal level error."
		else
			cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all  || err_exit "cov-analyze normal level error."
		fi
	fi
	echo -e "\e[1;35m[2] run cov-analyze OK. \e[0m"

	echo ""
	echo -e "\e[1;35m[3] run cov-format-errors ... \e[0m"
	cov-format-errors --dir ${COV_IM_DIR} --html-output ${COV_RESULT_HTML} --filesort --strip-path $MKPATH -x || err_exit "cov-format-errors error."
	echo -e "\e[1;35m[3] run cov-format-errors OK. \e[0m"
	echo "end."

	# show coverity result info
	show_coverity_result

	if [ "${COVERITY_PUSH}" = "1" ]; then
		echo ""
		echo -e "\e[1;35m[4] run cov-commit-defects ... \e[0m"
		if [ -z ${COVERITY_KEY} -o -z ${COVERITY_STREAM} ]; then
			echo -e "\e[1;35m[4] --key or --stream is not set, push stream exit ! \e[0m"
		else
			cov-commit-defects --dir ${COV_IM_DIR} --auth-key-file ${COVERITY_KEY} --host 10.18.11.122 --stream ${COVERITY_STREAM} --noxrefs || err_exit "cov-format-errors error."
			echo -e "\e[1;35m[4] run cov-commit-defects OK. \e[0m"
			echo ""
			echo -e "\e[1;35m[snapshot-result] \e[0m"
			echo "you can view the result with the [New snapshot ID] above on coverity server!"
			echo "path: http://10.18.11.122:8080/ -> choose projects -> choose SNAPSHOTS(ALL in Project) -> choose ID"
			echo " "
		fi
	fi

	rm -rf ${COV_IM_DIR}
}

function show_coverity_result() {
	echo ""
	echo -e "\e[1;35m[html-result] \e[0m"
	echo "you can open the index.html files through a browser, and view code defects."
	echo "path: ${MKPATH}/`basename ${COV_RESULT_HTML}`/index.html"
	echo " "
}

########
# mk_script.sh call function here !
########
function check_coverity() {

	MK_ARGV=$@
	if [ -z "${DIR_FILTER}" ];then
		echo "DIR_FILTER not set, ignore. "
		PATTERN_ENABLE="0"
	else
		if [ -e "./${DIR_FILTER}" ];then
			echo "DIR_FILTER set ok: ${DIR_FILTER}"
			PATTERN_ENABLE="1"
		else
			echo "DIR_FILTER invalid, ingore."
			PATTERN_ENABLE="2"
		fi
	fi

	echo -e "\e[1;35m=========== run check_coverity() ===========\e[0m"

	echo "------------------------------------------------------------------"
	echo "coverity raw command  : ./mk $@ "

	# check argv
	result=$(echo ${MK_ARGV} | grep "\-\-cov-high")
	if [[ "$result" != "" ]]; then
		echo "coverity defect level : high"
		HIGH_LEVEL="1"
		NEW_ARGV=`echo "${MK_ARGV//--cov-high/ }"`
	else
		echo "coverity defect level : normal"
		HIGH_LEVEL="0"
		NEW_ARGV=`echo "${MK_ARGV//--cov/ }"`
	fi
	if [ "PATTERN_ENABLE" != "0" ];then
		NEW_ARGV=`echo "${NEW_ARGV//${DIR_FILTER}/ }"`
	fi
	echo "coverity new command  : ./mk ${NEW_ARGV}"
	MKPATH=`pwd -P`
	echo "coverity run path     : $MKPATH"
	echo "------------------------------------------------------------------"

	# check_cov_path
	check_cov_path

	# run_coverity
	run_coverity "./mk ${NEW_ARGV}"

	# analysis coverity
	analysis_coverity
}

#############
# for bl2/core
#############
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

function run_cov_for_bl2_core() {
	# get all support soc
	cd ../ree/plat/
	array=`ls -d *`
	cd - &> /dev/null

	skiped=("common" "fvp" "juno" "golden")
	for item in ${skiped[@]}
	{
		# remove skiped item
		array=${array//${item}/''}
	}

	RESULT='\n'"Build BL2 core for SoC: "${array[@]}'\n'
	echo -e $RESULT

	# loop all soc
	for soc in ${array[@]}
	do
		TEST_BRANCH=projects/$soc
		echo "TEST_BRANCH=:$TEST_BRANCH"

		# prepare code
		sync_code firmware ${TEST_BRANCH}

		# run test
		run_coverity ./mk $soc --dusb
		run_coverity ./mk $soc --dsto
	done
}

function show_help() {
	echo
	echo -e "\e[1;35m [usage] \e[0m"
	echo "    ./check_coverity.sh -c '[cmd_string]'"
	echo
	echo -e "\e[1;35m [option] \e[0m"
	echo "    -c : set cmd string, eg: './check.sh' or './mk sc2_ah212 --avb'"
	echo "    -d : set dir filter, eg: 'plat/t3/'(relative path, no prefix './')"
	echo "    -t : set top level mode, could detect more errors."
	echo "    -k : set coverity key, must be used with '-p'"
	echo "    -s : set coverity stream, must be used with '-p'"
	echo "    -p : push coverity result to cov server."
	echo "    -h : show help"
	echo
	echo -e "\e[1;35m [example] \e[0m"
	echo "    1) run coverity with compile cmd"
	echo "       cd bl2/core; ../../fip/check_coverity.sh -c './mk sc2 --dsto'"
	echo "       cd bl2/core; ../../fip/check_coverity.sh -c './mk sc2 --dsto' -d plat/sc2/"
	echo "    2) quick for [bl2/core]:"
	echo "       bash /path/to/fip/check_coverity.sh  -c  bl2_core"
	echo "    3) quick for [bl2/src] [bl2/ree] [bl2/tee] [bl31_1.3/src] [bl32_3.8/src]:"
	echo "       bash /path/to/fip/check_coverity.sh  -c  ./check.sh"
	echo "    4) quick for [bl33/v2015] [bl33/v2019]:"
	echo "       bash /path/to/fip/check_coverity.sh  -c  ./check_compile.sh"
	echo "    5) run coverity and push result to cov server"
	echo "       bash /path/to/fip/check_coverity.sh  -c  './mk sc2' -k /path/to/user.key -s bootloader-amlogic-dev-bl2-core -p"
	echo
	return
}

#############
# main
#############
function main() {
	echo
	echo "[shell   name] : $RUN_BASH"
	echo "[compile  cmd] : $CMD_STRING"
	if [ ! -z "${DIR_FILTER}" ]; then
		echo "[pattern path] : $DIR_FILTER"
	fi

	if [ "${COVERITY_PUSH}" = "1" ]; then
		echo "[coverity keynum] : $COVERITY_KEY"
		echo "[coverity stream] : $COVERITY_STREAM"
	fi

	if [ $IS_CMD_STRING -eq 1 ]; then
		# check coverity path
		check_cov_path

		# run_coverity
		if [[ "${CMD_STRING}" = "bl2_core" ]]; then
			run_cov_for_bl2_core
		else
			run_coverity ${CMD_STRING}
		fi

		# analysis coverity
		analysis_coverity
	fi

	return
}

#
# start here.
#
MKPATH=`pwd -P`

if [[ "$0" =~ "check_coverity" ]]; then
	RUN_BASH="$0"
	if [ $# -gt 0 ]; then
		MANUAL_RUN_FLAG=1
	fi
fi

if [ ${MANUAL_RUN_FLAG} -eq 1 ]; then
	ARGS=$(getopt -o d:k:s:pth -a -l c:,d:,k:,s:pth -n "check_coverity.sh" -- "$@")
	if [ $? != 0 ]; then
		exit 0
	fi

	eval set -- "$ARGS"
	while true
	do
		case "$1" in
		-c | --c | --cmd)
			CMD_STRING="$2"
			IS_CMD_STRING=1
			shift 2
			;;
		-d | --dir)
			DIR_FILTER="$2"
			if [ -e "./${DIR_FILTER}" ];then
				PATTERN_ENABLE="1"
			else
				PATTERN_ENABLE="2"
			fi
			shift 2
			;;
		-t | --top)
			HIGH_LEVEL="1"
			shift
			;;
		-k | --key)
			COVERITY_KEY="$2"
			shift 2
			;;
		-s | --stream)
			COVERITY_STREAM="$2"
			shift 2
			;;
		-p | --push)
			COVERITY_PUSH="1"
			shift
			;;
		-h | --help)
			show_help
			exit 0
			;;
		--)
			shift
			break
			;;
		esac
	done

	main
fi
