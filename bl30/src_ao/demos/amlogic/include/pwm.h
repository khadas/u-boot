#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif
#include <pwm_plat.h>

#define	MESON_PWM_0			0
#define	MESON_PWM_1			1
#define	MESON_PWM_2			2
#define	MESON_PWM_3			3

typedef struct xPwmMesondevice {
	xPwmMesonChip_t *chip;
	uint32_t hwpwm;
	void *chip_data;
	uint32_t pwm_hi;
	uint32_t pwm_lo;
	uint32_t pwm_pre_div;
} xPwmMesondevice_t;

/**
 * vPwmMesonPwmDebug() - Dump pwm register
 * @pwm: pwm channel
 */
extern void vPwmMesonPwmDebug(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonClear() - Clean pwm register
 * @pwm: pwm channel
 */
extern void vPwmMesonClear(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonSetPolarity() - Set pwm polarity
 * @pwm: pwm channel
 * @val: pwm polarity
 */
extern void vPwmMesonSetPolarity(xPwmMesondevice_t *pwm, uint32_t val);

/**
 * xPwmMesonIsBlinkComplete() - Check blink complete
 * @pwm: pwm channel
 *
 * Returns 0 on blink complete.
 */
extern int32_t xPwmMesonIsBlinkComplete(xPwmMesondevice_t *pwm);

/**
 * prvPwmConstantDisable() - Disable constant function
 * @pwm: pwm channel
 *
 */
extern void  vPwmConstantDisable(xPwmMesondevice_t *pwm);

/**
 * prvPwmConstantEnable() - Enable constant function
 * @pwm: pwm channel
 *
 */
extern void vPwmConstantEnable(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonBlinkEnable() - Enable blink function
 * @pwm: pwm channel
 */
extern void vPwmMesonBlinkEnable(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonBlinkDisable() - Disabled blink function
 * @pwm: pwm channel
 */
extern void vPwmMesonBlinkDisable(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonSetBlinkTimes() - Set blink times
 * @pwm: pwm channel
 * @times: blink times
 */
extern void vPwmMesonSetBlinkTimes(xPwmMesondevice_t *pwm, uint32_t times);

/**
 * vPwmMesonSetTimes() - Set times
 * @pwm: pwm channel
 * @times: times
 */
extern void vPwmMesonSetTimes(xPwmMesondevice_t *pwm, uint32_t times);

/**
 * vPwmMesonEnable() - Enabled a pwm channel
 * @pwm: pwm channel
 */
extern void vPwmMesonEnable(xPwmMesondevice_t *pwm);

/**
 * vPwmMesonDisable() - Disabled a pwm channel
 * @pwm: pwm channel
 */
extern void vPwmMesonDisable(xPwmMesondevice_t *pwm);

/**
 * xPwmMesonConfig() - Config a pwm channel
 * @pwm: pwm channel
 * @duty_ns: effective time (ns)
 * @period_ns: period (ns)
 *
 * Returns 0 on success, negative value on error.
 */
extern int32_t xPwmMesonConfig(xPwmMesondevice_t *pwm, uint32_t duty_ns, uint32_t period_ns);

/**
 * vPwmMesonChannelFree() - Free a pwm channel
 * @pwm: pwm channel
 */
extern void vPwmMesonChannelFree(xPwmMesondevice_t *pwm);

/**
 * xPwmMesonChannelApply() - Apply a pwm channel
 * @chip_id: pwm controller id
 * @channel_id: pwm channel id
 *
 * Returns a pwm channel.
 */
extern xPwmMesondevice_t *xPwmMesonChannelApply(uint32_t chip_id, uint32_t channel_id);

/**
 * vPwmMesonSetVoltage() - Set voltage
 * @voltage_id: voltage select
 * @voltage_mv: voltage
 */
extern int32_t vPwmMesonSetVoltage(uint32_t voltage_id, uint32_t voltage_mv);

/**
 * vPwmMesonGetVoltage() - Get voltage
 * @voltage_id: voltage select
 *
 * Returns voltage.
 */
extern int32_t vPwmMesonGetVoltage(uint32_t voltage_id);

#ifdef __cplusplus
}
#endif
#endif
