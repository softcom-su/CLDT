/* intrCtrl.c - interrupt controller for K128 library */

/* forward declaration */

int boardIntDeMux(int unused);
#if OS_MAJOR >= 4
typedef struct {
#if __LP64__
    uintptr_t vector;
#else
    uintptr_t vector;
    int       pad;
#endif
    int    status;      /* interrupt level   */
    int    demux;         /* pad for ease of use  */
} INT_ACTION;
INT_ACTION const intLevelActionTbl[14] =
{
    { .vector = (uintptr_t) 200,           .status = 0xffff,   .demux = 0}, /* sw trap 0 */
    { .vector = (uintptr_t) 201,           .status = 0xffff,   .demux = 0}, /* sw trap 1 */
    { .vector = (uintptr_t) boardIntDeMux,  .status = 0xffff,   .demux = 1}, /* int(0)*/
    { .vector = (uintptr_t) IV_SPURIOUS1,  .status = 0xffff,   .demux = 0}, /* int(1)*/
    { .vector = (uintptr_t) IV_SPURIOUS2,  .status = 0xffff,   .demux = 0}, /* int(2) */
    { .vector = (uintptr_t) IV_SPURIOUS3,  .status = 0xffff,   .demux = 0}, /* int(3)*/
    { .vector = (uintptr_t) IV_SPURIOUS4,  .status = 0xffff,   .demux = 0}, /* int(4)*/
    { .vector = (uintptr_t) IV_TIMER,      .status = 0xffff,   .demux = 0},  /* int(5) */
};
#else
/***********************************************************************/

/* interrupt hash table */

typedef struct {
    u_long intCause;            /* cause of interrupt   */
    u_long bsrTableOffset;      /* index to BSR table   */
    u_long statusReg;           /* interrupt level      */
    u_long pad;                 /* pad for ease of use  */
} INT_ACTION;

#if OS_VERSION < 300
unsigned char intLevelPrioTbl[256];
#endif

#if OS_VERSION >= 300
const INT_ACTION intLevelActionTbl[8]  = {
    {0,0,                       0x3fff,0},      /* sw trap 0 */
    {0,0,                       0x3fff,0},      /* sw trap 1 */
#else
INT_ACTION intLevelActionTbl[8]  = {
#ifdef IV_SWTRAP0_VEC
    {0,IV_SWTRAP0_VEC,		0x3fff,0},	/* sw trap 0 */
    {0,IV_SWTRAP1_VEC,		0x3fff,0},	/* sw trap 1 */
#else
    {0,0,			0x3fff,0},	/* sw trap 0 */
    {0,0,			0x3fff,0},	/* sw trap 1 */
#endif
#endif
#warning  !!!!!! enable nested IRQ for timer !!!!!!!!!!
    {0,(int)boardIntDeMux,      0x3fff,1},   // 0x3f7f,1},      /* INT#0 */  /* Enable clock ! */ /* ??? */
    {0,IV_SPURIOUS1,            0x3fff,0},      /* INT#1 */
    {0,IV_SPURIOUS2,            0x3fff,0},      /* INT#2 */
    {0,IV_SPURIOUS3,            0x3fff,0},      /* INT#3 */
    {0,IV_SPURIOUS4,            0x3fff,0},      /* INT#4 */
    {0,IV_TIMER,                0x3fff,0},      /* INT#5 */
};
#endif
/* interrupt mask */

unsigned boardIntMask = 0;

/* known interrupt names */

static const char* irqName[INT_REQ_MAX] = {
    [INT_REQ_SPI]       = "SPI     ",
    [INT_REQ_UART0]     = "UART0   ",
    [INT_REQ_PCI_S]     = "PCI_S   ",
    [INT_REQ_UART1]     = "UART1   ",
    [INT_REQ_I2C]       = "I2C     ",
    [INT_REQ_TIMER0]    = "TIMER0  ",
    [INT_REQ_DMA]       = "DMA     ",
    [INT_REQ_CP2]       = "CP2     ",
    [INT_REQ_PCI_M]     = "PCI_M   ",
    [INT_REQ_TIMER1]    = "TIMER1  ",
    [INT_REQ_TIMER2]    = "TIMER2  ",
    [INT_REQ_RIO]       = "RIO     ",
    [INT_REQ_DDR2]      = "DDR2    ",
    [INT_REQ_NMI]       = "NMI     ",  /* 15 */
    [16]                = "RESRVED ",
    [17]                = "RESRVED ",
    [18]                = "RESRVED ",
    [19]                = "RESRVED ",
    [20]                = "RESRVED ",
    [21]                = "RESRVED ",
    [22]                = "RESRVED ",
    [23]                = "RESRVED ",
    [24]                = "RESRVED ",
    [25]                = "RESRVED ",
    [26]                = "ExtInt0 ",   /* WDog */
    [27]                = "ExtInt1 ",
    [28]                = "ExtInt2 ",
    [29]                = "ExtInt3 ",   /* User int 0 */
    [30]                = "ExtInt4 ",   /* User int 1 */
    [31]                = "ExtInt5 "
};

unsigned boardIvCount[INT_REQ_MAX];

/******************************************************************************/

/* priority of interrupt requests */

static int lineToPriority[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/******************************************************************************/

/* get a pin with highest priority from cause mask */

static inline int causeHash(int cause)
{
    int highPri, highPin, pin;
    
    for(highPri = -1, highPin = 0, pin = 0; pin < 8; pin++)
    {
        if(cause & (1 << pin))
        {
            if(lineToPriority[pin] > highPri)
            {
                highPri = lineToPriority[pin];
                highPin = pin;
            }
        }
    }
    return highPin;
}

/******************************************************************************/

/* get a pin with highest priority from cause mask */

static void intPrioritySet()
{
    int cause = 0, i;
    
    for(i = 0; i < 0x100; i++)
#if OS_VERSION >= 300
        modLinkPageInternal->mlp_LevelPrioTbl[i] = causeHash(cause++);
#else
        intLevelPrioTbl[i] = causeHash(cause++);
#endif
}

/******************************************************************************/

/* this routine service spurious (no request) interrupts */

void boardSpuriousInt(int line)
{
#ifdef  BAD_INTS_LOGGING
    logMsg("%s: #INT%d\n",(int)__FUNCTION__,line,0,0,0,0);
#endif
}

/***********************************************************************/

/* acknowledge edge interrupt request */

void boardIntAck(unsigned req)
{
    if(req >= INT_REQ_MAX)
        return;
      // kprint("boardIntAck: for req %i *%#x=%#x *%#x <- %#x\n\r", 
	 //  req, SYS_INTREQ_REG, *SYS_INTREQ_REG, SYS_INTREQ_REG, (1 << req));
    *SYS_INTREQ_REG = 1 << req;
     // kprint("boardIntAck: for req %i SYS_INTREQ_REG (*%#x)=%#x\n\r", 
	 //  req, SYS_INTREQ_REG, *SYS_INTREQ_REG);
}

/***********************************************************************/

/* interrupt demultiplexor */

int boardIntDeMux(int unused)
{
    unsigned req = *SYS_INTREQ_REG & boardIntMask, ix;

    for(ix = 0; ix < INT_REQ_MAX; ix++)
    {
        if((1 << ix) & req)
        {
#ifdef  INCLUDE_IV_COUNT
            boardIvCount[ix]++;
#endif
            if (SYS_INTMAP_REG[ix] & 0x20)    /* if edge mode interrupt */
	          boardIntAck (ix);
	    return IV_BASE+ix;
        }
    }

    return IV_SPURIOUS0;
}

/***********************************************************************/

/* set SYS_INTMAP_REG register */

static int boardIntMap(unsigned req, unsigned val)
{
    if(req >= INT_REQ_MAX)
        return -1;
    //kprint ("boardIntMap: req=%i val=%#x  SYS_INTMAP_REG adr %#x\n\r", req, val, &SYS_INTMAP_REG[req]);
    //kprint ("boardIntMap:  boardIntMask = %#x\n\r", boardIntMask);
    
    SYS_INTMAP_REG[req] = val;
    return 0;
}

/***********************************************************************/
/* set interrupt for given mode (level high/low, edge rize/fall) */
/* bit 0 - LEVEL (1) or EDGE (0)
 * bit 1 - FALL / LOW (1) or RISE / HIGH (0)
 * bit 2 - ENABLE (1) or DISABLE (0)
*/

int boardIvEnableMode (unsigned iv, int mode)
{ unsigned mask, val, req = iv-IV_BASE;
  if (req < 0 || req >= INT_REQ_MAX) return -1;
  val = 0;              // map all interrupts to #INT0
  if ((mode & 1) == 0) val |= INT_SENSE_EDGE;
  if ((mode & 2) == 0) val |=  INT_EDGE_FALL;  /* for LVL - same code */ 
  if ((mode & 4) == 0) val |= INT_MAP_DISABLE;
  mask = intLock();
  boardIntMap(req, val);  
  boardIntMask |= 1 << req;
//  kprint("boardIvEnable: boardIntMask = %#x MAP reg=%#x\n\r", boardIntMask, val);
  intUnlock(mask);
  return 0;
}

/*------------------------------------------*/
/* enable interrupts by vector */
/* default mode from intrCtrlInit: INT_LVL_HIGH */
/* but we don't change mode here */
/* (it is for boardExtIntNSet) */

int boardIvEnable(unsigned iv)
{ unsigned mask, req = iv-IV_BASE;
  
  if (req < 0 || req >= INT_REQ_MAX) return -1;
  mask = intLock();
  boardIntMap (req, SYS_INTMAP_REG[req] & 0x30);    // map all interrupts to #INT0
  boardIntMask |= 1 << req;
//  kprint("boardIvEnable: boardIntMask = %#x MAP reg=%#x\n\r", boardIntMask, SYS_INTMAP_REG[req]);
  intUnlock(mask);
  return 0;
}

/***********************************************************************/

/* disable interrupts by vector */

int boardIvDisable(unsigned iv)
{
  unsigned mask, req = iv - IV_BASE;

  if (req < 0 || req >= INT_REQ_MAX) return -1;

   mask = intLock();
   boardIntMap(req, SYS_INTMAP_REG[req] | INT_MAP_DISABLE);  /* preserve int mode */
   boardIntMask &= ~(1 << req);
//    kprint("boardIvDisable: boardIntMask = %#x \n\r", boardIntMask);
   intUnlock(mask);
   return 0;
}

/***********************************************************************/

/* initialize interrupt controller */

void intrCtrlInit(void)
{
    unsigned ix;

    *SYS_INTENABLE_REG = 0;

    intPrioritySet();

    for(ix = 0; ix < INT_REQ_MAX; ix++)
        boardIntMap(ix,INT_MAP_DISABLE);
    *SYS_INTREQ_REG = 0xffffffff;

    intConnect(IV_SPURIOUS0,boardSpuriousInt,0);
    intConnect(IV_SPURIOUS1,boardSpuriousInt,1);
    intConnect(IV_SPURIOUS2,boardSpuriousInt,2);
    intConnect(IV_SPURIOUS3,boardSpuriousInt,3);
    intConnect(IV_SPURIOUS4,boardSpuriousInt,4);

    *SYS_INTENABLE_REG = 1;
}

/***********************************************************************/

/* show interrupt controller state */

void intrCtrlShow(void)
{
    unsigned ix, line, sense, pol, val; const char* name;

    printf("Requests: active=%08x mask=%08x\n",*SYS_INTREQ_REG,boardIntMask);

    for(ix = 0; ix < INT_REQ_MAX; ix++)
    {
        if(!(name = irqName[ix]))
            name = "RESERVED";

        printf("%02d: %s: ",ix,name);

        val = SYS_INTMAP_REG[ix];

        line = val & 0xf;
        sense = val & 0x20;
        pol = val & 0x10;

        if(line == INT_MAP_DISABLE)
        {
            printf("OFF");
        }
        else
        {
            printf("line=%d sense: ",line);
            if(sense == INT_SENSE_LVL)
            {
                printf("%s LEVEL",(pol == INT_LVL_HIGH) ? "HIGH" : "LOW");
            }
            else
            {
                printf("%s EDGE",(pol == INT_EDGE_RISE) ? "RISING" : "FALLING");
            }
        }
#ifdef  INCLUDE_IV_COUNT
        printf(" count=%d\n",boardIvCount[ix]);
#else
        printf("\n");
#endif
    }
}
/***********************************************************************/
short boardWDReset (void)
{
   *GPIO_OUT_REG = *GPIO_OUT_REG & 0x7fff; 
   *GPIO_OUT_REG = *GPIO_OUT_REG | (1 << 15);
   return 0;
}
/***********************************************************************/

#if OS_VERSION >= 300

/* get interrupt vector name */

const char* boardIntVecName(int vec)
{
    const char* name;
    
    if(vec >= IV_BASE && vec < IV_BASE+INT_REQ_MAX)
    {
        if((name = irqName[vec-IV_BASE]))
            return name;
        return "SYSTEM_UNKNOWN";
    }
    
    switch(vec)
    {
        case IV_TIMER:
            return "CPU_TIMER";
        case IV_SPURIOUS0:
            return "SPURIOUS0";
        case IV_SPURIOUS1:
            return "SPURIOUS1";
        case IV_SPURIOUS2:
            return "SPURIOUS2";
        case IV_SPURIOUS3:
            return "SPURIOUS3";
        case IV_SPURIOUS4:
            return "SPURIOUS4";
    }
    return "UNKNOWN";
}
#endif
