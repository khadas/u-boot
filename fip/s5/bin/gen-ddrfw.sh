 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

BASEDIR_BUILD="${BASEDIR_TOP}/ddrfw_out"

declare DDRFW_DIR="ddrfw_out"
declare CHIPACS_SIZE="8192"
declare DDRFW_SIZE="212992"
declare -a CHIPACS_FEATURE=("orige" "merge")

function gen_ddrfw_area_data() {
	local ddr_input=$1
	local ddr_output=$2
	local ddr_type=$3
	local ddr_param_bin=$4

	echo "$BASH_SOURCE,$LINENO"
	echo "ddr_input=$ddr_input, ddr_output=$ddr_output, ddr_type=$ddr_type"
	echo "$BASH_SOURCE,$LINENO"

	if [ "$ddr_type" == "ddr4" ]; then
		dd if=${ddr_input}/ddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=${ddr_input}/ddr4_2d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=36864  &> /dev/null
	elif [ "$ddr_type" == "ddr3" ]; then
		dd if=${ddr_input}/ddr3_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=/dev/zero of=${ddr_output}/ddrfw_2d.bin bs=36864 count=1
	elif [ "$ddr_type" == "lpddr4" ]; then
		dd if=${ddr_input}/lpddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=69632
		dd if=${ddr_input}/lpddr4_1d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=69632
	elif [ "$ddr_type" == "lpddr5" ]; then
		dd if=${ddr_input}/lpddr5_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=69632
		dd if=${ddr_input}/lpddr5_1d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=69632
	elif [ "$ddr_type" == "lpddr4_lpddr5" ]; then
		dd if=${ddr_input}/lpddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=69632
		dd if=${ddr_input}/lpddr5_1d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=69632
	elif [ "$ddr_type" == "lpddr3" ]; then
		dd if=${ddr_input}/lpddr3_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=/dev/zero of=${ddr_output}/ddrfw_2d.bin bs=36864 count=1
	else
		echo "un-recognized ddr_type: ${ddr_type}"
		echo "---- use default ddr4 ----"
		dd if=${ddr_input}/ddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=${ddr_input}/ddr4_2d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=36864  &> /dev/null
	fi

	piei_size=`stat -c %s ${ddr_input}/piei.fw`
	if [ $piei_size -gt 24672 ]; then
		dd if=${ddr_input}/piei.fw of=${ddr_output}/ddrfw_piei.bin skip=96 bs=1 count=24576  &> /dev/null
	else
		dd if=/dev/zero of=${ddr_output}/ddrfw_piei.bin bs=24576 count=1  &> /dev/null
		dd if=${ddr_input}/piei.fw of=${ddr_output}/ddrfw_piei.bin skip=96 bs=1 conv=notrunc  &> /dev/null
	fi

	if [ ! -f $ddr_param_bin ]; then
		echo "$ddr_param_bin is not exist"
		#dd if=/dev/zero of=${ddr_param_bin} bs=8192 count=1
		exit -1
	fi
	ddrparam_size=`stat -c %s $ddr_param_bin`
	if [ $ddrparam_size -gt 8192 ]; then
		dd if=$ddr_param_bin of=${ddr_output}/aml_ddr_param.bin  bs=1 count=8192
	else
		dd if=/dev/zero of=${ddr_output}/aml_ddr_param.bin bs=8192 count=1
		dd if=$ddr_param_bin of=${ddr_output}/aml_ddr_param.bin  bs=1 conv=notrunc
	fi

	aml_ddr_size=`stat -c %s ${ddr_input}/aml_ddr.fw`
	if [ $aml_ddr_size -gt 40960 ]; then
		dd if=${ddr_input}/aml_ddr.fw of=${ddr_output}/aml_ddr.bin  bs=1 count=40960
	else
		dd if=/dev/zero of=${ddr_output}/aml_ddr.bin bs=40960 count=1
		dd if=${ddr_input}/aml_ddr.fw of=${ddr_output}/aml_ddr.bin  bs=1 conv=notrunc
	fi

	cat ${ddr_output}/ddrfw_1d.bin ${ddr_output}/ddrfw_2d.bin \
		${ddr_output}/ddrfw_piei.bin ${ddr_output}/aml_ddr_param.bin \
		${ddr_output}/aml_ddr.bin > ${ddr_output}/ddr-fwdata.bin

	rm -rf ${ddr_output}/ddrfw_1d.bin ${ddr_output}/ddrfw_2d.bin \
			${ddr_output}/ddrfw_piei.bin ${ddr_output}/aml_ddr_param.bin \
			${ddr_output}/aml_ddr.bin

	if [ ! -f ${ddr_output}/ddr-fwdata.bin ]; then
		echo "ddr-fwdata payload does not exist in ${ddr_output} !"
		exit -1
	fi

	ddrfw_data_size=`stat -c %s ${ddr_output}/ddr-fwdata.bin`
	if [ $ddrfw_data_size -ne ${DDRFW_SIZE} ]; then
		echo "ddr-fwdata size is not equal to ${DDRFW_SIZE}, $ddrfw_data_size"
		exit -1
	fi
}

function generate_ddrfw() {
    local argv=("$@")
    local i=0

     # Parse args
	while [ $i -lt $# ]; do
	    arg="${argv[$i]}"
				#echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --input)
               input="${argv[$i]}" ;;
            --output)
                output="${argv[$i]}" ;;
            --ddr_type)
                ddr_type="${argv[$i]}" ;;
            --ddr_param)
                ddr_param="${argv[$i]}" ;;
            --soc)
                soc="${argv[$i]}" ;;
		esac
        i=$((i + 1))
	done

	if [ -z ${soc} ]; then
		soc="p1"
	fi
	if [ -z ${ddr_type} ]; then
		ddr_type="lpddr4_lpddr5"
	fi
	if [ -z ${ddr_param} ] || [ ! -f ${ddr_param} ]; then
		exit -1;
	fi
	if [ -z ${output} ]; then
		echo "output ${output} invalid"
		exit 1
	fi

	mkdir ${BASEDIR_BUILD}

	gen_ddrfw_area_data ${BASEDIR_TOP} ${BASEDIR_BUILD} ${ddr_type} ${ddr_param}
	cp ${BASEDIR_BUILD}/ddr-fwdata.bin ${output}

	echo "$BASH_SOURCE, $LINENO"
}

rm -rf ${BASEDIR_BUILD}
generate_ddrfw $@
rm -rf ${BASEDIR_BUILD}

