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
    ram (rw) : ORIGIN = configMEM_START, LENGTH = configMEM_LEN
}


SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

    .text :
    {
#if N200_REVA
	KEEP (*(SORT_NONE(.init)))
#else
	*(.vtable)
#endif
        . = ALIGN(4);
        *(.text)
        *(.rodata*)
        _etext = .;
    } > ram

    .data :
    {
        _data = .;
        *(vtable)
        *(.data)
        _edata = .;
    } > ram

    .bss :
    {
        _bss = .;
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        . = ALIGN(4);
        _ebss = .;
    } > ram

/*    .stack ORIGIN(ram) + LENGTH(ram) - __stack_size :*/
    .stack :
    {
        PROVIDE( _heap_end = . );
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
