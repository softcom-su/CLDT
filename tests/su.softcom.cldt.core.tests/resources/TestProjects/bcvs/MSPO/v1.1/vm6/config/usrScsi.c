/* usrScsi.c - example of SCSI initialization */

#include <system.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scsiLib.h"
#include "am53c974.h"

/* locals */

#define	INCLUDE_SCSI_TEST

#ifdef	INCLUDE_SCSI_TEST
static SCSI_DEVICE* sd[16];
static BLK_DEV* _bd[16];
#endif

/******************************************************************************/

/* host adapter initialize routine, called from boardHwInit3() */

STATUS sysScsiInit()
{
    if(!(pSysScsiHost = am53c974HostCreate(0,BTM406_ID,
	    BTM406_IO_PRI,BTM406_BQ_PRI)))
	return -1;
    return 0;
}

/******************************************************************************/

/* set unrestricted reordering allowed queue algorithm modifier */

STATUS scsiReorderEnable(SCSI_DEVICE* device, int save)
{
    int result = -1; unsigned char buf[32]; unsigned len;

    if(scsiModeSense(device,TRUE,0xa,buf,1) != -1)
    {
	/* get allocation length */

	len = buf[0] + 1;

	if(len > 32)
	    goto done;

	/* get control mode page changeable values */ 

	if(scsiModeSense(device,TRUE,0x4a,buf,len) == -1)
	    goto done;

	/* drive cannot change queue algorithm ? */

	if(!(buf[7] & 0xf0))
	    goto done;

	/* get control mode page current values */ 

	if(scsiModeSense(device,TRUE,0xa,buf,len) == -1)
	    goto done;

	/* unrestricted reordering allowed already set */ 

	if(!(buf[7] & 0xf0) == 0x10)
	{
	    result = 0; goto done;
	}

	/* clear reserved fields, set modifier */

	buf[0] = 0;
	buf[4] &= ~0x80;
	buf[7] = (buf[7] & 0x0f) | 0x10;

	/* set modifier */

	result = scsiModeSelect(device,1,save,buf,len);
    }

done:
    return result;
}

/******************************************************************************/

#define	SCSI_INIT(x)							   \
do {									   \
    if(!(sdev = scsiDevCreate(pSysScsiHost,SCSI_MAN_ID##x,SCSI_MAN_LUN##x, \
	SCSI_MAN_DISCONNECT##x,SCSI_MAN_SYNC##x,SCSI_MAN_WIDE##x,	   \
	SCSI_MAN_TAGGED##x,SCSI_MAN_TOUT##x,SCSI_MAN_SPINUP##x)))	   \
    {									   \
	printf("usrScsiConfig: scsiDevCreate %i %i failed",		   \
	    SCSI_MAN_ID##x,SCSI_MAN_LUN##x);				   \
	break;								   \
    }									   \
									   \
    if(!(io = scsiBlkDevInstall(sdev,0,0,SCSI_MAN_NAME##x,		   \
	SCSI_MAN_MBR##x)))		   				   \
    {									   \
	printf("usrScsiConfig: scsiBlkDevInstall %i %i failed",		   \
	    SCSI_MAN_ID##x,SCSI_MAN_LUN##x);				   \
	break;								   \
    }									   \
									   \
    if(SCSI_MAN_TAGGED##x)					    	   \
	scsiReorderEnable(sdev,FALSE);				   	   \
									   \
} while(0);

/******************************************************************************/

/* configure scsi devices, called from boardHwInit3() */


STATUS usrScsiConfig()
{
    BLK_IO_DEV* io; SCSI_DEVICE* sdev;

    /* sysScsiInit() failed ? */

    if(!pSysScsiHost)
    {
	printf("usrScsiConfig: pSysScsiHost not initialized\n\r"); return -1;
    }

#ifdef	SCSI_AUTO_CONFIG
    {
	char name[32]; int id, lun;

	/* autoconfigure scsi bus */

	scsiDebugSet(0);  scsiIntsDebugSet(0);
	scsiExtAutoConfig(pSysScsiHost,SCSI_AUTO_LUNS,SCSI_AUTO_DISCONNECT,
	    SCSI_AUTO_SYNC,SCSI_AUTO_WIDE,SCSI_AUTO_TAGGED,SCSI_AUTO_TOUT,
	    SCSI_AUTO_SPINUP);
	scsiDebugSet (1);  scsiIntsDebugSet (1);


	for(id = 0; id < pSysScsiHost->wide; id++)
	{
	    for(lun = 0; lun < 8; lun++)
	    {
		if((sdev = scsiDevDescGet(pSysScsiHost,id,lun)))
		{
		    if(SCSI_AUTO_LUNS)
		    {
			sprintf(name,"%s%i_%i",SCSI_AUTO_NAME,id,lun);
		    }
		    else
		    {
			sprintf(name,"%s%i",SCSI_AUTO_NAME,id);
		    }

		    if(!(io = scsiBlkDevInstall(sdev,0,0,name,SCSI_AUTO_MBR)))
		    {
			printf("usrScsiConfig: scsiBlkDevInstall %i %i "
			    "failed",id,lun);
		    }

//		    if(io->bd->bd_removable)
//			io->bd->bd_statusChk(io->bd);

		    if(SCSI_AUTO_TAGGED)
			scsiReorderEnable(sdev,FALSE);

#ifdef	INCLUDE_SCSI_TEST
		    sd[id] = sdev; _bd[id] = io->bd;
#endif
		}

		if(!SCSI_AUTO_LUNS)
		    break;
	    }
	}
    }
#else	/* SCSI_AUTO_CONFIG */

  #ifdef	SCSI_MAN_CONFIG0
    SCSI_INIT(0);
  #endif
  #ifdef	SCSI_MAN_CONFIG1
    SCSI_INIT(1);
  #endif
  #ifdef	SCSI_MAN_CONFIG2
    SCSI_INIT(2);
  #endif
  #ifdef	SCSI_MAN_CONFIG3
    SCSI_INIT(3);
  #endif
  #ifdef	SCSI_MAN_CONFIG4
    SCSI_INIT(4);
  #endif
  #ifdef	SCSI_MAN_CONFIG5
    SCSI_INIT(5);
  #endif
  #ifdef	SCSI_MAN_CONFIG6
    SCSI_INIT(6);
  #endif
#endif	/* SCSI_AUTO_CONFIG */


    /* show information about attached scsi devices */

    scsiShow(pSysScsiHost);

    return 0;
}


#ifdef	INCLUDE_SCSI_TEST

#include <taskLib.h>
#include <blkDevTest.h>

#define	TEST_TIME	10

static char multbuf[0x100000];

int mrd(int id, int mult, int nsec)
{
    int sec = 0;
    if(!nsec)
	nsec = 1;
    if(!mult)
	mult = 2;
    while(mult--)
    {
	sp(scsiRead,sd[id],sec,nsec,multbuf);
	sec += 10000;
    }
}

int scsiSpeed(int id, int rw, int mode)
{
    int xfer; int xferMax = 8192;

    mode |= BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;

    if(mode & BDS_BUFFER)
	xferMax = 256;

    for(xfer = 1; xfer <= xferMax; xfer <<= 1)
    {   //  unsigned t0=time(0), tick0=boardTickGet();
	blkDevSpeed(_bd[id],xfer,mode,TEST_TIME);
        // printf ("%i seconds  %i ticks\n\r", time(0)-t0, boardTickGet()-tick0);
    }
    return 0;
}
void spScsiSpeed(int pri, int id, int rw, int blkSize, int time)
{
    int mode = BDS_TIME;

    if(rw)
	mode |= BDS_WRITE;
    
    taskSpawn(NULL,pri,0,16384,(FUNCPTR)blkDevSpeed,(int)_bd[id],blkSize,
	mode,time,0,0,0,0,0,0);
}
STATUS scsiTest(int id, unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    return blkDevTestShow(_bd[id],blkSize,blkCount,buf,ignore,"SCSI read/write test ... ");
}
void spScsiTest(int pri, int id, unsigned blkSize, unsigned blkCount, char* buf, int ignore)
{
    taskSpawn(NULL,pri,0,16384,scsiTest,id,blkSize,blkCount,(int)buf,ignore,0,0,0,0,0);
}
int scsiExtTest(int id, unsigned blkSize, unsigned blkCount, unsigned pattern,
    unsigned wrOrder, unsigned rdOrder, unsigned* buf,
    unsigned options, unsigned offset)
{
    if(!blkSize)
	printf("%s ",__FUNCTION__);
    return blkDevExtTest("SCSI",_bd[id],blkSize,blkCount,pattern,wrOrder,
	rdOrder,buf,options,offset);
}
STATUS scsiRd(int id, int sec, int nsec, char* buf)
{
    return _bd[id]->bd_blkRd(_bd[id],sec,nsec,buf);
}

STATUS scsiWr(int id, int sec, int nsec, char* buf)
{
    return _bd[id]->bd_blkWrt(_bd[id],sec,nsec,buf);
}
STATUS scsiChk(int id)
{
    if(!_bd[id]->bd_statusChk)
	return -1;
    return _bd[id]->bd_statusChk(_bd[id]);
}

#endif	/* INCLUDE_SCSI_TEST */
