#!/bin/bash -e

set -e  #exit if get error
#set -x

ToolPath="$(cd $(dirname $0); pwd)/.."
kerenlFile=$1
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
if [ ! -f "$kerenlFile" ]; then
    echo "$kerenlFile is not existed kernel filw"
    usage
fi
if [ ! -d "$keyPath" ]; then
    echo "not dir path for key, choose path containning key"
    usage
fi


${ToolPath}/signing-tool-gxl/sign-boot-gxl.sh --sign-kernel \
        -i ${kerenlFile} \
        -k ${keyPath}/kernelkey.pem \
        -a ${keyPath}/kernelaeskey \
        --iv ${keyPath}/kernelaesiv \
        -o $outPut

