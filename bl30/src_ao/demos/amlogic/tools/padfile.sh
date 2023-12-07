#!/bin/bash

pad_file() {
    local file=$1
    local len=0

    echo "$file"
    if [ ! -f "$1" ]; then
        echo "Argument error, \"$1\" "
        exit 1
    fi
    local filesize=$(wc -c < ${file})
    echo "filesize= $filesize "
    if [ $filesize -lt 28672 ]; then
        len=28672
    elif [ $filesize -lt 61440 ]; then
        len=61440
    elif [ $filesize -lt 77824 ]; then
        len=77824
    fi
    local padlen=$(( $len - $filesize ))
    if [ $len -lt $filesize ]; then
        echo "File larger than expected.  $filesize, $len"
        exit 1
    fi
    dd if=/dev/zero of=$file oflag=append conv=notrunc bs=1 \
        count=$padlen >& /dev/null
}

pad_file $1
