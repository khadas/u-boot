#!/bin/bash

SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}
autobuild=${SCRIPT_PATH}/build-`date +%Y%m%d%H%M%S`
#remove all build/xxxxx folders
rm -fr ./build-*
#mkdir new build/xxxxx folders
mkdir -p $autobuild
echo $autobuild

RESULT="Amlogic bl33z auto build --- $autobuild"'\n'
declare -i TOTAL_CFG=1

arry=`find ./include/asm/ -name "regs.h" | cut  -d  '/' -f 4 | cut -d '-' -f 2- | sort`
arry=(${arry//' '/})
#arry: p1 a1 g12a sc2 c3 t5m t5w s4 c2 t3 c1 s5 t7 g12b a5
echo "AML SOC LIST: ${arry[*]}"

skiped=("c1" "c2" "c3" "g12a" "g12b")
echo "AML SOC SKIP: ${skiped[*]}"

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

RESULT='\n\n'"Build bl33z for SoC: "${arry[*]}'\n''\n'
echo -e $RESULT
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
