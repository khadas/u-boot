#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
set -e
set -o pipefail

version=1.2

EXEC_BASEDIR=$(dirname $(readlink -f $0))
VENDOR_KEYTOOL=${EXEC_BASEDIR}/../../binary-tool/vendor-keytool

check_dir() {
    if [ ! -d "$1" ]; then echo "Error: directory \""$1"\" does NOT exist"; usage ; fi
}

trace ()
{
	echo ">>> $@" > /dev/null
	#echo ">>> $@"
}

gen_rootkey()
{
	outfile=$1
	mrkname=$2
	boot_stage=$3

	trace " mrk_file   $mrk_file"
	trace " outfile    $outfile_suffix"
	trace " mrk_name     $mrkname"
	trace " boot_stage $boot_stage"

	$VENDOR_KEYTOOL gen-root-aes256 --chipset=SC2 --mrk-file=${mrk_file} --mrk-name=${mrkname} --boot-stage=$3 | xxd -r -p > $outfile
}

usage() {
    cat << EOF
Usage: $(basename $0) --help | --version

       Generate Amlogic SC2 chipset Device Vendor Secure Chipset Startup (SCS) AES root key

       $(basename $0)
		--key-dir <root-key-dir> \\
		--mrk-bin <master-root-key-bin-file> \\
		{--mrk-name [DVGK | ACGK]} \\
		{--project <project-name>}
EOF
    exit 1
}

key_dir=""
part=""
mrk_file=""
stage="root"
mrk_name=""

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
            --mrk-bin)
                mrk_file="${argv[$i]}"
		;;
            --mrk-name)
                mrk_name="${argv[$i]}"
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
trace "  mrk-bin $mrk_file"
trace " mrk-name $mrk_name"

if [ -z "$key_dir" ]; then
	usage
fi

if [ -z "$stage" ]; then
	usage
fi

if [ -z "$mrk_name" ]; then
	mrk_name="DVGK"
fi

if [ ${stage,,} != "root" ] && [ ${stage,,} != "boot-blobs" ] && [ ${stage,,} != "fip" ]; then
	echo "Error: Invalid stage $stage"
	usage
fi

if [ ${mrk_name^^} != "ACGK" ] && [ ${mrk_name^^} != "DVGK" ]; then
	echo "Error: Invalid MRK name $mrk_name"
	usage
fi

if [ $stage == "root" ]; then
	if [ -z "$part" ]; then
		root_aes_path=${key_dir}/root/aes
	else
		root_aes_path=${key_dir}/root/aes/$part
	fi

	trace " root_aes_path $root_aes_path"
	mkdir -p $root_aes_path/rootkey

	echo "Generate 256-bit AES root key(s)"

	for i in 0 1 2 3
	do
		gen_rootkey $root_aes_path/rootkey/aes256-device-rootkey-bootstage-${i}.bin $mrk_name $i
	done
fi
