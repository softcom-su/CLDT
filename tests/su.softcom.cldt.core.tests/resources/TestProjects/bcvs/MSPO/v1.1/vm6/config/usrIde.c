/* usrIde.c - example of IDE initialization */

#include <system.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <blkIo.h>
#include <ideDrv.h>

#ifdef	__INCLUDE_BTM405_IDE
#include <baget.h>
#endif
#ifdef	__INCLUDE_CMD64X_IDE
#include <cmd64x.h>
#endif
#ifdef  BT_UZPI
#include <uzpi.h>
#endif

#define  INCLUDE_IDE_TEST

/******************************************************************************/

/* ide controllers ptrs */

IDE_CTRL* sysIdeCtrl[IDE_MAX_CTRL] = { NULL, NULL, };

/******************************************************************************/

/* baget timings (modify corresponding with attached drives) */

#ifdef	__INCLUDE_BTM405_IDE

BAGET_TIMINGS bagetTimings[BAGET_MAX_CTRL] = {
    {	/* controller 0 */

	BTM405_CTRL0_CH0_PIO,	/* channel 1 PIO */
	BTM405_CTRL0_CH0_DMA,	/* channel 1 DMA */
	BTM405_CTRL0_CH1_PIO,	/* channel 2 PIO */
	BTM405_CTRL0_CH1_DMA	/* channel 2 DMA */
    },
    {	/* controller 1 */

	BTM405_CTRL1_CH0_PIO,	/* channel 1 PIO */
	BTM405_CTRL1_CH0_DMA,	/* channel 1 DMA */
	BTM405_CTRL1_CH1_PIO,	/* channel 2 PIO */
	BTM405_CTRL1_CH1_DMA	/* channel 2 DMA */
    }
};

#endif	/* __INCLUDE_BTM405_IDE */

/******************************************************************************/

/* controller(s) initialization routine, called from boardHwInit3() */
extern int ideDebug;
STATUS sysIdeInit()
{
//     ideDebug=3;
#ifdef IDE_SLOW_IO
    ideSlowIOSet (1);   /* default value = 1 - slow handling of IDE-registers */ 
#else
    ideSlowIOSet (0);
#endif
//    ideDebugSet (5);
#ifdef	__INCLUDE_BTM405_IDE
    if(ideBagetCtrlInit(IDE_RESET,IDE_PRI) == -1)
	return -1;
#endif	/* __INCLUDE_BTM405_IDE */
#ifdef	__INCLUDE_CMD64X_IDE
    if(!(sysIdeCtrl[CMD64X_CTRL] = cmd64xCtrlInit(CMD64X_UNIT,
	IDE_RESET,IDE_PRI,CMD64X_PIO)))
    {
	return -1;
    }
#endif	/* __INCLUDE_CMD64X_IDE */
#ifdef  BT_UZPI
    if(!(sysIdeCtrl[0] = uzpiIdeCtrlInit(IDE_RESET,IDE_PRI)))
    {
        return -1;
    }
#endif  /* BT_UZPI */
    return 0;
}

#ifdef	INCLUDE_IDE_TEST
static BLK_DEV* pIdeBd;
#endif
//=====================================================================================	
#define	IDE_INIT(x)						   		\
{										\
    /* create block device */					  		\
										\
    if(!(io = _ideBlkDevInstall(sysIdeCtrl[IDE_CTRL##x*2 + IDE_CHANNEL##x],	\
	    IDE_DRIVE##x,0,0,IDE_NAME##x,IDE_DMA##x,IDE_MBR##x)))		\
    {										\
	printf("usrIdeConfig: _ideBlkDevInstall %i %i %i %s %i %i failed\n\r",	\
	    IDE_CTRL##x,IDE_CHANNEL##x,IDE_DRIVE##x,IDE_NAME##x,IDE_DMA##x,	\
	    IDE_MBR##x);	    						\
	return -1;								\
    }										\
										\
    bd = io->bd;								\
}
/******************************************************************************/

/* file sytem initialization on IDE drive, called from boardHwInit3() */

extern int ideSlowIO;

STATUS usrIdeConfig()
{
    BLK_DEV* bd; BLK_IO_DEV* io;

//printf("ideSlowIO = %d",ideSlowIO);
#ifdef	IDE_PARAMS0
    IDE_INIT(0);
#endif
#ifdef	IDE_PARAMS1
    IDE_INIT(1);
#endif
#ifdef	IDE_PARAMS2
    IDE_INIT(2);
#endif
#ifdef	IDE_PARAMS3
    IDE_INIT(3);
#endif

#ifdef IDE_TEST_FOR_KRYN
if (bd == NULL)
    bd = (BLK_DEV*)ideDevCreateTest();
#endif

#ifdef	INCLUDE_IDE_TEST
    pIdeBd = bd;

#ifdef IDE_POLL_ENABLE
#warning !!!!!!!!!! usrIdeConfig: IDE_IOCTL_POLL_ENABLE !!!!!!!!!!!!
    pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_POLL_ENABLE,0);
#endif

//pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_WCACHE_ENABLE,0);

#endif

    return 0;
}

#ifdef	INCLUDE_IDE_TEST

#include <taskLib.h>
#include <blkDevTest.h>

#define	TEST_TIME	10

int ideSpeed(int rw, int mode)
{
    int xfer; int xferMax = 256;

    mode |= BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;

    if(mode & BDS_BUFFER)
	xferMax = 256;

    for(xfer = 1; xfer <= xferMax; xfer <<= 1)
    {
#ifdef	INCLUDE_IDLE
	idleStart();
#endif	/* INCLUDE_IDLE */
	blkDevSpeed(pIdeBd,xfer,mode,TEST_TIME);
#ifdef	INCLUDE_IDLE
	idleShow();
#endif	/* INCLUDE_IDLE */
    }
    return 0;
}

void spIdeSpeed(int pri, int rw, int blkSize, int time)
{
    int mode = BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;
    
    taskSpawn(NULL,pri,0,16384,(FUNCPTR)blkDevSpeed,(int)pIdeBd,blkSize,
	mode,time,0,0,0,0,0,0);
}

int ideTest(unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    return blkDevTestShow(pIdeBd,blkSize,blkCount,buf,ignore,"Ide read/write test ... ");
}

int ideExtTest(unsigned blkSize, unsigned blkCount, unsigned pattern,
    unsigned wrOrder, unsigned rdOrder, unsigned* buf,
    unsigned options, unsigned offset)
{
    if(!blkSize)
        printf("%s ",__FUNCTION__);
    return blkDevExtTest("Ide",pIdeBd,blkSize,blkCount,pattern,wrOrder,
    	rdOrder,buf,options,offset);
}

#ifdef IDE_TEST_FOR_KRYN
int ideExtTestSimple(unsigned blkSize, unsigned blkCount, unsigned pattern)
{
    if(!blkSize)
        printf("%s ",__FUNCTION__);
    return blkDevExtTestSimple("Ide",pIdeBd,blkSize,blkCount,pattern);
}
#endif
			 
void spIdeTest(int pri, unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    taskSpawn(NULL,pri,0,16384,(FUNCPTR)ideTest,blkSize,blkCount,(int)buf,ignore,0,0,0,0,0,0);
}
static void *pbufr=NULL, *pbufw=NULL;
static int nsbufr=0, nsbufw=0;
STATUS ideRd(int sect, int nsec, char* buf)
{
    if (buf == NULL)
      { if (pbufr != NULL && nsbufr < nsec)
          { free (pbufr); pbufr = NULL;
          }
        if (pbufr == NULL)
          { if ((pbufr = malloc(nsec*512)) == NULL) return -1;
            nsbufr = nsec;
          }
        buf = pbufr;
      }
    return pIdeBd->bd_blkRd(pIdeBd,sect,nsec,buf);
}
STATUS ideWr(int sect, int nsec, char* buf)
{
    if (buf == NULL)
      { if (pbufw != NULL && nsbufw < nsec)
          { free (pbufw); pbufw = NULL;
          }
        if (pbufw == NULL)
          { if ((pbufw = calloc(nsec*512,1)) == NULL) return -1;

            nsbufw = nsec;
          }
        buf = pbufw;
      }
    return pIdeBd->bd_blkWrt(pIdeBd,sect,nsec,buf);
}
int ideFill (int mode, unsigned code)
{ int i; unsigned *p = (unsigned *)pbufw;
    if (pbufw == NULL) return -1;
    if (mode == 0)
        for (i=0; i < nsbufw * 512 / 4; i++)
            *p++ = code;
    else
        for (i=0; i < nsbufw * 512 / 4; i++)
            *p++ = code + i;
    return 0;
}
int ideCheck (int nsec)
{ unsigned *pr=(unsigned *)pbufr, *pw=(unsigned *)pbufw;
  int i, n = nsec * (512/4);
  for (i=0; i < n; i++)
     { if (*pr++ != *pw++)
         printf ("i=%i read %x write %x\n\r", i, *(pr-1), *(pw-1));
     }
  return 0;
}

STATUS ideReset()
{
    return pIdeBd->bd_reset(pIdeBd);
}
STATUS ideSleep()
{
    return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_SLEEP,0);
}
STATUS ideStandby()
{
    return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_STANDBY,0);
}
STATUS ideIdle()
{
    return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_IDLE,0);
}
STATUS ideMode(int mode)
{
    return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_IO_MODE_SET,mode);
}
STATUS ideDma(int enable)
{
    if(enable)
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_DMA_ENABLE,0);
    else
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_DMA_DISABLE,0);
}
STATUS ideMult(int enable)
{
    if(enable)
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_MULT_ENABLE,0);
    else
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_MULT_DISABLE,0);
}
STATUS idePoll(int enable)
{
    if(enable)
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_POLL_ENABLE,0);
    else
	return pIdeBd->bd_ioctl(pIdeBd,IDE_IOCTL_POLL_DISABLE,0);
}

/******************************************************************************/

#define	HZ	boardClkRateGet()   // boardTimerHZ
#define	TICK	tickGet()

static int testIdeSpeed(int testId, unsigned xfer, int mode, unsigned stop,int sec,int nf)
{
    int byte,kb; char* buf; unsigned startTick, stopTick; int rw;

    byte = xfer*sec;
    
//    printf("testId = %d xfer = %d mode = %d stop = %d sec = %d byte = %d\n",
//            testId,xfer,mode,stop,sec,byte);
    
    
    if(!(buf = (char*)malloc(byte)))
    {
	printf("Not enought memory\n"); return -1;
    }

    startTick = TICK; rw = (mode & BDS_WRITE) != 0;
  
    stop = stop*HZ + startTick; kb = 0;

//    printf("startTick = %d stop = %d\n",startTick,stop);

    while(1)
    {
        if(TICK >= stop)	break;
	if(rw)
	  {
           if(write(testId,buf,byte) != byte)
	     {
	        printf("test write error (%1d)\n",nf);
		return -1;
	     }	   
	   //sync();
	  }
	else
	  {
           if(read(testId,buf,byte) != byte)
	     {
	        printf("test read error (%1d)\n",nf);
		return -1;
	     }	
	  }
        kb += byte;
    }

    stopTick = TICK;

    printf("%s %s %i KB/S (block size %i sectors)(%i byte)(%1d)\n", (mode & BDS_BUFFER) ?
    "Buffer" : ((mode & BDS_RANDOM) ? "Random" : "Sequental"), rw ? "write" :
    "read", (((kb) / 1024) * HZ) / (stopTick - startTick),xfer, byte,nf);

    free(buf);
    return 0;
}


int ideSpeed1(int rw, int mode,int nf)
{
    int xfer; int xferMax = 256;int testId = 0;
    char ms[25];

    mode |= BDS_TIME;

    bzero(ms,sizeof(ms));
    sprintf(ms,"/vfat/test%1d.dan",nf);

    if(rw)
      {
       mode |= BDS_WRITE;

       remove(ms);
       sync();

       testId = creat (ms,0777);	 
      }	 
    else
      {
       testId = open (ms,0x0,0);
      }
 
    if(testId == -1)
      {
       printf("test error (%1d)\n",nf);
       return -1;  
      } 
//    printf("testId = %d\n",testId);

  taskDelay(boardClkRateGet() * 2);
      
  for(xfer = 1; xfer <= xferMax; xfer <<= 1)
    {
        if(!rw)	lseek(testId,SEEK_SET,0);
//	lseek(testId,SEEK_SET,0);
	if(testIdeSpeed(testId,xfer,mode,TEST_TIME,pIdeBd->bd_bytesPerBlk,nf) == -1) break;

    }
    close(testId);
    return 0;
}


void spIdeSpeed1(int rw, int mode, int ts)
{
int i;

for(i = 0;i < ts;i ++)
  {
    taskSpawn(NULL,100,0,16384,(FUNCPTR)ideSpeed1,rw,mode,i,0,0,0,0,0,0,0);  
//    taskDelay(boardClkRateGet());
  }
}

void ideTestRemove()
{
int i;
char ms[25];

for(i = 0;i < 9;i ++)
  {
    bzero(ms,sizeof(ms));
    sprintf(ms,"/vfat/test%1d.dan",i);  
    remove(ms);
  } 
sync();      
}
/******************************************************************************/

#endif	/* INCLUDE_IDE_TEST */
