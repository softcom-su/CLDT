/* configbrd.h - BT206 configuration header */

#ifndef	INCconfigbrdh
#define	INCconfigbrdh

#include <board.h>

#include "x_configbrd.h"
#include "dependbrd.h"

#define INCLUDE_DE9		/* include embedded ethernet */
#define	FIX_DE9_SROM_BUG	/* fix srom bug on de9 */
#define	INCLUDE_IV_COUNT	/* collect interrupt counts */
#define FLASH_NVRAM_SECTOR     (-1)

#define	RIO_STARTUP_DELAY	0

#ifdef SUBMICRON
  #define	SUBMICRON_SYNC_TOUT	7
  #define	SUBMICRON_SYNC_ID	25
  #define	SUBMICRON_TIMER_LIVE	5
  #define	SUBMICRON_TIMER_LIVE_PERIOD	100  /* мс */
  #define	SUBMICRON_TIMER_LIVE_TIME	3000 /* мс */
#endif

//#define	FORCED_RIO_OLD_MSG

#define	RMSG_RX_THREAD_PRI	(NET_THREAD_PRIORITY+1)
//#define	RMSG_RX_THREAD

#define	RMSG_TX_RING_SIZE	256

// 1 delay tick ~6 us on altera and ~7.5 us on MPON
#if 0
#define	RMSG_RX_INT_COUNT	16
#define	RMSG_RX_INT_DELAY	100
#define	RMSG_TX_INT_COUNT	32
#define	RMSG_TX_INT_DELAY	200
#else
#define	RMSG_RX_INT_COUNT	0
#define	RMSG_RX_INT_DELAY	0
#define	RMSG_TX_INT_COUNT	0
#define	RMSG_TX_INT_DELAY	0
#endif

#define FCSR_FSbit   1          /* default value of FS bit of FCSR register */

/******************************************************************************/

#if	FALSE			/* EXCLUDED FACILITIES */
#define SQRTD_OFF               /* hardware SQRTD remains OFF for a027 CPU version ! */
#define TRACE_ON		/* enable trace buffer */
#define	INCLUDE_PRIME_NVRAM	/* if possible use PRIME NVRAM */
#define	INCLUDE_RGB
#define	INCLUDE_IDE_TEST	/* IDE test routines */
#define	INCLUDE_FTL		/* FTL */
#define	INCLUDE_FTL_TEST	/* FTL test routines */
#define	INCLUDE_SCSI_TEST	/* SCSI test routines */
#define	INCLUDE_ECC		/* DRAM ECC */
#define	INCLUDE_PF_TEST		/* LPT LS-120 Parallel Floppy test routines */
#define INCLUDE_OLD_FLASH	/* old flash library */
#define CPU_RUN_UNCACHED
#endif	/* FALSE */

/******************************************************************************/

#ifndef __ASSEMBLER__

/* memory pool start address (typically end of image) */

#define FREE_RAM_ADRS		(_end)

/******************************************************************************/

/* TRACE definitons */

//#define	TRACE_MASK	0xffffffff	/* which sources should be tracing */
#define	TRACE_MASK	((1 << TR_START))

#define	TR_ERROR	0
#define	TR_INTR		1
#define	TR_PCI		8
#define	TR_START	16
#define	TR_VME		24
#define	TR_X6		26
#define	TR_X5		27
#define	TR_X4		28
#define	TR_X3		29
#define	TR_X2		30
#define	TR_X1		31

#ifdef TRACE_ON

/* trace whole 32 bit word */

#define	TRACE32(level,value)	\
    if((1 << (level)) & TRACE_MASK) { boardToTraceBuf((value)); }

/* trace word merged with level (upper byte) */

#define TRACE(level,value)	\
    TRACE32(level,(((level) << 24) | (value)))

/* size of trace buffer */

#define TRACE_BUF_SIZE	0x100000	/* 1MB */

extern void boardToTraceBuf(unsigned code);

#else	/* TRACE_ON */

#define TRACE(level,value)
#define TRACE32(level,value)
#define TRACE_BUF_SIZE	0

#endif	/* TRACE_ON */

#endif	/* __ASSEMBLER__ */

#endif   /* INCconfigbrdh  */
