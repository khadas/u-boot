#!/bin/bash

user_input=
user_rsa=
user_aes=
user_out=
soc=
hash_ver=2

while getopts "s:h:z:p:r:a:uno:" opt; do
  case $opt in
    s) readonly soc="$OPTARG" ;;
    z) readonly user_package="$OPTARG" ;;
    p) readonly user_input="$OPTARG" ;;
    r) readonly user_rsa="$OPTARG" ;;
    a) readonly user_aes="$OPTARG" ;;
    o) readonly user_out="$OPTARG" ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 2
      ;;
  esac
done

tool_type=gxl

if [ ${soc} == "g12a" ]; then
tool_type=g12a
fi

if [ ${soc} == "g12b" ]; then
tool_type=g12a
fi

if [ $soc == "gxl" ]; then
hash_ver=1
fi

readonly tools_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
uboot_encrypt_signed="${tools_dir}/amlogic-sign-${tool_type}.sh"
kernel_encrypt_signed="${tools_dir}/signing-tool-${tool_type}/sign-boot-${tool_type}.sh"
ARBCONFIG="${tools_dir}/fw_arb.txt"

INPUTDIR=
OUTPUTDIR=
RSAKEYDIR=
AESKEYDIR=
temp_dir=

if [[ -f $user_package ]]; then  
	temp_dir="$(dirname $user_package )"/"$(basename $user_package)"-`date +%Y%m%d-%H%M%S`
	mkdir -p $temp_dir
	if [[ -d $temp_dir ]]; then  
		unzip $user_package -d $temp_dir >& /dev/null
		if [[ -d $user_input ]]; then
			echo "error!user package and input conflicts! Only one set is legal!"
			exit 1;
		else
			user_input=$temp_dir
		fi
	fi
fi

if [[ -d $user_input ]]; then  
	INPUTDIR=$user_input
fi
if [[ -d $user_rsa ]]; then
  RSAKEYDIR=$user_rsa
fi
if [[ -d $user_aes ]]; then  
	AESKEYDIR=$user_aes
fi
if [[ ! -z $user_out ]]; then  
	OUTPUTDIR=$user_out
fi

mkdir -p ${OUTPUTDIR}

#to sign uboot and output to ${OUTPUTDIR}
if [ -e ${INPUTDIR}/bl2_new.bin ]; then
  echo
  echo "$user_package signing process ..."
  "$uboot_encrypt_signed" -p ${INPUTDIR} -r ${RSAKEYDIR} -a ${AESKEYDIR} -o ${OUTPUTDIR} -h ${hash_ver} -s ${tool_type} -b ${ARBCONFIG}
fi

#check and sign&encrypt kernel/recovery/dtb image
arry=("boot.img" "recovery.img" "dtb.img" "dt.img");
image_name="";
for item in ${arry[@]}
{
	image_name=${INPUTDIR}/$item
	if [ -e $image_name ]; then
    echo
    echo "$image_name signing process ..."
    "$kernel_encrypt_signed" --sign-kernel \
    -i $image_name                \
    -k ${RSAKEYDIR}/kernelkey.pem \
    -a ${RSAKEYDIR}/kernelaeskey  \
    --iv ${RSAKEYDIR}/kernelaesiv \
    -o ${OUTPUTDIR}/$item.signed.encrypted \
    -h ${hash_ver}
  fi
}

if [ -d $temp_dir ]; then
	rm -fr $temp_dir
fi
