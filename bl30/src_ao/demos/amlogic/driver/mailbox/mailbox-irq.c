#include <stdlib.h>
#include <stdint.h>
#include "myprintf.h"

#include "mailbox-irq.h"
#include "mailbox.h"

#define AOTAG "AOCPU"
#define PRINT_DBG(...)	//printf(__VA_ARGS__)
#define PRINT_ERR(...)	printf(__VA_ARGS__)
#define NO_DISABLE_IRQ	1

#define aml_writel32(val, reg)		(REG32(reg) = val)
#define aml_readl32(reg)		(REG32(reg))

xHandlerTableEntry xMbHandlerTable[IRQ_MAX];

void vMbDefaultHandler(void *vArg)
{
	vArg = vArg;
	return;
}

void vSetMbIrqHandler(unsigned int xNum, vHandlerFunc vHandler, void *vArg,
		    unsigned int xPriority)
{
	PRINT_DBG("vSetMbIrqHandler xNum:%d\n", xNum);
	if (vHandler == NULL) {
		xMbHandlerTable[xNum].vHandler = &vMbDefaultHandler;
		xMbHandlerTable[xNum].vArg = NULL;
	} else {
		xMbHandlerTable[xNum].vHandler = vHandler;
		xMbHandlerTable[xNum].vArg = vArg;
	}
	xMbHandlerTable[xNum].xPriority = xPriority;
}

void vMbSetIrqPriority(unsigned int xNum, unsigned int xPriority)
{
	xMbHandlerTable[xNum].xPriority = xPriority;
}

void vEnableMbInterrupt(unsigned int xMask)
{
#ifndef NO_DISABLE_IRQ
	unsigned int val = 0;

	val = aml_readl32(MAILBOX_IRQ_MASK) | xMask;
	aml_writel32(val, MAILBOX_IRQ_MASK);
#else
	xMask = xMask;
#endif
}

void vDisableMbInterrupt(unsigned int xMask)
{
#ifndef NO_DISABLE_IRQ
	unsigned int val = 0;

	val = aml_readl32(MAILBOX_IRQ_MASK) & (~xMask);
	aml_writel32(val, MAILBOX_IRQ_MASK);
#else
	xMask = xMask;
#endif
}

void vClrMbInterrupt(uint64_t xMask)
{
#ifdef IRQ_MORE
        uint32_t lval = 0, hval = 0;

        lval = xMask & 0xffffffff;
        hval = (xMask >> 32) & 0xffffffff;
        aml_writel32(lval, MAILBOX_IRQ_CLR);
        aml_writel32(hval, MAILBOX_IRQ_CLR1);
#else
        aml_writel32(xMask, MAILBOX_IRQ_CLR);
#endif
}

uint64_t xGetMbIrqStats(void)
{
#ifdef IRQ_MORE
        uint64_t val = 0;
        uint32_t lval = 0, hval = 0;

        lval = aml_readl32(MAILBOX_IRQ_STS);
        hval = aml_readl32(MAILBOX_IRQ_STS1);
        val = hval;
        val = ((val & 0xffffffff) << 32) | lval;
        return val;
#else
        return aml_readl32(MAILBOX_IRQ_STS);
#endif
}
