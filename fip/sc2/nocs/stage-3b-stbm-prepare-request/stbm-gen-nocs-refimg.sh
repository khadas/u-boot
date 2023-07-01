#!/bin/bash

set -e

# Uncomment following line for debugging
# set -x

if [ -f "common.bashrc" ]; then
    source common.bashrc
fi

check_bootloader_builddir

#
# Variables and settings
#

# Input files
FIN_BB1ST_BIN="intermediate/bb1st.bin"
FIN_BLOB_BL2E="intermediate/blob-bl2e.bin"

# Output filename according to size of BL2E
FOUT_REFIMG_PREFIX="output/stbmRef"
FOUT_REFIMG_SUFFIX="Img.bin"

#
# Functions
#

function set_flag_fprot_en() {
  fname=$1

  val="0x$(cat ${fname} \
          | dd iflag=skip_bytes,count_bytes status=none \
               skip=$(( 0x201F )) count=1 \
          | xxd -ps)"
  val=$(( $val | 0x80 ))
  printf '%02x' $val | xxd -ps -r \
    | dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none \
         count=1 seek=$(( 0x201F )) \
         of=$1

}


function clr_flag_keypreserve() {
  fname=$1

  val="0x$(cat ${fname} \
          | dd iflag=skip_bytes,count_bytes status=none \
               skip=$(( 0x201F )) count=1 \
          | xxd -ps)"
  val=$(( $val & 0xBF ))
  printf '%02x' $val | xxd -ps -r \
    | dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none \
         count=1 seek=$(( 0x201F )) \
         of=$1

}

#
# Main
#

### Preparation ###
mkdir -p output/

### Size of BL2E ###
SZ_BLOB_BL2E=$(stat -c %s -L ${FIN_BLOB_BL2E})

case "${SZ_BLOB_BL2E}" in
  74864)
    FOUT_REFIMG_SZNAME="A"
    _sz_scs_total_area="701c0500"
    ;;
  140400)
    FOUT_REFIMG_SZNAME="B"
    _sz_scs_total_area="701c0600"
    ;;
  271472)
    FOUT_REFIMG_SZNAME="C"
    _sz_scs_total_area="701c0800"
    ;;
  1057904)
    FOUT_REFIMG_SZNAME="D"
    _sz_scs_total_area="701c1400"
    ;;
  *)
    echo "ERROR: unsupported size of 'blob-bl2e.bin': ${SZ_BLOB_BL2E}"
    exit -1
    ;;
esac

FOUT_REFIMG_NAME="${FOUT_REFIMG_PREFIX}${FOUT_REFIMG_SUFFIX}"

echo "BL2E: Size ${FOUT_REFIMG_SZNAME}"

# Generate 'refSize?Img.bin'
cat ${FIN_BB1ST_BIN} ${FIN_BLOB_BL2E} > ${FOUT_REFIMG_NAME}

# Set flags
#echo 01 | xxd -ps -r \
#  | dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none\
#      count=1 seek=$(( 0x2010 )) \
#      of=${FOUT_REFIMG_NAME} \

#if [ "x${CONFIG_FLASHPROT_ENABLE}" == "xy" ]; then
#  set_flag_fprot_en ${FOUT_REFIMG_NAME}
#fi

clr_flag_keypreserve ${FOUT_REFIMG_NAME}

#
# Currently FIP script generates 'bb1st.bin', 'blob-bl2e.bin' and 'blob-bl2x'
# through different round of run. In case that 'bb1st.bin' is taken from
# prebuilt image where as 'blob-bl2e.bin' is built from source code with
# different size of payload, 'Size of SCS total Area' in 'bb1st.bin' cannot be
# updated since it's used as is
#
# Solution: always update size of total area in ExtStartParams according to
# size of 'blob-bl2e'

echo ${_sz_scs_total_area} | xxd -ps -r \
  | dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none \
       count=4 seek=$(( 0x2040 )) \
       of=${FOUT_REFIMG_NAME}

# Fill nonces of Device Level-1/2 PubRSA with 0
dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none\
  count=16 seek=$(( 0x1910 )) \
  if=/dev/zero of=${FOUT_REFIMG_NAME} \

dd iflag=count_bytes oflag=seek_bytes conv=notrunc status=none\
  count=16 seek=$(( 0x2090 )) \
  if=/dev/zero of=${FOUT_REFIMG_NAME}

# Generate 'sha256partial-bl2.bin'
FOUT_BL2_PARTIAL="output/sha256partial-bl2.bin"

cat ${FIN_BB1ST_BIN} \
  | dd iflag=skip_bytes,count_bytes skip=$(( 0x3f5e0 )) count=128 status=none\
  > ${FOUT_BL2_PARTIAL}

# vim: set tabstop=2 expandtab shiftwidth=2:
