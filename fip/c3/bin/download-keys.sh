 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

REPO_ADDR_HEAD="ssh://scgit.amlogic.com:29418/security"

key_type=$1
soc=$2
key_name=$3
key_path=$4

#chipset repo-name
CHIPSET_REPO_NAME[0]="/keys/${key_type}/${soc}/chipset/bl2/aes"
CHIPSET_REPO_NAME[1]="/keys/${key_type}/${soc}/chipset/bl2/rsa"
CHIPSET_REPO_NAME[2]="/keys/${key_type}/${soc}/chipset/bl31/aes"
CHIPSET_REPO_NAME[3]="/keys/${key_type}/${soc}/chipset/bl31/rsa"
CHIPSET_REPO_NAME[4]="/keys/${key_type}/${soc}/chipset/bl32/aes"
CHIPSET_REPO_NAME[5]="/keys/${key_type}/${soc}/chipset/bl32/rsa"
CHIPSET_REPO_NAME[6]="/keys/${key_type}/${soc}/chipset/bl40/aes"
CHIPSET_REPO_NAME[7]="/keys/${key_type}/${soc}/chipset/bl40/rsa"
CHIPSET_REPO_NAME[8]="/keys/${key_type}/${soc}/chipset/cert-template"

#device repo-name
DEVICE_REPO_FOLDER="/keys/${key_type}/${soc}/device/"
DEVICE_REPO_NAME[0]="boot-blobs"
DEVICE_REPO_NAME[1]="fip"

if [ ${key_name} == "chipset"  ]; then
	for NAME in ${CHIPSET_REPO_NAME[@]};
	do
		if [ ! -d ${BASEDIR_TOP}/${NAME} ]; then
			git clone ${REPO_ADDR_HEAD}${NAME} ${BASEDIR_TOP}/${NAME}
		fi
	done
elif [ ${key_name} == "device"  ]; then
	for NAME in ${DEVICE_REPO_NAME[@]};
	do
		if [ ! -d ${key_path}/${NAME} ]; then
			git clone ${REPO_ADDR_HEAD}${DEVICE_REPO_FOLDER}${NAME} ${key_path}/${NAME}
		fi
	done
fi