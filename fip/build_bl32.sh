#!/bin/bash

function build_bl32() {
	echo -n "Build bl32...Please wait... "
	local target="$1/bl32.img"
	local target2="$1/bl32.bin"
	# $1: src_folder, $2: bin_folder, $3: soc
	cd $1
	/bin/bash build.sh $3 ${CONFIG_CAS}
	if [ $? != 0 ]; then
		cd ${MAIN_FOLDER}
		echo "Error: Build bl32 failed... abort"
		exit -1
	fi
	cd ${MAIN_FOLDER}
	cp ${target} $2 -f
	if [ "$ADVANCED_BOOTLOADER" == "1" ]; then
		$1/tools/scripts/pack_dtb.py \
			--dev-rsk fip/$3/keys/${CONFIG_AMLOGIC_KEY_TYPE}/$3/chipset/bl32/rsa/${CONFIG_CHIPSET_NAME}/bl32-rsk-rsa-priv.pem \
			--cert "fip/$3/keys/${CONFIG_AMLOGIC_KEY_TYPE}/$3/chipset/cert-template/${CONFIG_CHIPSET_NAME}/bl32-fw-cert.bin" \
			--in ${target2}

		cp ${target2} $2 -f
	fi
	echo "done"
	return
}
