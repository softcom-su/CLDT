/* bt128clk.c - clocks routines for K128RIO */

/*
modification history
--------------------

*/

#include "configbrd.h"
#include "bsp.h"

/* clock control structure */

#define T0_STATUS 		(TIMER0_BASE + TIMER_STATUS_REG)
#define T0_READ_LO		(TIMER0_BASE + TIMER_READ_LO_REG)
#define T0_READ_HI		(TIMER0_BASE + TIMER_READ_HI_REG)
#define T0_LOAD_LO		(TIMER0_BASE + TIMER_LOAD_LO_REG)
#define T0_LOAD_HI		(TIMER0_BASE + TIMER_LOAD_HI_REG)

#define T1_STATUS 		(TIMER1_BASE + TIMER_STATUS_REG)
#define T1_READ_LO 		(TIMER1_BASE + TIMER_READ_LO_REG)
#define T1_READ_HI 		(TIMER1_BASE + TIMER_READ_HI_REG)
#define T1_LOAD_LO 		(TIMER1_BASE + TIMER_LOAD_LO_REG)
#define T1_LOAD_HI 		(TIMER1_BASE + TIMER_LOAD_HI_REG)

#define T2_STATUS		(TIMER2_BASE + TIMER_STATUS_REG)
#define T2_READ_LO 		(TIMER2_BASE + TIMER_READ_LO_REG)
#define T2_READ_HI 		(TIMER2_BASE + TIMER_READ_HI_REG)
#define T2_LOAD_LO 		(TIMER2_BASE + TIMER_LOAD_LO_REG)
#define T2_LOAD_HI 		(TIMER2_BASE + TIMER_LOAD_HI_REG)

typedef struct {
    int			connected;	/* TRUE if intConnect passed */
    int                 ticks;          /* number of ticks */
    FUNCPTR		rtn;		/* user interrupt handler */
    long		arg;		/* handler argument */
    int			rate;		/* clock rate */
    int			iv;		/* interrupt vector */
    volatile unsigned *	ctrlReg;	/* status and control */
    volatile unsigned *	readRegLo;	/* read low */
    volatile unsigned *	readRegHi;	/* read high */
    volatile unsigned *	loadRegLo;	/* load low */
    volatile unsigned *	loadRegHi;	/* load high */
} TIMER;

/* locals */

static TIMER timer[3] = {
    [0] = {
	iv		: IV_TIMER0,
	ctrlReg		: (volatile unsigned *)T0_STATUS,
	readRegLo	: (volatile unsigned *)T0_READ_LO,
	readRegHi	: (volatile unsigned *)T0_READ_HI,
	loadRegLo	: (volatile unsigned *)T0_LOAD_LO,
	loadRegHi	: (volatile unsigned *)T0_LOAD_HI,
            },
    [1] = {
	iv		: IV_TIMER1,
	ctrlReg		: (volatile unsigned *)T1_STATUS,
	readRegLo	: (volatile unsigned *)T1_READ_LO,
	readRegHi	: (volatile unsigned *)T1_READ_HI,
	loadRegLo	: (volatile unsigned *)T1_LOAD_LO,
	loadRegHi	: (volatile unsigned *)T1_LOAD_HI,
    },
    [2] = {
	iv		: IV_TIMER2,
	ctrlReg		: (volatile unsigned *)T2_STATUS,
	readRegLo	: (volatile unsigned *)T2_READ_LO,
	readRegHi	: (volatile unsigned *)T2_READ_HI,
	loadRegLo	: (volatile unsigned *)T2_LOAD_LO,
	loadRegHi	: (volatile unsigned *)T2_LOAD_HI,
    }
};

/******************************************************************************/

/* get number of ticks happened */

static int tmrTickGet (unsigned ix)
{
    return timer[ix].ticks;
}

/******************************************************************************/

/* load timer counter */

int boardTimerCountSet64(unsigned ix, uint64_t count)
{
    int mask;

    if(ix > 2)
	return -1;

    *timer[ix].loadRegLo = count & 0xffffffff;
    *timer[ix].loadRegHi = count >> 32;
    mask = intLock();
    *timer[ix].ctrlReg |= TIMER_COUNT_LOAD;
    intUnlock(mask);
    udelay(1);
    return 0;
}

/******************************************************************************/

/* load timer counter from console */

int boardTimerCountSet32(unsigned ix, uint32_t lo, uint32_t hi)
{
    int mask;

    if(ix > 2)
	return -1;

    *timer[ix].loadRegLo = lo;
    *timer[ix].loadRegHi = hi;
    mask = intLock();
    *timer[ix].ctrlReg |= TIMER_COUNT_LOAD;
    intUnlock(mask);
    udelay(1);
    return 0;
}

/******************************************************************************/

/* read timer counter */

uint64_t boardTimerCountGet64(unsigned ix)
{
    uint32_t lo, hi;

    if(ix > 2)
	return 0;

    lo = *timer[ix].readRegLo;
    hi = *timer[ix].readRegHi;
    return (uint64_t)lo | ((uint64_t)hi << 32);
}

/******************************************************************************/

/* set timer control register */

int boardTimeModeSet(unsigned ix, uint32_t mode)
{
    if(ix > 2)
	return -1;

    *timer[ix].ctrlReg = mode;
    return 0;
}

/******************************************************************************/

/* get timer control register */

uint32_t boardTimeModeGet(unsigned ix)
{
    if(ix > 2)
	return 0xffffffff;

    return *timer[ix].ctrlReg;
}

/******************************************************************************/

/* show timer state */

int boardTimerShow(unsigned ix)
{
    uint32_t val, lo, hi;
    uint64_t load;

    if(ix > 2)
	return -1;

    val = *timer[ix].ctrlReg;

    printf("TIMER%d regs at %p\n",ix,timer[ix].loadRegLo);

    printf("MODE=%x: ",val);
    if(!val)
    {
	printf("DISABLE");
    }
    else
    {
	if(val & TIMER_ONE_SHOT)
	    printf("ONE_SHOT ");
	if(val & TIMER_RUN)
	    printf("RUN ");
	if(val & TIMER_INT_ENABLE)
	    printf("IRQ_ON ");
	if(val & TIMER_INT_PENDED)
	    printf("IRQ_PENDED ");
	if(val & TIMER_EXTERNAL_FREQ)
	    printf("EXT_FREQ ");
	if(val & TIMER_RST)
	    printf("RST ");
	if(val & TIMER_FEN)
	    printf("FEN ");
    }

    lo = *timer[ix].loadRegLo;
    hi = *timer[ix].loadRegHi;
    load = (uint64_t)lo | ((uint64_t)hi << 32);

    printf("\n LOAD=%lld COUNT=%lld\n",load,boardTimerCountGet64(ix));
    return 0;
}

/******************************************************************************/

/* This routine handles clock interrupts */

static void tmrIntr(int ix)
{
    timer[ix].ticks++;
    *timer[ix].ctrlReg &= ~TIMER_INT_PENDED;
    if(timer[ix].rtn)
	timer[ix].rtn(timer[ix].arg);
}

/*******************************************************************************/

/* tmrConnect - connect a routine to the clock interrupt */
 
static inline int tmrConnect(int ix, FUNCPTR rtn, intptr_t arg)
{
    if(!timer[ix].connected)
    {
	if(intConnect(timer[ix].iv,tmrIntr,ix) == -1)
	    return -1;
	
	timer[ix].connected = TRUE;
    }

    timer[ix].rtn = rtn;
    timer[ix].arg = arg;

    return 0;
}

/*******************************************************************************/

/* tmrDisable - stop timer count and disable interrupts */

static inline void tmrDisable(int ix)
{
    *timer[ix].ctrlReg = 0;
}

/********************************************************************************/

/* tmrEnable - start timer and enable interrupts */

static inline void tmrEnable(int ix)
{
    int mask;

    if(!timer[ix].connected)
    {
	if(intConnect(timer[ix].iv,tmrIntr,ix) == -1)
	    return;

	timer[ix].connected = TRUE;
    }

    mask = intLock();
    *timer[ix].ctrlReg = 0;
    *timer[ix].ctrlReg = TIMER_RUN | TIMER_INT_ENABLE | TIMER_EXTERNAL_FREQ;
    boardIvEnable(timer[ix].iv);
    intUnlock(mask);
}

/*******************************************************************************/

/* tmrRateSet - set the clock interrupt rate */

static inline STATUS tmrRateSet(int ix, unsigned ticksPerSecond)
{
    unsigned count;

    if(ticksPerSecond <= 0)
	return -1;

    count = TIMER_CLC_FREQ / ticksPerSecond;

    if(count < 2)
	return -1;

    tmrDisable(ix);

    boardTimerCountSet64(ix,count);

    timer[ix].rate = ticksPerSecond;
    return 0;
}


/******************************************************************************/

/* standard routines */

int boardAuxClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(AUX_CLOCK_TIMER,rtn,arg); }
int boardAux2ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(AUX2_CLOCK_TIMER,rtn,arg); }
int boardAux3ClkConnect(FUNCPTR rtn, intptr_t arg) { return tmrConnect(AUX3_CLOCK_TIMER,rtn,arg); }

void boardAuxClkEnable(void) { tmrEnable(AUX_CLOCK_TIMER); }
void boardAux2ClkEnable(void) { tmrEnable(AUX2_CLOCK_TIMER); }
void boardAux3ClkEnable(void) { tmrEnable(AUX3_CLOCK_TIMER); }

void boardAuxClkDisable(void) { tmrDisable(AUX_CLOCK_TIMER); }
void boardAux2ClkDisable(void) { tmrDisable(AUX2_CLOCK_TIMER); }
void boardAux3ClkDisable(void) { tmrDisable(AUX3_CLOCK_TIMER); }

int boardAuxClkFreqGet (void) { return TIMER_CLC_FREQ; }
int boardAux2ClkFreqGet (void) { return TIMER_CLC_FREQ; }
int boardAux3ClkFreqGet (void) { return TIMER_CLC_FREQ; }

int boardAuxClkRateGet(void) { return timer[AUX_CLOCK_TIMER].rate; }
int boardAux2ClkRateGet(void) { return timer[AUX2_CLOCK_TIMER].rate; }
int boardAux3ClkRateGet(void) { return timer[AUX3_CLOCK_TIMER].rate; }

int boardAuxClkRateSet(int _hz) { return tmrRateSet(AUX_CLOCK_TIMER,_hz); }
int boardAux2ClkRateSet(int _hz) { return tmrRateSet(AUX2_CLOCK_TIMER,_hz); }
int boardAux3ClkRateSet(int _hz) { return tmrRateSet(AUX3_CLOCK_TIMER,_hz); }

int boardAuxTickGet(void) { return tmrTickGet(AUX_CLOCK_TIMER); }
int boardAux2TickGet(void) { return tmrTickGet(AUX2_CLOCK_TIMER); }
int boardAux3TickGet(void) { return tmrTickGet(AUX3_CLOCK_TIMER); }

