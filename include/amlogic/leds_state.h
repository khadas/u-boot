/*
 * include/amlogic/leds_state.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

 /*
  *
 * Copyright (C) 2012 Amlogic, Inc.
 *
 * Author: jiayi.zhou@amlogic.com
 *
 */
#ifndef _LEDS_STATE_H_
#define _LEDS_STATE_H_

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/secure_apb.h>

#define	MAXIMUM_BLINK_NUM	15
#define MAXIMUM_HIGH_MS		50*63
#define MAXIMUM_LOW_MS		50*63
#define MAXIMUM_BREATH_ID       3


enum LedState {
	LED_STATE_DEFAULT = 0,
	LED_STATE_BRIGHTNESS,
	LED_STATE_BREATH,
	LED_STATE_BLINK_ON,
	LED_STATE_BLINK_OFF,
	LED_STATE_BLINK_BREATH,
	/* */
	LED_STATE_BLINK_ON_HANDLE,
	LED_STATE_BLINK_OFF_HANDLE,
	LED_STATE_BLINK_BREATH_HANDLE,
#if 0
	/* TODO: */
	LED_STATE_SET_MAX_BRIGHTNESS,
	LED_STATE_SET_PINMUX,
	LED_STATE_CLEAR_PINMUX,
	LED_STATE_INFO,
	LED_STATE_DEBUG,
#endif
	LED_STATE_INVALID,
};

enum led_brightness {
	LED_OFF = 0,
	LED_HALF = 127,
	LED_FULL = 255,
};

uint32_t meson_ledstate_set_brightness(uint32_t led_id, uint32_t brightness);
uint32_t meson_ledstate_set_breath(uint32_t led_id, uint32_t breath_id);

/*to do:Parameters five and six are reserved parameters*/
uint32_t meson_ledstate_set_blink_times_on(uint32_t led_id, uint32_t
		blink_times, uint32_t blink_high, uint32_t blink_low,
		uint32_t brightness_high, uint32_t brightness_low);
uint32_t meson_ledstate_set_blink_times_off(uint32_t led_id,
		uint32_t blink_times,uint32_t blink_high,uint32_t blink_low,
		uint32_t brightness_high, uint32_t brightness_low);

#endif
/*to do*/
#if 0
uint32_t set_led_blink_times_breath(uint32_t led_id,uint32_t blink_times,
		uint32_t blink_high,uint32_t blink_low );
#endif
