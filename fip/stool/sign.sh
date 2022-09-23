#!/bin/bash

user_input=
user_rsa=
user_aes=
user_out=
soc=
hash_ver=2
encryption_option=
kernel_encryption=
uimage_encryption=
postfix="signed"
bl40_option=

while getopts "s:h:z:p:r:a:um:no:" opt; do
  case $opt in
    s) readonly soc="$OPTARG" ;;
    z) readonly user_package="$OPTARG" ;;
    p) readonly user_input="$OPTARG" ;;
    r) readonly user_rsa="$OPTARG" ;;
    a) readonly user_aes="$OPTARG" ;;
    o) readonly user_out="$OPTARG" ;;
    m) readonly bl40_option="$OPTARG" ;;
    n) readonly encryption_option="-n" ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 2
      ;;
  esac
done

tool_type=gxl
if [ ${soc} == "gxl" ]; then
hash_ver=1
fi

if [ ${soc} == "gxl" ] || [ ${soc} == "txlx" ] || [ ${soc} == "axg" ]; then
soc_type=gxl
tool_type=gxl
fi

if [ ${soc} == "g12a" ] || [ ${soc} == "g12b" ]; then
soc_type=g12a
tool_type=g12a
fi

if [ ${soc} == "tl1" ] || [ ${soc} == "tm2" ]; then
soc_type=tl1
tool_type=tl1
fi

if [ ${soc} == "c1" ]; then
soc_type=c1
tool_type=c1
hash_ver=3
fi

if [ ${soc} == "t5" ] || [ ${soc} == "t5d" ] || [ ${soc} == "t5w" ]; then
soc_type=t5
tool_type=c1
hash_ver=3
fi

readonly tools_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
uboot_encrypt_signed="${tools_dir}/amlogic-sign-${soc_type}.sh"
kernel_encrypt_signed="${tools_dir}/signing-tool-${tool_type}/sign-boot-${tool_type}.sh"
uimage_encrypt_signed="${tools_dir}/signing-tool-${tool_type}/sign-boot-${tool_type}.sh"
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
  "$uboot_encrypt_signed" -p ${INPUTDIR} -r ${RSAKEYDIR} -a ${AESKEYDIR} -o ${OUTPUTDIR} -h ${hash_ver} -s ${soc} -b ${ARBCONFIG} $encryption_option $bl40_option
fi

if [ -z $encryption_option ]; then
  postfix="signed.encrypted"
  kernel_encryption="-a ${AESKEYDIR}/kernelaeskey --iv ${AESKEYDIR}/kernelaesiv"
  uimage_encryption="-a ${AESKEYDIR}/bl2aeskey --iv ${AESKEYDIR}/bl2aesiv"
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
    -i $image_name                         \
    -k ${RSAKEYDIR}/kernelkey.pem          \
    $kernel_encryption                     \
    -o ${OUTPUTDIR}/$item.${postfix}       \
    -h ${hash_ver}
  fi
}

#check and sign&encrypt rtos/NBG
arry=("rtos-uImage" "nbg-uImage");
image_name="";
for item in ${arry[@]}
{
  image_name=${INPUTDIR}/$item
  if [ -e $image_name ]; then
    echo
    echo "$image_name signing process ..."
    "$uimage_encrypt_signed" --sign-uimage \
    -i $image_name                         \
    -k ${RSAKEYDIR}/bl2.pem                \
    $uimage_encryption                     \
    -o ${OUTPUTDIR}/$item.${postfix}
  fi
}

if [ -d $temp_dir ]; then
	rm -fr $temp_dir
fi
