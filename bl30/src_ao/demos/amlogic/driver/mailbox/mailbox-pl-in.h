#ifndef __MAIL_IN_H__
#define __MAIL_IN_H__

#include "mailbox.h"
#include "mailbox-api.h"

#define UNUSED(x)           (void)(x)

#define ACK_OK				1
#define ACK_FAIL			2
#define QUEUE_LENGTH		20
#define	TASK_POOL_SIZE		3
#if( configMAX_PRIORITIES < 2 )
	#error configMAX_PRIORITIES should no less than 3
#endif
#define MHU_MB_TASK_PRIORITIES	configMAX_PRIORITIES - 2

#define MAILBOX_BUFFER_SIZE             MHU_MAX_SIZE
#define MAILBOX_CMD_MAX                 0xFFFF

#define PRINT_DBG(...)			//printf(__VA_ARGS__)
#define aml_writel32(val, reg)		(REG32(reg) = val)
#define aml_readl32(reg)		(REG32(reg))

enum sync_type {
	MB_ASYNC = 1,
	MB_SYNC,
};

typedef struct {
	uint32_t cmd:16;
	uint32_t size:9;
	uint32_t sync:7;
} MbStat_t;

typedef union {
	MbStat_t st;
	uint32_t reg;
} MbStat_in_t;

typedef struct __attribute__((__packed__)) {
	uint32_t status;
	uint64_t taskid;
	uint64_t complete;
	uint64_t ullclt;
}mbHeadInfo;

typedef struct __attribute__((__packed__)) {
	uint32_t status;
	uint64_t taskid;
	uint64_t complete;
	uint64_t ullclt;
	uint8_t data[MHU_DATA_SIZE];
}mboxData;

typedef struct __attribute__((__packed__)) {
	mboxData mbdata;	// hack to pass more data
	uint32_t ulCmd;
	uint32_t ulSize;
	uint32_t ulChan;
}mbPackInfo;

#define VALID_CMD_SIZE(cmd, size) \
    configASSERT(cmd <= MAILBOX_CMD_MAX && size <= MAILBOX_BUFFER_SIZE)

#define VALID_CHANNEL(chan) \
    configASSERT(chan == AOREE_CHANNEL || chan == AOTEE_CHANNEL)

#define VALID_BUFFER_SIZE(size) \
    configASSERT(size <= MAILBOX_BUFFER_SIZE)


static inline MbStat_t xGetMboxStats(uint32_t reg)
{
	MbStat_in_t stat;

	stat.reg = aml_readl32(reg);
	return stat.st;
}

static inline void vSetMboxStats(uint32_t reg, MbStat_t st)
{
	MbStat_in_t stat;

	stat.st = st;
	aml_writel32(stat.reg, reg);
}

static inline void vClrMboxStats(uint32_t reg)
{
	aml_writel32(0xFFFFFFFF, reg);
}

static inline uint32_t xGetChan(uint32_t mbox)
{
	switch (mbox) {
	case MAILBOX_ARMREE2AO:
		return AOREE_CHANNEL;
	case MAILBOX_ARMTEE2AO:
		return AOTEE_CHANNEL;
	default:
		configASSERT(0);
	}
	return 0;
}

static inline uint32_t xGetRevMbox(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return MAILBOX_ARMREE2AO;
	case AOTEE_CHANNEL:
		return MAILBOX_ARMTEE2AO;
	default:
		configASSERT(0);
	}
	return 0;
}

static inline uint32_t xGetSendMbox(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return 0;
	case AOTEE_CHANNEL:
		return 0;
	default:
		configASSERT(0);
	}
	return 0;
}

static inline uint32_t *xRevAddr(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return (uint32_t *)MAILBOX_REE2AO;
	case AOTEE_CHANNEL:
		return (uint32_t *)MAILBOX_TEE2AO;
	default:
		configASSERT(0);
	}
	return NULL;
}

static inline uint32_t *xSendAddr(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return (uint32_t *)MAILBOX_AO2REE;
	case AOTEE_CHANNEL:
		return (uint32_t *)MAILBOX_AO2TEE;
	default:
		configASSERT(0);
	}
	return NULL;
}
/*send back for sync api*/
static inline uint32_t *xSendAddrBack(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return (uint32_t *)MAILBOX_REE2AO;
	case AOTEE_CHANNEL:
		return (uint32_t *)MAILBOX_TEE2AO;
	default:
		configASSERT(0);
	}
	return NULL;
}

static inline void *mbmemset(void *dst, int val, size_t count)
{
	char *ptr = dst;

	while (count--)
		*ptr++ = val;

	return dst;
}

static inline void *mbmemcpy(void *dst, const void *src, size_t len)
{
	const char *s = src;
	char *d = dst;

	while (len--)
		*d++ = *s++;

	return dst;
}

static inline void vGetPayload(void *addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("get payload: addr:0x%x, 0x%x\n", addr, size);
	if (data != NULL) {
		mbmemcpy(data, addr, size);
	}

}

static inline void vBuildPayload(void *addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("vBuildPayload: addr:0x%x\n", addr);
	if (data != NULL) {
		mbmemcpy(addr, data, size);
	}
}

static inline void vReBuildPayload(void *addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("vBuildPayload: addr:0x%x\n", addr);
	if (data != NULL) {
		mbmemset(addr, 0, MAILBOX_BUFFER_SIZE);
		mbmemcpy(addr, data, size);
	}
}
#endif
