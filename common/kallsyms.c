/*
 * Helper functions for working with the builtin symbol table
 *
 * Copyright (c) 2008-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <asm/sections.h>

DECLARE_GLOBAL_DATA_PTR;

/* We need the weak marking as this symbol is provided specially */
extern const char system_map[] __attribute__((weak));

/* Given an address, return a pointer to the symbol name and store
 * the base address in caddr.  So if the symbol map had an entry:
 *		03fb9b7c_spi_cs_deactivate
 * Then the following call:
 *		unsigned long base;
 *		const char *sym = symbol_lookup(0x03fb9b80, &base);
 * Would end up setting the variables like so:
 *		base = 0x03fb9b7c;
 *		sym = "_spi_cs_deactivate";
 */
const char *symbol_lookup(unsigned long addr, unsigned long *caddr, unsigned long *naddr)
{
	const char *sym, *csym;
	unsigned long sym_addr;
	char sym_addr_tmp[16] = {0};
	unsigned long text_start;
	unsigned long text_end;

	sym = system_map;
	csym = NULL;
	*caddr = 0;
	*naddr = 0;
	text_start = (ulong)&__image_copy_start - gd->reloc_off;
	text_end = (ulong)&__rodata_start - gd->reloc_off;

	/*
	 * within text section?
	 */
	if (addr < text_start || addr > text_end)
		return NULL;

	while (*sym) {
		memcpy(sym_addr_tmp, sym, 16);
		sym_addr = simple_strtoul(sym_addr_tmp, NULL, 16);
		sym = sym+16;
		if (sym_addr > addr) {
			*naddr = sym_addr;
			break;
		}
		*caddr = sym_addr;
		csym = sym;
		sym += strlen(sym) + 1;
	}
	/*
	 * the last symbol?
	 */
	if (*sym == 0x00)
		*naddr = text_end;

	return csym;
}
