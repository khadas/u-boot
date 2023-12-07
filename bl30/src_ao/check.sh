#!/bin/bash
#
#  Copyright (c) 2019 Amlogic, Inc. All rights reserved.
#
source scripts/amlogic/util.sh

if [ "$1" == "clear-all" ];then
        rm -fr demos/amlogic/n200/*/*/gcc
        rm bl30.bin
        exit 0
fi

declare -i FAIL_COUNTER=0
SCRIPT_PATH=${SCRIPT_PATH:-$(dirname $(readlink -f $0))}
autobuild=${SCRIPT_PATH}/build-`date +%Y%m%d%H%M%S`
#mkdir -p $autobuild
echo $autobuild

declare -i TOTAL_CFG=1

board=$(list_boards)
ALL_BOARDS='\n'"Build BL30 for boards: "$board'\n'"--- $autobuild"'\n\n'

echo -e $ALL_BOARDS

RESULT='\n'"-------------------------------------------------------"'\n'

for item in ${board[@]}
{
        ./mk $item

        if [ $? != 0 ]; then
                FAIL_COUNTER=$FAIL_COUNTER+1
                RESULT=$RESULT"$TOTAL_CFG."'\t'${item}'\t\033[41;37m--- build failed\033[0m\n'
        else
                RESULT=$RESULT"$TOTAL_CFG.\t"${item}'\r\t\t\t\t--- pass\n'

                #cp -fr ./build/$item $autobuild/
                rm -fr demos/amlogic/n200/*/${item}/gcc
        fi
        TOTAL_CFG+=1;

        echo -e $RESULT
}

if [ $FAIL_COUNTER != 0 ]; then
        RESULT=$RESULT"\n\tCompile failed SoC number : "'\033[41;37m'$FAIL_COUNTER'\033[0m\n'
else
        RESULT=$RESULT"\n\tCompile all passed!\n\n"
fi

#echo -e $RESULT

exit $FAIL_COUNTER
