/* usrLpt.c - example of LPT initialization */

#include <system.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <lptBaget.h>
#ifdef	INCLUDE_LS120
  #include <pf.h>
#endif
/******************************************************************************/

/* low-level controller initialization, called from boardHwInit3() */

/*
    For printer and LS-120 together: lptBagetCtrlInit(0,TRUE,C_EPP,-1)
    For LS-120 only:                 lptBagetCtrlInit(0,FALSE,C_EPP,-1)
    For printer only without DMA:    lptBagetCtrlInit(0,FALSE,C_SPP,-1)
    For printer only with DMA:       lptBagetCtrlInit(0,FALSE,C_ECP,ECP_PPF)
*/

STATUS sysLptInit()
{
#ifdef	BTM405_LPT_MAN
    if(!(pSysLptCtrl = lptBagetCtrlInit(0,BTM405_LPT_SHARED, BTM405_LPT_MODE|
       BTM405_LPT_PCI|BTM405_LPT_RFIFO|BTM405_LPT_WFIFO|BTM405_LPT_EPP_DELAY|
       BTM405_LPT_EPP_17|BTM405_LPT_ISA_DELAY, BTM405_LPT_ECP)))
    {
	return -1;
    }
#else	/* BTM405_LPT_MAN */
#if defined(INCLUDE_LS120) && defined (INCLUDE_PRINTER)
    if(!(pSysLptCtrl = lptBagetCtrlInit(0,TRUE,C_EPP,-1)))
	return -1;
#elif defined(INCLUDE_LS120)
    if(!(pSysLptCtrl = lptBagetCtrlInit(0,FALSE,C_EPP,-1)))
	return -1;
#elif defined(INCLUDE_PRINTER)
    if(!(pSysLptCtrl = lptBagetCtrlInit(0,FALSE,C_ECP,ECP_PPF)))
	return -1;
#else
    if(!(pSysLptCtrl = lptBagetCtrlInit(0,FALSE,C_SPP,-1)))
	return -1;
#endif
#endif	/* BTM405_LPT_MAN */
    return 0;
}


/* globals */

#ifdef	INCLUDE_LS120
#ifdef	INCLUDE_PF_TEST
static BLK_DEV* pPfBd;		/* pointer to a block device */
#endif
#endif	/* INCLUDE_LS120 */

/******************************************************************************/

/* printer and LS120 initialization, called from boardHwInit3() */

STATUS usrLptConfig()
{
#ifdef	INCLUDE_LS120
    PF_UNIT* pf; BLK_IO_DEV* io;
#endif	/* INCLUDE_LS120 */

#ifdef	INCLUDE_PRINTER
    LPT_CONFIG config;
#endif	/* INCLUDE_PRINTER */

    if(!pSysLptCtrl)
    {
	printf("usrLptConfig: sysLptInit failed\n\r");
	return -1;
    }

#ifdef	INCLUDE_PRINTER

    /* create printer device */

    if(lptDevCreate(PRN_CTRL,PRN_NAME,LPT_THREAD_PRI) == -1)
    {
	printf("usrLptConfig: lptDevCreate failed\n\r");
	return -1;
    }

    /* open printer file */

    if((lptFd = open(PRN_NAME,O_WRONLY,0)) == -1)
    {
	printf("usrLptConfig: cannot open file %s %i\n\r",PRN_NAME,errno);
	return -1;
    }
    /* get driver configuration */

    if(ioctl(lptFd,LPT_CONFIG_GET,(int)&config) == -1)
    {
	printf("usrLptConfig: ioctl LPT_CONFIG_GET error %i\n\r",errno);
	return -1;
    }

    /* tune driver configuration */

#ifdef	PRN_MAN
    config.userRtn	= PRN_RTN;
    config.userArg	= (int)(PRN_ARG);
    config.sppCR	= PRN_CR;
    config.lptMode	= PRN_LPT_MODE;
    config.ecpMode	= PRN_ECP_MODE;
    config.pendTout	= PRN_PEND;
    config.pendStep	= PRN_STEP;
    config.busyTout	= PRN_BUSY;
    config.busyCount	= PRN_BUSY_CNT;
    config.selectCount	= PRN_SELECT_CNT;
    config.fifoCount	= PRN_FIFO_CNT;
    config.dmaTout	= PRN_DMA_TOUT;
    config.dmaThreshold = PRN_DMA_THR;
    config.dmaEnable	= PRN_DMA;
#endif	/* PRN_MAN */

    /* set driver configuration */

    if(ioctl(lptFd,LPT_CONFIG_SET,(int)&config) == -1)
    {
	printf("usrLptConfig: ioctl LPT_CONFIG_SET error %i\n\r",errno);
	return -1;
    }
#endif	/* INCLUDE_PRINTER */

#ifdef	INCLUDE_LS120
    /* create a LS-120 device structure */

    if(!(pf = pfDevCreate(PF_CTRL,PF_MODE,PF_UNIT_,PF_PROTO,PF_DELAY,PF_DRIVE,
	PF_LUN,PF_CLUSTER,PF_NICE,PF_VERBOSE,PF_PRI)))
    {
	printf("usrLptConfig: pfDevCreate failed\n\r");
	return -1;
    }

    /* create a block device on LS-120 */

    if(!(io = pfBlkDevInstall(pf,0,0,PF_NAME)))
    {
	printf("usrLptConfig: pfBlkDevCreate failed\n\r");
	return -1;
    }

#ifdef	INCLUDE_PF_TEST
    pPfBd = io->bd;
#endif

#endif	/* INCLUDE_LS120 */

    return 0;
}


#ifdef	INCLUDE_PF_TEST

#include <taskLib.h>
#include <blkDevTest.h>

#define	TEST_TIME	10

int pfSpeed(int rw, int mode)
{
    int xfer; int xferMax = 128;

    mode |= BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;

    if(mode & BDS_BUFFER)
	xferMax = 64;

    for(xfer = 1; xfer <= xferMax; xfer <<= 1)
    {
	blkDevSpeed(pPfBd,xfer,mode,TEST_TIME);
    }
    return 0;
}

void spPfSpeed(int pri, int rw, int blkSize, int time)
{
    int mode = BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;
    
    taskSpawn(NULL,pri,0,16384,(FUNCPTR)blkDevSpeed,(int)pPfBd,blkSize,
	mode,time,0,0,0,0,0,0);
}

STATUS pfTest(unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    printf("LS-120 read/write test ... "); fflush(stdout);
    return blkDevTest(pPfBd,blkSize,blkCount,buf,ignore);
}

void spPfTest(int pri, unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    taskSpawn(NULL,pri,0,16384,pfTest,blkSize,blkCount,(int)buf,ignore,0,0,0,0,0,0);
}

STATUS pfRd(int sect, int nsec, char* buf)
{
    return pPfBd->bd_blkRd(pPfBd,sect,nsec,buf);
}
STATUS pfWr(int sect, int nsec, char* buf)
{
    return pPfBd->bd_blkWrt(pPfBd,sect,nsec,buf);
}
STATUS pfReset()
{
    return pPfBd->bd_reset(pPfBd);
}
STATUS pfStatus()
{
    return pPfBd->bd_statusChk(pPfBd);
}

#endif	/* INCLUDE_PF_TEST */

#ifdef BT202
/***************************************************************************
*
* Old LPT driver config
*
****************************************************************************/

#include "lpt.h"

STATUS usrLptOldConfig() {
    lpt_init();

#if BTM405_LPT_DRV_RW_MODE==SPP_LPT_MODE
    if (lptsppDevCreate(BTM405_LPT_DRV_RW_NAME,BTM405_LPT_DRV_RW_SLOT,
	BTM405_LPT_DRV_RW_SPPBUFFER,BTM405_LPT_DRV_RW_THREAD_PRI) == NULL)
	return -1;
#endif

#if BTM405_LPT_DRV_RW_MODE==EPP_LPT_MODE
    if (lpteppDevCreate(BTM405_LPT_DRV_RW_NAME,BTM405_LPT_DRV_RW_SLOT,
	BTM405_LPT_DRV_RW_ISA_DELAY,BTM405_LPT_DRV_RW_EPP_DELAY,BTM405_LPT_DRV_RW_EPP_TYPE,
  #ifdef BTM405_LPT_DRV_RW_REPEAT
	 LPT_EPP_REPEAT
  #else
         0
  #endif
       ) == NULL)
	return -1;
#endif

#if BTM405_LPT_DRV_RW_MODE==ECP_LPT_MODE
    if (lptecpDevCreate(BTM405_LPT_DRV_RW_NAME,BTM405_LPT_DRV_RW_SLOT,
	BTM405_LPT_DRV_RW_WISE_USER ? (BTM405_LPT_DRV_RW_ECP|LPT_ECP_WISE_USER) : BTM405_LPT_DRV_RW_ECP,
	BTM405_LPT_DRV_RW_ISA_DELAY,BTM405_LPT_DRV_RW_EPP_DELAY,BTM405_LPT_DRV_RW_BUFFER,
	BTM405_LPT_DRV_RW_REPEAT ? LPT_ECP_REPEAT : 0,
	((BTM405_LPT_DRV_RW_CTRL_INIT ? CTRL_INIT : 0) |
	 (BTM405_LPT_DRV_RW_CTRL_SELECT ? CTRL_SELECT : 0) |
	 (BTM405_LPT_DRV_RW_CTRL_IRQ_ENABLE ? CTRL_IRQ_ENABLE : 0))) == NULL)
	return -1;
#endif

    return 0;
}
#endif
