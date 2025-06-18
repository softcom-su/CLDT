#include <stdio.h>
#include <pciLib.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arch/interrupt.h>
#include <signal.h>
#include <logLib.h>
#include <taskLib.h>
#include <cacheLib.h>
#include <system.h>
#include <board.h>

#if defined(MPON) && defined(K64RIO)
#error defined(MPON) && defined(K64RIO) !!!
#endif

#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))
#include <channel/mpchannel.h>
#include <channel/riochannel.h>
#endif

#include "configbrd.h"
#include "rio.h"

/* maintenance regs */

#define	CAR_DEV_ID		0x000000
#define	CAR_DEV_INFO		0x000004
#define	CAR_ASBLY_ID		0x000008
#define	CAR_ASBLY_INFO		0x00000c
#define	CAR_PE_FEAT		0x000010
#define	CAR_SWITCH_PI		0x000014
#define	CAR_SOURCE_OP		0x000018
#define	CAR_LUT_SIZE		0x000034
#define	CSR_BASE_DEV_ID		0x000060
#define	CSR_HB_DEV_ID_LOCK	0x000068
#define	CSR_COMP_TAG		0x00006c
#define	CSR_ROUTE_CFG_DESTID	0x000070
#define	CSR_ROUTE_CFG_PORT	0x000074
#define	CSR_LUT_ATTR		0x000078
#define	PORT_LT_CTL		0x000120
#define	PORT_RT_CTL		0x000124
#define	PORT_GEN_CTL		0x00013c
#define	PORT_ERR_STAT0		0x000158
#define	PORT_CTL		0x00015c
#define	PKT_TTL			0x00102c
#define	FAB_TEA_CTL0		0x0b6000
#define	PORT_UNRECOV_ERR_DET	0x010e00
#define	PORT_UNRECOV_ERR_ENABLE	0x010e04
#define	PORT_RECOV_ERR_DET	0x010e08

#define	TUNDRA_MPON_INT_CLOCK		200000000
#define	TUNDRA_SWITCH_INT_CLOCK		80000000
#define	MPON_INT_CLOCK			33333333
#define	LINK_CLOCK			125000000
#define	NIISI_SWITCH_INT_CLOCK		72000000
#define	MPON_PCI_CLOCK			33000000

/*
 *   Tundra FAB_TEA_CTLx value, 
 *    [15:0] - packet timeout divided by 2^15 at Internal Clock clock,
 *             1 is 164 us at 200 MHz or 410 us at 80 MHz
 *    [16] - timeout enable
*/

#define	TUNDRA_FAB_TEA_CTL_VAL		0x00010001	// enable packet timeout

/* Tundra PORT_LT_CTL value at Link Clock, divisor is unknown */

#define	TUNDRA_LT_CTL_VAL		0x0000800	// ? us Link Time-Out

/* Host PORT_RT_CTL value at MPON Internal Clock */

#define	HOST_RT_CTL_VAL			0x0001700	// 180 us response timeout at 33 MHz Clock

/* Host PORT_LT_CTL value, [31:8] timeout divided by 2^5 at 125 MHz Link Clock */

#define	HOST_LT_CTL_VAL			0x00000800	// 2 us Link Time-Out

/*
 *   NIISI PKT_TTL value, 
 *    [31:16] - packet Time To Live divided by 2^7 at Internal Clock,
 *             1 is 1600 ns at 80 MHz, or 1778 ns at 72 MHz
*/

#define	NIISI_PKT_TTL_VAL		0x005a0000	// 160 us TTL at 72 MHz
#define	HOST_PKT_TTL_VAL		0x009d0000	// 200 us TTL at 100 MHz

/* regs layout */

typedef struct {				// RIO		PCI
    volatile unsigned	ioBarSize;		// 11000	00000
    volatile unsigned	memBarSize;		// 11004	00004
    volatile unsigned	pfmemBarSize;		// 11008	00008
    volatile unsigned	addCtl;			// 1100c	0000c
    volatile unsigned	ga;			// 11010	00010
    volatile unsigned	axiTimers;		// 11014	00014
    volatile unsigned	scratch[2];		// 11018	00018
    volatile unsigned	semaphore;		// 11020	00020
    volatile unsigned	maintOutTimer;		// 11024	00024
    volatile unsigned	maintOut1;		// 11028	00028
    volatile unsigned	maintOut2;		// 1102c	0002c
    volatile unsigned	maintOut3;		// 11030	00030
    volatile unsigned	maintOut4;		// 11034	00034
    volatile unsigned	maintOut5;		// 11038	00038
    volatile unsigned	unused1;		// 1103c	0003c
    volatile unsigned	maintOut6;		// 11040	00040
    volatile unsigned	maintOut7;		// 11044	00044
    volatile unsigned	in1;			// 11048	00048
    volatile unsigned	in2;			// 1104c	0004c
    volatile unsigned	in3;			// 11050	00050
    volatile unsigned	unused2;		// 11054	00054
    volatile unsigned	in4;			// 11058	00058
    volatile unsigned	unused3;		// 1105c	0005c
    volatile unsigned	pktDisID;		// 11060	00060
    volatile unsigned	pktDisAddr;		// 11064	00064
    volatile unsigned	pktDisData;		// 11068	00068
    volatile unsigned	unused4;		// 1106c	0006c
    volatile unsigned	pktSerrID;		// 11070	00070
    volatile unsigned	pktSerrAddr;		// 11074	00074
    volatile unsigned	pktSerrData;		// 11078	00078
    volatile unsigned	unused5;		// 1107c	0007c
    struct {
	volatile unsigned id;			// 80, 90, a0, b0
	volatile unsigned addr;			// 84, 94, a4, b4
	volatile unsigned data0;		// 88, 98, a8, b8
	volatile unsigned data1;		// 8c, 9c, ac, bc
    } pktIntOut[4];
    struct {
	volatile unsigned stat;			// c0, c8, d0, d8
	volatile unsigned unused;		// c4, cc, d4, dc
    } pktIntIn[4];
    volatile unsigned	pktSerrIn;		// 110e0	000e0
    volatile unsigned	unused6;		// 110e4	000e4
    volatile unsigned	pciRioMode;		// 110e8	000e8
    volatile unsigned	unused7;		// 110ec	000ec
    volatile unsigned	wdoCountL;		// 110f0	000f0
    volatile unsigned	wdoCountH;		// 110f4	000f4
    volatile unsigned	timerCountL;		// 110f8	000f8
    volatile unsigned	timerCountH;		// 110fc	000fc
} RIO_PCI_USER_REGS;				// 11100	00100

typedef struct {				// RIO		PCI
    volatile unsigned	id;			// 11100	01000
    volatile unsigned	csr;			// 11104	01004
    volatile unsigned	class;			// 11108	01008
    volatile unsigned	misc1;			// 1110c	0100c
    volatile unsigned	ioBAR;			// 11110	01010
    volatile unsigned	memBAR;			// 11114	01014
    volatile unsigned	pfmemBAR;		// 11118	01018
    volatile unsigned	regsBAR;		// 1111c	0101c
    volatile unsigned	unused1[3];		// 11120	01020
    volatile unsigned	sid;			// 1112c	0102c
    volatile unsigned	unused2[3];		// 11130	01030
    volatile unsigned	misc2;			// 1113c	0103c
    volatile unsigned	bus;			// 11140	01040
    volatile unsigned	unused3[47];		// 11144	01044
} RIO_PCI_STD_REGS;				// 11200	01100

typedef struct {				// RIO,PCI
    volatile unsigned   errBlkHdr;              // 01000
    volatile unsigned   reserved1;              // 01004
    volatile unsigned   logErrDet;              // 01008
    volatile unsigned   logErrDetEn;            // 0100c
    volatile unsigned   reserved2;              // 01010
    volatile unsigned   logErrAddr;             // 01014
    volatile unsigned   logErrDevId;            // 01018
    volatile unsigned   logErrCtrlInfo;         // 0101c
    volatile unsigned   reserved3[2];           // 01020
    volatile unsigned   logErrIdPWR;            // 01028
    volatile unsigned   pktTTL;                 // 0102c
    volatile unsigned   unused[244];		// 01030
} RIO_ERR_CTRL_REGS;				// 01400

typedef struct {				// RIO		PCI
    volatile unsigned	ioTranslAddr[16];	// 12400	01400
    volatile unsigned	unused1[16];		// 12440	01440
    volatile unsigned	memTranslAddr[16];	// 12480	01480
    volatile unsigned	unused2[16];		// 124c0	014c0
    volatile unsigned	pfmemTranslAddr[16];	// 12500	01500
    volatile unsigned	unused3[16];		// 12540	01540
    volatile unsigned	ioID[16];		// 12580	01580
    volatile unsigned	memID[16];		// 125c0	015c0
    volatile unsigned	pfmemID[16];		// 12600	01600
    volatile unsigned	unused4[112];		// 12640	01640
    volatile unsigned	type1ID[256];		// 12800	01800
    volatile unsigned	cfg1TranslAddr[256];	// 12c00	01c00
} RIO_LUT_REGS;					// 13000	02000

typedef struct {				// RIO,PCI
    volatile unsigned	unused1[3];		// 10200
    volatile unsigned	ioBaseAddr;		// 1020c
    volatile unsigned	ioSize;			// 10210
    volatile unsigned	ioTranslAddr;		// 10214
    volatile unsigned	memBaseAddr;		// 10218
    volatile unsigned	memSize;		// 1021c
    volatile unsigned	memTranslAddr;		// 10220
    volatile unsigned	pfmemBaseAddr;		// 10224
    volatile unsigned	pfmemSize;		// 10228
    volatile unsigned	pfmemTranslAddr;	// 1022c
    volatile unsigned	cfg0BaseAddr;		// 10230
    volatile unsigned	unused2;		// 10234
    volatile unsigned	cfg1BaseAddr;		// 10238
    volatile unsigned	unused3;		// 1023c
    volatile unsigned	enable;			// 10240
    volatile unsigned	devInfo1;		// 10244
    volatile unsigned	asblyID;		// 10248
    volatile unsigned	asblyInfo;		// 1024c
    struct {
	volatile unsigned srcAddr;		// 10250, 10270
	volatile unsigned srcID;		// 10254, 10274
	volatile unsigned dstAddr;		// 10258, 10278
	volatile unsigned dstID;		// 1025c, 1027c
	volatile unsigned count;		// 10260, 10280
	volatile unsigned unused1;		// 10264, 10284
	volatile unsigned status;		// 10268, 10288
	volatile unsigned unused2;		// 1026c, 1028c
    } dma[2];
    volatile unsigned	ckiStatus;		// 10290
    volatile unsigned	ckiMask;		// 10294
    volatile unsigned	softRst;		// 10298
    volatile unsigned	wdoCtrl;		// 1029c
    volatile unsigned	portWrite[3];		// 102a0, 102a4, 102a8
    volatile unsigned	portWrStatus;		// 102ac
    volatile unsigned	unused6[20];		// 102b0
} RIO_SPEC_REGS;				// 10300

typedef struct {				// RIO,PCI
    volatile unsigned	msgOut[3];		// 10800, 10804, 10808
    volatile unsigned	unused1[3];		// 1080c
    volatile unsigned	msgOutStatus;		// 10818
    volatile unsigned	msgOutError;		// 1081c
    volatile unsigned	msgIn[2];		// 10820, 10824
    volatile unsigned	msgInStatus;		// 10828
    volatile unsigned	msgInError;		// 1082c
    volatile unsigned	dbOut[2];		// 10830, 10834
    volatile unsigned	dbOutStatus;		// 10838
    volatile unsigned	dbOutError;		// 1083c
    volatile unsigned	dbIn[2];		// 10840, 10844
    volatile unsigned	dbInCtrl;		// 10848
    volatile unsigned	dbInError;		// 1084c
    volatile unsigned	dbInStatus[4];		// 10850, 10854, 10858, 1085c
    volatile unsigned	miscIx;			// 10860
    volatile unsigned	miscData[2];		// 10864, 10868
    volatile unsigned	miscStatus;		// 1086c
    volatile unsigned	trap[4];		// 10870, 10874, 10878, 1087c
    volatile unsigned	trapStatus;		// 10880
    volatile unsigned	unused2;		// 10884
    volatile unsigned	inboundStatus;		// 10888
    volatile unsigned	unused3;		// 1088c
    volatile unsigned	priority;		// 10890
    volatile unsigned	unused4[3];		// 10894
    volatile unsigned	intrEnable;		// 108a0
    volatile unsigned   intrEnableAdd;          // 108a4
    volatile unsigned   msgIn3;                 // 108a8
    volatile unsigned	unused5;		// 108ac
    volatile unsigned	intrStatus;		// 108b0
    volatile unsigned   intrStatusAdd;          // 108b4
    volatile unsigned	unused6[18];		// 108b8
} RIO_ENDPOINT_REGS;				// 10900

typedef struct {				// RIO,PCI
    volatile unsigned	rw[4];			// 10c00, 10c04, 10c08, 10c0c
    volatile unsigned	unused1[12];		// 10c10
    volatile unsigned	status1;		// 10c40
    volatile unsigned	status2;		// 10c44
    volatile unsigned	unused2[2];		// 10c4c
    volatile unsigned	r[16];			// 10c50
    volatile unsigned	unused3[28];		// 10c90
} RIO_ERR_REGS;					// 10d00

typedef struct {				// RIO		PCI
    volatile unsigned	devID;			// 00000	11000
    volatile unsigned	devInfo;		// 00004	11004
    volatile unsigned	asblyID;		// 00008	11008
    volatile unsigned	asblyInfo;		// 0000c	1100c
    volatile unsigned	peFeat;			// 00010	11010
    volatile unsigned	switchPortInfo;		// 00014	11014
    volatile unsigned	srcOps;			// 00018	11018
    volatile unsigned	dstOps;			// 0001c	1101c
    volatile unsigned	unused[8];		// 00020	11020
} RIO_CAR_REGS;					// 00040	11040

typedef struct {				// RIO		PCI
    volatile unsigned	mbox;			// 00040	11040
    volatile unsigned	unused1;		// 00044	11044
    volatile unsigned	unused2;		// 00048	11048
    volatile unsigned	peLLCtl;		// 0004c	1104c
    volatile unsigned	unused3[3];		// 00050	11050
    volatile unsigned	cfgLBAR;		// 0005c	1105c
    volatile unsigned	baseDevID;		// 00060	11060
    volatile unsigned	unused4;		// 00064	11064
    volatile unsigned	hbDevIDLock;		// 00068	11068
    volatile unsigned	compTag;		// 0006c	1106c
    volatile unsigned	routeDstID;		// 00070	11070
    volatile unsigned	routePort;		// 00077	11074
    volatile unsigned	routeDefPort;		// 00078	11078
    volatile unsigned	unused5[33];		// 0007c	1107c
} RIO_CSR_REGS;					// 00100	11100

typedef struct {				// RIO		PCI
    volatile unsigned	mbh0;			// 00100	11100
    volatile unsigned	unused1[7];		// 00104	11104
    volatile unsigned	ltCtl;			// 00120	11120
    volatile unsigned	rtCtl;			// 00124	11124
    volatile unsigned	unused2[5];		// 00128	11128
    volatile unsigned	genCtl;			// 0013c	1113c
    volatile unsigned	unused3[6];		// 00140	11140
    struct {
	volatile unsigned errStat;		// 158, 178, 198
	volatile unsigned ctl;			// 15c, 17c, 19c
	volatile unsigned unused[6];		// 160, 180, 1a0
    } port[3];
    volatile unsigned	unused4[18];		// 001b8	111b8
    volatile unsigned	mbh1;			// 00200	11200
    volatile unsigned	unused5;		// 00204	11204
    volatile unsigned	errDetect;		// 00208	11208
    volatile unsigned	unused6[125];		// 0020c	1120c
} RIO_EXT_REGS;					// 00400	11400

typedef struct {				// RIO,PCI
    volatile unsigned unused[64];		// 20000
} RIO_IN_OUT_REGS;				// 20100

typedef struct {				// RIO,PCI 30000, 50000, 70000, 90000
    volatile unsigned	portConfig;		// 00
    volatile unsigned	trnStat;		// 04
    volatile unsigned	outQueDeep[2];		// 08, 0c
    volatile unsigned	outRetryErr[8];		// 10 ... 2c
    volatile unsigned	outPackErr[2];		// 30, 34
    volatile unsigned	inConfig;		// 38
    volatile unsigned	inQueDeep[2];		// 3c, 40
    volatile unsigned	inCrcErr;		// 44
    volatile unsigned	inReorder;		// 48
    volatile unsigned	inDataCount;		// 4c
    volatile unsigned	inPktCount;		// 50
    volatile unsigned	unused1[7];		// 54
    volatile unsigned	routeDstID;		// 70
    volatile unsigned	routePort;		// 74
    volatile unsigned	routeBase;		// 78
    volatile unsigned	unused2;		// 7c
    volatile unsigned	rdPktCount[4];		// 80, 84, 88, 8c
    volatile unsigned	wrPktCount[4];		// 90, 94, 98, 9c
    volatile unsigned	intrStat;		// a0
    volatile unsigned	intrEnable;		// a4
    volatile unsigned	inPktErrStat[3];	// a8, ac, b0
    volatile unsigned	unused3[32723];		// b4
} RIO_PORT_REGS;				// 20000

typedef struct {				// RIO,PCI
    volatile unsigned	unused1[6144];		// b0000
    volatile unsigned	ctl[3];			// b6000, b6004, b6008
    volatile unsigned	unused2[12];		// b0000
    volatile unsigned	stat;			// b603c
    volatile unsigned	unused3[10224];		// b6040
} RIO_FAB_REGS;					// c0000

typedef struct {				// PCI
    RIO_PCI_USER_REGS	user;			// 00000
    RIO_PCI_STD_REGS	std;			// 00100
    volatile unsigned	unused1[896];		// 00200
    RIO_ERR_CTRL_REGS	errCtrl;		// 01000
    RIO_LUT_REGS	lut;			// 01400
    volatile unsigned	unused2[14464];		// 02000
    RIO_SPEC_REGS	spec;			// 10200
    volatile unsigned	unused3[320];		// 10300
    RIO_ENDPOINT_REGS	endp;			// 10800
    volatile unsigned	unused4[192];		// 10900
    RIO_ERR_REGS	err;			// 10c00
    volatile unsigned	unused5[192];		// 10d00
    RIO_CAR_REGS	car;			// 11000
    RIO_CSR_REGS	csr;			// 11040
    RIO_EXT_REGS	ext;			// 11100
    volatile unsigned	unused6[15104];		// 11400
    RIO_IN_OUT_REGS	inOut;			// 20000
    volatile unsigned	unused7[16320];		// 20100
    RIO_PORT_REGS	port[4];		// 30000
    RIO_FAB_REGS	fab;			// b0000
} RIO_PCI_REGS;					// c0000

/* input doorbell status register bits */

#define	DB_IN_FAIL		0x00000800
#define	DB_IN_TAIL_DONE		0x00000400
#define	DB_IN_COMPLETE		0x00000200
#define	DB_IN_DONE		0x00000100
#define	DB_IN_STATUS		0x00000f00
#define	DB_IN_DESC_READ		0x00000008
#define	DB_IN_NOT_EMPTY		0x00000004
#define	DB_IN_MODE		0x00000002
#define	DB_IN_RUN		0x00000001

/* bits reserved for demultiplexor in doorbell data */

#define	DB_HANDLER_BITS		4
#ifdef MPON
#define	DB_HANDLER_COUNT	(1 << DB_HANDLER_BITS)
#else
#define	DB_HANDLER_COUNT	(1 << (DB_HANDLER_BITS-1))
#endif
#define	DB_HANDLER_MASK		(DB_HANDLER_COUNT-1)

/* output doorbell status register bits */

#define	DB_OUT_FAIL		0x00000800
#define	DB_OUT_DONE		0x00000100
#define	DB_OUT_RUN		0x00000001

/* dma status register bits */

#define	DMA_STAT_FAIL	0x00000800
#define	DMA_STAT_DONE	0x00000100
#define	DMA_STAT_RUN	0x00000001

#define	RIO_DEBUG(level,msg)	\
    if(level <= rioDebug)	\
	logMsg msg;


/******************************************************************************/

/* globals */

RIO_PCI_REGS* rioRegs = NULL;				// pointer to regs base
int rioMaintTout = 1000000;				// maintenance cycle timeout us
volatile unsigned* rioDbDesc;				// base address of doorbell descriptors
volatile int rioDbLock = FALSE;				// output doorbell lock flag
int rioBus, rioDev, rioRev;				// controller location on PCI and it revision number
int rioDebug = 1;					// debug level
int rioInitDoneFlag = FALSE;				// controller initialization flag
unsigned rioDmaBound[256];				// aligned up dma bounds
unsigned rioMaintDelay = 10;				// delay in us after maintenance cycle
unsigned rioCtrlRev = 0;                                // controller revision
unsigned rioMsgMode = 0;                                // message controller mode

/* locals */

static int rioIntsCount [32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int dbIntsInCount[256], dbIntsOutCount[256], dmaStartCount [256];
static VOIDFUNCPTR rioRtn[33];				// user interrupt handler routines
static int rioArg[32];					// user interrupt handler arguments
static VOIDFUNCPTR dbRtn[DB_HANDLER_COUNT];		// doorbell user interrupt handler routines
static int dbArg[DB_HANDLER_COUNT];			// doorbell user interrupt handler arguments
#ifndef MPON
static char dbOutCount[256];
static char dbInCount[256];
#endif
static pthread_mutex_t dbMutex;                         // doorbell mutex
static pthread_mutex_t maintMutex;			// maintenance mutex
#ifdef MPON
static int rioCrossType = -1;                           // crossboard type
#endif

#if OS_VERSION >= 300

/******************************************************************************/

/* init process shared mutex */

static int pshared_mutex_init(pthread_mutex_t* mutex, const char* name)
{
    pthread_mutexattr_t mattr;
    int res;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setname(&mattr,name);
    if((res = pthread_mutex_init(mutex,&mattr)))
        kprint("%s: %s pthread_mutex_init error=%s\n\r",__FUNCTION__,name,strerror(res));
    return res;
}

#endif

/******************************************************************************/

/* check initialization flag */

int rioInitDone(void)
{
    return rioInitDoneFlag;
}

/******************************************************************************/

/* enable interrupts routine  */

int rioIntEnable(unsigned mask)
{
    int val, m = intLock();
    rioRegs->endp.intrEnable |= CT_LE_L(mask);
    val = rioRegs->endp.intrEnable;
    intUnlock(m);
    return val;
}

/******************************************************************************/

/* disable interrupts routine  */

int rioIntDisable(unsigned mask)
{
    int val, m = intLock();
    rioRegs->endp.intrEnable &= ~CT_LE_L(mask);
    val = rioRegs->endp.intrEnable;
    intUnlock(m);
    return val;
}

/******************************************************************************/

/* interrupt connect routine  */

int rioIntConnect(unsigned irq, VOIDFUNCPTR rtn, int arg)
{
    if(irq >= 33)
	return -1;

    rioRtn[irq] = rtn;
    rioArg[irq] = arg;
    return 0;
}

/******************************************************************************/

/* interrupt service routine  */

extern void rmsgIntr(unsigned stat);

void rioIntr(unsigned unused)
{
    unsigned stat, ix, err = FALSE;

#if OS_VERSION < 300
    if(intNestLevel > 32)
	kprint("%s: intNestLevel=%d\n\r",__FUNCTION__,intNestLevel);
#endif
#if (defined(K64RIO) || defined(MPON_RIO_MSG))
    if((stat = rioRegs->endp.intrStatusAdd))
    {
        stat = CF_LE_L(stat);
        if(rioRtn[32])
           rioRtn[32](rioArg[32],stat);
        RIO_DEBUG(5,("%s: msg stat=%x\n",(long)__FUNCTION__,stat,0,0,0,0));
    }
#endif

    if(!(stat = rioRegs->endp.intrStatus))
	return;

    stat = CF_LE_L(stat);

    for(ix = 0; ix < 32; ix++)
    {
	if(!(stat & (1 << ix)))
	    continue;

        rioIntsCount[ix]++;

	if(rioRtn[ix])
	{
	    rioRtn[ix](rioArg[ix],stat);
	}
	else
	{
// 	    *(volatile unsigned*)(0xb2100000) = 0;
	    err = TRUE;
	    kprint("%s: unhandled IRQ=%d\n\r",__FUNCTION__,ix);
	}
    }

    if(err)
    {
	unsigned mask = rioRegs->endp.intrEnable;
	mask = CF_LE_L(mask);
	kprint("%s: stat=%08x mask=%08x\n\r",__FUNCTION__,stat,mask);
    }

    RIO_DEBUG(5,("%s: stat=%x\n",(long)__FUNCTION__,stat,0,0,0,0));
}

/******************************************************************************/

/* maintenance cycle in/out routine  */

int rioMaintInOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data,
    unsigned* pData, const char* fname, unsigned* err)
{
    unsigned val, ix, addStat;

    if(addr & ~0xfffffc)
    {
inval:
        RIO_DEBUG(1,("%s: invalid param: hopCount=%x addr=%x dstID=%x\n",
            (long)fname,hopCount,addr,dstID,0,0));
        if(err)
            *err = EINVAL;
        return -1;
    }

    /* access host regs */

    if(dstID == 0xffffffff && !hopCount)
    {
        volatile unsigned* p;

        switch(addr)
        {
            case 0 ... 0x3fc:
                addr += 0x11000;
                break;
            case 0x11000 ... 0x12ffc:
                addr -= 0x11000;
                break;
        }

        p = (void*)rioRegs + addr;

        if(pData)
        {
            val = *p;
            *pData = CF_LE_L(val);
        }
        else
        {
            *p = CT_LE_L(data);
        }
        return 0;
    }

    if(hopCount > 0xff || dstID > 0xff)
        goto inval;

    pthread_mutex_lock(&maintMutex);

    /* clear status */

    rioRegs->user.maintOut5 = rioRegs->user.maintOut5;

    val = (dstID & 0xff) | (((addr & 4) ? 0xf0 : 0x0f) << 24);

    if(!pData)
    {
	val |= 0x100000;

	if(addr & 4)
	    rioRegs->user.maintOut3 = CT_LE_L(data);
	else
	    rioRegs->user.maintOut2 = CT_LE_L(data);
    }

    rioRegs->user.maintOut1 = CT_LE_L(val);
    rioRegs->user.maintOut4 = CT_LE_L((hopCount << 24) | addr);

    for(ix = 0; ix < rioMaintTout; ix++)
    {
	val = rioRegs->user.maintOut5;

	if(val & CT_LE_L(1))
	{
	    rioRegs->user.maintOut5 = val;
#if 1
	    if(val & CT_LE_L(0x10000))
	    {
		addStat = rioRegs->err.status1;
		RIO_DEBUG(1,("%s: error: hopCount=%x addr=%x dstID=%x stat=%lx add=%lx\n",
		    (long)fname,hopCount,addr,dstID,CF_LE_L(val),CF_LE_L(addStat)));
		pthread_mutex_unlock(&maintMutex);
                if(err)
                    *err = EIO;
                return -1;
	    }
#else
#warning K128 test code
	    val = CF_LE_L(val);
	    if((val & 0x10000) && (val & 0x0e70000) != 0x50000)
	    {
		addStat = rioRegs->err.status1;
		RIO_DEBUG(1,("%s: error: hopCount=%x addr=%x dstID=%x stat=%lx add=%lx\n",
		    (long)fname,hopCount,addr,dstID,val,CF_LE_L(addStat)));
		pthread_mutex_unlock(&maintMutex);
                if(err)
                    *err = EIO;
                return -1;
	    }
#endif
	    if(pData)
	    {
		if(addr & 4)
		    val = rioRegs->user.maintOut7;
		else
		    val = rioRegs->user.maintOut6;

		*pData = CF_LE_L(val);
	    }

	    if(rioMaintDelay)
		udelay(rioMaintDelay);

	    pthread_mutex_unlock(&maintMutex);
	    return 0;
	}
	udelay(1);
    }

    addStat = rioRegs->err.status1;
    RIO_DEBUG(1,("%s: timeout: hopCount=%x addr=%x dstID=%x stat=%lx add=%lx\n",
	(long)fname,hopCount,addr,dstID,CF_LE_L(val),CF_LE_L(addStat)));
    pthread_mutex_unlock(&maintMutex);
    if(err)
        *err = ETIMEDOUT;
    return -1;
}

/******************************************************************************/

/* maintenance cycle in routine  */

int rioMaintIn(unsigned hopCount, unsigned addr, unsigned dstID, unsigned* pData)
{
    if(!pData)
	return -1;

    return rioMaintInOut(hopCount,addr,dstID,0,pData,__FUNCTION__,NULL);
}

/******************************************************************************/

/* maintenance cycle out routine  */

int rioMaintOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data)
{
    return rioMaintInOut(hopCount,addr,dstID,data,NULL,__FUNCTION__,NULL);
}

/******************************************************************************/

/* input doorbell interrupt service routine  */

void rioDbInIntr(int inMode, unsigned stat)
{
    unsigned val, err, id, data, i, tail, v1, v2, j;

    val = rioRegs->endp.dbInCtrl;

    do
    {
	if(val & CT_LE_L(DB_IN_FAIL))
	{
	    rioRegs->endp.dbInCtrl = val;
	    err = rioRegs->endp.dbInError;
	    rioRegs->endp.dbInError = err;
	    RIO_DEBUG(1,("%s: error: stat=%lx err=%lx\n",(long)__FUNCTION__,CF_LE_L(val),CF_LE_L(err),0,0,0));
	}

	if(val & CT_LE_L(DB_IN_TAIL_DONE))
	{
	    rioRegs->endp.dbInCtrl = CT_LE_L(DB_IN_TAIL_DONE);
	    kprint("%s: doorbell overflow stat=%x\n\r",__FUNCTION__,CF_LE_L(val));
	}
    
	if(val & CT_LE_L(DB_IN_DONE|DB_IN_NOT_EMPTY))
	{
	    if(!(val & CT_LE_L(DB_IN_NOT_EMPTY)))
	    {
		rioRegs->endp.dbInCtrl = val;
		return;
	    }
	    
	    tail = rioRegs->endp.dbInStatus[3];
	    tail = CF_LE_L(tail) & 0x7f;
	    for(j = 0; j < 100; j++)
	    {
		v1 = rioDbDesc[tail*2];
		v2 = rioDbDesc[tail*2+1];
		if(v1 != 0xffffffff && v2 != 0xffffffff)
		    break;
		if(j > 80)
		    udelay(1);
	    }
#if 0
	    id = v1 = rioDbDesc[tail*2];
	    id = CF_LE_L(id) & 0xff;
	    data = v2 = rioDbDesc[tail*2+1];
	    data = CF_LE_L(data) & 0xffff;
#else
	    if(v1 == 0xffffffff || v2 == 0xffffffff)
	    {
		unsigned head = rioRegs->endp.dbInStatus[2];
		head = CF_LE_L(head) & 0x7f;
		RIO_DEBUG(1,("%s: v1=%08x v2=%08x tail=%x head=%x\n",(long)__FUNCTION__,v1,v2,tail,head,0));
	    }
	    id = CF_LE_L(v1) & 0xff;
	    data = CF_LE_L(v2) & 0xffff;
#endif
	    rioDbDesc[tail*2] = 0xffffffff;
	    rioDbDesc[tail*2+1] = 0xffffffff;
	    rioRegs->endp.dbInCtrl = val|CT_LE_L(DB_IN_DESC_READ);
	    if(rioDebug >= 3)
	    {
		unsigned head = rioRegs->endp.dbInStatus[2];
		head = CF_LE_L(head) & 0x7f;
		RIO_DEBUG(3,("%s: id=%02x data=%04x stat=%lx tail=%x head=%x\n",(long)__FUNCTION__,
		    id,data,CF_LE_L(val),tail,head));
	    }

            i = (id < RIO_ID_MAX ? id : RIO_ID_MAX-1); dbIntsInCount[i]++;

	    i = data & DB_HANDLER_MASK;

#ifndef MPON
	    j = (data >> (DB_HANDLER_BITS-1)) & 1;

	    if(dbInCount[id] != -1)
	    {
		if(j != dbInCount[id])
		{
		    RIO_DEBUG(1,("%s: duplicated doorbell: id=%02x data=%04x\n",
			(int)__FUNCTION__,id,data,0,0,0));
		    goto next;
		}
	    }

	    dbInCount[id] = j ^ 1;

#endif
	    if(dbRtn[i])
	    {
		dbRtn[i](dbArg[i],data >> DB_HANDLER_BITS,id);
	    }
	    else
	    {
		RIO_DEBUG(2,("%s: user handler not connected: id=%02x data=%04x stat=%lx\n",
		    (long)__FUNCTION__,id,data,CF_LE_L(val),0,0));
	    }
	}
#ifndef MPON
next:
#endif	
	val = rioRegs->endp.dbInCtrl;

    } while(val & CT_LE_L(DB_IN_NOT_EMPTY));
}

/******************************************************************************/

/* doorbell interrupt connect routine  */

int rioDbIntConnect(unsigned irq, VOIDFUNCPTR rtn, int arg)
{
    int mask;

    if(irq >= DB_HANDLER_COUNT)
	return -1;

    mask = intLock();
    dbRtn[irq] = rtn;
    dbArg[irq] = arg;
    intUnlock(mask);
    return 0;
}

/******************************************************************************/

/* input doorbell initialization routine  */

static void dbInInit(int inMode)
{
    unsigned val;

    rioDbDesc = _cacheMalloc(128*8,0x1000);

    if(!rioDbDesc)
    {
	kprint("%s: _cacheMalloc %d bytes failed\n\r",__FUNCTION__,128*8);
	return;
    }

#ifndef MPON
    for(val = 0; val < 256; val++)
	dbInCount[val] = -1;
#endif

    memset((void*)rioDbDesc,0xff,128*8);

    val = pciLocalToBus((void*)rioDbDesc);
    rioRegs->endp.dbIn[0] = CT_LE_L(val);		// decriptor address
    rioRegs->endp.dbIn[1] = CT_LE_L(3);			// [7:0] line size (0 - 16, 3 - 128) 
    rioRegs->endp.dbInError = rioRegs->endp.dbInError;	// clear errors
    rioRegs->endp.dbInCtrl = CT_LE_L(DB_IN_STATUS);	// clear all status bits
    val = (inMode << 1) | DB_IN_RUN;			// set mode and enable input
    rioRegs->endp.dbInCtrl = CT_LE_L(val);
    rioIntConnect(IRQ_DB_IN_DONE,rioDbInIntr,inMode);	// connect common doorbell handler
    rioIntConnect(IRQ_DB_IN_TAIL,rioDbInIntr,inMode);	// connect common doorbell overflow handler
    rioIntConnect(IRQ_DB_IN_ERR,rioDbInIntr,inMode);	// connect common doorbell error handler
    rioIntEnable(INTR_DB_IN_DONE|INTR_DB_IN_TAIL|INTR_DB_IN_ERR);	// enable input, error and overflow doorbell interrupts
#if OS_VERSION >= 300
    pshared_mutex_init(&dbMutex,"rioDb");
#else
    pthread_mutex_init(&dbMutex,NULL);
#endif
}

/******************************************************************************/

/* send doorbell */

int rioDbOut(unsigned dstID, unsigned i, unsigned data)
{
    unsigned val, err, ix, mask, dbval, retry = 10;

    if(i >= DB_HANDLER_COUNT)
    {
	RIO_DEBUG(1,("%s: invalid handler number=%d\n",(long)__FUNCTION__,i,0,0,0,0));
	return EINVAL;
    }

    mask = intLock();

    if(rioDbLock)
    {
	intUnlock(mask);
	return EAGAIN;
    }

    rioDbLock = TRUE;
    intUnlock(mask);

    if(dstID < RIO_ID_MAX)
	dbIntsOutCount[dstID]++; 

    if((val = rioRegs->endp.dbOutStatus) & CF_LE_L(DB_OUT_RUN))
    {
	err = rioRegs->endp.dbOutError;
	rioRegs->endp.dbOutError = err;
	rioRegs->endp.dbOutStatus = val;
	RIO_DEBUG(1,("%s: busy: stat=%lx err=%lx\n",(long)__FUNCTION__,CF_LE_L(val),CF_LE_L(err),0,0,0));
        rioDbLock = FALSE;
	return EBUSY;
    }

    rioRegs->endp.dbOutError = rioRegs->endp.dbOutError;
    rioRegs->endp.dbOutStatus = rioRegs->endp.dbOutStatus;

    dbval = (data << DB_HANDLER_BITS) | i;
#ifndef MPON
    dbval |= dbOutCount[dstID] << (DB_HANDLER_BITS-1);
#endif
again:
    rioRegs->endp.dbOut[0] = CT_LE_L(dstID);
    rioRegs->endp.dbOut[1] = CT_LE_L(dbval);
    rioRegs->endp.dbOutStatus = CT_LE_L(DB_OUT_RUN);

    for(ix = 0; ix < 100000; ix++)
    {
	val = rioRegs->endp.dbOutStatus;

	if(val & CF_LE_L(DB_OUT_DONE))
	{
	    rioRegs->endp.dbOutStatus = val;

	    if(val & CF_LE_L(DB_OUT_FAIL))
	    {
		err = rioRegs->endp.dbOutError;
		rioRegs->endp.dbOutError = err;
		if(retry--)
		    goto again;
		RIO_DEBUG(1,("%s: error: dstID=%d value=%04x stat=%lx err=%lx\n",(long)__FUNCTION__,
		    dstID,dbval,CF_LE_L(val),CF_LE_L(err),0));
    		rioDbLock = FALSE;
		return EIO;
	    }

    	    rioDbLock = FALSE;
	    RIO_DEBUG(3,("%s: dstID=%d i=%d data=%03x val=%04x OK\n",(int)__FUNCTION__,dstID,i,data,dbval,0));
#ifndef MPON
	    dbOutCount[dstID] ^= 1;
#endif
	    return 0;
	}

	udelay(1);
    }

    err = rioRegs->endp.dbOutError;
    rioRegs->endp.dbOutStatus = 0;
    RIO_DEBUG(1,("%s: timeout: stat=%lx err=%lx\n",(long)__FUNCTION__,CF_LE_L(val),CF_LE_L(err),0,0,0));
    rioDbLock = FALSE;
    return ETIMEDOUT;
}

int _rioDbOut(unsigned dstID, unsigned i, unsigned data)
{
    int res;
    pthread_mutex_lock(&dbMutex);
    res = rioDbOut(dstID,i,data);
    pthread_mutex_unlock(&dbMutex);
    return res;
}

/******************************************************************************/

/* dump lookup table */

void lutShow(unsigned port, unsigned char lut[], unsigned maxId)
{
    unsigned i, j;
    if(port != -1)
	printf("PORT%d ",port);
    printf("LUT:\n");
    for(i = 0; i < maxId;)
    {
	printf("  ");
	for(j = 0; j < 16 && i < maxId; j++)
	    printf("%02x ",lut[i++]);
	printf("\n");
    }
}

/******************************************************************************/

/* display switch or endpoint regs */

void rioHopShow(unsigned hop, unsigned id, int errClear, unsigned maxId)
{
    unsigned val, devid, nPort, ix, stdRoute;

    if(!maxId)
	maxId = RIO_ID_MAX;
    if(maxId > 256)
	maxId = 256;

    if(rioMaintIn(hop,CAR_DEV_ID,id,&val) == -1)
	return;
    printf("R%03x=%08x DEV_ID\n",CAR_DEV_ID,val);
    devid = val;
    if(rioMaintIn(hop,CAR_DEV_INFO,id,&val) == -1)
	return;
    printf("R%03x=%08x DEV_INFO\n",CAR_DEV_INFO,val);
    if(rioMaintIn(hop,CAR_ASBLY_ID,id,&val) == -1)
	return;
    printf("R%03x=%08x ASBLY_ID\n",CAR_ASBLY_ID,val);
    if(rioMaintIn(hop,CAR_ASBLY_INFO,id,&val) == -1)
	return;
    printf("R%03x=%08x ASBLY_INFO\n",CAR_ASBLY_INFO,val);
    if(rioMaintIn(hop,CAR_PE_FEAT,id,&val) == -1)
	return;
    printf("R%03x=%08x PE_FEAT\n",CAR_PE_FEAT,val);
    stdRoute = val & 0x100;
    if(rioMaintIn(hop,CAR_SWITCH_PI,id,&val) == -1)
	return;
    printf("R%03x=%08x SWITCH_PI\n",CAR_SWITCH_PI,val);
    nPort = (val >> 8) & 0xff;
    if(rioMaintIn(hop,CAR_SOURCE_OP,id,&val) == -1)
	return;
    printf("R%03x=%08x SOURCE_OP\n",CAR_SOURCE_OP,val);
    if(devid == 0x87100074)
    {
	if(rioMaintIn(hop,CSR_BASE_DEV_ID,id,&val) == -1)
	    return;
	printf("  R%05x=%08x BASE_DEV_ID\n",CSR_BASE_DEV_ID,val);
	if(rioMaintIn(hop,0x11010,id,&val) == -1)
	    return;
	printf("  R%05x=%08x GA\n",0x11010,val);
    }
    if(rioMaintIn(hop,CSR_HB_DEV_ID_LOCK,id,&val) == -1)
	return;
    printf("R%03x=%08x HB_DEV_ID_LOCK\n",CSR_HB_DEV_ID_LOCK,val);
    if(rioMaintIn(hop,CSR_BASE_DEV_ID,id,&val) == -1)
        return;
    printf("R%03x=%08x CSR_BASE_DEV_ID\n",CSR_BASE_DEV_ID,val);
    if(rioMaintIn(hop,CSR_COMP_TAG,id,&val) == -1)
	return;
    printf("R%03x=%08x COMP_TAG\n",CSR_COMP_TAG,val);
    if(rioMaintIn(hop,PORT_GEN_CTL,id,&val) == -1)
	return;
    printf("R%03x=%08x PORT_GEN_CTL\n",PORT_GEN_CTL,val);

    if((devid & 0xffff) == 0x0074)
    {
	if(rioMaintIn(hop,PKT_TTL,id,&val) == -1)
	    return;
	printf("R%03x=%08x PKT_TTL\n",PKT_TTL,val);
    }

    for(ix = 0; ix < nPort; ix++)
    {
	if(rioMaintIn(hop,PORT_ERR_STAT0+ix*0x20,id,&val) == -1)
	    return;
	printf("    PORT=%d R%05X=%08x ERR_STAT:    %s\n",ix,PORT_ERR_STAT0+ix*0x20,val,(val & 2) ? "OK" : "ERROR");
	if(errClear && rioMaintOut(hop,PORT_ERR_STAT0+ix*0x20,id,val) == -1)
	    return;
	if(rioMaintIn(hop,PORT_CTL+ix*0x20,id,&val) == -1)
	    return;
	printf("           R%05X=%08x CTL\n",PORT_CTL+ix*0x20,val);
	if(devid == 0x0500000d)
	{
	    if(rioMaintIn(hop,FAB_TEA_CTL0+ix*4,id,&val) == -1)
		return;
	    printf("           R%05X=%08x FAB_TEA_CTL\n",FAB_TEA_CTL0+ix*4,val);
	    if(rioMaintIn(hop,PORT_UNRECOV_ERR_DET+ix*0x20000,id,&val) == -1)
		return;
	    printf("           R%05X=%08x UNRECOV_ERR_DET\n",PORT_UNRECOV_ERR_DET+ix*0x20000,val);
	    if(errClear && val)
	      rioMaintOut(hop,PORT_UNRECOV_ERR_DET+ix*0x20000,id,val);
	    if(rioMaintIn(hop,PORT_RECOV_ERR_DET+ix*0x20000,id,&val) == -1)
		return;
	    printf("           R%05X=%08x RECOV_ERR_DET\n",PORT_RECOV_ERR_DET+ix*0x20000,val);
	    if(errClear && val)
	      rioMaintOut(hop,PORT_RECOV_ERR_DET+ix*0x20000,id,val);
	}
    }

    if(devid == 0x0500000d)
    {
	unsigned port, i, j; unsigned char lut[256];
	for(port = 0; port < nPort; port++)
	{
	    for(ix = 0, i = 0; ix < 128; ix += 4)
	    {
		if(rioMaintIn(hop,0x10a00+ix+port*0x20000,id,&val) == -1)
		    return;
		for(j = 0; j < 8; j++, val >>= 4)
		    lut[i++] = (val & 0xf);
	    }
	    lutShow(port,lut,maxId);
	}
    }
    else if(devid == 0x00010074)
    {
	unsigned port; unsigned char lut[256];
	for(port = 0; port < nPort; port++)
	{
	    for(ix = 0; ix < 256; ix++)
	    {
		if(rioMaintOut(hop,0x10070+port*0x400,id,ix) == -1)
		    return;
		if(rioMaintIn(hop,0x10074+port*0x400,id,&val) == -1)
		    return;
		lut[ix] = val & 0xff;
	    }
	    lutShow(port,lut,maxId);
	}
    }
    else
    {
	unsigned char lut[256];
	if(!stdRoute)
	    printf("Warning: standard route configuration not supported!!!\n");
	if(rioMaintIn(hop,CAR_LUT_SIZE,id,&val) == -1)
	    return;
	printf("R%03x=%08x LUT_SIZE\n",CAR_LUT_SIZE,val);
	if(rioMaintIn(hop,CSR_LUT_ATTR,id,&val) == -1)
	    return;
	printf("R%03x=%08x LUT_ATTR\n",CSR_LUT_ATTR,val);
	for(ix = 0; ix < 256; ix++)
	{
	    if(rioMaintOut(hop,CSR_ROUTE_CFG_DESTID,id,ix) == -1)
		return;
	    if(rioMaintIn(hop,CSR_ROUTE_CFG_PORT,id,&val) == -1)
		return;
	    lut[ix] = val & 0xff;
	}
	lutShow(-1,lut,maxId);
    }
}

/******************************************************************************/

/* show interrupt status */

static void intrShow(unsigned val)
{
    unsigned ix;

    static const char* n[32] = {
	"MSG_OUT_OWN",
	"MSG_OUT_COMPLETE",
	"MSG_OUT_EOL",
	"MSG_OUT_ERR",
	"MSG_IN_DONE",
	"MSG_IN_COMPLETE",
	"MSG_IN_EOL",
	"MSG_IN_ERR",
	"DB_OUT_DONE",
	"DB_OUT_ERR",
	"RESERVED9",
	"RESERVED10",
	"DB_IN_DONE",
	"DB_IN_COMPLETE",
	"DB_IN_TAIL",
	"DB_IN_ERR",
	"MISC_DONE",
	"MANUAL_IN_DONE",
	"MANUAL_IN_OVF",
	"MSG_IN_OWN",
	"MSG_OUT_POL",
	"TRAP1_DONE",
	"MSG_IN_POL",
	"TRAP3_DONE",
	"MAINT_OUT_DONE",
	"RESERVED25",
	"DMA0_DONE",
	"DMA1_DONE",
	"IN_DONE",
	"TR_ERR",
	"DST_ID_ERR",
	"PORT_WR_DONE"
    };

    printf("    ");
    for(ix = 0; ix < 32; ix++)
    {
	if(val & (1 << ix))
	    printf("%s ",n[ix]);
    }
    printf("\n");
}

/******************************************************************************/

/* print firmware date */

static void rioDate(void)
{
    unsigned val, day, mon, hour, min, mask, val2, mux, year, rev;

    mask = intLock();
    mux = rioRegs->err.rw[0];
    rioRegs->err.rw[0] = mux & ~CT_LE_L(0x400000);
    val = rioRegs->err.r[13];
    val = CF_LE_L(val);
    rioRegs->err.rw[0] = mux | CT_LE_L(0x400000);
    val2 = rioRegs->err.r[13];
    val2 = CF_LE_L(val2);
    rioRegs->err.rw[0] = mux;
    intUnlock(mask);

    day = val >> 24;
    mon = (val >> 16) & 0xff;
    hour = (val >> 8) & 0xff;
    min = val & 0xff;

    if(val2 != val)
    {
        year = val2 & 0xffff;
        rev = val2 >> 16;
        printf("Firmware Date: %02x.%02x.%x %02x:%02x rev=%x\n",day,mon,year,hour,min,rev);
    }
    else
    {
        printf("Firmware Date: %02x.%02x.200x %02x:%02x\n",day,mon,hour,min);
    }
}

/******************************************************************************/

/* parse bridge registers routine */

void rioShow(int errClear)
{
    unsigned temp, val, bits, ix;
#ifndef	K64RIO
    char* s;
#endif

    if(!rioRegs)
    {
	printf("Bridge not initialized\n");
	return;
    }

#ifndef	K64RIO
    printf("PCI-TO-RIO: BUS=%d DEVICE=%d ID=0x%08x REV=%d ",
	rioBus,rioDev,pciConfigInLong(rioBus,rioDev,0,0),rioRev);
#endif

    rioDate();
    printf("\n");

#ifndef	K64RIO
    temp = pciConfigInLong(rioBus,rioDev,0,0x04);
    printf("Command/Status: 0x%08x\n",temp);
    printf("\tMemory access:\t%s\n",(temp & 2) ? "ENABLE" : "DISABLE");
    printf("\tBus master:\t%s\n",(temp & 4) ? "ENABLE" : "DISABLE");
    printf("\tParity report:\t%s\n",(temp & 0x40) ? "ENABLE" : "DISABLE");
    printf("\tSERR# driver:\t%s\n",(temp & 0x100) ? "ENABLE" : "DISABLE");
    if(temp & 0xf9000000)
    {
	printf("\tERRORS:\n");
	if(errClear)
	    pciConfigOutLong(rioBus,rioDev,0,0x04,temp);
    }
    if(temp & 0x01000000) printf("\t\tDATA PARITY REPORTED\n");
    if(temp & 0x08000000) printf("\t\tSIGNALED TARGET ABORT\n");
    if(temp & 0x10000000) printf("\t\tRECEIVED TARGET ABORT\n");
    if(temp & 0x20000000) printf("\t\tRECEIVED MASTER ABORT\n");
    if(temp & 0x40000000) printf("\t\tSIGNALED SYSTEM ERROR\n");
    if(temp & 0x80000000) printf("\t\tDETECTED PARITY ERROR\n");

    temp = pciConfigInLong(rioBus,rioDev,0,0x10);
    printf("\nBAR0 (IO WINDOWS):         PCI=0x%08x\n",temp);
    temp = pciConfigInLong(rioBus,rioDev,0,0x14);
    printf(  "BAR1 (MASTER WINDOWS):     PCI=0x%08x LOCAL=%#x\n",temp,pciMemToLocal(temp));
    temp = pciConfigInLong(rioBus,rioDev,0,0x18);
    printf(  "BAR2 (PREFETCHED WINDOWS): PCI=0x%08x\n",temp);
    temp = pciConfigInLong(rioBus,rioDev,0,0x1c);
    printf(  "BAR3 (BRIDGE REGISTERS):   PCI=0x%08x LOCAL=%#x\n\n",temp,pciMemToLocal(temp));
#endif

    val = rioRegs->user.ga;
    val = CF_LE_L(val);
    printf("GA: REG=%p VAL=%08x\n",&rioRegs->user.ga,val);
    bits = (val >> 8) & 0x1f;
    printf("    [12:8]  = %02x:\tPA\n",bits);
#ifdef MPON
    bits = (val >> 7) & 0x1;
    printf("    [7]     = %x:\tGA Is %s\n",bits,bits ? "Valid" : "Not Valid!!!");
    bits = (val >> 5) & 0x1;
    printf("    [5]     = %x:\tGA Parity\n",bits);
#else
    bits = (val >> 5) & 0x1;
    printf("    [5]     = %x: \tCPU GA\n",bits);
#endif
    bits = val & 0x1f;
    printf("    [4:0]   = %02x:\tGA\n\n",bits);

    val = rioRegs->csr.cfgLBAR;
    val = CF_LE_L(val);
    printf("CFG_LBAR: REG=%p VAL=%08x\n",&rioRegs->csr.cfgLBAR,val);
    bits = val & 0x3ff;
    printf("    [9:0]   = %03x:\tLBAR=%x%08x\n\n",bits,bits >> 8,bits << 24);

    val = rioRegs->csr.baseDevID;
    val = CF_LE_L(val);
    printf("BASE_DEV_ID: REG=%p VAL=%08x\n",&rioRegs->csr.baseDevID,val);
    bits = (val >> 16) & 0xff;
    printf("    [23:16] = %02x:\tID8\n",bits);
    bits = val & 0xffff;
    printf("    [15:0]  = %04x:\tID16\n\n",bits);

    val = rioRegs->ext.port[0].ctl;
    val = CF_LE_L(val);
    printf("PORT0_CTL: REG=%p VAL=%08x\n",&rioRegs->ext.port[0].ctl,val);
    bits = (val >> 30) & 0x1;
    printf("    [30]    = %x:\tTX Enable\n",bits);
    bits = (val >> 26) & 0x1;
    printf("    [26]    = %x:\tRX Enable\n\n",bits);

    val = rioRegs->ext.port[0].errStat;
    if(errClear)
	rioRegs->ext.port[0].errStat = val;
    val = CF_LE_L(val);
    printf("PORT0_ERR_STAT: REG=%p VAL=%08x\n",&rioRegs->ext.port[0].errStat,val);
    bits = (val >> 25) & 0x1;
    if(bits)
	printf("    [25]    = %x:\tError Fail Threshold Overflow\n",bits);
    bits = (val >> 24) & 0x1;
    if(bits)
	printf("    [24]    = %x:\tError Degree Threshold Overflow\n",bits);
    bits = (val >> 20) & 0x1;
    if(bits)
	printf("    [20]    = %x:\tOutput Retry-Stopped Happen\n",bits);
    bits = (val >> 19) & 0x1;
    if(bits)
	printf("    [19]    = %x:\tOutput Retry Active\n",bits);
    bits = (val >> 18) & 0x1;
    if(bits)
	printf("    [18]    = %x:\tOutput Retry-Stopped State\n",bits);
    bits = (val >> 17) & 0x1;
    if(bits)
	printf("    [17]    = %x:\tOutput Error-Stopped Happen\n",bits);
    bits = (val >> 16) & 0x1;
    if(bits)
	printf("    [16]    = %x:\tOutput Error-Stopped State\n",bits);
    bits = (val >> 10) & 0x1;
    if(bits)
	printf("    [10]    = %x:\tInput Retry-Stopped State\n",bits);
    bits = (val >> 9) & 0x1;
    if(bits)
	printf("    [9]     = %x:\tInput Error-Stopped Happen\n",bits);
    bits = (val >> 8) & 0x1;
    if(bits)
	printf("    [8]     = %x:\tInput Error-Stopped State\n",bits);
    bits = (val >> 3) & 0x1;
    if(bits)
	printf("    [3]     = %x:\tRX Clock Active\n",bits);
    bits = (val >> 2) & 0x1;
    if(bits)
	printf("    [2]     = %x:\tUnrecoverable output error\n",bits);
    bits = (val >> 1) & 0x1;
    if(bits)
	printf("    [1]     = %x:\tInitialization Complete\n",bits);
    bits = (val >> 0) & 0x1;
    if(bits)
	printf("    [0]     = %x:\tInitialization In Progress\n",bits);
    printf("\n");

    val = rioRegs->ext.port[1].ctl;
    val = CF_LE_L(val);
    printf("PORT1_CTL: REG=%p VAL=%08x\n",&rioRegs->ext.port[0].ctl,val);
    bits = (val >> 22) & 0x1;
    printf("    [22]    = %x:\tTX Enable\n",bits);
    bits = (val >> 21) & 0x1;
    printf("    [21]    = %x:\tRX Enable\n\n",bits);
    
    val = rioRegs->ext.port[1].errStat;
    if(errClear)
        rioRegs->ext.port[1].errStat = val;
    val = CF_LE_L(val);
    printf("PORT1_ERR_STAT: REG=%p VAL=%08x\n",&rioRegs->ext.port[0].errStat,val);
    bits = (val >> 25) & 0x1;
    if(bits)
        printf("    [25]    = %x:\tError Fail Threshold Overflow\n",bits);
    bits = (val >> 24) & 0x1;
    if(bits)
        printf("    [24]    = %x:\tError Degree Threshold Overflow\n",bits);
    bits = (val >> 20) & 0x1;
    if(bits)
        printf("    [20]    = %x:\tOutput Retry-Stopped Happen\n",bits);
    bits = (val >> 19) & 0x1;
    if(bits)
        printf("    [19]    = %x:\tOutput Retry Active\n",bits);
    bits = (val >> 18) & 0x1;
    if(bits)
        printf("    [18]    = %x:\tOutput Retry-Stopped State\n",bits);
    bits = (val >> 17) & 0x1;
    if(bits)
        printf("    [17]    = %x:\tOutput Error-Stopped Happen\n",bits);
    bits = (val >> 16) & 0x1;
    if(bits)
        printf("    [16]    = %x:\tOutput Error-Stopped State\n",bits);
    bits = (val >> 10) & 0x1;
    if(bits)
        printf("    [10]    = %x:\tInput Retry-Stopped State\n",bits);
    bits = (val >> 9) & 0x1;
    if(bits)
        printf("    [9]     = %x:\tInput Error-Stopped Happen\n",bits);
    bits = (val >> 8) & 0x1;
    if(bits)
        printf("    [8]     = %x:\tInput Error-Stopped State\n",bits);
    bits = (val >> 3) & 0x1;
    if(bits)
        printf("    [3]     = %x:\tRX Clock Active\n",bits);
    bits = (val >> 2) & 0x1;
    if(bits)
        printf("    [2]     = %x:\tUnrecoverable output error\n",bits);
    bits = (val >> 1) & 0x1;
    if(bits)
        printf("    [1]     = %x:\tInitialization Complete\n",bits);
    bits = (val >> 0) & 0x1;
    if(bits)
        printf("    [0]     = %x:\tInitialization In Progress\n",bits);
    printf("\n");

    val = rioRegs->err.status1;
    if(errClear)
	rioRegs->err.status1 = val;
    val = CF_LE_L(val);
    printf("ADD_STATUS1: REG=%p VAL=%08x\n",&rioRegs->err.status1,val);
    bits = (val >> 7) & 0x1;
    if(bits)
	printf("    [7]     = %x:\tInvalid length field in NREAD response\n",bits);
    bits = (val >> 6) & 0x1;
    if(bits)
	printf("    [6]     = %x:\tInvalid SRC_ID field in NREAD response\n",bits);
    bits = (val >> 5) & 0x1;
    if(bits)
	printf("    [5]     = %x:\tInvalid DST_ID field in NREAD response\n",bits);
    bits = (val >> 4) & 0x1;
    if(bits)
	printf("    [4]     = %x:\tInvalid TID field in maintanance response\n",bits);
    bits = (val >> 3) & 0x1;
    if(bits)
	printf("    [3]     = %x:\tInvalid TID field in NREAD response\n",bits);
    bits = (val >> 2) & 0x1;
    if(bits)
	printf("    [2]     = %x:\tExceed Retry in NREAD response\n",bits);
    bits = (val >> 1) & 0x1;
    if(bits)
	printf("    [1]     = %x:\tReserved status field in NREAD response\n",bits);
    bits = (val >> 0) & 0x1;
    if(bits)
	printf("    [0]     = %x:\tNREAD response timeout\n",bits);
    printf("\n");

    val = rioRegs->err.status2;
    if(errClear)
	rioRegs->err.status2 = val;
    val = CF_LE_L(val);
    printf("ADD_STATUS2: REG=%p VAL=%08x\n",&rioRegs->err.status2,val);
    printf("\n");

    val = rioRegs->ext.ltCtl;
    val = CF_LE_L(val);
    printf("LT_CTL: REG=%p VAL=%08x\n\n",&rioRegs->ext.ltCtl,val);

    val = rioRegs->ext.rtCtl;
    val = CF_LE_L(val);
    printf("RT_CTL: REG=%p VAL=%08x\n\n",&rioRegs->ext.rtCtl,val);

    val = rioRegs->spec.memSize;
    val = CF_LE_L(val);
    printf("SLAVE_MEM_SIZE: REG=%p VAL=%08x\n",&rioRegs->spec.memSize,val);
    bits = val >> 24;
    printf("    [31:24] = %02x:\tSIZE=%x\n\n",bits,~(bits << 24) + 1);

    val = rioRegs->spec.memBaseAddr;
    val = CF_LE_L(val);
    printf("SLAVE_MEM_ON_RIO: REG=%p VAL=%08x\n\n",&rioRegs->spec.memBaseAddr,val);

    val = rioRegs->spec.memTranslAddr;
    val = CF_LE_L(val);
    printf("SLAVE_MEM_ON_PCI: REG=%p VAL=%08x\n\n",&rioRegs->spec.memTranslAddr,val);

    val = rioRegs->spec.enable;
    val = CF_LE_L(val);
    printf("ENABLE: REG=%p VAL=%08x\n",&rioRegs->spec.enable,val);
    bits = (val >> 31) & 0x1;
    printf("    [31]    = %x:\tSOFT RESET\n",bits);
    bits = (val >> 29) & 0x1;
    printf("    [29]    = %x:\tSerialRIO Enable\n",bits);
    bits = (val >> 28) & 0x1;
    printf("    [28]    = %x:\tSerialRIO Mode %dX\n",bits,bits ? 1 : 4);
    bits = (val >> 27) & 0x1;
    printf("    [27]    = %x:\tDestID Check\n",bits);
    bits = (val >> 26) & 0x1;
    printf("    [26]    = %x:\tRegs Access\n",bits);
    bits = (val >> 25) & 0x1;
    printf("    [25]    = %x:\tConfiguration Access Type1\n",bits);
    bits = (val >> 24) & 0x1;
    printf("    [24]    = %x:\tConfiguration Access Type0\n",bits);
    bits = (val >> 23) & 0x1;
    printf("    [23]    = %x:\tMessages Enable\n",bits);
    bits = (val >> 22) & 0x1;
    printf("    [22]    = %x:\tDoorbell Enable\n",bits);
    bits = (val >> 16) & 0x1;
    printf("    [16]    = %x:\tPrefetched Memory Access\n",bits);
    bits = (val >> 15) & 0x1;
    printf("    [15]    = %x:\tNREAD Response Enable\n",bits);
    bits = (val >> 14) & 0x1;
    printf("    [14]    = %x:\tMESSAGE Response Enable\n",bits);
    bits = (val >> 13) & 0x1;
    printf("    [13]    = %x:\tNWRITE_R Response Enable\n",bits);
    bits = (val >> 12) & 0x1;
    printf("    [12]    = %x:\tNREAD Enable\n",bits);
    bits = (val >> 11) & 0x1;
    printf("    [11]    = %x:\tSWRITE Enable\n",bits);
    bits = (val >> 10) & 0x1;
    printf("    [10]    = %x:\tNWRITE_R Enable\n",bits);
    bits = (val >> 9) & 0x1;
    printf("    [9]     = %x:\tNWRITE Enable\n",bits);

    bits = (val >> 8) & 0x1;
    printf("    [8]     = %x:\tMemory Access\n",bits);
    bits = val & 0x1;
    printf("    [0]     = %x:\tIO Access\n\n",bits);

    val = rioRegs->user.memBarSize;
    val = CF_LE_L(val);
    printf("MEM_BAR_SIZE: REG=%p VAL=%08x\n",&rioRegs->user.memBarSize,val);
    bits = val & 0x7ffff;
    printf("    [18:0]  = %x:\tSIZE=%x\n\n",bits,(~(bits << 12) & 0x7fffffff) + 1);

#ifdef      K64RIO
    val = rioRegs->errCtrl.logErrDet;
    val = CF_LE_L(val);
    printf("LOG_ERR_DET: REG=%p VAL=%08x\n",&rioRegs->errCtrl.logErrDet,val);
    bits = (val >> 31) & 0x1;
    if(bits)
        printf("    [31]    = %x:\tL_IO_ERR_RESP\n",bits);
    bits = (val >> 30) & 0x1;
    if(bits)
        printf("    [30]    = %x:\tL_MSG_ERR_RESP\n",bits);
    bits = (val >> 28) & 0x1;
    if(bits)
        printf("    [28]    = %x:\tL_MSG_FORMAT_ERR\n",bits);
    bits = (val >> 27) & 0x1;
    if(bits)
        printf("    [27]    = %x:\tL_ILL_TRANS_DEC\n",bits);
    bits = (val >> 26) & 0x1;
    if(bits)
        printf("    [26]    = %x:\tL_ILL_TRANS_TARERR\n",bits);
    bits = (val >> 25) & 0x1;
    if(bits)
        printf("    [25]    = %x:\tL_MSG_REQ_TIMEOUT\n",bits);
    bits = (val >> 24) & 0x1;
    if(bits)
        printf("    [24]    = %x:\tL_RESP_TIMEOUT\n",bits);
    bits = (val >> 23) & 0x1;
    if(bits)
        printf("    [23]    = %x:\tL_UNSOL_RESP\n",bits);
    bits = (val >> 22) & 0x1;
    if(bits)
        printf("    [22]    = %x:\tL_UNSUP_TRANS\n",bits);
    if(errClear)
        rioRegs->errCtrl.logErrDet = 0;
    printf("\n");

    val = rioRegs->errCtrl.logErrDetEn;
    val = CF_LE_L(val);
    printf("LOG_ERR_DET_EN: REG=%p VAL=%08x\n",&rioRegs->errCtrl.logErrDetEn,val);
    bits = (val >> 31) & 0x1;
    printf("    [31]    = %x:\tL_IO_ERR_RESP\n",bits);
    bits = (val >> 30) & 0x1;
    printf("    [30]    = %x:\tL_MSG_ERR_RESP\n",bits);
    bits = (val >> 28) & 0x1;
    printf("    [28]    = %x:\tL_MSG_FORMAT_ERR\n",bits);
    bits = (val >> 27) & 0x1;
    printf("    [27]    = %x:\tL_ILL_TRANS_DEC\n",bits);
    bits = (val >> 26) & 0x1;
    printf("    [26]    = %x:\tL_ILL_TRANS_TARERR\n",bits);
    bits = (val >> 25) & 0x1;
    printf("    [25]    = %x:\tL_MSG_REQ_TIMEOUT\n",bits);
    bits = (val >> 24) & 0x1;
    printf("    [24]    = %x:\tL_RESP_TIMEOUT\n",bits);
    bits = (val >> 23) & 0x1;
    printf("    [23]    = %x:\tL_UNSOL_RESP\n",bits);
    bits = (val >> 22) & 0x1;
    printf("    [22]    = %x:\tL_UNSUP_TRANS\n\n",bits);

    val = rioRegs->errCtrl.pktTTL;
    val = CF_LE_L(val);
    printf("PKT_TTL: REG=%p VAL=%08x\n",&rioRegs->errCtrl.pktTTL,val);

#else
    val = rioRegs->user.addCtl;
    val = CF_LE_L(val);
    printf("ADD_CTL: REG=%p VAL=%08x\n",&rioRegs->user.addCtl,val);
    bits = (val >> 24) & 0x3;
    switch(bits)
    {
	case 0: s = "16"; break;
	case 1: s = "4"; break;
	case 2: s = "8"; break;
	default: s = "OFF";
    }
    printf("    [25:24] = %x:\tMAX READ BURST %s\n",bits,s);
    bits = (val >> 16) & 0x3;
    switch(bits)
    {
	case 0: s = "16"; break;
	case 1: s = "4"; break;
	case 2: s = "8"; break;
	default: s = "OFF";
    }
    printf("    [17:16] = %x:\tMAX WRITE BURST %s\n",bits,s);
    bits = (val >> 10) & 0x1;
    printf("    [10]    = %x:\tTarget Abort %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 9) & 0x1;
    printf("    [9]     = %x:\tConfiguration Access Type1 %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 8) & 0x1;
    printf("    [8]     = %x:\tTarget Retry %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 7) & 0x1;
    printf("    [7]     = %x:\tInternal Arbiter %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 6) & 0x1;
    printf("    [6]     = %x:\t%sTransparent Bridge\n",bits,bits ? "Non-" : "");
    bits = (val >> 5) & 0x1;
    printf("    [5]     = %x:\tSet SERR# With Master Abort %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 4) & 0x1;
    printf("    [4]     = %x:\tSet SERR# With Target Abort %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 3) & 0x1;
    printf("    [3]     = %x:\tTranslate Master Abort to FFFFFFFF %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 2) & 0x1;
    printf("    [2]     = %x:\tTranslate Target Abort to FFFFFFFF %s\n",bits,bits ? "ON" : "OFF");
    bits = (val >> 1) & 0x1;
    printf("    [1]     = %x:\tSet SERR# With PERR# %s\n",bits,bits ? "ON" : "OFF");
    bits = val & 0x1;
    printf("    [0]     = %x:\tSet Target Abort With Master Abort %s\n\n",bits,bits ? "ON" : "OFF");
#endif

    printf("LUT    VALUE     RIO_ADDR  PRI FTYPE TTYPE ID\n");
    printf("--- ---------- ----------- --- ----- ----- --\n");
#ifdef	K64RIO
    for(ix = 0; ix < 1; ix++)
#else
    for(ix = 0; ix < 16; ix++)
#endif
    {
	val = rioRegs->lut.memTranslAddr[ix];
	val = CF_LE_L(val);
	temp = rioRegs->lut.memID[ix];
	temp = CF_LE_L(temp);
	printf(" %x  0x%08x 0x%x%08x  %x    %x     %x   %02x\n",ix,val,
	    (val >> 16) & 3,val & 0xfff00000,(val >> 8) & 3,
	    (val >> 4) & 0xf,val & 0xf,temp);
    }
    printf("\n");

    val = rioRegs->endp.intrEnable;
    val = CF_LE_L(val);
    printf("INTERRUPT_ENABLE: REG=%p VAL=%08x\n",&rioRegs->endp.intrEnable,val);
    if(val)
	intrShow(val);

    val = rioRegs->endp.intrStatus;
    val = CF_LE_L(val);
    printf("INTERRUPT_STATUS: REG=%p VAL=%08x\n",&rioRegs->endp.intrStatus,val);
    if(val)
	intrShow(val);
    printf("\n");

   
    printf ("All RIO interrupts counts:\n\r");
    for (ix=0; ix < 32; ix++)
       { if (rioIntsCount[ix] !=0)
           { printf ("num %2i count %8i \n\r", ix, rioIntsCount[ix]);
           }
       }
#ifdef MPON
    switch(rioCrossType)
    {
	case RIO_CROSS_RPKB:
	    printf("Working with RPKB crossboard\n");
	    break;
	case RIO_CROSS_NIISI2S:
	    printf("Working with NIISI crossboard, D9 and D10 switch configuration\n");
	    break;
	case RIO_CROSS_NIISI1S:
	    printf("Working with NIISI crossboard, D10 switch configuration\n\n");
	    break;
	case RIO_CROSS_NIISI2S_V2:
	    printf("Working with NIISI crossboard, D9 and D10 switch configuration version 2\n");
	    break;
	case RIO_CROSS_RPKB_RESERVE:
	    printf("Working with RPKB RESERVED crossboard\n");
	    break;
	case RIO_CROSS_RPKB_FULL:
	    printf("Working with RPKB MAIN and RESERVED crossboard\n");
	    break;
	case RIO_CROSS_UNKNOWN:
	    printf("Working with remote crossboard configuration\n");
	    break;
	default:
	    printf("Unknown crossboard type=%d\n",rioCrossType);
	    break;
    }
#endif
}

void rioDbShow()
{
   unsigned ix;
   printf ("DB in count   DB out count    DMA start count \n\r");
   for (ix=0; ix < RIO_ID_MAX; ix++)
      { printf ("%011i   %011i    %011i\n\r",  dbIntsInCount[ix], dbIntsOutCount[ix], dmaStartCount[ix]);
      }
}

/***************************************************************/

/* detect host type by ID */

int rioHostTypeGet(unsigned id)
{
    unsigned val, vendor, device;

    if(id > 255)
        return RIO_HOST_INVALID;

    if(id == boardProcNumGet())
    {
        if(rioMaintIn(0,CAR_DEV_ID,0xffffffff,&val) == -1)
            return RIO_HOST_INVALID;
    }
    else
    {
        if(rioMaintIn(255,CAR_DEV_ID,id,&val) == -1)
            return RIO_HOST_INVALID;
    }

    vendor = val & 0xffff;
    device = val >> 16;

    if(vendor != RIO_VENDOR_ID_NIISI)
        return RIO_HOST_UNKNOWN;

    switch(device)
    {
        case RIO_DEVICE_ID_K128:
            return RIO_HOST_K128;
    }

    return RIO_HOST_K64;
}

#ifdef MPON
#ifdef BT5328
  #include "bt5328Config.c"
#else
  #include "mponConfig.c"
#endif
  #include "mpon.c"
#else
  #include "k64rio.c"
#endif

#if (defined(K64RIO) || defined(MPON_RIO_MSG)) && !defined(NEW_K64RIO_DRV)
  #include "msg.c"
#endif

void rioShutdown(void)
{
#if (defined(K64RIO) || defined(MPON_RIO_MSG)) && !defined(NEW_K64RIO_DRV)
    if(boardRioInitDone)
    {
        unsigned ix;
        
        rioIntDisable(0xffffffff);
        rioRegs->endp.dbInCtrl = 0;                 /* ??? for Dima */
        
        for(ix = 0; ix < rmsgNBuf; ix++)
            rmsgRxDesc[ix].next |= CT_LE_L(1);
        
        if(!rioMsgMode)
        {
            _rmsgSendOld(boardProcNumGet(),0,rmsgRxDesc,256);
        }
        else
        {
            _rmsgTxQueue(boardProcNumGet(),0,256,rmsgRxDesc,NULL,1);
        }
        for(ix = 0; ix < 1000; ix++)
        {
            if(rioRegs->endp.msgInStatus & CT_LE_L(MSG_IN_EOL))
                break;
        }
    }
#endif
}

//#include "config.c"
