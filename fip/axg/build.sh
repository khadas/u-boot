#!/bin/bash

# include uboot pre-build macros
#declare CONFIG_FILE=("${buildtree}/.config")
#declare AUTOCFG_FILE=("${buildtree}/include/autoconf.mk")

function init_vari() {
	#source ${CONFIG_FILE} &> /dev/null # ignore warning/error
	#source ${AUTOCFG_FILE} &> /dev/null # ignore warning/error

	AML_BL2_NAME="bl2.bin"
	AML_KEY_BLOB_NAME="aml-user-key.sig"

	if [ "y" == "${CONFIG_AML_SECURE_BOOT_V3}" ]; then
		V3_PROCESS_FLAG="--level v3"
	fi

	if [ "y" == "${CONFIG_AML_BL33_COMPRESS_ENABLE}" ]; then
		BL33_COMPRESS_FLAG="--compress lz4"
	fi

	if [ "y" == "${CONFIG_FIP_IMG_SUPPORT}" ]; then
		BL3X_SUFFIX="img"
	fi
}

function fix_blx() {
	#bl2 file size 41K, bl21 file size 3K (file size not equal runtime size)
	#total 44K
	#after encrypt process, bl2 add 4K header, cut off 4K tail

	#bl30 limit 41K
	#bl301 limit 12K
	#bl2 limit 41K
	#bl21 limit 3K, but encrypt tool need 48K bl2.bin, so fix to 7168byte.

	declare blx_bin_limit=0
	declare blx01_bin_limit=0
	declare -i blx_size=0
	declare -i zero_size=0

	#$7:name flag
	if [ "$7" = "bl30" ]; then
		blx_bin_limit=40960   # PD#132613 2016-10-31 update, 41984->40960
		blx01_bin_limit=13312 # PD#132613 2016-10-31 update, 12288->13312
	elif [ "$7" = "bl2" ]; then
		blx_bin_limit=41984
		blx01_bin_limit=7168
	else
		echo "blx_fix name flag not supported!"
		exit 1
	fi

	# blx_size: blx.bin size, zero_size: fill with zeros
	blx_size=`du -b $1 | awk '{print int($1)}'`
	if [ $blx_size -gt $blx_bin_limit ]; then
		echo "Error: $7 ($1) too big. $blx_size > $blx_bin_limit"
		exit 1
	fi

	zero_size=$blx_bin_limit-$blx_size
	dd if=/dev/zero of=$2 bs=1 count=$zero_size
	cat $1 $2 > $3
	rm $2

	blx_size=`du -b $4 | awk '{print int($1)}'`
	zero_size=$blx01_bin_limit-$blx_size
	dd if=/dev/zero of=$2 bs=1 count=$zero_size
	cat $4 $2 > $5

	cat $3 $5 > $6

	rm $2
}

function cleanup() {
	rm -f ${BUILD_PATH}/bl*.enc ${BUILD_PATH}/bl2*.sig
	rm -f ${BUILD_PATH}/boot_new.bin
}

function encrypt_step() {
	dbg "encrypt: $@"
	local ret=0
	./${FIP_FOLDER}${CUR_SOC}/aml_encrypt_${CUR_SOC} $@
	ret=$?
	if [ 0 != $ret ]; then
		echo "Err! aml_encrypt_${CUR_SOC} return $ret"
		exit $ret
	fi
}

function encrypt() {
	# part1 for new soc flow(besides gxb/gxtvbb)
	if [ "y" == "${CONFIG_AML_SECURE_BOOT_V3}" ]; then
		# v3 flow
		encrypt_step --bl3sig  --input ${BUILD_PATH}/bl30_new.bin    --output ${BUILD_PATH}/bl30_new.bin.enc ${V3_PROCESS_FLAG} --type bl30
		encrypt_step --bl3sig  --input ${BUILD_PATH}/bl31.${BL3X_SUFFIX}  --output ${BUILD_PATH}/bl31.${BL3X_SUFFIX}.enc ${V3_PROCESS_FLAG} --type bl31
		if [ "${FIP_BL32}" == "${BUILD_PATH}/bl32.${BL3X_SUFFIX}" ]; then
			encrypt_step --bl3sig  --input ${BUILD_PATH}/bl32.${BL3X_SUFFIX} --output ${BUILD_PATH}/bl32.${BL3X_SUFFIX}.enc ${V3_PROCESS_FLAG} --type bl32
		fi
		encrypt_step --bl3sig  --input ${BUILD_PATH}/bl33.bin ${BL33_COMPRESS_FLAG} --output ${BUILD_PATH}/bl33.bin.enc ${V3_PROCESS_FLAG} --type bl33
	else
		# v2 flow
		encrypt_step --bl3enc  --input ${BUILD_PATH}/bl30_new.bin   --output ${BUILD_PATH}/bl30_new.bin.enc
		encrypt_step --bl3enc  --input ${BUILD_PATH}/bl31.${BL3X_SUFFIX} --output ${BUILD_PATH}/bl31.${BL3X_SUFFIX}.enc
		if [ "${FIP_BL32}" == "${BUILD_PATH}/bl32.${BL3X_SUFFIX}" ]; then
			encrypt_step --bl3enc  --input ${BUILD_PATH}/bl32.${BL3X_SUFFIX} --output ${BUILD_PATH}/bl32.${BL3X_SUFFIX}.enc
		fi
		encrypt_step --bl3enc  --input ${BUILD_PATH}/bl33.bin       --output ${BUILD_PATH}/bl33.bin.enc   ${BL33_COMPRESS_FLAG}
	fi

	encrypt_step --bl2sig  --input ${BUILD_PATH}/bl2_new.bin   --output ${BUILD_PATH}/bl2.n.bin.sig

	encrypt_step --bootmk  --output ${BUILD_PATH}/u-boot.bin \
		--bl2   ${BUILD_PATH}/bl2.n.bin.sig  --bl30  ${BUILD_PATH}/bl30_new.bin.enc  \
		--bl31  ${BUILD_PATH}/bl31.${BL3X_SUFFIX}.enc ${FIP_BL32_PROCESS} --bl33  ${BUILD_PATH}/bl33.bin.enc ${V3_PROCESS_FLAG}
	# part1 end
	# part1 for old soc (gxb/gxtvbb)
	#encrypt_step --bootsig --input ${BUILD_PATH}/boot_new.bin --output ${BUILD_PATH}/u-boot.bin

	if [ "y" == "${CONFIG_AML_CRYPTO_UBOOT}" ]; then
		# new soc only (besides gxb/gxtvbb)
		encrypt_step --efsgen --amluserkey ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/${AML_KEY_BLOB_NAME} \
				--output ${BUILD_PATH}/u-boot.bin.encrypt.efuse ${V3_PROCESS_FLAG}

		# for all soc
		encrypt_step --bootsig --input ${BUILD_PATH}/u-boot.bin --amluserkey ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/${AML_KEY_BLOB_NAME} \
			--aeskey enable --output ${BUILD_PATH}/u-boot.bin.encrypt ${V3_PROCESS_FLAG}
	fi

	if [ "y" == "${CONFIG_AML_CRYPTO_IMG}" ]; then
		encrypt_step --imgsig --input ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/boot.img --amluserkey ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/${AML_KEY_BLOB_NAME} --output ${BUILD_PATH}/boot.img.encrypt
	fi

	return
}

function build_fip() {
	fix_blx \
		${BUILD_PATH}/bl30.bin \
		${BUILD_PATH}/zero_tmp \
		${BUILD_PATH}/bl30_zero.bin \
		${BUILD_PATH}/bl301.bin \
		${BUILD_PATH}/bl301_zero.bin \
		${BUILD_PATH}/bl30_new.bin \
		bl30

	# acs_tool process ddr timing and configurable parameters
	python ${FIP_FOLDER}/acs_tool.pyc ${BUILD_PATH}/${AML_BL2_NAME} ${BUILD_PATH}/bl2_acs.bin ${BUILD_PATH}/acs.bin 0

	# fix bl2/bl21
	fix_blx \
		${BUILD_PATH}/bl2_acs.bin \
		${BUILD_PATH}/zero_tmp \
		${BUILD_PATH}/bl2_zero.bin \
		${BUILD_PATH}/bl21.bin \
		${BUILD_PATH}/bl21_zero.bin \
		${BUILD_PATH}/bl2_new.bin \
		bl2

	# v2: bl30/bl301 merged since 2016.03.22
	FIP_ARGS="--bl30 ${BUILD_PATH}/bl30_new.bin --bl31 ${BUILD_PATH}/bl31.${BL3X_SUFFIX}"

	if [ "y" == "${CONFIG_NEED_BL32}" ]; then
		FIP_BL32="`find ${BUILD_PATH} -name "bl32.${BL3X_SUFFIX}"`"
		if [ "${FIP_BL32}" == "${BUILD_PATH}/bl32.${BL3X_SUFFIX}" ]; then
			FIP_ARGS="${FIP_ARGS}"" --bl32 ${BUILD_PATH}/bl32.${BL3X_SUFFIX}"
			FIP_BL32_PROCESS=" --bl32 ${BUILD_PATH}/bl32.${BL3X_SUFFIX}.enc"
		fi
	fi
	FIP_ARGS="${FIP_ARGS}"" --bl33 ${BUILD_PATH}/bl33.bin"

	# create fip.bin
	./${FIP_FOLDER}/fip_create ${FIP_ARGS} ${BUILD_PATH}/fip.bin
	./${FIP_FOLDER}/fip_create --dump ${BUILD_PATH}/fip.bin

	# build final bootloader
	cat ${BUILD_PATH}/bl2_new.bin ${BUILD_PATH}/fip.bin > ${BUILD_PATH}/boot_new.bin

	return
}

function copy_other_soc() {
	cp ${UBOOT_SRC_FOLDER}/build/scp_task/bl301.bin ${BUILD_PATH} -f
	cp ${UBOOT_SRC_FOLDER}/build/${BOARD_DIR}/firmware/bl21.bin ${BUILD_PATH} -f
	cp ${UBOOT_SRC_FOLDER}/build/${BOARD_DIR}/firmware/acs.bin ${BUILD_PATH} -f
	# todo. cp bl40?
}

function package() {
	# BUILD_PATH without "/"
	x=$((${#BUILD_PATH}-1))
	if [ "\\" == "${BUILD_PATH:$x:1}" ] || [ "/" == "${BUILD_PATH:$x:1}" ]; then
		BUILD_PATH=${BUILD_PATH:0:$x}
	fi

	init_vari $@
	build_fip $@

	if [ "y" == "${CONFIG_AML_SIGNED_UBOOT}" ]; then

		mv -f ${BUILD_PATH}/bl33.bin  ${BUILD_PATH}/bl33.bin.org
		encrypt_step --bl3sig  --input ${BUILD_PATH}/bl33.bin.org --output ${BUILD_PATH}/bl33.bin.org.lz4 --compress lz4 --level v3 --type bl33
		#get LZ4 format bl33 image from bl33.bin.enc with offset 0x720
		dd if=${BUILD_PATH}/bl33.bin.org.lz4 of=${BUILD_PATH}/bl33.bin bs=1 skip=1824 >& /dev/null

		list_pack="${BUILD_PATH}/bl2_new.bin ${BUILD_PATH}/bl30_new.bin ${BUILD_PATH}/bl31.img ${BUILD_PATH}/bl32.img ${BUILD_PATH}/bl33.bin"
		u_pack=${BUILD_FOLDER}/"$(basename ${BOARD_DIR})"-u-boot.aml.zip
		zip -j $u_pack ${list_pack} >& /dev/null

		${FIP_FOLDER}/stool/sign.sh -s ${CUR_SOC} -z $u_pack -o ${BUILD_FOLDER} -r ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/aml-key -a ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/aml-key

		if [ "y" == "${CONFIG_AML_CRYPTO_IMG}" ]; then
				${FIP_FOLDER}/stool/sign.sh -s ${CUR_SOC} -p ${UBOOT_SRC_FOLDER}/${BOARD_DIR} -o ${BUILD_FOLDER} -r ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/aml-key -a ${UBOOT_SRC_FOLDER}/${BOARD_DIR}/aml-key
		fi

	else
		encrypt $@
	fi
	#copy_file
	#cleanup
	echo "Bootloader build done!"
}
