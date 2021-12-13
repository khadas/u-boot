#!/bin/bash -e

# Copyright (c) 2018 Amlogic, Inc. All rights reserved.
#
# This source code is subject to the terms and conditions defined in the
# file 'LICENSE' which is part of this source code package.

# For g12a and later

#set -x

TOOL_PATH=$(dirname $(readlink -f $0))

# Temporary files directory
if [ -z "$TMP" ]; then
    TMP=${SCRIPT_PATH}/tmp
fi

trace() {
    echo ">>> $@" > /dev/null
}

usage() {
    cat << EOF
    Usage:
        $(basename $0) --help

EOF
    exit 1
}

check_file() {
    if [ ! -f "$2" ]; then echo Error: Unable to open $1: \""$2"\"; exit 1 ; fi
}

# Check file argument exists and is given size
# $1 arg name
# $2 size
# $3 file
check_file_and_size() {
    check_file "$1" "$3"
    local filesize=$(wc -c < "$3")
    if [ $filesize -ne $2 ]; then
        echo "Incorrect size $filesize != $2 for $1 $3"
        exit 1
    fi
}

# Pad file to len by adding 0's to end of file
# $1: file
# $2: len
pad_file() {
    local file=$1
    local len=$2
    if [ ! -f "$1" ] || [ -z "$2" ]; then
        echo "Argument error, \"$1\", \"$2\" "
        exit 1
    fi
    local filesize=$(wc -c < ${file})
    local padlen=$(( $len - $filesize ))
    if [ $len -lt $filesize ]; then
        echo "File larger than expected.  $filesize, $len"
        exit 1
    fi
    dd if=/dev/zero of=$file oflag=append conv=notrunc bs=1 \
        count=$padlen >& /dev/null
}

# Write file to offset in output file
# 1: output file
# 2: file
# 3: offset
# 4: size
write_file() {
    if [[ $# -ne 4 ]]; then
        echo Error: bad args to write_file
        exit 1
    fi
    trace "write_file \"Xoutput\" $2 $3 $4"
    dd if="$2" of="$1" bs=1 seek=$3 count=$4 \
        conv=notrunc >& /dev/null
}

# Write hex to offset in output file
# 1: output file
# 2: value
# 3: offset
write_hex() {
    if [[ $# -ne 3 ]]; then
        echo Error: bad args to write_hex
        exit 1
    fi
    echo "$2" | xxd -r -p |
        dd of="$1" bs=1 seek=$3 \
            conv=notrunc >& /dev/null
}

# Write value to offset in output file
# 1: output file
# 2: value
# 3: offset
write_uint32_le() {
    local output=$1
    local input=$2
    local offset=$3
    local v=
    local vrev=
    v=$(printf %08x $input)
    # 00010001
    vrev=${v:6:2}${v:4:2}${v:2:2}${v:0:2}

    trace "write_uint32_le \"Xoutput\" $input $offset"
    echo $vrev | xxd -r -p |
        dd of="$1" bs=1 seek=$3 \
            conv=notrunc >& /dev/null
}

# Write value to offset in output file
# 1: output file
# 2: value
# 3: offset
write_uint8() {
    local output=$1
    local input=$2
    local offset=$3
    local v=
    trace "write_uint8 \"Xoutput\" $input $offset"
    v=$(printf %02x $input)
    echo $v | xxd -r -p |
        dd of="$output" bs=1 seek=$offset \
            conv=notrunc >& /dev/null
}

append_uint32_le() {
    local input=$1
    local output=$2
    local v=
    local vrev=
    v=$(printf %08x $input)
    # 00010001
    vrev=${v:6:2}${v:4:2}${v:2:2}${v:0:2}

    echo $vrev | xxd -r -p >> $output
}
## Create bl2 header
#${TOOL_PATH}/sign-boot-g12a --create-bl2-header \
#        -r $keylen \
#        --root-key "$TMP/rootkey.bin" \
#        --root-key-idx $rootkeyidx \
#        --root-key-sha-0 "$TMP/rootkey0.sha" \
#        --root-key-sha-1 "$TMP/rootkey1.sha" \
#        --root-key-sha-2 "$TMP/rootkey2.sha" \
#        --root-key-sha-3 "$TMP/rootkey3.sha" \
#        --bl2-key-sha-0 "$TMP/bl2key0.sha" \
#        --bl2-key-sha-1 "$TMP/bl2key1.sha" \
#        --bl2-key-sha-2 "$TMP/bl2key2.sha" \
#        --bl2-key-sha-3 "$TMP/bl2key3.sha" \
#        --bl2-key-idx 0 \
#        --bl2-key "$TMP/bl2key.bin" \
#        --marketid $marketid \
#        --bl2-size $bl2size \
#        -o "$TMP/bl2.hdr"
create_bl2_header() {
    local sigver="1"
    local keylen=""
    local rootkey=""
    local rootkeyidx="0"
    local rootkeysha0=""
    local rootkeysha1=""
    local rootkeysha2=""
    local rootkeysha3=""
    local bl2keysha0=""
    local bl2keysha1=""
    local bl2keysha2=""
    local bl2keysha3=""
    local bl2keyidx="0"
    local bl2key=""
    local bl2size=""
    local marketid=""
    local output=""
    local argv=("$@")
    local i=0

    # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -r) keylen="${argv[$i]}" ;;
            --root-key) rootkey="${argv[$i]}" ;;
            --root-key-idx) rootkeyidx="${argv[$i]}" ;;
            --root-key-sha-0) rootkeysha0="${argv[$i]}" ;;
            --root-key-sha-1) rootkeysha1="${argv[$i]}" ;;
            --root-key-sha-2) rootkeysha2="${argv[$i]}" ;;
            --root-key-sha-3) rootkeysha3="${argv[$i]}" ;;
            --bl2-key-sha-0) bl2keysha0="${argv[$i]}" ;;
            --bl2-key-sha-1) bl2keysha1="${argv[$i]}" ;;
            --bl2-key-sha-2) bl2keysha2="${argv[$i]}" ;;
            --bl2-key-sha-3) bl2keysha3="${argv[$i]}" ;;
            --bl2-key-idx) bl2keyidx="${argv[$i]}" ;;
            --bl2-key) bl2key="${argv[$i]}" ;;
            --marketid) marketid="${argv[$i]}" ;;
            --bl2-size) bl2size="${argv[$i]}" ;;
            -o) output="${argv[$i]}" ;;
            --sig-ver) sigver="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
    # Verify args
    if [[ $keylen -ne 1024 ]] && [[ $keylen -ne 2048 ]] && [[ $keylen -ne 4096 ]]; then
        echo Error: bad key len $keylen
        exit 1
    fi
    if [[ $keylen -ne 2048 ]]; then
        echo Error: currently only 2048 bit keylen supported
        # TODO fix this script to support other keylen
        exit 1
    fi
    check_file root-key "$rootkey"
    check_file_and_size root-key-sha-0 32 "$rootkeysha0"
    check_file_and_size root-key-sha-1 32 "$rootkeysha1"
    check_file_and_size root-key-sha-2 32 "$rootkeysha2"
    check_file_and_size root-key-sha-3 32 "$rootkeysha3"
    check_file_and_size bl2-key-sha-0 32 "$bl2keysha0"
    check_file_and_size bl2-key-sha-1 32 "$bl2keysha1"
    check_file_and_size bl2-key-sha-2 32 "$bl2keysha2"
    check_file_and_size bl2-key-sha-3 32 "$bl2keysha3"
    check_file bl2-key "$bl2key"

    if [[ -z "${rootkeyidx##*[!0-9]*}" ]] ||
            [[ $rootkeyidx -gt 3 ]]; then
        echo Error: bad rootkeyidx
        exit 1
    fi
    if [[ -z "${bl2keyidx##*[!0-9]*}" ]] ||
            [[ $bl2keyidx -gt 3 ]]; then
        echo Error: bad bl2keyidx
        exit 1
    fi
    if [ ! -z "$marketid" ] && [[ "$marketid" != "0" ]]; then
        echo Error: Market ID not yet supported;
        exit 1;
    fi
    if [[ -z $output ]] || [[ -f $output ]]; then
        echo Error: Invalid output file or output already exists
        exit 1
    fi


    # Get key version and check sizes
    local filesize=$(wc -c < "$rootkey")
    readonly rsa_public_key_size_v2=1036
    readonly rsa_public_key_size_v3=1052
    local key_ver
    if [[ $filesize -eq  $rsa_public_key_size_v2 ]]; then
        key_ver=2
        check_file_and_size bl2-key $rsa_public_key_size_v2 "$bl2key"
    elif [[ $filesize -eq  $rsa_public_key_size_v3 ]]; then
        key_ver=3
        check_file_and_size bl2-key $rsa_public_key_size_v3 "$bl2key"
        echo Error: not supported yet - v3
        exit 1
    else
        echo Error: bad key size for "$rootkey"
        exit 1
    fi

    # Create header
    cp "${TOOL_PATH}/bl2.hdr.template" "$output"
    write_file "$output" "$rootkey" 680 $rsa_public_key_size_v2
    write_uint32_le "$output" 1124 596  # v2 key nTotalSize
    write_file "$output" "$rootkeysha0" 1880 32
    write_file "$output" "$rootkeysha1" 1928 32
    write_file "$output" "$rootkeysha2" 1976 32
    write_file "$output" "$rootkeysha3" 2024 32
    write_file "$output" "$bl2keysha0" 2088 32
    write_file "$output" "$bl2keysha1" 2136 32
    write_file "$output" "$bl2keysha2" 2184 32
    write_file "$output" "$bl2keysha3" 2232 32
    write_file "$output" "$bl2key" 2928 $rsa_public_key_size_v2
    write_uint32_le "$output" 1124 2844  # v2 key nTotalSize

    write_uint8 "$output" $rootkeyidx 25
    write_uint32_le "$output" 320 44    # chkstart
    write_uint32_le "$output" 1928 60    # chksize
    write_uint32_le "$output" 0 76    # marketid
    write_uint8 "$output" 2 601    # keytype
    write_uint8 "$output" 6 1873    # root keytype
    write_uint8 "$output" 6 1921    # root keytype
    write_uint8 "$output" 6 1969    # root keytype
    write_uint8 "$output" 6 2017    # root keytype
    write_uint8 "$output" 6 2081    # bl2 keytype
    write_uint8 "$output" 6 2129    # bl2 keytype
    write_uint8 "$output" 6 2177    # bl2 keytype
    write_uint8 "$output" 6 2225    # bl2 keytype
    # hdr.bl2_aml_blk_hdr.nTotalSize = bl2size - 4 * 1024 + 1832;
    write_uint32_le "$output" $(( $bl2size - 4 * 1024 + 1832 )) 2268
    write_uint32_le "$output" 320 2292    # chkstart
    # hdr.bl2_aml_blk_hdr.nCHKSize = bl2_chksize;
    write_uint32_le "$output" $(( $bl2size - 4 * 1024 + 1832 - 64 - 256 )) 2308
    # hdr.bl2_aml_blk_hdr.nDataLen = bl2size - 4 * 1024;
    write_uint32_le "$output" $(( $bl2size - 4 * 1024 )) 2320
    write_uint8 "$output" 2 2849    # keytype
}

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
        case "$arg" in
            -h|--help)
                usage
                break ;;
            --create-bl2-header)
                create_bl2_header "${argv[@]:$((i + 1))}"
                break ;;
            *)
                echo "Unknown first option $1"; exit 1
                ;;
        esac
        i=$((i + 1))
    done
}

cleanup() {
    return
}

trap cleanup EXIT

if [ ! -d "$TMP" ]; then mkdir "$TMP" ; fi
cleanup
parse_main "$@"
