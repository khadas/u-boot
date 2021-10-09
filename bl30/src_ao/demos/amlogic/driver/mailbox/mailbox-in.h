#ifndef __MAIL_IN_H__
#define __MAIL_IN_H__

#include "myprintf.h"
#include "mailbox.h"
#include "mailbox-api.h"

#define UNUSED(x)           (void)(x)

#define ACK_OK				1
#define ACK_FAIL			2
#define MHU_MB_STK_SIZE		2048
#define QUEUE_LENGTH		20
#define	TASK_POOL_SIZE		3

#define MAILBOX_BUFFER_SIZE             MHU_MAX_SIZE
#define MAILBOX_CMD_MAX                 0xFFFF

#define PRINT_DBG(...)			//printf(__VA_ARGS__)
#define aml_writel32(val, reg)		(REG32(reg) = val)
#define aml_readl32(reg)		(REG32(reg))

#ifndef MAILBOX_DSPA2AO
#define MAILBOX_DSPA2AO 0xc
#endif
#ifndef MAILBOX_AO2DSPA
#define MAILBOX_AO2DSPA 0xd
#endif

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
    configASSERT(chan == AOREE_CHANNEL || chan == AOTEE_CHANNEL || chan == AODSPA_CHANNEL)

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
	case MAILBOX_DSPA2AO:
		return AODSPA_CHANNEL;
	default:
		configASSERT(0);
	}
}

static inline uint32_t xGetRevMbox(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
		return MAILBOX_ARMREE2AO;
	case AOTEE_CHANNEL:
		return MAILBOX_ARMTEE2AO;
	case AODSPA_CHANNEL:
		return MAILBOX_DSPA2AO;
	default:
		configASSERT(0);
	}
}

static inline uint32_t xGetSendMbox(uint32_t ulChan)
{
	switch (ulChan) {
	case AODSPA_CHANNEL:
		return MAILBOX_AO2DSPA;
	case AOREE_CHANNEL:
	case AOTEE_CHANNEL:
	default:
		configASSERT(0);
	}
	return 0;
}

static inline uint32_t xDspRevAddr(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
	case AOTEE_CHANNEL:
	case AODSPA_CHANNEL:
		return PAYLOAD_RD_BASE(xGetRevMbox(ulChan));
	default:
		configASSERT(0);
	}
}

static inline uint32_t xSendAddr(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
	case AOTEE_CHANNEL:
	case AODSPA_CHANNEL:
		return PAYLOAD_WR_BASE(xGetSendMbox(ulChan));
	default:
		configASSERT(0);
	}
}
/*send back for sync api*/
static inline uint32_t xSendAddrBack(uint32_t ulChan)
{
	switch (ulChan) {
	case AOREE_CHANNEL:
	case AOTEE_CHANNEL:
	case AODSPA_CHANNEL:
		return PAYLOAD_RD_BASE(xGetSendMbox(ulChan));
	default:
		configASSERT(0);
	}
}


static inline uint32_t xRevAddrMbox(uint32_t mbox)
{
	return PAYLOAD_RD_BASE(mbox);
}

static inline uint32_t xSendAddrMbox(uint32_t mbox)
{
	return PAYLOAD_WR_BASE(mbox);
}

static inline void MbWrite(uint32_t to, void *from, long count)
{
        int i = 0;
	int len = count / 4 + ((count % 4) ? 1 : 0);
	uint32_t *p = from;

        PRINT_DBG("vMbWrite Count: 0x%x, len: 0x%x\n", count, len);
        while (len > 0) {
		aml_writel32(p[i], to + (4 * i));
                PRINT_DBG("vMbwrite reg: 0x%x\n",  REG32(to + 4 * i + 0x800));
                len--;
                i++;
        }
}

static inline void MbRead(void *to, uint32_t from, long count)
{
	int i = 0;
	int len = count / 4 + ((count % 4) ? 1 : 0);
	uint32_t *p = to;

	PRINT_DBG("vMbRead Count:0x%x, len: 0x%x\n", count, len);
	while (len > 0) {
		p[i] = aml_readl32(from + (4 * i));
		PRINT_DBG("vMbread p: 0x%x\n",  p[i]);
		len--;
		i++;
	}
}

static inline void xGetPayloadHead(uint32_t addr, void *head)
{
	mbHeadInfo *phead = head;

	MbRead(&phead->status, addr, 0x4);
	MbRead(&phead->taskid, addr + 0x4, 0x8);
	MbRead(&phead->complete, addr + 0xc, 0x8);
	MbRead(&phead->ullclt, addr + 0x14, 0x8);
}
static inline void xBuildPayloadHead(uint32_t addr, void *head)
{
	mbHeadInfo *phead = head;

	MbWrite(addr, &phead->status,  0x4);
	MbWrite(addr + 0x4, &phead->taskid, 0x8);
	MbWrite(addr + 0xc, &phead->complete, 0x8);
	MbWrite(addr + 0x14, &phead->ullclt, 0x8);
}

static inline void vGetPayload(uint32_t addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("get payload: addr:0x%x\n", addr);
	if (data != NULL) {
		MbRead(data, addr, size);
	}

}

static inline void vBuildPayload(uint32_t addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("vBuildPayload: addr:0x%x\n", addr);
	if (data != NULL) {
		MbWrite(addr, data, size);
	}
}

static inline void vReBuildPayload(uint32_t addr, void *data, size_t size)
{
	VALID_BUFFER_SIZE(size);

	PRINT_DBG("vBuildPayload: addr:0x%x\n", addr);
	if (data != NULL) {
		MbWrite(addr, 0, MAILBOX_BUFFER_SIZE);
		MbWrite(addr, data, size);
	}
}
#endif
