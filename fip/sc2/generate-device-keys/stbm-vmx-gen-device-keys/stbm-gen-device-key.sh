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
		--stbm-key-dir stbm-key-dir> \\
		{--rsa-size [2048 | 4096]} \\
		--project <project-name> \\
		--template-dir  <template-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		--arb-config <arb-config-file> \\
		--out-vmx-dir <output-vmx-dir>
EOF
    exit 1
}

stbm_key_dir=""
part=""
size=""
template_dir=""
rootkey_index=0
output_dir=""
arb_config=""

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
            --stbm-key-dir)
                stbm_key_dir="${argv[$i]}"
		;;
            --project)
                part="${argv[$i]}"
		;;
            --rsa-size)
                size="${argv[$i]}"
		;;
            --template-dir)
                template_dir="${argv[$i]}"
		check_dir "${template_dir}"
		;;
		    --rootkey-index)
                rootkey_index="${argv[$i]}"
		check_value "$rootkey_index" 0 3
		;;
		    --arb-config)
                arb_config="${argv[$i]}"
		;;
		    --out-vmx-dir)
                out_vmx_dir="${argv[$i]}"
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

trace "  key-dir $stbm_key_dir"
trace "  project $part"
trace "  rsa-size $size"
trace "  template_dir $template_dir"
trace "  rootkey-index $rootkey_index"
trace "  arb-config $arb_config"
trace "  out-dir $out_vmx_dir"

if [ -z "$stbm_key_dir" ]; then
	usage
fi

if [ -z "$size" ]; then
	size=4096
	echo "default rsa size 4096"
fi

if [ -z "$template_dir" ]; then
	usage
fi

if [ -z "$rootkey_index" ]; then
	usage
fi

if [ ! -f "$arb_config" ]; then
	usage
fi

if [ -z "$out_vmx_dir" ]; then
	usage
fi

if [ "$size" -ne 2048 ] && [ "$size" -ne 4096 ]; then
	echo "Error: Invalid RSA key size $size"
	usage
fi


${BASEDIR_TOP}/gen_all_device_key.sh \
    --template-dir "$template_dir" \
    --project "$part" \
    --rsa-size "$size" \
    --out-dir "${stbm_key_dir}/outdir" \
    --key-dir "${stbm_key_dir}/keydir"

${BASEDIR_TOP}/export_signing_keys_and_sign_template.sh \
    --template-dir "$template_dir" \
    --project "$part" \
    --arb-config "$arb_config" \
    --out-dir "${stbm_key_dir}/outdir" \
    --key-dir "${stbm_key_dir}/keydir"

${EXEC_BASEDIR}/bin/stbm-prepare-sign-request.sh "${stbm_key_dir}" "${out_vmx_dir}"