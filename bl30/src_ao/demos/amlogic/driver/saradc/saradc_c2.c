/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * saradc driver
 */
#include "FreeRTOS.h"
#include "util.h"
#include "projdefs.h"
#include "portmacro.h"
#include "semphr.h"
#include "task.h"
#include "saradc.h"

#include <unistd.h>
#include "n200_func.h"
#include "common.h"

#define SARADC_DRV_NAME				"saradc"

#define SARADC_MAX_FIFO_SIZE			16

#define SAR_CLK_DIV_MASK			GENMASK(7, 0)
#define SAR_CLK_DIV_SHIFT			(0)
#define SAR_CLK_GATE				BIT(8)
#define SAR_CLK_MUX_MASK			GENMASK(10, 9)

#define P_SARADC(x)					(SARADC_BASE + (x))

#define SARADC_REG0					0x00
#define SARADC_REG0_BUSY_MASK			GENMASK(30, 28)
#define SARADC_REG0_DELTA_BUSY                 BIT(30)
#define SARADC_REG0_AVG_BUSY			BIT(29)
#define SARADC_REG0_SAMPLE_BUSY		BIT(28)
#define SARADC_REG0_FIFO_FULL			BIT(27)
#define SARADC_REG0_FIFO_EMPTY			BIT(26)
#define SARADC_REG0_FIFO_COUNT_SHIFT		(21)
#define SARADC_REG0_FIFO_COUNT_MASK		GENMASK(25, 21)
#define SARADC_REG0_CURR_CHAN_ID_MASK		GENMASK(18, 16)
#define SARADC_REG0_SAMPLING_STOP		BIT(14)
#define SARADC_REG0_FIFO_CNT_IRQ_MASK          GENMASK(8, 4)
#define SARADC_REG0_FIFO_CNT_IRQ_SHIFT		(4)
#define SARADC_REG0_FIFO_IRQ_EN                BIT(3)
#define SARADC_REG0_SAMPLING_START             BIT(2)
#define SARADC_REG0_CONTINUOUS_EN              BIT(1)
#define SARADC_REG0_SAMPLE_ENGINE_ENABLE       BIT(0)

#define SARADC_CHAN_LIST				0x04
#define SARADC_CHAN_LIST_MAX_INDEX_SHIFT	(24)
#define SARADC_CHAN_LIST_MAX_INDEX_MASK	GENMASK(26, 24)
#define SARADC_CHAN_LIST_ENTRY_SHIFT(_chan)	(_chan * 3)
#define SARADC_CHAN_LIST_ENTRY_MASK(_chan)	\
					(GENMASK(2, 0) << ((_chan) * 3))

#define SARADC_AVG_CNTL				0x08
#define SARADC_AVG_CNTL_AVG_MODE_SHIFT(_chan)	\
					(16 + ((_chan) * 2))
#define SARADC_AVG_CNTL_AVG_MODE_MASK(_chan)	\
					(GENMASK(17, 16) << ((_chan) * 2))
#define SARADC_AVG_CNTL_NUM_SAMPLES_SHIFT(_chan)  \
					(0 + ((_chan) * 2))
#define SARADC_AVG_CNTL_NUM_SAMPLES_MASK(_chan)  \
					(GENMASK(1, 0) << ((_chan) * 2))

#define SARADC_REG3					0x0c
#define SARADC_REG3_CTRL_CONT_RING_COUNTER_EN	BIT(27)
#define SARADC_REG3_CTRL_SAMPLING_CLOCK_PHASE	BIT(26)
#define SARADC_REG3_ADC_EN			BIT(21)
#define SARADC_REG3_BLOCK_DLY_SEL_MASK		GENMASK(9, 8)
#define SARADC_REG3_BLOCK_DLY_MASK		GENMASK(7, 0)

#define SARADC_DELAY					0x10
#define SARADC_DELAY_INPUT_DLY_SEL_MASK	GENMASK(25, 24)
#define SARADC_DELAY_INPUT_DLY_CNT_MASK	GENMASK(23, 16)
#define SARADC_DELAY_SAMPLE_DLY_SEL_MASK	GENMASK(9, 8)
#define SARADC_DELAY_SAMPLE_DLY_CNT_MASK	GENMASK(7, 0)

#define SARADC_LAST_RD					0x14

#define SARADC_FIFO_RD					0x18
#define SARADC_FIFO_RD_CHAN_ID_SHIFT		(22)
#define SARADC_FIFO_RD_CHAN_ID_MASK		GENMASK(24, 22)
#define SARADC_FIFO_RD_SAMPLE_VALUE_MASK	GENMASK(21, 0)

#define SARADC_AUX_SW					0x1c
#define SARADC_AUX_SW_MUX_SEL_CHAN_MASK(_chan)		\
					(GENMASK(10, 8) << (((_chan) - 2) * 3))

#define SARADC_CHAN_10_SW				0x20
#define SARADC_CHAN_10_SW_CHAN1_MUX_SEL_MASK	GENMASK(25, 23)
#define SARADC_CHAN_10_SW_CHAN0_MUX_SEL_MASK	GENMASK(9, 7)

#define SARADC_DETECT_IDLE_SW				0x24
#define SARADC_DETECT_IDLE_SW_DETECT_SW_EN	BIT(26)
#define SARADC_DETECT_IDLE_SW_DETECT_MUX_MASK	GENMASK(25, 23)
#define SARADC_DETECT_IDLE_SW_IDLE_MUX_SEL_MASK GENMASK(9, 7)

#define SARADC_DELTA_10				0x28

#define SARADC_REG13				0x34
#define SARADC_C2_REG13_VBG_SEL			BIT(16)
#define SARADC_C2_REG13_EN_VCM0P9		BIT(1)

#define SARADC_REG14				0x38

#define SARADC_C2_CH0_CTRL1			0x4c
#define SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_MASK	GENMASK(23, 21)
#define SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_SHIFT	(21)
#define SARADC_C2_CH0_CTRL1_AUX_DIFF_EN		BIT(17)
#define SARADC_C2_CH0_CTRL1_AUX_MODE_SEL	BIT(0)

#define SARADC_C2_CH0_CTRL2			0x50
#define SARADC_C2_CH0_CTRL3			0x54

#define SARADC_C2_DISCARD_DATA_CNT		30
#define SARADC_C2_SAVE_DATA_CNT			1

static void vAdcHandlerISR(void);

static uint32_t SaradcRegBackup[SARADC_REG_NUM] = {0};

const char *const ch7Vol[] = {
	"gnd",
	"vdd/4",
	"vdd/2",
	"vdd*3/4",
	"vdd",
};

static SemaphoreHandle_t adcSemaphoreMutex;
static SemaphoreHandle_t adcSemaphoreBinary;

static void vBackupSaradcReg(void)
{
	uint8_t ucIndex;
	for (ucIndex = 0; ucIndex < (SARADC_REG_NUM - 1); ucIndex++) {
		SaradcRegBackup[ucIndex] =
			REG32((unsigned long)P_SARADC(SARADC_REG0)
						+ 0x04 * ucIndex);
	}

	/* saradc clock reg */
	SaradcRegBackup[SARADC_REG_NUM - 1] = REG32((unsigned long)SAR_CLK_BASE);
}

static void vRestoreSaradcReg(void)
{
	uint8_t ucIndex;
	for (ucIndex = 0; ucIndex < (SARADC_REG_NUM - 1); ucIndex++)
		REG32((unsigned long)(P_SARADC(SARADC_REG0) + 0x04 * ucIndex))
						= SaradcRegBackup[ucIndex];

	/* saradc clock reg */
	REG32((unsigned long)SAR_CLK_BASE) = SaradcRegBackup[SARADC_REG_NUM - 1];
}

void vAdcInit(void)
{
	char i;

	vBackupSaradcReg();

	/* create association between logic and physical channel */
	REG32(P_SARADC(SARADC_AUX_SW)) = 0x03eb1a0c;
	REG32(P_SARADC(SARADC_CHAN_10_SW)) = 0x008c000c;

	/* disable all channels by default */
	REG32(P_SARADC(SARADC_CHAN_LIST)) = 0x00000000;

	/* delay between two input/samples = (10 + 1) * 1us */
	REG32(P_SARADC(SARADC_DELAY)) = 0x010a000a;

	/*
	 * BIT[21]:    disable the ADC by default
	 * BIT[23-25]: vdda*3/4 connect to channel-7 by default
	 * BIT[26]:    select the sampling clock period: 0:3T, 1:5T
	 * BIT[27]:    disable ring counter
	 * BIT[27]:    use the clk domain to add delay to
	 *             the start convert signal for hold times
	 */
	REG32(P_SARADC(SARADC_REG3)) = 0x8980000a;

	/* select VDDA as reference voltage */
	REG32_UPDATE_BITS(P_SARADC(SARADC_REG13),
			  SARADC_C2_REG13_VBG_SEL | SARADC_C2_REG13_EN_VCM0P9,
			  SARADC_C2_REG13_VBG_SEL | SARADC_C2_REG13_EN_VCM0P9);
	REG32_UPDATE_BITS(P_SARADC(SARADC_REG14),
			  SARADC_C2_REG13_VBG_SEL | SARADC_C2_REG13_EN_VCM0P9,
			  SARADC_C2_REG13_VBG_SEL | SARADC_C2_REG13_EN_VCM0P9);

	/* disable diff, enable normal mode */
	for (i = 0; i < SARADC_CH_MAX; i++) {
		REG32_UPDATE_BITS(P_SARADC(SARADC_C2_CH0_CTRL1 + i * 12),
				  SARADC_C2_REG13_VBG_SEL |
				  SARADC_C2_REG13_EN_VCM0P9,
				  SARADC_C2_REG13_VBG_SEL |
				  SARADC_C2_REG13_EN_VCM0P9);
		REG32_UPDATE_BITS(P_SARADC(SARADC_C2_CH0_CTRL1 + i * 12),
				  SARADC_C2_CH0_CTRL1_AUX_DIFF_EN |
				  SARADC_C2_CH0_CTRL1_AUX_MODE_SEL,
				  SARADC_C2_CH0_CTRL1_AUX_MODE_SEL);
	}

	/* clock initialization: 1.2M=24M/(0x13 + 1) */
	REG32_UPDATE_BITS(SAR_CLK_BASE, SAR_CLK_DIV_MASK,
			  0x13 << SAR_CLK_DIV_SHIFT);

	/* interrupt initialization */
	RegisterIrq(SARADC_INTERRUPT_NUM, 1, vAdcHandlerISR);
	EnableIrq(SARADC_INTERRUPT_NUM);

	/* create mutex semaphore */
	adcSemaphoreMutex = xSemaphoreCreateMutex();
	configASSERT(adcSemaphoreMutex != NULL);

	/* create binary semaphore */
	adcSemaphoreBinary = xSemaphoreCreateBinary();
	configASSERT(adcSemaphoreBinary != NULL);
}

void vAdcDeinit(void)
{
	DisableIrq(SARADC_INTERRUPT_NUM);
	UnRegisterIrq(SARADC_INTERRUPT_NUM);

	vSemaphoreDelete(adcSemaphoreMutex);
	vSemaphoreDelete(adcSemaphoreBinary);

	vRestoreSaradcReg();
}

void vAdcHwEnable(void)
{
	xSemaphoreTake(adcSemaphoreMutex, portMAX_DELAY);

	REG32_UPDATE_BITS(SAR_CLK_BASE, SAR_CLK_GATE, SAR_CLK_GATE);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_SAMPLE_ENGINE_ENABLE,
			  SARADC_REG0_SAMPLE_ENGINE_ENABLE);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG3),
			  SARADC_REG3_ADC_EN, SARADC_REG3_ADC_EN);

	xSemaphoreGive(adcSemaphoreMutex);
}

void vAdcHwDisable(void)
{
	xSemaphoreTake(adcSemaphoreMutex, portMAX_DELAY);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG3), SARADC_REG3_ADC_EN, 0);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_SAMPLE_ENGINE_ENABLE, 0);

	REG32_UPDATE_BITS(SAR_CLK_BASE, SAR_CLK_GATE, 0);

	xSemaphoreGive(adcSemaphoreMutex);
}

static inline void prvAdcClearFifo(void)
{
	uint16_t i;

	for (i = 0; i < 32; i++) {
		if (!((REG32(P_SARADC(SARADC_REG0)) >>
		       SARADC_REG0_FIFO_COUNT_SHIFT) & 0x1f))
			break;

		REG32(P_SARADC(SARADC_FIFO_RD));
	}
}

static inline void prvAdcEnableChannel(enum AdcChannelType ch, uint8_t idx)
{
	REG32_UPDATE_BITS(P_SARADC(SARADC_CHAN_LIST),
			  SARADC_CHAN_LIST_MAX_INDEX_MASK,
			  idx << SARADC_CHAN_LIST_MAX_INDEX_SHIFT);

	REG32_UPDATE_BITS(P_SARADC(SARADC_CHAN_LIST),
			  SARADC_CHAN_LIST_ENTRY_MASK(idx),
			  ch << SARADC_CHAN_LIST_ENTRY_SHIFT(idx));

	REG32(P_SARADC(SARADC_C2_CH0_CTRL2 + ch * 12)) =
						SARADC_C2_DISCARD_DATA_CNT;
	REG32(P_SARADC(SARADC_C2_CH0_CTRL3 + ch * 12)) =
						SARADC_C2_SAVE_DATA_CNT;
}

static inline void prvAdcStartSample(void)
{
	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_FIFO_IRQ_EN |
			  SARADC_REG0_SAMPLING_STOP, SARADC_REG0_FIFO_IRQ_EN);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_SAMPLING_START,
			  SARADC_REG0_SAMPLING_START);
}

static inline void prvAdcStopSample(void)
{
	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0), SARADC_REG0_SAMPLING_START, 0);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_SAMPLING_STOP |
			  SARADC_REG0_FIFO_IRQ_EN, SARADC_REG0_SAMPLING_STOP);
}

static inline void prvAdcSetAvgMode(enum AdcChannelType ch,
				    enum AdcAvgMode mode)
{
	if (mode == MEDIAN_AVERAGING)
		REG32_UPDATE_BITS(P_SARADC(SARADC_AVG_CNTL),
				  SARADC_AVG_CNTL_NUM_SAMPLES_MASK(ch),
				  EIGHT_SAMPLES <<
				  SARADC_AVG_CNTL_NUM_SAMPLES_SHIFT(ch));
	else if (mode == MEAN_AVERAGING)
		REG32_UPDATE_BITS(P_SARADC(SARADC_AVG_CNTL),
				  SARADC_AVG_CNTL_NUM_SAMPLES_MASK(ch),
				  FOUR_SAMPLES <<
				  SARADC_AVG_CNTL_NUM_SAMPLES_SHIFT(ch));

	REG32_UPDATE_BITS(P_SARADC(SARADC_AVG_CNTL),
			  SARADC_AVG_CNTL_AVG_MODE_MASK(ch),
			  mode << SARADC_AVG_CNTL_AVG_MODE_SHIFT(ch));
}

static inline void prvAdcUpdateIntThresh(uint8_t thresh)
{
	configASSERT(thresh <= SARADC_MAX_FIFO_SIZE);

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_CONTINUOUS_EN,
			  ((thresh == 1) ? 0 : SARADC_REG0_CONTINUOUS_EN));

	REG32_UPDATE_BITS(P_SARADC(SARADC_REG0),
			  SARADC_REG0_FIFO_CNT_IRQ_MASK,
			  thresh << SARADC_REG0_FIFO_CNT_IRQ_SHIFT);
}

static int32_t prvAdcReadRawSample(uint16_t *data, uint16_t datNum,
				   AdcInstanceConfig_t *conf)
{
	uint8_t fifo_ch;
	uint16_t count = 0;
	uint32_t rVal;

	if (xSemaphoreTake(adcSemaphoreBinary, 100 / portTICK_PERIOD_MS) == pdTRUE) {	/* timeout 100ms */
		while ((REG32(P_SARADC(SARADC_REG0)) >>
			SARADC_REG0_FIFO_COUNT_SHIFT) & 0x1f) {
			if (count >= conf->number || count >= datNum
			    || count >= SARADC_MAX_FIFO_SIZE)
				break;

			rVal = REG32(P_SARADC(SARADC_FIFO_RD));

			fifo_ch = (rVal >> SARADC_FIFO_RD_CHAN_ID_SHIFT) & 0x7;

			if (fifo_ch != conf->channel)
				return -pdFREERTOS_ERRNO_EINVAL;

			data[count] = rVal & SARADC_FIFO_RD_SAMPLE_VALUE_MASK;

			count++;
		}
	} else {
		prvAdcStopSample();
		return -pdFREERTOS_ERRNO_ETIMEDOUT;
	}

	return count;
}

static void vAdcHandlerISR(void)
{
	BaseType_t reschedule = pdFALSE;

	/* stop sampling before reading the data */
	prvAdcStopSample();

	xSemaphoreGiveFromISR(adcSemaphoreBinary, &reschedule);

	portYIELD_FROM_ISR(reschedule);
}

int32_t xAdcGetSample(uint16_t *data, uint16_t datNum,
		      AdcInstanceConfig_t *conf)
{
	int32_t ret;

	xSemaphoreTake(adcSemaphoreMutex, portMAX_DELAY);

	prvAdcClearFifo();

	prvAdcEnableChannel(conf->channel, 0);

	prvAdcSetAvgMode(conf->channel, conf->avgMode);

	prvAdcUpdateIntThresh(conf->number);

	prvAdcStartSample();

	ret = prvAdcReadRawSample(data, datNum, conf);

	xSemaphoreGive(adcSemaphoreMutex);

	if (ret <= 0) {
		iprintf("%s: faild to read sample for channel %d: %d\n",
			SARADC_DRV_NAME, conf->channel, ret);
		return -pdFREERTOS_ERRNO_EINVAL;
	}

	return ret;
}

void vAdcSelfTest(void)
{
	int32_t ret;
	uint32_t i;
	uint16_t adcData[1];
	AdcInstanceConfig_t adcConfig = { SARADC_CH7, NO_AVERAGING, 1 };

	vAdcHwEnable();

	for (i = 0; i < ARRAY_SIZE(ch7Vol); i++) {
		REG32_UPDATE_BITS(P_SARADC(SARADC_C2_CH0_CTRL1 +
					   SARADC_CH7 * 12),
				  SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_MASK,
				  i << SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_SHIFT);

		ret = xAdcGetSample(adcData, 1, &adcConfig);
		if (ret <= 0)
			return;

		iprintf("%-8s: %d\n", ch7Vol[i], adcData[0]);
	}

	vAdcHwDisable();
}
