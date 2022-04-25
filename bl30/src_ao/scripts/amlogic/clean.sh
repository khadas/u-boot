#!/bin/bash

# Usage:
# compile: Run in the root directory:
# ./scripts/amlogic/clean.sh [board]

if [ "$1" == "" ]; then
echo "usage: $0 [board]"
scripts/amlogic/mk.sh --config
exit -1
fi

export clean=no
scripts/amlogic/mk.sh $1 clean
