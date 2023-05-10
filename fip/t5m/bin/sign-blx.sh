 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#
#BASEDIR_BUILD="${BASEDIR_TOP}/output"
BASEDIR_BUILD="${BASEDIR_TOP}/`date +%Y%m%d%H%M%S%N`"
postfix=.signed
declare -a BLX_BIN_SIZE=("183200" "65536" "65536" "2048" "212992" "262144" "524288"  "98304")

declare CHIPACS_SIZE=${BLX_BIN_SIZE[3]}
declare DDRFW_SIZE=${BLX_BIN_SIZE[4]}

function split_ddrfw_from_chipacs() {
	local input=$1
	local output1=$2
	local output2=$3
	local size=`expr ${CHIPACS_SIZE} + ${DDRFW_SIZE}`
	local input_size=`stat -c %s ${input}`

	if [ $input_size -ne ${size} ]; then
		echo "$input is not chipacs and ddrfw merge !!!"
		return
	fi
	dd if=${input} of=${output1}.tmp bs=1 count=${CHIPACS_SIZE}
	dd if=${input} of=${output2}.tmp skip=${CHIPACS_SIZE} bs=1 count=${DDRFW_SIZE}
	cat ${output1}.tmp > ${output1}
	cat ${output2}.tmp > ${output2}
	rm -rf ${output1}.tmp ${output2}.tmp

	return
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
		chipset_name="azp1"
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
			if [[ "${chipset_variant}" =~ "nocs" ]]; then
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

	if [ -z ${key_type} ]; then
		key_type="dev-keys"
	fi

	if [ -z ${soc} ]; then
		soc="s5"
	fi

	if [ -z ${build_type} ]; then
        build_type=normal
    fi

	mkdir ${BASEDIR_BUILD}

	if [ ${blxname} == "bl2" ] || [ ${blxname} == "bl2e" ] || [ ${blxname} == "bl2x" ]; then
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2-payload.bin bs=${BLX_BIN_SIZE[0]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2e-payload.bin bs=${BLX_BIN_SIZE[1]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/bl2x-payload.bin bs=${BLX_BIN_SIZE[2]} count=1  &> /dev/null
		dd if=/dev/zero of=${BASEDIR_BUILD}/csinit-params.bin bs=${BLX_BIN_SIZE[3]} count=1  &> /dev/null
		#dd if=/dev/zero of=${BASEDIR_BUILD}/ddr-fwdata.bin bs=${BLX_BIN_SIZE[4]} count=1  &> /dev/null
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

	${EXEC_BASEDIR}/download-keys.sh ${key_type} ${soc} chipset

	ddrfw_split_flag=0

	if [ ${blxname} == "bl2" ] && [ ${build_type} == "normal" ]; then
		if [ -z ${chip_acs} ] || [ ! -f ${chip_acs} ]; then
			echo "chip_acs ${chip_acs} invalid"
			exit 1
		fi

		dd if=${chip_acs} of=${BASEDIR_BUILD}/csinit-params.bin conv=notrunc  &> /dev/null
		dd if=${input} of=${BASEDIR_BUILD}/${blxname}-payload.bin conv=notrunc  &> /dev/null

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
