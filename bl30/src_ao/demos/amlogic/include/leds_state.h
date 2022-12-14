#ifndef __LEDS_H
#define __LEDS_H

#include <pwm.h>
#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NAME				"[AOCPU Led]"

#define LED_PERIOD				5555
#define LED_TASK_TIME_MS			50
#define LED_PWM_CHAN_CNT			2
#define LED_ERROR_CNT				1

#define LED_MAX_BLINK_CNT			15
/* Limit on stick mem */
//#define LED_MAX_TIMES_CNT			255
#define LED_MAX_TIMES_CNT			0x3F
#define LED_MAX_HIGH_MS				(LED_TASK_TIME_MS * LED_MAX_TIMES_CNT)
#define LED_MAX_LOW_MS				(LED_TASK_TIME_MS * LED_MAX_TIMES_CNT)

#define STICK_LED_STATE_MASK			GENMASK(2, 0)	//3
#define STICK_LED_BRIGHTNESS_MASK		GENMASK(10, 3)
#define STICK_LED_BREATH_ID_MASK		GENMASK(12, 11)
#define STICK_LED_BLINK_TIMES_MASK		GENMASK(16, 13)
#define STICK_LED_BLINK_HIGH_MASK		GENMASK(22, 17)
#define STICK_LED_BLINK_LOW_MASK		GENMASK(28, 23)

enum StickMemLedIdx
{
	STICK_LED_STATE,
	STICK_LED_BRIGHTNESS,
	STICK_LED_BREATH_ID,
	STICK_LED_BLINK_TIMES,
	STICK_LED_BLINK_HIGH,
	STICK_LED_BLINK_LOW,
	STICK_LED_INVALID,
};

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

enum LedBrightness {
	LED_OFF = 0,
	LED_HALF = 127,
	LED_FULL = 255,
};

enum LedType {
	LED_TYPE_GPIO = 0,
	LED_TYPE_PWM,
};

enum LedPolarity {
	LED_POLARITY_POSITIVE = 0,
	LED_POLARITY_INVERT,
};

typedef struct LedCoord {
	uint32_t x;
	uint32_t y;
} LedCoord_t;

typedef struct LedDevice {
	const char	*name;
	uint32_t	curr_led_state;
	uint32_t	last_led_state;
	uint32_t	breathtime;
	uint32_t	id;
	uint32_t	hardware_id;
	uint32_t	brightness;
	uint32_t	polarity;
	xPwmMesondevice_t *pwm;
	xPwmMesondevice_t *pwm_sub;
	enum LedType	 type;
} LedDevice_t;

int32_t xLedsStateInit(void);
int32_t xLedsStateSetBreath(uint32_t id, uint32_t breath_id);
int32_t xLedsStateSetBrightness(uint32_t id, uint32_t brightness);
int32_t xLedsStateSetBlinkBreath(uint32_t id, uint32_t times, uint32_t high_ms, uint32_t low_ms, uint32_t high_br, uint32_t low_br);
int32_t xLedsStateSetBlinkOn(uint32_t id, uint32_t times, uint32_t high_ms, uint32_t low_ms, uint32_t high_br, uint32_t low_br);
int32_t xLedsStateSetBlinkOff(uint32_t id, uint32_t times, uint32_t high_ms, uint32_t low_ms, uint32_t high_br, uint32_t low_br);
#ifdef __cplusplus
}
#endif
#endif

