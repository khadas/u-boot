#!/bin/bash

set -euo pipefail

readonly script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly tools_dir=${script_dir}

sign_boot_tool="${tools_dir}/signing-tool-g12a/sign-boot-g12a.sh"
sign_boot_tool_dev="${tools_dir}/signing-tool-g12a-dev/sign-boot-g12a-dev.sh"
efuse_gen="${tools_dir}/signing-tool-g12a-dev/efuse-gen.sh"


function usage() {
  echo "usage: $(basename "$0") -p <bootloader-prebuilts-dir>"
  echo "-r <signing-rsa-public-key-dir>"
  echo "-a <encryption-aes-key-dir>"
  echo "-b <firmware-antirollback-config>"
  echo "-n"
  echo "-u"
  echo "-o <output-dir>"
}

soc=
prebuilts=
fw_krsa_dir=
fw_kaes_dir=
bl_out_dir=
encryption_option="FIPbl3x"
postfix="signed"
unsigned_only="false"
ddrfw=
BL32_IMG=
hash_ver=2
soc=
enable_antirollback="false"
fw_arb_config=
bl2_arb_cvn="0x0"
fip_arb_cvn="0x0"
bl30_arb_cvn="0x0"
bl31_arb_cvn="0x0"
bl32_arb_cvn="0x0"
bl33_arb_cvn="0x0"
with_bl40="false"
sign_bl40="false"

while getopts "s:h:p:r:a:b:unmdo:" opt; do
  case $opt in
    s) readonly soc="$OPTARG" ;;
    h) readonly hash_ver="$OPTARG" ;;
    p) readonly prebuilts="$OPTARG" ;;
    r) readonly fw_krsa_dir="$OPTARG" ;;
    a) readonly fw_kaes_dir="$OPTARG" ;;
    b) readonly fw_arb_config="$OPTARG" ;;
    n) readonly encryption_option="none" ;;
    m) readonly with_bl40="true" ;;
    d) readonly sign_bl40="true" ;;
    u) readonly unsigned_only="true"; encryption_option="none" ;;
    o) readonly bl_out_dir="$OPTARG" ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      usage
      exit 2
      ;;
  esac
done


soc_prebuilts=$prebuilts
if [[ -z $prebuilts || ! -d $soc_prebuilts ]]; then
  echo "SoC bootloader prebuilts directory $soc_prebuilts does not exist"
  usage
  exit 1
fi

if [[ -z $bl_out_dir || ! -d $bl_out_dir ]]; then
  echo "Output directory $bl_out_dir does not exist"
  usage
  exit 1
fi

if [ -e "${soc_prebuilts}/bl32.img"  ]; then
  BL32_IMG="${soc_prebuilts}/bl32.img"
fi

ddrfw=$soc_prebuilts
soc_fw_krsa_dir=$fw_krsa_dir
soc_fw_kaes_dir=$fw_kaes_dir

# Pre-defined key file in key directory
readonly rsa_root=${soc_fw_krsa_dir}/root.pem
readonly rsa_root0=${soc_fw_krsa_dir}/root0.pem
readonly rsa_root1=${soc_fw_krsa_dir}/root1.pem
readonly rsa_root2=${soc_fw_krsa_dir}/root2.pem
readonly rsa_root3=${soc_fw_krsa_dir}/root3.pem
readonly rsa_bl2=${soc_fw_krsa_dir}/bl2.pem
readonly rsa_bl30=${soc_fw_krsa_dir}/bl3xkey.pem
readonly rsa_bl31=${soc_fw_krsa_dir}/bl3xkey.pem
readonly rsa_bl32=${soc_fw_krsa_dir}/bl3xkey.pem
readonly rsa_bl33=${soc_fw_krsa_dir}/bl3xkey.pem
readonly rsa_bl40=${soc_fw_krsa_dir}/bl40key.pem
readonly rsa_kernel=${soc_fw_krsa_dir}/kernelkey.pem

readonly kaes_bl2=${soc_fw_kaes_dir}/bl2aeskey
readonly kaes_bl30=${soc_fw_kaes_dir}/bl3xaeskey
readonly kaes_bl31=${soc_fw_kaes_dir}/bl3xaeskey
readonly kaes_bl32=${soc_fw_kaes_dir}/bl3xaeskey
readonly kaes_bl33=${soc_fw_kaes_dir}/bl3xaeskey
readonly kaes_bl40=${soc_fw_kaes_dir}/bl40aeskey
readonly kaes_kernel=${soc_fw_kaes_dir}/kernelaeskey

readonly bl2_iv=${soc_fw_kaes_dir}/bl2aesiv
readonly bl30_iv=${soc_fw_kaes_dir}/bl3xaesiv
readonly bl31_iv=${soc_fw_kaes_dir}/bl3xaesiv
readonly bl32_iv=${soc_fw_kaes_dir}/bl3xaesiv
readonly bl33_iv=${soc_fw_kaes_dir}/bl3xaesiv
readonly bl40_iv=${soc_fw_kaes_dir}/bl40aesiv
readonly kernel_iv=${soc_fw_kaes_dir}/kernelaesiv

function check_rsa_key_files() {
    if [[ -z fw_krsa_dir || ! -d $soc_fw_krsa_dir ]]; then
      echo "SoC RSA key directory $soc_fw_krsa_dir does not exist"
      usage
      exit 1
    fi

    if [ ! -s ${rsa_root} ]; then
        echo "Missing ${rsa_root} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_root0} ]; then
        echo "Missing ${rsa_root0} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_root1} ]; then
        echo "Missing ${rsa_root1} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_root2} ]; then
        echo "Missing ${rsa_root2} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_root3} ]; then
        echo "Missing ${rsa_root3} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_bl2} ]; then
        echo "Missing ${rsa_bl2} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_bl30} ]; then
        echo "Missing ${rsa_bl30} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_bl31} ]; then
        echo "Missing ${rsa_bl31} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_bl32} ]; then
        echo "Missing ${rsa_bl32} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_bl33} ]; then
        echo "Missing ${rsa_bl33} RSA private key file"
        exit 1
    fi
    if [ ! -s ${rsa_kernel} ]; then
        echo "Missing ${rsa_kernel} RSA private key file"
        exit 1
    fi
}

function check_aes_key_files() {
    if [[ -z fw_kaes_dir || ! -d $soc_fw_kaes_dir ]]; then
      echo "SoC AES encryption key directory $soc_fw_kaes_dir does not exist"
      usage
      exit 1
    fi

    if [ ! -s ${kaes_bl2} ]; then
        echo "Missing ${kaes_bl2} AES key file"
        exit 1
    fi
    if [ ! -s ${kaes_bl30} ]; then
        echo "Missing ${kaes_bl30} AES key file"
        exit 1
    fi
    if [ ! -s ${kaes_bl31} ]; then
        echo "Missing ${kaes_bl31} AES key file"
        exit 1
    fi
    if [ ! -s ${kaes_bl32} ]; then
        echo "Missing ${kaes_bl32} AES key file"
        exit 1
    fi
    if [ ! -s ${kaes_bl33} ]; then
        echo "Missing ${kaes_bl33} AES key file"
        exit 1
    fi
    if [ ! -s ${kaes_kernel} ]; then
        echo "Missing ${kaes_kernel} AES key file"
        exit 1
    fi
}

function check_aes_iv_files() {
    if [[ -z fw_kaes_dir || ! -d $soc_fw_kaes_dir ]]; then
      echo "SoC AES iv directory $soc_fw_kaes_dir does not exist"
      usage
      exit 1
    fi

   if [ ! -s ${bl2_iv} ]; then
        echo "Missing ${bl2_iv} AES key file"
        exit 1
    fi
    if [ ! -s ${bl30_iv} ]; then
        echo "Missing ${bl30_iv} AES key file"
        exit 1
    fi
    if [ ! -s ${bl31_iv} ]; then
        echo "Missing ${bl31_iv} AES key file"
        exit 1
    fi
    if [ ! -s ${bl32_iv} ]; then
        echo "Missing ${bl32_iv} AES key file"
        exit 1
    fi
    if [ ! -s ${bl33_iv} ]; then
        echo "Missing ${bl33_iv} AES key file"
        exit 1
    fi
    if [ ! -s ${kernel_iv} ]; then
        echo "Missing ${kernel_iv} AES key file"
        exit 1
    fi
}

soc_fw_arb_config=$fw_arb_config
if [[ ! -z $fw_arb_config && -s $soc_fw_arb_config ]]; then
while read line
do
	eval "$line"
done < $soc_fw_arb_config
fi

if [ $unsigned_only != "true" ]; then
  check_rsa_key_files
fi

encryption_flags=

if [ $encryption_option != "none" ]; then
  # encryption enabled (default)

  postfix="signed.encrypted"
  check_aes_key_files
	check_aes_iv_files

  encryption_flags="${encryption_flags} --bl2-aes-key ${kaes_bl2}"
  encryption_flags="${encryption_flags} --bl2-aes-iv ${bl2_iv}"
  encryption_flags="${encryption_flags} --bl30-aes-key ${kaes_bl30}"
  encryption_flags="${encryption_flags} --bl30-aes-iv ${bl30_iv}"
  encryption_flags="${encryption_flags} --bl31-aes-key ${kaes_bl31}"
  encryption_flags="${encryption_flags} --bl31-aes-iv ${bl31_iv}"
  encryption_flags="${encryption_flags} --bl32-aes-key ${kaes_bl32}"
  encryption_flags="${encryption_flags} --bl32-aes-iv ${bl32_iv}"
  encryption_flags="${encryption_flags} --bl33-aes-key ${kaes_bl33}"
  encryption_flags="${encryption_flags} --bl33-aes-iv ${bl33_iv}"

  encryption_flags="${encryption_flags} --kernel-aes-key ${kaes_kernel}"
  encryption_flags="${encryption_flags} --kernel-aes-iv ${kernel_iv}"

  encryption_flags_bl40="--bl40-aes-key  ${kaes_bl40} --bl40-aes-iv ${bl40_iv}"
fi

bl40_flags=

if [ $unsigned_only != "true" ]; then
  if [ ${sign_bl40} == "true" ]; then
      "$sign_boot_tool" --create-signed-bl40 \
          --key-hash-ver 2                               \
          --root-key-idx 0                               \
          --root-key    "$rsa_bl40"                      \
          --root-key-0  "$rsa_bl40"                      \
          --root-key-1  "$rsa_bl40"                      \
          --bl40         "${soc_prebuilts}/bl40.bin"     \
          --bl40-key     "$rsa_bl40"                     \
          ${encryption_flags_bl40}                       \
          -e            "$encryption_option"             \
          --bl40-arb-cvn 0x0                             \
          -o            "${bl_out_dir}/bl40.bin.${postfix}"
      bl40_flags="--bl40 ${bl_out_dir}/bl40.bin.${postfix}"
  elif [ ${with_bl40} == "true" ]; then
      bl40_flags="--bl40 ${soc_prebuilts}/bl40.bin"
  fi

  "$sign_boot_tool" --create-signed-bl \
    --key-hash-ver 2                                     \
    --root-key-idx 0                                     \
    --root-key    "$rsa_root"                            \
    --root-key-0  "$rsa_root0"                           \
    --root-key-1  "$rsa_root1"                           \
    --root-key-2  "$rsa_root2"                           \
    --root-key-3  "$rsa_root3"                           \
    --bl2         "${soc_prebuilts}/bl2_new.bin"         \
    --bl2-key     "$rsa_bl2"                             \
    --bl30        "${soc_prebuilts}/bl30_new.bin"        \
    --bl30-key    "$rsa_bl30"                            \
    --bl31        "${soc_prebuilts}/bl31.img"            \
    --bl31-key    "$rsa_bl31"                            \
    --bl32        "${BL32_IMG}"                          \
    --bl32-key    "$rsa_bl32"                            \
    --bl33        "${soc_prebuilts}/bl33.bin"            \
    --bl33-key    "$rsa_bl33"                            \
    --fip-key     "$rsa_bl2"                             \
    --kernel-key  "$rsa_kernel"                          \
    --ddrfw1       "${ddrfw}/ddr4_1d.fw"                 \
    --ddrfw2       "${ddrfw}/ddr4_2d.fw"                 \
    --ddrfw3       "${ddrfw}/ddr3_1d.fw"                 \
    --ddrfw4       "${ddrfw}/piei.fw"                    \
    --ddrfw5       "${ddrfw}/lpddr4_1d.fw"               \
    --ddrfw6       "${ddrfw}/lpddr4_2d.fw"               \
    --ddrfw7       "${ddrfw}/diag_lpddr4.fw"             \
    --ddrfw8       "${ddrfw}/aml_ddr.fw"                 \
    --ddrfw9  "${soc_prebuilts}/lpddr3_1d.fw"            \
    ${bl40_flags}                                        \
    ${encryption_flags}                                  \
    -e            "$encryption_option"                   \
    --bl2-arb-cvn $bl2_arb_cvn                           \
    --fip-arb-cvn $fip_arb_cvn                           \
    --bl30-arb-cvn $bl30_arb_cvn                         \
    --bl31-arb-cvn $bl31_arb_cvn                         \
    --bl32-arb-cvn $bl32_arb_cvn                         \
    --bl33-arb-cvn $bl33_arb_cvn                         \
    -o            "${bl_out_dir}/u-boot.bin.${postfix}"

  head -c 65536 "${bl_out_dir}/u-boot.bin.${postfix}" > "${bl_out_dir}/u-boot.bin.usb.bl2.${postfix}"
  tail -c +65537 "${bl_out_dir}/u-boot.bin.${postfix}" > "${bl_out_dir}/u-boot.bin.usb.tpl.${postfix}"
	dd if=/dev/urandom of=${bl_out_dir}/u-boot.bin.${postfix}.sd.bin count=1 >& /dev/null
	cat ${bl_out_dir}/u-boot.bin.${postfix} >> ${bl_out_dir}/u-boot.bin.${postfix}.sd.bin
fi

if [ ${with_bl40} == "true" ]; then
  bl40_flags="--bl40 ${soc_prebuilts}/bl40.bin"
fi
"$sign_boot_tool_dev" --create-unsigned-bl              \
    --bl2 "${soc_prebuilts}/bl2_new.bin"                \
    --bl30 "${soc_prebuilts}/bl30_new.bin"              \
    --bl31 "${soc_prebuilts}/bl31.img"                  \
    --bl32 "${BL32_IMG}"                                \
    --bl33 "${soc_prebuilts}/bl33.bin"                  \
    ${bl40_flags}                                       \
    --ddrfw1       "${ddrfw}/ddr4_1d.fw"                \
    --ddrfw2       "${ddrfw}/ddr4_2d.fw"                \
    --ddrfw3       "${ddrfw}/ddr3_1d.fw"                \
    --ddrfw4       "${ddrfw}/piei.fw"                   \
    --ddrfw5       "${ddrfw}/lpddr4_1d.fw"              \
    --ddrfw6       "${ddrfw}/lpddr4_2d.fw"              \
    --ddrfw7       "${ddrfw}/diag_lpddr4.fw"            \
    --ddrfw8       "${ddrfw}/aml_ddr.fw"                \
    --ddrfw9  "${soc_prebuilts}/lpddr3_1d.fw"           \
    -o "${bl_out_dir}/u-boot.bin.unsigned"

head -c 65536 "${bl_out_dir}/u-boot.bin.unsigned" > "${bl_out_dir}/u-boot.bin.usb.bl2.unsigned"
tail -c +65537 "${bl_out_dir}/u-boot.bin.unsigned" > "${bl_out_dir}/u-boot.bin.usb.tpl.unsigned"
dd if=/dev/urandom of=${bl_out_dir}/u-boot.bin.unsigned.sd.bin count=1 >& /dev/null
cat ${bl_out_dir}/u-boot.bin.unsigned >> ${bl_out_dir}/u-boot.bin.unsigned.sd.bin

if [ ${sign_bl40} == "true" ]; then
"$sign_boot_tool_dev" --create-unsigned-bl40 \
    --bl40 "${soc_prebuilts}/bl40.bin" \
    -o "${bl_out_dir}/bl40.bin.unsigned"
fi

if [ $unsigned_only != "true" ]; then
    rsa_root_hash=${soc_fw_krsa_dir}/rootkeys-hash.bin

    "$sign_boot_tool_dev" --create-root-hash \
        --key-hash-ver 2                       \
        --root-key-0 "$rsa_root0"              \
        --root-key-1 "$rsa_root1"              \
        --root-key-2 "$rsa_root2"              \
        --root-key-3 "$rsa_root3"              \
        -o "$rsa_root_hash"

    "$efuse_gen" --generate-efuse-pattern \
        --soc g12a                          \
        --key-hash-ver 2                    \
        --root-hash "$rsa_root_hash"        \
        --aes-key "${kaes_bl2}"             \
        --enable-sb true                    \
        --enable-aes true                   \
        --enable-anti-rollback $enable_antirollback    \
        -o "${bl_out_dir}/pattern.secureboot.efuse"

    #if found usb.password.hash.bin then generate pattern.usb.efuse
    #which contain usb password pattern

    readonly passwordhash=${soc_fw_krsa_dir}/usb.password.hash.bin
    if [ -f $passwordhash ]; then
        "$efuse_gen" --generate-efuse-pattern \
        --soc g12a                          \
        --key-hash-ver 2                    \
        --root-hash "$rsa_root_hash"        \
        --aes-key "${kaes_bl2}"             \
        --enable-sb true                    \
        --enable-aes true                   \
        --enable-anti-rollback $enable_antirollback    \
        --enable-usb-password true          \
        --password-hash $passwordhash       \
        -o "${bl_out_dir}/pattern.efuse"

        "$efuse_gen" --generate-efuse-pattern \
        --soc g12a                          \
        --enable-usb-password true          \
        --password-hash $passwordhash       \
        -o "${bl_out_dir}/pattern.usb.efuse"
    else
      cp -f "${bl_out_dir}/pattern.secureboot.efuse" "${bl_out_dir}/pattern.efuse"
    fi

    rm -f "$rsa_root_hash"
fi
