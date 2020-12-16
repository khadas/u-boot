/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/gadget/v2_burning/v2_burning_i.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __V2_BURNING_I_H__
#define __V2_BURNING_I_H__

#include <config.h>
#include <common.h>
#include <environment.h>
#include <asm/string.h>
#include <asm-generic/errno.h>
#include <asm/byteorder.h>
#include <malloc.h>
#include <u-boot/sha1.h>

#include <amlogic/aml_v2_burning.h>
//#include <asm/arch/reboot.h>
#include <asm/arch/romboot.h>
//#include <amlogic/aml_lcd.h>
#include <amlogic/storage_if.h>
#include "v2_common/sparse_format.h"
#include "v2_common/optimus_download.h"
#include "v2_common/amlImage_if.h"
#include "v2_common/optimus_progress_ui.h"

extern int cli_simple_parse_line(char *line, char *argv[]);

#endif//ifndef __V2_BURNING_I_H__

