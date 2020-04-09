#!/bin/bash -e

readonly script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly fip_parser_tool="${script_dir}/fip_parser"

soc=""
uboot=""
outdir=""

#default g12a/g12b config
fipsize=16384       #fipsize(16k)(0x4000)
bl2hdr_size=4096
bl2bin_size=61440   #gxl/txlx is different from g12a/g12b/tl1/tm2
bl2size=            #bl2hdr_size+bl2bin_size.
bl30hdr_size=4096   #gxl/txlx is different from g12a/g12b/tl1/tm2
bl30bin_size=54272  #tm2 is different from g12a/g12b/tl1/gxl/txlx
bl30size=           #bl30hdr_size+bl30bin_size
bl3xhdr_size=656

#output firmwares
bl2="bl2_new.bin"
bl30="bl30_new.bin"
bl31="bl31.img"
bl32="bl32.img"
bl33="bl33.bin"
ddrfw=("ddr4_1d.fw" "ddr4_2d.fw" "ddr3_1d.fw" "piei.fw" "lpddr4_1d.fw"
		"lpddr4_2d.fw" "diag_lpddr4.fw" "aml_ddr.fw" "lpddr3_1d.fw")

#tmp files and variables
TMP=`date +%Y%m%d%H%M%S`
uboot_no_bl2="$TMP/uboot_no_bl2.bin"
fip="$TMP/fip.bin"
fip_info="$TMP/fip_info.txt"

bl31_hdr="bl31.hdr"
bl32_hdr="bl32.hdr"
bl30_bin="$TMP/bl30.bin"
bl31_bin="$TMP/bl31.bin"
bl32_bin="$TMP/bl32.bin"
bl33_bin="$TMP/bl33.bin"
ddrhdr=("ddr1.hdr" "ddr2.hdr" "ddr3.hdr" "ddr4.hdr" "ddr5.hdr" "ddr6.hdr"
		"ddr7.hdr" "ddr8.hdr" "ddr9.hdr")
ddrbin=("$TMP/ddr1.bin" "$TMP/ddr2.bin" "$TMP/ddr3.bin" "$TMP/ddr4.bin"
		"$TMP/ddr5.bin" "$TMP/ddr6.bin" "$TMP/ddr7.bin" "$TMP/ddr8.bin"
		"$TMP/ddr9.bin")

bl30_entry_offset=""
bl30_entry_size=""
bl31_entry_offset=""
bl31_entry_size=""
bl32_entry_offset=""
bl32_entry_size=""
bl33_entry_offset=""
bl33_entry_size=""
ddrfw_count=""
ddr1_entry_offset=""
ddr1_entry_size=""
ddr2_entry_offset=""
ddr2_entry_size=""
ddr3_entry_offset=""
ddr3_entry_size=""
ddr4_entry_offset=""
ddr4_entry_size=""
ddr5_entry_offset=""
ddr5_entry_size=""
ddr6_entry_offset=""
ddr6_entry_size=""
ddr7_entry_offset=""
ddr7_entry_size=""
ddr8_entry_offset=""
ddr8_entry_size=""
ddr9_entry_offset=""
ddr9_entry_size=""

usage() {
	echo "parse unsigned uboot.bin to firmwares"
	echo "usage: $(basename $0) -s <soc> -i <uboot.bin> -o <outdir>"
	echo "support soc: gxl/txlx/g12a/g12b/tl1/tm2"
	exit 0
}

cleanup() {
	if [ $? -ne 0 ]; then
		echo "Failed to parse unsigned uboot!"
	fi
	if [ -d $TMP ]; then rm -rf $TMP; fi
	local tmpfiles="bl31.hdr bl32.hdr ddr1.hdr ddr2.hdr ddr3.hdr ddr4.hdr
		ddr5.hdr ddr6.hdr ddr7.hdr ddr8.hdr ddr9.hdr"
	for i in $tmpfiles ; do
		rm -f $i
	done
}

check_file() {
	if [[ ! -s $2 ]]; then echo Error:  $1: \""$2"\" does not exit or filesize is 0; exit 1 ; fi
}
check_value() {
	if [[ ! $2 -gt 0 ]]; then echo Error:  $1 value is  \""$2"\"; exit 1 ; fi
}

generate_bl2() {
	local input="$1"
	local uboot_no_bl2="$2"
	local bl2_out="$3"
	local bl2_with_hdr="$TMP/bl2_with_hdr.bin"

	echo -n "generate $bl2_out..."
	check_file "input" $input
	# output bl2 with header
	cp $input $bl2_with_hdr
	truncate -s $bl2size $bl2_with_hdr

	# cut off bl2 from uboot, output uboot without bl2
	tail -c +$(($bl2size+1)) $input > $uboot_no_bl2

	# cut off bl2 header, output bl2
	tail -c +$(($bl2hdr_size+1)) $bl2_with_hdr > $bl2_out
	echo "done"
}

generate_fip() {
	local input="$1"
	local fip_out="$2"

	echo -n "generate $fip_out..."
	check_file "input" $input
	dd if=$input of=$fip_out count=$fipsize bs=1 >& /dev/null
	echo "done"
}

parse_fip() {
	local input="$1"
	local fip_info_out="$2"

	echo -n "generate $fip_info, bl31.hdr,bl32.hdr,ddr*.hdr..."
	check_file "input" $input
	${fip_parser_tool} $input $fip_info_out

	# parse $fip_info
	if [[ -s $fip_info_out ]]; then
		while read line
		do
			eval "$line"
		done < $fip_info_out
	else
		echo "ERROR no $fip_info_out generated."
		cleanup
		exit 1
	fi
	echo "done"
}

generate_ddrfw() {
	local input="$1"
	local ddrfw="$2"
	local ddrhdr="$3"
	local ddrbin="$4"
	local offset="$5"
	local size="$6"

	echo -n "generate $ddrfw..."
	check_file "input" $input
	check_file "ddrhdr" $ddrhdr
	check_value "offset" $offset
	check_value "size" $size

	dd if=$input of=$ddrbin bs=1 skip=$offset count=$size >& /dev/null
	cat $ddrhdr $ddrbin > $ddrfw
	echo "done"
}

generate_bl3x() {
	local input="$1"
	local bl3x="$2"
	local bl3xhdr="$3"
	local bl3xbin="$4"
	local offset="$5"
	local size="$6"
	local bl3x_with_hdr="$TMP/bl3x_with_hdr.bin"

	echo -n "generate $bl3x..."
	check_file "input" $input
	check_value "offset" $offset
	check_value "size" $size

	dd if=$input of=$bl3x_with_hdr bs=1 skip=$offset count=$size >& /dev/null
	if [[ "$bl3x" = "$outdir/$bl30" ]]; then
		local insize=$(wc -c < $bl3x_with_hdr)
		if [[ $insize -gt $bl30size ]]; then
			truncate -s $bl30size $bl3x_with_hdr
		fi
		tail -c +$(($bl30hdr_size+1)) $bl3x_with_hdr > $bl3xbin
	else
		tail -c +$(($bl3xhdr_size+1)) $bl3x_with_hdr > $bl3xbin
	fi
	if [[ "$bl3xhdr" = "" ]]; then
		cp $bl3xbin $bl3x
	else
		check_file "bl3xhdr" $bl3xhdr
		cat $bl3xhdr $bl3xbin > $bl3x
	fi
	echo "done"
}

##################################################################
trap cleanup EXIT

#parse args
if [ "$#" -eq 0 ];  then echo "arg num $# err"; usage; fi
while getopts "s:i:o:h" opt; do
	case $opt in
		s) readonly soc="$OPTARG" ;;
		i) readonly uboot="$OPTARG" ;;
		o) readonly outdir="$OPTARG" ;;
		h) usage ;;
		\?)
			echo "Invalid option: $OPTARG" >&2 ; usage ;;
	esac
done

if [ $soc == "g12a" ] || [ $soc == "g12b" ] || [ $soc == "tl1" ] || [ $soc == "tm2" ]; then
	bl2bin_size=61440
	bl30hdr_size=4096
	if [ $soc == "tm2" ]; then
		bl30bin_size=62464
	fi
elif [ $soc == "txlx" ] || [ $soc == "gxl" ]; then
	bl2bin_size=45056
	bl30hdr_size=656
else
	echo "invalid soc <$soc>"
	echo "only support gxl, txlx, g12a, g12b, tl1, tm2"
	exit 1
fi
bl2size=$(expr $bl2hdr_size + $bl2bin_size)
bl30size=$(expr $bl30hdr_size + $bl30bin_size)

if [ ! -d $TMP ]; then mkdir $TMP; fi
if [ ! -d $TMP ]; then exit 1; fi
if [ ! -d $outdir ]; then mkdir $outdir; fi
if [ ! -d $outdir ]; then exit 1; fi

generate_bl2 "$uboot" "$uboot_no_bl2" "$outdir/$bl2"

generate_fip "$uboot_no_bl2" "$fip"
parse_fip "$fip" "$fip_info"

if [ $ddrfw_count -gt "0" ]; then
	#echo "ddrfw_count $ddrfw_count"
	i=1
	while [ $i -le $ddrfw_count ]
	do
		ddrfw=$outdir/${ddrfw[$(($i-1))]}
		ddrhdr=${ddrhdr[$(($i-1))]}
		ddrbin=${ddrbin[$(($i-1))]}
		ddroffset=`eval echo '$'ddr"$i"_entry_offset`
		ddrsize=`eval echo '$'ddr"$i"_entry_size`
		#echo $ddrfw $ddrhdr $ddrbin $ddroffset $ddrsize
		generate_ddrfw "$uboot_no_bl2" "$ddrfw" "$ddrhdr" "$ddrbin" "$ddroffset" "$ddrsize"
		((i++))
	done
fi

generate_bl3x "$uboot_no_bl2" "$outdir/$bl30" ""         "$bl30_bin" "$bl30_entry_offset" "$bl30_entry_size"
generate_bl3x "$uboot_no_bl2" "$outdir/$bl31" "$bl31_hdr" "$bl31_bin" "$bl31_entry_offset" "$bl31_entry_size"
if [[ "$bl32_entry_size" != 0 ]]; then
	generate_bl3x "$uboot_no_bl2" "$outdir/$bl32" "$bl32_hdr" "$bl32_bin" "$bl32_entry_offset" "$bl32_entry_size"
fi
generate_bl3x "$uboot_no_bl2" "$outdir/$bl33" ""         "$bl33_bin" "$bl33_entry_offset" "$bl33_entry_size"

echo "Parse uboot done"

