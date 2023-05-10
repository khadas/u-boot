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
       $(basename $0) [--device-roothash device_roothash.bin] \\
                      [--dvgk dvgk.bin] \\
                      [--dvuk dvuk.bin] \\
                      [--enable-usb-password true] \\
                      [--enable-dif-password true] \\
                      [--enable-dvuk-derive-with-cid true] \\
                      [--enable-device-vendor-scs true] \\
                      -o pattern.efuse
       $(basename $0) --audio-id audio_id_value \\
                      -o audio_id.efuse
       $(basename $0) --device-scs-segid seg_id_value \\
                      -o device-scs-id.efuse
       $(basename $0) --device-vendor-segid vend_id_value \\
                      -o device-vendor-id.efuse
EOF
    exit 1
}

function generate_efuse_device_pattern() {
    local argv=("$@")
    local i=0
	local patt_text=$(mktemp --tmpdir)

    # Parse args
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
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

    cp $patt_text $output
    rm -f $patt_text
}

function generate_audio_id_pattern() {
    local argv=("$@")
    local i=0
	local patt_text=$(mktemp --tmpdir)

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

    v=$(printf %08x $audio_id_value)
    id_info=${v:6:2}${v:4:2}${v:2:2}${v:0:2}
    echo "efuse_obj set AUDIO_VENDOR_ID $id_info" >> $patt_text
    echo "efuse_obj lock AUDIO_VENDOR_ID" >> $patt_text

    cp $patt_text $output
    rm -f $patt_text
}


function generate_mkt_id_pattern() {
    local argv=("$@")
    local i=0
    local patt_text=$(mktemp --tmpdir)

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

    v=$(printf %08x $mkt_id_value)
    id_info=${v:6:2}${v:4:2}${v:2:2}${v:0:2}
    echo "efuse_obj set DEVICE_SCS_SEGID $id_info" >> $patt_text
    echo "efuse_obj lock DEVICE_SCS_SEGID" >> $patt_text

    cp $patt_text $output
    rm -f $patt_text
}

function generate_vendor_id_pattern() {
    local argv=("$@")
    local i=0
    local patt_text=$(mktemp --tmpdir)

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

    v=$(printf %08x $vend_id_value)
    id_info=${v:6:2}${v:4:2}${v:2:2}${v:0:2}
    echo "efuse_obj set DEVICE_VENDOR_SEGID $id_info" >> $patt_text
    echo "efuse_obj lock DEVICE_VENDOR_SEGID" >> $patt_text

    cp $patt_text $output
    rm -f $patt_text
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
