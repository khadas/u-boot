
#
# Variables and settings
#

BASHRC_BASEDIR=$(readlink -f $(dirname ${BASH_SOURCE[0]}))
echo ${BASHRC_BASEDIR}

ACPU_IMAGETOOL="${BASHRC_BASEDIR}/tools/bin/acpu-imagetool"

DEVICE_KEYS_BASEDIR=$(realpath -m ${BASHRC_BASEDIR}/stage-3a-stbm-generate-keysets/output/data-stbm/outdir/)
AESKEY_ROOT_BASEDIR=$(realpath -m ${BASHRC_BASEDIR}/stage-3a-stbm-generate-keysets/output/data-stbm/keydir/root/aes/${PRJ}/rootkey/)

#
# Checking
#

check_bootloader_builddir() {
    if [ -z "${AMLOGIC_BOOTLOADER_BUILDDIR}" ]; then
        echo "ERROR: AMLOGIC_BOOTLOADER_BUILDDIR is not set"
        exit 1
    fi
}

#
# Functions
#

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

# vim: set syntax=sh filetype=sh tabstop=4 expandtab shiftwidth=4 softtabstop=-1:
