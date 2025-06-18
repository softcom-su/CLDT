/* lptBaget.h - BTM23-405 and BT01-212 LPT controller driver header */

#ifndef	__INClptBageth
#define	__INClptBageth

#include <lptDrv.h>

/* LPT configuration register 0x40 fields */

#define	C_SPP		0x0001	/* controller mode is SPP */
#define	C_EPP		0x0002	/* controller mode is EPP */
#define	C_ECP		0x0003	/* controller mode is ECP */
#define	C_PCI_TOUT	0x0004	/* PCI timeout enable: 16 clock rule */
#define	C_RST		0x0008	/* soft reset */
#define	C_RFIFO_12	0x0010	/* read FIFO interrupt threshold 12 bytes */
#define	C_RFIFO_8	0x0020	/* read FIFO interrupt threshold 8 bytes */
#define	C_RFIFO_NONE	0x0030	/* read FIFO interrupt threshold disable */
#define	C_WFIFO_12	0x0040	/* write FIFO interrupt threshold 12 bytes */
#define	C_WFIFO_8	0x0080	/* write FIFO interrupt threshold 8 bytes */
#define	C_WFIFO_NONE	0x00c0	/* write FIFO interrupt threshold disable */
#define	C_EPP_DELAY_1	0x0100	/* EPP cycle delay is 1 PCI clock */
#define	C_EPP_DELAY_2	0x0200	/* EPP cycle delay is 2 PCI clock */
#define	C_EPP_DELAY_3	0x0300	/* EPP cycle delay is 3 PCI clock */
#define	C_EPP_17	0x0400	/* EPP 1.7 mode enable */
#define	C_ISA_DELAY_1	0x1000	/* take 250 ns delay after port access */
#define	C_ISA_DELAY_2	0x2000	/* take 750 ns delay after port access */
#define	C_ISA_DELAY_3	0x3000	/* take 1000 ns delay after port access */

/* DMA limits */

#define	DMA_QUANT	16	/* size should be aligned to this quant */
#define	DMA_LIMIT	0xfff0	/* maximum transfer size */

/* BTM23-405 DMA table cell */

typedef struct {
      uint32_t	dmaBase;	/* PCI base address of data */
      uint32_t	dmaSize;	/* size and end-of-table bit */
} DMA_CELL;

#if	(OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL))

/* BTM23-405 LPT controller descriptor */

struct lpt_baget_ctrl {
    LPT_CTRL	lpt;		/* generic controller header */
    int		bus;		/* pci bus */
    int		dev;		/* pci device */
    int		fun;		/* pci function */
    int		id;		/* device and vendor id's */
    sem_t	syncSem;	/* semaphore for synchronize with interrupt */
    DMA_CELL*	table;		/* dma table */
    unsigned char ecr;		/* saved in interrupt ECR */
};

#endif	/* (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL)) */

typedef struct lpt_baget_ctrl LPT_BAGET_CTRL;

/* function declarations */

LPT_CTRL* lptBagetCtrlInit(int ctrl, int share, unsigned mode, int ecpMode);
int lptBagetDmaCopy(LPT_BAGET_CTRL* lptBaget, char* buf, int nBytes,
    int rw, int timeout);
int lptBagetModeSet(LPT_BAGET_CTRL* lptBaget, unsigned mode);
void lptBagetIntr(LPT_BAGET_CTRL* lptBaget);

#endif	/* __INClptBageth */
