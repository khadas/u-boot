#!/bin/bash

# static
declare -a BLX_NAME=("bl2" "bl30" "bl31" "bl32")

declare -a BLX_SRC_FOLDER=("bl2/src" "bl30/src" "bl31/src" "bl32/src" "bl33")
declare -a BLX_BIN_FOLDER=("bl2/bin" "bl30/bin" "bl31/bin" "bl32/bin")
declare -a BLX_BIN_NAME=("bl2.bin" "bl30.bin" "bl31.bin" "bl32.bin")
declare -a BLX_IMG_NAME=("NULL" "NULL" "bl31.img" "bl32.img")
declare -a BLX_NEEDFUL=("true" "true" "true" "false")

declare -a BLX_SRC_GIT=("bootloader/spl" "firmware/scp" "ARM-software/arm-trusted-firmware" "OP-TEE/optee_os" "uboot")
declare -a BLX_BIN_GIT=("firmware/bin/bl2" "firmware/bin/bl30" "firmware/bin/bl31" "firmware/bin/bl32")

# blx priority. null: default, source: src code, others: bin path
declare -a BIN_PATH=("null" "null" "null" "null")

# variables
declare -a CUR_REV # current version of each blx
declare -a BLX_READY=("false", "false", "false", "false") # blx build/get flag

# package variables
declare BL33_COMPRESS_FLAG=""
declare BL3X_SUFFIX="bin"
declare V3_PROCESS_FLAG=""
declare FIP_ARGS=""
declare AML_BL2_NAME=""
declare AML_KEY_BLOB_NAME=""
declare FIP_BL32_PROCESS=""

BUILD_PATH=${FIP_BUILD_FOLDER}
