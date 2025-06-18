/* sc64Timer.c - SC64 auxiliary timer library */

/*
modification history
--------------------
*/

/* check timer index */

#define	TIMER_IX_CHECK()	\
    do { if(ix > 4) return -1; } while(0);

/* timer regs map */

#define	SC64_TIMER	((TIMER_REGS*)(TIMER_BASE))

/* local defines */

/* timer regs */

typedef struct {
    volatile uint64_t   count;		/* count register */
    volatile uint8_t	csr;		/* control/status register */
    volatile uint8_t	unused[7];
} TIMER_REGS;

/* clock control structure */

typedef struct {
    int			connected;	/* TRUE if intConnect passed */
    VOIDFUNCPTR		rtn;		/* user interrupt handler */
    long		arg;		/* handler argument */
    int			rate;		/* clock rate */
    int			iv;		/* interrupt vector */
    uint64_t		count;		/* initial value */
    uint8_t		mode;		/* timer mode */
    int                 ticks;          /* number of interrupts */
} TIMER;

/* locals */

static TIMER timer[5] = {
    [0] = {
	iv		: IV_TIMER0,
    },
    [1] = {
	iv		: IV_TIMER1,
    },
    [2] = {
	iv		: IV_TIMER2,
    },
    [3] = {
	iv		: IV_TIMER3,
    },
    [4] = {
	iv		: IV_TIMER4,
    }
};

/* timers frequency in HZ */

unsigned sc64TimerFreq = 4000000;

/******************************************************************************/

/* This routine handles clock interrupts */

static int tmrIntr(unsigned ix)
{
    if(timer[ix].rtn)
	timer[ix].rtn(timer[ix].arg);

    SC64_TIMER[ix].csr = timer[ix].mode|2;
    timer[ix].ticks++;
    return SC64_TIMER[ix].csr;
}

/*******************************************************************************
*
* tmrConnect - connect a routine to the clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* clock interrupt.
*
* RETURN: 0, or -1 if the routine cannot be connected to the interrupt.
*
*/

static inline int tmrConnect(unsigned ix, VOIDFUNCPTR rtn, long arg)
{
    if(!timer[ix].connected)
    {
	if(intConnect(timer[ix].iv,(void*)tmrIntr,ix) == -1)
	    return -1;

	timer[ix].connected = TRUE;
    }

    timer[ix].rtn = rtn;
    timer[ix].arg = arg;

    return 0;
}

/******************************************************************************/

/* start timer counter */

int sc64TimerCountStart(unsigned ix)
{
    int mask;

    TIMER_IX_CHECK();

    mask = intLock();

    /* Start the counter */

#ifdef	BT210
    *(volatile uint8_t*)(SC64_REGS+0x24b) |= (1 << ix);
#else
    *(volatile uint8_t*)(SC64_REGS+0xd0) |= (1 << ix);
#endif
    intUnlock(mask);

    return 0;
}

/******************************************************************************/

/* stop timer counter */

int sc64TimerCountStop(unsigned ix)
{
    int mask;

    TIMER_IX_CHECK();

    mask = intLock();

    /* Stop the counter */

#ifdef	BT210
    *(volatile uint8_t*)(SC64_REGS+0x24b) &= ~(1 << ix);
#else
    *(volatile uint8_t*)(SC64_REGS+0xd0) &= ~(1 << ix);
#endif

    intUnlock(mask);

    return 0;
}

#ifndef	BT210

/******************************************************************************/

/* enable NMI from timer 4 out */

void sc64Timer4NMIEnable(void)
{
    int mask = intLock();
    *(volatile uint8_t*)(SC64_REGS+0xd0) |= 0x20;
    intUnlock(mask);
}

/******************************************************************************/

/* disable NMI from timer 4 out */

void sc64Timer4NMIDisable(void)
{
    int mask = intLock();
    *(volatile uint8_t*)(SC64_REGS+0xd0) &= ~0x20;
    intUnlock(mask);
}

#endif

/*******************************************************************************
*
* tmrDisable - turn off clock interrupts
*
* This routine disables clock interrupts.
*
* RETURNS: N/A
*
*/

static inline void tmrDisable(unsigned ix)
{
    sc64TimerCountStop(ix);

    /* Disable interrupts */

    boardIvDisable(timer[ix].iv);
}

/********************************************************************************
*
* tmrEnable - turn on clock interrupts
*
* This routine enables clock interrupts.
*
* RETURNS: N/A
*
*/

static inline void tmrEnable(unsigned ix)
{
    if(!timer[ix].connected)
	return;

    sc64TimerCountStart(ix);

    /* Enable interrupts */

    boardIvEnable(timer[ix].iv);
}

/******************************************************************************/

/* load timer counter */

int sc64TimerCountSet(unsigned ix, uint64_t count)
{
    int i;

    TIMER_IX_CHECK();

    SC64_TIMER[ix].csr = timer[ix].mode;
    timer[ix].count = count;
    SC64_TIMER[ix].count = CT_LE_LL(timer[ix].count);

    for(i = 0; i < 100; i++)
	if(!(SC64_TIMER[ix].csr & 0x40))
	    return 0;

    return -1;
}

/******************************************************************************/

/* read timer counter */

int sc64TimerCountGet(unsigned ix, uint64_t* count)
{
    int i;

    TIMER_IX_CHECK();

    SC64_TIMER[ix].csr = timer[ix].mode|1;

    for(i = 0; i < 100; i++)
    {
	if(!(SC64_TIMER[ix].csr & 0x20))
	{
	    uint64_t val = SC64_TIMER[ix].count;
	    *count = CF_LE_LL(val);
	    return 0;
	}
    }
    return -1;
}

/*******************************************************************************
*
* sc64TimerRateSet - set the clock rate and mode
*
* This routine sets the interrupt rate of the clock.
*
* RETURNS: 0, or -1 if the tick rate is invalid.
*
*/

int sc64TimerRateSet(unsigned ix, unsigned mul, unsigned div,
    unsigned mode)
{
    uint64_t val;

    TIMER_IX_CHECK();

    if(!div || !mul)
	return -1;

    val = ((uint64_t)sc64TimerFreq * mul) / div;

    if(!val)
	return -1;

    timer[ix].count = val;
    timer[ix].rate = div / mul;
    timer[ix].mode = mode;

    return sc64TimerCountSet(ix,val);
}

/******************************************************************************/

/* set timer CSR */

int sc64TimerCSRSet(unsigned ix, unsigned char val)
{
    TIMER_IX_CHECK();
    SC64_TIMER[ix].csr = val;
    return 0;
}

/******************************************************************************/

/* get timer CSR */

int sc64TimerCSRGet(unsigned ix, unsigned char* val)
{
    TIMER_IX_CHECK();
    *val = SC64_TIMER[ix].csr;
    return 0;
}

/******************************************************************************/

/* measure cpu frequency in MHz */

unsigned sc64CpuFreqGet(int lock, unsigned shift)
{
    unsigned mask = 0, start, stop, delta, freq, rem;
    uint64_t v, v1 = 0, cmp, t;

    sc64TimerCountStop(0);
    start = boardCountGet();

    v = sc64TimerFreq;			// 1 sec
    cmp = v - v/1000;			// 100 us

    if(lock)
	mask = intLock();

    sc64TimerCountSet(0,v);
    sc64TimerCountStart(0);

    start = boardCountGet();
    sc64TimerCountGet(0,&v1);
    do {
	sc64TimerCountGet(0,&v);
    } while(v >= cmp);
    stop = boardCountGet();
    sc64TimerCountGet(0,&v);

    sc64TimerCountStop(0);

    if(lock)
	intUnlock(mask);

    t = (v1 - v) * (1000000000ULL / sc64TimerFreq);
    delta = (stop - start) << shift;

    freq = 1000000000ULL*delta/t;
    rem = freq % 1000000;
    freq = freq / 1000000;
    return (rem >= 500000) ? freq+1 : freq;
}

#ifndef BT210
/******************************************************************************/

/* timer library initialization */

void sc64TimerInit(void)
{
    unsigned char N;
#ifdef	BOARD_TIMER_DIV
    N = BOARD_TIMER_DIV;
    *(volatile unsigned char*)(SC64_REGS+0xd1) = N;
#else
    N = *(volatile unsigned char*)(SC64_REGS+0xd1);
#endif
    if(N == 255)
	sc64TimerFreq = boardFreqRef;
    else
	sc64TimerFreq = boardFreqRef / (2*(N+1));
}
#endif /* #ifndef BT210 */
/******************************************************************************/

/* standard global routines */

int boardAuxClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(0,(VOIDFUNCPTR)rtn,arg); }
int boardAux2ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(1,(VOIDFUNCPTR)rtn,arg); }
int boardAux3ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(2,(VOIDFUNCPTR)rtn,arg); }
int boardAux4ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(3,(VOIDFUNCPTR)rtn,arg); }
int boardAux5ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(4,(VOIDFUNCPTR)rtn,arg); }
void boardAuxClkDisable(void) { tmrDisable(0); }
void boardAux2ClkDisable(void) { tmrDisable(1); }
void boardAux3ClkDisable(void) { tmrDisable(2); }
void boardAux4ClkDisable(void) { tmrDisable(3); }
void boardAux5ClkDisable(void) { tmrDisable(4); }
void boardAuxClkEnable(void) { tmrEnable(0); }
void boardAux2ClkEnable(void) { tmrEnable(1); }
void boardAux3ClkEnable(void) { tmrEnable(2); }
void boardAux4ClkEnable(void) { tmrEnable(3); }
void boardAux5ClkEnable(void) { tmrEnable(4); }
int boardAuxClkRateGet(void) { return timer[0].rate; }
int boardAux2ClkRateGet(void) { return timer[1].rate; }
int boardAux3ClkRateGet(void) { return timer[2].rate; }
int boardAux4ClkRateGet(void) { return timer[3].rate; }
int boardAux5ClkRateGet(void) { return timer[4].rate; }
int boardAuxClkRateSet(int _hz) { return sc64TimerRateSet(0,1,_hz,TIMER_MODE_CYCLIC); }
int boardAux2ClkRateSet(int _hz) { return sc64TimerRateSet(1,1,_hz,TIMER_MODE_CYCLIC); }
int boardAux3ClkRateSet(int _hz) { return sc64TimerRateSet(2,1,_hz,TIMER_MODE_CYCLIC); }
int boardAux4ClkRateSet(int _hz) { return sc64TimerRateSet(3,1,_hz,TIMER_MODE_CYCLIC); }
int boardAux5ClkRateSet(int _hz) { return sc64TimerRateSet(4,1,_hz,TIMER_MODE_CYCLIC); }


int boardAuxTickGet (int ix) 
{  if (ix < 5) return timer[ix].ticks;
   return -1;
}

