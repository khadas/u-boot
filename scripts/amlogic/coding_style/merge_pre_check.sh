#!/bin/bash

# Amlogic gerrit code auto-fix script
# Author: xiaobo.gu@amlogic.com
# Init version: 2015.05.01

function check_defconfig() {
	err_cnt=0

	for line in `git status | grep "_defconfig"`; do
		if [[ "${line}" =~ "_defconfig" ]]; then
			cfg=${line##*/}
			#echo $cfg
			tmp=$(make $cfg >/dev/null 2>&1)
			tmp1=$(make $cfg savedefconfig >/dev/null 2>&1)
			diff=$(diff build/defconfig ./board/amlogic/defconfigs/$cfg)
			if [ -n "$diff" ]; then
				echo "WARNING: the $cfg not generated by savedefconfig !!!\n"
				err_cnt=1
			fi
		fi
	done
	if [ $err_cnt != 0 ]; then
		exit 1
	fi

}

#check_defconfig