/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

//is the uboot loaded from usb otg
int is_tpl_loaded_from_usb(void);

//is the uboot loaded from sdcard mmc 0
//note only sdmmc supported by romcode when external device boot
int is_tpl_loaded_from_ext_sdmmc(void);

//Check if uboot loaded from external sdmmc or usb otg
int aml_burn_check_uboot_loaded_for_burn(int flag);

int aml_burn_factory_producing(int flag, bd_t* bis);

//usb producing mode, if tpl loaded from usb pc tool and auto enter producing mode
int aml_try_factory_usb_burning(int flag, bd_t* bis);

//Auto enter sdcard burning if booted from sdcard and aml_sdc_burn.ini existed
int aml_try_factory_sdcard_burning(int flag, bd_t* bis);


