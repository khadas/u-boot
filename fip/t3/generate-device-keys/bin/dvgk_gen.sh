#!/bin/bash -e

#set -x
version=1.0

trace ()
{
	echo ">>> $@" > /dev/null
	#echo ">>> $@"
}

mrk_gen() {
	mrk_file=$1

	echo "Generating MRK $mrk_file ..."
	dd if=/dev/random of=$mrk_file.bin iflag=fullblock bs=16 count=1
	xxd -ps $mrk_file.bin $mrk_file.txt
}

usage() {
    cat << EOF
Usage: $(basename $0) <mrk-file-name>
EOF
    exit 1
}

if [ $# -ne 1 ]; then
  usage
fi

echo "mrk-file : $1"
echo ""

mrk_gen $1
