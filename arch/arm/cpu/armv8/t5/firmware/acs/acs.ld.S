
OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(acs_entry)

SECTIONS
{
    . = 0xFFFC3000;

    . : {
        __RO_START__ = .;
        *acs_entry.o(.text*)
        *(.text*)
        *(.data*)
		*(.ddr_settings*)
    }

    //ASSERT(. <= 0xFFFC4000, "timing image has exceeded its limit.")
}
