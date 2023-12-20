#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.2

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

       Export signing keys and sign template

       $(basename $0)
		--key-dir <key-dir> \\
		{--project <project-name>} \\
		--template-dir  <template-dir> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		--arb-config <arb-config-file> \\
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
boot_blobs_arb_args=
device_fip_arb_args=

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
trace "  template_dir $template_dir"
trace "  rootkey-index $rootkey_index"
trace "  out-dir $output_dir"

if [ -z "$key_dir" ]; then
	usage
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

if [ -s "${arb_config}" ]; then
    source ${arb_config}
    boot_blobs_arb_args="--device-scs-segid ${DEVICE_SCS_SEGID} --device-vendor-segid ${DEVICE_VENDOR_SEGID} --device-scs-vers ${DEVICE_SCS_VERS} --device-tee-vers ${DEVICE_TEE_VERS}"
    device_fip_arb_args="--device-vendor-segid ${DEVICE_VENDOR_SEGID} --device-tee-vers ${DEVICE_TEE_VERS} --device-ree-vers ${DEVICE_REE_VERS}"
fi

${EXEC_BASEDIR}/bin/gen_device_aes_protkey.sh --rootkey-index "$rootkey_index" --key-dir "$key_dir" --project "$part" --template-dir "${template_dir}" ${device_fip_arb_args}

${EXEC_BASEDIR}/bin/gen_device_root_hash.sh --rootkey-index "$rootkey_index" --key-dir "$key_dir" --project "$part" --template-dir "${template_dir}" ${boot_blobs_arb_args}

${EXEC_BASEDIR}/bin/export_dv_scs_signing_keys.sh --key-dir "$key_dir" --out-dir "$output_dir" --rootkey-index "$rootkey_index" --project "$part"

