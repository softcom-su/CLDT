#include <semaphore.h>
#include <errno.h>
#include <mem.h>

#define RIO_RX_MAX      (RIO_TX_MAX*4*RIO_ID_MAX)
#define RIO_TX_MAX      32
//#define       RIO_TX_MAX      1
#define RIO_BUF_MAX     (RIO_TX_MAX*0x1000)

#define RMSG_TX_RING_SIZE        2048

#define STAT_FIRST      0x80000000
#define STAT_LAST       0x40000000
#define STAT_ACK        0x20000000
#define STAT_NO_DATA    0x10000000

#define RMSG_OPT_NO_DATA        0x00000001
#define RMSG_OPT_NO_ACK         0x00000002
#define RMSG_OPT_TX_RESTART     0x00000004
#define RMSG_OPT_TX_CHAIN       0x00000010

#define STAT_MASK       0xffff0000

#define RMSG_DB         0

typedef struct {
    int         initDone;
    sem_t       dbSem;
    sem_t       txSem;
    unsigned    txCount;
    unsigned    rxCount;
    unsigned    nTxMsg;
    unsigned    nRxMsg;
    unsigned    nRxDb;
    unsigned    nTxDb;
    unsigned    nTxPOL;
    unsigned long long nTxBytes;
} RMSG_TEST;

static sem_t rxSem, txSem;
static volatile int txError = 0;
volatile unsigned nTxEOL = 0, nTxPOL = 0;
static volatile int rxError = 0;
static RMSG_TEST rmsgTest[RIO_ID_MAX][4];
static unsigned alignMask = 0xff >> 2;
static unsigned rxAlignSize = 0x1000;
int rmsgShow = FALSE;
volatile unsigned rmsgPrintPeriod = 0;
pthread_t rmsgRxParseThreadDesc;
unsigned rxOwnFlag = FALSE, rxOwnCount = 0, rxStopped = FALSE, txChainMode = FALSE;
static unsigned cpuCount[4];

unsigned char rioSegSize[256];
void* rxLastDesc;

#define RMSG_BUF_MAX    (RIO_RX_MAX*2)

typedef struct rmsg_buf_entry {
    struct rmsg_buf_entry*      next;
    void*                       addr;
    unsigned                    pciAddr;
    unsigned                    stat;
    unsigned                    id;
} RMSG_BUF_ENTRY;

RMSG_BUF_ENTRY rmsgBufArray[RMSG_BUF_MAX];
RMSG_BUF_ENTRY* rmsgBufFree;
RMSG_BUF_ENTRY* rmsgBufBusy[RIO_RX_MAX];
mqd_t rmsgMQ;
pthread_mutex_t rmsgMutex, rmsgTxMutex;
pthread_cond_t rmsgCond;
static RIO_TX_DESC* txRing;
//static
unsigned txDirty, txCurrent;

void rmsgTxAlign(unsigned size)
{
    if(!size)
    {
        alignMask = 0;
        return;
    }

    alignMask = (size - 1) >> 2;
}

void rmsgRxAlign(unsigned size)
{
    rxAlignSize = size;
}

void txSemVal(void)
{
    unsigned val = 0xeeeeeeee;
    if(sem_getvalue(&txSem,&val) == -1)
    {
        printf("sem_getvalue error\n");
    }
    else
    {
        printf("val=%d\n",val);
    }
}

void rioTxEOLIntr(unsigned unused, unsigned mask)
{
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_EOL);
    sem_post(&txSem);
    nTxEOL++;
}

void rioTxPOLIntr(unsigned unused, unsigned mask)
{
    unsigned d1 = 0;
    cpuCount[1] = boardCountGet();
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_POL|MSG_OUT_MODE|MSG_OUT_RUN);
    sem_post(&txSem);
    nTxPOL++;
#warning test code
    if(cpuCount[3])
        d1 = (cpuCount[3] - cpuCount[0]) * 1000 / 24;
//    logMsg("tx delta=%d d1=%d\n",(cpuCount[1]-cpuCount[0])*1000/24,d1,0,0,0,0);
}

void rioTxErrIntr(unsigned unused, unsigned mask)
{
    unsigned err = rioRegs->endp.msgOutError;
    unsigned stat = rioRegs->endp.msgOutStatus;
    rioRegs->endp.msgOutError = err;
    txError = CF_LE_L(err);
    rioRegs->endp.msgOutStatus = stat;
    sem_post(&txSem);
    logMsg("%s: error=%x stat=%x\n",(int)__FUNCTION__,txError,CF_LE_L(stat),0,0,0);
}

void rioRxErrIntr(unsigned unused, unsigned mask)
{
    unsigned err = rioRegs->endp.msgOutError;
    rioRegs->endp.msgOutError = err;
    rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_FAIL);
    rxError = CF_LE_L(err);
    logMsg("%s: error=%x\n",(int)__FUNCTION__,rxError,0,0,0,0);
}

void rioTxOwnIntr(unsigned unused, unsigned mask)
{
    unsigned err = rioRegs->endp.msgOutError, stat;
    stat = rioRegs->endp.msgOutStatus;
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_OWN);
    txError = 0x80000000;
    logMsg("%s: tx own: stat=%x err=%x\n",(int)__FUNCTION__,CF_LE_L(stat),CF_LE_L(err),0,0,0);
}

void rmsgDbHook(unsigned i, unsigned data, unsigned id)
{
    unsigned mbox = data & 0x3;

    if(id >= RIO_ID_MAX || !rmsgTest[id][mbox].initDone)
    {
        logMsg("%s: error: id=%d mbox=%d tick=%d\n",(int)__FUNCTION__,id,mbox,boardTickGet(),0,0);
//        allRioTestsStop = 1;
        return;
    }

    sem_post(&rmsgTest[id][mbox].dbSem);
    rmsgTest[id][mbox].nRxDb++;
}

void rioRxDoneIntr(unsigned unused, unsigned mask)
{
    unsigned stat = rioRegs->endp.msgInStatus;

    if(stat & CT_LE_L(MSG_IN_DONE))
    {
        if(stat & CT_LE_L(MSG_IN_FAIL))
        {
            unsigned err = rioRegs->endp.msgInError;
            rioRegs->endp.msgInError = err;
            rxError = CF_LE_L(err);
            logMsg("%s: error=%x stat=%x\n",(int)__FUNCTION__,rxError,CF_LE_L(stat),0,0,0);
        }
//      if(!(stat & CT_LE_L(MSG_IN_RUN)))
//            rxStopped = TRUE;
        sem_post(&rxSem);
    }

    RIO_DEBUG(4,("%s: stat=%x\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));
    rioRegs->endp.msgInStatus = stat & ~CT_LE_L(MSG_IN_OWN);
    rioIntDisable(INTR_MSG_IN_DONE);
}

void rioRxPolIntr(unsigned unused, unsigned mask)
{
    unsigned stat = rioRegs->endp.msgInStatus;

    cpuCount[2] = boardCountGet();
    if(stat & CT_LE_L(MSG_IN_POL))
    {
        if(stat & CT_LE_L(MSG_IN_FAIL))
        {
            unsigned err = rioRegs->endp.msgInError;
            rioRegs->endp.msgInError = err;
            rxError = CF_LE_L(err);
            logMsg("%s: error=%x stat=%x\n",(int)__FUNCTION__,rxError,CF_LE_L(stat),0,0,0);
        }
        //      if(!(stat & CT_LE_L(MSG_IN_RUN)))
        //            rxStopped = TRUE;
        sem_post(&rxSem);
    }

    RIO_DEBUG(4,("%s: stat=%x\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));
    rioRegs->endp.msgInStatus = stat & ~CT_LE_L(MSG_IN_OWN);
    rioIntDisable(INTR_MSG_IN_POL);

    #warning test code
//    logMsg("rx delta=%d\n",(cpuCount[2]-cpuCount[0])*1000/24,0,0,0,0,0);
}

void rioRxOwnIntr(unsigned unused, unsigned mask)
{
    unsigned stat = rioRegs->endp.msgInStatus;

    if(stat & CT_LE_L(MSG_IN_OWN))
    {
        unsigned err = rioRegs->endp.msgInError;
        rioRegs->endp.msgInError = err;
        rxError = CF_LE_L(err);
        if(!(stat & CT_LE_L(MSG_IN_RUN)) && (stat & CT_LE_L(MSG_IN_CHAIN)))
            rxStopped = TRUE;
        if(rxError || (!rxOwnCount && !(stat & CT_LE_L(MSG_IN_CHAIN))))
            logMsg("%s: rx own: error=%x stat=%x\n",(int)__FUNCTION__,rxError,CF_LE_L(stat),0,0,0);
        rxOwnCount++;
    }

    RIO_DEBUG(1,("%s: stat=%x\n",(int)__FUNCTION__,CF_LE_L(stat),0,0,0,0));


    rioRegs->endp.msgInStatus = stat & ~CT_LE_L(MSG_IN_DONE|MSG_IN_FAIL);
    rxOwnFlag = TRUE;
    sem_post(&rxSem);
}

static inline int waitTxDone(unsigned id, unsigned tout)
{
    if(semTimedwait(&txSem,tout) == -1)
    {
        printf("%s: semTimedwait TX error=%s id=%d\n",__FUNCTION__,strerror(errno),id);
        return -1;
    }

    if(txError)
    {
        txError = 0;
        return -1;
    }

    return 0;
}

static inline int waitDbDone(unsigned id, unsigned tout, unsigned mbox)
{
    if(semTimedwait(&rmsgTest[id][mbox].dbSem,tout) == -1)
    {
        printf("%s: semTimedwait DB error=%s id=%d mbox=%d\n",__FUNCTION__,strerror(errno),id,mbox);
        return -1;
    }

    return 0;
}

static void txDescInit(RIO_TX_DESC* tx, unsigned bufSize, unsigned* buf, unsigned id, unsigned eol, unsigned mbox)
{
    unsigned bufAddr = pciLocalToBus(buf);
    unsigned descAddr = pciLocalToBus(tx) + sizeof(RIO_TX_DESC);
    unsigned val;

    if(bufSize == 0x1000)
    {
        val = (0xf << 4) | (0xe << 8) | (0xe << 12) | 0x80000000;
    }
    else
    {
        if(bufSize <= 256)
        {
            unsigned segSize = rioSegSize[bufSize-1];
            val = (segSize << 8) | (segSize << 12) | 0x80000000;
        }
        else
        {
            unsigned nSeg = bufSize >> 8;
            unsigned segSize = 0xe;

            bufSize &= 0xff;

            if(bufSize)
            {
                nSeg++;
                segSize = rioSegSize[bufSize-1];
            }

            val = ((nSeg-1) << 4) | (0x0e << 8) | (segSize << 12) | 0x80000000;
        }
    }

    val |= mbox << 2;

    tx->stat = CT_LE_L(val);
    tx->id = CT_LE_L(id);
    tx->buf1 = CT_LE_L(bufAddr);
    tx->buf2 = CT_LE_L(bufAddr+256);

    if(txChainMode)
    {
        tx->next = eol ? CT_LE_L(2) : 0;
    }
    else
    {
        tx->next = eol ? CT_LE_L(1) : CT_LE_L(descAddr);
    }
}

static void txBufInit(RIO_TX_DESC* tx, unsigned* buf, unsigned id, unsigned txMsgSize, unsigned txListSize, unsigned randomSize,
                      unsigned stat, unsigned opt, unsigned mbox, unsigned* pTxListSize)
{
    unsigned nWords, ix, i, v, eol = FALSE;

    if(!txListSize)
        txListSize = (random() % RIO_TX_MAX) + 1;

    for(i = 0; i < txListSize; i++, buf += 0x400)
    {
        if(randomSize)
        {
            nWords = (random() % randomSize) >> 2;

            if(!(nWords & 1))
                nWords++;

            if(nWords > 0x3ff)
                nWords = 0x3ff;

            if(alignMask)
            {
                if((nWords+1) & alignMask)
                    nWords = ((nWords+1) & ~alignMask) + alignMask;
            }
        }
        else
        {
            nWords = (txMsgSize >> 2) - 1;
        }

        v = nWords;

        if(opt & RMSG_OPT_NO_DATA)
        {
            for(ix = 0; ix < nWords; ix += 256/4)
                buf[ix+1] = rmsgTest[id][mbox].txCount++;
            v |= STAT_NO_DATA;
        }
        else
        {
            for(ix = 0; ix < nWords; ix++)
                buf[ix+1] = rmsgTest[id][mbox].txCount++;
        }

        if(i == 0 && (stat & STAT_FIRST))
            v |= STAT_FIRST;

        if(i == (txListSize - 1))
        {
            if(stat & STAT_LAST)
                v |= STAT_LAST;
            if(!(opt & RMSG_OPT_NO_ACK))
                v |= STAT_ACK;
            eol = TRUE;
        }

        buf[0] = v;

        if(rmsgShow)
            printf("size=%x bytes\n",(nWords+1)*4);

        txDescInit(&tx[i],(nWords+1)*4,buf,id,eol,mbox);
        rmsgTest[id][mbox].nTxBytes += (nWords+1)*4;
    }

    *pTxListSize = txListSize;
    rmsgTest[id][mbox].nTxMsg += txListSize;
}

static int txBufCheck(RIO_TX_DESC* tx)
{
    unsigned ix, err = 0;

    for(ix = 0; ix < RIO_TX_MAX; ix++)
    {
        if(tx[ix].stat & CT_LE_L(1))
        {
            printf("%s: error: stat=%lx id=%lx next=%lx\n",__FUNCTION__,
                CF_LE_L(tx[ix].stat),CF_LE_L(tx[ix].id),CF_LE_L(tx[ix].next));
            err = -1;
        }
        if(tx[ix].next == CT_LE_L(1))
            return err;
    }

    printf("%s: error: unterminated tx list id[0]=%lx\n",__FUNCTION__,
        CF_LE_L(tx[ix].id));
    return -1;
}

void rmsgTxChainThread(void)
{
    RIO_TX_DESC* d;
    unsigned stat, id, mbox;

    while(1)
    {
        sem_wait(&txSem);

        while(1)
        {
            if(txError)
            {
                unsigned ix;

                d = &txRing[txDirty];
                stat = d->stat;
                stat = CF_LE_L(stat);

                printf("%s: error recover: desc=%p dirty=%d stat=%08x\n",__FUNCTION__,d,txDirty,stat);

                for(ix = 0; ix < RMSG_TX_RING_SIZE; ix++)
                {
                    txRing[ix].stat = CT_LE_L(0x7fffffff);
                    txRing[ix].id = 0xffffffff;
                }
                rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_MODE);
                txDirty = 0;
                txCurrent = 0;
                txError = 0;
                break;
            }

            d = &txRing[txDirty];

            if((stat = d->stat) & CT_LE_L(0x80000000))
                break;

            if(stat == CT_LE_L(0x7fffffff))
                break;

            stat = CF_LE_L(stat);
            mbox = (stat >> 2) & 3;
            id = d->id; id = CF_LE_L(id);

            if(stat & 0x10000)
            {
                printf("%s: error: desc=%p dirty=%d stat=%08x id=%d mbox=%d next=%lx\n",__FUNCTION__,
                    d,txDirty,stat,id,mbox,CF_LE_L(d->next));
            }
//             printf("%s: desc=%p dirty=%d stat=%08x id=%d mbox=%d next=%lx\n",__FUNCTION__,
//                    d,txDirty,stat,id,mbox,CF_LE_L(d->next));

            if(d->next & CT_LE_L(2))
            {
                sem_post(&rmsgTest[id][mbox].txSem);
                d->next &= ~CT_LE_L(2);
                rmsgTest[id][mbox].nTxPOL++;
            }

            if(id >= RIO_ID_MAX)
            {
                printf("%s: init error: desc=%p dirty=%d stat=%08x id=%d mbox=%d next=%lx\n",__FUNCTION__,
                    d,txDirty,stat,id,mbox,CF_LE_L(d->next));
                goto next;
            }

next:
            d->stat = CT_LE_L(0x7fffffff);
            txDirty = (txDirty + 1) % RMSG_TX_RING_SIZE;
        }
    }
}

static inline volatile unsigned descFlush(volatile unsigned* p)
{
    return *p;
}

static inline int txListInsert(RIO_TX_DESC* tx, unsigned txListSize)
{
    RIO_TX_DESC* d;
    int res = 0, i, j;

    pthread_mutex_lock(&rmsgTxMutex);
    for(i = 0; i < txListSize; i++)
    {
        j = txListSize - i - 1;
        d = &txRing[(txCurrent + j) % RMSG_TX_RING_SIZE];

        if(d->stat != CT_LE_L(0x7fffffff))
        {
            printf("%s: error: desc=%p dirty=%d current=%d i=%d stat=%08x\n",__FUNCTION__,
                   d,txDirty,txCurrent,i,(unsigned)(CF_LE_L(d->stat)));
            res = -1;
            goto done;
        }

        if(tx[j].next & CT_LE_L(2))
            d->next |= CT_LE_L(2);

        d->id = tx[j].id;
        d->buf1 = tx[j].buf1;
        d->buf2 = tx[j].buf2;
        d->stat = tx[j].stat;
    }

    descFlush(&d->stat);
    txCurrent = (txCurrent + txListSize) % RMSG_TX_RING_SIZE;
    if(!cpuCount[0])
        cpuCount[0] = boardCountGet();
    else
        cpuCount[3] = boardCountGet();
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_MODE);

done:
    pthread_mutex_unlock(&rmsgTxMutex);
    return res;
}

static inline void txStart(void* tx)
{
    unsigned descAddr = pciLocalToBus(tx);
    boardWbFlush();
    if(rioRegs->endp.msgOutStatus & CT_LE_L(MSG_OUT_RUN))
        printf("%s: BUSY!!!\n",__FUNCTION__);
    rioRegs->endp.msgOut[0] = CT_LE_L(descAddr);
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN);
}

typedef struct {
    unsigned id;
    unsigned nTx;
    unsigned txMsgSize;
    unsigned randomSize;
    unsigned txListSize;
    unsigned opt;
    unsigned mbox;
} RMSG_RX_PARAM;

void rmsgTxThread(RMSG_RX_PARAM* param)
{
    unsigned ix, i, stat, semVal, tout = boardClkRateGet()*4, id, nTx, txMsgSize, randomSize, txListSize, opt, mbox, listSize;
    RIO_TX_DESC* tx[2];
    unsigned* buf[2];
    struct timespec ts1, ts2;
    unsigned long long ms1, ms2, lastTxBytes;

    nTx = param->nTx;
    id = param->id;
    txMsgSize = param->txMsgSize;
    randomSize = param->randomSize;
    txListSize = param->txListSize;
    opt = param->opt;
    mbox = param->mbox;
    free(param);

    if(!nTx)
    {
        printf("\nUsage: %s id,nTx,txMsgSize,randomSize,txListSize,opt\n",__FUNCTION__);
        return;
    }
    if((txMsgSize & 7) || txMsgSize > 0x1000)
    {
        printf("invalid txMsgSize=%x\n",txMsgSize);
        return;
    }

    if(id >= RIO_ID_MAX)
    {
        printf("invalid id=%d\n",id);
        return;
    }

    if(mbox >= 4)
    {
        printf("invalid mbox=%d\n",mbox);
        return;
    }

    if(txListSize > RIO_TX_MAX)
    {
        printf("invalid txListSize=%x\n",txListSize);
        return;
    }

    if(txChainMode)
    {
        tx[0] = memAlloc(NULL,RIO_TX_MAX*8*sizeof(RIO_TX_DESC),64);
    }
    else
    {
        tx[0] = _cacheMalloc(RIO_TX_MAX*8*sizeof(RIO_TX_DESC),64);
    }
//#warning test code
//    tx[0] = (void*)0xa2000000;
    tx[1] = tx[0] + RIO_TX_MAX;

    buf[0] = _cacheMalloc(RIO_TX_MAX*0x1000*8,0x1000);
//#warning test code
//    buf[0] = (void*)0xa2800000;
    buf[1] = buf[0] + RIO_TX_MAX*0x1000/4;

    if(!tx[0] || !buf[0])
    {
        printf("_cacheMalloc failed\n");
        return;
    }

    printf("tx: desc=%p(%p) mem=%p(%p)\n",tx[0],tx[1],buf[0],buf[1]);

    memset(tx[0],0,RIO_TX_MAX*2*sizeof(RIO_TX_DESC));
    memset(buf[0],0x22,RIO_TX_MAX*0x1000*2);

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

    rmsgTest[id][mbox].txCount = 0;
    rmsgTest[id][mbox].nTxMsg = 0;
    rmsgTest[id][mbox].nRxDb = 0;
    rmsgTest[id][mbox].nTxBytes = 0;

    sem_init(&rmsgTest[id][mbox].dbSem,0,2);
    sem_init(&rmsgTest[id][mbox].txSem,0,2);

    if(opt & RMSG_OPT_TX_RESTART)
    {
        if(sem_getvalue(&txSem,&semVal) == 0)
            sem_post(&txSem);
    }

    rmsgTest[id][mbox].initDone = TRUE;

    clock_gettime(CLOCK_REALTIME,&ts1);
    ms1 = ts1.tv_sec*1000ULL + ts1.tv_nsec/1000000ULL;
    lastTxBytes = 0;
    cpuCount[0] = 0;
    cpuCount[3] = 0;
    for(i = 0, stat = STAT_FIRST; i < nTx; i++, stat = 0)
    {
        if(!(opt & RMSG_OPT_NO_ACK))
            if(waitDbDone(id,tout,mbox) == -1)
                goto done;

        if(i == nTx - 1)
            stat |= STAT_LAST;

        if(i > 1 && !txChainMode)
            txBufCheck(tx[i & 1]);

        if(txChainMode)
        {
            if(semTimedwait(&rmsgTest[id][mbox].txSem,tout) == -1)
            {
                unsigned v = rioRegs->endp.msgOutStatus, v2 = rioRegs->endp.intrEnable;
                printf("%s: semTimedwait error=%s id=%d mbox=%d\n     txDirty=%d outStat=%lx intr=%lx nTxPOL=%d\n",__FUNCTION__,
                        strerror(errno),id,mbox,txDirty,CF_LE_L(v),CF_LE_L(v2),nTxPOL);
                for(ix = 0; ix < 32; ix++)
                {
                    RIO_TX_DESC* d = &txRing[(txDirty + ix) % RMSG_TX_RING_SIZE];
                    printf("d=%p stat=%lx next=%lx\n",d,CF_LE_L(d->stat),CF_LE_L(d->next));
                }
                goto done;
            }

            txBufInit(tx[i & 1],buf[i & 1],id,txMsgSize,txListSize,randomSize,stat,opt,mbox,&listSize);

            if(txListInsert(tx[i & 1],listSize) == -1)
                goto done;

            sched_yield();
        }
        else
        {
            txBufInit(tx[i & 1],buf[i & 1],id,txMsgSize,txListSize,randomSize,stat,opt,mbox,&listSize);

            sched_yield();

            if(waitTxDone(id,tout) == -1)
                goto done;

            txStart(tx[i & 1]);
        }

        if(rmsgPrintPeriod)
        {
            clock_gettime(CLOCK_REALTIME,&ts2);
            ms2 = ts2.tv_sec*1000ULL + ts2.tv_nsec/1000000ULL;
            if(ms2 - ms1 > rmsgPrintPeriod*1000)
            {
                printf("%s: id=%d mbox=%d nTxMsg=%d count=%d nRxDb=%d speed=%dKB/s\n",
                    __FUNCTION__,id,mbox,rmsgTest[id][mbox].nTxMsg,rmsgTest[id][mbox].txCount,
                    rmsgTest[id][mbox].nRxDb,(unsigned)((1000ULL*
                    (rmsgTest[id][mbox].nTxBytes - lastTxBytes))/(ms2-ms1))/1024);
                ms1 = ms2;
                lastTxBytes = rmsgTest[id][mbox].nTxBytes;
            }
        }
    }

    clock_gettime(CLOCK_REALTIME,&ts2);

done:
    sleep(1);
    sem_getvalue(&rmsgTest[id][mbox].dbSem,&semVal);
    rmsgTest[id][mbox].initDone = FALSE;
    sem_destroy(&rmsgTest[id][mbox].dbSem);
    sem_destroy(&rmsgTest[id][mbox].txSem);
    printf("%s: id=%d mbox=%d nTxMsg=%d count=%d nRxDb=%d semVal=%d nTxPOL=%d\n",__FUNCTION__,
           id,mbox,rmsgTest[id][mbox].nTxMsg,rmsgTest[id][mbox].txCount,rmsgTest[id][mbox].nRxDb,
           semVal,rmsgTest[id][mbox].nTxPOL++);
    ms1 = ts1.tv_sec*1000ULL + ts1.tv_nsec/1000000ULL;
    ms2 = ts2.tv_sec*1000ULL + ts2.tv_nsec/1000000ULL;
    if(ms2-ms1)
        printf("%s: speed=%dKB/s\n",__FUNCTION__,(unsigned)((1000ULL*
            rmsgTest[id][mbox].nTxBytes)/(ms2-ms1))/1024);
//#warning test code
    cacheFree(tx[0]);
    cacheFree(buf[0]);
//    printf("free: desc=%p mem=%p\n",tx[0],buf[0]);
}

void rTx(unsigned id, unsigned nTx, unsigned txMsgSize, unsigned randomSize, unsigned txListSize, unsigned opt, unsigned mbox)
{
    struct sched_param schedparam;
    pthread_attr_t attr;
    RMSG_RX_PARAM* param = malloc(sizeof(RMSG_RX_PARAM));
    unsigned res;

    param->nTx = nTx;
    param->id = id;
    param->txMsgSize = txMsgSize;
    param->randomSize = randomSize;
    param->txListSize = txListSize;
    param->opt = opt;
    param->mbox = mbox;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
    schedparam.sched_priority = 175;
    pthread_attr_setschedparam(&attr,&schedparam);

    if((res = pthread_create(NULL,&attr,(void*)&rmsgTxThread,param)))
    {
        printf("%s: pthread_create error=%s (%d)\n",__FUNCTION__,strerror(res),res);
        return;
    }
}

void _rTx(unsigned idMask, unsigned nTx, unsigned txMsgSize, unsigned randomSize, unsigned txListSize, unsigned opt, unsigned mboxMask, unsigned excludeSelf)
{
    unsigned id, mbox;

    if(!mboxMask)
        mboxMask = 1;

    for(id = 0; id < RIO_ID_MAX; id++)
    {
        if(excludeSelf && id == boardProcNumGet())
            continue;
        if(idMask & (1 << id))
        {
            for(mbox = 0; mbox < 4; mbox++)
            {
                if(mboxMask & (1 << mbox))
                    rTx(id,nTx,txMsgSize,randomSize,txListSize,opt,mbox);
            }
        }
    }
}

static inline void rmsgBufFreePut(RMSG_BUF_ENTRY* e)
{
    int notify = FALSE;
    pthread_mutex_lock(&rmsgMutex);
    if(!rmsgBufFree)
        notify = TRUE;
    e->next = rmsgBufFree;
    rmsgBufFree = e;
    pthread_mutex_unlock(&rmsgMutex);
    if(notify)
        pthread_cond_signal(&rmsgCond);
}

static inline RMSG_BUF_ENTRY* rmsgBufFreeGet()
{
    RMSG_BUF_ENTRY* e;
    pthread_mutex_lock(&rmsgMutex);
    while(!rmsgBufFree)
        pthread_cond_wait(&rmsgCond,&rmsgMutex);
    e = rmsgBufFree;
    rmsgBufFree = e->next;
    e->next = NULL;
    pthread_mutex_unlock(&rmsgMutex);
    return e;
}

void rmsgRxThread(RIO_RX_DESC* rx)
{
    unsigned ix = 0, stat; RMSG_BUF_ENTRY* rb;

    while(1)
    {
        sem_wait(&rxSem);

//      if(rxError)
//          break;

        while(1)
        {
fetch:
            stat = rx[ix].stat;

            if(stat & CT_LE_L(0x80000000))
            {
                unsigned ix1 = (ix + 1) & (RIO_RX_MAX - 1), count;

                if(!(rx[ix1].stat & CT_LE_L(0x80000000)))
                {
                    for(count = 0; count < 10000; count++)
                    {
                        if(!(rx[ix].stat & CT_LE_L(0x80000000)))
                            goto fetch;
                        if(count > 800)
                            udelay(1);
                    }
                    printf("%s: skip desc=%p ix=%d\n",__FUNCTION__,&rx[ix],ix);
#warning test code
                    return;
                    ix = ix1;
                    goto fetch;
                }
                if(rxStopped)
                {
//                     ix = 0;
//                    rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN|MSG_IN_CHAIN);
                    rxStopped = FALSE;
                }
//                 rioIntEnable(INTR_MSG_IN_DONE);
                rioIntEnable(INTR_MSG_IN_POL);
                break;
            }

            rb = rmsgBufBusy[ix];

#warning test code
            if(rx[ix].buf1 != CT_LE_L(rmsgBufBusy[ix]->pciAddr) ||
                rx[ix].buf2 != CT_LE_L(rmsgBufBusy[ix]->pciAddr+256))
            {
                printf("!!!!!!!!!!!!!!!!!!!!! 22222222222222222\n");
            }

            rb->stat = stat;
            rb->id = rx[ix].id;

            if(mq_send(rmsgMQ,(char*)&rb,sizeof(void*),0) == -1)
                printf("%s: mq_send failed, errno=%s (%d)\n",__FUNCTION__,
                    strerror(errno),errno);

            if(!(rmsgBufBusy[ix] = rmsgBufFreeGet()))
            {
                printf("%s: out of free buffers\n",__FUNCTION__);
                goto done;
            }

            rx[ix].id = 0;
            rx[ix].buf1 = CT_LE_L(rmsgBufBusy[ix]->pciAddr);
            rx[ix].buf2 = CT_LE_L(rmsgBufBusy[ix]->pciAddr+256);
            rx[ix].stat = CT_LE_L(0x80000000);
            descFlush(&rx[ix].stat);

            rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_OWN|MSG_IN_DONE|MSG_IN_RUN|MSG_IN_CHAIN);

            if(rxOwnFlag)
            {
//              unsigned addr = pciLocalToBus(&rx[ix]);
//                unsigned stat = MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN;
//              rioRegs->endp.msgIn[0] = CT_LE_L(addr);
//              rioRegs->endp.msgInStatus = CT_LE_L(stat);
                rxOwnFlag = FALSE;
            }

#warning test code
            if(rx[ix].buf1 != CT_LE_L(rmsgBufBusy[ix]->pciAddr) ||
                rx[ix].buf2 != CT_LE_L(rmsgBufBusy[ix]->pciAddr+256))
            {
                printf("!!!!!!!!!!!!!!!!!!!!!\n");
            }
            rxLastDesc = &rx[ix];
            ix = (ix + 1) & (RIO_RX_MAX - 1);

            if(rxStopped)
            {
//                 ix = 0;
//                 rioRegs->endp.msgInStatus = CT_LE_L(MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN|MSG_IN_CHAIN);
                rxStopped = FALSE;
            }
        }
    }

done:

    rioIntDisable(INTR_MSG_OUT_EOL|INTR_MSG_OUT_ERR|INTR_MSG_OUT_OWN|INTR_MSG_IN_DONE|INTR_MSG_IN_OWN);
    sem_destroy(&rxSem);
    sem_destroy(&txSem);
}

void rmsgRxParseThread(void* unused)
{
    unsigned prio, id, nWords, w0, val, nErr, i, stat, nSeg, segSize, lastSegSize, msgSize, mbox;
    RMSG_BUF_ENTRY* rb;
    unsigned* p;
    unsigned* pu;

    while(1)
    {
        if(mq_receive(rmsgMQ,(char*)&rb,sizeof(void*),&prio) == -1)
        {
            printf("%s: mq_receive failed, errno=%s (%d)\n",__FUNCTION__,
                strerror(errno),errno);
            return;
        }

        p = rb->addr;
        pu = (void*)K0_TO_K1(p);
        id = rb->id; id = CF_LE_L(id);

        stat = rb->stat; stat = CF_LE_L(stat);
        nSeg = (stat >> 4) & 0xf;
        segSize = 8 << (((stat >> 8) & 0xf) - 9);
        lastSegSize = 8 << (((stat >> 12) & 0xf) - 9);
        msgSize = segSize*nSeg + lastSegSize;
        mbox = (stat >> 2) & 3;

        w0 = pu[0];

        if(id >= RIO_ID_MAX)
        {
            printf("%s: invalid id=%d\n",__FUNCTION__,id);
            goto next;
        }

        if(w0 & STAT_FIRST)
        {
            printf("%s: get start message from id=%d mbox=%d\n",__FUNCTION__,id,mbox);
            rmsgTest[id][mbox].rxCount = 0;
            rmsgTest[id][mbox].nRxMsg = 0;
            rmsgTest[id][mbox].nTxDb = 0;
        }

        nWords = w0 & ~STAT_MASK;

        if(nWords > 0x3ff || !nWords)
        {
            printf("%s: id=%d mbox=%d invalid nWords=%d stat=%x p=%p\n",__FUNCTION__,id,mbox,nWords,w0,pu);
                goto next;
        }

        rmsgTest[id][mbox].nRxMsg++;

        nErr = 0;

        if(w0 & STAT_NO_DATA)
        {
            for(i = 0; i < nWords; i += 256/4)
            {
                val = pu[i+1];

                if(val != rmsgTest[id][mbox].rxCount)
                {
                    printf("%s: nRxMsg=%d i=0x%x ptr=%p read=%08x expected=%08x\n",__FUNCTION__,
                        rmsgTest[id][mbox].nRxMsg,i,&pu[i+1],val,rmsgTest[id][mbox].rxCount);
                    nErr++;
                }
                rmsgTest[id][mbox].rxCount++;
            }

            if(nErr)
            {
                printf("id=%d mbox=%d stat=%08x nSeg=%d segSize=%x lastSegSize=%x msgSize=%x\n",
                    id,mbox,stat,nSeg,segSize,lastSegSize,msgSize);
                rxError = -1;
                return;
            }
        }
        else
        {
            cacheDataInvalidate(p,msgSize);

            for(i = 0; i < nWords; i++)
            {
                val = p[i+1];

                if(val != rmsgTest[id][mbox].rxCount)
                {
                    printf("%s: nRxMsg=%d i=0x%x ptr=%p read=%08x expected=%08x\n",__FUNCTION__,
                        rmsgTest[id][mbox].nRxMsg,i,&p[i+1],val,rmsgTest[id][mbox].rxCount);
                    nErr++;
                }

                if(nErr > 16)
                {
                    printf("id=%d mbox=%d stat=%08x nSeg=%d segSize=%x lastSegSize=%x msgSize=%x\n",
                        id,mbox,stat,nSeg,segSize,lastSegSize,msgSize);
                    rxError = -1;
                    return;
                }
                rmsgTest[id][mbox].rxCount++;
            }
        }

        for(i = 0; i < 0x1000/4; i += 0x100/4)
            pu[i] = 0x66666666;

        if(w0 & STAT_ACK)
        {
            _rioDbOut(id,RMSG_DB,mbox);
            rmsgTest[id][mbox].nTxDb++;
        }

        if(w0 & STAT_LAST)
        {
            printf("%s: get stop message from id=%d mbox=%d: nRxMsg=%d count=%d nTxDb=%d\n",__FUNCTION__,
                id,mbox,rmsgTest[id][mbox].nRxMsg,rmsgTest[id][mbox].rxCount,rmsgTest[id][mbox].nTxDb);
        }

next:
//      memset(p,0xee,msgSize);
//      cacheDataClear(p,0x1000);
        rmsgBufFreePut(rb);
        sched_yield();
    }
}

void rRx(int retry, int txChain)
{
    unsigned ix, res, stat;
    unsigned* buf;
    unsigned bufAddr, descAddr;
    RIO_RX_DESC* rx;
    struct sched_param schedparam;
    pthread_attr_t attr;

    struct mq_attr mqattr = {
        mq_maxmsg       : RMSG_BUF_MAX,
        mq_msgsize      : sizeof(void*),
        mq_flags        : 0,
        mq_curmsgs      : 0
    };

    if((rmsgMQ = mq_open("rmsgMQ",O_RDWR|O_CREAT|O_EXCL,0,&mqattr)) == (mqd_t)(-1))
    {
        printf("%s: mq_open error=%s (%d)\n",__FUNCTION__,strerror(errno),errno);
        return;
    }

    if((res = pthread_mutex_init(&rmsgMutex,NULL)))
    {
        printf("%s: pthread_mutex_init error=%s (%d)\n",__FUNCTION__,strerror(res),res);
        return;
    }

    if((res = pthread_cond_init(&rmsgCond,NULL)))
    {
        printf("%s: pthread_cond_init error=%s (%d)\n",__FUNCTION__,strerror(res),res);
        return;
    }

    rioRegs->endp.msgOut[2] = CT_LE_L(0xc);
    rioRegs->endp.msgOutError = rioRegs->endp.msgOutError;
    rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_FAIL|MSG_OUT_EOL|MSG_OUT_OWN|MSG_OUT_COMPLETE|MSG_OUT_POL);
    rioIntConnect(IRQ_MSG_OUT_ERR,rioTxErrIntr,0);
    if(txChain)
    {
        if((res = pthread_mutex_init(&rmsgTxMutex,NULL)))
        {
            printf("%s: pthread_mutex_init error=%s (%d)\n",__FUNCTION__,strerror(res),res);
            return;
        }
        rioIntConnect(IRQ_MSG_OUT_POL,rioTxPOLIntr,0);
        rioIntEnable(INTR_MSG_OUT_ERR|INTR_MSG_OUT_POL);
        txChainMode = TRUE;
        sem_init(&txSem,0,0);
    }
    else
    {
        rioIntConnect(IRQ_MSG_OUT_EOL,rioTxEOLIntr,0);
        rioIntConnect(IRQ_MSG_OUT_OWN,rioTxOwnIntr,0);
        rioIntEnable(INTR_MSG_OUT_EOL|INTR_MSG_OUT_ERR|INTR_MSG_OUT_OWN);
        sem_init(&txSem,0,1);
    }
    rxError = 0;

    sem_init(&rxSem,0,0);

    rioDbIntConnect(RMSG_DB,rmsgDbHook,0);

    rx = _cacheMalloc(RIO_RX_MAX*sizeof(RIO_RX_DESC),0x100);

    if(rxAlignSize >= 0x1000)
        buf = memAlloc(NULL,RMSG_BUF_MAX*0x1000,rxAlignSize);
    else
        buf = memAlloc(NULL,RMSG_BUF_MAX*0x1000+0x1000,0x1000) + rxAlignSize;


//#warning test code
//rx = (void*)0xa3000000;
//buf = (void*)0xa3800000;

    if(!rx || !buf)
    {
        printf("%s: malloc error\n",__FUNCTION__);
        return;
    }

    printf("rx: desc=%p mem=%p\n",rx,buf);

    memset(rx,0,RIO_RX_MAX*sizeof(RIO_RX_DESC));
    memset(buf,0xee,RMSG_BUF_MAX*0x1000);
    cacheDataClear(buf,RMSG_BUF_MAX*0x1000);

    bufAddr = pciLocalToBus(buf);

    for(ix = 0; ix < RMSG_BUF_MAX; ix++)
    {
        rmsgBufArray[ix].next = NULL;
        rmsgBufArray[ix].addr = (char*)buf + 0x1000*ix;
        rmsgBufArray[ix].pciAddr = bufAddr + 0x1000*ix;
    }

    for(ix = RIO_RX_MAX; ix < RMSG_BUF_MAX; ix++)
        rmsgBufFreePut(&rmsgBufArray[ix]);

    descAddr = pciLocalToBus(rx) + sizeof(RIO_RX_DESC);

    for(ix = 0; ix < RIO_RX_MAX; ix++)
    {
        rmsgBufBusy[ix] = &rmsgBufArray[ix];
        rx[ix].stat = CT_LE_L(0x80000000);
        rx[ix].id = 0;
        rx[ix].buf1 = CT_LE_L(rmsgBufBusy[ix]->pciAddr);
        rx[ix].buf2 = CT_LE_L(rmsgBufBusy[ix]->pciAddr+256);
        rx[ix].next = CT_LE_L(descAddr|2);
        descAddr += sizeof(RIO_RX_DESC);
    }

    descAddr = pciLocalToBus(rx);
    rx[ix-1].next = CT_LE_L(descAddr|2);

    rioIntConnect(IRQ_MSG_IN_DONE,rioRxDoneIntr,0);
    rioIntConnect(IRQ_MSG_IN_POL,rioRxPolIntr,0);
    rioIntConnect(IRQ_MSG_IN_OWN,rioRxOwnIntr,0);

    rioRegs->endp.msgIn[0] = CT_LE_L(descAddr);
    if(retry)
    {
        rioRegs->endp.msgIn[1] = CT_LE_L(0x30000);      // [16] - retry enable, [17] - error response on timeout;
        rioRegs->err.rw[1] |= CT_LE_L(0x400);           // [10] - msgInnOwnEna
        rioRegs->endp.msgOut[1] = CT_LE_L(0xff00);      // [15:8] - retry count bits [15:8]
        rioRegs->err.rw[3] |= CT_LE_L(0x400000);        // [22] - MsgIn9LetterStrong
//         rioIntEnable(INTR_MSG_IN_DONE);
        rioIntEnable(INTR_MSG_IN_POL);
    }
    else
    {
        rioIntEnable(INTR_MSG_IN_DONE|INTR_MSG_IN_OWN);
    }

    rioRegs->endp.msgInError = rioRegs->endp.msgInError;

    stat = MSG_IN_FAIL|MSG_IN_OWN|MSG_IN_POL|MSG_IN_EOL|MSG_IN_COMPLETE|MSG_IN_DONE|MSG_IN_RUN;
    if(retry)
        stat |= MSG_IN_CHAIN;
    rioRegs->endp.msgInStatus = CT_LE_L(stat);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
    schedparam.sched_priority = 190;
    pthread_attr_setschedparam(&attr,&schedparam);

    if((res = pthread_create(NULL,&attr,(void*)&rmsgRxThread,rx)))
    {
        printf("%s: pthread_create error=%s (%d)\n",__FUNCTION__,strerror(res),res);
        return;
    }

    schedparam.sched_priority = 175;
    pthread_attr_setschedparam(&attr,&schedparam);

    if((res = pthread_create(&rmsgRxParseThreadDesc,&attr,(void*)&rmsgRxParseThread,NULL)))
    {
        printf("%s: pthread_create error=%s (%d)\n",__FUNCTION__,strerror(res),res);
        return;
    }

    if(txChain)
    {
        txRing = _cacheMalloc(RMSG_TX_RING_SIZE*sizeof(RIO_TX_DESC),64);

        if(!txRing)
        {
            printf("_cacheMalloc %x bytes error\n",RMSG_TX_RING_SIZE*sizeof(RIO_TX_DESC));
            return;
        }

        memset(txRing,0,RMSG_TX_RING_SIZE*sizeof(RIO_TX_DESC));
        printf("txRing=%p\n",txRing);

        descAddr = pciLocalToBus(txRing) + sizeof(RIO_TX_DESC);
        for(ix = 0; ix < RMSG_TX_RING_SIZE; ix++)
        {
            txRing[ix].stat = CT_LE_L(0x7fffffff);
            txRing[ix].id = 0xffffffff;
            txRing[ix].buf1 = 0;
            txRing[ix].buf2 = 0;
            txRing[ix].next = CT_LE_L(descAddr);
            descAddr += sizeof(RIO_TX_DESC);
        }

        descAddr = pciLocalToBus(txRing);
        txRing[ix-1].next = CT_LE_L(descAddr);

        schedparam.sched_priority = 180;
        pthread_attr_setschedparam(&attr,&schedparam);

        if((res = pthread_create(NULL,&attr,(void*)&rmsgTxChainThread,NULL)))
        {
            printf("%s: pthread_create error=%s (%d)\n",__FUNCTION__,strerror(res),res);
            return;
        }

        rioRegs->endp.msgOut[0] = CT_LE_L(descAddr);
        rioRegs->endp.msgOutStatus = CT_LE_L(MSG_OUT_RUN|MSG_OUT_MODE);
    }
}

void rk128(void)
{
    rioRegs->csr.baseDevID = CT_LE_L(2|(2<<16));      // bridge ID on RIO
    boardProcNumSet(2);

    rioMaintOut(0,PKT_TTL,0,NIISI_PKT_TTL_VAL);

    rioMaintOut(0,CSR_ROUTE_CFG_DESTID,0,1);    // ID=1
    rioMaintOut(0,CSR_ROUTE_CFG_PORT,0,2);      // PORT=2

    rioMaintOut(0,CSR_ROUTE_CFG_DESTID,0,2);    // ID=2
    rioMaintOut(0,CSR_ROUTE_CFG_PORT,0,4);      // PORT=4

    rioMaintOut(0,PORT_CTL+2*0x20,0,0x44000000);
    rioMaintOut(0,PORT_CTL+4*0x20,0,0x44000000);
}


static unsigned* vv[16];

int _mstr(unsigned* base, unsigned size, unsigned val)
{
    unsigned ix, i; val = ~val;
    for(ix = 0, i = 0; ix < size/4 && i < 15; ix++)
    {
        if(base[ix] == val)
            vv[i++] = base+ix;
    }
    return i;
}

void mstr(unsigned* base, unsigned size, unsigned val)
{
    unsigned ix, i;
    i = _mstr(base,size,val);
    
    for(ix = 0; ix < i; ix++)
        printf("ptr=%p\n",vv[ix]);
}

/******************************************/
void tnslp(int sec, long int nsec)
{ struct timespec t1;
t1.tv_sec = sec; t1.tv_nsec = nsec;
nanosleep (&t1, NULL);
}
/******************************************/
void mrioSInit(int mode4x)
{
    unsigned val = mode4x ? CT_LE_L(0x00600000) : CT_LE_L(0x10600000);
    
    rioRegs->ext.port[1].ctl = val;                     // enable in/out serial ports
    rioRegs->ext.port[1].errStat = 0xffffffff;          // clear port status
    
    if(boardProcNumGet() == 1)
    {
        rioRegs->csr.routeDstID = CT_LE_L(2);
        rioRegs->csr.routePort = CT_LE_L(1);
    }
    else
    {
        rioRegs->csr.routeDstID = CT_LE_L(1);
        rioRegs->csr.routePort = CT_LE_L(1);
    }
}

/******************************************************************************/

/* 'manual' packet I/O */

static sem_t mRxSem;
static sem_t mTxSem;
static pthread_mutex_t mTxMutex;
static VOIDFUNCPTR mHookRtn;
static int mHookArg;

/******************************************************************************/

/* manual rx done interrupt */

static void rioMRxIntr(unsigned unused)
{
    sem_post(&mRxSem);
    rioIntDisable(INTR_IN_DONE);
}

/******************************************************************************/

/* manual tx done interrupt */

static void rioMTxIntr(unsigned unused)
{
    sem_post(&mTxSem);
    rioIntDisable(INTR_MISC_DONE);
}

/******************************************************************************/

/* manual tx done interrupt */

void rioMHookSet(VOIDFUNCPTR rtn, int arg)
{
    int mask = intLock();
    mHookRtn = rtn;
    mHookArg = arg;
    intUnlock(mask);
}

/******************************************************************************/

/* manual rx thread */

void rioMThread(void)
{
    unsigned ix, in4, in2, nQW, val1 = 0, val2 = 0, i;
    unsigned long long v64, hdr = 0, r1, r2, buf[32];
    
    rioRegs->user.in4 = CT_LE_L(0x101);
    
    while(1)
    {
        /* waiting for interrupt */
        
        sem_wait(&mRxSem);
        in4 = rioRegs->user.in4;
        again:
        in2 = rioRegs->user.in2;
        nQW = CF_LE_L(in2) & 0x3f;
        
        /* fetch first 11 qwords from FIFO: 0 - header, 1 ... 9 - 64-bit data, 10 - CRC with 48-bit data */
        
        for(ix = 0, i = 0; ix < 11 && ix < nQW; ix++)
        {
            val1 = rioRegs->user.in1; val1 = CF_LE_L(val1);
            val2 = rioRegs->user.in1; val2 = CF_LE_L(val2);
            
            /* first qword is header */
            
            if(!ix)
                hdr = ((unsigned long long)val2 << 32ULL) | (unsigned long long)val1;
            
            /* check first 9 data qwords */
            
            if(ix && ix < 10 && ix != nQW - 1)
            {
                v64 = ((unsigned long long)val2 << 32ULL) | (unsigned long long)val1;
                buf[i++] = v64;
            }
        }
        
        /* after 9 data qword 16-bit CRC split data qwords */
        
        for(r1 = val1, r2 = val2; ix < nQW; ix++)
        {
            val1 = rioRegs->user.in1; val1 = CF_LE_L(val1);
            val2 = rioRegs->user.in1; val2 = CF_LE_L(val2);
            
            v64 = (r2 << 48ULL) | (r1 << 16ULL) | (val2 >> 16);
            r1 = val1;
            r2 = val2;
            val1 = v64 >> 32ULL;
            val2 = v64;
            
            if(i < 32)
                buf[i++] = v64;
        }
        
        rioRegs->user.in4 = in4;
        sched_yield();
        
        if(mHookRtn)
            mHookRtn(mHookArg,hdr,buf,i);
        
        /* short poll for new packet */
        
        for(ix = 0; ix < 10; ix++)
        {
            in4 = rioRegs->user.in4;
            
            /* check new packet in FIFO */
            
            if(in4 & CF_LE_L(1))
                goto again;
            
            if(ix > 5)
                udelay(1);
        }
        
        /* reenable interrupts */
        
        rioIntEnable(INTR_IN_DONE);
    }
}

/******************************************************************************/

/* manual packet tx routine (Ftype 6, SWRITE) */

int rioMSend(unsigned dstID, unsigned long long* buf, unsigned nQW, unsigned addr)
{
    unsigned ix, val; unsigned long long v64, resid = 0; int res = 0;
    
    if(nQW > 256/8)
        return EINVAL;
    
    pthread_mutex_lock(&mTxMutex);
    rioRegs->endp.miscIx = 0;
    rioRegs->endp.miscStatus = rioRegs->endp.miscStatus;
    
    val = 0x04060000 | (dstID << 8) | boardProcNumGet();
    
    rioRegs->endp.miscData[0] = CT_LE_L(val);   // header
    rioRegs->endp.miscData[1] = CT_LE_L(addr);  // address
    
    for(ix = 0; ix < 9 && ix < nQW; ix++)
    {
        v64 = buf[ix];
        val = (v64 >> 32ULL);
        rioRegs->endp.miscData[0] = CT_LE_L(val);
        val = v64 & 0xffffffff;
        rioRegs->endp.miscData[1] = CT_LE_L(val);
    }
    
    /* leave space for 16-bit CRC after 10 qword */
    
    for(; ix < nQW; ix++)
    {
        v64 = buf[ix];
        val = (resid << 16) | (v64 >> 48);
        rioRegs->endp.miscData[0] = CT_LE_L(val);
        val = (v64 >> 16) & 0xffffffff;
        rioRegs->endp.miscData[1] = CT_LE_L(val);
        resid = v64 & 0xffff;
    }
    
    val = resid << 16;
    rioRegs->endp.miscData[0] = CF_LE_L(val);
    
    rioRegs->endp.miscStatus = CT_LE_L(1);
    sched_yield();
    
    /* short poll status */
    
    for(ix = 0; ix < 10; ix++)
    {
        if(rioRegs->endp.miscStatus & CT_LE_L(0x100))
        {
            rioRegs->endp.miscStatus = CT_LE_L(0x100);
            goto done;
        }
        if(ix > 5)
            udelay(1);
    }
    
    /* waiting for interrupt */
    
    rioIntEnable(INTR_MISC_DONE);
    if(semTimedwait(&mTxSem,boardClkRateGet()*5) == -1)
        res = errno;
    rioRegs->endp.miscStatus = CT_LE_L(0x100);
    done:
    pthread_mutex_unlock(&mTxMutex);
    return res;
}

/******************************************************************************/

/* manual I/O init routine */

int rioMInit(unsigned prio)
{
    static int init = FALSE;
    pthread_attr_t attr;
    struct sched_param param;
    int res;
    
    if(init)
        return EBUSY;
    
    rioRegs->spec.enable = 0;   // disable regs and memory access from RIO
    
    sem_init(&mRxSem,0,0);
    sem_init(&mTxSem,0,0);
    pthread_mutex_init(&mTxMutex,NULL);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_attr_getschedparam (&attr,&param);
    param.sched_priority = prio;
    pthread_attr_setschedparam(&attr,&param);
    
    if((res = pthread_create(NULL,&attr,(void*)rioMThread,NULL)))
        return res;
    
    rioIntConnect(IRQ_IN_DONE,rioMRxIntr,0);
    rioIntConnect(IRQ_MISC_DONE,rioMTxIntr,0);
    rioIntEnable(INTR_IN_DONE);
    init = TRUE;
    return 0;
}

#if 0

/******************************************************************************/

/* test routines */

extern void intcp(void);
extern void outtcp();
extern long random();
#include <crc32.h>
#include <net/netutils.h>

int mm(unsigned nQW)
{
    unsigned long long ix;
    unsigned long long buf[256/8];
    
    for(ix = 0; ix < 256/8; ix++)
    {
        buf[ix] = ix | (~ix << 32ULL);
    }
    
    return mSend(boardProcNumGet(),buf,nQW,0);
}

int mtt(unsigned nPkt, unsigned mtuQW, unsigned dstID)
{
    unsigned i, ix, nQW = mtuQW;
    unsigned long long buf[256/8], count = 0;
    
    if(!mtuQW || (mtuQW != -1 && mtuQW > 32))
        return -1;
    
    mInit();
    
    for(ix = 0; ix < nPkt; ix++)
    {
        if(mtuQW == -1)
            nQW = (random() & 0x1f) + 1;
        
        for(i = 0; i < nQW; i++, count++)
            buf[i] = count | (~count << 32ULL);
        
        if(mSend(dstID,buf,nQW,0) == -1)
            return -1;
    }
    
    buf[0] = 0xffffffffffffffffULL;
    mSend(dstID,buf,1,0);
    
    printf("%s: count=%qx\n",__FUNCTION__,count);
    return 0;
}

#if 0
void nn()
{
    static char ip[16];
    crcInit();
    sprintf(ip,"7.0.0.%d",boardProcNumGet()+1);
    netIPSet("sm0",ip);
    _sp(60,intcp,0,0,0,0,0,0,0,0,0,0);
}
void nt()
{
    char* ip = (boardProcNumGet() == 2) ? "7.0.0.5" : "7.0.0.3";
    outtcp(ip,1000000,2048);
}

#endif

void cc(int n)
{
    register unsigned crc1, crc2;
    crc1 = crcGet((void*)0xa0000000,0x100000);
    while(n--)
    {
        crc2 = crcGet((void*)0xa0000000,0x100000);
        if(crc1 != crc2)
            printf("crc1=%08x crc2=%08x\n",crc1,crc2);
    }
}

void rd(unsigned id, unsigned count)
{
    void* mem = cacheMalloc(0x100000);
    
    while(count--)
    {
        if(rioDmaStart(0,mem,DMA_SRC_PCI,boardProcNumGet(),0,DMA_DST_RIO|DMA_DST_SWRITE,id,0x100000))
        {
            printf("rioDmaStart error\n");
            goto done;
        }
        
        if(rioDmaWait(0,FALSE,1000000))
        {
            printf("rioDmaWait error\n");
            goto done;
        }
    }
    
    done:
    cacheFree(mem);
    printf("done\n");
}

void pd(unsigned count)
{
    void* mem = cacheMalloc(0x100000);
    
    while(count--)
    {
        if(rioDmaStart(0,mem,DMA_SRC_PCI,boardProcNumGet(),mem,DMA_DST_PCI,boardProcNumGet(),0x100000))
        {
            printf("rioDmaStart error\n");
            goto done;
        }
        
        if(rioDmaWait(0,FALSE,1000000))
        {
            printf("rioDmaWait error\n");
            goto done;
        }
    }
    
    done:
    cacheFree(mem);
    printf("done\n");
}

#endif
        

unsigned mn(unsigned i, unsigned ix)
{
    return rioMuxRegGet(i,ix);
}

void tdel(unsigned cnt, unsigned tmr)
{
    unsigned v;
    v = rioRegs->endp.msgOut[1];
    v = CF_LE_L(v) & 0xffff;
    v |= (cnt << 16) | (tmr << 24);
    rioRegs->endp.msgOut[1] = CT_LE_L(v);
}

void rdel(unsigned cnt, unsigned tmr)
{
    unsigned v;
    v = (cnt << 16) | (tmr << 24);
    rioRegs->endp.msgIn3 = CT_LE_L(v);
    //     rioRegs->endp.unused5 = CT_LE_L(v);
}

#if 0
#warning test code
void rs(unsigned id, unsigned n, unsigned mbox)
{
    unsigned ix;

    if(!n)
	n = 1;

    for(ix = 0; ix < n; ix++)
	if(rmsgTxQueue(id,mbox,0x1000,0xa4000000,NULL,ix == n - 1 ? 2 : 0))
	    break;
}
#endif
