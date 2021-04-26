/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/gxl_p281_v1/eth_setup.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <environment.h>
#include <fdt_support.h>
#include <libfdt.h>
#include <asm/arch/eth_setup.h>
/*
 *
 *setup eth device board socket
 *
 */
struct eth_board_socket* eth_board_setup(char *name){
	struct eth_board_socket* new_board;
	new_board= (struct eth_board_socket*) malloc(sizeof(struct eth_board_socket));
	if (NULL == new_board) return NULL;
	if (name != NULL) {
		new_board->name = (char*)malloc(strlen(name) + 2);
		strncpy(new_board->name, name, strlen(name));
	}else{
		new_board->name="gxb";
	}

	new_board->eth_pinmux_setup=NULL ;
	new_board->eth_clock_configure=NULL;
	new_board->eth_hw_reset=NULL;
	return new_board;
}
//pinmux   HHI_GCLK_MPEG1[bit 3]
//
