/* bsp.h - common bsp header */

#ifndef	__INCbsph__
#define	__INCbsph__

#ifndef __ASSEMBLER__ 
#include <system.h>
#endif

#ifndef	BOOL
#define	BOOL int
#endif

/* image CRC-32 error behavior */

#define	CRC_IGNORE	0	/* nothing */
#define	CRC_REBOOT	1	/* jump to CRC_REBOOT_ENTRY */
#define	CRC_STOP	2	/* forever loop */

/* UART ioctl commands */

#define	FIOBAUDRATE		100		/* set baud rate */
#define	FIOSETUARTOPTIONS	101		/* set LCR register */
#define FIOUARTOPTIONS		102		/* get LCR register */
#define FIOSETLOOPBACK		0xa0000002	/* on/off local loopback */
#define FIOGETUARTERRORS	0xa0000003	/* rx errors count */
#define FIOGETUARTDROPPED	0xa0000004	/* rx dropped bytes */

/* for bsm driver */

#define SIOCDEVPRIVATE		0x89f0

#ifndef __ASSEMBLER__ 

/* globals */

extern FUNCPTR nv_init;			/* NVRAM initialization routine */
extern FUNCPTR nv_get;			/* NVRAM read routine */
extern FUNCPTR nv_put;			/* NVRAM write routine */

extern unsigned boardCpuClockRate;	/* CPU frequency (MHz) */

/* function declarations */

/* backplane bus */

int boardProcNumGet(void);
void boardProcNumSet(int procNum);

/* errno */
void boardSetErrno (int err);
int  boardGetErrno (void);

/* version */

char* bspLibVersion();
int bspVersion(int silence);

/* trace */

void boardToTraceBuf(unsigned message);

/* cpu */

void udelay(int us);
unsigned boardCpuClockRateGet(void);
int boardToEntry(int* startAddr, int arg);
void boardCpuHalt(void);

/* system clock */

void boardClockDisable(void);
int boardClkRateGet(void);
int boardClkRateSet(int _hz);
unsigned boardTickGet(void);
void boardTickSet(unsigned val);

/* RM7000 timer */

void boardClockAltSource(int flag);

/* timers */

//#if 1  /*OS_VERSION > 330*/ 
int boardAuxClkConnect(FUNCPTR rtn, intptr_t arg);
int boardAux2ClkConnect(FUNCPTR rtn, intptr_t arg);
int boardAux3ClkConnect(FUNCPTR rtn, intptr_t arg);
int boardAux4ClkConnect(FUNCPTR rtn, intptr_t arg);
/*
 * #else
int boardAuxClkConnect(FUNCPTR rtn, int arg);
int boardAux2ClkConnect(FUNCPTR rtn, int arg);
int boardAux3ClkConnect(FUNCPTR rtn, int arg);
int boardAux4ClkConnect(FUNCPTR rtn, int arg);
#endif
*/
void boardAuxClkDisable(void);
void boardAux2ClkDisable(void);
void boardAux3ClkDisable(void);
void boardAux4ClkDisable(void);
void boardAuxClkEnable(void);
void boardAux2ClkEnable(void);
void boardAux3ClkEnable(void);
void boardAux4ClkEnable(void);
int boardAuxClkRateGet(void);
int boardAux2ClkRateGet(void);
int boardAux3ClkRateGet(void);
int boardAux4ClkRateGet(void);
int boardAuxClkRateSet(int _hz);
int boardAux2ClkRateSet(int _hz);
int boardAux3ClkRateSet(int _hz);
int boardAux4ClkRateSet(int _hz);
unsigned boardTimerCountGet(unsigned ix);
int boardTimerCountSet(unsigned ix, unsigned count);

/* real time clock */

void boardRtcIntEnable(void);

/* interrupt */

int boardIvEnable(unsigned iv);
int boardIvDisable(unsigned iv);
int boardIvKnown(int iv);

/* memory */

#if OS_VERSION < 300
char* boardMemTop(void);
#endif

/* serial ports */

void* auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed);
void* _auxDevInit(char* name, int channel, int rdSize, int wrSize, int speed, int intEnable);

/* watchdog and descreet signals */

void boardReset(void);
void boardTestOK(void);
void boardWDEnable(void);
void boardWDDisable(void);
short boardWDReset(void);
#if defined(FONDOR) || defined(LINK32)
unsigned boardUserIn(void);
#else
unsigned short boardUserIn(void);
#endif
int boardUserOut(int n, int b);
int boardRunMode(void);

/* NVRAM */

int sysNvLineSize(char string[]);
int sysNvRamCheck();
unsigned char sysNvRamCheckSum(void);
int sysNvRamGet(char* string, int maxLen, int offset);
int sysNvRamSet(char* string, int maxLen, int offset);

/* VME */

int boardLocalToBusAdrs(int am, char* localAdrs, char** pBusAdrs);
int boardBusToLocalAdrs(int am, char* busAdrs, char** pLocalAdrs);
int boardIntEnable(int line);
int boardIntDisable(int line);

#ifdef BT202
  unsigned
#else
  int
#endif
      boardFWTestResult (void);

#endif /*__ASSEMBLER__*/

#endif	/* __INCbsph__ */
