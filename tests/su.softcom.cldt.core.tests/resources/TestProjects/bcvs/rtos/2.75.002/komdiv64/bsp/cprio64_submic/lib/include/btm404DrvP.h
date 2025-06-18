/* btm404DrvP.h - private BTM404 (ARINC-429) header */

#ifndef	__INCbtm404DrvPh
#define	__INCbtm404DrvPh

#include <pciLib.h>

/* print debug message macro */

#define	ARINC_DEBUG	\
    if(arincDebug)	\
	logMsg

/* pci device and vendor id */

#define	BTM404_PCI_ID	0x04048888

/* register main control bits */

#define	MAIN_CTRL_TEST		0x80000000

#define	MAIN_CTRL_SHIFT_TX_SYNC	0
#define	MAIN_CTRL_SHIFT_RX_SYNC	4
#define	MAIN_CTRL_SHIFT_TF	8

/* register receiver control bits */

#define	RX_CTRL_SYNC0		0x00
#define	RX_CTRL_SYNC1		0x01
#define	RX_CTRL_SYNC2		0x02
#define	RX_CTRL_SYNC3		0x03
#define	RX_CTRL_SYNC_MASK	0x03
#define	RX_CTRL_SPD_12K		0x04
#define	RX_CTRL_SPD_50K		0x08
#define	RX_CTRL_SPD_100K	0x0c
#define	RX_CTRL_SPD_MASK	0x0c
#define	RX_CTRL_PAR_NONE	0x10
#define	RX_CTRL_PAR_EVEN	0x20
#define	RX_CTRL_PAR_ODD		0x30
#define	RX_CTRL_PAR_MASK	0x30
#define	RX_CTRL_SMOD		0x40
#define	RX_CTRL_USE		0x80

/* register receiver status bits */

#define	RX_STATUS_PTR_MASK	0x0ff
#define	RX_STATUS_ERR_MASK	0xf00

#define	RX_STATUS_SHIFT_ERR	8

/* register transmitter control bits */

#define	TX_CTRL_PAR_NONE	0x1
#define	TX_CTRL_PAR_EVEN	0x2
#define	TX_CTRL_PAR_ODD		0x3
#define	TX_CTRL_PAR_MASK	0x3
#define	TX_CTRL_SPD_12K		0x4
#define	TX_CTRL_SPD_50K		0x8
#define	TX_CTRL_SPD_100K	0xc
#define	TX_CTRL_SPD_MASK	0xc

/* register transmitter dma control bits */

#define	TX_DMA_CTRL_BANK	0x1
#define	TX_DMA_CTRL_CYCLIC	0x2
#define	TX_DMA_CTRL_PAUSE	0x4
#define	TX_DMA_CTRL_START	0x8

/* register transmitter dma status bits */

#define	TX_DMA_STATUS_PTR_MASK	0x0ff
#define	TX_DMA_STATUS_DONE	0x100

/* register interrupt 1 bits */

#define	INTR1_GR_SYNC		0x00000001	/* group sync irq pended */
#define	INTR1_GR_TX_DMA		0x00000002	/* group tx dma irq pended */
#define	INTR1_GR_RX_GET		0x00000004	/* group rx get irq pended */
#define	INTR1_GR_RX_ERR		0x00000008	/* group rx err irq pended */
#define	INTR1_M_GR_SYNC		0x00010000	/* group sync irq mask */
#define	INTR1_M_GR_TX_DMA	0x00020000	/* group tx dma irq mask */
#define	INTR1_M_GR_RX_GET	0x00040000	/* group rx get irq mask */
#define	INTR1_M_GR_RX_ERR	0x00080000	/* group rx err irq mask */

#define	INTR1_SHIFT_SYNC	4
#define	INTR1_SHIFT_TX_DMA	8
#define	INTR1_SHIFT_M_SYNC	20
#define	INTR1_SHIFT_M_TX_DMA	24

/* register interrupt 2 bits */

#define	INTR1_SHIFT_RX_GET	0
#define	INTR1_SHIFT_RX_ERR	16

/* btm404 registers map */

typedef struct {
    volatile unsigned mainCtrl;			/* 0x000 - 0x004 */
    volatile unsigned pad_0x004_0x100[63];	/* 0x004 - 0x100 */
    volatile unsigned intr1;			/* 0x100 - 0x104  */
    volatile unsigned intr2;			/* 0x104 - 0x108  */
    volatile unsigned pad_0x108_0x200[62];	/* 0x108 - 0x200 */
    volatile unsigned rxLabel[16];		/* 0x200 - 0x240 */
    volatile unsigned pad_0x240_0x300[48];	/* 0x240 - 0x300 */
    volatile unsigned rxCtrl[16];		/* 0x300 - 0x340 */
    volatile unsigned pad_0x340_0x400[48];	/* 0x340 - 0x400 */
    volatile unsigned txCtrl[8];		/* 0x400 - 0x420 */
    volatile unsigned pad_0x420_0x500[56];	/* 0x420 - 0x500 */
    volatile unsigned txDmaLength[8];		/* 0x500 - 0x520 */
    volatile unsigned pad_0x520_0x600[56];	/* 0x520 - 0x600 */
    volatile unsigned txDmaCtrl[8];		/* 0x600 - 0x620 */
    volatile unsigned pad_0x620_0x700[56];	/* 0x620 - 0x700 */
    volatile unsigned rxStatus[16];		/* 0x700 - 0x740 */
    volatile unsigned pad_0x740_0x800[48];	/* 0x740 - 0x800 */
    volatile unsigned txStatus;			/* 0x800 - 0x804 */
    volatile unsigned pad_0x804_0x900[63];	/* 0x804 - 0x900 */
    volatile unsigned txDmaStatus[8];		/* 0x900 - 0x920 */
} BTM404_REG_MAP;

/* btm404 tx/rx buffer type */

typedef volatile unsigned BTM404_BUF [256];

/* btm404 memory map */

typedef struct {
    BTM404_BUF rxLabelBuf[16];		/* 0x0000 - 0x4000 */
    BTM404_BUF rxCyclicBuf[16];		/* 0x4000 - 0x8000 */
    BTM404_BUF txLoHiBuf[8][2];		/* 0x8000 - 0xc000 */
} BTM404_MEM_MAP;

/* types of descriptors */

typedef enum {
    TX, RX, CTRL			/* transmitter, receiver, controller */
} BTM404_DESC_TYPE;

/* receiver descriptor */

typedef struct btm404RxDesc {
    BTM404_DESC_TYPE	type;		/* RX */
    struct btm404Desc*	pCtrlDesc;	/* ptr to controller descriptor */
    unsigned		channel;	/* rx channel number */
    unsigned		readBuf;	/* RX_LABEL or RX_CYCLIC */
} BTM404_RX_DESC;

/* transmitter descriptor */

typedef struct btm404TxDesc {
    BTM404_DESC_TYPE	type;		/* TX */
    struct btm404Desc*	pCtrlDesc;	/* ptr to controller descriptor */
    unsigned		channel;	/* tx channel number */
    unsigned		bank;		/* current tx bank (0 or 1) */
} BTM404_TX_DESC;

/* user interrupt handler */

typedef struct {
    void (* rtn)();			/* handler routine address */
    int		arg;			/* first argument to routine */
} BTM404_INTR_HANDLER;
 
/* btm404 descriptor */

typedef struct btm404Desc {
    BTM404_DESC_TYPE	type;		/* CTRL */
    PCI_DEV		pciDev;		/* ptr to pci config space */
    BTM404_MEM_MAP*	memMap;		/* ptr to memory map */
    BTM404_REG_MAP*	regMap;		/* ptr to registers map */
    int			tfType;		/* transmitter chips type */
    int			ctrl;		/* controller number */
    BTM404_TX_DESC*	txDesc[8];	/* tx descriptors */
    BTM404_RX_DESC*	rxDesc[16];	/* rx descriptors */
    unsigned		rxLabel[16];	/* rx labels readable array */
    BTM404_INTR_HANDLER	handler;	/* user interrupt handler */
} BTM404_DESC;

#endif	/* __INCbtm404DrvPh */
