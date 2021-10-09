#!/bin/bash
#
#  author: dongqing.li@amlogic.com
#  2021.11.22 v1.0 init.
#

#############
# function
#############
MK_ARGV=""
NEW_ARGV=""
COVERITY_PATH="/proj/coverity/cov-analysis-linux64-2020.12/bin"
COV_IM_DIR="./cov-imdir"
COV_RESULT_HTML="./result-html"
HIGH_LEVEL="0"
PATTERN_PATH=""
PATTERN_ENABLE="0"

#############
# function
#############
function err_exit()
{
	echo "Error: $1"
	exit 1
}

function check_cov_path() {
	echo ""
	echo "check_cov_path ..."
	echo ""
	if [ -d "${COV_IM_DIR}" ]; then
		rm -rf ${COV_IM_DIR}
	fi
	mkdir -p ${COV_IM_DIR}
	if [ -d "${COV_RESULT_HTML}" ]; then
		rm -rf ${COV_RESULT_HTML}
	fi
}

function run_coverity() {
	echo ""
	echo -e "\e[1;35m[1] run cov-build ... \e[0m"
	${COVERITY_PATH}/cov-build --dir ${COV_IM_DIR} ./mk $@ || err_exit "cov-build error."
	echo -e "\e[1;35m[1] run cov-build OK. \e[0m"

	echo ""
	echo -e "\e[1;35m[2] run cov-analyze ... \e[0m"
	if [ ${HIGH_LEVEL} = "1" ]; then
        	if [ "${PATTERN_ENABLE}" = "1" ];then
			${COVERITY_PATH}/cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all --aggressiveness-level high --fb-max-mem 3072 --tu-pattern "file('/${PATTERN_PATH}')" || err_exit "cov-analyze high level error."
        	else
			${COVERITY_PATH}/cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all --aggressiveness-level high --fb-max-mem 3072 || err_exit "cov-analyze high level error."
		fi
	else
		if [ "${PATTERN_ENABLE}" = "1" ];then
			${COVERITY_PATH}/cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all  --tu-pattern "file('/${PATTERN_PATH}')" || err_exit "cov-analyze normal level error."
		else
			${COVERITY_PATH}/cov-analyze --dir ${COV_IM_DIR} --strip-path $MKPATH --all  || err_exit "cov-analyze normal level error."
		fi
	fi
	echo -e "\e[1;35m[2] run cov-analyze OK. \e[0m"

	echo ""
	echo -e "\e[1;35m[3] run cov-format-errors ... \e[0m"
	${COVERITY_PATH}/cov-format-errors --dir ${COV_IM_DIR} --html-output ${COV_RESULT_HTML} --filesort --strip-path $MKPATH -x || err_exit "cov-format-errors error."
	echo -e "\e[1;35m[3] run cov-format-errors OK. \e[0m"
	echo "end."

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
	if [ -z "${PATTERN_PATH}" ];then
		echo "pattern_path not set, ignore. "
		PATTERN_ENABLE="0"
	else
		if [ -e "./${PATTERN_PATH}" ];then
			echo "PATTERN_PATH set ok: ${PATTERN_PATH}"
			PATTERN_ENABLE="1"
		else
			echo "PATTERN_PATH invalid, ingore."
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
		NEW_ARGV=`echo "${NEW_ARGV//${PATTERN_PATH}/ }"`
	fi
	echo "coverity new command  : ./mk ${NEW_ARGV}"
	MKPATH=`pwd -P`
	echo "coverity run path     : $MKPATH"
	echo "------------------------------------------------------------------"

	# check_cov_path
	check_cov_path

	# run_coverity
	run_coverity ${NEW_ARGV}

	# show coverity result info
	show_coverity_result
}


function show_help() {

	echo -e "\e[1;35m [usage] \e[0m"
	echo "    $0 -c [config_name] -p [pattern_path] -u [update_blx] -t"
	echo ""
	echo -e "\e[1;35m [option] \e[0m"
	echo "    -c : config name, eg: t5_ak301_v1,  "
	echo "    -p : detect path, only output errors in this path."
	echo "    -t : top level mode，could detect more errors."
	echo "    -d : dump support socs name, when you don't know the soc name, can run it first."
	echo "    -u : build uboot with bl[x]/src source code, eg: bl2 bl2e bl2x bl30 bl31 bl32 "
	echo "    -h : show help"
	echo ""
	echo -e "\e[1;35m [example] \e[0m"
	echo "    1) dump support socs name:"
	echo "    	$0 -d"
	echo "    2) run coverity for bootloader:"
	echo "    	$0 -c t5_ak301_v1"
	echo "    3) run coverity, and filter out errors in the specified directory"
	echo "    	$0 -c t5_ak301_v1 -p bl33/v2019/"
	echo "    4) build uboot with bl[x]/src source code, and use top level mode"
	echo "    	$0 -c t5_ak301_v1 -p bl31_1.3/src/ -u bl31 -t"
	echo ""
	exit 1
}

function parser_update_blx() {
	if [ "${UPDATE}" = "bl2" ];then
		BLX_NAME="--update-bl2"
	elif [ "${UPDATE}" = "bl2" ];then
		BLX_NAME="--update-bl2e"
	elif [ "${UPDATE}" = "bl2x" ];then
		BLX_NAME="--update-bl2x"
	elif [ "${UPDATE}" = "bl30" ];then
		BLX_NAME="--update-bl30"
	elif [ "${UPDATE}" = "bl31" ];then
		BLX_NAME="--update-bl31"
	elif [ "${UPDATE}" = "bl32" ];then
		BLX_NAME="--update-bl32"
	else
		BLX_NAME=""
	fi
}

function main() {
	if [ $# -lt 1 ]; then
		show_help
        exit 1
	fi

	while getopts c:C:p:P:u:U:tTdDhH opt; do
			case ${opt} in
			c|C)
					CONFIG_NAME=${OPTARG}
					;;
			p|P)
					PATTERN_PATH=${OPTARG}
					;;
			u|U)
					UPDATE=${OPTARG}
					;;
			t|T)
					HIGH_LEVEL="1"
					;;
			d|D)
					DUMP_FLAG="1"
					;;
			h|H)
					show_help
					;;
			*)
					show_help
					;;
			esac
	done

	if [[ ${DUMP_FLAG} = "1" ]]; then
		echo ""
		echo "-----------------------------------------------------------------------------------------"
		echo "Please select a [config name] below, and try : $0 -c [config name]"
		echo "-----------------------------------------------------------------------------------------"
		./mk --config
		echo ""
		exit 1
	fi

	# parser update blx
	parser_update_blx

	# check coverity path
	check_cov_path

	# run_coverity
	run_coverity ${CONFIG_NAME} ${BLX_NAME}

	# show coverity result path
	show_coverity_result
}

#
# start here.
#
MKPATH=`pwd`
# main $@
