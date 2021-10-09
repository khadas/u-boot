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
                      [--device-roothash device_roothash.bin] \\
                      [--dvgk dvgk.bin] \\
                      [--dvuk dvuk.bin] \\
                      [--enable-usb-password true] \\
                      [--enable-dif-password true] \\
                      [--enable-dvuk-derive-with-cid true] \\
                      [--enable-device-vendor-scs true] \\
                      -o pattern.efuse
       $(basename $0) --audio-id audio_id_value \\
                      -o audio_id.efuse
EOF
    exit 1
}

function generate_efuse_device_pattern() {
    local argv=("$@")
    local i=0

	local patt_text=$(mktemp --tmpdir)

	local patt=$(mktemp --tmpdir)
	local wrlock=$(mktemp --tmpdir)
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
            --dvuk)
                dvuk="${argv[$i]}" ;;
			--device-roothash)
                device_roothash="${argv[$i]}" ;;
            --enable-usb-password)
                enable_usb_password="${argv[$i]}" ;;
            --enable-dif-password)
                enable_dif_password="${argv[$i]}" ;;
            --enable-dvuk-derive-with-cid)
                enable_dvuk_derive_with_cid="${argv[$i]}" ;;
            --enable-device-vendor-scs)
                enable_device_vendor_scs="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    # Verify args
    if [ -z "$output" ]; then echo Error: Missing output file option -o; exit 1; fi

    check_opt_file input 1024 "$input"
    check_opt_file dvgk 16 "$dvgk"
    check_opt_file dvuk 16 "$dvuk"
    check_opt_file device_roothash 32 "$device_roothash"

    check_opt_boolean enable-usb-password "$enable_usb_password"
    check_opt_boolean enable-dif-password "$enable_dif_password"
    check_opt_boolean enable-dvuk-derive-with-cid "$enable_dvuk_derive_with_cid"
    check_opt_boolean enable-device-vendor-scs "$enable_device_vendor_scs"

	#efuse_obj mode
	if [ "$dvgk" != "" ]; then
		keyinfo="$(xxd -p -c 16 $dvgk)"
		echo "efuse_obj set DVGK $keyinfo" >> $patt_text
		echo "efuse_obj lock DVGK" >> $patt_text
	fi

	if [ "$dvuk" != "" ]; then
		keyinfo="$(xxd -p -c 16 $dvuk)"
		echo "efuse_obj set DVUK $keyinfo" >> $patt_text
		echo "efuse_obj lock DVUK" >> $patt_text
	fi

	if [ "$device_roothash" != "" ]; then
		keyinfo="$(xxd -p -c 32 $device_roothash)"
		echo "efuse_obj set HASH_NORMAL_DEVICE_ROOTCERT $keyinfo" >> $patt_text
		echo "efuse_obj lock HASH_NORMAL_DEVICE_ROOTCERT" >> $patt_text
		echo "efuse_obj set HASH_DFU_DEVICE_ROOTCERT $keyinfo" >> $patt_text
		echo "efuse_obj lock HASH_DFU_DEVICE_ROOTCERT" >> $patt_text
	fi

	if [ "$enable_device_vendor_scs" == "true" ]; then
		echo "efuse_obj set FEAT_ENABLE_DEVICE_ROOT_PUBRSA_PROT 01" >> $patt_text
		echo "efuse_obj set FEAT_ENABLE_DEVICE_LVL1_PUBRSA_PROT 01" >> $patt_text
		echo "efuse_obj set FEAT_ENABLE_DEVICE_LVLX_PUBRSA_PROT 01" >> $patt_text
		echo "efuse_obj set FEAT_ENABLE_DEVICE_VENDOR_SIG 01" >> $patt_text
		echo "efuse_obj set FEAT_ENABLE_DEVICE_PROT 01" >> $patt_text
		echo "efuse_obj set FEAT_ENABLE_DEVICE_SCS_SIG 01" >> $patt_text
	fi

	if [ "$enable_usb_password" == "true" ]; then
		echo "efuse_obj set FEAT_ENABLE_USB_AUTH 01" >> $patt_text
	fi

	if [ "$enable_dif_password" == "true" ]; then
		echo "efuse_obj set FEAT_ENABLE_DIF_MASTER_PROT 01" >> $patt_text
	fi

	if [ "$enable_dvuk_derive_with_cid" == "true" ]; then
		echo "efuse_obj set FEAT_ENABLE_DVUK_DERIVE_WITH_CID 01" >> $patt_text
	fi
	cp $patt_text $output.obj
	rm -f $patt_text

	# Generate empty eFUSE pattern data
	if [ -n "$input" ]; then
		dd if="$input" of=$patt count=1024 bs=1 &> /dev/null
	else
		dd if=/dev/zero of=$patt count=1024 bs=1 &> /dev/null
	fi

	# Construct wrlock bits
	dd if=$patt of=$wrlock bs=16 skip=29 count=1 &> /dev/null
	b_1d2=$(xxd -ps -s2 -l1 $wrlock)
	b_1d3=$(xxd -ps -s3 -l1 $wrlock)
	b_1d6=$(xxd -ps -s6 -l1 $wrlock)

	if [ "$dvgk" != "" ]; then
		dd if="$dvgk" of="$patt" bs=16 seek=50 count=1 \
			conv=notrunc >& /dev/null
		b_1d6="$(printf %02x $(( 0x$b_1d6 | 0x04 )))"
	fi

	if [ "$dvuk" != "" ]; then
		dd if="$dvuk" of="$patt" bs=16 seek=51 count=1 \
			conv=notrunc >& /dev/null
		b_1d6="$(printf %02x $(( 0x$b_1d6 | 0x08 )))"
	fi

	if [ "$device_roothash" != "" ]; then
		dd if="$device_roothash" of="$patt" bs=16 seek=23 count=2 \
			conv=notrunc >& /dev/null
		dd if="$device_roothash" of="$patt" bs=16 seek=25 count=2 \
			conv=notrunc >& /dev/null
		b_1d2="$(printf %02x $(( 0x$b_1d2 | 0x80 )))"
		b_1d3="$(printf %02x $(( 0x$b_1d3 | 0x07 )))"
	fi

	echo $b_1d2 | xxd -r -p > $efusebit
	dd if=$efusebit of=$wrlock bs=1 seek=2 count=1 conv=notrunc >& /dev/null
	echo $b_1d3 | xxd -r -p > $efusebit
	dd if=$efusebit of=$wrlock bs=1 seek=3 count=1 conv=notrunc >& /dev/null
	echo $b_1d6 | xxd -r -p > $efusebit
	dd if=$efusebit of=$wrlock bs=1 seek=6 count=1 conv=notrunc >& /dev/null

	filesize=$(wc -c < $wrlock)
	if [ $filesize -ne 16 ]; then
		echo Internal Error -- Invalid write-lock pattern length
		exit 1
	fi
	dd if=$wrlock of=$patt bs=16 seek=29 count=1 conv=notrunc >& /dev/null

	dd if=$patt of=$license0 bs=16 skip=0 count=1 &> /dev/null
	b_001=$(xxd -ps -s1 -l1 $license0)
	b_002=$(xxd -ps -s2 -l1 $license0)
	b_003=$(xxd -ps -s3 -l1 $license0)
	if [ "$enable_device_vendor_scs" == "true" ]; then
		b_001="$(printf %02x $(( 0x$b_001 | 0x0C )))"
		b_002="$(printf %02x $(( 0x$b_002 | 0xFF )))"
		b_003="$(printf %02x $(( 0x$b_003 | 0x03 )))"
	fi
	echo $b_001 | xxd -r -p > $efusebit
	dd if=$efusebit of=$license0 bs=1 seek=1 count=1 conv=notrunc >& /dev/null
	echo $b_002 | xxd -r -p > $efusebit
	dd if=$efusebit of=$license0 bs=1 seek=2 count=1 conv=notrunc >& /dev/null
	echo $b_003 | xxd -r -p > $efusebit
	dd if=$efusebit of=$license0 bs=1 seek=3 count=1 conv=notrunc >& /dev/null

	b_007=$(xxd -ps -s7 -l1 $license0)
	if [ "$enable_usb_password" == "true" ]; then
		b_007="$(printf %02x $(( 0x$b_007 | 0x80 )))"
	fi
	echo $b_007 | xxd -r -p > $efusebit
	dd if=$efusebit of=$license0 bs=1 seek=7 count=1 conv=notrunc >& /dev/null

	filesize=$(wc -c < $license0)
	if [ $filesize -ne 16 ]; then
		echo Internal Error -- Invalid license0 pattern length
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
		echo Internal Error -- Invalid license1 pattern length
		exit 1
	fi
	dd if=$license1 of=$patt bs=16 seek=1 count=1 conv=notrunc >& /dev/null

	${BASEDIR_TOP}/aml_encrypt_p1 --efsproc --input $patt --output $output --option=debug

	rm -f $patt
	rm -f $wrlock
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

	local patt_text=$(mktemp --tmpdir)

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

	#efuse_obj mode
	v=$(printf %08x $audio_id_value)
	id_info=${v:6:2}${v:4:2}${v:2:2}${v:0:2}
	echo "efuse_obj set AUDIO_VENDOR_ID $id_info" >> $patt_text
	echo "efuse_obj lock AUDIO_VENDOR_ID" >> $patt_text
	cp $patt_text $output.obj
	rm -f $patt_text

	# Generate empty eFUSE pattern data
	dd if=/dev/zero of=$patt count=1024 bs=1 &> /dev/null

	append_uint32_le $audio_id_value $audio_id_efuse
	dd if=$audio_id_efuse of=$patt bs=1 seek=$audio_id_offset count=$audio_id_size \
		conv=notrunc >& /dev/null

	${BASEDIR_TOP}/aml_encrypt_p1 --efsproc --input $patt --output $output --option=debug

	rm -f $patt
	rm -f $audio_id_efuse
}

parse_main() {
    case "$@" in
        --help)
            usage
            ;;
        --version)
            echo "$(basename $0) version $VERSION"
            ;;
        *--audio-id*)
            generate_audio_id_pattern "$@"
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
