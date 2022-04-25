#!/bin/bash -e
#set -x

keyPath=$1

usage() {
    echo "$(basename $0) <key dir path>"
    exit 1
}
if [ "$#" -ne 1 ];  then
    echo "arg num $# err"
    usage
fi
if [ ! -d ${keyPath} ]; then
    mkdir ${keyPath}
fi

rsaKeyList='root0 root1 root2 root3 bl2 bl3xkey kernelkey'
for aKey in $(echo $rsaKeyList); do
    aPem="${keyPath}/${aKey}.pem"
    echo "to gen ${aPem}"
    openssl genrsa -out ${aPem} 2048
done
cp ${keyPath}/root0.pem ${keyPath}/root.pem

aeskeyList='bl2 bl3x kernel'
for aKey in $(echo $aeskeyList); do
    aesKey="${keyPath}/${aKey}aes"
    aesIv="${aesKey}iv"
    aesKey="${aesKey}key"
    echo "to gen aes[${aesKey}, ${aesIv}]"
    dd if=/dev/urandom of=${aesKey} bs=32 count=1
    dd if=/dev/urandom of=${aesIv} bs=16 count=1
done


