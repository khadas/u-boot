#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.0

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

trace ()
{
	echo ">>> $@" > /dev/null
	#echo ">>> $@"
}

check_dir() {
    if [ ! -d "$1" ]; then echo "Error: directory \""$1"\" does NOT exist"; usage ; fi
}

check_value() {
	local val=$1
	local begin=$2
	local end=$3

	if [ $val -lt $begin ] || [ $val -gt $end ]; then
		echo "Error: Value $val is not in range [$begin, $end]"
		exit 1
	fi
}

usage() {
    cat << EOF
Usage: $(basename $0) --help | --version

       Generate all Amlogic SC2 chipset Device Vendor Secure Chipset Startup (SCS) keys

       $(basename $0)
		--key-dir <key-dir> \\
		{--rsa-size [2048 | 4096]} \\
		{--project <project-name>} \\
		--template-dir  <template-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		--out-dir <output-dir>
EOF
    exit 1
}

key_dir=""
part=""
size=""
template_dir=""
rootkey_index=0
output_dir=""
external_dvgk=""
EXTERNAL_LVL1CERT_EPKS=""

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
		;;
            --project)
                part="${argv[$i]}"
		;;
            --rsa-size)
                size="${argv[$i]}"
		;;
	    --external-dvgk)
		external_dvgk="${argv[$i]}"
		;;
	    --external_lvl1cert_epks)
		EXTERNAL_LVL1CERT_EPKS="${argv[$i]}"
		export EXTERNAL_LVL1CERT_EPKS
		;;
            --template-dir)
                template_dir="${argv[$i]}"
		check_dir "${template_dir}"
		;;
		    --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value "$rootkey_index" 0 3
		;;
		    --out-dir)
                output_dir="${argv[$i]}"
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
trace "  rsa-size $size"
trace "  template_dir $template_dir"
trace "  rootkey-index $rootkey_index"
trace "  out-dir $output_dir"

if [ -z "$key_dir" ]; then
	usage
fi

if [ -z "$size" ]; then
	size=4096
fi

if [ -z "$template_dir" ]; then
	usage
fi

if [ -z "$rootkey_index" ]; then
	usage
fi

if [ -z "$output_dir" ]; then
	usage
fi

if [ "$size" -ne 2048 ] && [ "$size" -ne 4096 ]; then
	echo "Error: Invalid RSA key size $size"
	usage
fi

mkdir -p "$key_dir"
${EXEC_BASEDIR}/bin/gen_device_root_cert.sh --key-dir "$key_dir" --stage root --rsa-size "$size" --project "$part"
${EXEC_BASEDIR}/bin/gen_device_root_cert.sh --key-dir "$key_dir" --stage boot-blobs --rsa-size "$size" --project "$part"
${EXEC_BASEDIR}/bin/gen_device_root_cert.sh --key-dir "$key_dir" --stage fip --rsa-size "$size" --project "$part"

mkdir -p "$key_dir"/root/dvgk/"$part"

if [ ! -z ${external_dvgk} ]; then
	echo ==== use external dvgk ${external_dvgk} ====
	${EXEC_BASEDIR}/bin/derive_device_aes_rootkey.sh --key-dir "$key_dir" --mrk-bin ${external_dvgk} --mrk-name DVGK --project "$part"
else
	${EXEC_BASEDIR}/bin/dvgk_gen.sh "$key_dir"/root/dvgk/"$part"/dvgk
	${EXEC_BASEDIR}/bin/derive_device_aes_rootkey.sh --key-dir "$key_dir" --mrk-bin "$key_dir"/root/dvgk/"$part"/dvgk.bin --mrk-name DVGK --project "$part"
fi

${EXEC_BASEDIR}/bin/gen_device_aes_protkey.sh --rootkey-index "$rootkey_index" --key-dir "$key_dir" --project "$part" --template-dir "${template_dir}"

${EXEC_BASEDIR}/bin/gen_device_root_hash.sh --rootkey-index "$rootkey_index" --key-dir "$key_dir" --project "$part" --template-dir "${template_dir}"

${EXEC_BASEDIR}/bin/export_dv_scs_signing_keys.sh --key-dir "$key_dir" --out-dir "$output_dir" --rootkey-index "$rootkey_index" --project "$part"

