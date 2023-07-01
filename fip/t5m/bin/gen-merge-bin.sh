#!/bin/bash


function process_join() {
	local intput0=$1
	local size0=$2
	local intput1=$3
	local size1=$4
	local output=$5
	local temp_output=${output}.temp
	local temp_size=`expr ${size0} + ${size1}`
	
	#cat ${intput0} > ${intput0}.orig
	#cat ${intput1} > ${intput1}.orig
	dd if=/dev/zero of=${temp_output} bs=1 count=${temp_size}
	input_size=`stat -c %s ${intput0}`
	if [ $input_size -gt ${size0} ]; then
		dd if=${intput0} of=${temp_output} bs=1 count=${size0}
	else
		dd if=${intput0} of=${temp_output} bs=1 conv=notrunc
	fi
	input_size=`stat -c %s ${intput1}`
	if [ $input_size -gt ${size1} ]; then
		dd if=${intput1} of=${temp_output} seek=${size0} bs=1 count=${size1}
	else
		dd if=${intput1} of=${temp_output} seek=${size0} bs=1 conv=notrunc
	fi
	cat ${temp_output} > ${output}
	rm -rf ${temp_output}
	merge_size=`stat -c %s ${output}`
	if [ ${merge_size} -ne ${temp_size} ]; then
		echo "$output file size is not equal $temp_size, ${merge_size}"
		echo -1
	fi
	return
}

function generate_joining() {
    local argv=("$@")
    local i=0

     # Parse args
	while [ $i -lt $# ]; do
	    arg="${argv[$i]}"
				#echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --input0)
                input0="${argv[$i]}" ;;
			--size0)
				size0="${argv[$i]}" ;;
            --input1)
                input1="${argv[$i]}" ;;
			--size1)
				size1="${argv[$i]}" ;;
            --output)
                output="${argv[$i]}" ;;
		esac
        i=$((i + 1))
	done

	if [ -z ${input0} ] || [ ! -f ${input0} ] || [ -z ${size0} ]; then
		return
	fi
	if [ -z ${input1} ] || [ ! -f ${input1} ] || [ -z ${size1} ]; then
		return
	fi
	if [ -z ${output} ]; then
		return
	fi
	echo "$BASH_SOURCE, $LINENO"
	process_join ${input0} ${size0} ${input1} ${size1} ${output}
	echo "$BASH_SOURCE, $LINENO"
	return
}

generate_joining $@
