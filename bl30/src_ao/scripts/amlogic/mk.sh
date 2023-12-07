#!/usr/bin/env bash
source scripts/amlogic/util.sh
#export PATH=/opt/gcc-linaro-aarch64-none-elf-4.9-2014.09_linux/bin/:$PATH
#export RTOS_CROSS_PREFIX=aarch64-none-elf-
#export PATH=/opt/gcc-linaro-7.3.1-2018.05-i686_aarch64-elf/bin:$PATH

# auto stop when any command return error
set -o errexit

if [ "$1" == "" ]; then
echo "usage: $0 [board]"
echo "valid boards are:"
list_boards
exit -1
elif [ "$1" == "--config" ]; then
echo "valid board are:"
list_boards
exit -1
fi

board=$1
shift
other_args=$*
if [ "${other_args}" == "" ]; then
	other_args=all
fi
arch_dir=`get_arch "${board}"`
soc_dir=`get_soc "${board}"`

echo "arch_dir: ${arch_dir}, soc_dir: ${soc_dir}"

if [ "${arch_dir}" == "" ]; then
echo "invalid board ${board}"
echo "valid boards are:"
list_boards
exit -1
fi

if [ "$clean" == "" ]; then
clean=yes #yes no
fi
cpu=1

function build_subpath() {
    sub="$1/${arch_dir}/${soc_dir}/${board}"
    pushd "$sub"

    [[ "$clean" == "yes" ]] && make clean
    make -j8  ${other_args}
#[[ "$clean" == "yes" ]] && make ARCH_DIR="${arch_dir}" BOARD_DIR="${board}" clean
#   make ARCH_DIR="${arch_dir}" BOARD_DIR="${board}" -j"$cpu" ${other_args}

    popd
}

function build_modules() {
    build_subpath lib/third_party/amlogic/
    build_subpath lib/third_party/amlogic/audio/util
    build_subpath lib/third_party/amlogic/audio/samples
    build_subpath lib/third_party/cadence/xaf-hostless
}

function build_app() {
    build_subpath demos/amlogic/
}

function add_pre_commit_hook() {
    if [ ! -d .git/hooks ]
    then
	return
    fi
    if [ -f .git/hooks/pre-commit ]
    then
	flag=`awk '{if($0 ~ "AMLOGIC_PRE_COMMIT_HOOK"){print $0}}' .git/hooks/pre-commit`
        if [ -z "$flag" ]
        then
            cp .git/hooks/pre-commit .git/hooks/pre-commit-old
	    cp scripts/amlogic/pre-commit-hook.sh .git/hooks/pre-commit
        fi
    else
        cp scripts/amlogic/pre-commit-hook.sh .git/hooks/pre-commit
    fi
}

#add_pre_commit_hook
#build_modules
build_app
