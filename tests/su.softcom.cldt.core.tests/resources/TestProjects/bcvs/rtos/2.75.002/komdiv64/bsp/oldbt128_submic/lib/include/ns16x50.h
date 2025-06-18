/* ns16x50.h - header file for UART 16x50 driver core */

/*
modification history
--------------------
    03sep12,sok  change base type from int to long.
    04dec09,laz  NS_HOOK_ERR added
    23jul07,sok	 add nIn and nOut counters to NS_CHANNEL structure.
    04apr06,sok	 add ioctl's for enable/disable modem interrupts.
    12sec05,sok	 add MCR intial value to NS_CHANNEL structure.
    21apr04,sok	 written
*/

#ifndef INCns16x50h
#define INCns16x50h

/* LSR flag positions */

#define LSR_RxD		0x01
#define LSR_OE		0x02
#define	LSR_PE		0x04
#define	LSR_FE		0x08
#define	LSR_BI		0x10
#define	LSR_THRE	0x20
#define	LSR_TRE		0x40
#define	LSR_FIFO	0x80

/* IER bits */

#define IER_RxD		0x01
#define IER_THRE	0x02
#define IER_ERR		0x04
#define IER_MODEM	0x08

/* IIR bits */

#define IIR_IP		0x01
#define IIR_ID		0x06
#define IIR_EID		0x0e
#define IIR_ERR		0x06
#define IIR_TX		0x02
#define IIR_RX		0x04
#define IIR_RX_TOUT	0x0c
#define IIR_MODEM	0x00
#define IIR_FIFO_ON	0xc0

/* FCR bits */

#define FCR_EN		0x01
#define FCR_RX_RST	0x02
#define FCR_TX_RST	0x04
#define FCR_DMA		0x08
#define FCR_RX_L1	0x40
#define FCR_RX_L2	0x80
#define FCR_RX_L3	0xc0

/* LCR bits */

#define	LCR_LEN5		0x00
#define	LCR_LEN6		0x01
#define	LCR_LEN7		0x02
#define	LCR_LEN8		0x03
#define	LCR_LEN_MASK		0x03
#define	LCR_STOP1		0x00
#define	LCR_STOP2		0x04
#define	LCR_STOP_MASK		0x04
#define	LCR_PARITY_NO		0x00
#define	LCR_PARITY_ODD		0x08
#define	LCR_PARITY_EVEN		0x18
#define	LCR_PARITY_STICK	0x20
#define	LCR_PARITY_MASK		0x18
#define	LCR_BREAK		0x40
#define	LCR_DLAB		0x80

/* MCR bits */

#define	MCR_DTR			0x01	/* Data Terminal Ready */
#define	MCR_RTS			0x02	/* Request To Send */
#define	MCR_OUT1		0x04
#define	MCR_OUT2		0x08
#define	MCR_LOOP		0x10

/* MSR bits */

#define	MSR_DCTS		0x01	/* Delta Clear To Send */
#define	MSR_DDSR		0x02	/* Delta Data Set Ready */
#define	MSR_TERI		0x04	/* Trailing Edge Ring Indicator */
#define	MSR_DDCD		0x08	/* Delta Data Carrier Detected */
#define	MSR_CTS			0x10	/* Clear To Send */
#define	MSR_DSR			0x20	/* Data Set Ready */
#define	MSR_RI			0x40	/* Ring Indicator */
#define	MSR_DCD			0x80	/* Data Carrier Detected */

/* hook event key */

#define	NS_HOOK_TX		0	/* tx character */
#define	NS_HOOK_RX		1	/* rx character */
#define	NS_HOOK_VMODE		2	/* change video mode */
#define	NS_HOOK_OUT		3	/* tx character in outstring */
#define	NS_HOOK_CTS		4	/* CTS changed */
#define	NS_HOOK_RTS		5	/* RTS changed */
#define	NS_HOOK_MSR		6	/* modem interrupt happen and CTS/RTS not handled by system */
#define NS_HOOK_ERR             7       /* error registered */

/* ioctl command */

#define	NS_CMD_MASK		0xff000000
#define	NS_CMD_ID		0x55000000
#define	NS_CMD_DIV_GET		(NS_CMD_ID|0)	/* get divisor value */
#define	NS_CMD_FIFO_GET		(NS_CMD_ID|1)	/* get fifo depth */

#define	FIOFIFOLVLSET		0x56000000	/* set rx fifo trigger level */
#define	FIOTXFIFOSET		0x56000001	/* set tx fifo depth */
#define	FIOFIFODISABLE		0x56000002	/* disable tx and rx fifo */
#define	FIOMODEMINTDISABLE	0x56000003	/* disable modem interrupts */
#define	FIOMODEMINTENABLE	0x56000004	/* enable modem interrupts */
#define FIOTxNULL               0x56000005      /* output to null */

/* mcrInit have valid value */

#define	MCR_INIT_SET		0x80000000

/* register access */

#define	NS_IN(reg)		(ns->in(ns->base,(reg)))
#define	NS_OUT(reg,val)		(ns->out(ns->base,(reg),(val)))

/* channel descriptor */

typedef struct {
    void*		dev;		/* common terminal device header */
    char*		name;		/* channel name */
    uintptr_t		base;		/* base address of register */
    VOIDFUNCPTR		out;		/* write to register routine */
    FUNCPTR		in;		/* read from register routine */
    FUNCPTR		ioctl;		/* ioctl routine */
    long		id;		/* private channel data */
    FUNCPTR		hookRtn;	/* some hook routine */
    long		hookArg;	/* hook routine parameter */
    unsigned		nOverrun;	/* overrun error count */
    unsigned		nParity;	/* parity error count */
    unsigned		nFrame;		/* frame error count */
    unsigned		nBreak;		/* break error count */
    unsigned		nFIFO;		/* FIFO error count */
    unsigned		nDropped;	/* count of dropped bytes  */
    unsigned		fifo;		/* current FIFO depth */
    int			created;	/* TRUE if device created */
    int			debug;		/* debug level */
    int			rtsIsOff;	/* if TRUE RTS is inactive (high) */
    unsigned		rxFifoLevel;	/* rx FIFO trigger level */
    unsigned		mcrInit;	/* MCR initial value */
    unsigned		nIn;		/* count of input chars  */
    unsigned		nOut;		/* count of output chars  */
    int			txNull;         /* if TRUE - all output to null */
} NS_CHANNEL;

/* function declarations */

int nsDrvVersion(int silence);
int nsBaudSet(NS_CHANNEL* ns, int speed);
void nsHwInit(NS_CHANNEL* ns, int speed);
void nsIntr(NS_CHANNEL* ns);
int nsOutString(NS_CHANNEL* ns, char *s);
int nsDevInit(NS_CHANNEL* ns, char* name, int rdSize, int wrSize, int speed);
NS_CHANNEL* nsChannelFind(char* name);
int _nsDevInit(NS_CHANNEL* ns, char* name, int rdSize, int wrSize, int speed,
    int cread);
int nsMCRInitSet(NS_CHANNEL* ns, unsigned val);
int nsShow(char* name);

#endif /* INCns16x50h */
