/* qbusLib.h - BTM23-402 QBUS library header */

#ifndef __INCqbusLibh
#define __INCqbusLibh

/* 
modification history
--------------------
    12jul01,sok  remove pmcSlot field from QBUS_DESC
    24may01,sok  QBUS_IV_IRQ1 and QBUS_IV_IRQ2 changed
    22may01,sok  add brackets to qbusReg parameter in QBUS_REG macro
    22may01,sok  add QBUS_MBOX definition
*/

/* memory description on pci */

#define	QBUS_BUS_BASE		0x000000	/* qbus on pci */
#define	QBUS_BUS_SIZE		(1 << 19)	/* 512K on pci */
#define	QBUS_DPRAM_BASE		0x800000	/* dual port ram on pci */
#define	QBUS_DPRAM_SIZE		(1 << 15)	/* 32K on pci */
#define	QBUS_REGS_BASE		0xc00000	/* registers on pci */

/* registers descriptions */

#define	QBUS_REG_INT_VEC	0x000		/* interrupt vector */
#define	QBUS_REG_INT_SRC	0x040		/* interrupt source */
#define	QBUS_REG_INT_ENA	0x080		/* interrupt mask */
#define	QBUS_VECTOR_RAM		0x0c0		/* interrupt vectors ram */
#define	QBUS_REG_SCIO		0x100		/* signals */
#define	QBUS_REG_CTRL		0x140		/* control */
#define	QBUS_DELAY_RAM		0x180		/* qbus master delays ram */
#define	QBUS_REG_TASK_COUNT	0x1c0		/* tasks count */
#define	QBUS_REG_TASK_ADRS	0x200		/* tasks starting address */
#define	QBUS_REG_SLAVE_BASE	0x300		/* qbus slave address */
#define	QBUS_REG_SLAVE_IRQ1	0x340		/* qbus irq1 address */
#define	QBUS_REG_SLAVE_IRQ2	0x380		/* qbus irq2 address */
#define	QBUS_REG_SLAVE_IRQ	0x3c0		/* qbus irq request */

/* control register bits */

#define	QBUS_CTRL_SYNC0		0x00000000	/* sync 0 tackts */
#define	QBUS_CTRL_SYNC1		0x00004000	/* sync 1 tackt */
#define	QBUS_CTRL_SYNC2		0x00008000	/* sync 2 tackts */
#define	QBUS_CTRL_SYNC3		0x0000c000	/* sync 3 tackts */
#define	QBUS_CTRL_CONFIG_ON	0x00002000	/* config access enable */
#define	QBUS_CTRL_MASTER_ON	0x00001000	/* master enable */
#define	QBUS_CTRL_SLAVE_ON	0x00000800	/* slave enable */
#define	QBUS_CTRL_EXT_MASTER_ON	0x00000400	/* other module */
#define	QBUS_CTRL_RESET		0x00000200	/* reset */
#define	QBUS_CTRL_BUSY		0x00000100	/* qbus busy */
#define	QBUS_CTRL_ERR_GRANT	0x00000080	/* qbus grant error */
#define	QBUS_CTRL_ERR_TASK	0x00000040	/* qbus task error */
#define	QBUS_CTRL_ERR_VECTOR	0x00000020	/* qbus vector error */
#define	QBUS_CTRL_ERR_WORD	0x00000010	/* qbus word error */
#define	QBUS_CTRL_ERR		0x000000f0	/* qbus error mask */
#define	QBUS_CTRL_CYCLE_GRANT	0x00000008	/* qbus grant flag */
#define	QBUS_CTRL_CYCLE_TASK	0x00000004	/* qbus task flag  */
#define	QBUS_CTRL_CYCLE_VECTOR	0x00000002	/* qbus vector read */
#define	QBUS_CTRL_CYCLE_WORD	0x00000001	/* qbus word access */

/* delay ram */

#define	QBUS_TIMINGS_NUMBER	16	/* number of master delay timings */

/* task parametrs */

#define	QBUS_TASK_ERROR		0x8000	/* task failed flag */
#define	QBUS_TASK_GO		0x4000	/* execute task flag */
#define	QBUS_Q2D_TASK		0x2000	/* read from qbus write to dpram */
#define	QBUS_D2Q_TASK		0x0000	/* read from dpram write to qbus */
#define	QBUS_TASK_WORD_MASK	0x1fff	/* failed word number mask */

/* task counts register */

#define	QBUS_TASK_READY		0x00000400	/* tasks done flag */
#define	QBUS_TASK_BREAK		0x00000200	/* task break */

/* slave base register */

#define	QBUS_SLAVE_WIN_2K	0	/* 2K window size */
#define	QBUS_SLAVE_WIN_4K	1	/* 4K window size */
#define	QBUS_SLAVE_WIN_8K	2	/* 8K window size */
#define	QBUS_SLAVE_WIN_16K	3	/* 16K window size */

/* slave irq register */

#define	QBUS_SLAVE_IRQ1		0x20000000	/* slave irq1 request */
#define	QBUS_SLAVE_IRQ2		0x80000000	/* slave irq2 request */

/* scio register */

#define	QBUS_SCI0	0x00010000
#define	QBUS_SCI1	0x00020000
#define	QBUS_SCI2	0x00040000
#define	QBUS_SCI3	0x00080000
#define	QBUS_SCI4	0x00100000
#define	QBUS_SCI5	0x00200000
#define	QBUS_SCI6	0x00400000
#define	QBUS_SCI7	0x00800000
#define	QBUS_SCI_MASK	0x00ff0000
#define	QBUS_SCO0	0x01000000
#define	QBUS_SCO1	0x02000000
#define	QBUS_SCO2	0x04000000
#define	QBUS_SCO3	0x08000000
#define	QBUS_SCO4	0x10000000
#define	QBUS_SCO5	0x20000000
#define	QBUS_SCO6	0x40000000
#define	QBUS_SCO7	0x80000000
#define	QBUS_SCO_MASK	0xff000000

/* interrupt source and enable registers */

#define	QBUS_INT_ERROR	0x00000001
#define	QBUS_INT_IRQ2	0x00000002
#define	QBUS_INT_IRQ1	0x00000004
#define	QBUS_INT_TASK	0x00000008
#define	QBUS_INT_MBOX	0x00000010
#define	QBUS_INT_SCI0	0x00000020
#define	QBUS_INT_SCI1	0x00000040
#define	QBUS_INT_SCI2	0x00000080
#define	QBUS_INT_SCI3	0x00000100
#define	QBUS_INT_SCI4	0x00000200
#define	QBUS_INT_SCI5	0x00000400
#define	QBUS_INT_SCI6	0x00000800
#define	QBUS_INT_SCI7	0x00001000

#define	QBUS_INT_CFG	0x00008000	/* interrupt enable only */

/* vector ram description */

#define	QBUS_IV_ERROR	0
#define	QBUS_IV_TASK	3
#define	QBUS_IV_MBOX	4
#define	QBUS_IV_SCI0	5
#define	QBUS_IV_SCI1	6
#define	QBUS_IV_SCI2	7
#define	QBUS_IV_SCI3	8
#define	QBUS_IV_SCI4	9
#define	QBUS_IV_SCI5	10
#define	QBUS_IV_SCI6	11
#define	QBUS_IV_SCI7	12

/* slave qbus irq */

#define	QBUS_IV_IRQ1	2
#define	QBUS_IV_IRQ2	1

/* QBUS_REG_INT_SRC are zero, happen if SCIx active too short time */

#define	QBUS_IV_UNKNOWN	13

/* number of interrupt vectors */

#define	QBUS_IV_NUMBER	16

/* mailbox offset from dpram start */

#define	QBUS_MBOX	0x3ffe

/* qbus pci device and vendor id */

#define	QBUS_PCI_ID	0x04028888

/* address convertation */

#define	QBUS_LOCAL_TO_BUS(qbusId,localAdrs)				\
    (((localAdrs) - ((qbusId)->localBase + QBUS_BUS_BASE)) >> 1)

#define	QBUS_BUS_TO_LOCAL(qbusId,qbusAdrs)				\
    (((qbusAdrs) << 1) + (qbusId)->localBase + QBUS_BUS_BASE)

#define	QBUS_LOCAL_TO_DPRAM(qbusId,localAdrs)				\
    (((localAdrs) - ((qbusId)->localBase + QBUS_DPRAM_BASE)) >> 1)

#define	QBUS_DPRAM_TO_LOCAL(qbusId,dpramAdrs)				\
    ((((dpramAdrs) << 1) + (qbusId)->localBase + QBUS_DPRAM_BASE))

/* get address of qbus register */

#define	QBUS_REG(qbusId,qbusReg)					\
    ((volatile int*)((qbusId)->localBase + QBUS_REGS_BASE + (qbusReg)))

/* defines for rw parameter in function qbusMemProbe() */

#define	QBUS_READ	0
#define	QBUS_WRITE	1

/* debug print */

#define	QBUS_DEBUG(level,msg)	\
{				\
    if(qbusDebug >= (level))	\
	logMsg msg;		\
}

/* interrupt service descriptor */

typedef struct {
    VOIDFUNCPTR	rtn;	/* interrupt handler */
    int		arg;	/* handler argument */
} QBUS_HANDLER;

#if OS_VERSION == 206
  #include <inttypes.h>
#endif

/* generic qbus driver description */

typedef struct {
    uintptr_t	localBase;	/* base memory address (local) */
    unsigned	slaveWinSize;	/* size of slave window (0 - 3) */
    unsigned	slaveQbusBase;	/* slave window in dpram (qbus address) */
    unsigned	slaveDpramBase;	/* slave window in dpram (dpram address) */
    unsigned*	tblBase;	/* dma table local base address */
    unsigned	taskCount;	/* dma task initialized count */
    QBUS_HANDLER intrDesc[QBUS_IV_NUMBER]; /* array of interrupt handlers */
} QBUS_DESC;

typedef QBUS_DESC* QBUS_ID;

/* globals */

extern int qbusDebug;		/* debug level */

/* function declarations */

STATUS qbusLocalToQbusAdrs(QBUS_ID qbusId, uintptr_t localAdrs,
    unsigned* pQbusAdrs);
STATUS qbusQbusToLocalAdrs(QBUS_ID qbusId, unsigned qbusAdrs,
    uintptr_t* pLocalAdrs);
STATUS qbusLocalToDpramAdrs(QBUS_ID qbusId, uintptr_t localAdrs,
    unsigned* pDpramAdrs);
STATUS qbusDpramToLocalAdrs(QBUS_ID qbusId, unsigned dpramAdrs,
    uintptr_t* pLocalAdrs);
STATUS qbusSlaveWindowSet(QBUS_ID qbusId, unsigned qbusWinBase,
    unsigned winSize);
STATUS qbusXferStart(QBUS_ID qbusId, unsigned taskTableDpramAdrs,
    unsigned taskCount);
STATUS qbusIntConnect(QBUS_ID qbusId, int iv, VOIDFUNCPTR rtn, int arg);
volatile int* qbusReg(QBUS_ID qbusId, int regOffset);
void qbusIntr(QBUS_ID qbusId);
void qbusTimingsSet(QBUS_ID qbusId, int timings[QBUS_TIMINGS_NUMBER]);
QBUS_ID qbusCtrlInit(int pmcSlot);
QBUS_ID qbusCtrlFind(int ix);
STATUS qbusMemProbe(QBUS_ID qbusId, volatile unsigned* adrs, int rw, 
    volatile unsigned* pVal);
void qbusDmaSuspend(QBUS_ID qbusId, unsigned* savedCount, int* savedMask);
void qbusDmaResume(QBUS_ID qbusId, unsigned savedCount, int savedMask);
int qbusLibVersion(int silence);
void qbusReset(QBUS_ID qbusId);
void qbusSCOSet(QBUS_ID qbusId, int line);
void qbusSCOClear(QBUS_ID qbusId, int line);
int qbusSCIState(QBUS_ID qbusId, int line);

#endif /* __INCqbusLibh */

