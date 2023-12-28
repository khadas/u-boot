#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.1

trace ()
{
	echo ">>> $@" > /dev/null
	#echo ">>> $@"
}

check_dir() {
    if [ ! -d "$1" ]; then echo "Error: directory \""$1"\" does NOT exist"; usage ; fi
}

rsa_gen() {
	local chain_num=$1
	local path=$2
	local files=$3
	local size=$4

	echo "Generating $chain_num RSA key ..."

	for f in $files
	do
		local kpriv="$path/$f-priv.pem"
		local kpub="$path/$f-pub.pem"
		openssl genrsa -out $kpriv $size
		echo $kpriv
		echo $kpub
		openssl rsa -in $kpriv -outform PEM -pubout -out $kpub
		#openssl rsa -noout -text -inform PEM -in $kpub -pubin
	done
}

ek_gen() {
	local chain_num=$1
	local path=$2
	local files=$3

	echo "Generating $chain_num EKs ..."

	for f in $files
	do
		local file="$path/$f"
		echo $file
		dd if=/dev/random of=$file iflag=fullblock bs=64 count=1
		#xxd -p -c16 $file
	done
}

nonce_gen() {
	local chain_num=$1
	local path=$2
	local files=$3

	echo "Generating $chain_num NONCE ..."

	for f in $files
	do
		local file="$path/$f"
		echo $file
		dd if=/dev/random of=$file iflag=fullblock bs=16 count=1
		#xxd -p -c16 $file
	done
}

usage() {
    cat << EOF
Usage: $(basename $0) --help | --version

       Generate Amlogic SC2 chipset Device Vendor Secure Chipset Startup (SCS) chain root keys

       $(basename $0)
		--key-dir <key-dir> \\
		--stage [root | boot-blobs | fip] \\
		{--rsa-size [2048 | 4096]} \\
		{--project <project-name>}
EOF
    exit 1
}

key_dir=""
part=""
size=""
stage=""

parse_main() {
    local i=0
    local argv=()
    for arg in "$@" ; do
        argv[$i]="$arg"
        i=$((i + 1))
    done

    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -h|--help)
                usage
                break
		;;
            -v|--version)
                echo "Version $version";
		exit 0
		;;
            --key-dir)
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --project)
                part="${argv[$i]}"
		;;
            --rsa-size)
                size="${argv[$i]}"
		;;
            --stage)
                stage="${argv[$i]}"
		;;
            *)
                echo "Unknown option $arg";
		usage
                ;;
        esac
        i=$((i + 1))
    done
}

parse_main "$@"

trace "  key-dir $key_dir"
trace "  project $part"
trace " rsa-size $size"
trace "    stage $stage"

if [ -z "$key_dir" ]; then
	usage
fi

if [ -z "$size" ]; then
	size=4096
fi

if [ -z "$stage" ]; then
	usage
fi

if [ $size -ne 2048 ] && [ $size -ne 4096 ]; then
	echo "Error: Invalid RSA key size $size"
	usage
fi

if [ $stage != "root" ] && [ $stage != "boot-blobs" ] && [ $stage != "fip" ]; then
	echo "Error: Invalid stage $stage"
	usage
fi

if [ $stage == "root" ]; then
	if [ -z "$part" ]; then
		root_rsa_path=${key_dir}/root/rsa
	else
		root_rsa_path=${key_dir}/root/rsa/$part
	fi

	trace " root_rsa_path $root_rsa_path"
	mkdir -p $root_rsa_path/key
	mkdir -p $root_rsa_path/epk
	mkdir -p $root_rsa_path/nonce

	echo "Generate root certificate(s)"

	rsa_gen "Root" "$root_rsa_path/key" "rootrsa-0 rootrsa-1 rootrsa-2 rootrsa-3" $size
	ek_gen "Root" "$root_rsa_path/epk" "rootcert-epks.bin"
	nonce_gen "Root" "$root_rsa_path/nonce" "rootrsa-0-nonce.bin rootrsa-1-nonce.bin rootrsa-2-nonce.bin rootrsa-3-nonce.bin"
fi

if [ $stage == "boot-blobs" ]; then
	if [ ! -z "$part" ]; then
		boot_blobs_rsa_root=${key_dir}/boot-blobs/rsa/$part
	else
		boot_blobs_rsa_root=${key_dir}/boot-blobs/rsa
	fi

	trace " boot_blobs_rsa_root $boot_blobs_rsa_root"
	mkdir -p $boot_blobs_rsa_root

	for i in 0 1 2 3
	do
		boot_blobs_rsa_path=$boot_blobs_rsa_root/rootrsa-${i}
		trace " boot_blobs_rsa_path $boot_blobs_rsa_path"

		mkdir -p $boot_blobs_rsa_path/key
		mkdir -p $boot_blobs_rsa_path/epk
		mkdir -p $boot_blobs_rsa_path/nonce

		echo "Generate $stage chain #$i certificate"
		rsa_gen $i "$boot_blobs_rsa_path/key" "level-1-rsa level-2-rsa" $size
		ek_gen $i "$boot_blobs_rsa_path/epk" "lvl1cert-epks.bin lvl2cert-epks.bin"
		nonce_gen $i "$boot_blobs_rsa_path/nonce" "device-lvl1rsa-nonce.bin device-lvl2rsa-nonce.bin"
	done
fi

if [ $stage == "fip" ]; then
	if [ ! -z "$part" ]; then
		fip_rsa_root=${key_dir}/fip/rsa/$part
	else
		fip_rsa_root=${key_dir}/fip/rsa
	fi

	trace " fip_rsa_root $fip_rsa_root"
	mkdir -p $fip_rsa_root

	for i in 0 1 2 3
	do
		fip_rsa_path=$fip_rsa_root/rootrsa-${i}
		trace " fip_rsa_path $fip_rsa_path"

		mkdir -p $fip_rsa_path/key
		mkdir -p $fip_rsa_path/epk
		mkdir -p $fip_rsa_path/nonce

		echo "Generate ${stage^^} chain #$i certificate"
		rsa_gen $i "$fip_rsa_path/key" "bl30-level-3-rsa bl31-level-3-rsa bl32-level-3-rsa bl33-level-3-rsa bl40-level-3-rsa krnl-level-3-rsa" $size
		ek_gen $i "$fip_rsa_path/epk" "bl30-lvl3cert-epks.bin bl31-lvl3cert-epks.bin bl32-lvl3cert-epks.bin bl33-lvl3cert-epks.bin bl40-lvl3cert-epks.bin krnl-lvl3cert-epks.bin"
		nonce_gen $i "$fip_rsa_path/nonce" "bl30-dvlvl3cert-nonce.bin bl31-dvlvl3cert-nonce.bin bl32-dvlvl3cert-nonce.bin bl33-dvlvl3cert-nonce.bin bl40-dvlvl3cert-nonce.bin krnl-dvlvl3cert-nonce.bin"
	done
fi
