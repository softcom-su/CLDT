/* rio.c - K128 RapidIO library */

#include <stdio.h>
#include <pthread.h>
#include <logLib.h>
#include <arch/interrupt.h>
#include <board.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <cacheLib.h>
#include <rio.h>

#define RIO_ID_MAX              32  //!!! FIXME

#define DMA_INT_CP2             0x00000001
#define DMA_INT_RX              0x00000004
#define DMA_INT_VSK             0x00000008
#define DMA_INT_TX              0x00000010

#define DMA_RX_CTRL_START       0x00000001
#define DMA_RX_CTRL_STOP        0x00000002
#define DMA_RX_CTRL_STOP_INT    0x00000004
#define DMA_RX_CTRL_SEL9        0x00000008

#define DMA_RX_INT_CNT0         0x00000001
#define DMA_RX_INT_CNT1         0x00000002
#define DMA_RX_INT_CNT          0x00000004
#define DMA_RX_INT_LETTER       0x00000008
#define DMA_RX_INT_MSEG         0x00000010
#define DMA_RX_INT_LAST         0x00000020
#define DMA_RX_INT_MSG_DONE     0x00000040
#define DMA_RX_INT_SID9         0x00000080
#define DMA_RX_INT_CNT9_0       0x00000100
#define DMA_RX_INT_CNT9_1       0x00000200
#define DMA_RX_INT_CNT9         0x00000400
#define DMA_RX_INT_SEG_DONE     0x00000800
#define DMA_RX_INT_FIFO_OVF     0x00001000
#define DMA_RX_INT_FIFO_NE      0x00002000

#define DMA_TX_CTRL_START       0x00000001
#define DMA_TX_CTRL_STOP        0x00000002
#define DMA_TX_CTRL_FHALT       0x00000004
#define DMA_TX_CTRL_FINT        0x00000008

#define DMA_TX_STAT_HALT        0x00000001
#define DMA_TX_STAT_INT         0x00000002
#define DMA_TX_STAT_SEG_ERR     0x00000004
#define DMA_TX_STAT_OVERTIME    0x00000008
#define DMA_TX_STAT_FIFO_OVF    0x00000010
#define DMA_TX_STAT_FOVRTERR    0x00000020
#define DMA_TX_STAT_FUNDERR     0x00000040
#define DMA_TX_STAT_FSEGERR     0x00000080
#define DMA_TX_STAT_BUF_TOUT    0x00000100
#define DMA_TX_STAT_DESC_MASK   0x000f0000

#define DMA_TX_FLAG_HALT        0x01
#define DMA_TX_FLAG_INT         0x02
#define DMA_TX_FLAG_OVERTIME    0x04
#define DMA_TX_FLAG_SEG_ERR     0x08
#define DMA_TX_FLAG_FIFO_OVF    0x10
#define DMA_TX_FLAG_FIFO_WRITE  0x20
#define DMA_TX_FLAG_VALID       0x80

#define RIO_INT_CPU             0x00000001
#define RIO_INT_CSR             0x00000002
#define RIO_INT_DBI             0x00000004
#define RIO_INT_DBO             0x00000008
#define RIO_INT_ERR_PKT         0x00000010
#define RIO_INT_UNSUPP_PKT      0x00000020

// address of maintenance buffers

#define CPU_TX_ADDR     0x7800
#define CPU_RX_ADDR     0x7880

/* CPU I/O definintions */

#define NOT_CHECK_SID           (1<<29)
#define NOT_WAIT_ACK            (1<<28)

#define FTYPE_MAINT             (8<<24)
#define FTYPE_RESET             (0<<24)

#define TTYPE_MAINT_READ        (0<<20)
#define TTYPE_MAINT_WRITE       (1<<20)
#define SIZE_4BYTES             (8<<16)

#define MAINT_READ_HEAD         (NOT_CHECK_SID|FTYPE_MAINT|SIZE_4BYTES|TTYPE_MAINT_READ)
#define MAINT_WRITE_HEAD        (NOT_CHECK_SID|FTYPE_MAINT|SIZE_4BYTES|TTYPE_MAINT_WRITE)
#define MAINT_RESET_HEAD        (NOT_CHECK_SID|NOT_WAIT_ACK|FTYPE_RESET)

/* maintenance regs */

#define CAR_DEV_ID              0x000000
#define CAR_DEV_INFO            0x000004
#define CAR_ASBLY_ID            0x000008
#define CAR_ASBLY_INFO          0x00000c
#define CAR_PE_FEAT             0x000010
#define CAR_SWITCH_PI           0x000014
#define CAR_SOURCE_OP           0x000018
#define CAR_LUT_SIZE            0x000034
#define CSR_BASE_DEV_ID         0x000060
#define CSR_HB_DEV_ID_LOCK      0x000068
#define CSR_COMP_TAG            0x00006c
#define CSR_ROUTE_CFG_DESTID    0x000070
#define CSR_ROUTE_CFG_PORT      0x000074
#define CSR_LUT_ATTR            0x000078
#define PORT_LT_CTL             0x000120
#define PORT_RT_CTL             0x000124
#define PORT_GEN_CTL            0x00013c
#define PORT_ERR_STAT0          0x000158
#define PORT_CTL                0x00015c
#define PKT_TTL                 0x00102c
#define FAB_TEA_CTL0            0x0b6000
#define PORT_UNRECOV_ERR_DET    0x010e00
#define PORT_UNRECOV_ERR_ENABLE 0x010e04
#define PORT_RECOV_ERR_DET      0x010e08

typedef struct {
    unsigned    base;   // base address aligned to 16 byte
    unsigned    x;      // [31:8] - step x, [7:6] - unused, [6:0] - Bx burst
    unsigned    xy;     // [31:12] - Nx, [11:0] - step y high
    unsigned    y;      // [31:20] - step y low, [19:0] - Ny
    unsigned    next;   // next descriptor address aligned to 16 byte
    unsigned    user;   // user private
    unsigned    nIter;  // Nx*Ny*Bx
    unsigned    rio;    // [23:16] - destID, [9:8] - mbox, [7:0] - flags
} DMA_TX_DESC;

/* bits reserved for demultiplexor in doorbell data */

#define DB_HANDLER_BITS         4
#define DB_HANDLER_COUNT        (1 << (DB_HANDLER_BITS-1))
#define DB_HANDLER_MASK         (DB_HANDLER_COUNT-1)

#define RIO_DEBUG(level,msg)    \
    if(level <= rioDebug)       \
        logMsg msg;


/******************************************************************************/

/* globals */

int rioDebug = 1;                                       // debug level
int rioMaintTout = 1000;                                // maintenance cycle timeout us
int rioInitDoneFlag = FALSE;                            // initialization done flag
VOIDFUNCPTR rioDmaRxHook;                               // DMA RX interrupt hook

/***************************************************************/

#ifdef FOR_SUBMICRON
  #warning !!!!! SUBMICRON version of rio.c !!!!!!!!!!
int submicronSyncTout = SUBMICRON_SYNC_TOUT;
#endif

/* locals */

static VOIDFUNCPTR dbRtn[DB_HANDLER_COUNT];             // doorbell user interrupt handler routines
static int dbArg[DB_HANDLER_COUNT];                     // doorbell user interrupt handler arguments
static pthread_mutex_t dbMutex;                         // doorbell mutex
static pthread_mutex_t maintMutex;                      // maintenance mutex
static pthread_mutex_t txMutex;                         // dmaTxDescExec mutex
static DMA_TX_DESC* dummyTxDesc;                        // dummy tail
static char dbInCount[256];
static char dbOutCount[256];

/******************************************************************************/

/* get TCPU busy status */

static inline int tcpuIsBusy(void)
{
    return (*RIO_TCPU_STATUS_REG & 0x80000000) ? TRUE : FALSE;
}

/******************************************************************************/

/* reset TCPU */

static inline void tcpuReset(void)
{
    unsigned ix;
    *RIO_TCPU_HEAD_REG = MAINT_RESET_HEAD;
    for(ix = 0; ix < 100; ix++)
    {
        if(!tcpuIsBusy())
            return;
        udelay(1);
    }
    RIO_DEBUG(1,("%s: timeout\n",(int)__FUNCTION__,0,0,0,0,0));
}

/******************************************************************************/

/* wait until TCPU busy is cleared */

static int tcpuWaitBusy(unsigned tout)
{
    unsigned ix;

    for(ix = 0; ix < tout; ix++)
    {
        if(!tcpuIsBusy())
            return 0;
        udelay(1);
    }
    tcpuReset();
    return -1;
}

/******************************************************************************/

/* maintenance cycle in/out routine  */

int rioMaintInOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data,
                  unsigned* pData, const char* fname, unsigned* err)
{
    unsigned head;
    volatile unsigned* p;
    static unsigned char tid = 0;

    if(addr & ~0xfffffc)
    {
inval:
        RIO_DEBUG(1,("%s: invalid param: hopCount=%x addr=%x dstID=0x%x\n",
             (int)fname,hopCount,addr,dstID,0,0));
        if(err)
            *err = EINVAL;
        return -1;
    }

    /* access host regs */

    if(dstID == 0xffffffff && !hopCount)
    {
        if(addr >= 0x200)
            goto inval;

        p = (void*)RIO_CAR_DEV_ID_REG + addr;

        if(pData)
        {
            *pData = *p;
        }
        else
        {
            *p = data;
        }
        return 0;
    }
    
    if(hopCount > 0xff || dstID > 0xff)
        goto inval;

    pthread_mutex_lock(&maintMutex);

    if(tcpuIsBusy())
        tcpuReset();

    if(pData)
    {
        p = (void*)(SRAM_BASE + CPU_RX_ADDR);
        p[0] = 0x55555555;
        p[1] = 0xaaaaaaaa;

        *RIO_TCPU_RXA_REG = CPU_RX_ADDR;
        head = MAINT_READ_HEAD;
    }
    else
    {
        p = (void*)(SRAM_BASE + CPU_TX_ADDR);

        if(addr & 4)
            p[1] = data;
        else
            p[0] = data;

        *RIO_TCPU_TXA_REG = 0x10000 | CPU_TX_ADDR;
        head = MAINT_WRITE_HEAD;
    }

    *RIO_TCPU_ARIO_REG = (hopCount << 24) | addr;
    *RIO_TCPU_HEAD_REG = head | (tid++ << 8) | dstID;

    if(tcpuWaitBusy(rioMaintTout) == -1)
    {
        RIO_DEBUG(2,("%s: timeout=%dus: hopCount=%x addr=%x dstID=0x%x\n",
              (int)fname,rioMaintTout,hopCount,addr,dstID,0));
        pthread_mutex_unlock(&maintMutex);
        if(err)
            *err = ETIMEDOUT;
        return -1;
    }

    if(pData)
    {
        if(addr & 4)
            *pData = p[1];
        else
            *pData = p[0];
    }

    pthread_mutex_unlock(&maintMutex);
    return 0;
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
    
    if(devid == 0x00010074)
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

/***********************************************************************/

/* doorbell interrupt handler */

void rioDbIntr(void)
{
    unsigned stat, data, i, id, val, j;

    while((stat = *RIO_RDB_STATUS_REG) & 0x80000000)
    {
        val = *RIO_RDB_DB_REG;
        *RIO_RDB_STATUS_REG = stat;

        data = val & 0xffff;
        id = (val >> 16) & 0xff;

        i = data & DB_HANDLER_MASK;

        j = (data >> (DB_HANDLER_BITS-1)) & 1;

        if(dbInCount[id] != -1)
	{
	    if(j != dbInCount[id])
	    {
		RIO_DEBUG(1,("%s: duplicated doorbell: id=%02x data=%04x\n",
		    (int)__FUNCTION__,id,data,0,0,0));
		continue;
	    }
	}

	dbInCount[id] = j ^ 1;

        if(dbRtn[i])
        {
            dbRtn[i](dbArg[i],data >> DB_HANDLER_BITS,id);
        }
        else
        {
            RIO_DEBUG(2,("%s: user handler not connected: id=%02x stat=%x val=%08x\n",
                (int)__FUNCTION__,id,stat,val,0,0));
        }

        RIO_DEBUG(5,("%s: id=%02x stat=%x val=%08x\n",(int)__FUNCTION__,id,stat,val,0,0));
    }
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

/***********************************************************************/

/* send doorbell */

int rioDbOut(unsigned dstID, unsigned i, unsigned data)
{
    unsigned val, dbval, ix, retry = 10;

    if(i >= DB_HANDLER_COUNT || dstID > 0xff || (data & 0xfffff000))
    {
        RIO_DEBUG(1,("%s: invalid param: dstId=0x%x i=%d data=%x\n",(int)__FUNCTION__,dstID,i,data,0,0));
        return EINVAL;
    }

    while(retry--)
    {
	dbval = (dstID << 16) | (data << DB_HANDLER_BITS) | (dbOutCount[dstID] << (DB_HANDLER_BITS-1)) | i;
	*RIO_TDB_DB_REG = dbval;

	for(ix = 0; ix < 1000; ix++)
	{
    	    val = *RIO_TDB_STATUS_REG;
    	    if(!(val & 0x80000000))
    	    {
                RIO_DEBUG(3,("%s: dstID=0x%x i=%d data=%03x val=%04x code=%x OK\n",(int)__FUNCTION__,dstID,i,data,dbval,val & 0xf));
                if( val & 0xf )
                {
                    udelay(1000);
                    break;
                } else
                {
                    dbOutCount[dstID] ^= 1;
                    return 0;
                }
    	    }
    	    udelay(1);
	}

	RIO_DEBUG(2,("%s: timeout-retry: dstID=0x%x stat=%x\n",(int)__FUNCTION__,dstID,val,0,0,0));
    }

    RIO_DEBUG(1,("%s: timeout: dstID=0x%x stat=%x\n",(int)__FUNCTION__,dstID,val,0,0,0));
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

/***********************************************************************/

/* rio interrupt handler */

#if OS_VERSION >= 300
int ksprint(char *buf,const char *fmt, ...);
#endif

#if OS_MAJOR >= 4 
#define  INTRTYPE long
#else
#define  INTRTYPE int
#endif

void rioIntr(INTRTYPE unused)
{
    unsigned stat, mask, req;
#if OS_VERSION >= 300
    static const char *fmt="\20\1CPU\2CSR\3DBI\4DBO\5UNSUPP\6ERRPKT\7RBUFOVF\10RXDOVF\11VSKOVF\12UNEXPCPUACK";
#endif
    stat = *RIO_GRP_INT_REQ_REG;
    mask = *RIO_GRP_INT_EN_REG;

//     kprint("rio: stat=%x mask=%x\n\r",stat,mask);

    req = stat & mask;

    if(req & RIO_INT_DBI)
        rioDbIntr();

    if(req & (RIO_INT_CPU|RIO_INT_CSR|RIO_INT_DBO|RIO_INT_ERR_PKT|RIO_INT_UNSUPP_PKT))
    {
#if OS_VERSION >= 300
       static char buf[100];
       ksprint(buf,"%b",stat,fmt);
       RIO_DEBUG(1,("%s: stat=%s mask=%x\n",(int)__FUNCTION__,(int)buf,mask,0,0,0));
#else
       RIO_DEBUG(1,("%s: stat=%x mask=%x\n",(int)__FUNCTION__,stat,mask,0,0,0));
#endif
    }

    if(req & (RIO_INT_ERR_PKT|RIO_INT_UNSUPP_PKT))
        *RIO_GRP_INT_REQ_REG = stat;
}

#ifdef   INCLUDE_RMSG_NET

/***********************************************************************/

/* fill tx descriptor */

static inline void txDescFill(DMA_TX_DESC* d, void* addr, unsigned nBytes,
    unsigned flags, unsigned id, unsigned mbox, unsigned user)
{
    d->base = K1_TO_PHYS(addr);
    d->x = 16 << 8 | 16;
    d->xy = (nBytes / 256) << 12;
    d->y = 1;
    d->next = 0xffffffff;
    d->user = user;
    d->nIter = nBytes / 16;
    d->rio = (id << 16) | (mbox << 8) | flags | DMA_TX_FLAG_VALID;
    boardWbFlush();
}

/***********************************************************************/

/* execute tx descriptor */

static int dmaTxDescExec(DMA_TX_DESC* d)
{
    unsigned tout, retry;
    d->next = K1_TO_PHYS(dummyTxDesc);
    d->rio |= DMA_TX_FLAG_HALT;
    pthread_mutex_lock(&txMutex);
    for(retry = 0; retry < 2; retry++)
    {
        *DMA_CH_MT_DADDR_REG = K1_TO_PHYS(d);
        *DMA_CH_MT_CTRL_REG = DMA_TX_CTRL_START;
        tout = 10000;
        while(--tout)
        {
            udelay(1);
            if(*DMA_CH_MT_STAT_REG & DMA_TX_STAT_HALT)
            {
                //             printf("DONE stat=%x\n",*DMA_CH_MT_STAT_REG);
                pthread_mutex_unlock(&txMutex);
                return 0;
            }
        }
        printf("%s: timeout stat=%x retry=%d\n",__FUNCTION__,*DMA_CH_MT_STAT_REG,retry);
        *DMA_CH_MT_CTRL_REG = DMA_TX_CTRL_STOP;
        tout = 10000;
        while(--tout)
        {
            udelay(1);
            if(*DMA_CH_MT_STAT_REG & DMA_TX_STAT_HALT)
               break;
        }
        if(!tout)
        {
            printf("%s: stop timeout stat=%x retry=%d\n",__FUNCTION__,*DMA_CH_MT_STAT_REG,retry);
            break;
        }
    }
    pthread_mutex_unlock(&txMutex);
    return -1;
}

extern int if_jadd(void *,void *);

#endif

/***********************************************************************/

/* rx dma interrupt handler */

void dmaRxIntr(void)
{
    unsigned stat, mask, req;
    
    stat = *DMA_CH_MR_INT_REG;
    mask = *DMA_CH_MR_IE_REG;
    
    req = stat & mask;
    
    //     kprint("rx: stat=%x\n\r",stat);
    *DMA_CH_MR_INT_REG = stat;
    *DMA_CH_MR_INT_REG = ~stat;
    
    if(req & DMA_RX_INT_SID9)
    {
        *DMA_CH_MR_IE_REG = 0;
        if(rioDmaRxHook)
           rioDmaRxHook();
    }

    RIO_DEBUG(4,("%s: stat=%x mask=%x req=%x\n",(int)__FUNCTION__,stat,mask,req,0,0));
}

/***********************************************************************/

/* dma interrupt handler */

static void dmaIntr(INTRTYPE unused)
{
    unsigned stat, mask, req;
    
    stat = *DMA_IR_REG;
    mask = *DMA_IE_REG;
    
    req = stat & mask;

    //     kprint("dma: stat=%x mask=%x\n\r",stat,mask);
    
//    if(req & DMA_INT_TX)
//        dmaTxIntr();
    
    if(req & DMA_INT_RX)
        dmaRxIntr();
    
    if(req & (DMA_INT_CP2|DMA_INT_VSK))
        RIO_DEBUG(1,("%s: stat=%x mask=%x\n",(int)__FUNCTION__,stat,mask,0,0,0));

    RIO_DEBUG(4,("%s: stat=%x mask=%x req=%x\n",(int)__FUNCTION__,stat,mask,req,0,0));
}

#if OS_VERSION >= 300
int  my_mutex_init(pthread_mutex_t  *mutex, char *name)
{
   pthread_mutexattr_t mtxa;
   int error;
   pthread_mutexattr_init(&mtxa);
   pthread_mutexattr_settype(&mtxa,PTHREAD_MUTEX_RECURSIVE);
   pthread_mutexattr_setpshared(&mtxa,PTHREAD_PROCESS_SHARED);
   pthread_mutexattr_setname(&mtxa,name);
   error = pthread_mutex_init(mutex, &mtxa ); /* инициализация мьютекса             */
   return error;
}
#else
int  my_mutex_init(pthread_mutex_t  *mutex, char *name)
{
    return pthread_mutex_init(mutex,NULL);
}
#endif

/***********************************************************************/

int boardRioIdGet(void)
{
    return (*RIO_CSR_BASE_DEV_ID_REG >> 16) & 0xff;
}

/***********************************************************************/

void rioInit(int unused)
{
    unsigned ix, id;

    my_mutex_init(&maintMutex,"maintMutex");
    my_mutex_init(&dbMutex,"dbMutex");
    my_mutex_init(&txMutex,"txMutex");

    for(ix = 0; ix < 256; ix++)
	dbInCount[ix] = -1;

    intConnect(IV_RIO,rioIntr,0);
    *RIO_GRP_INT_EN_REG = RIO_INT_DBI|RIO_INT_ERR_PKT|RIO_INT_UNSUPP_PKT;
    boardIvEnable(IV_RIO);

    id = boardRioIdGet();
    boardProcNumSet(id);
    PRINTF("my id=%d\n",id);
    //    *RIO_CSR_BASE_DEV_ID_REG = boardProcNumGet() << 16;

    *RIO_PL_PORT_LT_CTL_REG = 0xffff00;
    *RIO_PL_PORT_RT_CTL_REG = 0xffff00;
    
    *DMA_CH_MT_CTRL_REG = DMA_TX_CTRL_STOP;
    *DMA_CH_MR_CTRL_REG = DMA_RX_CTRL_STOP;
    *RIO_HM_M_CTRL_REG = 0;

    *RIO_RBUFC_BUF_BASE_REG = 0x5000;
    *RIO_RBUFC_BUF_N_REG = 16;
    *RIO_TBUFC_BUF_BASE_REG = 0x6000;
    *RIO_TBUFC_BUF_N_REG = 32;

    for(ix = 0; ix < 8; ix++)
        DMA_CH_MR_SID_REG[ix] = 0xff;

    dummyTxDesc = _cacheMalloc(sizeof(DMA_TX_DESC),64);

    if(!dummyTxDesc)
    {
       printf("%s: cacheMalloc %d bytes failed\n",__FUNCTION__,sizeof(DMA_TX_DESC));
       return;
    }

    memset(dummyTxDesc,0,sizeof(DMA_TX_DESC));
    dummyTxDesc->next = K1_TO_PHYS(dummyTxDesc);

    intConnect(IV_DMA,dmaIntr,0);
    *DMA_IE_REG = DMA_INT_TX|DMA_INT_RX;
    boardIvEnable(IV_DMA);

    rioInitDoneFlag = TRUE;
}

/***********************************************************************/

void rioShutdown()
{
    unsigned ix;

    boardIvDisable(IV_RIO);
    boardIvDisable(IV_DMA);

    *DMA_IE_REG = 0;
    *RIO_GRP_INT_EN_REG = 0;

    *DMA_CH_MT_CTRL_REG = DMA_TX_CTRL_STOP;
    *DMA_CH_MR_CTRL_REG = DMA_RX_CTRL_STOP;
    *RIO_HM_M_CTRL_REG = 0;

    for(ix = 0; ix < 8; ix++)
        DMA_CH_MR_SID_REG[ix] = 0xff;
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

