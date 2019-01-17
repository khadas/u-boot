/*
 * Amlogic clkmsr command
 */
#include <common.h>
#include <command.h>
#include <asm/arch/oscring.h>

static int do_ringmsr(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int index = 0xff;

	if (argc < 2) {
		return CMD_RET_USAGE;
	}

	if (argc ==  2)
		index = simple_strtoul(argv[1], NULL, 16);
	ring_msr(index);
	return 0;
}

static char ringmsr_help_text[] =
	"ringmsr x\n"
	"  - for get chip ring info\n"
	"  - x:   \n"
	"  G12A/G12B/TL1:\n"
	"  0  : print all ring info and msr ring set voltage \n"
	"  1-7: ee9 ee1 ee0 cpu1 cpu0 iddee iddcpu\n"
	"  ff : print all ring info and msr ring no set voltage";

U_BOOT_CMD(
		ringmsr, 2, 1, do_ringmsr,
		"Amlogic measure ring",
		ringmsr_help_text
);
