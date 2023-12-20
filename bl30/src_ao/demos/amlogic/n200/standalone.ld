/******************************************************************************
 *
 * standalone.ld - Linker script for applications using startup.c and
 *                 DriverLib.
 *
 * Copyright (c) 2005-2007 Luminary Micro, Inc.  All rights reserved.
 *
 * Software License Agreement
 *
 * Luminary Micro, Inc. (LMI) is supplying this software for use solely and
 * exclusively on LMI's microcontroller products.
 *
 * The software is owned by LMI and/or its suppliers, and is protected under
 * applicable copyright laws.  All rights are reserved.  Any use in violation
 * of the foregoing restrictions may subject the user to criminal sanctions
 * under applicable laws, as well as to civil liability for the breach of the
 * terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * This is part of revision 1049 of the Stellaris Driver Library.
 *
 *****************************************************************************/
#include "lscript.h"

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
              "elf32-littleriscv")
OUTPUT_ARCH("riscv")
ENTRY( _start )


MEMORY
{
#if defined (SOC_sc2) || defined (SOC_s4)
    local_ram (rw) : ORIGIN = 0x10000000, LENGTH = 0x1000000
#endif
    ram (rw) : ORIGIN = configMEM_START, LENGTH = configMEM_LEN
#if defined (SOC_t3)
    bss_ram (rw) : ORIGIN = 0x10000000, LENGTH = 0x1800
#endif

}


SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

#if defined (SOC_sc2) || defined (SOC_s4)
    __heap_size = DEFINED(__heap_size) ? __heap_size : 32K;
    .heap :
    {
        PROVIDE( _heap = . );
	. = ALIGN(1024);
        . += __heap_size;
        PROVIDE( _eheap = . );
    } > local_ram
#endif

    .text :
    {
        . = ALIGN(4);
	PROVIDE( _text = . );
	*(.init)
        *(.text* .rodata*)
        . = ALIGN(0x100);
#if defined (SOC_sc2) || defined (SOC_s4)
        . = ALIGN(0x4000);
#endif
        PROVIDE( _etext = . );
#if N200_REVA
	KEEP (*(SORT_NONE(.init)))
#else
	*(.vtable)
#endif
    } > ram

#if defined (SOC_t3)
    . = ALIGN(4);
    _data_img_start = .;
#endif
    .data :
    {
        _data = .;
        *(vtable)
#if defined (SOC_t3)
	. = ALIGN(4);
	_module_pm_begin = .;
	KEEP (*(SORT(.sdata.__initcall*)))
	_module_pm_end = .;
        *(.sdata* .data*)
        _edata = .;
    } > bss_ram AT > ram
#else
        *(.sdata* .data*)
        _edata = .;
    } > ram
#endif

    .bss :
    {
        _bss = .;
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        . = ALIGN(4);
        _ebss = .;
#if defined (SOC_t3)
    } > bss_ram
#else
    } > ram
#endif

/*    .stack ORIGIN(ram) + LENGTH(ram) - __stack_size :*/
    .stack :
    {
        PROVIDE( _stack = . );
	. = ALIGN(1024);
        . += __stack_size;
        PROVIDE( _sp = . );
    } >ram
#if N200_REVA
    .init :
    {
    	KEEP (*(SORT_NONE(.init)))
    } >ram
#endif
}
