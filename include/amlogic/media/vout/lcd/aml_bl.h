/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef INC_AML_LCD_BL_H
#define INC_AML_LCD_BL_H

#include <amlogic/media/vout/lcd/lcd_vout.h>

#define BLPR(fmt, args...)     printf("bl: "fmt"", ## args)
#define BLERR(fmt, args...)    printf("bl: error: "fmt"", ## args)

enum bl_ctrl_method_e {
	BL_CTRL_GPIO = 0,
	BL_CTRL_PWM,
	BL_CTRL_PWM_COMBO,
	BL_CTRL_LOCAL_DIMMING,
	BL_CTRL_EXTERN,
	BL_CTRL_MAX,
};

enum bl_pwm_method_e {
	BL_PWM_NEGATIVE = 0,
	BL_PWM_POSITIVE,
	BL_PWM_METHOD_MAX,
};

enum bl_pwm_port_e {
	BL_PWM_A = 0,
	BL_PWM_B,
	BL_PWM_C,
	BL_PWM_D,
	BL_PWM_E,
	BL_PWM_F,
	BL_PWM_G,
	BL_PWM_H,
	BL_PWM_I,
	BL_PWM_J,
	BL_PWM_AO_A = 0x50,
	BL_PWM_AO_B,
	BL_PWM_AO_C,
	BL_PWM_AO_D,
	BL_PWM_AO_E,
	BL_PWM_AO_F,
	BL_PWM_AO_G,
	BL_PWM_AO_H,
	BL_PWM_VS = 0xa0,
	BL_PWM_MAX = 0xff,
};

enum bl_off_policy_e {
	BL_OFF_POLICY_NONE = 0,
	BL_OFF_POLICY_ALWAYS,
	BL_OFF_POLICY_ONCE,
	BL_OFF_POLICY_MAX,
};

#define XTAL_FREQ_HZ		(24*1000*1000) /* 24M in HZ */
#define XTAL_HALF_FREQ_HZ	(24*1000*500)  /* 24M/2 in HZ */

#define BL_FREQ_DEFAULT		1000 /* unit: HZ */
#define BL_FREQ_VS_DEFAULT	2    /* multiple 2 of vfreq */

#define BL_LEVEL_MAX		255
#define BL_LEVEL_MIN		10
#define BL_LEVEL_OFF		1

#define BL_LEVEL_MID		128
#define BL_LEVEL_MID_MAPPED	BL_LEVEL_MID //102
#define BL_LEVEL_DEFAULT	BL_LEVEL_MID

#define BL_GPIO_NUM_MAX		5
#define BL_INDEX_INVALID        0xff

struct bl_pwm_config_s {
	unsigned int index;
	enum bl_pwm_method_e pwm_method;
	enum bl_pwm_port_e pwm_port;
	unsigned int level_max;
	unsigned int level_min;
	unsigned int pwm_freq; /* pwm_vs: 1~4(vfreq), pwm: freq(unit: Hz) */
	unsigned int pwm_duty; /* unit: % */
	unsigned int pwm_duty_max; /* unit: % */
	unsigned int pwm_duty_min; /* unit: % */
	unsigned int pwm_cnt; /* internal used for pwm control */
	unsigned int pwm_pre_div; /* internal used for pwm control */
	unsigned int pwm_max; /* internal used for pwm control */
	unsigned int pwm_min; /* internal used for pwm control */
	unsigned int pwm_level; /* internal used for pwm control */
	unsigned int pwm_hi; /* internal used for pwm control */
	unsigned int pwm_lo; /* internal used for pwm control */
	unsigned int pwm_gpio;
	unsigned int pwm_gpio_off;
	unsigned int pinmux_flag;
	unsigned int pinmux_set[LCD_PINMUX_NUM][2];
	unsigned int pinmux_clr[LCD_PINMUX_NUM][2];
};

#define BL_NAME_MAX    30
struct bl_config_s {
	unsigned int index;
	char name[BL_NAME_MAX];
	int level_default;
	int level_min;
	int level_max;
	int level_mid;
	int level_mid_mapping;

	enum bl_ctrl_method_e method;
	unsigned int en_gpio;
	unsigned int en_gpio_on;
	unsigned int en_gpio_off;
	unsigned short power_on_delay;
	unsigned short power_off_delay;
	unsigned int dim_max;
	unsigned int dim_min;
	unsigned int en_sequence_reverse;

	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *bl_pwm_combo0;
	struct bl_pwm_config_s *bl_pwm_combo1;
	unsigned int pwm_on_delay;
	unsigned int pwm_off_delay;

	char gpio_name[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX];
	struct lcd_pinmux_ctrl_s *bl_pinmux;
	unsigned int pinmux_set[10][2];
	unsigned int pinmux_clr[10][2];
	int extern_index;
};

struct aml_bl_drv_s {
	unsigned int index;
	unsigned int key_valid;
	unsigned int state;
	unsigned int level;
	int bl_off_policy;
	int factory_bl_on_delay;

	struct bl_config_s config;
	struct aml_lcd_data_s *data;
};

struct aml_bl_drv_s *aml_bl_get_driver(int index);
void bl_driver_enable(int index);
void bl_driver_disable(int index);

#endif
