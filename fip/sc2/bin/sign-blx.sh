 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#
#BASEDIR_BUILD="${BASEDIR_TOP}/output"
BASEDIR_BUILD="${BASEDIR_TOP}/`date +%Y%m%d%H%M%S%N`"
postfix=.signed
declare -a BLX_BIN_SIZE=("127904" "65536" "65536" "4096" "86016" "262144" "524288"  "98304")

function process_ddrfw() {
	local ddr_input=$1
	local ddr_output=$2
	local ddr_type=$3

	if [ "$ddr_type" == "ddr4" ]; then
		dd if=${ddr_input}/ddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=${ddr_input}/ddr4_2d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=36864  &> /dev/null
	elif [ "$ddr_type" == "ddr3" ]; then
		dd if=${ddr_input}/ddr3_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=/dev/zero of=${ddr_output}/ddrfw_2d.bin bs=36864 count=1
	elif [ "$ddr_type" == "lpddr4" ]; then
		dd if=${ddr_input}/lpddr4_1d.fw of=${ddr_output}/ddrfw_1d.bin skip=96 bs=1 count=36864  &> /dev/null
		dd if=${ddr_input}/lpddr4_2d.fw of=${ddr_output}/ddrfw_2d.bin skip=96 bs=1 count=36864  &> /dev/null
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
	if [ $piei_size -gt 12384 ]; then
		dd if=${ddr_input}/piei.fw of=${ddr_output}/ddrfw_piei.bin skip=96 bs=1 count=12288  &> /dev/null
	else
		dd if=/dev/zero of=${ddr_output}/ddrfw_piei.bin bs=12288 count=1  &> /dev/null
		dd if=${ddr_input}/piei.fw of=${ddr_output}/ddrfw_piei.bin skip=96 bs=1 conv=notrunc  &> /dev/null
	fi

	cat ${ddr_output}/ddrfw_1d.bin ${ddr_output}/ddrfw_2d.bin \
		${ddr_output}/ddrfw_piei.bin > ${ddr_output}/ddr-fwdata.bin

	if [ ! -f ${ddr_output}/ddr-fwdata.bin ]; then
		echo "ddr-fwdata payload does not exist in ${ddr_output} !"
		exit -1
	fi
	ddrfw_data_size=`stat -c %s ${ddr_output}/ddr-fwdata.bin`
	if [ $ddrfw_data_size -ne 86016 ]; then
		echo "ddr-fwdata size is not equal to 84K, $ddrfw_data_size"
		exit -1
	fi
}

function parse_extra_args() {
	local bl2e_size=
	local oldifs="$IFS"
	local i=0

	# split each args
	IFS=",="

	local array=($1)
	local args=${#array[*]}

	echo ---- extra args: $1 ----
	while [ $i -lt $args ]; do
		arg="${array[$i]}"
		i=$((i + 1))
		case "$arg" in
		bl2e_size)
			BLX_BIN_SIZE[1]="${array[$i]}"
			if [ -z ${BL2E_PAYLOAD_SIZE} ]; then
				BL2E_PAYLOAD_SIZE="${array[$i]}"
				export BL2E_PAYLOAD_SIZE
			fi
			;;
		*)
			echo "Unknown option $arg";
			;;
		esac
		i=$((i + 1))
	done
	IFS="$oldifs"
}

function sign_blx() {
	local argv=("$@")
	local i=0

	# Parse args

	while [ $i -lt $# ]; do
		arg="${argv[$i]}"
				#echo "i=$i argv[$i]=${argv[$i]}"
		i=$((i + 1))
		case "$arg" in
		--blxname)
			blxname="${argv[$i]}" ;;
		--input)
			input="${argv[$i]}" ;;
		--output)
			output="${argv[$i]}" ;;
		--ddr_type)
			ddr_type="${argv[$i]}" ;;
		--chip_acs)
			chip_acs="${argv[$i]}" ;;
		--chipset_name)
			chipset_name="${argv[$i]}" ;;
		--chipset_variant)
			chipset_variant="${argv[$i]}" ;;
		--key_type)
			key_type="${argv[$i]}" ;;
		--soc)
			soc="${argv[$i]}" ;;
		--build_type)
			build_type="${argv[$i]}" ;;
		--extra_args)
			parse_extra_args "${argv[$i]}";;
		*)
			echo "Unknown option $arg"; exit 1
			;;
		esac
		i=$((i + 1))
	done

	if [ -z ${input} ] || [ ! -f ${input} ]; then
		echo "input ${input} invalid"
		exit 1
	fi

	if [ -z ${output} ]; then
		echo "output ${output} invalid"
		exit 1
	fi

	if [ -z ${blxname} ]; then
		echo "blxname ${blxname} invalid"
		exit 1
	fi

	if [ -z ${chipset_name} ]; then
		chipset_name="s905x4"
	fi

	# select bl2/bl2e sign template
	FEAT_BL2_TEMPLATE_TYPE=
	FEAT_BL2E_SIGPROT_MODE=0
	if [ -z ${chipset_variant} ] || [ ${chipset_variant} == "no_variant" ]; then
		chipset_variant_suffix=""
	else
		chipset_variant_suffix=".${chipset_variant}"
		if [[ "${input}" =~ ".sto" ]]; then
			FEAT_BL2_TEMPLATE_TYPE=".sto"
			if [[ "${chipset_variant}" =~ "nocs" && ${blxname} == "bl2e" ]]; then
				FEAT_BL2E_SIGPROT_MODE=1
			fi
		elif [[ "${input}" =~ ".usb" ]]; then
			FEAT_BL2_TEMPLATE_TYPE=".usb"
		else
			# for BL2X, it need a default value if chipset_variant
			# is set
			FEAT_BL2_TEMPLATE_TYPE=".sto"
		fi
	fi
	export FEAT_BL2_TEMPLATE_TYPE
	export FEAT_BL2E_SIGPROT_MODE

	if [[ ${blxname} == "bl2e" && ! -z ${BL2E_UPDATE_TYPE} ]]; then
		# copy pre-build bl2e of usb/sto if specified from build args
		if [[ "${BL2E_UPDATE_TYPE}" == "usb" && "${input}" =~ ".sto" ]]; then
			echo ~~~~ copy prebuild bl2e sto ~~~~
			cp  bl2/bin/${CUR_SOC}/${chipset_name}/blob-bl2e.sto${chipset_variant_suffix}.bin.signed ${output}
			return
		fi
		if [[ "${BL2E_UPDATE_TYPE}" == "sto" && "${input}" =~ ".usb" ]]; then
			echo ~~~~ copy prebuild bl2e usb ~~~~
			cp  bl2/bin/${CUR_SOC}/${chipset_name}/blob-bl2e.usb${chipset_variant_suffix}.bin.signed ${output}
			return
		fi
	fi

	if [ -z ${key_type} ]; then
		key_type="dev-keys"
	fi

	if [ -z ${soc} ]; then
		soc="sc2"
	fi

	if [ -z ${build_type} ]; then
		build_type=normal
	fi

	if [ -n ${BL2E_PAYLOAD_SIZE} ]; then
		BLX_BIN_SIZE[1]=${BL2E_PAYLOAD_SIZE}
	fi

	if [ -f "fip/_tmp/bl2e.bin" ]; then
		bl2e_bin_size=`stat -c %s fip/_tmp/bl2e.bin`
		if [[ ${bl2e_bin_size} -gt ${BLX_BIN_SIZE[1]} ]]; then
			echo ---- BL2E bin size ${bl2e_bin_size} is larger than ${BLX_BIN_SIZE[1]} ----
			exit -1
		fi
	fi

	mkdir ${BASEDIR_BUILD}

	if [ ${blxname} == "bl2" ] || [ ${blxname} == "bl2e" ] || [ ${blxname} == "bl2x" ]; then
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2-payload.bin bs=${BLX_BIN_SIZE[0]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2e-payload.bin bs=${BLX_BIN_SIZE[1]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2x-payload.bin bs=${BLX_BIN_SIZE[2]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/csinit-params.bin bs=${BLX_BIN_SIZE[3]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/ddr-fwdata.bin bs=${BLX_BIN_SIZE[4]} count=1  &> /dev/null
	elif [ ${blxname} == "bl31" ]; then
		dd if=/dev/zero of=${BASEDIR_BUILD}/${blxname}-payload.bin bs=${BLX_BIN_SIZE[5]} count=1  &> /dev/null
	elif [ ${blxname} == "bl32" ]; then
		dd if=/dev/zero of=${BASEDIR_BUILD}/${blxname}-payload.bin bs=${BLX_BIN_SIZE[6]} count=1  &> /dev/null
	elif [ ${blxname} == "bl40" ]; then
		dd if=/dev/zero of=${BASEDIR_BUILD}/${blxname}-payload.bin bs=${BLX_BIN_SIZE[7]} count=1  &> /dev/null
	else
		echo invalid blxname [$blxname]
		exit 1
	fi

	if [[ ${FEAT_BL2E_SIGPROT_MODE} == "0" ]]; then
		${EXEC_BASEDIR}/download-keys.sh ${key_type} ${soc} chipset
	fi

	if [ ${blxname} == "bl2" ] && [ ${build_type} == "normal" ]; then
		if [ -z ${chip_acs} ] || [ ! -f ${chip_acs} ]; then
			echo "chip_acs ${chip_acs} invalid"
			exit 1
		fi
		dd if=${chip_acs} of=${BASEDIR_BUILD}/csinit-params.bin conv=notrunc  &> /dev/null
		dd if=${input} of=${BASEDIR_BUILD}/${blxname}-payload.bin conv=notrunc  &> /dev/null
		process_ddrfw ${BASEDIR_TOP} ${BASEDIR_BUILD} ${ddr_type}
		${EXEC_BASEDIR}/gen-boot-blobs.sh ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc} ${chipset_variant_suffix}
	elif [ ${blxname} == "bl2" ] && [ ${build_type} == "bl2-only" ]; then
		dd if=${input} of=${BASEDIR_BUILD}/${blxname}-payload.bin conv=notrunc  &> /dev/null
		${EXEC_BASEDIR}/gen-boot-blob-bl2-only.sh ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc} ${chipset_variant_suffix}
	elif [ ${blxname} == "bl2" ] && [ ${build_type} == "bl2-final" ]; then
		if [ -z ${chip_acs} ] || [ ! -f ${chip_acs} ]; then
			echo "chip_acs ${chip_acs} invalid"
			exit 1
		fi
		dd if=${chip_acs} of=${BASEDIR_BUILD}/csinit-params.bin conv=notrunc  &> /dev/null
		process_ddrfw ${BASEDIR_TOP} ${BASEDIR_BUILD} ${ddr_type}
		dd if=${input} of=${BASEDIR_BUILD}/bb1st${FEAT_BL2_TEMPLATE_TYPE}${chipset_variant_suffix}.bin.bl2-only conv=notrunc  &> /dev/null
		${EXEC_BASEDIR}/gen-boot-blob-bl2-final.sh ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc} ${chipset_variant_suffix}
	elif [ ${blxname} == "bl2e" ] || [ ${blxname} == "bl2x" ]; then
		dd if=${input} of=${BASEDIR_BUILD}/${blxname}-payload.bin conv=notrunc  &> /dev/null
		${EXEC_BASEDIR}/gen-boot-blobs.sh ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc} ${chipset_variant_suffix}
	elif [ ${blxname} == "bl31" ] || [ ${blxname} == "bl32" ] || [ ${blxname} == "bl40" ]; then
		dd if=${input} of=${BASEDIR_BUILD}/${blxname}-payload.bin conv=notrunc  &> /dev/null
		if [ ${blxname} == "bl31" ]; then
			${EXEC_BASEDIR}/pack_aucpu_key.sh ${blxname:2:2} ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc}
		fi
		${EXEC_BASEDIR}/gen-bl3x-blobs.sh ${blxname:2:2} ${BASEDIR_BUILD} ${BASEDIR_BUILD} ${chipset_name} ${key_type} ${soc}
	fi

	if [ ${blxname} == "bl2" ]; then
		if [ ${build_type} == "bl2-only" ]; then
			cp ${BASEDIR_BUILD}/bb1st${FEAT_BL2_TEMPLATE_TYPE}${chipset_variant_suffix}.bin.bl2-only $output
		else
			cp ${BASEDIR_BUILD}/bb1st${FEAT_BL2_TEMPLATE_TYPE}${chipset_variant_suffix}.bin${postfix} $output
		fi
	else
		cp ${BASEDIR_BUILD}/blob-${blxname}.bin${postfix} $output
	fi
}

rm -rf ${BASEDIR_BUILD}
sign_blx $@
rm -rf ${BASEDIR_BUILD}
