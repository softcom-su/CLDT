/***************************************************************/

#include "rreg.h"

typedef struct {
    volatile unsigned stat;
    volatile unsigned id;
    volatile unsigned reserved1;
    volatile unsigned buf1;
    volatile unsigned reserved2;
    volatile unsigned buf2;
    volatile unsigned reserved3;
    volatile unsigned next;
} RIO_TX_DESC;

typedef struct {
    volatile unsigned stat;
    volatile unsigned id;
    volatile unsigned reserved1;
    volatile unsigned buf1;
    volatile unsigned reserved2;
    volatile unsigned buf2;
    volatile unsigned reserved3;
    volatile unsigned next;
} RIO_RX_DESC;

#define MSG_OUT_POL             0x00002000
#define MSG_OUT_OWN             0x00001000
#define MSG_OUT_FAIL            0x00000800
#define MSG_OUT_EOL             0x00000400
#define MSG_OUT_COMPLETE        0x00000200
#define MSG_OUT_MODE            0x00000040
#define MSG_OUT_RUN             0x00000001

#define MSG_IN_POL              0x00002000
#define MSG_IN_OWN              0x00001000
#define MSG_IN_FAIL             0x00000800
#define MSG_IN_EOL              0x00000400
#define MSG_IN_COMPLETE         0x00000200
#define MSG_IN_DONE             0x00000100
#define MSG_IN_CHAIN            0x00000040
#define MSG_IN_RUN              0x00000001

unsigned rioMuxRegGet(unsigned i, unsigned ix)
{
    unsigned val;
    if(ix == -1)
        rioRegs->err.rw[0] = 0;
    else
        rioRegs->err.rw[0] = CT_LE_L((ix << 23) | (1 << 22));
    val = rioRegs->err.r[i];
    return CF_LE_L(val);
}

void rioAdd(void)
{
    unsigned val, bits;
    
    val = rioMuxRegGet(0,-1);
    printf("ADD_R1: val=%08x\n",val);
    bits = (val >> 21) & 0x1;
    printf("    [21]    = %x:\tPack Done\n",bits);
    bits = (val >> 20) & 0x1;
    printf("    [20]    = %x:\tPack Not Empty\n",bits);
    bits = (val >> 19) & 0x1;
    printf("    [19]    = %x:\tPack Err\n",bits);
    bits = (val >> 18) & 0x1;
    printf("    [18]    = %x:\tPack Full\n",bits);
    bits = (val >> 17) & 0x1;
    printf("    [17]    = %x:\tPack Run\n",bits);
    bits = (val >> 16) & 0x1;
    printf("    [16]    = %x:\tPack Tail Done\n",bits);
    bits = (val >> 8) & 0xff;
    printf("    [15:8]  = %x:\tPack Tail\n",bits);
    bits = val & 0xff;
    printf("    [7:0]   = %x:\tPack Head\n\n",bits);
    
    printf("RX MAINTENANCE WRITE = %d\n",rioMuxRegGet(0,0));
    printf("RX MAINTENANCE PORT WRITE = %d\n",rioMuxRegGet(0,1));
    printf("RX SWRITE = %d\n",rioMuxRegGet(0,2));
    printf("RX NWRITE = %d\n",rioMuxRegGet(0,3));
    printf("RX NWRITE_R = %d\n",rioMuxRegGet(0,4));
    printf("RX MAINTENANCE WRITE RESPONSE = %d\n",rioMuxRegGet(0,5));
    printf("RX NWRITE_R RESPONSE = %d\n",rioMuxRegGet(0,6));
    printf("TX NWRITE = %d\n",rioMuxRegGet(0,7));
    printf("TX NWRITE_R = %d\n",rioMuxRegGet(0,8));
    printf("TX SWRITE = %d\n",rioMuxRegGet(0,9));
    printf("TX MAINTENANCE WRITE = %d\n",rioMuxRegGet(0,10));
    printf("TX MAINTENANCE WRITE RESPONSE = %d\n",rioMuxRegGet(0,11));
    printf("TX NWRITE_R RESPONSE = %d\n",rioMuxRegGet(0,12));
    printf("TX NREAD = %d\n",rioMuxRegGet(0,13));
    printf("TX MAINTENANCE READ = %d\n",rioMuxRegGet(0,14));
    printf("TX MAINTENANCE READ RESPONSE = %d\n",rioMuxRegGet(0,15));
    
    printf("TX NREAD RESPONSE = %d\n",rioMuxRegGet(1,0));
    printf("RX NREAD = %d\n",rioMuxRegGet(1,1));
    printf("RX MAINTENANCE READ = %d\n",rioMuxRegGet(1,2));
    printf("RX MAINTENANCE READ RESPONSE = %d\n",rioMuxRegGet(1,3));
    printf("RX NREAD RESPONSE = %d\n",rioMuxRegGet(1,4));
    printf("TX TOTAL = %d\n",rioMuxRegGet(1,5));
    printf("TX DOORBELL = %d\n",rioMuxRegGet(1,6));
    printf("RX DOORBELL = %d\n",rioMuxRegGet(1,7));
    printf("RX DOORBELL RESPONSE OK = %d\n",rioMuxRegGet(1,8));
    printf("RX DOORBELL RESPONSE RETRY = %d\n",rioMuxRegGet(1,9));
    printf("RX DOORBELL RESPONSE ERROR = %d\n",rioMuxRegGet(1,10));
    printf("RX DOORBELL RESPONSE RESERVED = %d\n",rioMuxRegGet(1,11));
    printf("TX DOORBELL RESPONSE OK = %d\n",rioMuxRegGet(1,12));
    printf("TX DOORBELL RESPONSE RETRY = %d\n",rioMuxRegGet(1,13));
    printf("TX DOORBELL RESPONSE ERROR = %d\n",rioMuxRegGet(1,14));
    printf("CLK MSG_OUT_RUN COUNT  = %d\n",rioMuxRegGet(1,15));
    
    printf("TX MESSAGE RESPONSE = %d\n",rioMuxRegGet(2,0));
    printf("RX MESSAGE = %d\n",rioMuxRegGet(2,1));
    printf("TX MESSAGE RESPONSE OK = %d\n",rioMuxRegGet(2,2));
    printf("TX MESSAGE RESPONSE RETRY = %d\n",rioMuxRegGet(2,3));
    printf("TX MESSAGE RESPONSE ERROR = %d\n",rioMuxRegGet(2,4));
    printf("RX MESSAGE DESCRIPTORS WITHOUT OWN = %d\n",rioMuxRegGet(2,5));
    printf("RX MESSAGE RESPONSE = %d\n",rioMuxRegGet(2,6));
    printf("TX MESSAGE = %d\n",rioMuxRegGet(2,7));
    printf("RX MESSAGE RESPONSE OK = %d\n",rioMuxRegGet(2,8));
    printf("RX MESSAGE RESPONSE RETRY = %d\n",rioMuxRegGet(2,9));
    printf("RX MESSAGE RESPONSE ERROR = %d\n",rioMuxRegGet(2,10));
    printf("TX MESSAGE DESCRIPTORS CLOSE = %d\n",rioMuxRegGet(2,11));
    printf("RX MESSAGE DESCRIPTORS CLOSE = %d\n",rioMuxRegGet(2,12));
    printf("RX TOTAL = %d\n",rioMuxRegGet(2,13));
    printf("TX MESSAGE DESCRIPTORS OPEN = %d\n",rioMuxRegGet(2,14));
    printf("TX MESSAGE DESCRIPTORS WITHOUT OWN = %d\n",rioMuxRegGet(2,15));
    
    val = rioMuxRegGet(5,-1);
    printf("ADD_R6: val=%08x\n",val);
    
    val = rioMuxRegGet(6,0);
    printf("ADD_R7: val=%08x\n",val);
    
    val = rioMuxRegGet(10,-1);
    printf("ADD_R11: val=%08x\n",val);
}

#ifdef  INCLUDE_RMSG_TEST
#include "rioTest.c"
#else

/***************************************************************/

/* globals */

unsigned char rioSegSize[256];
int rmsgRxThreadFlag = FALSE;
int boardRioInitDone = FALSE;
int rmsgAlignTxSize = TRUE;
VOIDFUNCPTR rmsgMboxRxHook[4];
VOIDFUNCPTR rmsgMboxTxHook[4];

/* locals */

static unsigned rmsgNBuf;
static void* rmsgBufBase;
static unsigned rmsgBufPCIBase, rmsgBufPCITop;
static volatile unsigned rmsgRxOwn, rmsgRxCurrent, rmsgNOwn;
static RIO_RX_DESC* rmsgRxDesc;
static unsigned* rmsgRxMsgSize;
static sem_t rmsgRxSem;
static RIO_TX_DESC* rmsgTxDesc;
static unsigned rmsgTxDescAddr;
static unsigned* rmsgTxBuf;
static volatile unsigned rmsgTxDirty, rmsgTxCurrent;
static RIO_TX_DESC* rmsgTxDesc;
static void* rmsgTxNode[RMSG_TX_RING_SIZE];

#if !defined(RIO_TX_MUTEX) && defined(INCLUDE_RMSG_NET) && (OS_VERSION < 300 && defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD)) ||(OS_VERSION >= 300 && defined(INCLUDE_MULTIPROCESSING))
#define RIO_TX_MUTEX
#endif
#ifdef RIO_TX_MUTEX
static pthread_mutex_t rmsgTxMutex;
#endif

#define RX_Q_ADDR       0
#define RX_Q_TRAP       4
#define RX_Q_STAT       8

#define RX_Q_INTR_COMPL 1
#define RX_Q_INTR_EOL   2
#define RX_Q_INTR_OWN   4
#define RX_Q_INTR_FAIL  8

#define RX_Q_REG(i,reg)     ((volatile unsigned*)((void*)rioRegs+rmsgRxQBase[(i)]+reg))

typedef struct {
    sem_t               sem;
    unsigned            nMax;
    unsigned            nCur;
    unsigned            id;
    unsigned            mbox;
    unsigned            stat;
    RIO_RX_DESC*        d;
    unsigned            d0;
} RX_Q_DESC;

static unsigned rmsgRxQBase[8] = { 0x10870, 0x10894, 0x108b8, 0x108c4, 0x108d0, 0x108dc, 0x108e8, 0x108f4 };
static RX_Q_DESC rmsgRxQDesc[8];

extern int rmsgRxQInit(unsigned i, unsigned size);
extern int rmsgRxQStop(unsigned i);
extern int rmsgRxQRun(unsigned i, unsigned id, unsigned mbox, void* buf, unsigned size);
extern int rmsgRxQWait(unsigned i, unsigned utimeout);

/***************************************************************/

int rmsgRxQIntEnable(unsigned i, unsigned mask)
{
    int lock;

    if(i > 7 || (mask & ~0xf))
        return -1;

    lock = intLock();
    rioRegs->endp.intrEnableAdd |= mask << i*4;
    intUnlock(lock);
    return 0;
}

/***************************************************************/

int rmsgRxQIntDisable(unsigned i, unsigned mask)
{
    int lock;

    if(i > 7 || (mask & ~0xf))
        return -1;

    lock = intLock();
    rioRegs->endp.intrEnableAdd &= ~(mask << i*4);
    intUnlock(lock);
    return 0;
}

/***************************************************************/

int rmsgRxQInit(unsigned i, unsigned size)
{
    unsigned n;

    if(i > 7 || !size || rmsgRxQDesc[i].nMax)
        return EINVAL;

    n = size / 0x1000;

    if(size & 0xfff)
        n++;

    if(!(rmsgRxQDesc[i].d = _cacheMalloc(n*sizeof(RIO_RX_DESC),64)))
        return ENOMEM;

    if(sem_init(&rmsgRxQDesc[i].sem,0,0) == -1)
    {
        cacheFree(rmsgRxQDesc[i].d);
        return errno;
    }

    rmsgRxQDesc[i].d0 = pciLocalToBus(rmsgRxQDesc[i].d);
    rmsgRxQDesc[i].nMax = n;
    rmsgRxQDesc[i].nCur = 0;
    memset(rmsgRxQDesc[i].d,0,n*sizeof(RIO_RX_DESC));
    return 0;
}

/***************************************************************/

int rmsgRxQStop(unsigned i)
{
    if(i > 7)
        return EINVAL;

    rmsgRxQDesc[i].nCur = 0;

    *RX_Q_REG(i,RX_Q_STAT) = *RX_Q_REG(i,RX_Q_STAT) & ~CT_LE_L(MSG_IN_RUN);

    if(*RX_Q_REG(i,RX_Q_STAT) & CT_LE_L(MSG_IN_RUN))
        return EBUSY;

    return 0;
}

/***************************************************************/

int rmsgRxQRun(unsigned i, unsigned id, unsigned mbox, void* buf, unsigned size)
{
    unsigned n, ix, daddr, baddr;

    n = size / 0x1000;

    if(size & 0xfff)
        n++;

    if(i > 7 || id > 0xff || mbox > 3 || !size || n > rmsgRxQDesc[i].nMax)
        return EINVAL;

    if((baddr = pciLocalToBus(buf)) == 0xffffffff)
        return EINVAL;

    if(*RX_Q_REG(i,RX_Q_STAT) & CT_LE_L(MSG_IN_RUN))
        return EBUSY;

    rmsgRxQDesc[i].nCur = n;
    rmsgRxQDesc[i].id = id;
    rmsgRxQDesc[i].mbox = mbox;

    daddr = rmsgRxQDesc[i].d0 + sizeof(RIO_RX_DESC);

    for(ix = 0; ix < n; ix++)
    {
        rmsgRxQDesc[i].d[ix].id = 0;
        rmsgRxQDesc[i].d[ix].buf1 = CT_LE_L(baddr);
        rmsgRxQDesc[i].d[ix].buf2 = CT_LE_L(baddr+256);
        rmsgRxQDesc[i].d[ix].next = CT_LE_L(daddr);
        rmsgRxQDesc[i].d[ix].stat = CT_LE_L(0x80000000);
    }
    rmsgRxQDesc[i].d[ix-1].next = CT_LE_L(1);
    boardWbFlush();

    *RX_Q_REG(i,RX_Q_ADDR) = CT_LE_L(rmsgRxQDesc[i].d0);
    *RX_Q_REG(i,RX_Q_TRAP) = CT_LE_L((mbox << 20) | id);
    *RX_Q_REG(i,RX_Q_STAT) = CT_LE_L(MSG_IN_RUN);
    return 0;
}

/***************************************************************/

int rmsgRxQWait(unsigned i, unsigned utimeout)
{
    unsigned stat, ix, n, id, mbox;

    if(i > 7 || !rmsgRxQDesc[i].nCur)
        return EINVAL;

    if((stat = *RX_Q_REG(i,RX_Q_STAT)) & CT_LE_L(MSG_IN_RUN))
    {
        struct timespec ts;
        unsigned long long t;
        int res;

        if(!utimeout)
            return EAGAIN;

        clock_gettime(CLOCK_REALTIME,&ts);
        t = ts.tv_sec * 1000000000ULL + ts.tv_nsec + utimeout*1000ULL;
        ts.tv_sec = t / 1000000000ULL;
        ts.tv_nsec = t % 1000000000ULL;

        rmsgRxQDesc[i].stat = 0;

        rmsgRxQIntEnable(i,RX_Q_INTR_EOL|RX_Q_INTR_FAIL);
        res = sem_timedwait(&rmsgRxQDesc[i].sem,&ts);
        rmsgRxQIntDisable(i,RX_Q_INTR_EOL|RX_Q_INTR_FAIL);

        if(res == -1)
            return errno;

        stat = rmsgRxQDesc[i].stat;

        if(!stat)
            return EACCES;
    }

    n = rmsgRxQDesc[i].nCur;
    rmsgRxQDesc[i].nCur = 0;

    *RX_Q_REG(i,RX_Q_STAT) = CT_LE_L(stat) & ~CT_LE_L(MSG_IN_RUN);

    if(stat & MSG_IN_FAIL)
        return EIO;

    RIO_DEBUG(4,("%s: i=%d d0=%x id=%d mbox=%d\n",(int)__FUNCTION__,i,
        (int)(&rmsgRxQDesc[i].d[0]),rmsgRxQDesc[i].id,rmsgRxQDesc[i].mbox,0));

    for(ix = 0; ix < n; ix++)
    {
        stat = rmsgRxQDesc[i].d[ix].stat;

        if(stat & CT_LE_L(0x80000000))
        {
            RIO_DEBUG(1,("%s: i=%d desc%d=%p own stat=%x\n",(int)__FUNCTION__,
                i,ix,(int)(&rmsgRxQDesc[i].d[ix]),CF_LE_L(stat),0));
            return EIO;
        }

        stat = CF_LE_L(stat);
        id = rmsgRxQDesc[i].d[ix].id;
        id = CF_LE_L(id);
        mbox = (stat >> 2) & 3;

        if(id != rmsgRxQDesc[i].id || mbox != rmsgRxQDesc[i].mbox)
        {
            RIO_DEBUG(1,("%s: i=%d desc%d=%p mismatch stat=%x id=%d\n",(int)__FUNCTION__,
                i,ix,(int)(&rmsgRxQDesc[i].d[ix]),stat,id));
            return EIO;
        }

        RIO_DEBUG(4,("%s: desc%d=%x stat=%x id=%d mbox=%d\n",(int)__FUNCTION__,
            ix,(int)(&rmsgRxQDesc[i].d[ix]),stat,id,mbox));
    }

    return 0;
}

/***************************************************************/

void rmsgIntr(unsigned stat)
{
    unsigned i, req, val;

    for(i = 0; i < 8 && stat; i++, stat >>= 4)
    {
        req = stat & 0xf;

        if(req)
        {
            int debug = 5;

            val = *RX_Q_REG(i,RX_Q_STAT);
            *RX_Q_REG(i,RX_Q_STAT) = val & ~CT_LE_L(MSG_IN_RUN);
            val = CF_LE_L(val);
            rmsgRxQDesc[i].stat = val;

            if(val & (MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_COMPLETE))
                debug = 1;

            RIO_DEBUG(debug,("%s: i=%d stat=%x\n",(int)__FUNCTION__,i,val,0,0,0));

            sem_post(&rmsgRxQDesc[i].sem);
        }
    }
}

/***************************************************************/

void boardRioFreeBuf(void* pBuf)
{
    unsigned offset = (unsigned)(pBuf - rmsgBufBase), i, mask, n, pa;
    
    i = offset / 0x1000;
    
    if(i > rmsgNBuf)
    {
        RIO_DEBUG(1,("%s: invalid buf=%p base=%x top=%x\n",(int)__FUNCTION__,(int)pBuf,
                     (int)rmsgBufBase,(int)(rmsgBufBase+(rmsgBufPCITop-rmsgBufPCIBase)),0,0));
                     return;
    }
    
    cacheDataInvalidate(pBuf,rmsgRxMsgSize[i]);
    
    mask = intLock();
    if((n = rmsgNOwn))
    {
        i = rmsgRxOwn;
        pa = rmsgBufPCIBase + offset;
        rmsgRxDesc[i].id = 0;
        rmsgRxDesc[i].buf1 = CT_LE_L(pa);
        rmsgRxDesc[i].buf2 = CT_LE_L(pa+256);
        rmsgRxDesc[i].stat = CT_LE_L(0x80000000);
        boardWbFlush();
        if(!rioMsgMode)
            rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_OWN|MSG_IN_DONE|MSG_IN_RUN|MSG_IN_CHAIN);
        rmsgNOwn--;
        rmsgRxOwn = (rmsgRxOwn + 1) % rmsgNBuf;
    }
    intUnlock(mask);
    
    if(!n)
        RIO_DEBUG(1,("%s: zero nOwn\n",(int)__FUNCTION__,0,0,0,0,0));
}

/***************************************************************/

static inline void rmsgRxJob(void)
{
    unsigned nSeg, segSize, lastSegSize, msgSize, mbox, id, stat, addr, i, j, letter;
    VOIDFUNCPTR hook;
    void* p;
    
    while(1)
    {
        i = rmsgRxCurrent;
        stat = rmsgRxDesc[i].stat;

        if((stat & CT_LE_L(0x80000000)) || stat == CT_LE_L(0x7fffffff))
            break;

        addr = rmsgRxDesc[i].buf1;
        addr = CF_LE_L(addr);

        if(addr < rmsgBufPCIBase || addr >= rmsgBufPCITop)
        {
            RIO_DEBUG(1,("%s: desc%d=%x invalid buf1=%x base=%x top=%x\n",(int)__FUNCTION__,
                         i,(int)(rmsgRxDesc+i),addr,rmsgBufPCIBase,rmsgBufPCITop));
            return;
        }

        stat = CF_LE_L(stat);
        id = rmsgRxDesc[i].id;
        id = CF_LE_L(id);
        nSeg = (stat >> 4) & 0xf;
        segSize = 8 << (((stat >> 8) & 0xf) - 9);
        lastSegSize = 8 << (((stat >> 12) & 0xf) - 9);
        msgSize = segSize*nSeg + lastSegSize;
        mbox = (stat >> 2) & 3;
        letter = stat & 3;

        if(msgSize > 0x1000)
        {
            RIO_DEBUG(1,("%s: invalid message i=%d desc=%p size=%x stat=%x id=%x\n",
                (int)__FUNCTION__,i,(int)&rmsgRxDesc[i],msgSize,stat,id));
            msgSize = 0;
        }

        p = rmsgBufBase + (addr - rmsgBufPCIBase);

        RIO_DEBUG(4,("%s: desc%d=%x stat=%x id=%d mbox=%d\n",(int)__FUNCTION__,
                     i,(int)(rmsgRxDesc+i),stat,id,mbox));
        RIO_DEBUG(4,("    buf=%p nSeg=%d segSize=%x lastSegSize=%x msgSize=%x letter=%d\n",
                     (int)p,nSeg,segSize,lastSegSize,msgSize,letter));

        rmsgRxDesc[i].stat = CT_LE_L(0x7fffffff);

        if(!rmsgNOwn)
            rmsgRxOwn = rmsgRxCurrent;

        rmsgRxCurrent = (rmsgRxCurrent + 1) % rmsgNBuf;
        rmsgNOwn++;

        j = (addr - rmsgBufPCIBase) / 0x1000;
        rmsgRxMsgSize[j] = msgSize;
        
        if((hook = rmsgMboxRxHook[mbox]) && msgSize)
        {
#if defined(INCLUDE_RREG) && defined(RMSG_CHAN_MBOX)
            if(!rioMsgMode && mbox == RMSG_CHAN_MBOX && rregHostTypeGet(id) == RIO_HOST_K128)
            {
                unsigned ix, nw = msgSize / 4, val;
                unsigned* p32 = p;

                for(ix = 0; ix < nw; ix++)
                {
                    val = p32[ix];
                    p32[ix] = CT_LE_L(val);
                }
            }
#endif
            hook(p,id,msgSize);
        }
        else
        {
            boardRioFreeBuf(p);
        }
    }
}

/***************************************************************/

void rmsgRxIntr(unsigned unused, unsigned mask)
{
    unsigned stat = rioRegs->endp.msgInStatus;

    if(stat & CT_LE_L(MSG_IN_FAIL))
    {
        unsigned err = rioRegs->endp.msgInError;
        rioRegs->endp.msgInError = err;
        RIO_DEBUG(1,("%s: error=%x stat=%x\n",(int)__FUNCTION__,CF_LE_L(err),CF_LE_L(stat),0,0,0));
    }

    if(!rioMsgMode && !(stat & CT_LE_L(MSG_IN_RUN)))
    {
        RIO_DEBUG(1,("%s: stat=%x NOT RUNNING!!!\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));
    }

    RIO_DEBUG(4,("%s: stat=%x\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));
    rioRegs->endp.msgInStatus = stat & ~CT_LE_L(MSG_IN_OWN);

    if(rmsgRxThreadFlag)
    {
        if(!rioMsgMode)
            rioIntDisable(INTR_MSG_IN_DONE);
        else
            rioIntDisable(INTR_MSG_IN_POL);
        sem_post(&rmsgRxSem);
    }
    else
    {
        rmsgRxJob();
    }
}

/***************************************************************/

void* rmsgRxThread(void* unused)
{
    while(1)
    {
        if(sem_wait(&rmsgRxSem) == -1)
        {
            RIO_DEBUG(1,("%s: sem_wait error: %s (%d)\n\r",(int)__FUNCTION__,
                         (int)strerror(errno),errno,0,0,0));
                         return NULL;
        }

        rmsgRxJob();

        if(!rioMsgMode)
            rioIntEnable(INTR_MSG_IN_DONE);
        else
            rioIntEnable(INTR_MSG_IN_POL);
    }
}

/***************************************************************/

int _rmsgSendOld(unsigned id, unsigned mbox, void* ptr, unsigned size)
{
    unsigned pa, val, ix, stat, err;
    
    pa = pciLocalToBus(ptr);
    
    if(pa & 7)
    {
        RIO_DEBUG(1,("%s: id=%d unaligned pa=%08x ptr=%p\n",(int)__FUNCTION__,
                     id,pa,(int)ptr,0,0));
                     return EINVAL;
    }
    
    cacheDataFlush(ptr,size);
    
    if(size == 0x1000)
        val = (0xf << 4) | (0xe << 8) | (0xe << 12) | 0x80000000 | (mbox << 2);
    else
        val = (0xe << 8) | (0xe << 12) | 0x80000000 | (mbox << 2);
    
    RIO_DEBUG(4,("%s: id=%d pa=%x\n",(int)__FUNCTION__,id,pa,0,0,0));
    
    rmsgTxDesc->id = CT_LE_L(id);
    rmsgTxDesc->buf1 = CT_LE_L(pa);
    rmsgTxDesc->buf2 = CT_LE_L(pa+256);
    rmsgTxDesc->stat = CT_LE_L(val);
    
    boardWbFlush();
    
    stat = rioRegs->endp.msgOutStatus;
    
    if(stat & CT_LE_L(MSG_OUT_RUN))
    {
        RIO_DEBUG(1,("%s: controller busy: stat=%x\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));
        return EIO;
    }
    
    if(stat & CT_LE_L(MSG_OUT_FAIL))
    {
        err = rioRegs->endp.msgOutError;
        rioRegs->endp.msgOutError = err;
        RIO_DEBUG(1,("%s: last controller error=%x stat=%x\n",(int)__FUNCTION__,CF_LE_L(err),CF_LE_L(stat),0,0,0));
    }

    rioRegs->endp.msgOut[0] = rmsgTxDescAddr;
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_FAIL|MSG_OUT_EOL);

    for(ix = 0; ix < 1000; ix++)
    {
        if(!((stat = rioRegs->endp.msgOutStatus) & CT_LE_L(MSG_OUT_RUN)))
        {
            rioRegs->endp.msgOutStatus = stat;

            if(stat & CT_LE_L(MSG_OUT_FAIL))
            {
                err = rioRegs->endp.msgOutError;
                rioRegs->endp.msgOutError = err;
                RIO_DEBUG(1,("%s: controller error=%x stat=%x id=%x pa=%x\n",(int)__FUNCTION__,CF_LE_L(err),CF_LE_L(stat),id,pa,0));
                return EIO;
            }

            stat = rmsgTxDesc->stat;

            if((stat & CT_LE_L(0x80010000)))
            {
                err = rioRegs->endp.msgOutStatus;
                RIO_DEBUG(1,("%s: invalid descriptor stat=%x id=%x pa=%x ctrl=%x\n",(int)__FUNCTION__,CF_LE_L(stat),id,pa,CF_LE_L(err),0));
                return EIO;
            }

            return 0;

        }
        udelay(1);
    }

    RIO_DEBUG(1,("%s: timeout stat=%x id=%x pa=%x\n",(int)__FUNCTION__,CF_LE_L(stat),id,pa,0,0));
    return ETIMEDOUT;
}

int rmsgSendOld(unsigned id, unsigned mbox, void* ptr, unsigned size)
{
#ifdef RIO_TX_MUTEX
    int res;
    pthread_mutex_lock(&rmsgTxMutex);
    res = _rmsgSendOld(id,mbox,ptr,size);
    pthread_mutex_unlock(&rmsgTxMutex);
    return res;
#else
    return _rmsgSendOld(id,mbox,ptr,size);
#endif
}

#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))

/***************************************************************/

int boardRioSendMsg0(int cpu, chanBUFNODE* pBufNode, int size)
{
    unsigned err, n, ix, partsize, tail, lastpart, k128;
    void* ptr = chanBUFNODE_TO_HEADBUF(pBufNode);
    void* buf;
    VOIDFUNCPTR hook = rmsgMboxTxHook[RMSG_CHAN_MBOX];
    
    if(!size)
    {
        RIO_DEBUG(1,("%s: invalid size=%x\n",(int)__FUNCTION__,size,0,0,0,0));
        err = EINVAL;
        goto fail;
    }

    if(!rregIsReady(cpu,RREG_DB_CHAN_IX))
    {
        RIO_DEBUG(1,("%s: cpu=%d not ready\n",(int)__FUNCTION__,cpu,0,0,0,0));
        err = EINVAL;
        goto fail;
    }

    n = size / 0x1000;
    tail = size & 0xfff;
    lastpart = 0x1000;
    k128 = (rregHostTypeGet(cpu) == RIO_HOST_K128);

    if(tail)
    {
        n++;

        if(tail <= 256)
    	    lastpart = 256;
    }

    RIO_DEBUG(3,("%s:cpu=%d size=%x ptr=%p tail=%x\n",(int)__FUNCTION__,
	cpu,size,(int)ptr,tail,0));

    for(ix = 0, err = 0, partsize = 0x1000; ix < n; ix++, ptr += 0x1000)
    {
	if(ix == n - 1)
	    partsize = lastpart;

	if(k128)
	{
    	    unsigned* src = ptr;
    	    unsigned nBytes = 0x1000, nw, val, i;

	    if(ix == n - 1 && tail)
		nBytes = tail;

	    nw = nBytes / 4;
    	    if(nBytes & 3)
        	nw++;
    	    for(i = 0; i < nw; i++)
    	    {
        	val = src[i];
        	rmsgTxBuf[i] = CT_LE_L(val);
    	    }
    	    buf = rmsgTxBuf;
	}
	else
	{
	    buf = ptr;
	}
    
        if((err = rmsgSendOld(cpu,RMSG_CHAN_MBOX,buf,partsize)))
    	    goto fail;
    }

fail:
    hook(cpu,pBufNode,err);
    return err;
}

#endif

/***************************************************************/

void rmsgTxCheck(void)
{
    unsigned mask = 0, stat, id, mbox, err, i, debug, job;
    RIO_TX_DESC* d;
    void* ptr;
    VOIDFUNCPTR hook;

    do
    {
        ptr = NULL;
        err = 0;
        id = -1;
        mbox = -1;
        debug = 4;
        job = FALSE;

        if(!intNestLevel)
            mask = intLock();

        i = rmsgTxDirty;
        d = &rmsgTxDesc[i];

        stat = d->stat;
        stat = CF_LE_L(stat);

        if(stat & 0x80000000)
            goto done;

        if(stat == 0x7fffffff)
            goto done;

        if(stat & 0x10000)
            err = EIO;

        mbox = (stat >> 2) & 3;
        id = d->id; id = CF_LE_L(id);

        ptr = rmsgTxNode[i];
        d->stat = CT_LE_L(0x7fffffff);
        rmsgTxDirty = (rmsgTxDirty + 1) % RMSG_TX_RING_SIZE;
        rmsgTxNode[i] = NULL;
        job = TRUE;

done:
        if(!intNestLevel)
            intUnlock(mask);

        if(err)
            debug = 1;

        if(ptr)
        {
            if((hook = rmsgMboxTxHook[mbox]))
                hook(id,ptr,err);
        }

        RIO_DEBUG(debug,("%s: desc%d=%x stat=%x id=%d mbox=%d\n",(int)__FUNCTION__,
                         i,(int)d,stat,id,mbox));

    } while(job);
}

/***************************************************************/

void rmsgTxPolIntr(unsigned unused, unsigned mask)
{
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_POL|MSG_OUT_MODE|MSG_OUT_RUN);
    rmsgTxCheck();
}

/***************************************************************/

void rmsgTxErrRecover(void)
{
    unsigned stat, id, mbox, err, i;
    RIO_TX_DESC* d;
    void* ptr;
    VOIDFUNCPTR hook;
    
    while(1)
    {
        err = 0;
        
        i = rmsgTxDirty;
        d = &rmsgTxDesc[i];
        
        stat = d->stat;
        stat = CF_LE_L(stat);
        
        if(stat == 0x7fffffff)
        {
            unsigned descAddr0 = pciLocalToBus(rmsgTxDesc);
            rmsgTxDirty = 0;
            rmsgTxCurrent = 0;
            rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_FAIL|MSG_OUT_EOL|MSG_OUT_OWN|MSG_OUT_COMPLETE|MSG_OUT_POL);
            rioRegs->endp.msgOut[0] = CT_LE_L(descAddr0);
            return;
        }
        
        if(stat & 0x80010000)
            err = EIO;
        
        mbox = (stat >> 2) & 3;
        id = d->id; id = CF_LE_L(id);
        
        ptr = rmsgTxNode[i];
        d->stat = CT_LE_L(0x7fffffff);
        rmsgTxDirty = (rmsgTxDirty + 1) % RMSG_TX_RING_SIZE;
        rmsgTxNode[i] = NULL;
        
        if(ptr)
        {
            if((hook = rmsgMboxTxHook[mbox]))
                hook(id,ptr,err);
        }
        
        RIO_DEBUG(1,("%s: desc%d=%x stat=%x id=%d mbox=%d\n",(int)__FUNCTION__,
                     i,(int)d,stat,id,mbox));
    }
}

/***************************************************************/

void rmsgTxErrIntr(unsigned unused, unsigned mask)
{
    unsigned err = rioRegs->endp.msgOutError;
    rioRegs->endp.msgOutError = err;
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_FAIL);
    
    RIO_DEBUG(1,("%s: error=%x\n",(int)__FUNCTION__,CF_LE_L(err),0,0,0,0));
    
    if(err & CT_LE_L(0x800))
        rmsgTxCheck();
    else
        rmsgTxErrRecover();
}

/***************************************************************/

int _rmsgTxQueue(int id, int mbox, int size, void* ptr, void* arg, unsigned pol)
{
    unsigned pa, val;
    RIO_TX_DESC* d = &rmsgTxDesc[rmsgTxCurrent];

    if(!size || size > 0x1000)
    {
        RIO_DEBUG(1,("%s: invalid size=%x\n",(int)__FUNCTION__,size,0,0,0,0));
        return EINVAL;
    }

    if(rmsgAlignTxSize && (size & 0xff))
        size = (size + 0xff) & ~0xff;

    pa = pciLocalToBus(ptr);

    if(pa & 7)
    {
        RIO_DEBUG(1,("%s: id=%d unaligned pa=%08x node=%p ptr=%p\n",(int)__FUNCTION__,
    	    id,pa,(int)arg,(int)ptr,0));
        return EINVAL;
    }

    cacheDataFlush(ptr,size);

    if(size == 0x1000)
    {
        val = (0xf << 4) | (0xe << 8) | (0xe << 12) | 0x80000000;
    }
    else
    {
        if(size <= 256)
        {
            unsigned segSize = rioSegSize[size-1];
            val = (segSize << 8) | (segSize << 12) | 0x80000000;
        }
        else
        {
            unsigned nSeg = size >> 8;
            unsigned segSize = 0xe;

            size &= 0xff;

            if(size)
            {
                nSeg++;
                segSize = rioSegSize[size-1];
            }

            val = ((nSeg-1) << 4) | (0x0e << 8) | (segSize << 12) | 0x80000000;
        }
    }

    val |= mbox << 2;

    if(d->stat != CT_LE_L(0x7fffffff))
    {
        RIO_DEBUG(1,("%s: desc%d=%p invalid stat=%x\n",(int)__FUNCTION__,rmsgTxCurrent,(int)d,CT_LE_L(d->stat),0,0));
        return EAGAIN;
    }

    rmsgTxCheck();

    RIO_DEBUG(4,("%s: desc%d=%p stat=%x id=%d pa=%x\n",(int)__FUNCTION__,rmsgTxCurrent,(int)d,val,id,pa));

    d->id = CT_LE_L(id);
    d->buf1 = CT_LE_L(pa);
    d->buf2 = CT_LE_L(pa+256);
    rmsgTxNode[rmsgTxCurrent] = arg;
    d->next = (d->next & CT_LE_L(0xfffffffc)) | CT_LE_L(pol);
    d->stat = CT_LE_L(val);

    boardWbFlush();
    rmsgTxCurrent = (rmsgTxCurrent + 1) % RMSG_TX_RING_SIZE;
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_MODE);
    return 0;
}

int rmsgTxQueue(int id, int mbox, int size, void* ptr, void* arg, unsigned pol)
{
#ifdef RIO_TX_MUTEX
    int res;
    pthread_mutex_lock(&rmsgTxMutex);
    res = _rmsgTxQueue(id,mbox,size,ptr,arg,pol);
    pthread_mutex_unlock(&rmsgTxMutex);
    return res;
#else
    return _rmsgTxQueue(id,mbox,size,ptr,arg,pol);
#endif
}

#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))

/***************************************************************/

int boardRioSendMsg1(int cpu, chanBUFNODE* pBufNode, int size)
{
    int err = 0;
    unsigned ix, n, partsize, pol;
    VOIDFUNCPTR hook = rmsgMboxTxHook[RMSG_CHAN_MBOX];

    if(!rregIsReady(cpu,RREG_DB_CHAN_IX))
    {
        RIO_DEBUG(1,("%s: cpu=%d not ready\n",(int)__FUNCTION__,cpu,0,0,0,0));
        err = EINVAL;
        goto fail;
    }

#if 0
    #ifdef chanHEADBUF_PART
    {
        chanPART_NODE* pPart;
        chanHEADBUF* pHdrBuf;
        void* arg = NULL;
        
        pHdrBuf = chanBUFNODE_TO_FIRST_HEADBUF(pBufNode);
        n = chanHEADBUF_PARTS(pHdrBuf);
        pPart = chanBUFNODE_TO_FIRST_NODEPART(pBufNode);
        
        for(ix = 0, pol = 0; ix < n; ix++, pPart = chanHEADPART_NEXT(pPart))
        {
            if(ix == n - 1)
            {
                arg = pBufNode;
                pol = 2;
            }
            pHdrBuf = chanHEADPART_TO_HEADBUF(pPart);
            partsize = chanHEADBUF_SIZE(pHdrBuf) + chanHEADBUFSIZE;
            if((err = rmsgTxQueue(cpu,RMSG_CHAN_MBOX,partsize,pHdrBuf,arg,pol)))
                goto fail;
        }
    }
    #endif
#endif

    if(size <= 0x1000)
    {
        if((err = rmsgTxQueue(cpu,RMSG_CHAN_MBOX,size,chanBUFNODE_TO_HEADBUF(pBufNode),pBufNode,2)))
            goto fail;
    }
    else
    {
        void* ptr = chanBUFNODE_TO_HEADBUF(pBufNode);
        void* arg = NULL;

        n = size / 0x1000;
        if(size & 0xfff)
            n++;

        for(ix = 0, pol = 0, partsize = 0x1000; ix < n; ix++, ptr += 0x1000)
        {
            if(ix == n - 1)
            {
                arg = pBufNode;
                pol = 2;
                if(size & 0xfff)
                    partsize = size & 0xfff;
            }
            if((err = rmsgTxQueue(cpu,RMSG_CHAN_MBOX,partsize,ptr,arg,pol)))
                goto fail;
        }
    }
    return 0;
fail:
    hook(cpu,pBufNode,err);
    return err;
}

#endif  // OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))

/***************************************************************/

#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))

int boardRioSendMsg(int cpu, chanBUFNODE* pBufNode, int size)
{
    if(!rioMsgMode)
        return boardRioSendMsg0(cpu,pBufNode,size);
    return boardRioSendMsg1(cpu,pBufNode,size);
}

#endif

/***************************************************************/

#if defined(chanHEADBUF_PART) || OS_VERSION > 305 || (OS_VERSION == 305 && OS_RELEASE >= 7)  || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))

void boardRioParamGet(BOARD_RIO_PARAM* p)
{
    p->mtu = 0x1000;
    p->sizeAlign = 0x100;
    p->addrAlign = CACHE_ALIGN_SIZE;
}

#endif

/***************************************************************/

#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))
int boardRioInit(int nBuf, int size, void(*txRtn)(int,chanBUFNODE*,int), void(*rxRtn)(void*,int))
#else
int boardRioInitOld(int nBuf, int size)
#endif
{
    unsigned ix, descAddr, descAddr0, pciAddr, pol, rreg = FALSE, v;

    if(size > 0x1000)
    {
        RIO_DEBUG(1,("%s: invalid size=%x\n",(int)__FUNCTION__,size,0,0,0,0));
        return EINVAL;
    }
    
#if OS_VERSION >= 300 || (defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD))
    if(rxRtn || txRtn)
    {
        rmsgMboxRxHook[RMSG_CHAN_MBOX] = (void*)rxRtn;
        rmsgMboxTxHook[RMSG_CHAN_MBOX] = (void*)txRtn;
        rreg = TRUE;
    }
#endif

#if defined(INCLUDE_RMSG_NET)
    if(boardRioInitDone)
    {
        #ifdef RMSG_CHAN_RX_SIZE
        if(nBuf > RMSG_CHAN_RX_SIZE)
        {
            RIO_DEBUG(1,("%s: RIO configuration nBuf=%d > RMSG_CHAN_RX_SIZE=%d\n",
                         (int)__FUNCTION__,nBuf,RMSG_CHAN_RX_SIZE,0,0,0));
        }
        #elif defined(INCLUDE_CHANNELS) && defined(MPCHAN_METHOD)
        #error RMSG_CHAN_RX_SIZE not defined
        #endif
        goto done;
    }
#endif
    
    RIO_DEBUG(4,("%s: nBuf=%d size=%x\n",(int)__FUNCTION__,nBuf,size,0,0,0));
    
    if(nBuf < 256)
        nBuf = 256;
    
    rmsgNBuf = nBuf;
    rioRegs->endp.msgOut[2] = CT_LE_L(0xc);
    rioRegs->endp.msgOutError = rioRegs->endp.msgOutError;
    if(!rioMsgMode)
        rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_FAIL|MSG_OUT_EOL|MSG_OUT_OWN|MSG_OUT_COMPLETE);
    else
        rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_FAIL|MSG_OUT_EOL|MSG_OUT_OWN|MSG_OUT_COMPLETE|MSG_OUT_POL);
    
    size = rmsgNBuf*sizeof(int);
    if(!(rmsgRxMsgSize = calloc(rmsgNBuf,sizeof(int))))
    {
        RIO_DEBUG(1,("%s: cannot calloc %d bytes\n",(int)__FUNCTION__,size,0,0,0,0));
        return ENOMEM;
    }
    
    size = rmsgNBuf*sizeof(RIO_RX_DESC);
    if(!(rmsgRxDesc = _cacheMalloc(size,64)))
    {
        RIO_DEBUG(1,("%s: cannot cacheMalloc %d bytes\n",(int)__FUNCTION__,size,0,0,0,0));
        return ENOMEM;
    }
    memset(rmsgRxDesc,0,size);
    
    size = rmsgNBuf*0x1000;
    if(!(rmsgBufBase = _cacheMalloc(size,0x1000)))
    {
        RIO_DEBUG(1,("%s: cannot cacheMalloc %d bytes\n",(int)__FUNCTION__,size,0,0,0,0));
        return ENOMEM;
    }
    memset(rmsgBufBase,0,size);
#if OS_VERSION >= 300
    rmsgBufBase = boardUncachedToCached(rmsgBufBase);
#else
    rmsgBufBase = (void*)K1_TO_K0(rmsgBufBase);
#endif

    size = !rioMsgMode ? sizeof(RIO_TX_DESC) :  RMSG_TX_RING_SIZE*sizeof(RIO_TX_DESC);
    if(!(rmsgTxDesc = _cacheMalloc(size,64)))
    {
        RIO_DEBUG(1,("%s: cannot cacheMalloc %d bytes\n",(int)__FUNCTION__,size,0,0,0,0));
        return ENOMEM;
    }
    memset(rmsgTxDesc,0,size);

    if(!rioMsgMode)
    {
        if(!(rmsgTxBuf = (void*)cacheAlignMalloc(0x1000,0x100)))
        {
            RIO_DEBUG(1,("%s: cannot cacheAlignMalloc %d bytes\n",(int)__FUNCTION__,0x1000,0,0,0,0));
            return ENOMEM;
        }
    }

    {
        pthread_attr_t attr;
        struct sched_param schedparam;
        unsigned res;
        
        if(sem_init(&rmsgRxSem,1,0) == -1)
        {
            RIO_DEBUG(1,("%s: sem_init error: %s (%d)\n\r",(int)__FUNCTION__,
                         (int)strerror(errno),errno,0,0,0));
                         return errno;
        }
        
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
        schedparam.sched_priority = RMSG_RX_THREAD_PRI;
        pthread_attr_setschedparam(&attr,&schedparam);
        
        if((res = pthread_create(NULL,&attr,(void*)&rmsgRxThread,NULL)))
        {
            RIO_DEBUG(1,("%s: pthread_create error: %s (%d)\n\r",(int)__FUNCTION__,
                         (int)strerror(res),res,0,0,0));
                         return res;
        }
    }
    
    pciAddr = pciLocalToBus(rmsgBufBase);
    rmsgBufPCIBase = pciAddr;
    rmsgBufPCITop = pciAddr + rmsgNBuf*0x1000;
    
    descAddr0 = pciLocalToBus(rmsgRxDesc);
    descAddr = descAddr0 + sizeof(RIO_RX_DESC);
    
    pol = !rioMsgMode ? 0 : 2;
    
    for(ix = 0; ix < rmsgNBuf; ix++, pciAddr += 0x1000, descAddr += sizeof(RIO_RX_DESC))
    {
        rmsgRxDesc[ix].stat = CT_LE_L(0x80000000);
        rmsgRxDesc[ix].id = 0;
        rmsgRxDesc[ix].buf1 = CT_LE_L(pciAddr);
        rmsgRxDesc[ix].buf2 = CT_LE_L(pciAddr+256);
        rmsgRxDesc[ix].next = CT_LE_L(descAddr|pol);
    }
    
    rmsgRxDesc[ix-1].next = CT_LE_L(descAddr0|pol);
    rioRegs->endp.msgIn[0] = CT_LE_L(descAddr0);
    
    rioRegs->endp.msgInError = rioRegs->endp.msgInError;

    if(!rioMsgMode)
    {
        rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN);
    }
    else
    {
        rioRegs->endp.msgIn[1] = CT_LE_L(0x30000);  // [16] - retry enable, [17] - error response on timeout;
        rioRegs->err.rw[1] |= CT_LE_L(0x400);           // [10] - msgInnOwnEna
        rioRegs->endp.msgOut[1] = CT_LE_L(0xff00);      // [15:8] - retry count bits [15:8]
#warning MsgIn9LetterStrong OFF
        rioRegs->err.rw[3] &= ~CT_LE_L(0x400000);        // [22] - MsgIn9LetterStrong
//         rioRegs->err.rw[3] |= CT_LE_L(0x400000);        // [22] - MsgIn9LetterStrong
        rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN|MSG_IN_CHAIN|MSG_IN_POL);
    }

#ifdef RIO_TX_MUTEX
    #if OS_VERSION >= 300
        pshared_mutex_init(&rmsgTxMutex,"rmsgTx");
    #else
        pthread_mutex_init(&rmsgTxMutex,NULL);
    #endif
#endif

    if(!rioMsgMode)
    {
        rmsgTxDescAddr = pciLocalToBus(rmsgTxDesc);
        rmsgTxDescAddr = CT_LE_L(rmsgTxDescAddr);

        rmsgTxDesc->stat = CT_LE_L(0x7fffffff);
        rmsgTxDesc->id = 0xffffffff;
        rmsgTxDesc->buf1 = 0;
        rmsgTxDesc->buf2 = 0;
        rmsgTxDesc->next = CT_LE_L(1);
    }
    else
    {
        descAddr0 = pciLocalToBus(rmsgTxDesc);
        descAddr = descAddr0 + sizeof(RIO_TX_DESC);
        for(ix = 0; ix < RMSG_TX_RING_SIZE; ix++, descAddr += sizeof(RIO_TX_DESC))
        {
            rmsgTxDesc[ix].stat = CT_LE_L(0x7fffffff);
            rmsgTxDesc[ix].id = 0xffffffff;
            rmsgTxDesc[ix].buf1 = 0;
            rmsgTxDesc[ix].buf2 = 0;
            rmsgTxDesc[ix].next = CT_LE_L(descAddr|2);
        }

        rmsgTxDesc[ix-1].next = CT_LE_L(descAddr0|2);
        rioRegs->endp.msgOut[0] = CT_LE_L(descAddr0);

        rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_MODE);

        rmsgTxDirty = 0;
        rmsgTxCurrent = 0;
    }

    rmsgRxOwn = 0;
    rmsgRxCurrent = 0;
    
    for(ix = 0; ix < 8; ix++)
        rioSegSize[ix] = 0x9;
    for(; ix < 16; ix++)
        rioSegSize[ix] = 0xa;
    for(; ix < 32; ix++)
        rioSegSize[ix] = 0xb;
    for(; ix < 64; ix++)
        rioSegSize[ix] = 0xc;
    for(; ix < 128; ix++)
        rioSegSize[ix] = 0xd;
    for(; ix < 256; ix++)
        rioSegSize[ix] = 0xe;

#ifdef  RMSG_RX_THREAD
    rmsgRxThreadFlag = TRUE;
#else
    rmsgRxThreadFlag = FALSE;
#endif

    if(!rioMsgMode)
    {
        rioIntConnect(IRQ_MSG_IN_DONE,rmsgRxIntr,0);
        rioIntEnable(INTR_MSG_IN_DONE);
    }
    else
    {
        v = rioRegs->endp.msgOut[1];
        v = CF_LE_L(v) & 0xffff;
        v |= (RMSG_TX_INT_COUNT << 16) | (RMSG_TX_INT_DELAY << 24);
        rioRegs->endp.msgOut[1] = CT_LE_L(v);

        v = (RMSG_RX_INT_COUNT << 16) | (RMSG_RX_INT_DELAY << 24);
        rioRegs->endp.msgIn3 = CT_LE_L(v);

        rioIntConnect(IRQ_MSG_IN_POL,rmsgRxIntr,0);
        rioIntConnect(IRQ_MSG_IN_ERR,rmsgRxIntr,0);
        rioIntConnect(IRQ_MSG_OUT_POL,rmsgTxPolIntr,0);
        rioIntConnect(IRQ_MSG_OUT_ERR,rmsgTxErrIntr,0);
        rioIntEnable(INTR_MSG_OUT_ERR|INTR_MSG_IN_POL|INTR_MSG_IN_ERR|INTR_MSG_OUT_POL);
    }

    boardRioInitDone = TRUE;

    RIO_DEBUG(2,("%s: txDesc=%p rxDesc=%p rxBufs=%p nRxBuf=%d\n",(int)__FUNCTION__,
                 (int)rmsgTxDesc,(int)rmsgRxDesc,(int)rmsgBufBase,rmsgNBuf,0));

#if defined(INCLUDE_RMSG_NET)
done:
#endif

#ifdef  INCLUDE_RREG
    if(rreg)
    {
        rregInit(RREG_ID_MAX,RREG_THREAD_PRI,NULL, RIO_HOST_K64);
        rregEnable(RREG_DB_CHAN_IX);
        rregResyncAll(RREG_DB_CHAN_IX,FALSE);
#ifdef RREG_ID00
        rregWait(RREG_DB_CHAN_IX,RREG_TIMEOUT);
#endif
    }

#else
    #error RREG not included
    sleep(1);
#endif
    return 0;
}

void rmsgShow()
{
    printf("RX: n=%d base=%p desc=%p\n cur=%d (%p) nOwn=%d own=%d (%p)\n",rmsgNBuf,
	rmsgBufBase,rmsgRxDesc,rmsgRxCurrent,&rmsgRxDesc[rmsgRxCurrent],rmsgNOwn,
	rmsgRxOwn,&rmsgRxDesc[rmsgRxOwn]);
    printf("TX: n=%d buf=%p desc=%p\n cur=%d (%p) dirty=%d (%p)\n",RMSG_TX_RING_SIZE,
	rmsgTxBuf,rmsgTxDesc,rmsgTxCurrent,&rmsgTxDesc[rmsgTxCurrent],
	rmsgTxDirty,&rmsgTxDesc[rmsgTxDirty]);
}

#endif  // INCLUDE_RMSG_TEST



