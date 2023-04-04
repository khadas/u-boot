#!/bin/bash

SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}
declare -i TOTAL_CFG=1

arry=`find ./include/asm/ -name "regs.h" | cut  -d  '/' -f 4 | cut -d '-' -f 2- | sort`
arry=(${arry//' '/})
#23.03 arry list: a1 a5 c1 c2 c3 g12a g12b p1 s4 s5 sc2 t3 t5m t5w t7
echo "AML SOC LIST: ${arry[*]}"
RESULT=$RESULT'\n'"--------------------------------------------\n"
RESULT=$RESULT"############## Aml RAMDUMP bl33z.bin #############\n"
#RESULT=$RESULT"Amlogic bl33/v2019 SoC: "${arry[*]}'\n'

skiped=("a1" "c1" "c2" "c3" "g12a" "g12b" "sm1" "t5w")
echo "AML SOC SKIP: ${skiped[*]}"

if [ ! -z "$1" ]; then
	echo "Compile bl33z only for: $1"
	filter=`echo $1 | cut -d '_' -f 1`

	if [[ "${skiped[@]}"  =~ "$filter" ]]; then
		RESULT=$RESULT"Soc ("$filter") is in skip list, skip."'\n'
	elif [[ "${arry[@]}"  =~ "$filter" ]]; then
		rm -fr ./build/*
		make PLAT=$filter
		if [ $? != 0 ]; then
			RESULT=$RESULT"$TOTAL_CFG."'\t'$filter'\t\033[41;37m--- build failed\033[0m\n'
		else
			RESULT=$RESULT"$TOTAL_CFG."'\t'$filter'\t--- build pass\t\n'
		fi
	else
		RESULT=$RESULT"Soc ("$filter") is not in support list, skip."'\n'
	fi
	echo -e $RESULT
	exit 0
fi

for((i=0;i<${#skiped[@]};i++)); do
	for((j=0;j<${#arry[@]};j++)); do
		if [ "${arry[$j]}" = "${skiped[$i]}" ]; then
			arry[$j]="NULL"
			break
		fi
	done
done
arry=( ${arry[*]/'NULL'} )
echo "FINALE BUILD: ${arry[*]}"

RESULT=$RESULT"Build bl33z.bin for SoC: "${arry[*]}'\n''\n'
for item in ${arry[@]}
{
	make PLAT=$item

	if [ $? != 0 ]; then
		RESULT=$RESULT"$TOTAL_CFG."'\t'${item}'\t\033[41;37m--- build failed\033[0m\n'
	else
		RESULT=$RESULT"$TOTAL_CFG."'\t'${item}'\t--- build pass\t\n'
	fi
	rm -fr ./build/*
	TOTAL_CFG+=1;
}

echo -e $RESULT
if [ $# -gt 0 ]; then
	echo -e $*
fi
