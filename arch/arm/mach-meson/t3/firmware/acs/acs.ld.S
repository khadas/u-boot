
OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(acs_entry)

SECTIONS
{
	. = 0x00001000;
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

	. = 0x00000100;
	.generic_param :
	{
		*(.generic_param*)
	}

	. = 0x0000200;
	.clk_param :
	{
		*(.clk_param*)
	}

	. = 0x0000300;
	.misc_param :
	{
		*(.misc_param*)
	}

	. = 0x0000500;
	.store_param :
	{
		*(.store_param*)
	}

	. = 0x0000600-0x40;//max 0x600-96
	.ddr_param :
	{
		*(.ddr_param*)
	}
	//. = 0x0000c00;//max 0x600-96
	//. = 0x0000d00;//max 0x600-96
	.ddr_2acs_index :
	{
		*(.ddr_2acs_index*)
	}
	.ddr_2acs_data :
	{
		*(.ddr_2acs_data*)
	}

    ASSERT(. <= 0x00001000, "params image has exceeded its limit.")
}
