#!/bin/bash -e

set -e  #exit if get error
#set -x

ToolPath="$(cd $(dirname $0); pwd)/.."
kernelFile=$1
keyPath=$2
outPut=$3

usage() {
    echo "$(basename $0) <kernel file> <key dir path> <output file path>"
    exit 1
}
if [ "$#" -ne 3 ];  then
    echo "arg num $# err"
    usage
fi
if [ ! -f "$kernelFile" ]; then
    echo "$kernelFile is not existed kernel file"
    usage
fi
if [ ! -d "$keyPath" ]; then
    echo "not dir path for key, choose path containing key"
    usage
fi


${ToolPath}/signing-tool-tl1/sign-boot-tl1.sh --sign-kernel \
        -i ${kernelFile} \
        -k ${keyPath}/kernelkey.pem \
        -a ${keyPath}/kernelaeskey  \
        --iv ${keyPath}/kernelaesiv \
        -h 2                        \
        -o $outPut

