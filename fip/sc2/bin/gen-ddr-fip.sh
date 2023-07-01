#!/bin/bash
# due to size limit of BL2, only one type of DDR firmware is
# built into bl2 code package. For support other ddr types, we
# need bind them to ddr_fip.bin and let bl2 fw to try it.
#
# Note: No piei fw in following arry because it have build into
# bl2
# Total ddr-fip.bin size: 256KB, 4KB for header, 252(36*7)KB for fw
# so max 7 ddr fw support
function mk_ddr_fip()
{
	local key_type=$1
	local chip_set_name=$2
	local build_path=$3
	local ddr_fip_name=$4

	local ddr3_1d="${build_path}/ddr3_1d.fw"
	local ddr4_1d="${build_path}/ddr4_1d.fw"
	local ddr4_2d="${build_path}/ddr4_2d.fw"
	local lpddr3_1d="${build_path}/lpddr3_1d.fw"
	local lpddr4_1d="${build_path}/lpddr4_1d.fw"
	local lpddr4_2d="${build_path}/lpddr4_2d.fw"
	local input_dir=./${FIP_FOLDER}${CUR_SOC}
	local out_fip=${build_path}/${ddr_fip_name}
	local exec_basedir=$(dirname $(readlink -f $0))

	if [ -z ${key_type} ]; then
		key_type="dev-keys"
	fi

	dd if=${input_dir}/ddr3_1d.fw   of=${ddr3_1d}   skip=96 bs=1 count=36864 &> /dev/null
	dd if=${input_dir}/ddr4_1d.fw   of=${ddr4_1d}   skip=96 bs=1 count=36864 &> /dev/null
	dd if=${input_dir}/ddr4_2d.fw   of=${ddr4_2d}   skip=96 bs=1 count=36864 &> /dev/null
	dd if=${input_dir}/lpddr3_1d.fw of=${lpddr3_1d} skip=96 bs=1 count=36864 &> /dev/null
	dd if=${input_dir}/lpddr4_1d.fw of=${lpddr4_1d} skip=96 bs=1 count=36864 &> /dev/null
	dd if=${input_dir}/lpddr4_2d.fw of=${lpddr4_2d} skip=96 bs=1 count=36864 &> /dev/null

	RSA_BL2_BASE="${FIP_FOLDER}${CUR_SOC}/keys/${key_type}/${CUR_SOC}/chipset/bl2/rsa/${chip_set_name}/"
	AES_BL2_BASE="${FIP_FOLDER}${CUR_SOC}/keys/${key_type}/${CUR_SOC}/chipset/bl2/aes/${chip_set_name}/"

	${exec_basedir}/download-keys.sh ${key_type} ${CUR_SOC} chipset

	echo ==== create ddr_fip ====
	./${FIP_FOLDER}${CUR_SOC}/binary-tool/acpu-imagetool create-ddrfw-fip \
		--infile-ddrfw-ddr3-1d=${ddr3_1d} \
		--infile-ddrfw-ddr4-1d=${ddr4_1d} \
		--infile-ddrfw-ddr4-2d=${ddr4_2d} \
		--infile-ddrfw-lpddr3-1d=${lpddr3_1d} \
		--infile-ddrfw-lpddr4-1d=${lpddr4_1d} \
		--infile-ddrfw-lpddr4-2d=${lpddr4_2d} \
		--infile-signkey-chipset-lvl1=${RSA_BL2_BASE}level-1-rsa-priv.pem \
		--infile-aes256-ddrfw-ddr3-1d=${AES_BL2_BASE}genkey-prot-ddrfw-ddr3-1d.bin \
		--infile-aes256-ddrfw-ddr4-1d=${AES_BL2_BASE}genkey-prot-ddrfw-ddr4-1d.bin \
		--infile-aes256-ddrfw-ddr4-2d=${AES_BL2_BASE}genkey-prot-ddrfw-ddr4-2d.bin \
		--infile-aes256-ddrfw-lpddr3-1d=${AES_BL2_BASE}genkey-prot-ddrfw-lpddr3-1d.bin \
		--infile-aes256-ddrfw-lpddr4-1d=${AES_BL2_BASE}genkey-prot-ddrfw-lpddr4-1d.bin \
		--infile-aes256-ddrfw-lpddr4-2d=${AES_BL2_BASE}genkey-prot-ddrfw-lpddr4-2d.bin \
		--outfile-ddrfw-fip=${out_fip}

	rm -rf ${build_path}/*ddr*_*d.fw
}

mk_ddr_fip $@
