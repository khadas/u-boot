#!/bin/bash

# static
declare BLX_BIN_SUB_CHIP="${CONFIG_CHIPSET_NAME}"

if [ -n "${SCRIPT_ARG_CHIPSET_VARIANT}" ]; then
	declare CHIPSET_VARIANT_SUFFIX=".${SCRIPT_ARG_CHIPSET_VARIANT}"
elif [ -n "${CONFIG_CHIPSET_VARIANT}" ]; then
	declare CHIPSET_VARIANT_SUFFIX=".${CONFIG_CHIPSET_VARIANT}"
else
	declare CHIPSET_VARIANT_SUFFIX=""
fi
declare -a BLX_NAME=("bl2"	\
		     "bl2"	\
		     "bl2e"	\
		     "bl2e"	\
		     "bl2x"	\
		     "bl31"	\
		     "bl32"	\
		     "bl40"	\
		     "bl30")

declare -a BLX_SRC_FOLDER=("bl2/core"		\
			   "bl2/core"		\
			   "bl2/ree"		\
			   "bl2/ree"		\
			   "bl2/tee"		\
			   "bl31_1.3/src"	\
			   "bl32_3.8/src"	\
			   "NULL"		\
			   "bl30/src_ao"	\
			   "bl33")

declare -a BLX_BIN_FOLDER=("bl2/bin"		\
			   "bl2/bin"		\
			   "bl2/bin"		\
			   "bl2/bin"		\
			   "bl2/bin"		\
			   "bl31_1.3/bin"	\
			   "bl32_3.8/bin"	\
			   "bl40/bin"		\
			   "bl30/bin_ao")

declare -a BLX_BIN_NAME=("bb1st.sto${CHIPSET_VARIANT_SUFFIX}.bin.signed"     \
			 "bb1st.usb${CHIPSET_VARIANT_SUFFIX}.bin.signed"     \
			 "blob-bl2e.sto${CHIPSET_VARIANT_SUFFIX}.bin.signed" \
			 "blob-bl2e.usb${CHIPSET_VARIANT_SUFFIX}.bin.signed" \
			 "blob-bl2x.bin.signed"                              \
			 "blob-bl31.bin.signed"                              \
			 "blob-bl32.bin.signed"                              \
			 "blob-bl40.bin.signed"                              )

declare -a BLX_BIN_SIZE=("407552"	\
			 "407552"	\
			 "74864"	\
			 "74864"	\
			 "66672"	\
			 "266240"	\
			 "528384"	\
			 "102400"	\
			 "NULL")

declare BL30_BIN_SIZE="65536"
declare BL33_BIN_SIZE="1572864"
declare DEV_ACS_BIN_SIZE="28672"
declare -a BLX_RAWBIN_NAME=("bl2.bin.sto"	\
			    "bl2.bin.usb"	\
			    "bl2e.bin.sto"	\
			    "bl2e.bin.usb"	\
			    "bl2x.bin"		\
			    "bl31.bin"		\
			    "bl32.bin"		\
			    "bl40.bin"		\
			    "NULL")

declare -a BLX_IMG_NAME=("NULL"	\
			 "NULL"	\
			 "NULL"	\
			 "NULL"	\
			 "NULL"	\
			 "NULL"	\
			 "NULL"	\
			 "NULL")

declare -a BLX_NEEDFUL=("true"	\
			"true"	\
			"true"	\
			"true"	\
			"true"	\
			"ture"	\
			"true"	\
			"true")

declare -a BLX_SRC_GIT=("bootloader/amlogic-advanced-bootloader/core" \
			"bootloader/amlogic-advanced-bootloader/core" \
			"bootloader/amlogic-advanced-bootloader/ree" \
			"bootloader/amlogic-advanced-bootloader/ree" \
			"bootloader/amlogic-advanced-bootloader/tee" \
			"ARM-software/arm-trusted-firmware" \
			"OP-TEE/optee_os" \
			"firmware/aocpu" \
			"uboot")

declare -a BLX_BIN_GIT=("firmware/bin/bl2" \
			"firmware/bin/bl2" \
			"firmware/bin/bl2" \
			"firmware/bin/bl2" \
			"firmware/bin/bl2" \
			"firmware/bin/bl31" \
			"firmware/bin/bl32" \
			"firmware/bin/b40")

# blx priority. null: default, source: src code, others: bin path
declare -a BIN_PATH=("null"	\
		     "null"	\
		     "null"	\
		     "null"	\
		     "null"	\
		     "null"	\
		     "null"	\
		     "null"	\
		     "source")

# variables
declare -a CUR_REV # current version of each blx
declare -a BLX_READY=("false",	\
		      "false",	\
		      "false",	\
		      "false",	\
		      "false",	\
		      "false",	\
		      "false",	\
		      "false",	\
		      "false") # blx build/get flag

# package variables
declare BL33_COMPRESS_FLAG=""
declare BL3X_SUFFIX="bin"
declare V3_PROCESS_FLAG=""
declare FIP_ARGS=""
declare AML_BL2_NAME=""
declare AML_KEY_BLOB_NAME=""
declare FIP_BL32_PROCESS=""
declare BOOT_SIG_FLAG=""
declare EFUSE_GEN_FLAG=""
#declare DDRFW_TYPE="${CONFIG_DDRFW_TYPE}"
declare DDRFW_TYPE=""

BUILD_PATH=${FIP_BUILD_FOLDER}
BUILD_PAYLOAD=${FIP_BUILD_FOLDER}/payload
CHIPSET_TEMPLATES_PATH="soc/templates"
CONFIG_DDR_FW=0
DDR_FW_NAME="aml_ddr.fw"

CONFIG_NEED_BL32=y
ADVANCED_BOOTLOADER=1
