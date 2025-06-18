/* nand.c - NAND flash support library */

#include <semaphore.h>
#include <board.h>

#define	NAND_RESET_TIMEOUT	500
#define	NAND_WRITE_TIMEOUT	700
#define	NAND_ERASE_TIMEOUT	30000
#define	NAND_READ_TIMEOUT	50

/* validate params */

#define	NAND_CHECK_PARAMS()							\
do {										\
    if((!data && !oob))								\
    {										\
	NAND_DEBUG(1,("%s: invalid params data=0 oob=0 chunk=%#x (%d)\n",	\
	    (long)__PRETTY_FUNCTION__,chunk,chunk,0,0,0));			\
	return -1;								\
    }										\
} while(0);

/* select the chip */

#define	NAND_CHIP_SELECT()					\
do {								\
    chip = chunk / NAND_PAGES_PER_CHIP;				\
								\
    if(chip >= NAND_N_CHIPS)					\
    {								\
	NAND_DEBUG(1,("%s: invalid chunk=%#x (%d)\n",(long)	\
	    __PRETTY_FUNCTION__,chunk,chunk,0,0,0));		\
	return -1;						\
    }								\
								\
    nandEntry = __PRETTY_FUNCTION__;				\
    nandChunk = chunk;						\
								\
    *NAND_CTRL_REG = NAND_CTRL_CS|chip;				\
								\
    chunk %= NAND_PAGES_PER_CHIP;				\
								\
} while(0);


/* debug print */

#define	NAND_DEBUG(level,msg)		\
{					\
    if(nandDebug >= (level))		\
	logMsg msg;			\
}

/* globals */

int nandDebug = 1;		/* debug level */
int nandWritePoll = FALSE;	/* enable write poll busy mode instead IRQ */
int nandErasePoll = FALSE;	/* enable erase poll busy mode instead IRQ */
int nandInitDone = FALSE;	/* initialization done flag */
int nandBlocksPerChip = NAND_BLOCKS_PER_CHIP;	/* blocks per chip */

/* locals */

static sem_t nandSem;		/* interrupt synchronization semaphore */
static const char* nandEntry;	/* entry name */
static unsigned nandChunk;	/* selected chunk */

/******************************************************************************/

/* interrupt service routine */

void nandIntr(long arg)
{
    NAND_DEBUG(4,("%s: chunk=%#x (%d)\n",(long)__PRETTY_FUNCTION__,nandChunk,nandChunk,0,0,0));
    *NAND_CTRL_REG |= NAND_CTRL_IRQ;
    sem_post(&nandSem);
}

/******************************************************************************/

/* polling flash ready/busy status with timeout */

inline int nandPollBusy(unsigned timeout)
{
    unsigned ix;

    for(ix = 0; ix < timeout; ix++)
    {
	if(*NAND_CTRL_REG & NAND_CTRL_FLASH_READY)
	    return 0;
	udelay(1);
    }

    NAND_DEBUG(1,("%s: %s: chunk=%#x (%d) timeout=%dus\n",(long)__PRETTY_FUNCTION__,
	(long)nandEntry,nandChunk,nandChunk,timeout,0));
    return -1;
}

/******************************************************************************/

/* issue RESET command for each chip */

inline int nandReset(void)
{
    int chip;

    nandEntry = __PRETTY_FUNCTION__;
    nandChunk = 0;

    for(chip = 0; chip < NAND_N_CHIPS; chip++)
    {
	*NAND_CTRL_REG = NAND_CTRL_CS|chip;
	*NAND_CMD_REG = 0xff;
    }

    udelay(1);

    return nandPollBusy(NAND_RESET_TIMEOUT);
}

/******************************************************************************/

/* parse operation status */

int nandStatus(void)
{
    unsigned char val;

    *NAND_CMD_REG = 0x70;
    udelay(1);
    val = *NAND_DATA_BYTE;

    if(val & 1)
    {
	NAND_DEBUG(1,("%s: %s: chunk=%#x (%d) status=0x%02x\n",(long)__PRETTY_FUNCTION__,
	    (long)nandEntry,nandChunk,nandChunk,val,0));
	return -1;
    }

    return 0;
}

/******************************************************************************/

/* waiting for interrupt: Ready/Busy 0->1 */

int nandIrqWait(int poll, unsigned timeout)
{
    if(poll)
    {
	unsigned ix;

	for(ix = 0; ix < timeout; ix++)
	{
	    if(*NAND_CTRL_REG & NAND_CTRL_FLASH_READY)
		return 0;
	    udelay(1);
	}

	NAND_DEBUG(1,("%s: %s: chunk=%#x (%d) timeout=%dus\n",(long)__PRETTY_FUNCTION__,
	    (long)nandEntry,nandChunk,nandChunk,timeout,0));
	return -1;
    }
    else
    {
	int res; struct timespec ts;

	boardIvEnable(IV_NAND);
	clock_gettime(CLOCK_REALTIME,&ts);
	ts.tv_sec++;
	res = sem_timedwait(&nandSem,&ts);
	boardIvDisable(IV_NAND);

	if(res == -1)
	{
	    NAND_DEBUG(1,("%s: %s: chunk=%#x (%d) irq timeout\n",(long)__PRETTY_FUNCTION__,
		(long)nandEntry,nandChunk,nandChunk,0,0));
	    return -1;
	}

	return 0;
    }
}

/******************************************************************************/

/* read chunk from flash */

int nandRead(unsigned chunk, void* data, void* oob)
{
    unsigned chip, addr = data ? 0 : 0x08, ix;
    volatile unsigned long long dw;
    unsigned char* p = (void*)&dw;

    NAND_CHECK_PARAMS();
    NAND_CHIP_SELECT();
//printf("chip=%d sr=%x chunk=%x (%d)\n",chip,*NAND_CTRL_REG,chunk,chunk);
    *NAND_CMD_REG = 0x00;
    *NAND_ADDR_REG = 0;
    *NAND_ADDR_REG = addr;
    *NAND_ADDR_REG = chunk & 0xff;
    *NAND_ADDR_REG = (chunk >> 8) & 0xff;
    *NAND_ADDR_REG = (chunk >> 16) & 0xff;
    *NAND_CMD_REG = 0x30;

    udelay(1);

    if(nandPollBusy(NAND_READ_TIMEOUT) == -1)
	return -1;

    if(!data)
    {
	for(ix = 0; ix < 64; ix += 8)
	{
	    dw = *NAND_DATA_DWORD;
	    *(unsigned char*)(oob+ix+0) = p[7];
	    *(unsigned char*)(oob+ix+1) = p[6];
	    *(unsigned char*)(oob+ix+2) = p[5];
	    *(unsigned char*)(oob+ix+3) = p[4];
	    *(unsigned char*)(oob+ix+4) = p[3];
	    *(unsigned char*)(oob+ix+5) = p[2];
	    *(unsigned char*)(oob+ix+6) = p[1];
	    *(unsigned char*)(oob+ix+7) = p[0];
	}
    }
    else
    {
	for(ix = 0; ix < 2048; ix += 8)
	{
	    dw = *NAND_DATA_DWORD;
	    *(unsigned char*)(data+ix+0) = p[7];
	    *(unsigned char*)(data+ix+1) = p[6];
	    *(unsigned char*)(data+ix+2) = p[5];
	    *(unsigned char*)(data+ix+3) = p[4];
	    *(unsigned char*)(data+ix+4) = p[3];
	    *(unsigned char*)(data+ix+5) = p[2];
	    *(unsigned char*)(data+ix+6) = p[1];
	    *(unsigned char*)(data+ix+7) = p[0];
	}
	if(oob)
	    for(ix = 0; ix < 64; ix += 8)
	    {
		dw = *NAND_DATA_DWORD;
		*(unsigned char*)(oob+ix+0) = p[7];
		*(unsigned char*)(oob+ix+1) = p[6];
		*(unsigned char*)(oob+ix+2) = p[5];
		*(unsigned char*)(oob+ix+3) = p[4];
		*(unsigned char*)(oob+ix+4) = p[3];
		*(unsigned char*)(oob+ix+5) = p[2];
		*(unsigned char*)(oob+ix+6) = p[1];
		*(unsigned char*)(oob+ix+7) = p[0];
	    }
    }

    *NAND_CTRL_REG = 0;

    /* if oob readed chip automatically load next page */

    udelay(1);

    return nandPollBusy(NAND_READ_TIMEOUT);
}


/******************************************************************************/

/* check that chunk is free (2048+64 bytes of 0xff) */

int nandIsFree(unsigned chunk, int* res)
{
    unsigned chip, ix;

    NAND_CHIP_SELECT();

    *NAND_CMD_REG = 0x00;

    *NAND_ADDR_REG = 0;
    *NAND_ADDR_REG = 0;
    *NAND_ADDR_REG = chunk & 0xff;
    *NAND_ADDR_REG = (chunk >> 8) & 0xff;
    *NAND_ADDR_REG = (chunk >> 16) & 0xff;

    *NAND_CMD_REG = 0x30;

    udelay(1);

    if(nandPollBusy(NAND_READ_TIMEOUT) == -1)
	return -1;

    *res = TRUE;

    for(ix = 0; ix < 2112/8; ix++)
    {
	if(*NAND_DATA_DWORD != -1ULL)
	{
	    *res = FALSE;
	    break;
	}
    }

    *NAND_CTRL_REG = 0;

    /* if oob readed chip automatically load next page */

    udelay(1);

    return nandPollBusy(NAND_READ_TIMEOUT);
}

/******************************************************************************/

/* check that byte 2048 is not 0xff */

int nandIsBad(unsigned chunk, int* res)
{
    unsigned chip;

    NAND_CHIP_SELECT();

    *NAND_CMD_REG = 0x00;
    *NAND_ADDR_REG = 0x00;
    *NAND_ADDR_REG = 0x08;
    *NAND_ADDR_REG = chunk & 0xff;
    *NAND_ADDR_REG = (chunk >> 8) & 0xff;
    *NAND_ADDR_REG = (chunk >> 16) & 0xff;
    *NAND_CMD_REG = 0x30;

    udelay(1);

    if(nandPollBusy(NAND_READ_TIMEOUT) == -1)
        return -1;

    *res = (*NAND_DATA_BYTE != 0xff) ? TRUE : FALSE;

    *NAND_CTRL_REG = 0;

    return nandPollBusy(NAND_READ_TIMEOUT);
}

/******************************************************************************/

/* write chunk(s) to flash */

int nandWrite(unsigned chunk, void* data, void* oob)
{
    unsigned chip, ix, addr = data ? 0 : 0x08;
    volatile unsigned long long dw;
    unsigned char* p = (void*)&dw;

    NAND_CHECK_PARAMS();
    NAND_CHIP_SELECT();

    *NAND_CMD_REG = 0x80;
    *NAND_ADDR_REG = 0;
    *NAND_ADDR_REG = addr;
    *NAND_ADDR_REG = chunk & 0xff;
    *NAND_ADDR_REG = (chunk >> 8) & 0xff;
    *NAND_ADDR_REG = (chunk >> 16) & 0xff;

    if(data)
	for(ix = 0; ix < 2048; ix += 8)
	{
	    p[7] = *(unsigned char*)(data+ix+0);
	    p[6] = *(unsigned char*)(data+ix+1);
	    p[5] = *(unsigned char*)(data+ix+2);
	    p[4] = *(unsigned char*)(data+ix+3);
	    p[3] = *(unsigned char*)(data+ix+4);
	    p[2] = *(unsigned char*)(data+ix+5);
	    p[1] = *(unsigned char*)(data+ix+6);
	    p[0] = *(unsigned char*)(data+ix+7);
	    *NAND_DATA_DWORD = dw;
	}

    if(oob)
	for(ix = 0; ix < 64; ix++)
	    *NAND_DATA_BYTE = *(unsigned char*)(oob+ix);

    *NAND_CTRL_REG = NAND_CTRL_CS|NAND_CTRL_IRQ|chip;
    *NAND_CMD_REG = 0x10;

    udelay(10);

    if(nandIrqWait(nandWritePoll,NAND_WRITE_TIMEOUT) == -1)
	return -1;

    return nandStatus();
}

/******************************************************************************/

/* erase specified block(s) */

int nandErase(unsigned chunk)
{
    unsigned chip;

    NAND_CHIP_SELECT();

    *NAND_CMD_REG = 0x60;
    *NAND_ADDR_REG = chunk & 0xff;
    *NAND_ADDR_REG = (chunk >> 8) & 0xff;
    *NAND_ADDR_REG = (chunk >> 16) & 0xff;
    *NAND_CTRL_REG = NAND_CTRL_CS|NAND_CTRL_IRQ|chip;
    *NAND_CMD_REG = 0xd0;

    udelay(1);

    if(nandIrqWait(nandErasePoll,NAND_ERASE_TIMEOUT) == -1)
	return -1;

    return nandStatus();
}

/******************************************************************************/

/* get ID codes */

int nandIdGet(unsigned chip, unsigned char* buf)
{
    unsigned ix;

    if(chip >= NAND_N_CHIPS)
    {
	NAND_DEBUG(1,("%s: invalid chip=%d\n",(long)
	    __PRETTY_FUNCTION__,chip,0,0,0,0));
	return -1;
    }

    *NAND_CTRL_REG = NAND_CTRL_CS|chip;

    *NAND_CMD_REG = 0x90;
    *NAND_ADDR_REG = 0;

    udelay(1);

    for(ix = 0; ix < 5; ix++)
	buf[ix] = *NAND_DATA_BYTE;

    *NAND_CTRL_REG = 0;
    return 0;
}

/******************************************************************************/

/* library initialization routine */

int nandInit(void)
{
    if(nandInitDone)
	return 0;

#if OS_VERSION >= 300
    if(sem_init(&nandSem,1,0) == -1)
	return -1;
#else
    if(sem_init(&nandSem,0,0) == -1)
	return -1;
#endif
    intConnect(IV_NAND,(void *)nandIntr,0);

    nandInitDone = TRUE;

    return 0;
}

