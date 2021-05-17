/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/aml_hdmirx.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _AML_HDMIRX_H
#define _AML_HDMIRX_H

void hdmirx_hw_init(unsigned int port_map,
		unsigned char *pedid_data,
		int edid_size);
void rx_set_phy_rterm(void);

#endif /* _AML_HDMIRX_H */
