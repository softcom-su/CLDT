/* usrFtl.c - example of FTL initialization */

#include <system.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <blkIo.h>
#include <ftlDrv.h>

#ifdef	INCLUDE_FTL_TEST
static BLK_DEV* pFtlBd;
#endif

#define	FTL_NAME	"/dev/flash"
#define	FTL_OFFSET	0x400000
#define	FTL_PRI		(-1)

/******************************************************************************/

/* file sytem initialization on FTL, called from boardHwInit3() */

STATUS usrFtlConfig()
{
    BLK_IO_DEV* io;

    /* sysFlashInit() failed ? */

    if(!pSysFlashDesc)
    {
	printf("usrFtlConfig: pSysFlashDesc not initialized\n\r");
	return -1;
    }

    /* format partition */
/*
    if(ftlFormat(pSysFlashDesc,FTL_OFFSET,0x200000,2,10,2) == -1)
    {
	printf("usrFtlConfig: ftlFormat failed\n\r"); return -1;
    }
*/
    /* create FTL device */

    if(!(pSysFtlDev = ftlDevInit(pSysFlashDesc,FTL_OFFSET,FTL_PRI)))
    {
	printf("usrFtlConfig: ftlDevInit failed\n\r"); return -1;
    }

    /* create block device */

    if(!(io = ftlBlkDevInstall(pSysFtlDev,0,0,FTL_NAME)))
    {
	printf("usrFtlConfig: ftlBlkDevInstall failed\n\r"); return -1;
    }

#ifdef	INCLUDE_FTL_TEST
    pFtlBd = io->bd;
#endif

    return 0;
}

#ifdef	INCLUDE_FTL_TEST

#include <taskLib.h>
#include <blkDevTest.h>

#define	TEST_TIME	10

int ftlSpeed(int rw, int mode)
{
    int xfer; int xferMax = 32;

    mode |= BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;

    if(mode & BDS_BUFFER)
	xferMax = 256;

    for(xfer = 1; xfer <= xferMax; xfer <<= 1)
    {
	blkDevSpeed(pFtlBd,xfer,mode,TEST_TIME);
    }
    return 0;
}

void spFtlSpeed(int pri, int rw, int blkSize, int time)
{
    int mode = BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;
    
    taskSpawn(NULL,pri,0,16384,(FUNCPTR)blkDevSpeed,(int)pFtlBd,blkSize,
	mode,time,0,0,0,0,0,0);
}

STATUS ftlTest(unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    printf("Ftl read/write test ... "); fflush(stdout);
    return blkDevTest(pFtlBd,blkSize,blkCount,buf,ignore);
}

void spFtlTest(int pri, unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    taskSpawn(NULL,pri,0,16384,ftlTest,blkSize,blkCount,(int)buf,ignore,0,0,0,0,0,0);
}

STATUS ftlRd(int sect, int nsec, char* buf)
{
    return pFtlBd->bd_blkRd(pFtlBd,sect,nsec,buf);
}
STATUS ftlWr(int sect, int nsec, char* buf)
{
    return pFtlBd->bd_blkWrt(pFtlBd,sect,nsec,buf);
}
STATUS ftlPrepare()
{
    return pFtlBd->bd_ioctl(pFtlBd,FTL_IOCTL_PREPARE,0);
}

#endif	/* INCLUDE_FTL_TEST */
