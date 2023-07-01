#!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)

function read_csv_and_generate() {
    local argv=("$@")
    local i=0

    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            -i)
                input="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            --debug)
                is_debug_enabled="true";;
        esac
        i=$((i + 1))
    done

    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    local patt=$(mktemp -p .)
    local license0=$(mktemp -p .)
    local license1=$(mktemp -p .)
    local dgpk1=$(mktemp -p .)
    local device_scs_segid=$(mktemp -p .)
    local device_vendor_segid=$(mktemp -p .)
    local nsk_otp_oid=$(mktemp -p .)
    local nsk_otp_vid=$(mktemp -p .)
    local lic_lock_1=$(mktemp -p .)
    local efusebit=$(mktemp -p .)
    local temp_byte=$(mktemp -p .)
    local binary_string_hash=$(mktemp -p .)

    echo "reading $input, Generating $output"
    dos2unix $input
    sed -i -e '$a\' $input

    #Generate empty efuse pattern bytes array
    dd if=/dev/zero of=$patt count=4096 bs=1 &> /dev/null

    # Construct zero-filled license, lock bits
    dd if=/dev/zero of=$license0 count=16 bs=1 &>/dev/null
    dd if=/dev/zero of=$license1 count=16 bs=1 &>/dev/null
    dd if=/dev/zero of=$lic_lock_1 count=1 bs=1 &> /dev/null
    dd if=/dev/zero of=$dgpk1 count=16 bs=1 &>/dev/null
    dd if=/dev/zero of=$device_scs_segid count=4 bs=1 &>/dev/null
    dd if=/dev/zero of=$device_vendor_segid count=4 bs=1 &>/dev/null
    dd if=/dev/zero of=$nsk_otp_oid count=2 bs=1 &>/dev/null
    dd if=/dev/zero of=$nsk_otp_vid count=2 bs=1 &>/dev/null
    dd if=/dev/zero of=$binary_string_hash count=37 bs=1 &>/dev/null

    # generate license bit array values with initial value 0
    b_lic0_01=$(xxd -ps -s1 -l1 $license0)
    b_lic0_02=$(xxd -ps -s2 -l1 $license0)
    b_lic0_04=$(xxd -ps -s3 -l1 $license0)
    b_lic0_05=$(xxd -ps -s4 -l1 $license0)
    b_lic0_07=$(xxd -ps -s6 -l1 $license0)

    b_lic1_00=$(xxd -ps -l1 $license1)
    b_lic1_01=$(xxd -ps -s1 -l1 $license1)
    b_lic1_13=$(xxd -ps -s13 -l1 $license1)

    b_lic_lock_1=$(xxd -ps -l1 $lic_lock_1)

    while IFS=',' read feat sz value
    do
        len_value=${#value}
        case $feat in
            FEAT_ENABLE_DEVICE_SCS_SIG_0)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_01="$(printf %02x $(( 0x$b_lic0_01 | 0x04 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=0 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DEVICE_SCS_SIG_1)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_01="$(printf %02x $(( 0x$b_lic0_01 | 0x08 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=1 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DEVICE_VENDOR_SIG_0)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_02="$(printf %02x $(( 0x$b_lic0_02 | 0x10 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=2 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DEVICE_VENDOR_SIG_1)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_02="$(printf %02x $(( 0x$b_lic0_02 | 0x20 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=3 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DEVICE_PROT_0)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_02="$(printf %02x $(( 0x$b_lic0_02 | 0x40 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=4 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DEVICE_PROT_1)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_02="$(printf %02x $(( 0x$b_lic0_02 | 0x80 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=5 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_SPI_NOR)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_04="$(printf %02x $(( 0x$b_lic0_04 | 0x01 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=6 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_NAND)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_04="$(printf %02x $(( 0x$b_lic0_04 | 0x02 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=7 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_EMMC)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_04="$(printf %02x $(( 0x$b_lic0_04 | 0x04 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=8 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_SDCARD)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_04="$(printf %02x $(( 0x$b_lic0_04 | 0x08 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=9 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_SPI_NAND)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_04="$(printf %02x $(( 0x$b_lic0_04 | 0x10 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=10 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_DFU_USB)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_05="$(printf %02x $(( 0x$b_lic0_05 | 0x04 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=11 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_ALTSRC)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_05="$(printf %02x $(( 0x$b_lic0_05 | 0x08 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=12 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_BOOT_NORMAL_FALLBACK2USB)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_05="$(printf %02x $(( 0x$b_lic0_05 | 0x10 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=13 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_SW_BOOT_OVERRIDE)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_05="$(printf %02x $(( 0x$b_lic0_05 | 0x40 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=14 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_EXT_BOOT_OVERRIDE)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_05="$(printf %02x $(( 0x$b_lic0_05 | 0x80 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=15 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_DDR_FIP_CONTAINER)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_07="$(printf %02x $(( 0x$b_lic0_07 | 0x40 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=16 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_USB_AUTH)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic0_07="$(printf %02x $(( 0x$b_lic0_07 | 0x80 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=17 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DIF_MASTER_PROT)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_00="$(printf %02x $(( 0x$b_lic1_00 | 0x01 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=18 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_ACPU_JTAG_PROT)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_00="$(printf %02x $(( 0x$b_lic1_00 | 0x10 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=19 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_AOCPU_JTAG_PROT)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_00="$(printf %02x $(( 0x$b_lic1_00 | 0x80 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=20 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_ENABLE_DSP_JTAG_PROT)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_01="$(printf %02x $(( 0x$b_lic1_01 | 0x08 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=21 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            FEAT_DISABLE_NSK_CONCURRENCY)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_13="$(printf %02x $(( 0x$b_lic1_13 | 0x20 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=22 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            NSK_PUB_OTP_OID)
                echo found ${feat} bit length ${sz} value ${value}
                if [ $len_value != $(($sz / 4)) ]; then
                    echo feature $feat : $value has invalid length $(($len_value * 4)).
                    echo expected length is $sz bits.
                    break
                fi
                if ! [[ $value =~ ^[0-9a-fA-F]{$len_value}$ ]]; then
                    echo $feat:$value invalid format, Only hexadecimal numbers are acceptable.
                    break
                fi
                echo $value | xxd -r -p > $nsk_otp_oid
                echo $value | xxd -r -p > $temp_byte
                dd if="$temp_byte" of=$binary_string_hash bs=1 seek=23 count=2 conv=notrunc >& /dev/null
                ;;
            NSK_PUB_OTP_VID)
                echo found ${feat} bit length ${sz} value ${value}
                if [ $len_value != $(($sz / 4)) ]; then
                    echo feature $feat : $value has invalid length $(($len_value * 4)).
                    echo expected length is $sz bits.
                    break
                fi
                if ! [[ $value =~ ^[0-9a-fA-F]{$len_value}$ ]]; then
                    echo $feat:$value invalid format, Only hexadecimal numbers are acceptable.
                    break
                fi
                echo $value | xxd -r -p > $nsk_otp_vid
                echo $value | xxd -r -p > $temp_byte
                dd if="$temp_byte" of=$binary_string_hash bs=1 seek=25 count=2 conv=notrunc >& /dev/null
                ;;
            DEVICE_SCS_SEGID)
                echo found ${feat} bit length ${sz} value ${value}
                if [ $len_value != $(($sz / 4)) ]; then
                    echo feature $feat : $value has invalid length $(($len_value * 4)).
                    echo expected length is $sz bits.
                    break
                fi
                if ! [[ $value =~ ^[0-9a-fA-F]{$len_value}$ ]]; then
                    echo $feat:$value invalid format, Only hexadecimal numbers are acceptable.
                    break
                fi
                echo $value | xxd -r -p > $device_scs_segid
                echo $value | xxd -r -p > $temp_byte
                dd if="$temp_byte" of=$binary_string_hash bs=1 seek=27 count=4 conv=notrunc >& /dev/null
                ;;
            DEVICE_VENDOR_SEGID)
                echo found ${feat} bit length ${sz} value ${value}
                if [ $len_value != $(($sz / 4)) ]; then
                    echo feature $feat : $value has invalid length $(($len_value * 4)).
                    echo expected length is $sz bits.
                    break
                fi
                if ! [[ $value =~ ^[0-9a-fA-F]{$len_value}$ ]]; then
                    echo $feat:$value invalid format, Only hexadecimal numbers are acceptable.
                    break
                fi
                echo $value | xxd -r -p > $device_vendor_segid
                echo $value | xxd -r -p > $temp_byte
                dd if="$temp_byte" of=$binary_string_hash bs=1 seek=31 count=4 conv=notrunc >& /dev/null
                ;;
            DGPK1)
                echo found ${feat} bit length ${sz} value ${value}
                if [ $len_value != $(($sz / 4)) ]; then
                    echo feature $feat : $value has invalid length $(($len_value * 4)).
                    echo expected length is $sz bits.
                    break
                fi
                if ! [[ $value =~ ^[0-9a-fA-F]{$len_value}$ ]]; then
                    echo $feat:$value invalid format, Only hexadecimal numbers are acceptable.
                    break
                fi
                echo $value | xxd -r -p > $dgpk1
                ;;
            LICENSE_LOCK_1_B228_LOCK)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic_lock_1="$(printf %02x $(( 0x$b_lic_lock_1 | 0x10 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=35 count=1 conv=notrunc >& /dev/null
                fi
                ;;
            NSK_OTP_WRITE_PASSWORD_PROTECT)
                echo found ${feat} bit length ${sz} value ${value}
                if ([ "$value" != "0" ] && [ "$value" != "1" ]); then
                    echo feature $feat : $value is not in expected range
                    break
                fi
                if [ "$value" == "1" ]; then
                    b_lic1_13="$(printf %02x $(( 0x$b_lic1_13 | 0x08 )))"
                    echo 01 | xxd -r -p > $temp_byte
                    dd if="$temp_byte" of=$binary_string_hash bs=1 seek=36 count=1 conv=notrunc >& /dev/null
                    fi
                ;;
            *)
                echo Feature ${feat} is not supported
                ;;
        esac
    done < <(tail -n +2 $input)

    # Combine license 0 block
    echo $b_lic0_01 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license0 bs=1 seek=1 count=1 conv=notrunc >& /dev/null
    echo $b_lic0_02 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license0 bs=1 seek=2 count=1 conv=notrunc >& /dev/null
    echo $b_lic0_04 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license0 bs=1 seek=4 count=1 conv=notrunc >& /dev/null
    echo $b_lic0_05 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license0 bs=1 seek=5 count=1 conv=notrunc >& /dev/null
    echo $b_lic0_07 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license0 bs=1 seek=7 count=1 conv=notrunc >& /dev/null

    # Combine license 1 block
    echo $b_lic1_00 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license1 bs=1 seek=0 count=1 conv=notrunc >& /dev/null
    echo $b_lic1_01 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license1 bs=1 seek=1 count=1 conv=notrunc >& /dev/null
    echo $b_lic1_13 | xxd -r -p > $efusebit
    dd if="$efusebit" of=$license1 bs=1 seek=13 count=1 conv=notrunc >& /dev/null

    # write otp lic lock 1
    echo $b_lic_lock_1 | xxd -r -p > $lic_lock_1

    # Combine generated blocks into a single binary
    dd if="$license0" of="$patt" bs=16 seek=0 count=1 conv=notrunc >& /dev/null
    dd if="$license1" of="$patt" bs=16 seek=1 count=1 conv=notrunc >& /dev/null
    dd if="$device_scs_segid" of="$patt" bs=4 seek=42 count=1 conv=notrunc >& /dev/null
    dd if="$device_vendor_segid" of="$patt" bs=4 seek=43 count=1 conv=notrunc >& /dev/null
    dd if="$dgpk1" of="$patt" bs=16 seek=228 count=2 conv=notrunc >& /dev/null
    dd if="$nsk_otp_oid" of="$patt" bs=1 seek=528 count=2 conv=notrunc >& /dev/null
    dd if="$nsk_otp_vid" of="$patt" bs=1 seek=530 count=2 conv=notrunc >& /dev/null
    dd if="$lic_lock_1" of="$patt" bs=1 seek=508 count=1 conv=notrunc >& /dev/null

    cp $binary_string_hash ./hash_of_output
    md5sum --tag hash_of_output
    sha256sum --tag hash_of_output
    sha1sum --tag hash_of_output

    if [ "$is_debug_enabled" == "true" ];
    then
        echo debug mode enabled
        cp $patt ./$output.bin
        cp $binary_string_hash ./hash_calculation.bin
        ${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output.bin.efuse --option=debug
    else
        ${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output.bin.efuse
    fi

    rm -f $efusebit
    rm -f $patt
    rm -f $license0
    rm -f $license1
    rm -f $device_scs_segid
    rm -f $device_vendor_segid
    rm -f $dgpk1
    rm -f $nsk_otp_oid
    rm -f $nsk_otp_vid
    rm -f $lic_lock_1
    rm -f $temp_byte
    rm -f $binary_string_hash
    rm -f ./hash_of_output
}

usage(){
        cat << EOF
Usage: $(basename $0) --help
       $(basename $0) --version
       $(basename $0) -i input_filename.csv \\
                      -o output_file
EOF
    exit 1
}

function generate_efuse_device_pattern() {
    local argv=("$@")

    echo "Generating $@..."
}

parse_main() {
    case "$@" in
        --help)
            usage
            ;;
        --version)
            echo "$(basename $0) version 0.1"
            ;;
        *-i*)
            read_csv_and_generate "$@"
            ;;
        *)
            usage "$@"
            ;;
    esac
}

parse_main "$@"
