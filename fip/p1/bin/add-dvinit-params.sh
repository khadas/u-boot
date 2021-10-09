#!/bin/bash

set -e
# set -x

#
# Variables
#

EXEC_BASEDIR=$(dirname $(readlink -f $0))
ACPU_IMAGETOOL=${EXEC_BASEDIR}/../binary-tool/acpu-imagetool

BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

#
# Settings
#

BASEDIR_TEMPLATE=$1
BASEDIR_PAYLOAD=$2
BASEDIR_OUTPUT_BLOB=$3
SOC_FAMILY=$4

#
# Arguments
#

BB1ST_ARGS="${BB1ST_ARGS}"

### Input: template ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-template-bb1st=${BASEDIR_TEMPLATE}"

### Input: payloads ###
BB1ST_ARGS="${BB1ST_ARGS} --infile-dvinit-params=${BASEDIR_PAYLOAD}"

BB1ST_ARGS="${BB1ST_ARGS} --scs-family=${SOC_FAMILY}"

### Output: blobs ###
BB1ST_ARGS="${BB1ST_ARGS} --outfile-bb1st=${BASEDIR_OUTPUT_BLOB}"

#
# Main
#

set -x

${ACPU_IMAGETOOL} \
        create-boot-blobs \
        ${BB1ST_ARGS}

# vim: set tabstop=2 expandtab shiftwidth=2:
