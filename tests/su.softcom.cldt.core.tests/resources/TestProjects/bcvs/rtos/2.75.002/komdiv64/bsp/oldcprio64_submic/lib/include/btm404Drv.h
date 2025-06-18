/* btm404Drv.h - BTM404 (ARINC-429) header */

#ifndef	__INCbtm404Drvh
#define	__INCbtm404Drvh

/* defines */

#define	TX_CHAN_NR	8	/* # of transmitters */
#define	RX_CHAN_NR	16	/* # of receivers */
#define	TX_SYNC_NR	4	/* # of tx sync signals */
#define	RX_SYNC_NR	4	/* # of rx sync signals */

#define	TF_TYPE1	1	/* transmitters type 1 */
#define	TF_TYPE2	2	/* transmitters type 2 */

/* ioctl commands */

#define	TX_IOCTL	0x80000000	/* tx ioctl flag */
#define	RX_IOCTL	0x40000000	/* rx ioctl flag */

/* transmitter ioctl commands */

#define	FIOTXDMACNTSET	(TX_IOCTL|0)	/* set tx dma length */
#define	FIOTXDMASTART	(TX_IOCTL|1)	/* start tx dma */
#define	FIOTXDMASTOP	(TX_IOCTL|2)	/* stop tx dma */
#define	FIOTXDMAPAUSE	(TX_IOCTL|3)	/* pause tx dma */
#define	FIOTXDMASTATGET	(TX_IOCTL|4)	/* get tx dma */
#define	FIOTXBANKSET	(TX_IOCTL|5)	/* set tx bank */
#define	FIOTXBANKGET	(TX_IOCTL|6)	/* get current tx bank */
#define	FIOTXCTRLSET	(TX_IOCTL|7)	/* set TX_CTRL register */
#define	FIOTXSPEEDSET	(TX_IOCTL|8)	/* set bits in MAIN_CTRL register */
#define	FIOTXINTUNMASK	(TX_IOCTL|9)	/* unmask tx done irq */
#define	FIOTXINTMASK	(TX_IOCTL|10)	/* mask tx done irq */

/* receiver ioctl commands */

#define	FIORXBUFSEL	(RX_IOCTL|0)	/* select label buf fo read */
#define	FIORXSTATUSGET	(RX_IOCTL|1)	/* get RX_STATUS register */
#define	FIORXSTART	(RX_IOCTL|2)	/* start rx dma */
#define	FIORXSTOP	(RX_IOCTL|3)	/* stop rx dma */

/* common ioctl commands */

#define	FIOINTCONNECT	0		/* connect interrupt handler */
#define	FIOINTMASK	1		/* mask specified interrupts */
#define	FIOINTUNMASK	2		/* unmask specified interrupts */
#define	FIODESCGET	3		/* get ptr to descriptor structure */
#define	FIORXLABELSET	4		/* set rx label options */
#define	FIOTXSYNCON	5		/* enable tx sync channel */
#define	FIOTXSYNCOFF	6		/* disable tx sync channel */

/* FIORXBUFSEL ioctl command arguments */

#define	RX_CYCLIC	0		/* read from rx cyclic buffer */
#define	RX_LABEL	1		/* read from rx label buffer */

/* FIOTXSPEEDSET ioctl command arguments */

#define	TX_SPD_12K	0
#define	TX_SPD_50K	1
#define	TX_SPD_100K	TX_SPD_50K

/* FIORXLABELSET ioctl command arguments */

#define	RX_LABEL_INTEN	0x80000000
#define	RX_STORE_CYCLIC	0x40000000

/* globals */

extern int arincDebug;	/* set TRUE if want show debug messages */

/* function declarations */

extern int  btm404Init();

void   arincDebugSet (int);
 
#endif	/* __INCbtm404Drvh */

