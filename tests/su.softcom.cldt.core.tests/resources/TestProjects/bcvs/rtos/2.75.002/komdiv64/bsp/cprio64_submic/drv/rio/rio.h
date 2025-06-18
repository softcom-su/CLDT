#ifndef __ASSEMBLER__
#ifndef	__INCrioh__
#define	__INCrioh__

#include <system.h>

/* rio host type */

#define RIO_HOST_INVALID        (-1)
#define RIO_HOST_UNKNOWN        0
#define RIO_HOST_K128           1
#define RIO_HOST_K64            2

/* device vendor ID */

#define RIO_VENDOR_ID_NIISI     0x0074

/* device ID */

#define RIO_DEVICE_ID_K128      0x0128
#define RIO_DEVICE_ID_K64RIO    0x8717


/* crossboard types */

#define	RIO_CROSS_UNKNOWN	(-1)
#define	RIO_CROSS_RPKB		1
#define	RIO_CROSS_NIISI2S	2
#define	RIO_CROSS_NIISI1S	3
#define	RIO_CROSS_NIISI2S_V2	4
#define	RIO_CROSS_RPKB_RESERVE	5
#define	RIO_CROSS_RPKB_FULL	6
#define	RIO_CROSS_BT5328	7

/* crossboard configuration timeout in seconds */

#define	RIO_CONFIG_TIMEOUT	10

/* maximum number of ID on RapidIO */

#ifdef BT5328
#define	RIO_ID_MAX	3
#elif defined(MPON)
#define	RIO_ID_MAX	8
#else
#define	RIO_ID_MAX	255
#endif

/* irq numbers */

#define	IRQ_PORT_WR_DONE	31
#define	IRQ_DST_ID_ERR		30
#define	IRQ_TR_ERR		29
#define	IRQ_TIMER64		28
#define	IRQ_DMA1_DONE		27
#define	IRQ_DMA0_DONE		26
#define	IRQ_MAINT_OUT_DONE	24
#define	IRQ_TRAP3_DONE		23
#define	IRQ_MSG_IN_POL		22
#define	IRQ_TRAP1_DONE		21
#define	IRQ_MSG_OUT_POL		20
#define	IRQ_MSG_IN_OWN		19
#define	IRQ_MAN_IN_OVF		18
#define	IRQ_MAN_IN_DONE		17
#define	IRQ_MISC_DONE		16
#define	IRQ_DB_IN_ERR		15
#define	IRQ_DB_IN_TAIL		14
#define	IRQ_DB_IN_COMPLETE	13
#define	IRQ_DB_IN_DONE		12
#define	IRQ_DB_OUT_ERR		11
#define	IRQ_IN_DONE		10
#define	IRQ_DB_OUT_DONE		8
#define	IRQ_MSG_IN_ERR		7
#define	IRQ_MSG_IN_EOL		6
#define	IRQ_MSG_IN_COMPLETE	5
#define	IRQ_MSG_IN_DONE		4
#define	IRQ_MSG_OUT_ERR		3
#define	IRQ_MSG_OUT_EOL		2
#define	IRQ_MSG_OUT_COMPLETE	1
#define	IRQ_MSG_OUT_OWN		0

/* irq bits */

#define	INTR_PORT_WR_DONE	(1 << IRQ_PORT_WR_DONE)
#define	INTR_DST_ID_ERR		(1 << IRQ_DST_ID_ERR)
#define	INTR_TR_ERR		(1 << IRQ_TR_ERR)
#define	INTR_TIMER64		(1 << IRQ_TIMER64)
#define	INTR_DMA1_DONE		(1 << IRQ_DMA1_DONE)
#define	INTR_DMA0_DONE		(1 << IRQ_DMA0_DONE)
#define	INTR_MAINT_OUT_DONE	(1 << IRQ_MAINT_OUT_DONE)
#define	INTR_TRAP3_DONE		(1 << IRQ_TRAP3_DONE)
#define	INTR_MSG_IN_POL		(1 << IRQ_MSG_IN_POL)
#define	INTR_TRAP1_DONE		(1 << IRQ_TRAP1_DONE)
#define	INTR_MSG_OUT_POL	(1 << IRQ_MSG_OUT_POL)
#define	INTR_MSG_IN_OWN		(1 << IRQ_MSG_IN_OWN)
#define	INTR_MAN_IN_OVF		(1 << IRQ_MAN_IN_OVF)
#define	INTR_MAN_IN_DONE	(1 << IRQ_MAN_IN_DONE)
#define	INTR_MISC_DONE		(1 << IRQ_MISC_DONE)
#define	INTR_DB_IN_ERR		(1 << IRQ_DB_IN_ERR)
#define	INTR_DB_IN_TAIL		(1 << IRQ_DB_IN_TAIL)
#define	INTR_DB_IN_COMPLETE	(1 << IRQ_DB_IN_COMPLETE)
#define	INTR_DB_IN_DONE		(1 << IRQ_DB_IN_DONE)
#define	INTR_DB_OUT_ERR		(1 << IRQ_DB_OUT_ERR)
#define	INTR_IN_DONE		(1 << IRQ_IN_DONE)
#define	INTR_DB_OUT_DONE	(1 << IRQ_DB_OUT_DONE)
#define	INTR_MSG_IN_ERR		(1 << IRQ_MSG_IN_ERR)
#define	INTR_MSG_IN_EOL		(1 << IRQ_MSG_IN_EOL)
#define	INTR_MSG_IN_COMPLETE	(1 << IRQ_MSG_IN_COMPLETE)
#define	INTR_MSG_IN_DONE	(1 << IRQ_MSG_IN_DONE)
#define	INTR_MSG_OUT_ERR	(1 << IRQ_MSG_OUT_ERR)
#define	INTR_MSG_OUT_EOL	(1 << IRQ_MSG_OUT_EOL)
#define	INTR_MSG_OUT_COMPLETE	(1 << IRQ_MSG_OUT_COMPLETE)
#define	INTR_MSG_OUT_OWN	(1 << IRQ_MSG_OUT_OWN)

/* dma direction and mode */

#define	DMA_SRC_RIO	0
#define	DMA_SRC_PCI	1
#define	DMA_DST_RIO	0
#define	DMA_DST_PCI	1
#define	DMA_DST_SWRITE	4
#define	DMA_DST_NWRITE	0

/* CKI status bits */

#define	CKI_STAT_MEM_ERR	0x00000001
#define	CKI_STAT_PCI_ADDR_ERR	0x00000002
#define	CKI_STAT_WDO		0x00000004
#define	CKI_STAT_SERR		0x00000008
#define	CKI_STAT_PERR_RD	0x00000010
#define	CKI_STAT_PERR_WR	0x00000020
#define	CKI_STAT_GOOD		0x00000100
#define	CKI_STAT_FAIL		0x80000000

/* CKI mask bits */

#define	CKI_MASK_MEM_ERR	CKI_STAT_MEM_ERR
#define	CKI_MASK_PCI_ADDR_ERR	CKI_STAT_PCI_ADDR_ERR
#define	CKI_MASK_WDO		CKI_STAT_WDO
#define	CKI_MASK_SERR		CKI_STAT_SERR
#define	CKI_MASK_PERR		CKI_STAT_PERR_RD
#define	CKI_MASK_NMI		0x00000100
#define	CKI_MASK_FAIL		0x00000200
#define	CKI_MASK_RESET		0x00000400
#define	CKI_MASK_WDO_INT	0x00010000
#define	CKI_MASK_WDO_MODE	0x00020000
#define	CKI_MASK_TIMER_START	0x01000000
#define	CKI_MASK_TIMER_CYCLIC	0x02000000

/* globals */

extern int rioMaintTout;				// maintainance cycle timeout us
extern int rioDebug;					// debug level
extern unsigned rioDmaBound[256];			// aligned up dma bounds

/* function declarations */

int rioInitDone(void);
int rioIntEnable(unsigned mask);
int rioIntDisable(unsigned mask);
int rioIntConnect(unsigned irq, VOIDFUNCPTR rtn, int arg);
int rioMaintIn(unsigned hopCount, unsigned addr, unsigned dstID, unsigned* pData);
int rioMaintOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data);
int rioDbIntConnect(unsigned irq, VOIDFUNCPTR rtn, int arg);
int rioDbOut(unsigned dstID, unsigned i, unsigned data);
int _rioDbOut(unsigned dstID, unsigned i, unsigned data);
int rioDmaStart(unsigned ch, unsigned src, unsigned srcMode, unsigned srcID,
    unsigned dst, unsigned dstMode, unsigned dstID, unsigned nBytes);
int rioDmaWait(unsigned ch, unsigned poll, unsigned timeout);
void rioHopShow(unsigned hop, unsigned id, int errClear, unsigned maxId);
void* rioMasterBaseGet(void);
void* rioSlaveBaseGet(void);
void rioShow(int errClear);
void rioInit(int crossType);
int rioMSend(unsigned dstID, unsigned long long* buf, unsigned nQW, unsigned addr);
int rioMInit(unsigned prio);
unsigned boardCKIStatusGet(void);
void boardCKIStatusClear(unsigned stat);
void boardCKIEnable(unsigned val);
void boardCKIDisable(unsigned val);
int rioHostTypeGet(unsigned id);
void rioShutdown(void);

#endif	/* __INCrioh__ */
#endif
