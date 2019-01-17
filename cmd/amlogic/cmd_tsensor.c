#include <common.h>
#include <command.h>
#include <asm/arch/tsensor.h>

static int do_read_temp(cmd_tbl_t *cmdtp, int flag1, int argc, char * const argv[])
{
	printf("read temp\n");
	return temp_read_entry();
}

static int do_temp_triming(cmd_tbl_t *cmdtp, int flag1, int argc, char * const argv[])
{
	int ret = -1;
	int ver, temp_base;

	if (argc <3) {
		printf("too little args for temp triming!!\n");
		return CMD_RET_USAGE;
	}

	temp_base = simple_strtoul(argv[1], NULL, 10);
	printf("set base temperature: %d\n", temp_base);

	ver = simple_strtoul(argv[2], NULL, 16);
	ret = temp_trim_entry(temp_base, ver);
	if (!ret)
		printf("temperature triming error %x\n", ret);
	return ret;
}

static int do_boot_cooling(cmd_tbl_t *cmdtp, int flag1, int argc, char * const argv[])
{
	return temp_cooling_entry();
}

U_BOOT_CMD(
	boot_cooling,	5,	0,	do_boot_cooling,
	"cpu temp-system",
	"boot_cooling pos"
);

U_BOOT_CMD(
	read_temp,	5,	0,	do_read_temp,
	"cpu temp-system",
	"read_temp pos"
);

static char temp_trim_help_text[] =
	"temp_triming x [ver]\n"
	"  - for manual trimming chip\n"
	"  - x:     [decimal]the temperature of the chip surface\n"
	"  - [ver]: [decimal]only for New thermal sensor\n"
	"           BBT: OPS socket board, which can change chips\n"
	"	    online: reference boards witch chip mounted\n"
	"	AXG or TXHD:\n"
	"           5  (0101)b: BBT, thermal0\n"
	"           6  (0110)b: BBT, thermal1\n"
	"           7  (0111)b: BBT, thermal01\n"
	"           d  (1101)b: online, thermal0\n"
	"           e  (1110)b: online, thermal1\n"
	"           f  (1111)b: online, thermal01\n"
	" 	G12A or G12B:\n"
	"	    88	(10001000)b: BBT-SW, thermal1 thermal2, valid thermal cali data\n"
	"	    89	(10001001)b: BBT-OPS, thermal1 thermal2, valid thermal cali data\n"
	"	    8b	(10001001)b: SLT, thermal1 thermal2, valid thermal cali data\n"
	" 	TL1:\n"
	"	    8c	(10001001)b: BBT-SW, thermal1 ~ 3, valid thermal cali data\n"
	"	    8d	(10001001)b: BBT-OPS, thermal1 ~ 3, valid thermal cali data\n"
	"	    8f	(10001001)b: SLT, thermal1 ~ 3, valid thermal cali data\n";

U_BOOT_CMD(
	temp_triming,	5,	1,	do_temp_triming,
	"cpu temp-system",
	temp_trim_help_text
);

