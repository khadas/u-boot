 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))
BASEDIR_TOP=$(readlink -f ${EXEC_BASEDIR}/..)
VENDOR_KEYTOOL=${EXEC_BASEDIR}/../binary-tool/vendor-keytool
#
# Settings
#
VERSION=0.4

# Check file
check_file() {
    if [ ! -f "$2" ]; then echo Error: Unable to open $1: \""$2"\"; exit 1 ; fi
}

# Check file is size or exit. $1: file, $2: size
check_size() {
    local filesize=$(wc -c < "$1")
    if [ $filesize -ne $2 ]; then
        echo "Error: File \"$1\" incorrect size. Was $filesize, expected $2"
        exit 1
    fi
}

# Check optional file argument exists and is given size
# $1 arg name
# $2 size
# $3 file
check_opt_file() {
    if [ -n "$3" ]; then
        check_file "$1" "$3"
        local filesize=$(wc -c < "$3")
        if [ $filesize -ne $2 ]; then
            echo "Incorrect size $filesize != $2 for $1 $3"
            exit 1
        fi
    fi
}

check_opt_boolean() {
    if [ -n "$2" ]; then
        if [ "$2" != "true" ] && [ "$2" != "false" ]; then
            echo Error: invalid value $1: \""$2"\"
            exit 1
        fi
    fi
}

usage() {
    cat << EOF
Usage: $(basename $0) --help
       $(basename $0) --version
       $(basename $0) [--input base.efuse.bin] \\
                      [--dvgk dvgk.bin] \\
                      [--enable-usb-password true] \\
                      [--enable-dif-password true] \\
                      -o pattern.efuse
       $(basename $0) --device-scs-segid seg_id_value \\
                      -o device-scs-id.efuse
       $(basename $0) --device-vendor-segid vend_id_value \\
                      -o device-vedor-id.efuse
EOF
    exit 1
}

function generate_efuse_device_pattern() {
    local argv=("$@")
    local i=0

    local patt=$(mktemp --tmpdir)
    local wrlock0=$(mktemp --tmpdir)
    local wrlock1=$(mktemp --tmpdir)
    local hmac=$(mktemp --tmpdir)
    local license0=$(mktemp --tmpdir)
    local license1=$(mktemp --tmpdir)
    local efusebit=$(mktemp --tmpdir)

    # Parse args
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --input)
                input="${argv[$i]}" ;;
            -o)
                output="${argv[$i]}" ;;
            --dvgk)
                dvgk="${argv[$i]}" ;;
#            --dvuk)
#                dvuk="${argv[$i]}" ;;
#			--device-roothash)
#                device_roothash="${argv[$i]}" ;;
            --enable-usb-password)
                enable_usb_password="${argv[$i]}" ;;
            --enable-dif-password)
                enable_dif_password="${argv[$i]}" ;;
#            --enable-dvuk-derive-with-cid)
#                enable_dvuk_derive_with_cid="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    check_opt_file input 4096 "$input"
    check_opt_file dvgk 16 "$dvgk"
    check_opt_file dvuk 16 "$dvuk"
    check_opt_file device_roothash 32 "$device_roothash"

    check_opt_boolean enable-usb-password "$enable_usb_password"
    check_opt_boolean enable-dif-password "$enable_dif_password"
    check_opt_boolean enable-dvuk-derive-with-cid "$enable_dvuk_derive_with_cid"

    # Generate empty eFUSE pattern data
    if [ -n "$input" ]; then
        dd if="$input" of=$patt count=4096 bs=1 &> /dev/null
    else
        dd if=/dev/zero of=$patt count=4096 bs=1 &> /dev/null
    fi

    # Construct wrlock bits
    dd if=$patt of=$wrlock0 bs=16 skip=30 count=1 &> /dev/null
    dd if=$patt of=$wrlock1 bs=16 skip=31 count=1 &> /dev/null
    b_1e2=$(xxd -ps -s2 -l1 $wrlock0)
    b_1e3=$(xxd -ps -s3 -l1 $wrlock0)
    b_1fc=$(xxd -ps -s12 -l1 $wrlock1)

    if [ "$dvgk" != "" ]; then
		${VENDOR_KEYTOOL} gen-mrk-chknum --chipset=SC2 --mrk-file="$dvgk" --mrk-name=DVGK | grep 'Long checknum: ' | \
            grep "Long checknum:" | sed 's/Long checknum: //' | sed 's/ (.*//' | xxd -r -p > $hmac

        dd if="$dvgk" of="$patt" bs=16 seek=226 count=1 \
            conv=notrunc >& /dev/null
        dd if="$hmac" of="$patt" bs=16 seek=194 count=1 \
            conv=notrunc >& /dev/null
        b_1fc="$(printf %02x $(( 0x$b_1fc | 0x04 )))"
    fi

    if [ "$dvuk" != "" ]; then
        ${VENDOR_KEYTOOL} gen-mrk-chknum --chipset=SC2 --mrk-file="$dvuk" --mrk-name=DVUK | grep 'Long checknum: ' | \
            grep "Long checknum:" | sed 's/Long checknum: //' | sed 's/ (.*//' | xxd -r -p > $hmac

        dd if="$dvuk" of="$patt" bs=16 seek=227 count=1 \
            conv=notrunc >& /dev/null
        dd if="$hmac" of="$patt" bs=16 seek=195 count=1 \
            conv=notrunc >& /dev/null
        b_1fc="$(printf %02x $(( 0x$b_1fc | 0x08 )))"
    fi

    if [ "$device_roothash" != "" ]; then
        dd if="$device_roothash" of="$patt" bs=16 seek=23 count=2 \
            conv=notrunc >& /dev/null
	    dd if="$device_roothash" of="$patt" bs=16 seek=25 count=2 \
            conv=notrunc >& /dev/null
		b_1e2="$(printf %02x $(( 0x$b_1e2 | 0x80 )))"
		b_1e3="$(printf %02x $(( 0x$b_1e3 | 0x07 )))"
    fi

    echo $b_1e2 | xxd -r -p > $efusebit
    dd if=$efusebit of=$wrlock0 bs=1 seek=2 count=1 conv=notrunc >& /dev/null
    echo $b_1e3 | xxd -r -p > $efusebit
    dd if=$efusebit of=$wrlock0 bs=1 seek=3 count=1 conv=notrunc >& /dev/null
    echo $b_1fc | xxd -r -p > $efusebit
    dd if=$efusebit of=$wrlock1 bs=1 seek=12 count=1 conv=notrunc >& /dev/null

    filesize=$(wc -c < $wrlock0)
    if [ $filesize -ne 16 ]; then
        echo Internal Error -- Invalid write-lock0 pattern length
        exit 1
    fi
    dd if=$wrlock0 of=$patt bs=16 seek=30 count=1 conv=notrunc >& /dev/null

    filesize=$(wc -c < $wrlock1)
    if [ $filesize -ne 16 ]; then
        echo Internal Error -- Invalid write-lock1 pattern length
        exit 1
    fi
    dd if=$wrlock1 of=$patt bs=16 seek=31 count=1 conv=notrunc >& /dev/null


    dd if=$patt of=$license0 bs=16 skip=0 count=1 &> /dev/null
    b_007=$(xxd -ps -s7 -l1 $license0)
    if [ "$enable_usb_password" == "true" ]; then
        b_007="$(printf %02x $(( 0x$b_007 | 0x80 )))"
    fi
    echo $b_007 | xxd -r -p > $efusebit
    dd if=$efusebit of=$license0 bs=1 seek=7 count=1 conv=notrunc >& /dev/null

    filesize=$(wc -c < $license0)
    if [ $filesize -ne 16 ]; then
        echo Internal Error -- Invalid write-lock0 pattern length
        exit 1
    fi
    dd if=$license0 of=$patt bs=16 seek=0 count=1 conv=notrunc >& /dev/null

    dd if=$patt of=$license1 bs=16 skip=1 count=1 &> /dev/null
    b_010=$(xxd -ps -s0 -l1 $license1)
    if [ "$enable_dif_password" == "true" ]; then
        b_010="$(printf %02x $(( 0x$b_010 | 0x01 )))"
    fi
    echo $b_010 | xxd -r -p > $efusebit
    dd if=$efusebit of=$license1 bs=1 seek=0 count=1 conv=notrunc >& /dev/null

    b_015=$(xxd -ps -s5 -l1 $license1)
    if [ "$enable_dvuk_derive_with_cid" == "true" ]; then
        b_015="$(printf %02x $(( 0x$b_015 | 0x02 )))"
    fi
    echo $b_015 | xxd -r -p > $efusebit
    dd if=$efusebit of=$license1 bs=1 seek=5 count=1 conv=notrunc >& /dev/null

    filesize=$(wc -c < $license1)
    if [ $filesize -ne 16 ]; then
        echo Internal Error -- Invalid write-lock0 pattern length
        exit 1
    fi
    dd if=$license1 of=$patt bs=16 seek=1 count=1 conv=notrunc >& /dev/null

    ${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output --option=debug

    rm -f $patt
    rm -f $wrlock0
    rm -f $wrlock1
    rm -f $hmac
    rm -f $license0
    rm -f $license1
    rm -f $efusebit
}

function append_uint32_le() {
    local input=$1
    local output=$2
    local v=
    local vrev=
    v=$(printf %08x $input)
    # 00010001
    vrev=${v:6:2}${v:4:2}${v:2:2}${v:0:2}

    echo $vrev | xxd -r -p >> $output
}

function generate_audio_id_pattern() {
    local argv=("$@")
    local i=0
    local patt=$(mktemp --tmpdir)
    local audio_id_efuse=$(mktemp --tmpdir)
    # default audio_id_offset 0xB8
    local audio_id_offset=184
    local audio_id_size=4
     # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --audio-id)
                audio_id_value="${argv[$i]}" ;;
           -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    if [ -z $audio_id_value ]; then
        echo Error: invalid audio_id_value
        exit 1
    fi

    # Generate empty eFUSE pattern data
    dd if=/dev/zero of=$patt count=4096 bs=1 &> /dev/null

    append_uint32_le $audio_id_value $audio_id_efuse
    dd if=$audio_id_efuse of=$patt bs=1 seek=$audio_id_offset count=$audio_id_size \
        conv=notrunc >& /dev/null

	${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output --option=debug

    rm -f $patt
    rm -f $audio_id_efuse
}

function generate_mkt_id_pattern() {
    local argv=("$@")
    local i=0
    local patt=$(mktemp --tmpdir)
    local mkt_id_efuse=$(mktemp --tmpdir)
    # default mkt_id_offset 0xA8
    local mkt_id_offset=168
    local mkt_id_size=4
     # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --device-scs-segid)
                mkt_id_value="${argv[$i]}" ;;
           -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    if [ -z $mkt_id_value ]; then
        echo Error: invalid mkt_id_value
        exit 1
    fi

    # Generate empty eFUSE pattern data
    dd if=/dev/zero of=$patt count=4096 bs=1 &> /dev/null

    append_uint32_le $mkt_id_value $mkt_id_efuse
    dd if=$mkt_id_efuse of=$patt bs=1 seek=$mkt_id_offset count=$mkt_id_size \
        conv=notrunc >& /dev/null

	${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output --option=debug

    rm -f $patt
    rm -f $mkt_id_efuse
}

function generate_vendor_id_pattern() {
    local argv=("$@")
    local i=0
    local patt=$(mktemp --tmpdir)
    local vend_id_efuse=$(mktemp --tmpdir)
    # default vend_id_offset 0xAC
    local vend_id_offset=172
    local vend_id_size=4
     # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --device-vendor-segid)
                vend_id_value="${argv[$i]}" ;;
           -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    if [ -z $vend_id_value ]; then
        echo Error: invalid vendor_id_value
        exit 1
    fi

    # Generate empty eFUSE pattern data
    dd if=/dev/zero of=$patt count=4096 bs=1 &> /dev/null

    append_uint32_le $vend_id_value $vend_id_efuse
    dd if=$vend_id_efuse of=$patt bs=1 seek=$vend_id_offset count=$vend_id_size \
        conv=notrunc >& /dev/null

	${BASEDIR_TOP}/aml_encrypt_sc2 --efsproc --input $patt --output $output --option=debug

    rm -f $patt
    rm -f $vend_id_efuse
}


parse_main() {
    case "$@" in
        --help)
            usage
            ;;
        --version)
            echo "$(basename $0) version $VERSION"
            ;;
        *--device-vendor-segid*)
            generate_vendor_id_pattern "$@"
            ;;
        *--device-scs-segid*)
            generate_mkt_id_pattern "$@"
            ;;
        *-o*)
            generate_efuse_device_pattern "$@"
            ;;
        *)
            usage "$@"
            ;;
    esac
}

parse_main "$@"
