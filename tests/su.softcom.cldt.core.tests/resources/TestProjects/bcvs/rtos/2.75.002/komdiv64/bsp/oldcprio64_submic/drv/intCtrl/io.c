/* io.c - SC64 discrete signals library */

#define	IO_SET_A	((volatile unsigned char*)(SC64_REGS+0x250+0x0))
#define	IO_PIN_A	((volatile unsigned char*)(SC64_REGS+0x250+0x0))
#define	IO_CLEAR_A	((volatile unsigned char*)(SC64_REGS+0x250+0x1))
#define	IO_LATCH_A	((volatile unsigned char*)(SC64_REGS+0x250+0x1))
#define	IO_DIR_A	((volatile unsigned char*)(SC64_REGS+0x250+0x2))
#define	IO_FUNC_A	((volatile unsigned char*)(SC64_REGS+0x250+0x3))
#define	IO_INT_A	((volatile unsigned char*)(SC64_REGS+0x250+0x4))
#define	IO_IMM_A	((volatile unsigned char*)(SC64_REGS+0x250+0x5))

#define	IO_SET_B	((volatile unsigned char*)(SC64_REGS+0x258+0x0))
#define	IO_PIN_B	((volatile unsigned char*)(SC64_REGS+0x258+0x0))
#define	IO_CLEAR_B	((volatile unsigned char*)(SC64_REGS+0x258+0x1))
#define	IO_LATCH_B	((volatile unsigned char*)(SC64_REGS+0x258+0x1))
#define	IO_DIR_B	((volatile unsigned char*)(SC64_REGS+0x258+0x2))
#define	IO_FUNC_B	((volatile unsigned char*)(SC64_REGS+0x258+0x3))
#define	IO_INT_B	((volatile unsigned char*)(SC64_REGS+0x258+0x4))
#define	IO_IMM_B	((volatile unsigned char*)(SC64_REGS+0x258+0x5))

#define	IO_EXT_INT01	((volatile unsigned char*)(SC64_REGS+0x130))
#define	IO_EXT_INT23	((volatile unsigned char*)(SC64_REGS+0x131))
#define	IO_EXT_INT45	((volatile unsigned char*)(SC64_REGS+0x132))
#define	IO_EXT_INT6	((volatile unsigned char*)(SC64_REGS+0x133))

#define IO_DIR_INPUT    0
#define IO_DIR_OUTPUT   1

/* function register bits */

#define	FUNC_EXT_TIMER_CLK	1
#define	FUNC_TIMER0_OUT		2

#ifdef K64RIO
#define	FUNC_EXT_INT2		0x10
#define	FUNC_EXT_INT3		0x20
#define	FUNC_EXT_INT4		0x40
#define	FUNC_EXT_INT5		0x80
#define	FUNC_EXT_INT6		0x40
#else
#define	FUNC_EXT_INT0		4
#define	FUNC_EXT_INT1		8
#endif

/*******************************************************/
void boardPioInit (void)
{
    /* set PIO directions */

#ifdef BT55211
    boardUserDir(0, IO_DIR_INPUT);
    boardUserDir(1, IO_DIR_OUTPUT);
    boardUserDir(2, IO_DIR_INPUT);
    boardUserDir(3, IO_DIR_INPUT);
    boardUserDir(4, IO_DIR_OUTPUT);
    boardUserDir(5, IO_DIR_INPUT);
    boardUserDir(6, IO_DIR_INPUT);
    boardUserDir(7, IO_DIR_INPUT);
    boardUserDir(8, IO_DIR_OUTPUT);
    boardUserDir(9, IO_DIR_OUTPUT);
    boardUserDir(10, IO_DIR_OUTPUT);
    boardUserDir(11, IO_DIR_OUTPUT);
    boardUserDir(12, IO_DIR_INPUT);
    boardUserDir(13, IO_DIR_INPUT);
    boardUserDir(14, IO_DIR_INPUT);
    boardUserDir(15, IO_DIR_OUTPUT);
#else
  #ifdef CPRIO64 
    #ifdef SUBMICRON
      #warning  !!!!!!!!!!!!!SUBMICRON version of GPIO !!!!!!!!!!!
    boardUserDir(0, IO_DIR_OUTPUT);
    boardUserDir(1, IO_DIR_INPUT);
    boardUserDir(2, IO_DIR_INPUT);
    boardUserDir(3, IO_DIR_INPUT);
    boardUserDir(4, IO_DIR_INPUT);
    boardUserDir(5, IO_DIR_INPUT);
    boardUserDir(6, IO_DIR_INPUT);
    boardUserDir(7, IO_DIR_OUTPUT);
    boardUserDir(8, IO_DIR_INPUT);   /* port B:0 */
    boardUserDir(9, IO_DIR_INPUT);
    boardUserDir(10, IO_DIR_INPUT);
    boardUserDir(11, IO_DIR_INPUT);
    boardUserDir(12, IO_DIR_INPUT);
    boardUserDir(13, IO_DIR_INPUT);
    boardUserDir(14, IO_DIR_INPUT);
    boardUserDir(15, IO_DIR_OUTPUT);
    #else
    boardUserDir(0, IO_DIR_INPUT);
    boardUserDir(1, IO_DIR_OUTPUT);
    boardUserDir(2, IO_DIR_INPUT);
    boardUserDir(3, IO_DIR_INPUT);
    boardUserDir(4, IO_DIR_INPUT);
    boardUserDir(5, IO_DIR_INPUT);
    boardUserDir(6, IO_DIR_INPUT);
    boardUserDir(7, IO_DIR_INPUT);
    boardUserDir(8, IO_DIR_INPUT);   /* port B:0 */
    boardUserDir(9, IO_DIR_INPUT);
    boardUserDir(10, IO_DIR_INPUT);
    boardUserDir(11, IO_DIR_OUTPUT);
    boardUserDir(12, IO_DIR_OUTPUT);
    boardUserDir(13, IO_DIR_OUTPUT);
    boardUserDir(14, IO_DIR_OUTPUT);
    boardUserDir(15, IO_DIR_OUTPUT);
    #endif
  #else
    #ifdef BT211
      boardUserDir(0, IO_DIR_INPUT);   /* Ext timer freq */
      boardUserDir(1, IO_DIR_OUTPUT);  /* ??? */
      boardUserDir(2, IO_DIR_INPUT);   /* IF_EXTINT0 */
      boardUserDir(3, IO_DIR_INPUT);   /* IF_EXTINT1 */
      boardUserDir(4, IO_DIR_OUTPUT);   /* Test OK */
      boardUserDir(5, IO_DIR_INPUT);   /* run mode - 3 bits */
      boardUserDir(6, IO_DIR_INPUT);
      boardUserDir(7, IO_DIR_INPUT);
      boardUserDir(8, IO_DIR_OUTPUT);   /* port B:0    USEROUT[0..3] */
      boardUserDir(9, IO_DIR_OUTPUT);
      boardUserDir(10, IO_DIR_OUTPUT);
      boardUserDir(11, IO_DIR_OUTPUT);
      boardUserDir(12, IO_DIR_INPUT);  /*              USERIN [0..2] */
      boardUserDir(13, IO_DIR_INPUT);
      boardUserDir(14, IO_DIR_INPUT);
      boardUserDir(15, IO_DIR_INPUT);  /* Sys OK */
    #else
      #ifdef BT83M
        boardUserDir(0, IO_DIR_INPUT);   /* Ext timer freq */
        boardUserDir(1, IO_DIR_OUTPUT);   /* TSOUT  ( before -EXT_INT3) */
        boardUserDir(2, IO_DIR_INPUT);   /* EXT_INT2 */
        boardUserDir(3, IO_DIR_INPUT);    /* EXT_INT3 (before - TSOUT) */ 
        boardUserDir(4, IO_DIR_OUTPUT);   /* Test OK */
        boardUserDir(5, IO_DIR_INPUT);    /* UERIN[...] */
        boardUserDir(6, IO_DIR_INPUT);
        boardUserDir(7, IO_DIR_INPUT);
        boardUserDir(8, IO_DIR_OUTPUT);   /* port B:0    USEROUT[0..6] */
        boardUserDir(9, IO_DIR_OUTPUT);
        boardUserDir(10, IO_DIR_OUTPUT);
        boardUserDir(11, IO_DIR_OUTPUT);
        boardUserDir(12, IO_DIR_OUTPUT);  /*    */
        boardUserDir(13, IO_DIR_OUTPUT);
        boardUserDir(14, IO_DIR_OUTPUT);
        boardUserDir(15, IO_DIR_OUTPUT);  /* Sys OK */
        *IO_FUNC_A = 0xc;                 /* Enable EXT_INT0 and EXT_INT1 on A[2,3] */
      #else
        #error !!!!!!!!!! No define with module type !!!!!
      #endif
    #endif 
  #endif
#endif
} 
/******************************************************************************/

/* flush write buffer */

static inline unsigned ioFlush(volatile unsigned char* reg)
{
    return *reg;
}

/******************************************************************************/

/* read 16 bits of Users Inputs */

unsigned short boardUserIn(void)
{
#if defined(BT83M)
  return ((unsigned short)(*(unsigned char *)RTC_BASE ) & 0xff);
#else
    return *IO_PIN_A | (*IO_PIN_B << 8);
#endif  
}

/******************************************************************************/

/* output bit b to users line n */

int boardUserOut(int nline, int b)
{
    volatile unsigned char* reg;
    unsigned char val;
    int n;

    if((unsigned)nline > 15 || (unsigned)b > 1)
	return -1;

#ifndef FOR_SUBMICRON
    switch (nline)
    { case 0: n = BOARD_USEROUT_0; break;
      case 1: n = BOARD_USEROUT_1; break;
      case 2:
#if defined(BT55211)
      case 10:
#endif
	n = BOARD_USEROUT_2; break;
      case 3: 
#if defined(BT55211)
      case 11:
#endif	
	n = BOARD_USEROUT_3; break;
      #if defined(BT83M)
        case 4: n = BOARD_USEROUT_4; break;
        case 5: n = BOARD_USEROUT_5; break;
        case 6: n = BOARD_USEROUT_6; break;
      #endif
      default: return -1;
    }
#else  
    //gmu: for bcvs (Для прибора БЦВС могут быть установлены только GP_A[0,7], GP_B[7])
   if((nline!=0)&&(nline!=7)&&(nline!=15))
   {
     return -1;
   }
   else 
   {
     n = nline;
   }
#endif //FOR_SUBMICRON   

    if(n > 7)
    {
	reg = b ? IO_SET_B : IO_CLEAR_B;
	val = 1 << (n - 8);
    }
    else
    {
	reg = b ? IO_SET_A : IO_CLEAR_A;
	val = 1 << n;
    }

    *reg = val;

    return 0;
}

#ifndef MPON

/******************************************************************************/

/* output signal Test OK */

void boardTestOK(void)
{
    *IO_SET_A = (1<<4);
}

/*-------------------------------------------------------------------------*/
/* clear signal Test OK */

void boardClrTestOK(void)
{
    *IO_CLEAR_A = (1<<4) ;
}

#endif

/******************************************************************************/

/* set all 16 user out's */

void boardUserOutAll(unsigned short val)
{
#ifndef FOR_SUBMICRON
#if defined(BT55211)
    if(val & 0xc00)
      val >>= 8;
#else
    #if defined(BT83M)
    val = (val & 0x7f) << BOARD_USEROUT_0;
    #else
    val = (val & 0xf) << BOARD_USEROUT_0;
    #endif
    *IO_IMM_A = val & 0xff;
    *IO_IMM_B = val >> 16;
#endif /* BT55211 */
#else
 //gmu: bcvs must be output GP_A[0,7],GP_B[7]
 // clear latch
  *IO_CLEAR_A=0x81;  // !!!!
  *IO_CLEAR_B=0x80;
 // set val
  *IO_SET_A=val&0x81;
  *IO_SET_B=(val>>8)&0x80;
#endif /* FOR_SUBMICRON */
}

/******************************************************************************/

/* read 16 bits of Users Outputs */

unsigned short boardUserLatch(void)
{
    return *IO_LATCH_A | (*IO_LATCH_B << 8);
}

/******************************************************************************/

/* set direction of descrete signal */

int boardUserDir(unsigned n, unsigned dir)
{
    volatile unsigned char* reg;
    unsigned char val;
    unsigned mask;

    if(n > 15)
	return -1;

    if(n > 7)
    {
	reg = IO_DIR_B;
	val = 1 << (n - 8);
    }
    else
    {
	reg = IO_DIR_A;
	val = 1 << n;
    }

    switch(dir)
    {
	case IO_DIR_INPUT:
	    mask = intLock();
	    *reg &= ~val;
	    intUnlock(mask);
	    break;
	case IO_DIR_OUTPUT:
	    mask = intLock();
	    *reg |= val;
	    intUnlock(mask);
	    break;
	default:
	    return -1;
    }

    return 0;
}
#ifdef BT83M
/***********************************************/

void boardExtInt0Set(int mode)
{ unsigned w = *(volatile unsigned char *)IRQ_BASE & 0xcb;
  switch (mode & 7)
  { case 0x0: w |= 0x30; break;   /* edge, fall, disable */
    case 0x1: w |= 0x10; break;   /* level, low, disable */
    case 0x2: w |= 0x20; break;   /* edge, rise, disable */
    case 0x3: w |= 0x00; break;   /* level, high, disable */
    case 0x4: w |= 0x34; break;   /* edge, fall, enable */
    case 0x5: w |= 0x14; break;   /* level, low, enable */
    case 0x6: w |= 0x24; break;   /* edge, rise, enable */
    case 0x7: w |= 0x04; break;   /* level, high, enable */
  }
  *(volatile unsigned char *)IRQ_BASE = w;
}
/***********************************************/

void boardExtInt1Set(int mode)
{ unsigned w = *(volatile unsigned char *)IRQ_BASE & 0x37;
  switch (mode & 7)
  { case 0x0: w |= 0xc0; break;   /* edge, fall, disable */
    case 0x1: w |= 0x40; break;   /* level, low, disable */
    case 0x2: w |= 0x80; break;   /* edge, rise, disable */
    case 0x3: w |= 0x00; break;   /* level, high, disable */
    case 0x4: w |= 0xc8; break;   /* edge, fall, enable */
    case 0x5: w |= 0x48; break;   /* level, low, enable */
    case 0x6: w |= 0x88; break;   /* edge, rise, enable */
    case 0x7: w |= 0x08; break;   /* level, high, enable */
  }
  *(volatile unsigned char *)IRQ_BASE = w;
}
/***********************************************/

void boardExtInt2Set(int mode)
{ unsigned char w = *IO_INT_A & 0xcb;
  switch (mode & 7)
  { case 0x0: w |= 0x30; break;   /* edge, fall, disable */
    case 0x1: w |= 0x10; break;   /* level, low, disable */
    case 0x2: w |= 0x20; break;   /* edge, rise, disable */
    case 0x3: w |= 0x00; break;   /* level, high, disable */
    case 0x4: w |= 0x34; break;   /* edge, fall, enable */
    case 0x5: w |= 0x14; break;   /* level, low, enable */
    case 0x6: w |= 0x24; break;   /* edge, rise, enable */
    case 0x7: w |= 0x04; break;   /* level, high, enable */
  }
  *IO_INT_A = w;
}
#if 1
/***********************************************/

void boardExtInt3Set(int mode)
{ unsigned char w = *IO_INT_A & 0x35;
  switch (mode & 7)
  { case 0x0: w |= 0xc0; break;   /* edge, fall, disable */
    case 0x1: w |= 0x40; break;   /* level, low, disable */
    case 0x2: w |= 0x80; break;   /* edge, rise, disable */
    case 0x3: w |= 0x00; break;   /* level, high, disable */
    case 0x4: w |= 0xc8; break;   /* edge, fall, enable */
    case 0x5: w |= 0x48; break;   /* level, low, enable */
    case 0x6: w |= 0x88; break;   /* edge, rise, enable */
    case 0x7: w |= 0x08; break;   /* level, high, enable */
  }
  *IO_INT_A |= w;
}
#endif
/************************************************/
void boardExtInt0Clear()
{ unsigned w=intLock(); 
  *(volatile unsigned char *)IRQ_BASE |= 1;
  intUnlock(w);
}
/************************************************/
void boardExtInt1Clear()
{ unsigned w=intLock(); 
  *(volatile unsigned char *)IRQ_BASE |= 2;
  intUnlock(w);
}
/************************************************/
void boardExtInt2Clear()
{ unsigned w=intLock(); 
  *IO_INT_A |= 1;
  intUnlock(w);
}
#if 1
/************************************************/
void boardExtInt3Clear()
{ unsigned w=intLock(); 
  *IO_INT_A |= 2;
  intUnlock(w);
}
#endif
#endif
#ifdef K64RIO
int boardExtInt0Get()
{
	if (*IO_EXT_INT01&1) return 1;
	else return 0;
}

int boardExtInt1Get()
{
	if (*IO_EXT_INT01&2) return 1;
	else return 0;
}

int boardExtInt2Get()
{
	if (*IO_EXT_INT23&1) return 1;
	else return 0;
}

int boardExtInt3Get()
{
	if (*IO_EXT_INT23&2) return 1;
	else return 0;
}

int boardExtInt4Get()
{
	if (*IO_EXT_INT45&1) return 1;
	else return 0;
}

int boardExtInt5Get()
{
	if (*IO_EXT_INT45&2) return 1;
	else return 0;
}

int boardExtInt6Get()
{
	if (*IO_EXT_INT6&1) return 1;
	else return 0;
}

void boardExtInt0Clear()
{
	*IO_EXT_INT01 = (*IO_EXT_INT01&~2)|1;
}

void boardExtInt1Clear()
{
	*IO_EXT_INT01 = (*IO_EXT_INT01&~1)|2;
}

void boardExtInt2Clear()
{
	*IO_EXT_INT23 = (*IO_EXT_INT23&~2)|1;
}

void boardExtInt3Clear()
{
	*IO_EXT_INT23 = (*IO_EXT_INT23&~1)|2;
}

void boardExtInt4Clear()
{
	*IO_EXT_INT45 = (*IO_EXT_INT45&~2)|1;
}

void boardExtInt5Clear()
{
	*IO_EXT_INT45 = (*IO_EXT_INT45&~1)|2;
}

void boardExtInt6Clear()
{
	*IO_EXT_INT6 = (*IO_EXT_INT6&~2)|1;
}

#endif
/******************************************************************************/

/* set/clear desired bit in function register */

static inline void ioFuncSetA(unsigned bit, unsigned clear)
{
    unsigned mask = intLock();
    if(clear)
	*IO_FUNC_A &= ~bit;
    else
	*IO_FUNC_A |= bit;
    intUnlock(mask);
}

static inline void ioFuncSetB(unsigned bit, unsigned clear)
{
    unsigned mask = intLock();
    if(clear)
	*IO_FUNC_B &= ~bit;
    else
	*IO_FUNC_B |= bit;
    intUnlock(mask);
}

/******************************************************************************/

/* enable/disable external timer clock */

void boardTimerExtClkEnable(void)
{
    ioFuncSetA(FUNC_EXT_TIMER_CLK,FALSE);
}

void boardTimerExtClkDisable(void)
{
    ioFuncSetA(FUNC_EXT_TIMER_CLK,TRUE);
}

/******************************************************************************/

/* enable/disable timer0 out to signal 1 of port A */

void boardTimer0OutEnable(void)
{
    ioFuncSetA(FUNC_TIMER0_OUT,FALSE);
}

void boardTimer0OutDisable(void)
{
    ioFuncSetA(FUNC_TIMER0_OUT,TRUE);
}

/******************************************************************************/

/* enable external interrupt */
#ifdef K64RIO
static inline int ioExtIntEnable(unsigned ix, unsigned mode)
{
    unsigned mask;

    if(mode & ~3)
	return -1;

    switch(ix)
    {
	case 0:
	    //ioFuncSetA(FUNC_EXT_INT0,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT01)|= (1<<2) | mode<<4;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ0);
	    break;
	case 1:
	    //ioFuncSetA(FUNC_EXT_INT1,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT01)|= (1<<3) | mode<<6;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ1);
	    break;
	case 2:
	    ioFuncSetA(FUNC_EXT_INT2,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT23)|= (1<<2) | mode<<4;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ0);
	    break;
	case 3:
	    ioFuncSetA(FUNC_EXT_INT3,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT23)|= (1<<3) | mode<<6;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ1);
	    break;
	case 4:
	    ioFuncSetA(FUNC_EXT_INT4,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT45)|= (1<<2) | mode<<4;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ0);
	    break;
	case 5:
	    ioFuncSetA(FUNC_EXT_INT5,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT45)|= (1<<3) | mode<<6;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ1);
	    break;
	case 6:
	    ioFuncSetA(FUNC_EXT_INT6,FALSE);
	    mask = intLock();
	    (*IO_EXT_INT6)|= (1<<2) | mode<<4;
	    intUnlock(mask);
	    boardIvEnable(IV_IRQ0);
	    break;
    }
    return 0;
}
#else
static inline int ioExtIntEnable(unsigned ix, unsigned mode)
{
    unsigned mask;

    if(mode & ~3)
	return -1;

    switch(ix)
    {
	case 0:
	    ioFuncSetA(FUNC_EXT_INT0,FALSE);
	    mask = intLock();
	    *IO_INT_A = (*IO_INT_A & ~(3 << 4)) | 0x5 | (mode << 4);
	    intUnlock(mask);
	    boardIvEnable(IV_GPIO2);
	    break;
	case 1:
	    ioFuncSetA(FUNC_EXT_INT1,FALSE);
	    mask = intLock();
	    *IO_INT_A = (*IO_INT_A & ~(3 << 6)) | 0xa | (mode << 6);
	    intUnlock(mask);
	    boardIvEnable(IV_GPIO3);
	    break;
	case 2:
	    ioFuncSetB(FUNC_EXT_INT0,FALSE);
	    mask = intLock();
	    *IO_INT_B = (*IO_INT_B & ~(3 << 4)) | 0x5 | (mode << 4);
	    intUnlock(mask);
	    boardIvEnable(IV_GPIO10);
	    break;
	case 3:
	    ioFuncSetB(FUNC_EXT_INT1,FALSE);
	    mask = intLock();
	    *IO_INT_B = (*IO_INT_B & ~(3 << 6)) | 0xa | (mode << 6);
	    intUnlock(mask);
	    boardIvEnable(IV_GPIO11);
	    break;
    }
    return 0;
}
#endif
int boardExtInt0Enable(unsigned mode)
{
    return ioExtIntEnable(0,mode);
}

int boardExtInt1Enable(unsigned mode)
{
    return ioExtIntEnable(1,mode);
}

int boardExtInt2Enable(unsigned mode)
{
    return ioExtIntEnable(2,mode);
}
#if !defined(BT83M)
int boardExtInt3Enable(unsigned mode)
{
    return ioExtIntEnable(3,mode);
}

int boardExtInt4Enable(unsigned mode)
{
    return ioExtIntEnable(4,mode);
}

int boardExtInt5Enable(unsigned mode)
{
    return ioExtIntEnable(5,mode);
}

int boardExtInt6Enable(unsigned mode)
{
    return ioExtIntEnable(6,mode);
}
#endif
/******************************************************************************/

/* disable external interrupt */
#ifdef K64RIO
static inline void ioExtIntDisable(unsigned ix)
{
    unsigned mask;
    switch(ix)
    {
	case 0:
	    //ioFuncSetA(FUNC_EXT_INT0,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT01)&= ~(1<<2);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT23&(1<<2))&&!(*IO_EXT_INT45&(1<<2))&&!(*IO_EXT_INT6&(1<<2))) boardIvDisable(IV_IRQ0);
	    break;
	case 1:
	    //ioFuncSetA(FUNC_EXT_INT1,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT01)&= ~(1<<3);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT23&(1<<3))&&!(*IO_EXT_INT45&(1<<3))) boardIvDisable(IV_IRQ1);
	    break;
	case 2:
	    ioFuncSetA(FUNC_EXT_INT2,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT23)&= ~(1<<2);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT01&(1<<2))&&!(*IO_EXT_INT45&(1<<2))&&!(*IO_EXT_INT6&(1<<2))) boardIvDisable(IV_IRQ0);
	    break;
	case 3:
	    ioFuncSetA(FUNC_EXT_INT3,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT23)&= ~(1<<3);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT01&(1<<3))&&!(*IO_EXT_INT45&(1<<3))) boardIvDisable(IV_IRQ1);
	    break;
	case 4:
	    ioFuncSetA(FUNC_EXT_INT3,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT45)&= ~(1<<2);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT23&(1<<2))&&!(*IO_EXT_INT01&(1<<2))&&!(*IO_EXT_INT6&(1<<2))) boardIvDisable(IV_IRQ0);
	    break;
	case 5:
	    ioFuncSetA(FUNC_EXT_INT3,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT45)&= ~(1<<3);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT23&(1<<3))&&!(*IO_EXT_INT01&(1<<3))) boardIvDisable(IV_IRQ1);
	    break;
	case 6:
	    ioFuncSetA(FUNC_EXT_INT3,TRUE);
	    mask = intLock();
	    (*IO_EXT_INT6)&= ~(1<<2);
	    intUnlock(mask);
	    if (!(*IO_EXT_INT23&(1<<2))&&!(*IO_EXT_INT45&(1<<2))&&!(*IO_EXT_INT01&(1<<2))) boardIvDisable(IV_IRQ0);
	    break;
    }
}
#else
static inline void ioExtIntDisable(unsigned ix)
{
  #ifdef BT83M
  volatile unsigned char *a1 = (volatile unsigned char *)IRQ_BASE;
  volatile unsigned char *a2 = IO_INT_A;
  int irq [4] = { IV_IRQ0, IV_IRQ1, IV_GPIO2, IV_GPIO3};
  #else
  volatile unsigned char *a1 = IO_INT_A;
  volatile unsigned char *a2 = IO_INT_B;
  int irq [4] = { IV_GPIO2, IV_GPIO3, IV_GPIO10, IV_GPIO11};
  #endif
    unsigned mask;

    switch(ix)
    {
	case 0:
	    ioFuncSetA(FUNC_EXT_INT0,TRUE);
	    mask = intLock();
	    *a1 &= ~4;
	    intUnlock(mask);
	    boardIvDisable(irq[0]);
	    break;
	case 1:
	    ioFuncSetA(FUNC_EXT_INT1,TRUE);
	    mask = intLock();
	    *a1 &= ~8;
	    intUnlock(mask);
	    boardIvDisable(irq[1]);
	    break;
	case 2:
	    ioFuncSetB(FUNC_EXT_INT0,TRUE);
	    mask = intLock();
	    *a2 &= ~4;
	    intUnlock(mask);
	    boardIvDisable(irq[2]);
	    break;
	case 3:
	    ioFuncSetB(FUNC_EXT_INT1,TRUE);
	    mask = intLock();
	    *a2 &= ~8;
	    intUnlock(mask);
	    boardIvDisable(irq[3]);
	    break;
    }
}
#endif
void boardExtInt0Disable(void)
{
    ioExtIntDisable(0);
}

void boardExtInt1Disable(void)
{
    ioExtIntDisable(1);
}

void boardExtInt2Disable(void)
{
    ioExtIntDisable(2);
}
#if !defined(BT83M)
void boardExtInt3Disable(void)
{
    ioExtIntDisable(3);
}

void boardExtInt4Disable(void)
{
    ioExtIntDisable(4);
}

void boardExtInt5Disable(void)
{
    ioExtIntDisable(5);
}

void boardExtInt6Disable(void)
{
    ioExtIntDisable(6);
}
#endif
/******************************************************************************/

/* external interrupt handler */

static VOIDFUNCPTR boardExtIntRtn[7];
static int boardExtIntArg[7];
static int boardExtIntConncted[7];
#ifdef K64RIO
void boardExtInt1(unsigned ix)
{
    unsigned mask;
    short int i;
    for (i=0; i<7; i+=2)
    {
	if ((i==0)&&(boardExtIntRtn[i])&&(*IO_EXT_INT01&1))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT01 = (*IO_EXT_INT01&~2)|1;
		intUnlock(mask);
		return;
	}

	if ((i==2)&&(boardExtIntRtn[i])&&(*IO_EXT_INT23&1))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT23 = (*IO_EXT_INT23&~2)|1;
		intUnlock(mask);
		return;
	}

	if ((i==4)&&(boardExtIntRtn[i])&&(*IO_EXT_INT45&1))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT45 = (*IO_EXT_INT45&~2)|1;
		intUnlock(mask);
		return;
	}

	if ((i==6)&&(boardExtIntRtn[i])&&(*IO_EXT_INT6&1))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT6 = (*IO_EXT_INT6&~2)|1;
		intUnlock(mask);
		return;
	}
	
    }
    logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);
}

void boardExtInt2(unsigned ix)
{
    unsigned mask;
    short int i;
    for (i=1; i<6; i+=2)
    {
	if ((i==1)&&(boardExtIntRtn[i])&&(*IO_EXT_INT01&2))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT01 = (*IO_EXT_INT01&~1)|2;
		intUnlock(mask);
		return;
	}

	if ((i==3)&&(boardExtIntRtn[i])&&(*IO_EXT_INT23&2))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT23 = (*IO_EXT_INT23&~1)|2;
		intUnlock(mask);
		return;
	}

	if ((i==5)&&(boardExtIntRtn[i])&&(*IO_EXT_INT45&2))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*IO_EXT_INT45 = (*IO_EXT_INT45&~1)|2;
		intUnlock(mask);
		return;
	}
	
    }
    logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);
}
/*
void boardExtInt1(unsigned ix)
{
    unsigned mask;
    short int i, error=0;
    for (i=0; i<3; i+=2)
    {
    	if((boardExtIntRtn[i]) && ((*((i==0)?IO_EXT_INT23:IO_EXT_INT45))&1))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*((i==0)?IO_EXT_INT23:IO_EXT_INT45)|=1;
		intUnlock(mask);
	}
	else if(!(boardExtIntRtn[i]) && ((*((i==0)?IO_EXT_INT23:IO_EXT_INT45))&1)) error=1;
	
    }
    if (error) logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);
}

void boardExtInt2(unsigned ix)
{
    short int i, error=0;
    unsigned mask;
    for (i=1; i<4; i+=2)
    {
    	if((boardExtIntRtn[i]) && ((*((i==1)?IO_EXT_INT23:IO_EXT_INT45))&2))
	{
		boardExtIntRtn[i](boardExtIntArg[i]);
		mask = intLock();
		*((i==1)?IO_EXT_INT23:IO_EXT_INT45)|=2;
		intUnlock(mask);
	}
	else if(!(boardExtIntRtn[i]) && ((*((i==1)?IO_EXT_INT23:IO_EXT_INT45))&2)) error=1;

    }
    if (error) logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);
}
*/
#else
  #ifdef BT83M
void boardExtIntA(unsigned ix)
{
    unsigned mask;

    mask = intLock();
    *(volatile unsigned char *)IRQ_BASE = (*(volatile unsigned char *)IRQ_BASE & ~3) | (1 << ix);
    intUnlock(mask);

    if(boardExtIntRtn[ix])
        boardExtIntRtn[ix](boardExtIntArg[ix]);
    else
        logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);

    ioFlush((volatile unsigned char *)IRQ_BASE);
}

void boardExtIntB(unsigned ix)
{
    unsigned mask;

    mask = intLock();
    *IO_INT_A = (*IO_INT_A & ~3) | (1 << (ix-2));
    intUnlock(mask);

    if(boardExtIntRtn[ix])
        boardExtIntRtn[ix](boardExtIntArg[ix]);
    else
        logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);

    ioFlush(IO_INT_A);
}
  #else
void boardExtIntA(unsigned ix)
{
    unsigned mask;

    mask = intLock();
    *IO_INT_A = (*IO_INT_A & ~3) | (1 << ix);
    intUnlock(mask);

    if(boardExtIntRtn[ix])
	boardExtIntRtn[ix](boardExtIntArg[ix]);
    else
	logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);

    ioFlush(IO_INT_A);
}

void boardExtIntB(unsigned ix)
{
    unsigned mask;

    mask = intLock();
    *IO_INT_B = (*IO_INT_B & ~3) | (1 << (ix-2));
    intUnlock(mask);

    if(boardExtIntRtn[ix])
	boardExtIntRtn[ix](boardExtIntArg[ix]);
    else
	logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);

    ioFlush(IO_INT_B);
}
  #endif
#endif
/******************************************************************************/

/* connect interrupt handler */
static void ioExtIntConnect(unsigned ix, VOIDFUNCPTR rtn, int arg)
{
    unsigned iv = 0; VOIDFUNCPTR irtn = NULL;
#ifdef K64RIO
    short int n;

    switch(ix)
    {
	case 0:
	    iv = IV_IRQ0;
	    irtn = boardExtInt1;
	    break;
	case 1:
	    iv = IV_IRQ1;
	    irtn = boardExtInt2;
	    break;
	case 2:
	    iv = IV_IRQ0;
	    irtn = boardExtInt1;
	    break;
	case 3:
	    iv = IV_IRQ1;
	    irtn = boardExtInt2;
	    break;
	case 4:
	    iv = IV_IRQ0;
	    irtn = boardExtInt1;
	    break;
	case 5:
	    iv = IV_IRQ1;
	    irtn = boardExtInt2;
	    break;
	case 6:
	    iv = IV_IRQ0;
	    irtn = boardExtInt1;
	    break;
    }

    n=(iv==IV_IRQ0)?0:1;

    if(!boardExtIntConncted[n])
    {
	intConnect(iv,(void*)irtn,0);
	boardExtIntConncted[n] = TRUE;
    }

    boardExtIntRtn[ix] = rtn;
    boardExtIntArg[ix] = arg;
#else
  #ifdef BT83M
    switch(ix)
    {
        case 0:
            iv = IV_IRQ0;
            irtn = boardExtIntA;
            break;
        case 1:
            iv = IV_IRQ1;
            irtn = boardExtIntA;
            break;
        case 2:
            iv = IV_GPIO2;
            irtn = boardExtIntB;
            break;
        case 3:
            iv = IV_GPIO3;
            irtn = boardExtIntB;
            break;          
    }
  #else
    switch(ix)
    {
	case 0:
	    iv = IV_GPIO2;
	    irtn = boardExtIntA;
	    break;
	case 1:
	    iv = IV_GPIO3;
	    irtn = boardExtIntA;
	    break;
	case 2:
	    iv = IV_GPIO10;
	    irtn = boardExtIntB;
	    break;
	case 3:
	    iv = IV_GPIO11;
	    irtn = boardExtIntB;
	    break;
    }
  #endif
    if(!boardExtIntConncted[ix])
    {
	intConnect(iv,(void*)irtn,ix);
	boardExtIntConncted[ix] = TRUE;
    }

    boardExtIntRtn[ix] = rtn;
    boardExtIntArg[ix] = arg;
#endif
}

void boardExtInt0Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(0,rtn,arg);
}

void boardExtInt1Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(1,rtn,arg);
}

void boardExtInt2Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(2,rtn,arg);
}
void boardExtInt3Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(3,rtn,arg);
}

#if !defined(BT83M)
void boardExtInt4Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(4,rtn,arg);
}

void boardExtInt5Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(5,rtn,arg);
}

void boardExtInt6Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtIntConnect(6,rtn,arg);
}
#endif
/******************************************************************************/

/* enable external device interrupt */

static inline int ioExtDevIntEnable(unsigned ix, unsigned mode)
{
    unsigned mask, val, iv, mshift;

    if(mode & ~3)
	return -1;

    if(ix == 0)
    {
	val = 0x5 | (mode << 4);
	mshift = 4;
	iv = IV_IRQ0;
    }
    else
    {
	val = 0xa | (mode << 6);
	mshift = 6;
	iv = IV_IRQ1;
    }

    mask = intLock();
    *IO_EXT_INT01 = (*IO_EXT_INT01 & ~(3 << mshift)) | val;
    intUnlock(mask);
    boardIvEnable(iv);
    return 0;
}

int boardExtDevInt0Enable(unsigned mode)
{
    return ioExtDevIntEnable(0,mode);
}

int boardExtDevInt1Enable(unsigned mode)
{
    return ioExtDevIntEnable(1,mode);
}

/******************************************************************************/

/* disable external device interrupt */

static inline void ioExtDevIntDisable(unsigned ix)
{
    unsigned mask, val, iv;

    if(ix == 0)
    {
	val = 0x4;
	iv = IV_IRQ0;
    }
    else
    {
	val = 0x8;
	iv = IV_IRQ1;
    }

    mask = intLock();
    *IO_EXT_INT01 &= ~val;
    intUnlock(mask);
    boardIvDisable(iv);
}

void boardExtDevInt0Disable(void)
{
    ioExtDevIntDisable(0);
}

void boardExtDevInt1Disable(void)
{
    ioExtDevIntDisable(1);
}

/******************************************************************************/

/* external device interrupt handler */

static VOIDFUNCPTR boardExtDevIntRtn[2];
static int boardExtDevIntArg[2];
static int boardExtDevIntConncted[2];

void boardExtDevInt(unsigned ix)
{
    unsigned mask;

    mask = intLock();
    *IO_EXT_INT01 = (*IO_EXT_INT01 & ~3) | (1 << ix);
    intUnlock(mask);

    if(boardExtDevIntRtn[ix])
	boardExtDevIntRtn[ix](boardExtDevIntArg[ix]);
    else
	logMsg("%s: handler is not connected\n",(int)__FUNCTION__,0,0,0,0,0);

    ioFlush(IO_EXT_INT01);
}

/******************************************************************************/

/* connect device interrupt handler */

static void ioExtDevIntConnect(unsigned ix, VOIDFUNCPTR rtn, int arg)
{
    unsigned iv = (ix == 0) ? IV_IRQ0 : IV_IRQ1;

    if(!boardExtDevIntConncted[ix])
    {
	intConnect(iv,(void*)boardExtDevInt,ix);
	boardExtDevIntConncted[ix] = TRUE;
    }

    boardExtDevIntRtn[ix] = rtn;
    boardExtDevIntArg[ix] = arg;
}

void boardExtDevInt0Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtDevIntConnect(0,rtn,arg);
}

void boardExtDevInt1Connect(VOIDFUNCPTR rtn, int arg)
{
    ioExtDevIntConnect(1,rtn,arg);
}

