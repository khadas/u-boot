
OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(acs_entry)

SECTIONS
{
	. = 0x00007000;
	. = ALIGN(4);
	.entry : {
		__RO_START__ = .;
		*acs_entry.o(.text*)
		*(.text*)
		*(.data*)
		*(.bss*)
    }

	. = 0x00000000;
	.dev_header :
	{
		*(.dev_header*)
	}

	. = 0x00000200;
	.generic_param :
	{
		*(.generic_param*)
	}

#if !defined(CONFIG_CHIP_NOCS)
	. = 0x00003800;
	.ddr_param :
	{
		*(.ddr_param*)
	}
#endif

	. = 0x00005800;
	.clk_param :
	{
		*(.clk_param*)
	}

	. = 0x00005c00;
	.misc_param :
	{
		*(.misc_param*)
	}

	. = 0x00005e00;
	.store_param :
	{
		*(.store_param*)
	}

#if defined(CONFIG_CHIP_NOCS)
	. = 0x00006000;
	.ddr_param :
	{
		*(.ddr_param*)
	}
#endif

#if defined(CONFIG_MDUMP_COMPRESS)
	. = 0x00006800;
	.ramdump_param :
	{
		*(.ramdump_param*)
	}
#endif

    ASSERT(. <= 0x00007000, "params image has exceeded its limit.")
}
