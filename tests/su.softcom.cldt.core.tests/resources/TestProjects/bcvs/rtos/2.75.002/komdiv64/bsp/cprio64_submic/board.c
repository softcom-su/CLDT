#include <fenv.h>
#include <arch/board.h>
#include <board.h>
#include <cacheLib.h>
#include <bspversion.h>
#include <logLib.h>
#include <taskLib.h>
#include <ns16x50.h>
#include <bagetSerial.h>

#ifdef	BAD_INTS_LOGGING
#define	BAD_LOG(fmt,a1,a2,a3,a4,a5,a6)	logMsg(fmt,a1,a2,a3,a4,a5,a6)
#else
#define	BAD_LOG(fmt,a1,a2,a3,a4,a5,a6)
#endif

#undef  BOARD_HW_INIT1 
#define BOARD_HW_INIT1 boardHWInit1()

#if defined(CPRIO64)     /* ROM-program type */
   #define PMON
  #if OS_VERSION < 300
    #include "drv/pmon/vl_move.c"
  #endif
#endif
/******************************************************************************/

/* globals */

int mpPSN;				/* processor serial number */
int boardProcNum = -1;			/* processor number of this cpu */
int boardCpu = CPU;			/* system CPU type (MC680x0) */
unsigned boardCpuClockRate = 600;	/* CPU frequency (MHz) */
unsigned boardNsDelayK = 512*600;	/* udelay coeffecient*1024*boardCpuClockRate */
unsigned boardSdramSize;		/* size of onboard SDRAM */
char boardModelName[] = MODEL_NAME;
char boardCpuName[] = CPU_NAME;
unsigned boardTfknTestResult;
unsigned boardStartAddr=0;
unsigned boardFreqRef = BOARD_FREQ_REF;

extern FUNCPTR boardOutStringRtn;	/* routine for kprint */
extern long boardOutStringArg;		/* first argument */
extern unsigned cpuSRdefault;


#ifdef FOR_SUBMICRON
unsigned time_start_boardHWInit1;   //gmu:otl
unsigned time_clear_cp0count_hw1;       //gmu:otl
unsigned time_clear_cp0count_hw2;       //gmu:otl
unsigned trace_submic[1024];
unsigned counter_trace_submic;
unsigned time_sync_VM6_VM7;       //gmu: otl

#ifdef DEBUG_SAVE_INIT_TIME  //debug
char string_buf_for_init_info[4096];  // буфер для хранения информации о инициализации
unsigned counter_buf_for_init;  // указатель на область для записи в буфер инициализации
#endif //DEBUG_SAVE_INIT_TIME

// для того чтобы понять какие функции запускаются при инициализации и в какое время

#define TRACE_SUBMIC_ON  //debug

#ifdef TRACE_SUBMIC_ON
#define TRACE_SUBMIC(a) trace_submic[counter_trace_submic++]=a; if(counter_trace_submic>=1024)counter_trace_submic=0;
#else 
#define TRACE_SUBMIC(a) 
#endif //TRACE_SUBMIC_ON

#else
#define TRACE_SUBMIC(a) 

#endif //FOR_SUBMICRON


#ifdef K64SMP
extern void cp1RegSet(unsigned val);
#endif

#if defined(MPON) || defined(BT83M)
  #include <ns16x50.h>
#endif
#include <drv/intCtrl/intCtrl.c>
#include <drv/intCtrl/io.c>
#include <drv/serial/i2c.c>
#include <drv/pci/pciLib.c>
#if (OS_VERSION > 301) || (OS_VERSION == 301 && OS_RELEASE >= 90)
    #include <drv/timer/mipsR4kTimerNew.c>
#else
    #warning !!!!!!!! <drv/timer/mipsR4kTimer.c> !!!!
    #include <drv/timer/mipsR4kTimer.c>
#endif

#include <drv/timer/sc64Timer.c>
#include <drv/mem/nand.c>
#include <drv/mem/nandTest.c>

#ifdef INCLUDE_RIO
  #include <drv/rio/rio.h>
#endif

/* interrupt hash table */

#if OS_VERSION < 300
u_char intLevelPrioTbl[0x10000];
#endif

typedef struct {
    u_long intCause;		/* cause of interrupt	*/
    u_long bsrTableOffset; 	/* index to BSR table	*/
    u_long statusReg;		/* interrupt level	*/
    u_long pad;			/* pad for ease of use	*/
} INT_ACTION;

#if OS_VERSION >= 300
const INT_ACTION intLevelActionTbl[14] = {
    {0,0,			0x3fff,0},	/* sw trap 0 */
    {0,0,			0x3fff,0},	/* sw trap 1 */
#else
INT_ACTION intLevelActionTbl[14] = {
#ifdef IV_SWTRAP0_VEC
    {0,IV_SWTRAP0_VEC,		0x3fff,0},	/* sw trap 0 */
    {0,IV_SWTRAP1_VEC,		0x3fff,0},	/* sw trap 1 */
#else
    {0,0,			0x3fff,0},	/* sw trap 0 */
    {0,0,			0x3fff,0},	/* sw trap 1 */
#endif
#endif
    {0,(int)boardIntDemux,	0x3fff,1},	/* INT#0 */
#ifdef	INCLUDE_VME
    {0,(int)boardVmeDemux,	0x3fff,2},	/* INT#1 */
#else
    {0,(int)boardIntDemux,	0x3fff,2},	/* INT#1 */
#endif
    {0,(int)boardIntDemux,	0x3fff,3},	/* INT#2 */
    {0,(int)boardIntDemux,	0x3fff,4},	/* INT#3 */
    {0,(int)boardIntDemux,	0x3fff,5},	/* INT#4 */
    {0,IV_TIMER,		0x3fff,0},	/* INT#5 */
};

#ifdef	INCLUDE_VME
  #include "drv/vme/pcivme.c"
#endif	/* INCLUDE_VME */

/******************************************************************************/

#ifdef	INCLUDE_CRC
extern unsigned long buildin_crc32;
extern unsigned long current_crc32;
extern void crcInit(void);

void crcShow()
{
    if(buildin_crc32 == current_crc32)
	printf("OK: buildin and current CRC-32 are equal: 0x%08lx\n",
	    buildin_crc32);
    else
	printf("ERROR: buildin: 0x%08lx, current: 0x%08lx\n",
	    buildin_crc32,current_crc32);
}

void crcInclude()
{
    crcInit();
}
#endif	/* INCLUDE_CRC */

/******************************************************************************/

#if OS_VERSION < 300
#include "configbrd.c"
#endif

/******************************************************************************/

#ifdef TRACE_ON
static unsigned* pTraceBufStart = NULL;
static unsigned* pTraceBufLimit = NULL;

void boardToTraceBuf(unsigned message)
{
    if(!pTraceBufStart)
    {
#if OS_VERSION >= 300
	pTraceBufStart = (void*)(BOARD_UNCACHED_MEM_BASE+BOARD_UNCACHED_MEM_SIZE-VME_MEM_TOTAL_SIZE-TRACE_BUF_SIZE);
#else
	pTraceBufStart = (void*)K0_TO_K1(boardMemTop());
#endif
	pTraceBufLimit = (int*)((char*)pTraceBufStart + TRACE_BUF_SIZE);
	memset((char*)pTraceBufStart,0,TRACE_BUF_SIZE);
    }

    if(pTraceBufStart < pTraceBufLimit)
	*pTraceBufStart++ = message;
}
#else	/* TRACE_ON */

void boardToTraceBuf(unsigned message) {}

#endif	/* TRACE_ON */

/******************************************************************************/

/* calculate Fout value */

static unsigned foutGet(unsigned div, unsigned div_ref, unsigned postD)
{
    return (unsigned long long)boardFreqRef * (div+2) / ((div_ref+2) << postD);
}

/******************************************************************************/

/* calculate Fvco value */

static unsigned fvcoGet(unsigned fout, unsigned postD, unsigned vcod)
{
    return fout << (postD + vcod);
}

/******************************************************************************/

/* get CPU frequency in Hz */

unsigned boardCpuClockRateGet(void)
{
#ifdef VM8
    return CPU_CLK_RATE;
#else
    unsigned val, div, div_ref, postD;
    val = *(volatile unsigned*)(SYSTEM_REGS+0x184);
    div_ref = val & 0xff;
    div = (val >> 8) & 0xff;
    postD = (val >> 16) & 0x3;
    return foutGet(div,div_ref,postD);
#endif
}

/******************************************************************************/

/* calibrate udelay */

unsigned boardUdelayCalibrate()
{
    unsigned v1, v2, cycles = boardCpuClockRate*0x4000;
    v1 = boardCountGet();
    boardNsDelay(cycles);
    v2 = boardCountGet();
    cycles += 64;
    return ((cycles*1024ULL)*boardCpuClockRate) / (v2 - v1);
}

/******************************************************************************/

/* program delay on specified number of microseconds */

void udelay(int us)
{
    unsigned cycles = (unsigned long long)us*boardNsDelayK/1024 + 8;
#if OS_VERSION >= 300
    boardNsDelay(cycles);
#else
    cpuCycleDelay(cycles);
#endif
}

/******************************************************************************/

/* disable SDRAM ECC */

void boardECCDisable(void)
{
    *(volatile unsigned*)(SYSTEM_REGS+0x174) &= ~0x31;
}

/******************************************************************************/

/* default ECC handler */

void boardECCDefHadler(unsigned status, unsigned paddr)
{
    logMsg("%s: status=%x addr=%08x\n",(int)__FUNCTION__,status,paddr,0,0,0);
}

/******************************************************************************/

/* ECC handler */

VOIDFUNCPTR boardECCHadler = boardECCDefHadler;

/******************************************************************************/

/* ECC interrupt handler */

void boardECCIntr(int unused)
{
    unsigned val = *(volatile unsigned*)(SYSTEM_REGS+0x174);

    if(val & 0x40)
    {
	if(boardECCHadler)
	    boardECCHadler(val,*(volatile unsigned*)(SYSTEM_REGS+0x178));
	*(volatile unsigned*)(SYSTEM_REGS+0x174) = val;
    }
}

/******************************************************************************/

/* inherit ECC mode, enable ECC interrupt */

void boardECCIntEnable(void)
{
    unsigned val;
    intConnect(IV_SC,(void*)boardECCIntr,0);
    val = *(volatile unsigned*)(SYSTEM_REGS+0x174);
    *(volatile unsigned*)(SYSTEM_REGS+0x174) = (val | 0x40) & ~0x30;
    boardIvEnable(IV_SC);
}

/******************************************************************************/

/* enable branch prediction */

void boardBPEnable(void)
{
    int mask = intLock();
    boardConfigSet(boardConfigGet() & ~0x10000);
    *(volatile unsigned*)(SYSTEM_REGS+0x154) &= ~0x8000;
    intUnlock(mask);
}

/******************************************************************************/

/* disable branch prediction */

void boardBPDisable(void)
{
    int mask = intLock();
    *(volatile unsigned*)(SYSTEM_REGS+0x154) |= 0x8000;
    intUnlock(mask);
}

/******************************************************************************/

/* get top of system memory */

#if OS_VERSION < 300

char* boardMemTop(void)
{
    unsigned sdramSize = SDRAM_SIZE;
    unsigned reservedSize = 0;

#ifdef  TRACE_ON
    reservedSize +=  TRACE_BUF_SIZE;
#endif
#ifdef	INCLUDE_VME
    reservedSize += VME_MEM_TOTAL_SIZE;
#elif 	defined(INCLUDE_RIO) && defined(MPON)
    reservedSize += 0x1000000;
#endif

    return (char*)(LOCAL_MEM_LOCAL_ADRS + sdramSize - reservedSize);
}

#endif

#if OS_VERSION >= 300

/******************************************************************************/

/* convert address routines */

void* boardUncachedToCached(void* addr)
{
    unsigned p32 = (unsigned)addr;

    if(p32 < BOARD_UNCACHED_MEM_BASE || p32 > BOARD_UNCACHED_MEM_BASE + BOARD_UNCACHED_MEM_SIZE)
	return NULL;
    return (void*)((p32 - BOARD_UNCACHED_MEM_BASE) + BOARD_CACHED_MEM_BASE);
}

void* boardCachedToUncached(void* addr)
{
    unsigned p32 = (unsigned)addr;

    if(p32 < BOARD_CACHED_MEM_BASE || p32 > BOARD_CACHED_MEM_BASE + BOARD_UNCACHED_MEM_SIZE)
	return NULL;
    return (void*)((p32 - BOARD_CACHED_MEM_BASE) + BOARD_UNCACHED_MEM_BASE);
}

#else

/******************************************************************************/

/* convert address routines */

void* boardUncachedToCached(void* addr)
{
    return (void*)K1_TO_K0(addr);
}

void* boardCachedToUncached(void* addr)
{
    return (void*)K0_TO_K1(addr);
}

#endif

/******************************************************************************/

/* oc2000 extract from this string host name and FTP user name and password */

char* getAddrBootString(void)
{
    return (char*)0x80000700;
}

/******************************************************************************/

/* display board logo */

#if OS_VERSION < 300

void boardLogo(void)
{
    printf(
    "   board:  %s      Memory Size: %#x\n"
    "   CPU: %s       Creation date: %s\n"
    "   BSP version %i.%02i%s    BSP library compiled for OS version %s\n\r",
	boardModelName,	boardMemTop()-(char*)LOCAL_MEM_LOCAL_ADRS,
	boardCpuName,IMAGE_DATE,
	BSP_VERSION_MAJOR,BSP_VERSION_MINOR,
#if BSP_VERSION_BETA==1
           "beta ",
#else
           "     ",
#endif
        bspLibVersion());
}

#else

void boardLogo(void)
{
    printf(
    "   BSP version %i.%02i%s    build %s\n"
    "   Board:  %s     CPU: %s                "
#ifdef BT211
    "VME processor #%i\n"
#else
    "\n"
#endif
    "   Kernel image creation date: %s \n",
	BSP_VERSION_MAJOR,BSP_VERSION_MINOR,
#if BSP_VERSION_BETA==1
           "beta ",
#else
           "     ",
#endif
        bspLibVersion(),
	boardModelName, boardCpuName, 
#ifdef BT211
        boardProcNumGet(),
#endif
	IMAGE_DATE
       );
}
#endif

/******************************************************************************/

/* get board model name */

char* boardModel(void)
{
    return boardModelName;
}

/*******************************************************************************
*
* boardToEntry - transfer control to some address
*
* This routine jumps to absolute address
*
*/

int boardToEntry(int* startAddr, int arg)
{
    FUNCPTR adr = (FUNCPTR)startAddr;

    cacheDataInvalidate(NULL,0);
#if OS_VERSION < 300
    cacheInstInvalidate(NULL,0);
#endif

    (*adr)(arg);

    return 0;     /* in case we ever continue from entry */
}

/******************************************************************************/
#if !defined(BOOTER) && defined(INCLUDE_NVRAM) && defined(BT211)

/* Try to restore PRIMES data if it is saved in PRIMES_DATA_SAVE_ADRS and violated in PRIMES_DATA */

void restorePrimesData (void)
{
   if ((*(int *)PRIMES_DATA != 0x12345678 || *((int *)PRIMES_DATA+1) != ~0x12345678 
       || *((int *)PRIMES_DATA+2) != 0xaaaa5555 || *((int *)PRIMES_DATA+3) != ~0xaaaa5555)
       && *(int *)PRIMES_DATA_SAVE_ADRS == 0x12345678 
       && *((int *)PRIMES_DATA_SAVE_ADRS+1) == ~0x12345678 
       && *((int *)PRIMES_DATA_SAVE_ADRS+2) == 0xaaaa5555
       && *((int *)PRIMES_DATA_SAVE_ADRS+3) == ~0xaaaa5555
      )
        memmove ((void *)PRIMES_DATA, (void *)PRIMES_DATA_SAVE_ADRS, PRIMES_DATA_SIZE);
}
#endif
/*--------------------------------------------------------------------------*/
/* ^X and r call this routine */

#if OS_VERSION >= 300
void boardCpuReboot()
#else
void cpuReboot()
#endif
{
#if OS_VERSION >= 300
    MODCALL_LOCK(
#endif
    kprint("%s ... ",__FUNCTION__);
    pciDevsDisable(TRUE);
#ifdef INCLUDE_RIO
    rioShutdown();
#endif
#if OS_VERSION < 300
#ifdef BOOTER
    memmove((void *)PRIMES_DATA, (void *)PRIMES_DATA_SAVE_ADRS, PRIMES_DATA_SIZE);
#elif  defined(INCLUDE_NVRAM) && defined(BT211)
    restorePrimesData();
#endif
	intLock();
#endif
	boardToEntry((int*)WARM_REBOOT,0);
#if OS_VERSION >= 300
    );
#endif
}

/******************************************************************************/

/* boardProcNumGet - get the processor number */

int boardProcNumGet()
{
    return boardProcNum;
}

/******************************************************************************/

/* detect module mode: 0 - regular, 1 - technological */

#if OS_VERSION < 300
  #if !defined(RS6M)
int boardRunMode(void)
{
    return ((*(unsigned char *)0xbb400250 & 0x20) >> 5) ^ 1;
}
  #endif
#endif

/******************************************************************************
*
* boardProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
*/

void boardProcNumSet(int procNum)
{
    boardProcNum = procNum;
}

/******************************************************************************/

/* get firmware test result */

int boardFWTestResult(void)
{
   return boardTfknTestResult;
}

/******************************************************************************/

/* this routine called from kprint */

#if OS_VERSION < 300

int tty_outstring(char* s)
{
    int res = -1;
    if(boardOutStringRtn && boardOutStringRtn != (void*)(-1))
	res = boardOutStringRtn(boardOutStringArg,s);
#ifdef BOOTER_CONSOLE_PORT2
    if(boardOutStringRtn2 && boardOutStringRtn2 != (void*)(-1))
	res = boardOutStringRtn2(boardOutStringArg2,s);
#endif
    return res;
}

#endif

/******************************************************************************/

/* common serial port initialization routine */

#ifdef BT211
#include <bagetSerial.h>
extern BAGET_SERIAL_CTRL* pSysBsCtrl;
#endif

void* _auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed, int intEnable)
{
#ifdef BT211
    if(channel >= 0 && channel < 5)
    {
	return _bagetAuxDevInit(pSysBsCtrl,name,channel,rdSize,wrSize,speed,intEnable);
    }
    else if(channel == 5 || channel == 6)
    {
	return _sc64AuxDevInit(name,channel-5,rdSize,wrSize,speed, intEnable);
    }
#else
    if (channel == 0 || channel == 1)
	return _sc64AuxDevInit(name,channel,rdSize,wrSize,speed, intEnable);
#endif
    return NULL;
}

void* auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed)
{
    return _auxDevInit(name,channel,rdSize,wrSize,speed,TRUE);
}

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

/* calculate memory sum, it used in old flash library */

unsigned boardMemSum(unsigned* ptr, int nLongs)
{
    unsigned sum, ix;

    for(sum = nLongs, ix = 0; ix < nLongs; ix++)
	sum += ptr[ix];

    return sum;
}

/******************************************************************************/

/* return TRUE if interrupt vector known */

int boardIvKnown(int iv)
{
    switch(iv)
    {
	case IV_LOW_VEC ... (USER_VEC_START - 1):
#if OS_VERSION < 300 && defined(IV_FPA_BASE_VEC) && defined(IV_SWTRAP0_VEC)
	case IV_FPA_BASE_VEC ... IV_FPA_PREC_VEC:
	case IV_SWTRAP0_VEC:
	case IV_SWTRAP1_VEC:
#endif
	case IV_BASE ... IV_TIMER:
	    return TRUE;
    }
    return FALSE;
}

/******************************************************************************/

int boardHWInit1()
{
    TRACE(TR_START,0xb0000);

#ifdef FOR_SUBMICRON
    time_start_boardHWInit1 = boardCountGet();  // gmu: otl
    time_clear_cp0count_hw1 = 0;       // gmu: otl
    counter_trace_submic = 0;  
    time_sync_VM6_VM7 = 0;
#endif

#if OS_VERSION >= 300
                                          /* bit 31 - COP1X floating instructions */
    modLinkPageInternal->mlp_cpuSRDefault = 0x80000000|SR_FR|SR_CU1|SR_CU0|SR_IEC|SR_KSSUPER|SR_PX|SR_KX|SR_UX|SR_SX|0x8c00;
   #if OS_VERSION >= 306
    modLinkPageInternal->mlp_cpuFpuSRDefault = FCSR_FSbit << 24;        /* set FS bit for no IEEE754 mode */
    #endif
#endif

#ifdef	INCLUDE_CRC
    if(buildin_crc32 != current_crc32)
    {
	TRACE(TR_START,0xb0001);
	kprint("CRC ERROR: buildin - 0x%08lx, current - 0x%08lx\n\r",
	    buildin_crc32,current_crc32);
	TRACE(TR_START,0xb0002);
    }
#endif

#if defined(PMON) && OS_VERSION < 300
    move_pmons_vars ();
#endif

#if defined(MPON)
    if(*(volatile unsigned char*)(UART0_BASE+3) & LCR_STOP2)
    {
      boardFreqRef = 25000000;
	TRACE(TR_START,0xb0004);
    }
    // kprint ("boardHWInit1: boardFreqRef=%i\n\r", boardFreqRef);

    boardTfknTestResult = *(volatile unsigned*)(SYSTEM_REGS+0x158);
    TRACE(TR_START,0xb0008);
#endif

    /* fill interrupt hash table and set level masks */

    intPrioritySet();
    TRACE(TR_START,0xb0010);

    boardPioInit();
    
    /* initiating the interrupt controller */

    intrCtrlInit();
    TRACE(TR_START,0xb0040);

    /* timer library initialization */

    sc64TimerInit();
    TRACE(TR_START,0xb0048);

    /* detect cpu frequency */

    boardCpuClockRate = boardCpuClockRateGet() / 1000000;
    TRACE(TR_START,0xb0050);

    /* branch prediction on/off */
#if defined(MPON)
#ifdef	DISABLE_BRANCH_PREDICTION
    boardBPDisable();
    TRACE(TR_START,0xb0052);
#else
    if((*(volatile unsigned*)(SYSTEM_REGS+0x154) >> 16) >= 0xa025)
    {
	boardBPEnable();
	TRACE(TR_START,0xb0054);
    }
#endif
#endif
    /* calibrate udelay */

    boardNsDelayK = boardUdelayCalibrate();
    TRACE(TR_START,0xb0058);

    /* connect ISR's */

    intConnect(IV_PCI_INTA,pciInt,0);
    intConnect(IV_PCI_INTB,pciInt,1);
    intConnect(IV_PCI_INTC,pciInt,2);
    intConnect(IV_PCI_INTD,pciInt,3);
    TRACE(TR_START,0xb0060);

    /* initialize system clock */

    intConnect(IV_TIMER,(vfuncptr_t)boardClockInt,0);
#if (OS_VERSION < 301) || (OS_VERSION == 301 && OS_RELEASE < 90)
    boardClockInit(BOARD_TIMER_HZ);
#endif
#if OS_VERSION >= 300
    modClockInit(CLOCK_REALTIME,BOARD_TIMER_HZ,modCLOCK_FLAG_PROCESS_CPUTIME|modCLOCK_FLAG_THREAD_CPUTIME);
#if (OS_VERSION > 301) || (OS_VERSION == 301 && OS_RELEASE >= 90)
    boardClockEnable();
#else
    boardClockConnect((funcptr_t)modClockTick,CLOCK_REALTIME);
#endif
#endif
#if (OS_VERSION < 300) || (OS_VERSION == 301 && OS_RELEASE >= 90 && OS_RELEASE <= 114)

#ifdef FOR_SUBMICRON
    time_clear_cp0count_hw1 = boardCountGet();       // gmu: otl
#endif
    boardCountSet(0);
    boardCompareSet(0);

    TRACE(TR_START,0xb0080);
#endif    

#if OS_VERSION < 300
    cp1FCSRSet(cp1FCSRGet() | ( FCSR_FSbit << 24 ) );  /* set FS bit  in FCSR reg*/
    cpuSRdefault=0x34008c01;
    intUnlock(cpuSRdefault);
#endif
#ifdef K64SMP
  #ifndef SQRTD_OFF                 /* K64SMP problem with SQRTD handling */
    if ((*(unsigned *)(SYSTEM_REGS + 0x154) & 0xffff0000) == 0xa0270000)  /* CPU with no SQRTD by default */
       cp1RegSet(0x0);                                                   /* switch on SQRTD */
  #endif
#endif
    TRACE(TR_START,0xbffff);
    return 0;
}

/******************************************************************************/

/* show CPU configuration */

void cpuShow(void)
{
    unsigned val, bits, div, div_ref, postD, vcod, fout;
#if defined(MPON) || defined(K64RIO) || defined(BT83M) 
    unsigned revision;
#endif

    val = cp0PrIDGet();
    printf("CPU Clock %d MHz, Fref %d MHz, Processor ID: %x.%x\n\n",
	boardCpuClockRate, boardFreqRef/1000000, (val >> 8) & 0xff, val & 0xff);
    val = cp0ConfigGet();
    printf("CP0 Config0=0x%08x\n",val);
    bits = val >> 31;
    printf("    [31]    = %x:\tConfig1 Register %s\n",bits,bits ? "Available" : "Not Available");
#if defined(MPON) || defined(K64RIO) || defined(BT83M) 
    revision = ((*(volatile unsigned*)(SYSTEM_REGS+0x154)) >> 16) & 0xffff;    /* CPU revision */
    if (revision >= 0xa025)
    { bits = (val >> 28) & 1;
      printf("    [28]    = %x:\tStreaming loads %s\n",bits,!bits ? "Enable" : "Disable");
    }
#else
    bits = (val >> 29) & 1;
    printf("    [29]    = %x:\tShort Loops %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 28) & 1;
    printf("    [28]    = %x:\tSpeculative Processing %s\n",bits,!bits ? "Enable" : "Disable");
#endif    
    bits = (val >> 27) & 1;
    printf("    [27]    = %x:\tL1 Hit-Invalidate Cache Lookup %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 26) & 1;
    printf("    [26]    = %x:\tMachine Check TLB Shutdown Exception %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 25) & 1;
    printf("    [25]    = %x:\tLOAD/STORE Access %s Address\n",bits,bits ? "Physical" : "Virtual");
    bits = (val >> 24) & 1;
    printf("    [24]    = %x:\tLOAD/STORE Cache Access %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 23) & 1;
    printf("    [23]    = %x:\tDSP/VLIW %s\n",bits,!bits ? "Enable!!!" : "Disable");
    bits = (val >> 22) & 1;
    printf("    [22]    = %x:\tMemory Coherency %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 21) & 1;
    printf("    [21]    = %x:\tL2 Cache %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 20) & 1;
    printf("    [20]    = %x:\tInstruction Scheduling %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 19) & 1;
    printf("    [19]    = %x:\tCache %s\n",bits,!bits ? "Enable" : "Disable!!!");
    bits = (val >> 18) & 1;
    printf("    [18]    = %x:\tTLB %s\n",bits,!bits ? "Enable" : "Disable!!!");
    bits = (val >> 17) & 1;
    printf("    [17]    = %x:\tSuperscalar %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 16) & 1;
    printf("    [16]    = %x:\tBranch Prediction %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 15) & 1;
    printf("    [15]    = %x:\tByte Order %s\n",bits,bits ? "Big Endian" : "Little Endian!!!");
    {
	static char* type[4] = {
	    "MIPS32", "MIPS64 32-bit segments", "MIPS64 32-bit and 64-bit segments", "Reserved"
	};
	bits = (val >> 13) & 0x3;
	printf("    [14:13] = %x:\tArchitecure Type: %s\n",bits,type[bits]);
    }
    bits = (val >> 10) & 0x7;
    printf("    [12:10] = %x:\tArchitecure Revision: %d\n",bits,bits+1);
    {
	static char* type[8] = {
	    "None", "Standard TLB", "Standard BAT", "Standard fixed mapping"
	};

	bits = (val >> 7) & 0x7;
	printf("    [9:7]   = %x:\tMMU Type: %s\n",bits,type[bits] ? type[bits] : "Reserved");
    }
    bits = (val >> 3) & 1;
    printf("    [3]     = %x:\tCache Instruction Is %sVirtual\n",bits,bits ? "" : "Not ");
    {
	static const char* attr[] = {
	    "Write Through Without Write-Allocate, Secondary Bypass, Non-Blocking",
	    "Write Through With Write-Allocate, Secondary Bypass, Non-Blocking",
	    "Uncached, Blocking",
	    "Write Through With Write-Allocate, Secondary Writeback, Non-Blocking",
	    "Write Through Without Write-Allocate, Secondary Bypass, Non-Blockin",
	    "Write Through With Write-Allocate, Secondary Bypass, Non-Blocking",
	    "Uncached, Non-Blocking",
	    "Write Through With Write-Allocate, Secondary Writeback, Non-Blocking"
	};
	bits = val & 0x7;
	printf("    [2:0]   = %x:\tKSEG0 Coherency Algorithm: %s\n\n",bits,attr[bits]);
    }

    val = cp0Config1Get();
    printf("CP0 Config1=0x%08x\n",val);
    bits = val >> 31;
    printf("    [31]    = %x:\tConfig2 Register %s\n",bits,bits ? "Available" : "Not Available");
    bits = (val >> 25) & 0x3f;
    printf("    [30:25] = %x:\tMMU Size %d Entries\n",bits,bits+1);
    bits = (val >> 22) & 0x7;
    printf("    [24:22] = %x:\tI-Cache Size %d Lines Per Way\n",bits,64 << bits);
    bits = (val >> 19) & 0x7;
    printf("    [21:19] = %x:\tI-Cache Line Size %d Bytes\n",bits,bits ? (4 << (bits-1)) : 0);
    bits = (val >> 16) & 0x7;
    printf("    [18:16] = %x:\tI-Cache Associative: ",bits);
    if(!bits)
	printf("Direct Mapped\n");
    else
	printf("%d-Way Set\n",bits+1);
    bits = (val >> 13) & 0x7;
    printf("    [15:13] = %x:\tD-Cache Size %d Lines Per Way\n",bits,64 << bits);
    bits = (val >> 10) & 0x7;
    printf("    [12:10] = %x:\tD-Cache Line Size %d Bytes\n",bits,bits ? (4 << (bits-1)) : 0);
    bits = (val >> 7) & 0x7;
    printf("    [9:7]   = %x:\tD-Cache Associative: ",bits);
    if(!bits)
	printf("Direct Mapped\n");
    else
	printf("%d-Way Set\n",bits+1);
    bits = (val >> 6) & 1;
    printf("    [6]     = %x:\tCoprocessor 2 %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 5) & 1;
    printf("    [5]     = %x:\tMDMX ASE %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 4) & 1;
    printf("    [4]     = %x:\tPerfomance Counter %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 3) & 1;
    printf("    [3]     = %x:\tWatch Register %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 2) & 1;
    printf("    [2]     = %x:\tMIPS 16e %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 1) & 1;
    printf("    [1]     = %x:\tEJTAG %sAvailable\n",bits,bits ? "" : "Not ");
    bits = val & 1;
    printf("    [0]     = %x:\tFPU %sAvailable\n\n",bits,bits ? "" : "Not ");

    val = cp0Config2Get();
    printf("CP0 Config2=0x%08x\n",val);
    bits = val >> 31;
    printf("    [31]    = %x:\tConfig3 Register %s\n",bits,bits ? "Available" : "Not Available");
    bits = (val >> 28) & 0x7;
    printf("    [30:28] = %x:\tL3 Control/Status\n",bits);
    bits = (val >> 24) & 0xf;
    printf("    [27:24] = %x:\tL3 Cache Size %d Lines Per Way\n",bits,64 << bits);
    bits = (val >> 20) & 0xf;
    printf("    [23:20] = %x:\tL3 Cache Line Size %d Bytes\n",bits,bits ? (4 << (bits-1)) : 0);
    bits = (val >> 16) & 0xf;
    printf("    [19:16] = %x:\tL3 Cache Associative: ",bits);
    if(!bits)
	printf("Direct Mapped\n");
    else
	printf("%d-Way Set\n",bits+1);
    bits = (val >> 12) & 0x7;
    printf("    [15:12] = %x:\tL2 Control/Status\n",bits);
    bits = (val >> 8) & 0xf;
    printf("    [11:8]  = %x:\tL2 Cache Size %d Lines Per Way\n",bits,64 << bits);
    bits = (val >> 4) & 0xf;
    printf("    [7:4]   = %x:\tL2 Cache Line Size %d Bytes\n",bits,bits ? (4 << (bits-1)) : 0);
    bits = val & 0xf;
    printf("    [3:0]   = %x:\tL2 Cache Associative: ",bits);
    if(!bits)
	printf("Direct Mapped\n\n");
    else
	printf("%d-Way Set\n\n",bits+1);

    val = cp0Config3Get();
    printf("CP0 Config3=0x%08x\n",val);
    bits = val >> 31;
    printf("    [31]    = %x:\tConfig4 Register %s\n",bits,bits ? "Available" : "Not Available");
    bits = (val >> 7) & 1;
    printf("    [7]     = %x:\tLarge Physical Address and PageGrain Register %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 6) & 1;
    printf("    [6]     = %x:\tExternal Interrupt Controller %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 5) & 1;
    printf("    [5]     = %x:\tInterrupt Vector %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 4) & 1;
    printf("    [4]     = %x:\t1KB Page and PageGrain Register %sAvailable\n",bits,bits ? "" : "Not ");
    bits = (val >> 1) & 1;
    printf("    [1]     = %x:\tSmartMIPS ASE %sAvailable\n",bits,bits ? "" : "Not ");
    bits = val & 1;
    printf("    [0]     = %x:\tTrace Logic %sAvailable\n\n",bits,bits ? "" : "Not ");

    val = cp0StatusGet();
    printf("CP0 Status=0x%08x\n",val);
    bits = val >> 31;
    printf("    [31]    = %x:\tMIPS IV Instructions %s in user mode\n",bits,bits ? "usable" : "unusable");
    bits = (val >> 30) & 1;
    printf("    [30]    = %x:\tCP2 %s\n",bits,bits ? "Usable!!!" : "Unusable");
    bits = (val >> 29) & 1;
    printf("    [29]    = %x:\tCP1 %s\n",bits,bits ? "Usable" : "Unusable!!!");
    bits = (val >> 28) & 1;
    printf("    [28]    = %x:\tCP0 %s\n",bits,bits ? "Usable" : "Unusable!!!");
    bits = (val >> 26) & 1;
    printf("    [26]    = %x:\t%d Floating-Point Registers\n",bits,16 << bits);
    bits = (val >> 24) & 1;
    printf("    [24]    = %x:\tMDMX %s\n",bits,!bits ? "Disable" : "Enable");
    bits = (val >> 22) & 1;
    printf("    [22]    = %x:\tException vectors in %s\n",bits,bits ? "ROM!!!" : "RAM");
    bits = (val >> 21) & 1;
    printf("    [21]    = %x:\tTLB Shutdown %s\n",bits,!bits ? "Disable" : "Enable");
    bits = (val >> 17) & 1;
    printf("    [17]    = %x:\tL2 Hamming Checking %s\n",bits,!bits ? "Disable" : "Enable");
    bits = (val >> 16) & 1;
    printf("    [16]    = %x:\tL2 Hamming Correction %s\n",bits,!bits ? "Disable" : "Enable");
    bits = (val >> 8) & 0xff;
    printf("    [15:8]  = %x:\tInterrupts Unmasked:",bits);
    if(!bits)  printf(" NONE");
    if(bits & 0x01) printf(" SW0");
    if(bits & 0x02) printf(" SW1");
    if(bits & 0x04) printf(" INT#0");
    if(bits & 0x08) printf(" INT#1");
    if(bits & 0x10) printf(" INT#2");
    if(bits & 0x20) printf(" INT#3");
    if(bits & 0x40) printf(" INT#4");
    if(bits & 0x80) printf(" INT#5/TIMER");
    printf("\n\n");

    val = cp0CauseGet();
    printf("CP0 Cause=0x%08x\n",val);
    bits = (val >> 30) & 1;
    printf("    [30]    = %x:\tTimer Interrupt %sPended\n",bits,bits ? "" : "Not ");
    bits = (val >> 27) & 1;
    printf("    [27]    = %x:\tTimer Count %s\n",bits,bits ? "Disable" : "Enable");
    bits = (val >> 26) & 1;
    printf("    [26]    = %x:\tPerfomance Counter Interrupt %sPended\n",bits,bits ? "" : "Not ");
    bits = (val >> 23) & 1;
    printf("    [23]    = %x:\t%s Interrupt Vector Used\n",bits,bits ? "Special" : "Common");
    bits = (val >> 22) & 1;
    printf("    [22]    = %x:\tWatch Interrupt %sPended\n",bits,bits ? "" : "Not ");
    bits = (val >> 8) & 0xff;
    printf("    [15:8]  = %x:\tInterrupts Pended:",bits);
    if(!bits)  printf(" NONE");
    if(bits & 0x01) printf(" SW0");
    if(bits & 0x02) printf(" SW1");
    if(bits & 0x04) printf(" INT#0");
    if(bits & 0x08) printf(" INT#1");
    if(bits & 0x10) printf(" INT#2");
    if(bits & 0x20) printf(" INT#3");
    if(bits & 0x40) printf(" INT#4");
    if(bits & 0x80) printf(" INT#5/TIMER");
    printf("\n\n");

    val = *(volatile unsigned*)(SYSTEM_REGS+0x17c);
    printf("MISC R17C=0x%08x\n",val);
    bits = (val >> 25) & 1;
    printf("    [25]    = %x:\tPCI Bound 4K %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 24) & 1;
    printf("    [24]    = %x:\tAXI prefetch %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 22) & 1;
    printf("    [22]    = %x:\tPCI32 to AXI64 %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 21) & 1;
    printf("    [21]    = %x:\tMemory write merging %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 20) & 1;
    printf("    [20]    = %x:\tB_ERRORS out\n",bits);
    bits = (val >> 19) & 1;
    printf("    [19]    = %x:\tWDT -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 18) & 1;
    printf("    [18]    = %x:\tB_ERRORS in\n",bits);
    bits = (val >> 17) & 1;
    printf("    [17]    = %x:\tPCI Arbiter %s\n",bits,!bits ? "Enabled" : "Disabled");
    bits = (val >> 16) & 1;
    printf("    [16]    = %x:\tRIO Reset %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 15) & 1;
    printf("    [15]    = %x:\tShared RIO %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 14) & 1;
    printf("    [14]    = %x:\tMem Write Invalidate Prefetch Buffers  %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 13) & 1;
    printf("    [13]    = %x:\tMemory Prefetch Buffers %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 12) & 1;
    printf("    [12]    = %x:\tPCI Read FIFO %sEmpty\n",bits,bits ? "" : "Not ");
    bits = (val >> 11) & 1;
    printf("    [11]    = %x:\tPCI Write FIFO %sEmpty\n",bits,bits ? "" : "Not ");
    bits = (val >> 10) & 1;
    printf("    [10]    = %x:\tTwo AXI Write FIFO %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 9) & 1;
    printf("    [9]     = %x:\tPCI Host Detected Parity Error -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 8) & 1;
    printf("    [8]     = %x:\tPCI Host Signaled System Error -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 7) & 1;
    printf("    [7]     = %x:\tPCI Host Recevied Mater Abort -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 6) & 1;
    printf("    [6]     = %x:\tECC Unrecoverable Error -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 5) & 1;
    printf("    [5]     = %x:\tRead BUS Error -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 4) & 1;
    printf("    [4]     = %x:\tWrite BUS Error -> B_ERRORS %s\n",bits,bits ? "Enable" : "Disable");
    bits = (val >> 2) & 1;
    printf("    [2]     = %x:\tPCI Host Recevied Mater Abort value\n",bits);
    bits = (val >> 1) & 1;
    printf("    [1]     = %x:\tPCI Host Recevied Target Abort value\n",bits);
    bits = (val >> 0) & 1;
    printf("    [0]     = %x:\tPCI Host Signaled Target Abort value\n",bits);
    printf("\n");

    val = *(volatile unsigned*)(SYSTEM_REGS+0x154);
    printf("R154=0x%08x\n",val);
    bits = (val >> 16) & 0xffff;
    printf("    [31:16] = %x:\tChip/Firmware Version\n",bits);
    bits = (val >> 15) & 1;
    printf("    [15]    = %x:\tBranch Prediction %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 14) & 1;
    printf("    [14]    = %x:\tL2 Cache %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 10) & 1;
    printf("    [10]    = %x:\tCache %s\n",bits,!bits ? "Enable" : "Disable!!!");
    bits = (val >> 9) & 1;
    printf("    [9]     = %x:\tTLB %s\n",bits,!bits ? "Enable" : "Disable!!!");
    bits = (val >> 8) & 1;
    printf("    [8]     = %x:\tSuperscalar %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 7) & 1;
    printf("    [7]     = %x:\tMemory Coherency %s\n",bits,!bits ? "Enable" : "Disable");
    bits = (val >> 6) & 1;
    printf("    [6]     = %x:\tDSP/VLIW %s\n",bits,!bits ? "Enable!!!" : "Disable");
    bits = val & 0x3f;
    printf("    [5:0]   = %x:\tInterrupts Active:",bits);
    if(!bits)  printf(" NONE");
    if(bits & 0x01) printf(" INT#0");
    if(bits & 0x02) printf(" INT#1");
    if(bits & 0x04) printf(" INT#2");
    if(bits & 0x08) printf(" INT#3");
    if(bits & 0x10) printf(" INT#4");
    if(bits & 0x20) printf(" INT#5");
    printf("\n\n");

    val = *(volatile unsigned*)(SYSTEM_REGS+0x180);
    printf("MEM_SYNTH R180=0x%08x\n",val);
    div_ref = val & 0xff;
    div = (val >> 8) & 0xff;
    postD = (val >> 16) & 0x3;
    vcod = (val >> 22) & 0x3;
    fout = foutGet(div,div_ref,postD) / 1000000;
    printf("    Fout=%d Fvco=%d\n\n",fout,fvcoGet(fout,postD,vcod));

    val = *(volatile unsigned*)(SYSTEM_REGS+0x184);
    printf("CORE_SYNTH R184=0x%08x\n",val);
    div_ref = val & 0xff;
    div = (val >> 8) & 0xff;
    postD = (val >> 16) & 0x3;
    vcod = (val >> 22) & 0x3;
    fout = foutGet(div,div_ref,postD) / 1000000;
    printf("    Fout=%d Fvco=%d\n\n",fout,fvcoGet(fout,postD,vcod));
}
