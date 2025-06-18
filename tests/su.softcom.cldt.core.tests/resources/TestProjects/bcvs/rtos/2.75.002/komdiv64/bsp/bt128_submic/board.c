/* board.c - for K128RIO */

/*
modification history
--------------------
  **oct09,iso - darft version
*/

//#include <fenv.h>
#include <arch/board.h>
#include <board.h>
#include <cacheLib.h>
#include <bspversion.h>
#include <logLib.h>
#include <taskLib.h>
#include <ns16x50.h>

#ifdef BT128
  #define PMON
  #if OS_VERSION < 300
    #include "drv/pmon/vl_move.c"
  #endif
#endif

#undef  BOARD_HW_INIT1 
#define BOARD_HW_INIT1 boardHWInit1()

/* cpu frequency detect delay in cpu commands */

#define	DETECT_DELAY	16384

/******************************************************************************/

/* globals */

int boardProcNum = -1;			/* processor number of this cpu */
int boardCpu = CPU;			/* system CPU type */
char* boardCpuName = CPU_NAME;		/* CPU revision name */
int boardFlags;				/* boot flags */
char boardModelName[] = MODEL_NAME;	/* board name */
int boardStartType = -1;		/* BOOT_COLD or BOOT_NORMAL */
unsigned boardCpuClockRate = 200;	/* cpu clock rate in MHz */
unsigned boardTfknTestResult;		/* POST result */
unsigned boardNsDelayK = 512*600;       /* udelay coeffecient*1024*boardCpuClockRate */

extern FUNCPTR boardOutStringRtn;	/* routine for kprint */
extern long boardOutStringArg;		/* first argument */


#ifdef FOR_SUBMICRON
unsigned time_start_boardHWInit1;   //gmu: otl
unsigned time_clear_cp0count_hw1;       //gmu: otl
unsigned time_clear_cp0count_hw2;       //gmu: otl
unsigned time_sync_VM6_VM7;       //gmu: otl

#ifdef DEBUG_SAVE_INIT_TIME  //debug
char string_buf_for_init_info[4096];  // буфер для хранения информации о инициализации
unsigned counter_buf_for_init;  // указатель на область для записи в буфер инициализации
#endif //DEBUG_SAVE_INIT_TIME

#endif //FOR_SUBMICRON

#if (OS_VERSION > 301) || (OS_VERSION == 301 && OS_RELEASE >= 90)
    #include <drv/timer/mipsR4kTimerNew.c>
#else
    #include <drv/timer/mipsR4kTimer.c>
#endif

#include <drv/timer/k128clk.c>

#include <drv/intrCtrl/intrCtrl.c>
#include <drv/pci/pciLib.c>

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


#if OS_VERSION < 300
  #include "configbrd.c"
#endif

/******************************************************************************/

/* globals */

extern uint32_t cpuSRdefault;

/******************************************************************************/
/*****************************************************************/

int boardFWTestResult (void)
{
    return  boardTfknTestResult;
}

/******************************************************************************/

/* calibrate udelay */

unsigned boardUdelayCalibrate()
{
    unsigned v1, v2, cycles = boardCpuClockRate*0x4000;
    v1 = boardCountGet();
    boardNsDelay(cycles*2);
    v2 = boardCountGet();
    cycles += 32;
    return ((cycles*1024ULL)*boardCpuClockRate) / (v2 - v1);
}

/******************************************************************************/

/* program delay on specified number of microseconds */

void udelay(int us)
{
#ifdef CMODEL
      unsigned cycles = (unsigned long long)us/(1000 / 160) +8;                 /* CMODEL - speceal treat */ 
#else
      unsigned cycles = (unsigned long long)us * boardNsDelayK/1024 + 8;
#endif
#if OS_VERSION >= 300
    boardNsDelay(cycles);
#else
    cpuCycleDelay(cycles);
#endif
}

/******************************************************************************/

unsigned boardCpuClockRateGet(void)
{
#ifdef CHIP
   if ((cp0PrIDGet() & 0xf) < 4)   /* K128v2 */
       return 125000000;           /* 125 Mgz */
   else
       return 200000000;           /* K128v3 - 200 Mgz */
#else
  return CPU_CLK_RATE;
#endif
}

/******************************************************************************/

int boardProcNumGet(void)
{
    return boardProcNum;
}

/******************************************************************************/

void boardProcNumSet(int procNum)
{
    boardProcNum = procNum;
}

/*******************************************************************************
*
* boardToEntry - transfer control to some address
*
* This routine jumps to absolute address
*
*/

STATUS boardToEntry(int* startAddr, int arg)
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

/* get top of system memory */

#if OS_VERSION < 300
char* boardMemTop(void)
{
#ifdef CMODEL
  return (char*)(LOCAL_MEM_LOCAL_ADRS + 0x1000000 - TRACE_BUF_SIZE);
#else
  return (char*)(LOCAL_MEM_LOCAL_ADRS + 0x10000000 - TRACE_BUF_SIZE);
#endif
}
#endif

/******************************************************************************/

/* calculate memory sum, it used in old flash library */

unsigned boardMemSum(unsigned* ptr, int nLongs)
{
    unsigned sum, ix;

    for(sum = nLongs, ix = 0; ix < nLongs; ix++)
	sum += ptr[ix];

    return sum;
}

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
    "   Model:  %s      Memory Size: %#x\n"
    "   CPU: %s       Creation date: %s %s\n"
    "   BSP version %i.%i    BSP library compiled for OC2000 version %s\n",
	boardModelName,	boardMemTop()-(char*)LOCAL_MEM_LOCAL_ADRS,
	boardCpuName,__DATE__,__TIME__,
	BSP_VERSION_MAJOR,BSP_VERSION_MINOR,bspLibVersion());
}
#else
void boardLogo(void)
{
    printf(
    "   BSP version %i.%02i%s    build %s\n"
    "   Board:  %s     CPU: %s           processor #%i\n"
    "   Kernel image creation date: %s %s \n",
        BSP_VERSION_MAJOR,BSP_VERSION_MINOR,
#if BSP_VERSION_BETA==1
           "beta ",
#else
           "     ",
#endif
        bspLibVersion(),
        boardModelName, boardCpuName, boardProcNumGet(),
        /*IMAGE_DATE*/__DATE__,__TIME__
    );
}
#endif

/******************************************************************************/

/* get (and show) version of BSP */

int bspVersion(int silence)
{
    if(!silence)
	printf("%s BSP Version %i.%i\n",boardModelName,
	    BSP_VERSION_MAJOR,BSP_VERSION_MINOR);
    return BSP_VERSION;
}

/******************************************************************************/

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
	// pciDevsDisable(TRUE);
#if OS_VERSION < 300
	intLock();
#endif
	boardToEntry((int*)WARM_REBOOT,0);
#if OS_VERSION >= 300
    );
#endif
}

/******************************************************************************/

/* detect module mode. Returns: 0 - regular, 1 - technological */

int boardRunMode(void)
{
    return 1;
}

/******************************************************************************/

/* this routine called from kprint */

#if OS_VERSION < 300

int tty_outstring(char* s)
{
    if(boardOutStringRtn && boardOutStringRtn != (void*)(-1))
	return boardOutStringRtn(boardOutStringArg,s);
    return -1;
}

#endif

/******************************************************************************/

/* common serial port initialization routine */
extern void* _k128AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed, int cread);

void* _auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed,
                  int intEnable)
{
    if(channel == 0 || channel == 1)
    {
	return _k128AuxDevInit(name,channel,rdSize,wrSize,speed,intEnable);
    }
    return NULL;
}

void* auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed)
{
   return _auxDevInit(name, channel, rdSize, wrSize, speed, 1);
}

/********************************************************************************/

#ifdef TRACE_ON
unsigned* pTraceBufStart = NULL;
unsigned* pTraceBufLimit = NULL;
static unsigned *pTBShow, *pTBShow0, lenOfTBShow;

#ifdef TRACE_ON_MANUAL
  static int flagTraceOn = 0;
#else
  static int flagTraceOn = 1;
#endif

/********************************************************************************/

void boardToTraceBufOn(void)
{
    flagTraceOn = 1;
}

/********************************************************************************/

void boardToTraceBufOff(void)
{
    flagTraceOn = 0;
}

/********************************************************************************/

void boardToTraceBufRestore (void)
{
#if OS_VERSION >= 300
	pTraceBufStart = (void*)(BOARD_UNCACHED_MEM_BASE+BOARD_UNCACHED_MEM_SIZE-TRACE_BUF_SIZE);
#else
        pTraceBufStart = (int*)K0_TO_K1(boardMemTop());
#endif
   pTraceBufLimit = (int*)((char*)pTraceBufStart + TRACE_BUF_SIZE);
}

/********************************************************************************/

void boardToTraceBuf(unsigned message)
{
    if(!flagTraceOn)
        return;

#ifdef TRACE_ON_CHECKSTACK
    checkstack();
#endif
    if(!pTraceBufStart)
    {
#if OS_VERSION >= 300
	pTraceBufStart = (void*)(BOARD_UNCACHED_MEM_BASE+BOARD_UNCACHED_MEM_SIZE-TRACE_BUF_SIZE);
#else
        pTraceBufStart = (int*)K0_TO_K1(boardMemTop());
#endif
        pTraceBufLimit = (int*)((char*)pTraceBufStart + TRACE_BUF_SIZE);
        memset((char*)pTraceBufStart,0,TRACE_BUF_SIZE);
        *(pTraceBufStart) = (unsigned)(pTraceBufStart+2);
        *(pTraceBufStart+1) = 0;
    }

    if((unsigned *)*pTraceBufStart >= pTraceBufLimit)
    {
        *pTraceBufStart = (unsigned)(pTraceBufStart+2);
        *(pTraceBufStart+1) += 1;
    }
    *((unsigned *)(*pTraceBufStart)) = message;
    *pTraceBufStart += sizeof(unsigned);
}

/********************************************************************************/

static void sysTBSp (void)
{ int i0=0, i=0, j=0;  char s[40];
  unsigned *p = pTBShow;
  while(lenOfTBShow > 0)
     { if (j==0) printf("%07i: ", i0);
       printf ("%08x ", *pTBShow++); lenOfTBShow--;
       if (pTBShow >= pTraceBufLimit) pTBShow = pTraceBufStart+2;
       if (pTBShow == pTBShow0)
         { printf ("\n\r"); return;
         }
       j++; i0++;
       if (j > 7)  
        { printf ("\n\r"); i++; j = 0;
        }
       if (i >= 24)
         { i =0;
           printf ("-------more ? (from %p)-------", pTBShow); 
           fflush(stdout); s[0] = 0; gets(s);
           if (s[0] != 0) 
             { pTBShow = p;  return;
             }
         }
     }
}
/*-------------------------------------*/
/*     i0 > 0 - show last i0 messages */
/*     i0 < 0 - show -i0 messages preceding last shown */
static unsigned *pTBShowLast=0;
void sysTBS (int i0)
{ if (i0 < 0 && pTBShowLast != 0) 
    { pTBShow0 = pTBShow = pTBShowLast; i0 = -i0;
    }
  else
    { pTBShow = pTBShow0 = (unsigned *)*pTraceBufStart;
      printf ("%i loops, current pointer %#x\n\r", *(pTraceBufStart+1), *pTraceBufStart);
    }
  lenOfTBShow = i0;
  while(i0-- > 0) 
        { pTBShow--; 
          if (pTBShow <= pTraceBufStart+1)
            pTBShow = pTraceBufLimit-1;
        }
   pTBShowLast = pTBShow;
   printf ("Show starts from %p\n\r", pTBShow);
   sysTBSp ();
}
/*-------------------------------------*/
/* Find code 'code' on mask 'mask' starting back from current pointer */

void sysTBF (unsigned code, unsigned mask) 
{ unsigned *p; char s[40]; int i=0;
  pTBShow = pTBShow0 = (unsigned *)*pTraceBufStart;
  do 
    { pTBShow--; i--;
      if (pTBShow <= pTraceBufStart+1)
        pTBShow = pTraceBufLimit-1;
      if ((*pTBShow & mask) == code)
        { p = pTBShow; lenOfTBShow = 10;
          printf("found at offset %i\n\r", i);
          sysTBSp ();
          puts ("\n\r-------next ?-----");  s[0] = 0; gets(s);
          if (s[0] != 0) return;
          pTBShow = p;
        }
     }
   while (pTBShow != pTBShow0);
}
#else	/* TRACE_ON */

void boardToTraceBuf(unsigned message) {}

#endif	/* TRACE_ON */


/******************************************************************************/

char *boardModel(void)
{
    return boardModelName;
}

/******************************************************************************/
extern void k128AuxHwInit(void);

#if OS_VERSION >= 300
int boardHWInit1()
#else
void boardHWInit1()
#endif
{
    TRACE(TR_START,0xb0000);    

#ifdef FOR_SUBMICRON
    time_start_boardHWInit1 = boardCountGet();  // gmu: otl
    time_clear_cp0count_hw1 = 0;       // gmu: otl
    time_sync_VM6_VM7 = 0;
#endif

#if OS_VERSION >= 300
    modLinkPageInternal->mlp_cpuSRDefault = SR_FR|SR_CU3|SR_CU2|SR_CU1|SR_CU0|SR_IEC|SR_KSSUPER|SR_PX|SR_KX|SR_UX|SR_SX|0xfc00;
   #if OS_VERSION >= 306
    modLinkPageInternal->mlp_cpuFpuSRDefault = FCSR_FSbit << 24;        /* set FS bit for no IEEE754 mode */
    #endif
#endif

#if defined(PMON) && OS_VERSION < 300
    move_pmons_vars ();
#endif
    
    //boardCpuClockRate = cpuFreqDetect(DETECT_DELAY);
    boardCpuClockRate = boardCpuClockRateGet() / 1000000;
    TRACE(TR_START,0xb0002);

    /* calibrate udelay */

    boardNsDelayK = boardUdelayCalibrate();
    TRACE(TR_START,0xb0004);

    intrCtrlInit();
    TRACE(TR_START,0xb0008);

#if OS_VERSION < 300
    intConnect(IV_TIMER,boardClockInt,0);
#endif
    boardClockDisable();
    TRACE(TR_START,0xb0060);
#if (OS_VERSION < 301) || (OS_VERSION == 301 && OS_RELEASE < 90)
    boardClockInit(BOARD_TIMER_HZ);
#endif
#if OS_MAJOR >= 3
    modClockInit(CLOCK_REALTIME,BOARD_TIMER_HZ,modCLOCK_FLAG_PROCESS_CPUTIME|modCLOCK_FLAG_THREAD_CPUTIME);
#if (OS_VERSION > 301) || (OS_VERSION == 301 && OS_RELEASE >= 90)
    boardClockEnable();
#else
    boardClockConnect((funcptr_t)modClockTick,CLOCK_REALTIME);
#endif
#endif

    k128AuxHwInit();
    TRACE(TR_START,0xb0068);
                                    /* Set user's output lines */
#ifdef FOR_SUBMICRON // gmu: for bcvs
    *GPIO_OE_REG = 0xf000; /* for bcvs: sGPIO[15-12] - output, sGPIO[11-0] - input */
#else
    *GPIO_OE_REG = (1 << 15) | (0xf << 7);  /* WD reset line and 4 UO lines */
#endif

    
#if OS_VERSION < 300
    cpuSRdefault=0x74018c01;
    intUnlock(cpuSRdefault);
    cp1FCSRSet(cp1FCSRGet() | ( FCSR_FSbit << 24 ) );  /* set FS bit  in FCSR reg*/
#endif

#ifdef  INCLUDE_CRC
    if(buildin_crc32 != current_crc32)
    {
        TRACE(TR_START,0xc0024);
        kprint("CRC ERROR: buildin - 0x%08lx, current - 0x%08lx\n\r",
               buildin_crc32,current_crc32);
               TRACE(TR_START,0xc0028);
    }
#endif
    TRACE(TR_START,0xb00ff);
#if OS_VERSION >= 300
    return 0;
#endif
}
/******************************************************************************/
unsigned short boardUserIn(void)
{
  return (unsigned short)*GPIO_IN_REG;
}
/*-----------------------------------------------------------*/
int boardUserOut(int n, int b)
{ unsigned short out = *GPIO_OUT_REG;
#ifndef FOR_SUBMICRON
  switch (n)
  { case 0: out = (out & (0xe << 7)) | ((b & 1) <<7); break;
    case 1: out = (out & (0xd << 7)) | ((b & 1) <<8); break;
    case 2: out = (out & (0xb << 7)) | ((b & 1) <<9); break;
    case 3: out = (out & (0x7 << 7)) | ((b & 1) <<10); break;
    default: return -1;
  }
#else
  //gmu: bcvs must be output GPIO[12-15]
  if((n<12)||(n>15))return -1;
  else 
     out = out&(~(1<<n));
     out = out|(b<<n);   
#endif //FOR_SUBMICRON
  *GPIO_OUT_REG = out;
  return 0;
}
/*----------------------------------------------------------------*/
void boardExtInt0Set(int set)
{
   boardIvEnableMode (IV_EXT3, set);
}

void boardExtInt1Set(int set)
{
   boardIvEnableMode (IV_EXT4, set);
}
