#!/bin/bash -e

# Copyright (c) 2020 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

#set -x
version=1.0

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

copy_files() {
  src_dir=$1
  dst_dir=$2
  list=$3

  for f in $list; do
    d="$(dirname $f)"
    mkdir -p ${dst_dir}/${d}
    cp ${src_dir}/$f ${dst_dir}/${d}/.
  done
}

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

       STBM compose VMX device template

       $(basename $0)
		--stbm-key-dir <key-dir> \\
		--project <project-name> \\
		--vmx-cert-path  <VMX cert path> \\
		--rootkey-index [0 | 1 | 2 | 3] \\
		--arb-config <arb-config-file> \\
		--out-dir <output-dir>
EOF
    exit 1
}

key_dir=""
part=""
size=""
vmx_cert_path=""
rootkey_index=0
output_dir=""
arb_config=""
boot_blobs_arb_args=""
device_fip_arb_args=""

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
                key_dir="${argv[$i]}"
		check_dir "${key_dir}"
		;;
            --project)
                part="${argv[$i]}"
		;;
            --vmx-cert-path)
                vmx_cert_path="${argv[$i]}"
		check_dir "${vmx_cert_path}"
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
trace "  vmx_cert_path $vmx_cert_path"
trace "  rootkey-index $rootkey_index"
trace "  arb-config $arb_config"
trace "  out-dir $output_dir"

if [ -z "$key_dir" ]; then
	usage
fi

if [ -z "$part" ]; then
	usage
fi

if [ -z "$vmx_cert_path" ]; then
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

# Create output directories
OUTPUT_BASEDIR="${output_dir}"

OUTDIR_TEMPLATE_BB1ST="${OUTPUT_BASEDIR}/boot-blobs/template/${part}/rootrsa-${rootkey_index}"
mkdir -p "${OUTDIR_TEMPLATE_BB1ST}"

# Generate templates
${EXEC_BASEDIR}/bin/update-vmx-device-template-bb1st.sh --rootkey-index "$rootkey_index" --key-dir "$key_dir" --project "$part" --vmx-cert-path "${vmx_cert_path}" --output-dir "${OUTDIR_TEMPLATE_BB1ST}"  ${boot_blobs_arb_args}

mv "${OUTDIR_TEMPLATE_BB1ST}/bb1st.bin.device" \
   "${OUTDIR_TEMPLATE_BB1ST}/bb1st.bin" \

OUTDIR_TEMPLATE_DEVICE_FIP_HEADER="${OUTPUT_BASEDIR}/fip/template/${part}/rootrsa-${rootkey_index}"
mkdir -p "${OUTDIR_TEMPLATE_DEVICE_FIP_HEADER}"
cp ${vmx_cert_path}/${part}/device-fip-header*.bin ${OUTDIR_TEMPLATE_DEVICE_FIP_HEADER}/device-fip-header.bin

# Copy other files
#LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/template/${part}/rootrsa-${rootkey_index}/device-fip-header.bin"
LIST_OTHER_FILES="${LIST_OTHER_FILES} boot-blobs/rsa/${part}/rootrsa-${rootkey_index}/key/level-2-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} boot-blobs/rsa/${part}/rootrsa-${rootkey_index}/key/level-1-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} boot-blobs/rsa/${part}/rootrsa-${rootkey_index}/key/level-2-rsa-pub.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/rsa/${part}/rootrsa-${rootkey_index}/key/bl30-level-3-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/rsa/${part}/rootrsa-${rootkey_index}/key/bl40-level-3-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/rsa/${part}/rootrsa-${rootkey_index}/key/bl31-level-3-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/rsa/${part}/rootrsa-${rootkey_index}/key/bl32-level-3-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/rsa/${part}/rootrsa-${rootkey_index}/key/bl33-level-3-rsa-priv.pem"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/aes/${part}/protkey/genkey-prot-bl30.bin"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/aes/${part}/protkey/genkey-prot-krnl.bin"
LIST_OTHER_FILES="${LIST_OTHER_FILES} fip/aes/${part}/protkey/genkey-prot-bl33.bin"

copy_files ${key_dir} ${OUTPUT_BASEDIR} \
           "${LIST_OTHER_FILES}"

# vim: set filetype=sh tabstop=4 expandtab shiftwidth=4:
