/* intCtrl.c - SK64 interrupt controlling library */

#define	INDEX_MAP		0
#define	INDEX_PRI		12
#define	INDEX_VECTOR		36

/* controller regs */

typedef struct {
    volatile unsigned char	mask[3];
    volatile unsigned char	index;
    volatile unsigned char	data;
    volatile unsigned char	vector[10];
} SK64_INTR_CTRL;

#define	INTR_REGS	((SK64_INTR_CTRL*)INTR_CTRL_BASE)

#ifdef	K64SMP
  #define	PRI_PCI_INTA	0
  #define	PRI_PCI_INTB	1
  #define	PRI_PCI_INTC	2
  #define	PRI_PCI_INTD	3
#else
  #define	PRI_PCI0_INTA	0
  #define	PRI_PCI0_INTB	1
  #define	PRI_PCI0_INTC	2
  #define	PRI_PCI0_INTD	3
#endif
#define	PRI_TIMER4	4
#ifdef	K64SMP
#define	PRI_I2C		5
#else
#define	PRI_KBD		5
#endif
#define	PRI_UART0	6
#ifdef	K64SMP
#define	PRI_PCI		7
#define	PRI_NAND	8
#else
#define	PRI_PCI0	7
#define	PRI_PCI1	8
#endif
#define	PRI_TIMER3	9
#define	PRI_TIMER2	10
#define	PRI_TIMER1	11
#define	PRI_TIMER0	12
#define	PRI_GPIO2	13
#define	PRI_GPIO3	14
#define	PRI_SC		15
#define	PRI_UART1	16
#define	PRI_IRQ0	17
#if defined(MPON) || defined(BT83M)
#define	PRI_GPIO10	18
#define	PRI_GPIO11	19
#endif
#ifndef	K64SMP
#define	PRI_PCI1_INTA	18
#define	PRI_PCI1_INTB	19
#define	PRI_PCI1_INTC	20
#define	PRI_PCI1_INTD	21
#elif defined(K64RIO)
#define PRI_RIO         21
#endif
#define	PRI_IRQ1	22
#define	PRI_ETH		23

#ifdef	K64SMP
  #ifdef VG12_PMC
    #warning VME test code
    #define	INT_PCI_INTA	1
    #define	INT_PCI_INTB	0
    #define	INT_PCI_INTC	1
    #define	INT_PCI_INTD	0
  #else
    #define	INT_PCI_INTA	0
    #define	INT_PCI_INTB	0
#if defined(MPON) || defined(K64RIO) || defined(BT83M)
    #define	INT_PCI_INTC	0
#else
    #define	INT_PCI_INTC	1
#endif
    #define	INT_PCI_INTD	0
  #endif
#elif	defined(BT210)
#define	INT_PCI0_INTA	0
#define	INT_PCI0_INTB	0
#define	INT_PCI0_INTC	0
#define	INT_PCI0_INTD	0
#else
#define	INT_PCI0_INTA	0
#define	INT_PCI0_INTB	1
#define	INT_PCI0_INTC	0
#define	INT_PCI0_INTD	0
#endif
#define	INT_TIMER4	0
#ifdef	K64SMP
#define	INT_I2C		0
#else
#define	INT_KBD		0
#endif
#define	INT_UART0	0
#ifdef	K64SMP
#define	INT_PCI		0
#define	INT_NAND	0
#else
#define	INT_PCI0	0
#define	INT_PCI1	0
#endif
#define	INT_TIMER3	0
#define	INT_TIMER2	0
#define	INT_TIMER1	0
#define	INT_TIMER0	0
#define	INT_GPIO2	0
#define	INT_GPIO3	0
#define	INT_SC		0
#define	INT_UART1	0
#define	INT_IRQ0	0
#if defined(MPON) || defined(BT83M)
#define	INT_GPIO10	0
#define	INT_GPIO11	0
#endif
#ifdef	BT210
#define	INT_PCI1_INTA	1
#define	INT_PCI1_INTB	0
#define	INT_PCI1_INTC	0
#define	INT_PCI1_INTD	0
#elif	!defined(K64SMP)
#define	INT_PCI1_INTA	0
#define	INT_PCI1_INTB	0
#define	INT_PCI1_INTC	0
#define	INT_PCI1_INTD	0
#elif defined(K64RIO)
#define	INT_RIO		0
#endif
#define	INT_IRQ1	0
#define	INT_ETH		0

static unsigned char sk64IntrConfig[24][3] = {
#ifdef	K64SMP
    { PRI_PCI_INTA,	INT_PCI_INTA,	IV_PCI_INTA },
    { PRI_PCI_INTB,	INT_PCI_INTB,	IV_PCI_INTB },
    { PRI_PCI_INTC,	INT_PCI_INTC,	IV_PCI_INTC },
    { PRI_PCI_INTD,	INT_PCI_INTD,	IV_PCI_INTD },
#else
    { PRI_PCI0_INTA,	INT_PCI0_INTA,	IV_PCI0_INTA },
    { PRI_PCI0_INTB,	INT_PCI0_INTB,	IV_PCI0_INTB },
    { PRI_PCI0_INTC,	INT_PCI0_INTC,	IV_PCI0_INTC },
    { PRI_PCI0_INTD,	INT_PCI0_INTD,	IV_PCI0_INTD },
#endif
    { PRI_TIMER4,	INT_TIMER4,	IV_TIMER4 },
#ifdef	K64SMP
    { PRI_I2C,		INT_I2C,	IV_I2C },
#else
    { PRI_KBD,		INT_KBD,	IV_KBD },
#endif
    { PRI_UART0,	INT_UART0,	IV_UART0 },
#ifdef	K64SMP
    { PRI_PCI,		INT_PCI,	IV_PCI },
    { PRI_NAND,		INT_NAND,	IV_NAND },
#else
    { PRI_PCI0,		INT_PCI0,	IV_PCI0 },
    { PRI_PCI1,		INT_PCI1,	IV_PCI1 },
#endif
    { PRI_TIMER3,	INT_TIMER3,	IV_TIMER3 },
    { PRI_TIMER2,	INT_TIMER2,	IV_TIMER2 },
    { PRI_TIMER1,	INT_TIMER1,	IV_TIMER1 },
    { PRI_TIMER0,	INT_TIMER0,	IV_TIMER0 },
    { PRI_GPIO2,	INT_GPIO2,	IV_GPIO2 },
    { PRI_GPIO3,	INT_GPIO3,	IV_GPIO3 },
    { PRI_SC,		INT_SC,		IV_SC },
    { PRI_UART1,	INT_UART1,	IV_UART1 },
    { PRI_IRQ0,		INT_IRQ0,	IV_IRQ0 },
#ifdef	K64SMP
#if defined(MPON) || defined(BT83M)
    { PRI_GPIO10,	INT_GPIO10,	IV_GPIO10 },
    { PRI_GPIO11,	INT_GPIO11,	IV_GPIO11 },
#else
    { 18,		0,		0xff },
    { 19,		0,		0xff },
#endif
    { 20,		0,		0xff },
#ifdef	K64RIO
    { PRI_RIO,          INT_RIO,        IV_RIO },
#else
    { 21,		0,		0xff },
#endif
#else
    { PRI_PCI1_INTA,	INT_PCI1_INTA,	IV_PCI1_INTA },
    { PRI_PCI1_INTB,	INT_PCI1_INTB,	IV_PCI1_INTB },
    { PRI_PCI1_INTC,	INT_PCI1_INTC,	IV_PCI1_INTC },
    { PRI_PCI1_INTD,	INT_PCI1_INTD,	IV_PCI1_INTD },
#endif
    { PRI_IRQ1,		INT_IRQ1,	IV_IRQ1 },
    { PRI_ETH,		INT_ETH,	IV_ETH },
};

static const char* sk64IntrName[] = {
#ifdef	K64SMP
    "PCI_INTA",
    "PCI_INTB",
    "PCI_INTC",
    "PCI_INTD",
#else
    "PCI0_INTA",
    "PCI0_INTB",
    "PCI0_INTC",
    "PCI0_INTD",
#endif
    "TIMER4",
#ifdef	K64SMP
    "I2C  ",
#else
    "KBD  ",
#endif
    "UART0",
#ifdef	K64SMP
    "PCI  ",
    "NAND ",
#else
    "PCI0",
    "PCI1",
#endif
    "TIMER3",
    "TIMER2",
    "TIMER1",
    "TIMER0",
    "GPIO2",
    "GPIO3",
    "SC   ",
    "UART1",
    "IRQ0 ",
#ifdef	K64SMP
#if defined(MPON) || defined(BT83M)
    "GPIO10",
    "GPIO11",
#else
    "UNUSED18",
    "UNUSED19",
#endif
    "UNUSED20",
#ifdef K64RIO
    "RIO  ",
#else
    "UNUSED21",
#endif
#else
    "PCI1_INTA",
    "PCI1_INTB",
    "PCI1_INTC",
    "PCI1_INTD",
#endif
    "IRQ1 ",
    "ETH  ",
};

#ifdef	INCLUDE_IV_COUNT
unsigned boardIvCount[256];
#endif

/******************************************************************************/

/* hardware spurious interrupt handler */

void boardHwSpuriousIntr(int unused)
{
    BAD_LOG("%s!!!\n",(int)__FUNCTION__,0,0,0,0,0);
}

/******************************************************************************/

/* initialization routine */

void intrCtrlInit()
{
    unsigned ix; volatile unsigned char iv;

    /* mask all interrupts */

    INTR_REGS->mask[0] = INTR_REGS->mask[1] = INTR_REGS->mask[2] = 0xff;

    /* clear all pending interrupts */

    for(ix = 0; ix < 10; ix++)
    {
	iv = INTR_REGS->vector[ix];
    }

    for(ix = 0; ix < 24; ix++)
    {
	/* map SC interrupt lines to cpu int pin */

	if(!(ix & 1))
	{
	    INTR_REGS->index = INDEX_MAP + (ix / 2);
	    INTR_REGS->data = sk64IntrConfig[ix][1] |
		(sk64IntrConfig[ix+1][1] << 4);
	}

	/* assign line priority */

	INTR_REGS->index = INDEX_PRI + ix;
	INTR_REGS->data = sk64IntrConfig[ix][0];

	/* assign line vector */

	INTR_REGS->index = INDEX_VECTOR + ix;
	INTR_REGS->data = sk64IntrConfig[ix][2];
    }

    intConnect(IV_HW_SPURIOUS,boardHwSpuriousIntr,0);
}

/******************************************************************************/

void intrCtrlShow(void)
{
    unsigned ix, pin, pri, iv, mask;

    for(ix = 0; ix < 24; ix++)
    {
	INTR_REGS->index = INDEX_MAP + (ix / 2);
	pin = INTR_REGS->data;

	mask = INTR_REGS->mask[ix / 8];
	mask = mask & (1 << (ix & 7));

	if(ix & 1)
	    pin >>= 4;
	else
	    pin = pin & 0x0f;

	INTR_REGS->index = INDEX_PRI + ix;
	pri = INTR_REGS->data;

	INTR_REGS->index = INDEX_VECTOR + ix;
	iv = INTR_REGS->data;

#ifdef	INCLUDE_IV_COUNT
	printf(" %02d %s\t: pin=%d  pri=%02d iv=0x%02x %s count=%d\n\r",ix,
	    sk64IntrName[ix],pin,pri,iv,mask ? "OFF" : "ON ",boardIvCount[iv]);
#else
	printf(" %02d %s\t: pin=%d  pri=%02d iv=0x%02x %s\n\r",ix,
	    sk64IntrName[ix],pin,pri,iv,mask ? "OFF" : "ON");
#endif
    }
}

/******************************************************************************/

/* enable or disable specified interrupt source */

static int intrCtrlMask(unsigned iv, int enable)
{
    unsigned mask, line, byte, bit;

    if((line = IV_TO_LINE(iv)) > 23)
    {
#ifdef	BAD_INTS_LOGGING
	logMsg("boardIvEnable: invalid iv=%d\n\r",iv,0,0,0,0,0);
#endif
	return -1;
    }

    byte = line / 8;
    bit = line & 7;

    mask = intLock();

    if(enable)
    {
	INTR_REGS->mask[byte] &= ~(1 << bit);
    }
    else
    {
	INTR_REGS->mask[byte] |= (1 << bit);
    }

    intUnlock(mask);

    return 0;
}

/******************************************************************************/

/* enable interrupt source */

int boardIvEnable(unsigned iv)
{
    return intrCtrlMask(iv,TRUE);
}

/******************************************************************************/

/* disable interrupt source */

int boardIvDisable(unsigned iv)
{
    return intrCtrlMask(iv,FALSE);
}

/******************************************************************************/

/* interrupt demultiplexor */

int boardIntDemux(unsigned line)
{
    unsigned iv = INTR_REGS->vector[line-1];
    TRACE(TR_INTR,(line << 8)|iv);
#ifdef	INCLUDE_IV_COUNT
    boardIvCount[iv]++;
#endif
    if(iv < IV_BASE || iv > IV_ETH)
    {
	kprint("%s: invalid iv=%x line=%d\n\r",__FUNCTION__,iv,line);
	return 0xff;
    }
    return iv;
}

/******************************************************************************/

/* VME interrupt demultiplexor */

#ifdef	INCLUDE_VME

int boardVmeDemux(unsigned line)
{
    static volatile int iv;
    iv = INTR_REGS->vector[line-1];
    TRACE(TR_INTR,(line << 8)|iv);
#ifdef	INCLUDE_IV_COUNT
    boardIvCount[iv]++;
#endif
    return vmeIvCheck();
}

#endif	/* INCLUDE_VME */

/******************************************************************************/

/* get vector name */

#if OS_VERSION >= 300
const char* boardIntVecName(int vec)
{
    switch(vec)
    {
	case IV_TIMER:
	    return "TIMER";
    }

    if(vec < IV_BASE || vec > IV_ETH)
	return "Unknown";
    return sk64IntrName[vec-IV_BASE];
}
#endif

