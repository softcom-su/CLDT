/* configbrd.c - optional hardware initialization */

#ifndef	INCconfigbrdc
#define	INCconfigbrdc

#if OS_VERSION >= 300
#include "board.c"
#endif
// time 
unsigned time_init_debug_mas[20];
unsigned time_init_debug_counter;

int submicronSyncResult; //gmu: for bcvs
int rioInitAll(void); //gmu: for bcvs

#if OS_VERSION < 300
  #include "config/usrSerial.c"
#endif

#if defined(INCLUDE_RIO) && !defined(MODULES_MASK) && !defined(INCLUDE_RREG)
 #ifdef BT5328
   #warning !!!!! default MODULES_MASK 0x7 !!!!!!!!!!!
   #define MODULES_MASK 0x7
 #else
   #warning !!!!! default MODULES_MASK 0x7f !!!!!!!!!!!
   #define MODULES_MASK 0x7f
 #endif
#endif

/******************************************************************************/

/* kprint support routine */

#if OS_VERSION < 300

  #if defined(BT210) || defined(BT211) || defined(BT83RUS) || defined(SOK)|| defined(MPON) || defined(BT128) || defined(BT83M)
    FUNCPTR boardOutStringRtn = (FUNCPTR)nsOutString;
    #ifndef CONSOLE_PORT_NUM
    #define CONSOLE_PORT_NUM 0
    #endif
    long boardOutStringArg = (long)&uart[CONSOLE_PORT_NUM];
  #else
    FUNCPTR boardOutStringRtn = NULL;
    long boardOutStringArg =  0;
  #endif

#else

  extern FUNCPTR boardOutStringRtn;
  extern long boardOutStringArg;

#endif

#if defined(TRACE_ON) && defined(TRACE_ON_CHECKSTACK)
   #include "config/checkstack.c"
#endif

#if OS_VERSION >= 300
  #include "config/usrSerial.c"
#endif
/******************************************************************************/

#undef	BOARD_HW_INIT2
#undef	BOARD_HW_INIT3
#define	BOARD_HW_INIT2	boardHWInit2()
#define	BOARD_HW_INIT3	boardHWInit3()

#define OCPB_DRIVER 1
#define LINUX_DRIVER 2

//=============================================================================
#ifdef	BOOTER_INHERITANCE
    #include "config/usrBootLine.h"
#endif
//=============================================================================
#ifdef	INCLUDE_IDLE
    #include "idle.h"
#endif	/* INCLUDE_IDLE */

#if defined (FONDOR) || defined(LINK32)
  #include "drv/intrCtrl/io.c"
#endif

#ifdef _5890_DEMO
  #include "drv/discSignals/discSignals.c"
#endif
#ifdef	INCLUDE_NVRAM
  #include "drv/mem/flashNvRam.c"
  #ifdef	INCLUDE_PRIME_NVRAM
     #include "drv/mem/romFlashNvRam.c"
  #endif	/* INCLUDE_PRIME_NVRAM */
  #ifdef	FLASH_NVRAM_SECTOR
     #include "drv/mem/sysFlashNvRam.c"
  #endif	/* FLASH_NVRAM_SECTOR */
#else
    #include "drv/mem/nullNvRam.c"
#endif	/* INCLUDE_NVRAM */
#ifdef INCLUDE_FLASH
    #include "config/usrFlash.c"
#endif /* INCLUDE_FLASH */
#ifdef INCLUDE_FTL
    #include "config/usrFtl.c"
#endif /* INCLUDE_FTL */
#ifdef INCLUDE_PMC_PORTS
    #include "config/usrSerial_PMC.c"
#endif 

#ifdef	BOOTER_INHERITANCE
  #ifndef	BOOT_LINE_ADRS
    #define BOOT_LINE_ADRS		((char*)0x80000700)
  #endif
#endif	

#ifdef	INCLUDE_NETWORK
  #ifdef	INCLUDE_DE
    #include "if_de.h"
  #endif	/* INCLUDE_DE */
  #if defined(INCLUDE_VME) && defined(MP_NETWORK) && \
      (OS_VERSION >= 300 || \
       (OS_VERSION < 300 && MP_NETWORK_TYPE==MP_NETWORK_BUSNET)) && \
        BSM_DRIVER_TYPE==OCPB_DRIVER
     #if defined(FAST_BUSNET)  /* driver, accelerated by Raiko. Not checked in real life */
       #warning   !!! accelerated BUSNET driver !!!
       #include "drv/netif/bsm/bsm_fast/vme.c"
       #include "drv/netif/bsm/bsm_fast/bsm_core.c"
       #include "drv/netif/bsm/bsm_fast/if_mp2.c"
     #else
      #include "drv/netif/bsm/vme.c"
       #include "drv/netif/bsm/bsm_core.c"
       #include "drv/netif/bsm/if_mp2.c"
    #endif
  #endif

  #ifdef	INCLUDE_RL
    #include "if_rl.h"
  #endif

  #ifdef	BOOTER_INHERITANCE
    #include "config/usrBootLine.c"
    #include "config/usrNetwork.c"
  #endif

#endif	/* INCLUDE_NETWORK */

#ifdef	INCLUDE_VME
  #if OS_VERSION < 300 && defined(INCLUDE_MULTIPROCESSING) && defined(INCLUDE_MP_OBJS)
    #include "mpspin.c"
  #endif
  #ifdef	INCLUDE_VMESHOW
    #include "drv/vme/vmeShow.c"
  #endif
  #ifdef	VBI_FIXED
    #include "config/usrVme.c"
  #endif
#endif	/* INCLUDE_VME */

#ifdef	INCLUDE_IDE
    #include "config/usrIde.c"
#endif	/* INCLUDE_IDE */
#ifdef	INCLUDE_LPT
    #include "config/usrLpt.c"
#endif	/* INCLUDE_LPT */
#ifdef	INCLUDE_SCSI
    #include "config/usrScsi.c"
#endif	/* INCLUDE_SCSI */
#ifdef	INCLUDE_QBUS
    #include "config/usrQbus.c"
#endif	/* INCLUDE_QBUS */
#ifdef	INCLUDE_ARINC
    #include "btm404Drv.h"
    #include "btm404DrvP.h"
#endif	/* INCLUDE_ARINC */
#if defined(INCLUDE_BTM401) || defined(INCLUDE_BT401A) || defined(INCLUDE_BT83401)
    #include "config/usr401.c"
#endif
#ifdef	INCLUDE_RGB
    #include "config/usrRgb502.c"
#endif	/* INCLUDE_RGB */
#ifdef	INCLUDE_RTC
   #include "config/usrRtc.c"
#endif	/* INCLUDE_RTC */
#ifdef INCLUDE_BT23410
   #include "config/usr410.c"
#endif

#ifdef BT419
   #include "config/usrMulti.c"
#endif

#ifdef BT206PD
   #include "config/usrPDInt.c"
#endif
#ifdef BT55211
   #include "drv/dualPortRAM/dualPortRAM.c"
#endif

#ifdef INCLUDE_BT83_402
   #include "config/usrIsa.c"
#endif
#ifdef INCLUDE_RAMDISK
   #include "config/usrRam.c"
#endif

#ifdef INCLUDE_VI
    #include "saa.h"
#endif
    
#ifdef INCLUDE_AIC
    #include "aic.h"
#endif

#if defined(INCLUDE_RIO)
  extern int rioStartupDelay;
  #include "drv/rio/rio.c"
  #include "drv/rio/rreg.c"
#ifdef FOR_SUBMICRON  
  #include "drv/rio/rio_init_submic.c"   // gmu: for bcvs
#endif
  #if defined(MPON) && defined(INCLUDE_NETWORK) && defined(INCLUDE_RMSG_NET)
  #include "drv/netif/rmsg/if_rmsg.c"
  #endif
#endif


#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron  
#warning !!!!!!!! <drv/psc/pscLib.c> !!!!
#include <drv/psc/pscLib.c>
#endif


#ifndef	EXCLUDE_DUMP_LIB
    #include "dump.h"
#endif

#if OS_VERSION < 300
  #ifndef	MODCALL
  #define	MODCALL(x)	x
  #endif
#endif
#if 0
/* Тип компьютера: RM7000, и т.д. */
enum cpu_type_id {
 cptype_R3000   =1,
 cptype_R3081   =2,
 cptype_1B578   =3,
 cptype_1B812   =4,
 cptype_1890BM1 =5,
 cptype_1890BM2 =6,
 cptype_RM7000  = 100,
 cptype_1890BM3 =103,
 cptype_1890BM5 =105,
 cptype_1890BM6 =106,
 cptype_1890BM7 =107,
 cptype_i386    =300
} ;

/***********************************************************/
int boardGetCPUType(void)
{ char *p = boardCpuName;
  if      (strstr (p,"K64-SMP")) return cptype_1890BM5;
  else if (strstr (p,"K64-RIO")) return cptype_1890BM6;
  else if (strstr (p,"K128-RIO")) return cptype_1890BM7;
  else if (strstr (p,"RM7000C")) return cptype_RM7000;
  else if (strstr (p,"Komdiv64")) return cptype_1890BM3;
  else                            return cptype_1890BM2;
}
#endif
/******************************************************************************/
#if defined(CPRIO64) || defined(BT128)
  #include "drv/pmon/vl.c"
  #define PMON
  extern char ETS_results [];
#endif

/* additional hardware/software initialization */
#if OS_VERSION < 300
static void boardHWInit2(void)
#else
int boardHWInit2(void)
#endif
{
    TRACE(TR_START,0xc0000);
    time_init_debug_counter = 0; //gmu:otl
    time_init_debug_mas[time_init_debug_counter++]=boardCountGet();//gmu:otl
#if OS_VERSION >= 300
                 // interrupts masks in STATUS reg 
#ifdef	K64SMP
    intUnlock(intLock()|0x8c00);
#endif

#if defined(BT210) || defined(BT128) 
    intUnlock(intLock()|0xfc00);
#endif

#ifndef INCLUDE_VME
  #define VME_MEM_TOTAL_SIZE 0
#endif

#if defined(MPON)
    cacheInit2(BOARD_UNCACHED_MEM_BASE,0x1000000-TRACE_BUF_SIZE);
#elif defined(K64RIO) || defined(BT128) || defined(BT83M)
    cacheInit2(BOARD_UNCACHED_MEM_BASE,BOARD_UNCACHED_MEM_SIZE-TRACE_BUF_SIZE);
#elif !defined(XLP)
    cacheInit2(BOARD_UNCACHED_MEM_BASE,BOARD_UNCACHED_MEM_SIZE-VME_MEM_TOTAL_SIZE-TRACE_BUF_SIZE);
#endif
    TRACE(TR_START,0xc0002);
#endif /* #if OS_VERSION >= 300 */

#if OS_VERSION >= 305
    TRACE(TR_START,0xc1000);
    {
	MLP_BSP* bsp = (void*)modMPDSharedAlloc(shmemSYSDATA,sizeof(MLP_BSP),8);
	TRACE(TR_START,0xc1001);
	if(!bsp)
	    kprint("%s: modMPDSharedAlloc MLP_BSP size=%d error\n\r",__FUNCTION__,sizeof(MLP_BSP));
	MODCALL(modLinkPageInternal->mlp_ppmPrivate = bsp);
	TRACE(TR_START,0xc1002);
	bsp->cpuClockRate = boardCpuClockRate;
	TRACE(TR_START,0xc100f);
    }
#endif

#if OS_VERSION < 243
    boardClkInitDone = TRUE;
    TRACE(TR_START,0xc0004);
#endif

#if (OS_VERSION >= 300) && (OS_VERSION <=301 && OS_RELEASE < 90)
    boardClockEnable();
    TRACE(TR_START,0xc0006);
#endif

#if defined(INCLUDE_NVRAM) && defined(INCLUDE_PRIME_NVRAM)
    romNvInit();
    TRACE(TR_START,0xc0008);
#endif	/* INCLUDE_PRIME_NVRAM */

#ifndef CMODEL
//    kprint("pciLibInit ...");
#ifndef NO_PCI
    pciLibInit();
#endif
//    kprint(" OK\n\r");
    TRACE(TR_START,0xc0010);
#endif
#if defined(BT205)
    { extern void btm404Shutup ();
      btm404Shutup ();
      TRACE32(TR_START,w);
    }
#endif
#ifdef UZPI
    *ASIC_RamStatReg = *ASIC_RamStatReg & (~0x100);
#endif
#ifdef BT419
    { int mask = intLock();
      *ASIC_PioFuncReg |= 0x3003;
      intUnlock(mask);

      usrMultiConfig();
    }
#endif
    //KPRINT("usrSerialConfig()...");
    usrSerialConfig();
    TRACE(TR_START,0xc0020);
    //KPRINT("OK\n\r");
#if OS_VERSION > 218
  #ifndef NO_BSP_WARNINGS
  #warning !!!!!!!!!     setlocale(LC_ALL,"C@short"); !!!!!!!!!!!!!
  #endif
    setlocale(LC_ALL,"C@short"); 
#endif
    
#ifdef	INCLUDE_CRC
    if(buildin_crc32 != current_crc32)
    {
	TRACE(TR_START,0xc0024);
	kprint("CRC ERROR: buildin - 0x%08lx, current - 0x%08lx\n\r",
	    buildin_crc32,current_crc32);
	TRACE(TR_START,0xc0028);
    }
#endif

#ifdef INCLUDE_PMC_PORTS
    usrSerialConfig_PMC();
#endif

#if defined(INCLUDE_VME)
  #ifdef VME_DEBUG_LEVEL
    vmeDebug = VME_DEBUG_LEVEL;
  #endif
    vmeInit();
#endif

#ifdef	INCLUDE_RGB
    sysRgbInit();
    TRACE(TR_START,0xc0040);
#endif	/* INCLUDE_RGB */

#ifdef	INCLUDE_RTC
    sysRtcInit();
    TRACE(TR_START,0xc0050);
#endif	/* INCLUDE_RTC */

    TRACE(TR_START,0xc00ff);

#if defined(INCLUDE_NETWORK) && defined(BOOTER_INHERITANCE)
    //kprint("MODCALL(bootStringToStruct...");
    MODCALL(bootStringToStruct((char *)BOOT_LINE_ADRS,0););
#endif

#if defined(PMON)
    check_cku_results ();
#endif   

#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron
   #warning   !!!! SUBMICRON version for BCVS LiveInc !!!!!!!!!!!     
   #ifdef SUBMICRON_TIMER_LIVE
   #ifdef SUBMICRON_TIMER_LIVE_PERIOD
   #ifdef SUBMICRON_TIMER_LIVE_TIME    
   #warning   !!! SUBMICRON: pscLibInitLiveInc
   pscLibInitLiveInc(SUBMICRON_TIMER_LIVE,SUBMICRON_TIMER_LIVE_PERIOD,SUBMICRON_TIMER_LIVE_TIME);

   #endif //SUBMICRON_TIMER_LIVE_TIME
   #endif //SUBMICRON_TIMER_LIVE_PERIOD
   #endif //SUBMICRON_TIMER_LIVE

#endif
    time_init_debug_mas[time_init_debug_counter++]=boardCountGet();//gmu:otl
#if OS_VERSION >= 300
    return 0;
#endif
}

/******************************************************************************/
char *boardETSTestResults(void)
{
#ifdef PMON
  return ETS_results;
#else
  return NULL;
#endif  
}

void boardETSTestResultsShow (void)
{
  printf ("ETS results: %s\n\r", boardETSTestResults());
}
/*--------------------------------------------*/
void SKI_resultsShow (void)
{
  boardETSTestResultsShow ();
}
/******************************************************************************/

#if defined(INCLUDE_VME) && defined(INCLUDE_NETWORK) && defined(MP_NETWORK) && \
  (OS_VERSION >= 300 || \
   (OS_VERSION < 300 && MP_NETWORK_TYPE==MP_NETWORK_BUSNET && BSM_DRIVER_TYPE==OCPB_DRIVER))
  static int bsm_crc_flag = BSM_CRC_FLAG ;
#endif  

/* additional hardware/software initialization */

#ifdef	BOOTER_INHERITANCE
  extern int	usrNetInit(char *);
#endif		// BOOTER_INHERITANCE

extern int deDebug;

#if OS_VERSION < 300
static void boardHWInit3(void)
#else
int boardHWInit3(void)
#endif
{
//struct timespec tst, tfn;    
    time_init_debug_mas[time_init_debug_counter++]=boardCountGet();//gmu:otl
#if defined(INCLUDE_NETWORK) && defined(BOOTER_INHERITANCE)
    boot_params params;
    MODCALL(bootParamsCopy(&params););
#endif
    //kprint("boardHw3Init: started\n\r");
#if defined(INCLUDE_NETWORK) && defined(BOOTER_INHERITANCE) && defined(MP_NETWORK) && \
  (OS_VERSION >= 300 || \
   (OS_VERSION < 300 && MP_NETWORK_TYPE==MP_NETWORK_BUSNET && BSM_DRIVER_TYPE==OCPB_DRIVER))
    bsm_crc_flag = ((params.flags & 0x400) == 0) ? 1 : 0;
#endif

    TRACE(TR_START,0xd0000);
#ifdef FONDOR
    boardPioInit();
#endif
#ifdef	INCLUDE_LOGGING
    logInit(LOG_MSG_MAX);
    TRACE(TR_START,0xd0010);
#if defined(BT206) && defined(BAD_INTS_LOGGING)
    gtDefHandlerInit();
#endif  /* BAD_INTS_LOGGING */
#endif	/* INCLUDE_LOGGING */

#ifdef	INCLUDE_FLASH
    sysFlashInit();
    ftlLibInit();
    TRACE(TR_START,0xd0020);
#endif	/* INCLUDE_FLASH */

#ifdef BT55211
    dpRamInit();
#endif

#if defined(BT206) || defined(BT211) || defined(BT212)
  #if PS2MOUSE==1
    //print("\n\rbefore ps2mouseInit()\n\r");
    ps2mouseInit();
    //kprint("\n\rafter ps2mouseInit()\n\r");
  #endif
#endif

#ifdef INCLUDE_NETWORK 

#ifdef	INCLUDE_DE
  #ifdef	BOOTER_INHERITANCE
    if(!strcmp(params.netDev,"de") || !strcmp(params.netDev,"de9"))
  #endif
  {  
    #ifdef INCLUDE_DE9
      #ifndef NO_BSP_WARNINGS
      #warning !!!!!!!!!!! Using embedded ethernet !!!!!!!!!!!!!
      #endif
      #ifdef LINK32
       //deDebug=3;
      #endif
      #ifdef	FIX_DE9_SROM_BUG
        //printf ("deDevInit(9,DE0_CSR11,DE0_TTM,DE0_MEDIA|DE_SROM_BUG)\n\r");
        //deDebug=3;   
	deDevInit(9,DE0_CSR11,DE0_TTM,DE0_MEDIA|DE_SROM_BUG);
      #else
	deDevInit(9,DE0_CSR11,DE0_TTM,DE0_MEDIA);
      #endif
    #else
      #ifndef NO_BSP_WARNINGS
      #warning !!!!!!!!!!! Using 1890VG3 ethernet !!!!!!!!!!!!!
      #endif
#ifndef LINK32
	deDevInit(0,DE0_CSR11,DE0_TTM,DE0_MEDIA);
#endif
    #endif
    TRACE(TR_START,0xd0050);
  }
  #ifdef DE1_INCLUDE
//    deDebug=5;
    #ifndef LINK32
    deDevInit(1,DE1_CSR11,DE1_TTM,DE1_MEDIA);
    TRACE(TR_START,0xd0051);
    #endif
  #endif	/* DE1_INCLUDE */
#endif	/* INCLUDE_DE */

#ifdef INCLUDE_NAE
{
    extern int naeDevInit(unsigned port);
    naeDevInit(NAE_PORT);
}
#endif

#ifdef INCLUDE_RL
    rlinit();
#endif

#if defined(INCLUDE_VME) && defined(MP_NETWORK) && \
  (OS_VERSION >= 300 || \
   (OS_VERSION < 300 && MP_NETWORK_TYPE==MP_NETWORK_BUSNET))
  #ifdef	BOOTER_INHERITANCE
    if(params.smDev == TRUE)
  #endif
  {
    #if BSM_DRIVER_TYPE==OCPB_DRIVER
        bsmCrcFlagSet (bsm_crc_flag);
        if (mpDevInit() <0)
          { printf ("mpDevInit error !!!\n\r");
          }
        else
          {
           #ifndef INCLUDE_LOGO
             printf ("BSM2000: crc set to %s !!!\n", BSM_CRC_FLAG ? "ON":"OFF");
           #endif
           TRACE(TR_START,0xd0052);
          }
    #else            // Linux/MSVS driver !
          { extern void vmethDevInit (void);
//	    extern int vmeth_loglevel;
//	    vmeth_loglevel = 6;
            vmethDevInit();
            TRACE(TR_START,0xd0053);
          }
    #endif
  }    
#endif	/* defined(MP_NETWORK) && MP_NETWORK_TYPE==MP_NETWORK_BUSNET */

  #ifdef	BOOTER_INHERITANCE
  //kprint ("\n\rbefore usrNetInit\n\r");
    MODCALL(usrNetInit(0););
  //kprint ("\n\rafter usrNetInit\n\r");
  #endif
#endif	/* INCLUDE_NETWORK */

// clock_gettime(CLOCK_REALTIME, &tst);

#ifdef	INCLUDE_IDE
  {
   sysIdeInit();
    ideShowInit();
    usrIdeConfig();
    TRACE(TR_START,0xd0070);
   }    
#endif	/* INCLUDE_IDE */

// clock_gettime(CLOCK_REALTIME, &tfn);

//printf("IDE config time: %ld nsec\n", ((long)(tfn.tv_sec - tst.tv_sec))*1000000000 + (tfn.tv_nsec - tst.tv_nsec));
//printf("time start: %d %ld\n", tst.tv_sec, tst.tv_nsec);
//printf("time finish: %d %ld\n", tfn.tv_sec, tfn.tv_nsec);

#ifdef	INCLUDE_LPT
    sysLptInit();
    usrLptConfig();
    TRACE(TR_START,0xd0074);
#endif	/* INCLUDE_LPT */

#ifdef	INCLUDE_SCSI
    sysScsiInit();
    usrScsiConfig();
    TRACE(TR_START,0xd0080);
#endif	/* INCLUDE_SCSI */

#ifdef	INCLUDE_FTL
    usrFtlConfig();
    TRACE(TR_START,0xd0090);
#endif	/* INCLUDE_FTL */

#ifdef	INCLUDE_QBUS
    sysQbusInit();
    usrQbusConfig();
    TRACE(TR_START,0xd00a0);
#endif	/* INCLUDE_QBUS */

#if defined(INCLUDE_BTM401) || defined(INCLUDE_BT401A) || defined(INCLUDE_BT83401)
    usr401Init();
    TRACE(TR_START,0xd00b0);
#endif

#ifdef	INCLUDE_ARINC
    btm404Init();
    TRACE(TR_START,0xd00c0);
#endif	/* INCLUDE_QBUS */

#ifdef	INCLUDE_IDLE
    idleInit(IDLE_PRIORITY,IDLE_DELAY);
    TRACE(TR_START,0xd00d0);
#endif	/* INCLUDE_IDLE */

#if defined(INCLUDE_BT23410) && !defined(NOINIT410)
    usr410Config();
    TRACE(TR_START,0xd00e0);
#endif

#ifdef INCLUDE_BT83_402
    sysIsaInit();
  #if defined(INCLUDE_ED) && INCLUDE_NETWORK==1
  {
    extern int edDevInit(int unit, unsigned short iobase, int irq);
    // printf ("Call edDevInit(%i,%#x,%i)\n\r", 0, ED0_BASE, ED0_IRQ);
    edDevInit(0,ED0_BASE,ED0_IRQ);
  }
  #endif	
  #ifdef __BT_SERIAL
    usrBT83402Config ();
  #endif
#endif

#ifdef INCLUDE_RAMDISK
    if (usrRamConfig() !=0) printf ("usrRamConfig error !\n\r");;
#endif
#if 0                /* some keyboards need it */
#include <termios.h>
   { struct termios t;
     int fd;
     if ((fd = open (CONSOLE_DEVICE,O_RDWR)) < 0)
        { printf ("Error open %s\n\r", CONSOLE_DEVICE); return;
        }
     tcgetattr(fd, &t);
     t.c_iflag |= IGNCR;
     tcsetattr (fd, TCSANOW, &t);
     close(fd);
   }
#endif

#ifdef INCLUDE_VI
    viInit();
#endif
    
#ifdef INCLUDE_AIC
    aicInit();
#endif

#ifdef INCLUDE_RIO
#ifdef FOR_SUBMICRON  //gmu: debug
#warning !!! SUBMICRON: rioInitAll() must be start in SPO !!!
#else
   rioInitAll(); //gmu: for bcvs issled 
#endif
#endif 

#ifndef	EXCLUDE_DUMP_LIB
    dumpInclude();
#endif

    TRACE(TR_START,0xdffff);
//#if OS_VERSION >= 300
  #if defined(BT128) && !defined(CMODEL)
    { int fd = open ("/dev/console",2);
      if (fd >=0)
      { ioctl (fd,FIOFIFODISABLE,0);
        close(fd);
      }
    }
  #endif
#ifdef _5890_DEMO
  discSignalsInit();
#endif 

  time_init_debug_mas[time_init_debug_counter++]=boardCountGet();//gmu:otl

#if OS_VERSION >= 300
  return 0;
#endif
//#endif
}
/***************************************************/
void      boardSetErrno (int err) { errno = err; }
int  boardGetErrno (void) { return errno; }
/***************************************************/
#ifdef	BT206
void pciWbFlush(void)
{
    gtReadInternalReg(PCI0_SYNC_BAR_V_REG);
    gtReadInternalReg(PCI1_SYNC_BAR_V_REG);
}
#endif

#if OS_VERSION >= 300
#include "../ktty.c"
#endif

#ifdef R4000
void udel()
{
    int mask = intLock();
    unsigned start = boardCountGet();
    unsigned end;
    udelay(1000000);
    end = boardCountGet();
    intUnlock(mask);

    printf("1 sec = %u ns\n",(unsigned)((end - start)*(1000ULL*CPU_TIMER_DIV))/boardCpuClockRate);
}
#endif

#ifdef INCLUDE_TRACE
void board_PosixTraceEvent(trace_event_id_t tr_id, void *b, size_t sz){
    posix_trace_event(tr_id, b, sz);
}
#else
void board_PosixTraceEvent(trace_event_id_t tr_id, void *b, size_t sz){}
#endif
#ifdef XLP
void nlm_print(const char *fmt,...)
{
#if XLP_VERBOSE
    va_list args;
    int i;
    va_start(args, fmt);
    i=vfprintf(stdout,fmt,args);
    va_end(args);
#endif
}
#endif


#ifdef INCLUDE_RIO
int rioInitAll(void)
{
  int result = 0;
  submicronSyncResult = 0;  
  #ifdef MPON
    rioInit(RIO_CROSS_TYPE);
    rioRegistration(MODULES_MASK);
  #else
    #ifdef RIO_STARTUP_DELAY
    rioStartupDelay = RIO_STARTUP_DELAY;
    #endif
    
#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron
   #warning   !!!! SUBMICRON version for BCVS rio_init_submic(void) !!!!!!!!!!!
    result = rio_init_submic();
    if(result!=0)return -1;
#endif

    boardRioHWInit(RIO_SEND_THR_PRIORITY,RIO_TIMEOUT,RREG_ID_MAX,
	RREG_THREAD_PRI,RREG_TIMEOUT);
    #if defined(INCLUDE_RMSG_NET)
	rionetDevInit(0,"sm");
    #endif
    #ifdef INCLUDE_VSK
    {
	extern void vskDeviceInit (void);
	vskDeviceInit ();
    }
    #endif
  #endif
  
  return submicronSyncResult;
}
#endif //INCLUDE_RIO



#endif   /* INCconfigbrdc */

