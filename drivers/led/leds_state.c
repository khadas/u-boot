/*
 *  drivers/amlogic/leds/leds_state.c
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
#include <common.h>
#include <asm/arch/mailbox.h>
#include "amlogic/leds_state.h"

uint32_t meson_ledstate_set_brightness(uint32_t led_id, uint32_t brightness)
{
	uint32_t data[3];

	if (brightness > LED_FULL) {
		printf("brightness is not within a reasonable range\n");
		return -EINVAL;
	}

	data[0] = led_id;
	data[1] = LED_STATE_BRIGHTNESS;
	data[2] = brightness;
	scpi_send_data(AOCPU_REE_CHANNEL, CMD_LED_INFO, data, sizeof(data), NULL, 0);

	return 0;
}

uint32_t meson_ledstate_set_breath(uint32_t led_id, uint32_t breath_id)
{
	uint32_t data[3];

	if (breath_id > MAXIMUM_BREATH_ID) {
		printf("brightness is not within a reasonable range\n");
		return -EINVAL;
	}

	data[0] = led_id;
	data[1] = LED_STATE_BREATH;
	data[2] = breath_id;
	scpi_send_data(AOCPU_REE_CHANNEL, CMD_LED_INFO, data, sizeof(data), NULL, 0);

	return 0;
}

/*to do:Five and six parameters are extended parameters*/
uint32_t meson_ledstate_set_blink_times_on(uint32_t led_id, uint32_t blink_times,
	uint32_t blink_high, uint32_t blink_low, uint32_t brightness_high, uint32_t brightness_low)
{
	uint32_t data[5];

	if ((blink_times > MAXIMUM_BLINK_NUM) || (blink_high > MAXIMUM_HIGH_MS) ||
	(blink_low > MAXIMUM_LOW_MS)) {
		printf("Parameter setting out of range\n");
		return -EINVAL;
	}

	brightness_high = brightness_low =0;
	data[0] = led_id;
	data[1] = LED_STATE_BLINK_ON;
	data[2] = blink_times;
	data[3] = blink_high;
	data[4] = blink_low;
	scpi_send_data(AOCPU_REE_CHANNEL, CMD_LED_INFO, data, sizeof(data), NULL, 0);

	return 0;
}

uint32_t meson_ledstate_set_blink_times_off(uint32_t led_id, uint32_t blink_times,
	uint32_t blink_high, uint32_t blink_low, uint32_t brightness_high, uint32_t brightness_low)
{
	uint32_t data[5];

	if ((blink_times > MAXIMUM_BLINK_NUM) || (blink_high > MAXIMUM_HIGH_MS) ||
			(blink_low > MAXIMUM_LOW_MS)) {
		printf("Parameter setting out of range\n");
		return -EINVAL;
	}

	brightness_high = brightness_low =0;
	data[0] = led_id;
	data[1] = LED_STATE_BLINK_OFF;
	data[2] = blink_times;
	data[3] = blink_high;
	data[4] = blink_low;
	scpi_send_data(AOCPU_REE_CHANNEL, CMD_LED_INFO, data, sizeof(data), NULL, 0);

	return 0;
}

/*to do*/
#if 0
uint32_t set_led_blink_times_breath(uint32_t led_id,
		uint32_t blink_times, uint32_t blink_high, uint32_t blink_low )
{
	void ret;

	if (blink_times>15 || blink_high%50 != 0 || blink_low%50 != 0) {
		printf("Parameter setting out of range\n");
		return -1;
	}

	uint32_t data[5];
	data[0]=led_id;
	data[1]=LED_STATE_BLINK_BREATH;
	data[2]=blink_times;
	data[3]=blink_high;
	data[4]=blink_low ;
	ret = scpi_send_data(AOCPU_REE_CHANNEL, 0xF7, data, 5*sizeof(data), NULL, 0);
	if (a != 0) {
		printf("communication failed\n");
		return -1;
	}

	return 0;
}
#endif

