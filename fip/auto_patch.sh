#Copyright (C) 2018 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

function auto_patch()
{
    local patch_dir=$1

    for file in $patch_dir/*
    do
        if [ -f "$file" ] && [ "${file##*.}" == "patch" ]
        then
            local file_name=${file%.*};           #echo file_name $file_name
            local resFile=`basename $file_name`;  #echo resFile $resFile
            local dir_name1=${resFile//#/\/};     #echo dir_name $dir_name
            local dir_name=${dir_name1%/*};       #echo dir_name $dir_name
            local dir=$ROOT_DIR/$dir_name;               #echo $dir
            local change_id=`grep 'Change-Id' $file | head -n1 | awk '{print $2}'`
            echo "debug: $file_name $resFile $dir_name1 $dirname $dir $change_id"
            if [ -d "$dir" ]
            then
                cd $dir; git log -n 100 | grep $change_id 1>/dev/null 2>&1;
                if [ $? -ne 0 ]; then
                    echo "###patch ${file##*/}###      "
                    cd $dir; git am -q $file 1>/dev/null 2>&1;
                    if [ $? != 0 ]
                    then
                        git am --abort
                        echo " Error : Failed to patch [$file], Need check it.exit"
                        exit
                    fi
                fi
            fi
        fi
    done
}

function traverse_patch_dir()
{
    local local_dir=$1
    for file in `ls $local_dir`
    do
        if [ -d $local_dir$file ]
        then
            local dest_dir=$local_dir$file
            auto_patch $dest_dir
        fi
    done
    echo " Patch Finish."
    cd $UBOOT_DIR
}

CURRENT_DIR=$(pwd)/../../device/sei/adt4/pre_submit_for_adt4
echo "debug: $CURRENT_DIR"
LOCAL_PATH=$CURRENT_DIR/$(dirname $0)/
ROOT_DIR=$(pwd)/../..
UBOOT_DIR=$ROOT_DIR/bootloader/uboot-repo

if [ -d "$CURRENT_DIR" ]
then
    echo ""
	traverse_patch_dir $LOCAL_PATH
fi

if [ $? != 0 ]
then
    echo "patch error"
    return 1
fi
