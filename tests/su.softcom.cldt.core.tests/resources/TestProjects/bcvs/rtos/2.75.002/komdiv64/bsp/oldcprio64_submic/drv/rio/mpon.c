#define RIO_SECONDS_PER_BEAT            0
#define RIO_NANOSECONDS_PER_BEAT        100000000
#define RIO_REGISTRATION_TIMEOUT        10

/* reconfiguration timeout in ns */

#ifndef RIO_RECONFIG_TIMEOUT
#define RIO_RECONFIG_TIMEOUT            10000000
#endif

/* reconfiguration doorbell number */

#ifndef RIO_RECONFIG_DB
#define RIO_RECONFIG_DB                 5
#endif

#ifndef  NO_VME_DMA_LOCK       /* if define - no mutex blocking DMA access */
#warning !!!!!!!!! USING mutexes for safe DMA accesss !!!!!!!!!!!
#else
#warning !!!!!!!!! DON'T USING mutexes for safe DMA accesss !!!!!!!!!!!
#endif


/******************************************************************************/

/* globals */

void* rioMasterMemBase = (void*)RIO_MEM_BASE;           // pointer to master windows base
void* rioSlaveMemBase = (void*)RIO_SLAVE_MEM_BASE;      // pointer to slave window base
int rioRegistrationFlag = FALSE;                        // if TRUE registration passed
unsigned rioHeartbeatCount = 0;                         // heartbeat counter
timer_t rioTimer;                                       // heartbeat timer
unsigned rioRegistrationMask = 0;                       // registered module mask
VOIDFUNCPTR boardAux6ClkRtn;                            // timer user interrupt handler
int boardAux6ClkArg;                                    // timer user interrupt handler argument
unsigned boardAux6ClkRate;                              // timer interrupt clock rate in HZ
unsigned boardAux6ClkFreq = MPON_PCI_CLOCK;             // timer counter clock rate in HZ

/* locals */

static sem_t dmaSem[2];                               // semaphores for dma interrupt synchronization
#ifndef  NO_VME_DMA_LOCK
static pthread_mutex_t dmaMutex[2];                   // dma channel mutex
#endif
static volatile unsigned dmaStat[2];                  // dma channel status
#ifndef BT5328
static int rioSwitchN;                                // number of switches
static int rioBoardN;                                 // number of endpoints
#endif
static RIO_SWITCH_CONFIG** rioCrateConfig;            // crossboard configuration table

/******************************************************************************/

/* get CKI status */

unsigned boardCKIStatusGet(void)
{
    unsigned val;
    
    if(!rioRegs)
        return 0xffffffff;
    
    val = rioRegs->spec.ckiStatus;
    return CF_LE_L(val);
}

/******************************************************************************/

/* clear CKI status */

void boardCKIStatusClear(unsigned stat)
{
    if(!rioRegs)
        return;
    rioRegs->spec.ckiStatus = CT_LE_L(stat);
}

/******************************************************************************/

/* enable CKI mask */

void boardCKIEnable(unsigned val)
{
    unsigned mask;
    
    if(!rioRegs)
        return;
    
    mask = intLock();
    rioRegs->spec.ckiMask |= CT_LE_L(val);
    intUnlock(mask);
}

/******************************************************************************/

/* disable CKI mask */

void boardCKIDisable(unsigned val)
{
    unsigned mask;
    
    if(!rioRegs)
        return;
    
    mask = intLock();
    rioRegs->spec.ckiMask &= ~CT_LE_L(val);
    intUnlock(mask);
}

/******************************************************************************/

/* output signal Test OK */

void boardTestOK(void)
{
    if(!rioRegs)
        return;
    
    rioRegs->spec.ckiStatus = CT_LE_L(CKI_STAT_MEM_ERR|CKI_STAT_PCI_ADDR_ERR|CKI_STAT_WDO|CKI_STAT_GOOD);
}

/******************************************************************************/

/* clear signal Test OK */

void boardClrTestOK(void)
{
    if(!rioRegs)
        return;
    
    boardCKIEnable(CKI_MASK_FAIL);
    rioRegs->spec.ckiStatus = CT_LE_L(CKI_STAT_MEM_ERR|CKI_STAT_PCI_ADDR_ERR|CKI_STAT_WDO);
}

/******************************************************************************/

/* set watchdog timer interval */

int _boardWDTRateSet(unsigned mul, unsigned div)
{
    unsigned long long count; unsigned h, l;
    
    if(!rioRegs || !div)
        return -1;
    
    count = (unsigned long long)boardAux6ClkFreq * (unsigned long long)mul /
    (unsigned long long)div;
    
    if(!count)
        return -1;
    
    l = count & 0xffffffff;
    h = count >> 32;
    rioRegs->user.wdoCountL = CT_LE_L(l);
    rioRegs->user.wdoCountH = CT_LE_L(h);
    boardCKIEnable(CKI_MASK_WDO_INT|CKI_MASK_WDO_MODE);
    return 0;
}

int boardWDTRateSet(int ticksPerSecond)
{
    return _boardWDTRateSet(1,ticksPerSecond);
}

/******************************************************************************/

/* start watchdog timer */

void boardWDTStart(void)
{
    if(!rioRegs)
        return;
    boardCKIEnable(CKI_MASK_WDO|CKI_MASK_FAIL);
    rioRegs->spec.wdoCtrl = CT_LE_L(1);
}

/******************************************************************************/

/* stop watchdog timer */

void boardWDTStop(void)
{
    if(!rioRegs)
        return;
    boardCKIDisable(CKI_MASK_WDO|CKI_MASK_FAIL);
    rioRegs->spec.wdoCtrl = 0;
}

/******************************************************************************/

/* reset watchdog timer */

void boardWDTRestart(void)
{
    if(!rioRegs)
        return;
    rioRegs->spec.wdoCtrl = CT_LE_L(1);
}

/******************************************************************************/

/* This routine handles clock interrupts */

void boardAux6ClkInt(unsigned unused)
{
    if(boardAux6ClkRtn)
        boardAux6ClkRtn(boardAux6ClkArg);
    
    rioRegs->spec.softRst |= CT_LE_L(0x100);
}

/*******************************************************************************
*
* tmrConnect - connect a routine to the clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* clock interrupt.
*
* RETURN: 0, or -1 if the routine cannot be connected to the interrupt.
*
*/

int boardAux6ClkConnect(VOIDFUNCPTR rtn, int arg)
{
    if(!rioRegs)
        return -1;
    
    rioIntConnect(IRQ_TIMER64,boardAux6ClkInt,0);
    
    boardAux6ClkRtn = rtn;
    boardAux6ClkArg = arg;
    return 0;
}

/******************************************************************************/

/* set timer mode: one short or cyclic */

void boardAux6ClkMode(unsigned mode)
{
    if(mode == TIMER_MODE_CYCLIC)
        boardCKIEnable(CKI_MASK_TIMER_CYCLIC);
    else
        boardCKIDisable(CKI_MASK_TIMER_CYCLIC);
}

/******************************************************************************/

/* start timer counter */

int boardAux6ClkStart(void)
{
    unsigned mask;
    if(!rioRegs)
        return -1;
    mask = intLock();
    rioRegs->spec.softRst |= CT_LE_L(0x100);
    intUnlock(mask);
    boardCKIEnable(CKI_MASK_TIMER_START|CKI_MASK_WDO_MODE);
    return 0;
}

/******************************************************************************/

/* stop timer counter */

int boardAux6ClkStop(void)
{
    if(!rioRegs)
        return -1;
    boardCKIDisable(CKI_MASK_TIMER_START);
    return 0;
}

/*******************************************************************************
*
* This routine disables clock interrupts.
*
* RETURNS: N/A
*
*/

void boardAux6ClkDisable(void)
{
    boardAux6ClkStop();
    rioIntDisable(INTR_TIMER64);
}

/********************************************************************************
*
* This routine enables clock interrupts.
*
* RETURNS: N/A
*
*/

void boardAux6ClkEnable(void)
{
    if(!rioRegs)
        return;
    boardAux6ClkStart();
    rioIntEnable(INTR_TIMER64);
}

/******************************************************************************/

/* load timer counter */

int boardAux6ClkCountSet(uint64_t count)
{
    unsigned l, h;
    if(!rioRegs)
        return -1;
    l = count & 0xffffffff;
    h = count >> 32;
    rioRegs->user.timerCountL = CT_LE_L(l);
    rioRegs->user.timerCountH = CT_LE_L(h);
    return 0;
}

/******************************************************************************/

/* read timer counter */

int boardAux6ClkCountGet(uint64_t* count)
{
    unsigned l, h;
    if(!rioRegs || !count)
        return -1;
    l = rioRegs->user.timerCountL; l = CF_LE_L(l);
    h = rioRegs->user.timerCountH; h = CF_LE_L(h);
    *count = ((uint64_t)h << 32ULL) | (uint64_t)l;
    return 0;
}

/*******************************************************************************
*
* This routine sets the interrupt rate of the clock.
*
* RETURNS: 0, or -1 if the tick rate is invalid.
*
*/

int _boardAux6ClkRateSet(unsigned mul, unsigned div)
{
    uint64_t val;
    
    if(!div || !mul)
        return -1;
    
    val = ((uint64_t)boardAux6ClkFreq * mul) / div;
    
    if(!val)
        return -1;
    
    boardAux6ClkRate = div / mul;
    return boardAux6ClkCountSet(val);
}

int boardAux6ClkRateSet(int _hz)
{
    boardAux6ClkMode(TIMER_MODE_CYCLIC);
    return _boardAux6ClkRateSet(1,_hz);
}

int boardAux6ClkRateGet(void)
{
    return boardAux6ClkRate;
}

/******************************************************************************/

/* dma interrupt service routine  */

void rioDmaIntr(unsigned ch)
{
    unsigned stat = rioRegs->spec.dma[ch].status;
    dmaStat[ch] = CF_LE_L(stat);
    
    if(stat & CT_LE_L(DMA_STAT_FAIL))
    {
        logMsg("%s: channel=%d fail: stat=%x\n",(int)__FUNCTION__,ch,dmaStat[ch],0,0,0);
    }
    
    rioRegs->spec.dma[ch].status = stat & ~CT_LE_L(DMA_STAT_RUN);
    sem_post(&dmaSem[ch]);
    RIO_DEBUG(4,("%s(%i)\n",(int)__FUNCTION__,ch,0,0,0,0));
}

/******************************************************************************/

/* start dma transaction with physycal addresses */

int _rioDmaStart(unsigned ch, unsigned src, unsigned srcMode, unsigned srcID,
                 unsigned dst, unsigned dstMode, unsigned dstID, unsigned nBytes)
{
    unsigned tail = nBytes & 0xff;

    if(ch > 1 || !nBytes || tail != rioDmaBound[tail] || (src & 7) || (dst & 7) || (srcMode & ~1) || (dstMode & ~7))
        return EINVAL;

#ifndef  NO_VME_DMA_LOCK
    pthread_mutex_lock(&dmaMutex[ch]);
#endif
    if(rioRegs->spec.dma[ch].status & CT_LE_L(DMA_STAT_RUN))
    {
        rioRegs->spec.dma[ch].status = 0;
#ifndef  NO_VME_DMA_LOCK
        pthread_mutex_unlock(&dmaMutex[ch]);
#endif
        return EBUSY;
    }
    
    src |= srcMode;
    dst |= dstMode;
    
    dmaStat[ch] = 0;
    rioRegs->spec.dma[ch].srcAddr = CT_LE_L(src);
    rioRegs->spec.dma[ch].srcID = CT_LE_L(srcID);
    rioRegs->spec.dma[ch].dstAddr = CT_LE_L(dst);
    rioRegs->spec.dma[ch].dstID = CT_LE_L(dstID);
    rioRegs->spec.dma[ch].count = CT_LE_L(nBytes);
    rioRegs->spec.dma[ch].status = CT_LE_L(DMA_STAT_RUN);
    
    dmaStartCount[dstID]++;
    
    return 0;
}

int rioDmaStart(unsigned ch, unsigned src, unsigned srcMode, unsigned srcID,
                unsigned dst, unsigned dstMode, unsigned dstID, unsigned nBytes)
{
    if(srcMode & DMA_SRC_PCI)
        src = pciLocalToBus((void*)src);
    
    if(dstMode & DMA_DST_PCI)
        dst = pciLocalToBus((void*)dst);
    
    return _rioDmaStart(ch,src,srcMode,srcID,dst,dstMode,dstID,nBytes);
}

/******************************************************************************/

/* stop DMA channel */

int rioDmaStop(unsigned ch)
{
    if(ch > 1)
        return EINVAL;
    
    rioRegs->spec.dma[ch].status = 0;
    return 0;
}

/******************************************************************************/

/* waiting for dma transaction complete */

int rioDmaWait(unsigned ch, unsigned poll, unsigned timeout)
{
    unsigned stat, res = 0, toutVal = timeout;

    if(ch > 1)
        return EINVAL;

    if((stat = rioRegs->spec.dma[ch].status) & CT_LE_L(DMA_STAT_DONE))
        goto done;

    if(poll)
    {
        while(timeout--)
        {
            if((stat = rioRegs->spec.dma[ch].status) & CT_LE_L(DMA_STAT_DONE))
            {
                done:
                dmaStat[ch] = CF_LE_L(stat);
                rioRegs->spec.dma[ch].status = stat & ~CT_LE_L(DMA_STAT_RUN);
                goto finish;
            }
            udelay(1);
        }
        res = ETIMEDOUT;
        goto tout;
    }
    else
    {
        rioIntEnable(INTR_DMA0_DONE << ch);
        res =_sem_timedwait_us(&dmaSem[ch],timeout);
        rioIntDisable(INTR_DMA0_DONE << ch);

        if(res == -1)
        {
            stat = rioRegs->spec.dma[ch].status;
            res = errno;
            goto tout;
        }
    }

finish:
    if(dmaStat[ch] & DMA_STAT_FAIL)
    {
        logMsg("%s: channel=%d: stat=%x poll=%d\n",(int)__FUNCTION__,
               ch,dmaStat[ch],poll,0,0);
        res = EIO;
    }

exit:
#ifndef  NO_VME_DMA_LOCK
    pthread_mutex_unlock(&dmaMutex[ch]);
#endif
    if(res)
    {
        unsigned val, src, srcID, dst, dstID, nBytes;
        val = rioRegs->spec.dma[ch].srcAddr; src = CF_LE_L(val);
        val = rioRegs->spec.dma[ch].srcID; srcID = CF_LE_L(val);
        val = rioRegs->spec.dma[ch].dstAddr; dst = CF_LE_L(val);
        val = rioRegs->spec.dma[ch].dstID; dstID = CF_LE_L(val);
        val = rioRegs->spec.dma[ch].count; nBytes = CF_LE_L(val);
        logMsg("%s: src=%x srcID=%x dst=%x dstID=%x nBytes=%x\n",(int)__FUNCTION__,
               src,srcID,dst,dstID,nBytes);
    }
    RIO_DEBUG(4,("%s: ch=%d res=%d\n",(int)__FUNCTION__,ch,res,0,0,0));
    return res;

tout:
    logMsg("%s: channel=%d: timeout=%dus stat=%x poll=%d\n",(int)__FUNCTION__,
           ch,toutVal,CF_LE_L(stat),poll,0);
     goto exit;
}

/******************************************************************************/

/* dma initialization routine */

static void dmaInit(void)
{
    unsigned ix;

#ifndef  NO_VME_DMA_LOCK
    pthread_mutex_init(&dmaMutex[0],NULL);
    pthread_mutex_init(&dmaMutex[1],NULL);
#endif

#if OS_VERSION >= 300
    sem_init(&dmaSem[0],1,0);
    sem_init(&dmaSem[1],1,0);
#else
    sem_init(&dmaSem[0],0,0);
    sem_init(&dmaSem[1],0,0);
#endif
    
    rioIntConnect(IRQ_DMA0_DONE,rioDmaIntr,0);
    rioIntConnect(IRQ_DMA1_DONE,rioDmaIntr,1);
    
    rioDmaBound[0] = 0;
    for(ix = 1; ix <= 8; ix++)
        rioDmaBound[ix] = 8;
    for(; ix <= 16; ix++)
        rioDmaBound[ix] = 16;
    for(; ix < 256; ix++)
        rioDmaBound[ix] = (ix & ~0x1f) + (32 * ((ix & 0x1f) != 0));
}

/******************************************************************************/

/* RapidIO data copy routine */

int rioMemCpy(unsigned id, unsigned rioAddr, unsigned char* bufAddr,
        unsigned nBytes, unsigned dir)

{
    void* rioBase;

    if(id > 7 || rioAddr >= 0x800000 || nBytes > 0x800000 || rioAddr + nBytes > 0x800000)
        return EINVAL;

    rioBase = (char*)RIO_MEM_BASE + id*0x800000 + rioAddr;

    if(dir == RIO_TO_HOST)
        memcpy(bufAddr,rioBase,nBytes);
    else
        memcpy(rioBase,bufAddr,nBytes);
    
    return 0;
}

/******************************************************************************/

/* get master windows local address */

void* rioMasterBaseGet(void)
{
    return rioMasterMemBase;
}

/******************************************************************************/

/* get slave window local address */

void* rioSlaveBaseGet(void)
{
    return rioSlaveMemBase;
}

#ifndef BT5328
/******************************************************************************/

/* set onboard tundra default port */

int _rioRouteSet(unsigned hop, unsigned id, unsigned route)
{
    unsigned i, j, val, p, port;
    
    for(i = 0; i < RIO_ID_MAX; i += 8)
    {
        val = 0x88888888;
        
        for(j = 0; j < 8 && i + j < RIO_ID_MAX; j++)
        {
            if(i+j == id)
            {
                p = 3;
            }
            else
            {
                p = route;
            }
            
            val = (val & ~(0xf << j*4)) | (p << j*4);
        }
        
        for(port = 0; port < 4; port++)
            if(rioMaintOut(hop,0x10a00+i/2+port*0x20000,id,val) == -1)
                return -1;
    }
    
    return 0;
}

int rioRouteSet(unsigned route)
{
    return _rioRouteSet(0,boardProcNumGet(),route);
}

/******************************************************************************/

/* set onboard tundra default port to none */

int rioRouteFail(unsigned hop, unsigned id)
{
    unsigned i, val, port;
    
    for(i = 0; i < RIO_ID_MAX; i += 8)
    {
        val = 0x88888888;
        
        for(port = 0; port < 4; port++)
            if(rioMaintOut(hop,0x10a00+i/2+port*0x20000,id,val) == -1)
                return -1;
    }
    
    return 0;
}

/******************************************************************************/

/* get cross type by name */

unsigned rioCrossTypeGet(char* name)
{
    if(!(strcasecmp(name,"rpkb")))
        return RIO_CROSS_RPKB;
    if(!(strcasecmp(name,"niisi")))
        return RIO_CROSS_NIISI2S_V2;
    return atoi(name);
}

#if (OS_VERSION > 301) || ((OS_VERSION == 301) && (OS_RELEASE >= 123))

extern void mpchanReconnect();

static sem_t reconfigSem, reconfigDoneSem;
volatile unsigned rioReconfigMask = 0, rioReconfigDone = FALSE;
pthread_t pRioReconfigThread;

/******************************************************************************/

/* reconfiguration doorbell handler */

void rioReconfigIntr(int unused, unsigned db, unsigned id)
{
    rioReconfigMask |= 1 << id;
    sem_post(&reconfigSem);
    RIO_DEBUG(2,("%s: id=%d mask=%02x\n",(int)__FUNCTION__,id,rioReconfigMask,0,0,0));
}

/******************************************************************************/

/* reconfiguration thread  */

void* rioReconfigThread(unsigned modulesMask)
{
    struct timespec ts;
    int ix;
    
    sem_wait(&reconfigSem);
    
    for(ix = 0; ix < RIO_ID_MAX; ix++)
    {
        if((1 << ix) & modulesMask)
            _rioDbOut(ix,RIO_RECONFIG_DB,0);
    }
    
    while(1)
    {
        if((rioReconfigMask & modulesMask) == modulesMask)
            goto done;
        
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_nsec += RIO_RECONFIG_TIMEOUT;
        if(ts.tv_nsec > 999999999)
            ts.tv_sec++;
        
        if(sem_timedwait(&reconfigSem,&ts) == -1)
        {
            RIO_DEBUG(1,("%s: sem_timedwait error=%s mask=%02x expected=%02x\n",(int)__FUNCTION__,
                         (int)strerror(errno),rioReconfigMask,modulesMask,0,0));
                         goto done;
        }
    }
    
    done:
#if OS_VERSION >=330
    mpchanReconnect(NULL);
#else
    mpchanReconnect();
#endif
    rioReconfigDone = TRUE;
    sem_post(&reconfigDoneSem);
    RIO_DEBUG(2,("%s: mask=%02x DONE\n",(int)__FUNCTION__,rioReconfigMask,0,0,0,0));
    return NULL;
}

/******************************************************************************/

/* switch to RPKB reserved crossboard */

int rioReservedRouteSet(unsigned modulesMask)
{
    RIO_SWITCH_CONFIG* brd = rioCrateConfigRpkbRes[boardProcNumGet()*2+1];
    struct timespec ts;
    unsigned ix;
    
    if(!brd)
        return ENOSYS;
    
    if(rioReconfigDone)
        return EAGAIN;
    
    if(rioReconfigMask)
        goto done;
    
    rioRouteSet(1);
    
    modulesMask &= ~(1 << boardProcNumGet());
    
    for(ix = 0; ix < RIO_N_BOARD_RPKB; ix++)
    {
        if((1 << brd[ix].id) & modulesMask)
            _rioRouteSet(brd[ix].hop,brd[ix].id,1);
    }
    
    _rioDbOut(boardProcNumGet(),RIO_RECONFIG_DB,1);
    
    done:
    
    clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_nsec += RIO_RECONFIG_TIMEOUT;
    if(ts.tv_nsec > 999999999)
        ts.tv_sec++;
    
    if(sem_timedwait(&reconfigDoneSem,&ts) == -1)
    {
        RIO_DEBUG(1,("%s: sem_timedwait error=%s mask=%02x expected=%02x\n",(int)__FUNCTION__,
                     (int)strerror(errno),rioReconfigMask,modulesMask,0,0));
                     return errno;
    }
    
    return 0;
}

/******************************************************************************/

/* reconfig initilization */

unsigned rioReconfigInit(void)
{
    unsigned res;
    struct sched_param schedparam;
    pthread_attr_t attr;
    
    if(sem_init(&reconfigSem,1,0) == -1)
    {
        printf("%s: sem_init error=%s\n",__FUNCTION__,strerror(errno));
        return errno;
    }
    
    if(sem_init(&reconfigDoneSem,1,0) == -1)
    {
        printf("%s: sem_init error=%s\n",__FUNCTION__,strerror(errno));
        return errno;
    }
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
    schedparam.sched_priority = 255;
    pthread_attr_setschedparam(&attr,&schedparam);
    
    if((res = pthread_create(&pRioReconfigThread,&attr,(void*)&rioReconfigThread,(void*)MODULES_MASK)))
    {
        printf("%s: pthread_create error=%s\n",__FUNCTION__,strerror(res));
        return res;
    }
    
    if(rioDbIntConnect(RIO_RECONFIG_DB,rioReconfigIntr,0) == -1)
        return EINVAL;
    
    return 0;
}

#endif  // OS >= 3.01.123

#else // BT5328

/******************************************************************************/

/* get cross type by name */

unsigned rioCrossTypeGet(char* name)
{
    if(!(strcasecmp(name,"bt5328")))
        return RIO_CROSS_BT5328;
    return atoi(name);
}

#endif // BT5328

#if (OS_VERSION >= 300)
#if (OS_VERSION < 301) || (OS_VERSION == 301 && OS_RELEASE < 90) || !defined(IO_CHAN)
//#if 1
#warning !!!!!!!!!!!!! OLD_RIO_CHAN !!!!!!!!!!!!
#define RIO_MPCHAN_MEM_BASE     0
#define RIO_MPCHAN_MEM_SIZE     0x100000
#define RIO_MPCHAN_MEM_MASK     (RIO_MPCHAN_MEM_SIZE-1)

#define RIO_MPCHAN_DB           0

// #define      RIO_MPCHAN_DB_THREAD

/******************************************************************************/

/* convert local address to virtual bus address */

int boardLocalToBusAdrs(int am, char* localAdrs, char** pBusAdrs)
{
    unsigned la = (unsigned)localAdrs;
    
    if((la >= RIO_SLAVE_CACHED_MEM_BASE && la < RIO_SLAVE_CACHED_MEM_BASE+0x800000) ||
        (la >= RIO_SLAVE_MEM_BASE && la < RIO_SLAVE_MEM_BASE+0x800000))
    {
        *pBusAdrs = (void*)(boardProcNumGet()*0x800000 | (la & 0x7fffff));
        RIO_DEBUG(3,("%s: la=%p ba=%p\n",(int)__FUNCTION__,(int)localAdrs,(int)*pBusAdrs,0,0,0));
        return 0;
    }
    
    if(la >= RIO_MEM_BASE && la < RIO_MEM_BASE + 0x4000000)
    {
        *pBusAdrs = (void*)(la & 0x03ffffff);
        RIO_DEBUG(3,("%s: la=%p ba=%p\n",(int)__FUNCTION__,(int)localAdrs,(int)*pBusAdrs,0,0,0));
        return 0;
    }
    kprint("%s: invalid la=%p\n\r",__FUNCTION__,localAdrs);
    return -1;
}

/******************************************************************************/

/* convert virtual bus address into local */

int boardBusToLocalAdrs(int am, char* busAdrs, char** pLocalAdrs)
{
    unsigned ba = (unsigned)busAdrs;
    
    if(ba >= 0x4000000)
    {
        kprint("%s: invalid ba=%p\n\r",__FUNCTION__,busAdrs);
        return -1;
    }
    
    if(ba >= boardProcNumGet()*0x800000 && ba < boardProcNumGet()*0x800000 + 0x800000)
    {
        *pLocalAdrs = (void*)(RIO_SLAVE_MEM_BASE | (ba & 0x7fffff));
        RIO_DEBUG(3,("%s: ba=%p la=%p\n",(int)__FUNCTION__,(int)busAdrs,(int)*pLocalAdrs,0,0,0));
        return 0;
    }
    
    *pLocalAdrs = (void*)(RIO_MEM_BASE | (ba & 0x3ffffff));
    RIO_DEBUG(3,("%s: ba=%p la=%p\n",(int)__FUNCTION__,(int)busAdrs,(int)*pLocalAdrs,0,0,0));
    return 0;
}

/******************************************************************************/

/* get DMA alignment bits */

void boardDmaAlignment(unsigned* addrAlign, unsigned* sizeAlign)
{
    *addrAlign = 8;
    *sizeAlign = 8;
}

/******************************************************************************/


static VOIDFUNCPTR mpIrqRtn = NULL;
static sem_t mpIrqSem;

void* boardMPThread(void* unused)
{
    while(1)
    {
        sem_wait(&mpIrqSem);
        
        if(mpIrqRtn)
            mpIrqRtn(0);
        
        RIO_DEBUG(3,("%s\n",(int)__FUNCTION__,0,0,0,0,0));
    }
    return NULL;
}
void boardMPIrq(int unused)
{
    if(sem_post(&mpIrqSem) == -1)
        logMsg("%s: sem_post error=%s (%d)\n",(int)__FUNCTION__,
               (int)strerror(errno),errno,0,0,0);
               RIO_DEBUG(3,("%s\n",(int)__FUNCTION__,0,0,0,0,0));
}

int boardMPIrqConnect(void (rtn)(int))
{
    #if 0                                    /* !!! laz */
    #warning test code
    mpIrqRtn = rtn;
    #else
    rioDbIntConnect(RIO_MPCHAN_DB,rtn,0);
    #endif
    return 0;
}

int boardMPIrqEnable()
{
    return 0;
}

void boardMPIrqIntr(int cpu)
{
    if(!(rioRegistrationMask & (1 << cpu)))     /* !!! laz */
        return;
    
    RIO_DEBUG(3,("%s: cpu=%d\n",(int)__FUNCTION__,cpu,0,0,0,0));
    
    _rioDbOut(cpu,RIO_MPCHAN_DB,0);
}

int boardSendToVme(void *src, void* dst, size_t size)
{
    unsigned pa, vdst, id, res, poll;   // tail,
    
    //kprint(" %s: src=%p dst=%p size=%x\n\r",__FUNCTION__,src,dst,size);
    if(size < 16)
    {
        RIO_DEBUG(3,("%s: src=%p dst=%p size=%x\n",(int)__FUNCTION__,(int)src,(int)dst,size,0,0));
        memcpy(dst,src,size);
        return 0;
    }
    //#warning
    /* starting from 3.1.90 buffers moved from unmapped memory (0x80000000) to mapped (0xc8000000) */
    #if OS_VERSION > 301 || (OS_VERSION == 301 && OS_RELEASE >= 90)
    MODCALL( pa=cpuV2P(SYSPROC_PID,src) );
    #else
    pa = K0_TO_PHYS (src);
    #endif
    cacheDataFlush(src,size);
    
    #if 0
    tail = size & 0xff;
    size = (size & ~0xff) | rioDmaBound[tail];
    #else
    if(size & 0xff)
        size = (size & ~0xff) + 0x100;
    #endif
    
    if(boardLocalToBusAdrs(0xd,dst,(char**)&vdst) == -1)
    {
        kprint(" %s: boardLocalToBusAdrs dst=%p error\n\r",__FUNCTION__,dst);
        return EINVAL;
    }
    
    id = vdst >> 23;
    vdst = vdst & 0x7fffff;
    
    poll = (size < RIO_DMA_IRQ_TRSH) ? TRUE : FALSE;
    
    //kprint(" %s: pa=%p vdst=%p size=%x id=%d\n\r",__FUNCTION__,pa,vdst,size,id);
    
    RIO_DEBUG(3,("%s: src=%p pa=%p vdst=%p size=%x id=%d\n",(int)__FUNCTION__,(int)src,pa,vdst,size,id));
    
    if((res = _rioDmaStart(0,pa,DMA_SRC_PCI,boardProcNumGet(),vdst,DMA_DST_RIO,id,size)))
        return res;
    /*  Tracing event */
    if((res = rioDmaWait(0,poll,1000000)))
        return res;
    
    
    #if 0
    if(memcmp(dst,src,size))
    {
        unsigned ix, nErr = 0; unsigned* p1 = src; unsigned* p2 = dst;
        kprint(" %s: src=%p dst=%p size=%x vdst=%p id=%d pa=%p memcmp\n\r",__FUNCTION__,src,dst,size,vdst,id,pa);
        for(ix = 0; ix < size/4; ix++)
        {
            if(p1[ix] != p2[ix])
            {
                kprint("ix=%x src=%p dst=%p val1=%08x val2=%08x\n\r",ix,&p1[ix],&p2[ix],p1[ix],p2[ix]);
                if(nErr++ > 15)
                    break;
}
}
return EIO;
}
#endif
return 0;
}

int boardShMem(int cpu, int type, volatile void** ptr, size_t* size)
{
    unsigned base;
    
    RIO_DEBUG (2,("boardShMem: cpu=%i type=%s\n\r",cpu,(int)(type==SHMEM_OBJECTS ?
    "SHMEM_OBJECTS" : "UNKNOWN !"),0,0,0,0));
    errno = 0;
    
    if(cpu < 0 || cpu >= RIO_ID_MAX)
    {
        errno = EINVAL;
        return -1;
    }
    
    switch(type)
    {
        case SHMEM_OBJECTS:
            base = RIO_MPCHAN_MEM_BASE;
            *size = RIO_MPCHAN_MEM_SIZE;
            break;
        default:
            errno = EINVAL;
            return -1;
    }
    
    if(cpu == boardProcNumGet())
    {
        *ptr = (void*)(RIO_SLAVE_MEM_BASE+base);
    }
    else
    {
        if(!(rioRegistrationMask & (1 << cpu)))   /* !!! laz */
        {
            errno = EINVAL;
            return -1;
        }
        *ptr = (void*)(RIO_MEM_BASE+cpu*0x800000+base);
    }
    
    RIO_DEBUG(2,("%s: cpu=%d mem=%p size=%x\n",(int)__FUNCTION__,cpu,(int)*ptr,*size,0,0));
    return 0;
}


#else  /* #if (OS_VERSION < 301) || (OS_VERSION == 301 && OS_RELEASE < 90) || !defined(NEW_RIO_CHAN) */
#warning    !!!!!!!!!!! NEW_RIO_CHAN  !!!!!!!!!!!!!!!!!!!!!!!

#if OS_VERSION < 301 || (OS_VERSION == 301 && OS_RELEASE < 132)
int boardLocalToBusAdrs(int am, char* localAdrs, char** pBusAdrs)
{
    kprint("%s: am=%d la=%p\n\r",__FUNCTION__,am,localAdrs);
    return -1;
}

int boardShMem(int cpu, int type, volatile void** ptr, size_t* size)
{
    unsigned base;
    kprint("%s: cpu=%d type=%d\n\r",__FUNCTION__,cpu,type);
    
    RIO_DEBUG (0,("boardShMem: cpu=%i type=%s\n\r",cpu,(int)(type==SHMEM_OBJECTS ?
    "SHMEM_OBJECTS" : "UNKNOWN !"),0,0,0,0));
    errno = EINVAL;
    return -1;
}
#endif

#endif  /* #if (OS_VERSION < 301) || (OS_VERSION == 301 && OS_RELEASE < 90) || !defined(NEW_RIO_CHAN) */
#endif  /* #if (OS_VERSION >= 300)  */

/******************************************************************************/

/* registration heartbeat routine attached to rioTimer */

void rioHeartbeat(union sigval s)
{
    rioRegs->csr.compTag = rioHeartbeatCount++;
}

/******************************************************************************/

/* registration routine */

int rioRegistration(unsigned mask)
{
    unsigned mmask, hmask, pass, ix, val;
    struct sigevent event;
    struct itimerspec time;
    struct timespec ts, ts1, ts2;
    unsigned tag[RIO_ID_MAX];
    
    RIO_DEBUG(2,("%s: mask=%#x rioInitDoneFlag=%i\n\r", (int)__FUNCTION__, mask, rioInitDoneFlag,0,0,0));
    
    if(!rioInitDoneFlag)
    {
        printf("%s: rio not initialized!!!\n\r",__FUNCTION__);
        return -1;
    }
    
    if(!mask)
        mask = 0xffffffff;
    
    rioHeartbeatCount = 0; rioRegs->csr.compTag = 0;                              /* my rioHeartbeatCount */
    
    /* install heartbeat timer */
    
    event.sigev_notify          = SIGEV_FUNCTION;
    event.sigev_signo           = SIGRTMIN;
    event.sigev_notify_function = rioHeartbeat;
    
    if(timer_create(CLOCK_REALTIME,&event,&rioTimer) == -1)
    {
        printf("%s: timer_create error: %s (%d)\n\r",
               __FUNCTION__,strerror(errno),errno);
               return -1;
    }
    
    time.it_interval.tv_sec = RIO_SECONDS_PER_BEAT;
    time.it_interval.tv_nsec = RIO_NANOSECONDS_PER_BEAT;
    time.it_value.tv_sec = RIO_SECONDS_PER_BEAT;
    time.it_value.tv_nsec = RIO_NANOSECONDS_PER_BEAT;
    
    if(timer_settime(rioTimer,0,&time,NULL) == -1)
    {
        printf("%s: timer_settime error: %s (%d)\n\r",
               __FUNCTION__,strerror(errno),errno);
               return -1;
    }
    
    for(ix = 0, mmask = 0; ix < RIO_ID_MAX; ix++)
    {
        if(mask & (1 << ix))
        {
            if(rioMaintIn(255,CAR_DEV_ID,ix,&val) == -1)
                continue;
            mmask |= 1 << ix;
        }
    }
    // mmask &= ~(1 << boardProcNumGet());            /* Exclude myself */
    
    RIO_DEBUG(6,("%s: mmask=%X\n\r", (int)__FUNCTION__,mmask,0,0,0,0));
    
    if(!mmask)
    {
        printf("%s: maintenance mask=0\n\r",__FUNCTION__);
        return -1;
    }
    
    clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec += RIO_REGISTRATION_TIMEOUT;
    
    hmask = 0;
    pass = 0;
    
    memset(tag,0,sizeof(tag));
    
    ts2.tv_sec = RIO_SECONDS_PER_BEAT;
    ts2.tv_nsec = RIO_NANOSECONDS_PER_BEAT;
    
    while(1)
    {
        for(ix = 0; ix < RIO_ID_MAX; ix++)
        {
            if(!(mmask & (1 << ix)))
                continue;
            if(hmask & (1 << ix))
                continue;
            if(rioMaintIn(255,CSR_COMP_TAG,ix,&val) == -1)
            {
                printf("%s: id=%d error read CSR_COMP_TAG\n\r",__FUNCTION__,ix);
                mmask &= ~(1 << ix);
                hmask &= ~(1 << ix);
            }
            // if (cnt++ % 10 ==0)
            //          if(boardProcNumGet() == 1)
            { RIO_DEBUG(6,("%s: id=%i heartBeat=%i hmask=%x mmask=%x my hb==%i\n\r", (int)__FUNCTION__,ix, val, hmask,mmask, rioHeartbeatCount));
            }
            
            if(pass == 0)
            {
                tag[ix] = val;
            }
            else
            {
                if (tag[ix] != val)
                { RIO_DEBUG(6,("%s: ID=%i registered with heartBeat=%i my heartBeat=%i\n\r", (int)__FUNCTION__,ix, val,rioHeartbeatCount,0,0));
                hmask |= 1 << ix;
                }
            }
        }

        if(hmask == mmask)
            break;

        clock_gettime(CLOCK_REALTIME,&ts1);
        
        if(ts1.tv_sec > ts.tv_sec || (ts1.tv_sec == ts.tv_sec && ts1.tv_nsec > ts.tv_nsec))
        {
            printf("%s: registration timeout=%d sec,requested mask=%08x  maintenance mask=%08x  registered mask=%08x\n\r",
                   __FUNCTION__,RIO_REGISTRATION_TIMEOUT, mask, mmask, hmask);
                   rioRegistrationMask = hmask;
                   return -1;
        }
        
        pass = 1;
        
        nanosleep(&ts2,NULL);
    }
    
    rioRegistrationMask = hmask;
    printf("%s: DONE with mask=%08x (requested mask=%08x maintenance mask=%08x)\n\r", __FUNCTION__,rioRegistrationMask, mask, mmask);
    rioRegistrationFlag = TRUE;
    return 0;
}

/******************************************************************************/

/* RapidIO initialization */
/*
#define RIO_CROSS_UNKNOWN       (-1)    // no attempt to configure commutators
#define RIO_CROSS_RPKB          1
#define RIO_CROSS_NIISI2S       2       // D9 and D10 switch configuration
#define RIO_CROSS_NIISI1S       3       // D10 switch configuration
#define RIO_CROSS_NIISI2S_V2    4       // D9 and D10 switch configuration version 2
#define RIO_CROSS_RPKB_RESERVE  5
#define RIO_CROSS_RPKB_FULL     6
*/

void rioInit(int crossType)
{
    unsigned val, id = 0;
    unsigned intNum, ix; PCI_DEV d = { cookie: -1 };
    
    while(pciScan(&d) != -1)
    {
        if(d.id == 0x5a4d191e)
            goto found;
    }

    rioRegs = NULL;
    kprint("%s: controller not found\n\r",__FUNCTION__);
    return;

found:

    RIO_DEBUG(6,("%s: started, time=%i!\n\r", (int)__FUNCTION__,time(NULL),0,0,0,0));


    switch(crossType)
    {
#ifdef BT5328
        case RIO_CROSS_BT5328:                 /* ??? laz */
            printf("\nWorking with BT5328 crossboard\n");
            rioCrateConfig = rioCrateConfigBT5328;
            break;
#else
        case RIO_CROSS_RPKB:
            printf("\nWorking with RPKB crossboard\n");
            rioSwitchN = RIO_N_SWITCH_RPKB;
            rioBoardN = RIO_N_BOARD_RPKB;
            rioCrateConfig = rioCrateConfigRpkb;
            break;
        case RIO_CROSS_NIISI2S:
            printf("\nWorking with NIISI crossboard, D9 and D10 switch configuration\n");
            rioSwitchN = RIO_N_SWITCH_NIISI2S;
            rioBoardN = RIO_N_BOARD_NIISI2S;
            rioCrateConfig = rioCrateConfigNiisi2s;
            break;
        case RIO_CROSS_NIISI1S:
            printf("\nWorking with NIISI crossboard, D10 switch configuration\n\n");
            rioSwitchN = RIO_N_SWITCH_NIISI1S;
            rioBoardN = RIO_N_BOARD_NIISI1S;
            rioCrateConfig = rioCrateConfigNiisi1s;
            break;
        case RIO_CROSS_NIISI2S_V2:
            printf("\nWorking with NIISI crossboard, D9 and D10 switch configuration version 2\n");
            rioSwitchN = RIO_N_SWITCH_NIISI2S_V2;
            rioBoardN = RIO_N_BOARD_NIISI2S_V2;
            rioCrateConfig = rioCrateConfigNiisi2sv2;
            break;
        case RIO_CROSS_RPKB_RESERVE:
            printf("Working with RPKB RESERVED crossboard\n");
            rioSwitchN = RIO_N_SWITCH_RPKB;
            rioBoardN = RIO_N_BOARD_RPKB;
            rioCrateConfig = rioCrateConfigRpkbRes;
            break;
        case RIO_CROSS_RPKB_FULL:
            printf("Working with RPKB MAIN and RESERVED crossboard\n");
            rioSwitchN = RIO_N_SWITCH_RPKB;
            rioBoardN = RIO_N_BOARD_RPKB;
            rioCrateConfig = rioCrateConfigRpkb;
            break;
#endif
        case RIO_CROSS_UNKNOWN:
            printf("Working with remote crossboard configuration\n");
            break;
        default:
            printf("\nUnknown crossboard type=%d\n",crossType);
            return;
    }

    rioCrossType = crossType;

    rioRegs = (void*)RIO_REGS_BASE;

    RIO_DEBUG(6,("%s: genCtlReg=%x, time=%i\n\r", (int)__FUNCTION__, rioRegs->ext.genCtl,time(NULL),0,0,0));

    rioBus = d.bus;
    rioDev = d.dev;
    pciConfigOutLong(rioBus,rioDev,0,0x10,0xffffffff);
    pciConfigOutLong(rioBus,rioDev,0,0x14,0xffffffff);
    pciConfigOutLong(rioBus,rioDev,0,0x18,0xffffffff);
    pciConfigOutLong(rioBus,rioDev,0,0x1c,pciLocalToMem((unsigned)rioRegs));
    pciConfigOutLong(rioBus,rioDev,0,0x04,0xffff0002);  /* MPON create SERR and PERR from nowhere ! skip it */
    udelay(10);

    val = rioRegs->user.ga;
    val = CF_LE_L(val);
    
    if(val & 0x80)
    {
        id = (val & 0x1f) - 1;
        boardProcNumSet(id);
        printf("%s: this board ID=%d\n",__FUNCTION__,id);
    }
    else
    {
        printf("%s: GA not valid val=%x\n",__FUNCTION__,val);
#ifdef BT5328
	id = 2;
#endif
        boardProcNumSet(id);
        printf("%s: this board ID=%d\n",__FUNCTION__,id);
    }

    rioDate();
    
    rioRegs->csr.cfgLBAR = CT_LE_L(0x1000000 >> 24);    // [9:0] = bridge regs on RIO / 16MB
    rioRegs->csr.baseDevID = CT_LE_L(id|(id<<16));      // bridge ID on RIO
    
    // route all ID to port 0
    
    for(ix = 0; ix < 256; ix++)
    {
//        rioRegs->csr.routeDstID = CT_LE_L(ix);
//        rioRegs->csr.routePort = CT_LE_L(0);
        rioRegs->port[3].routeDstID = CT_LE_L(ix);
        rioRegs->port[3].routePort = CT_LE_L(0);
    }
    
    rioRegs->ext.port[0].ctl = CT_LE_L(0x44000000);     // enable in/out ports
    rioRegs->ext.port[0].errStat = 0xffffffff;          // clear port status
    
    rioRegs->spec.memSize = CT_LE_L(0xff000000);        // slave memory size on RIO 16MB
    rioRegs->spec.memBaseAddr = CT_LE_L(0);             // [31:24]->[31:24],[1:0]->[33:32] slave memory address on RIO
    val = pciLocalToBus((void*)RIO_SLAVE_MEM_BASE);     // [31:24] slave memory address on PCI
    rioRegs->spec.memTranslAddr = CT_LE_L(val);

    rioRegs->err.status1 = 0xffffffff;                  // clear additional status
    rioRegs->ext.ltCtl = CT_LE_L(HOST_LT_CTL_VAL);      // Link Time-Out
#ifdef MPON_RIO_MSG
    rioRegs->ext.rtCtl = CT_LE_L(HOST_RT_CTL_VAL);      // Response Time-Out
#else
    rioRegs->ext.rtCtl = CT_LE_L(HOST_RT_CTL_VAL << 8); // Response Time-Out
#endif
    rioRegs->spec.enable = CT_LE_L(0x80c0ff00);         // enable access from RIO
    rioRegs->err.rw[1] |= CT_LE_L(0x02000000);          // enable PCI-PCI dma boost

#if OS_VERSION < 300
    cacheDataInvalidate((void*)K1_TO_K0(RIO_SLAVE_MEM_BASE),0x1000000);
#endif

    rioRegs->user.memBarSize = CT_LE_L(0x7c000);        // 64MB
    rioRegs->user.addCtl = CT_LE_L(0x50c);              // enable target abort, retry, master and target abort translate to 0xffffffff

    pciConfigOutLong(rioBus,rioDev,0,0x14,pciLocalToMem(RIO_MEM_BASE));

    for(ix = 0; ix < 8; ix++)
    {
        // [31:20]->RIO ADDR[31:20], [17:16]->RIO ADDR[33:32], [9:8]->PRI, [7:4]->FTYPE, [3:0]->TTYPE
        rioRegs->lut.memTranslAddr[ix*2] = CT_LE_L(1);          // NWRITE_R,NREAD
        rioRegs->lut.memID[ix*2] = CT_LE_L(ix);
        rioRegs->lut.memTranslAddr[ix*2+1] = CT_LE_L(0x400001); // NWRITE_R,NREAD
        rioRegs->lut.memID[ix*2+1] = CT_LE_L(ix);
    }
    rioRegs->endp.intrEnable = 0;                       // disable all interrupts

    intNum = pciIntNum(rioBus,rioDev,0);
    pciIntConnect(intNum,rioIntr,0);
    pciIntEnable(intNum);
    
    if(boardFreqRef == 25000000)
        pciConfigOutLong(rioBus,rioDev,0,0x04,0xffff0006);   /* MPON create SERR and PERR from nowhere on the frequency! skip it    ???  */
    else
        pciConfigOutLong(rioBus,rioDev,0,0x04,0xffff0146);

#if OS_VERSION >= 300
    pshared_mutex_init(&maintMutex,"rioMaint");
#else
    pthread_mutex_init(&maintMutex,NULL);
#endif
    dbInInit(1);
    dmaInit();

    if(id >= RIO_ID_MAX)
    {
        printf("%s: too big board ID=%d\n",__FUNCTION__,id);
        return;
    }

    if(crossType == RIO_CROSS_UNKNOWN)
    {
        struct timespec t1, t2, t3;
        clock_getres(CLOCK_REALTIME,&t1);
        clock_gettime(CLOCK_REALTIME,&t2);
        t2.tv_sec += RIO_CONFIG_TIMEOUT;

        clock_gettime(CLOCK_REALTIME,&t3);
        RIO_DEBUG(6,("\n\r%s: waiting genCtlReg(now=%x), time={%i,%li} delay={%d,%d}\n\r", (int)__FUNCTION__,
            rioRegs->ext.genCtl,t3.tv_sec,t3.tv_nsec,t1.tv_sec,t1.tv_nsec));

        while(1)
        {
            if(rioRegs->ext.genCtl & CT_LE_L(0x20000000))
            {
                clock_gettime(CLOCK_REALTIME,&t3);
                if(!t1.tv_sec && t1.tv_nsec < 50000000)
                    t1.tv_nsec = 50000000;
                RIO_DEBUG(6,("\n\r%s: genCtlReg happened (=%x) at time=(%i,%li), making pause for (sec=%i,nsec=%li), \n\r",
                             (int)__FUNCTION__, rioRegs->ext.genCtl,t3.tv_sec,t3.tv_nsec,t1.tv_sec,t1.tv_nsec));
                nanosleep(&t1,NULL);

                clock_gettime(CLOCK_REALTIME,&t3);
                RIO_DEBUG(6,("\n\r%s: pause ended, time=(%i,%li)\n\r", (int)__FUNCTION__, t3.tv_sec,t3.tv_nsec,0,0,0));

                printf("\n\r%s: board configuration complete\n",__FUNCTION__);
                goto done;
            }
            clock_gettime(CLOCK_REALTIME,&t3);
            RIO_DEBUG(1,("%s: waiting genCtlReg continue, time=(%i,%li)\n\r",(int)__FUNCTION__, t3.tv_sec,t3.tv_nsec,0,0,0));
            nanosleep(&t1,NULL);
            clock_gettime(CLOCK_REALTIME,&t3);
            if(t3.tv_sec > t2.tv_sec || (t3.tv_sec == t2.tv_sec && t3.tv_nsec > t2.tv_nsec))
            {
                val = rioRegs->ext.genCtl; val = CF_LE_L(val);
                printf("%s: board configuration timeout: GEN_CTL=%x\n",__FUNCTION__,val);
                goto done;
            }
        }
    }

#ifdef BT5328
    rioCfg(rioCrateConfig[id]);
#else
    if(!rioCrateConfig[id*2])
    {
        printf("%s: crate configuration for ID=%d not available\n",__FUNCTION__,id);
        return;
    }

    if(crossType == RIO_CROSS_RPKB_FULL)
    {
        rioForcedCfg = TRUE;
        if(rioMaintOut(0,PORT_GEN_CTL,id,0) == -1)
            return;
        rioCfg(rioCrateConfigRpkbRes[id*2],rioSwitchN,NULL,0);
        if(rioMaintOut(0,PORT_GEN_CTL,id,0) == -1)
            return;
        rioCfg(rioCrateConfigRpkb[id*2],rioSwitchN,rioCrateConfigRpkb[id*2+1],rioBoardN);
    }
    else
    {
        rioCfg(rioCrateConfig[id*2],rioSwitchN,rioCrateConfig[id*2+1],rioBoardN);
    }
#endif
done:
#if ((OS_VERSION > 301) || ((OS_VERSION == 301) && (OS_RELEASE >= 123))) && !defined(BT5328)
    rioReconfigInit();
#endif
    rioInitDoneFlag = TRUE;
    rioMsgMode = 1;
}
