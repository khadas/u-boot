#!/bin/bash

set -e
# set -x

#
# Settings
#
BASEDIR_TOP=$1
SOC=$2
INPUT=$3
OUTPUT=$4

TOOLBIN=${BASEDIR_TOP}/${SOC}/aml_encrypt_${SOC}

COMPRESS_ORG=${INPUT}.org
COMPRESS_LZ4=${INPUT}.org.lz4


#
# Arguments
#

cp $INPUT $COMPRESS_ORG

${TOOLBIN} --bl3sig --input ${COMPRESS_ORG} --out ${COMPRESS_LZ4} --compress lz4 --level v3 --type bl32

ret=$?
if [ 0 != $ret ]; then
    echo "Err! aml_encrypt_${SOC} return $ret"
    exit $ret
fi

dd if=${COMPRESS_LZ4} of=${OUTPUT} bs=1 skip=1824 >& /dev/null


# vim: set tabstop=2 expandtab shiftwidth=2:
