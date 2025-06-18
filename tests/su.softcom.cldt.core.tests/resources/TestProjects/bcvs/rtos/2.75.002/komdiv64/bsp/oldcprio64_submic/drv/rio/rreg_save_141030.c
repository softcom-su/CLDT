#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arch/interrupt.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sched.h>
#include <pthread.h>

#include <logLib.h>
#include <cpuLib.h>
#include <board.h>
#include <rio.h>

#include "rreg.h"

/******************************************************************************/

/* debug print */

#define RREG_DEBUG(level,msg)           \
{                                       \
    if(rregDebug >= (level))            \
    {                                   \
        logMsg msg;                     \
    }                                   \
}

/* check params */

#define	RREG_ID_CHECK(action)								\
    do											\
    {											\
	if(id >= rregIdMax)								\
	{										\
	    RREG_DEBUG(1,("%s: invalid node=%x\n",(long)__FUNCTION__,id,0,0,0,0));	\
	    action ;									\
	}										\
    } while(0);


#define	RREG_DB_CHECK(action)								\
    do											\
    {											\
	if(db >= RREG_DB_MAX)								\
	{										\
	    RREG_DEBUG(1,("%s: invalid db=%x\n",(long)__FUNCTION__,db,0,0,0,0));		\
	    action ;									\
	}										\
    } while(0);

#define	RREG_CHECK(action)	\
    RREG_ID_CHECK(action)	\
    RREG_DB_CHECK(action)

/* doorbell values */

#define RREG_DB_ALIVE           0xffe

#define RREG_DB_ALIVE_ACKB      0xf00
#define RREG_DB_ALIVE_ACKE      0xf0f
#define RREG_DB_ALIVE_ACK2B     0xf10
#define RREG_DB_ALIVE_ACK2E     0xf1f

#define RREG_DB_RESYNC          0xffb
#define RREG_DB_SHUTDOWN        0xffa

/* maintenance values */

#define	RREG_TAG_MAGIC		0x579d0000
#define	RREG_TAG_INVALID	0
#define	RREG_TAG_READY		1
#define	RREG_TAG_POLL		2
#define	RREG_TAG_DONE		3
#define	RREG_TAG_MAP		4
#define	RREG_TAG_SYNC		5
#define	RREG_TAG_RESP		0x80

/* poll timeout in ms */

#define	RREG_POLL_TOUT		100

/* connection state */

typedef struct {
    volatile unsigned char   rxReady;        /* rx connection state */
    volatile unsigned char   txReady;        /* tx connection state */
    volatile unsigned char   ready;          /* ready flag */
} RREG_STATE;


/* forward declaration */

static unsigned rregDbOut(unsigned id, unsigned db, unsigned dbVal);

/******************************************************************************/

/* globals */

int rregDebug = 1;                      /* debug level */
int rregIdMax = 0;                      /* number of id's'*/

/* locals */

static mqd_t rregMq;
static RREG_STATE rregState[RREG_DB_MAX][256];
static void (*rregReadyHook)(int rio_id, unsigned db, const char *);
static char rregHostType[256];

/******************************************************************************/

/* (re)synchronize channel */

void rregResync(unsigned id, unsigned db)
{
    RREG_CHECK(return);
    rregState[db][id].ready = FALSE;
    rregState[db][id].rxReady = FALSE;
    rregState[db][id].txReady = FALSE;
    rregDbOut(id,db,RREG_DB_ALIVE);
}

/******************************************************************************/

/* delayed resynchronize channel via message queue */

int rregResyncPost(unsigned id, unsigned db)
{
    unsigned val = RREG_DB_RESYNC | (id << 16) | (db << 12);

    RREG_CHECK(return EINVAL);

    if(mq_send(rregMq,(char*)&val,4,0) == -1)
    {
        RREG_DEBUG(1,("%s: mq_send failed, errno=%s (%d)\n",(long)__FUNCTION__,
                      (long)strerror(errno),errno,0,0,0));
        return errno;
    }
    return 0;
}

/******************************************************************************/

/* shutdown channel */

void rregShutdown(unsigned id, unsigned db)
{
    RREG_CHECK(return);
    rregState[db][id].ready = FALSE;
    rregState[db][id].rxReady = FALSE;
    rregState[db][id].txReady = FALSE;
#warning test code
//    rregDbOut(id,db,RREG_DB_SHUTDOWN);
}

/******************************************************************************/

/* send doorbell routine */

static unsigned rregDbOut(unsigned id, unsigned db, unsigned dbVal)
{
    unsigned res, debug = (dbVal == RREG_DB_ALIVE) ? 2 : 1;

    if((res = _rioDbOut(id,db,dbVal)))
        goto err;

    return 0;

err:
    RREG_DEBUG(debug,("%s: node=%x db=%x dbVal=%x res=%s (%d)\n",
	(long)__FUNCTION__,id,db,dbVal,(long)strerror(res),res));

    switch(dbVal)
    {
	case RREG_DB_ALIVE:
	case RREG_DB_SHUTDOWN:
	case RREG_DB_RESYNC:
	    return res;
    }
    rregResync(id,db);
    return res;
}

/******************************************************************************/

/* interrupt service routine */

void rregIntr(unsigned db, unsigned dbVal, unsigned id)
{
    unsigned val, debug = 2;

    RREG_CHECK(return);

    switch(dbVal)
    {
        case RREG_DB_ALIVE:
        case RREG_DB_SHUTDOWN:
            rregState[db][id].ready = FALSE;
            rregState[db][id].rxReady = FALSE;
            rregState[db][id].txReady = FALSE;
            break;
        case RREG_DB_ALIVE_ACKB...RREG_DB_ALIVE_ACK2E:
            break;
        default:
            debug = 1;
            goto done;
    }

    val = dbVal | (id << 16) | (db << 12);
    if(mq_send(rregMq,(char*)&val,4,0) == -1)
        RREG_DEBUG(1,("%s: mq_send failed\n",(long)__FUNCTION__,
              0,0,0,0,0));

done:
    RREG_DEBUG(debug,("%s: db=%x dbVal=%03x id=%x\n",(long)__FUNCTION__,db,dbVal,id,0,0));
}

/******************************************************************************/

/* this thread parse synchronization doorbells */

void rregThread(void* hostT)
{
    unsigned val, db, dbVal, id, prio, nErr = 0;
    int hostType = (int)(long)hostT;
    while(1)
    {
        if(mq_receive(rregMq,(char*)&val,4,&prio) == -1)
        {
            RREG_DEBUG(1,("%s: mq_receive failed, errno=%s (%d)\n",(long)__FUNCTION__,
                          (long)strerror(errno),errno,0,0,0));
            if(nErr++ > 16)
                return;
            continue;
        }

        dbVal = val & 0xfff;
        db = (val >> 12) & 0xf;
        id = val >> 16;

        RREG_CHECK(continue);

        switch(dbVal)
        {
            case RREG_DB_ALIVE:
                if(!rregDbOut(id,db,RREG_DB_ALIVE_ACKB + hostType ))
                    rregState[db][id].txReady = TRUE;
                break;
            case RREG_DB_SHUTDOWN:
                break;
            case RREG_DB_ALIVE_ACKB...RREG_DB_ALIVE_ACKE:
                rregState[db][id].rxReady = TRUE;
                if(!rregState[db][id].txReady)
                {
                    if(!rregDbOut(id,db,RREG_DB_ALIVE_ACK2B + hostType ))
                        rregState[db][id].txReady = TRUE;
                }
                if(rregState[db][id].txReady && rregState[db][id].rxReady)
                {
                    RREG_DEBUG(2,("%s: db=%x node=%d ready with ACK\n",(long)__FUNCTION__,db,id,0,0,0));
                    rregState[db][id].ready = TRUE;
                    rregHostType[id] = dbVal & 0xf;
                    if(rregReadyHook && id != boardProcNumGet())
                       rregReadyHook(id,db,__FUNCTION__);
                }
                break;
            case RREG_DB_ALIVE_ACK2B...RREG_DB_ALIVE_ACK2E:
                rregState[db][id].rxReady = TRUE;
                if(rregState[db][id].txReady && rregState[db][id].rxReady)
                {
                    RREG_DEBUG(2,("%s: db=%x node=%d ready with ACK2\n",(long)__FUNCTION__,db,id,0,0,0));
                    rregState[db][id].ready = TRUE;
                    rregHostType[id] = dbVal & 0xf;
                    if(rregReadyHook && id != boardProcNumGet())
                       rregReadyHook(id,db,__FUNCTION__);
                }
                else
                {
                    RREG_DEBUG(1,("%s: db=%x node=%d is not ready with ACK2!!!\n",(long)__FUNCTION__,db,id,0,0,0));
                }
                break;
            case RREG_DB_RESYNC:
                if(rregState[db][id].ready)
                    rregResync(id,db);
                break;
            default:
                rregResync(id,db);
                break;
        }

        nErr = 0;
    }
}

/******************************************************************************/

/* check node ready state */

int rregIsReady(unsigned id, unsigned db)
{
    RREG_CHECK(return FALSE);
    return rregState[db][id].ready;
}

/******************************************************************************/

/* get node host type */

int rregHostTypeGet(unsigned id)
{
    RREG_ID_CHECK(return RIO_HOST_INVALID);
    return rregHostType[id];
}

/******************************************************************************/

/* set doorbell handler */

static int rregHandlerSet(unsigned db, VOIDFUNCPTR rtn, const char* name)
{
    unsigned dbNum;

    switch(db)
    {
	case RREG_DB_NET_IX:
	  dbNum = RREG_DB_NET;
	  break;
	case RREG_DB_CHAN_IX:
	  dbNum = RREG_DB_CHAN;
	  break;
	default:
	  RREG_DEBUG(1,("%s: invalid db=%x\n",(long)name,db,0,0,0,0));
	  return EINVAL;
    }

    rioDbIntConnect(dbNum,rtn,db);
    return 0;
}

int rregDisable(unsigned db)
{
    return rregHandlerSet(db,NULL,__FUNCTION__);
}

int rregEnable(unsigned db)
{
    return rregHandlerSet(db,rregIntr,__FUNCTION__);
}

/******************************************************************************/

/* (re)synchronize all channels */

void rregResyncAll(unsigned db, int forced)
{
    unsigned ix, cpu = boardProcNumGet();
    static int resyncPassed[RREG_DB_MAX];

    RREG_DB_CHECK(return);

    if(!forced && resyncPassed[db])
        return;

    for(ix = 0; ix < rregIdMax; ix++)
    {
        if(ix == cpu)
            continue;
        if(!rregIdMapped(ix) && !rregIsReady(ix,db))
            continue;
        rregResync(ix,db);
    }
    resyncPassed[db] = TRUE;
}

/******************************************************************************/

/* shutdown all channels */

void rregShutdownAll(unsigned db, int forced)
{
    unsigned ix, cpu = boardProcNumGet();

    RREG_DB_CHECK(return);

    for(ix = 0; ix < rregIdMax; ix++)
    {
        if(ix == cpu)
            continue;
        if(!rregIdMapped(ix) && !rregIsReady(ix,db))
            continue;
        if(forced || rregState[db][ix].ready)
            rregShutdown(ix,db);
    }
}

/******************************************************************************/

/* registration ID bitmap */

#if defined MPON

unsigned rregIdMap[256/32] = {
    (RREG_ID00 << 0) | (RREG_ID01 << 1) | (RREG_ID02 << 2) | (RREG_ID03 << 3) |
    (RREG_ID04 << 4) | (RREG_ID05 << 5) | (RREG_ID06 << 6) | (RREG_ID07 << 7),
    0,0,0,0,0,0,0
};

#else /* MPON */

#define RREG_MAP_WORD(i)                                                                             \
    ((RREG_ID##i##0 << 0) | (RREG_ID##i##1 << 1) | (RREG_ID##i##2 << 2) | (RREG_ID##i##3 << 3) |     \
     (RREG_ID##i##4 << 4) | (RREG_ID##i##5 << 5) | (RREG_ID##i##6 << 6) | (RREG_ID##i##7 << 7) |     \
     (RREG_ID##i##8 << 8) | (RREG_ID##i##9 << 9) | (RREG_ID##i##A << 10) | (RREG_ID##i##B << 11) |   \
     (RREG_ID##i##C << 12) | (RREG_ID##i##D << 13) | (RREG_ID##i##E << 14) | (RREG_ID##i##F << 15))

unsigned rregIdMap[256/32] = {
    RREG_MAP_WORD(0) | (RREG_MAP_WORD(1) << 16),
    RREG_MAP_WORD(2) | (RREG_MAP_WORD(3) << 16),
    RREG_MAP_WORD(4) | (RREG_MAP_WORD(5) << 16),
    RREG_MAP_WORD(6) | (RREG_MAP_WORD(7) << 16),
    RREG_MAP_WORD(8) | (RREG_MAP_WORD(9) << 16),
    RREG_MAP_WORD(A) | (RREG_MAP_WORD(B) << 16),
    RREG_MAP_WORD(C) | (RREG_MAP_WORD(D) << 16),
    RREG_MAP_WORD(E) | (RREG_MAP_WORD(F) << 16)
};

#endif /* MPON */

/******************************************************************************/

/* check ID configured as valid */

int rregIdMapped(unsigned id)
{
    RREG_ID_CHECK(return FALSE);
    return (rregIdMap[id >> 5] & (1 << (id & 0x1f))) ? TRUE : FALSE;
}

/******************************************************************************/

/* wait until all configured ID registered */

int rregWait(unsigned db, unsigned tout)
{
    struct timespec ts1, ts2, ts3;
    unsigned ix, myId = boardProcNumGet(), done;

    RREG_DB_CHECK(return -1);

    clock_gettime(CLOCK_REALTIME,&ts1);
    ts1.tv_sec += tout;

    ts3.tv_sec = 0;
    ts3.tv_nsec = 100000000;

    while(1)
    {
        for(ix = 0, done = TRUE; ix < rregIdMax; ix++)
        {
            if(ix == myId)
                continue;

            if(rregIdMapped(ix) && !rregIsReady(ix,db))
            {
                done = FALSE;
                break;
            }
        }

        if(done)
        {
            RREG_DEBUG(1,("%s: registration complete\n",(long)__FUNCTION__,0,0,0,0,0));
            return 0;
        }

        clock_gettime(CLOCK_REALTIME,&ts2);
        if(ts2.tv_sec > ts1.tv_sec || (ts2.tv_sec == ts1.tv_sec && ts2.tv_nsec >= ts1.tv_nsec))
        {
            if(tout)
	    {
                RREG_DEBUG(1,("%s: registration timeout\n",(long)__FUNCTION__,0,0,0,0,0));
		for(ix = 0; ix < rregIdMax; ix += 32)
		{
		    unsigned v1 = rregIdMap[ix/32], v2 = 0, i;
		    for(i = 0; i < 32 && ix+i < rregIdMax; i++)
		    {
			if(rregIsReady(ix+i,db))
			    v2 |= 1 << i;
		    }
		    RREG_DEBUG(1,("    ID%02x...%02x: wait=%08x ready=%08x\n",ix,ix+0x1f,v1,v2,0,0));
		}
	    }
            return -1;
        }

        nanosleep(&ts3,NULL);
    }
}

/******************************************************************************/

/* show ready bitmap */

void rregShow(void)
{
   unsigned ix, i, v1, v2;
   for (ix = 0; ix < rregIdMax; ix += 32)
   {
      for (i = 0, v1 = 0, v2 = 0; i < 32 && ix + i < rregIdMax; i++)
      {
         if (rregIsReady(ix + i, 0))
            v1 |= 1 << i;
         if (rregIsReady(ix + i, 1))
            v2 |= 1 << i;
      }
      printf("ID %02x...%02x: ready1=%08x ready2=%08x\n", ix, ix + 0x1f, v1,
            v2);
   }
}

/******************************************************************************/

unsigned char rregTagMap[256/8];

/******************************************************************************/

/* set tag value */

//static 
int rregTagSet(unsigned id, unsigned cmd)
{
    unsigned hop = (id == 0xffffffff) ? 0 : 255;
    return rioMaintOut(hop,0x6c,id,cmd|RREG_TAG_MAGIC);
}

/******************************************************************************/

/* get tag value */

//static 
int rregTagGet(unsigned id, unsigned* cmd)
{
    unsigned hop = (id == 0xffffffff) ? 0 : 255;
    return rioMaintIn(hop,0x6c,id,cmd);
}

/******************************************************************************/

/* do response */

static int rregTagResponse(unsigned cmd)
{
    return rregTagSet(0xffffffff,cmd|RREG_TAG_RESP);
}

/******************************************************************************/

/* set map bit */

static inline void tagMapSet(unsigned id)
{
    rregTagMap[id/8] |= 1 << (id & 7);
}

/******************************************************************************/

/* clear map bit */

static inline void tagMapClear(unsigned id)
{
    rregTagMap[id/8] &= ~(1 << (id & 7));
}

/******************************************************************************/

/* test map bit */

static inline int tagMapIsSet(unsigned id)
{
    return (rregTagMap[id/8] & (1 << (id & 7))) != 0;
}

/******************************************************************************/

/* get current time in ms */

static unsigned long long mstime(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    return (unsigned long long)ts.tv_sec*1000ULL + (unsigned long long)ts.tv_nsec/1000000;
}

/******************************************************************************/

/* display tag map */

void rregTagMapDump(void)
{
    unsigned ix, id, i, val;
    printf("    0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
    printf("    - - - - - - - - - - - - - - - -\n");
    for(ix = 0, id = 0; id < rregIdMax; id += 16, ix += 2)
    {
	printf("%02X: ",id);
	val = rregTagMap[ix];
	for(i = 0; i < 8; i++)
	    printf("%c ",(val & (1 << i)) ? 'x' : '.');
	val = rregTagMap[ix+1];
	for(i = 0; i < 8; i++)
	    printf("%c ",(val & (1 << i)) ? 'x' : '.');
	printf("\n");
    }
    printf("\n");
}

/******************************************************************************/

/* issue command and wait response with timeout */

static int tagCmd(unsigned reqcmd, unsigned long long tout)
{
    unsigned id, cmd, nCpu, respcmd;

    for(id = 0, nCpu = 0; id < rregIdMax; ++id)
    {
	if(!tagMapIsSet(id))
	    continue;

	if(id == boardProcNumGet())
	    continue;

	if(rregTagSet(id,reqcmd) == -1)
	{
	    if(reqcmd != RREG_TAG_READY)
		printf("rregTagRegistration: id=0x%02x rregTagSet error!\n",id);
	    tagMapClear(id);
	}

	nCpu++;
    }

    if(!nCpu)
	return 0;

    if(reqcmd == RREG_TAG_READY)
    {
	printf("Available cpu map:\n");
	rregTagMapDump();
    }

    reqcmd |= RREG_TAG_MAGIC;
    respcmd = reqcmd | RREG_TAG_RESP;

    for(id = 0, nCpu = 0; id < rregIdMax; ++id)
    {
	if(!tagMapIsSet(id))
	    continue;

	if(id == boardProcNumGet())
	    continue;

	tagMapClear(id);

	do
	{
	    if(rregTagGet(id,&cmd) == -1)
	    {
		printf("rregTagRegistration: id=0x%02x rregTagGet error!\n",id);
		break;
	    }

	    if(cmd == respcmd)
	    {
		nCpu++;
		tagMapSet(id);
		break;
	    }

	    if(cmd != reqcmd)
	    {
		printf("rregTagRegistration: id=0x%02x rregTagGet unexpected command=%x\n",id,cmd);
		rregTagSet(id,reqcmd);
	    }

	} while(mstime() < tout);
    }

    return nCpu;
}

/******************************************************************************/

/* assign tag map to id map */

static void rregTagMapAssign(void)
{
    unsigned ix, i;

    for(ix = 0, i = 0; ix < 256/32; ++ix, i += 4)
    {
	rregIdMap[ix] = rregTagMap[i] | (rregTagMap[i+1] << 8) |
	    (rregTagMap[i+2] << 16) | (rregTagMap[i+3] << 24);
    }
}

/******************************************************************************/

/* master cpu registration */

static int masterRegistration(unsigned long long tout)
{
    int mapped = TRUE, id, ix, cpu = boardProcNumGet(), nCpu, n = 0, val, debug = rioDebug;

    rioDebug = 0;
    for(id = 0, nCpu = 0; id < rregIdMax; ++id)
    {
	mapped = rregIdMapped(id);

	if(!mapped)
	    continue;

	if(id == cpu)
	{
	    tagMapSet(id);
	    continue;
	}

	if(rioMaintIn(0xff,0x60,id,&val) == 0)
	{
	    if(((val >> 16) & 0xff) == id)
		tagMapSet(id);
	}
    }
    rioDebug = debug;

    if(!(nCpu = tagCmd(RREG_TAG_READY,tout)))
    {
	printf("rregTagRegistration: other cpu not found\n");
	memset(rregIdMap,0,sizeof(rregIdMap));
	return 0;
    }

    printf("Ready cpu map:\n");
    rregTagMapDump();

    do
    {
	nCpu = tagCmd(RREG_TAG_POLL,mstime()+1000);

	for(id = 0, ix = 0; id < rregIdMax; ++ix, id += 8)
	{
	    if((n = tagCmd(RREG_TAG_MAP|(rregTagMap[ix] << 8),mstime()+RREG_POLL_TOUT)) != nCpu)
		break;
	}

	if(n == nCpu)
	    break;

    } while(mstime() < tout);

    nCpu = tagCmd(RREG_TAG_DONE,mstime()+RREG_POLL_TOUT);
    printf("Master registration complete cpu map:\n");
    rregTagMapDump();
    rregTagMapAssign();
    return nCpu;
}

/******************************************************************************/

/* slave cpu registration */

static int slaveRegistration(unsigned long long tout)
{
    struct timespec ts;
    unsigned mapIx = 0, poll = FALSE, cmd = 0;
    int res;

    clock_getres(CLOCK_REALTIME,&ts);
    if(!ts.tv_sec && ts.tv_nsec < 100000000)
	ts.tv_nsec = 100000000;

    do
    {
	res = rregTagGet(0xffffffff,&cmd);

	if((cmd & 0xffff0000) == RREG_TAG_MAGIC)
	{
	    switch(cmd & 0xff)
	    {
		case RREG_TAG_READY:
		    printf("Get READY command\n");
		    rregTagResponse(cmd);
		    poll = FALSE;
		    break;
		case RREG_TAG_POLL:
		    rregTagResponse(cmd);
		    printf("Get POLL command\n");
		    poll = TRUE;
		    mapIx = 0;
		    break;
		case RREG_TAG_MAP:
		    if(!poll || mapIx == sizeof(rregTagMap))
		    {
			printf("Unexpected MAP command: poll=%d mapIx=%d\n",poll,mapIx);
			break;
		    }
		    rregTagResponse(cmd);
		    rregTagMap[mapIx++] = (cmd >> 8) & 0xff;
		    break;
		case RREG_TAG_DONE:
		    if(boardProcNumGet() != boardRioIdGet())
			boardProcNumSet(boardRioIdGet());
		    rregTagResponse(cmd);
		    printf("Slave registration complete cpu map:\n");
		    rregTagMapDump();
		    rregTagMapAssign();
		    return 0;
	    }
	}

	if(!poll)
	    nanosleep(&ts,NULL);

    } while(mstime() < tout);

    if(boardProcNumGet() != boardRioIdGet())
	boardProcNumSet(boardRioIdGet());
    memset(rregIdMap,0,sizeof(rregIdMap));
    return -1;
}

/******************************************************************************/

/* master/slave registration */

int rregTagRegistration(unsigned isMain, unsigned tout)
{
    unsigned long long mstout = mstime() + (unsigned long long)tout*1000ULL;
    printf("%s tag registration with %d sec timeout\n",isMain ? "Master" : "Slave",tout);
    return isMain ? masterRegistration(mstout) : slaveRegistration(mstout);
}

/******************************************************************************/

/* sync with slave */

int rregTagMasterSync(unsigned id, unsigned tout)
{
    unsigned long long mstout = mstime() + (unsigned long long)tout*1000ULL;
    unsigned val, reqcmd, respcmd, cmd;

    printf("Master sync with id=0x%02x, %d sec ... ",id,tout);
    fflush(stdout);

    if(rioMaintIn(0xff,0x60,id,&val))
    {
	printf("Slave not found\n");
	return -1;
    }

    reqcmd = RREG_TAG_MAGIC | RREG_TAG_SYNC;
    respcmd = reqcmd | RREG_TAG_RESP;

    if(rregTagSet(id,reqcmd) == -1)
    {
	printf("rregTagSet error\n");
	return -1;
    }

    do
    {
	if(rregTagGet(id,&cmd) == -1)
	{
	    printf("rregTagGet error\n");
	    break;
	}

	if(cmd == respcmd)
	{
	    printf("OK\n");
	    return 0;
	}

	if(cmd != reqcmd)
	{
	    printf("rregTagGet unexpected command=%x\n",cmd);
	    rregTagSet(id,reqcmd);
	}

    } while(mstime() < mstout);

    printf("timeout\n");
    return -1;
}

/******************************************************************************/

/* slave synchronization */

int rregTagSlaveSync(unsigned tout)
{
    unsigned long long mstout = mstime() + (unsigned long long)tout*1000ULL;
    unsigned cmd = 0;
    int res;

    printf("Slave sync %d sec ... ",tout);
    fflush(stdout);

    do
    {
	res = rregTagGet(0xffffffff,&cmd);

	if(cmd == (RREG_TAG_MAGIC|RREG_TAG_SYNC))
	{
	    printf("OK\n");
	    rregTagResponse(cmd);
	    return 0;
	}

    } while(mstime() < mstout);

    printf("timeout\n");
    return -1;
}

/******************************************************************************/

/* library initialization routine */

int rregInit(unsigned nId, unsigned pri, void (*hook)(int, unsigned, const char *), int hostType)
{
    struct sched_param schedparam;
    pthread_attr_t attr;
    int res, ix;

    struct mq_attr mqattr = {
        mq_maxmsg       : RREG_MQ_SIZE,
        mq_msgsize      : 4,
        mq_flags        : 0,
        mq_curmsgs      : 0
    };

    if(rregIdMax)
	return 0;

    rregReadyHook = hook;

    if(!nId || nId > 256)
    {
        RREG_DEBUG(1,("%s: invalid nId=%d\n",(long)__FUNCTION__,nId,0,0,0,0));
        return -1;
    }

    if(boardProcNumGet() >= nId)
    {
        RREG_DEBUG(1,("%s: invalid self id=%d\n",(long)__FUNCTION__,boardProcNumGet(),0,0,0,0));
        return -1;
    }

    for(ix = 0; ix < 256; ix++)
        rregHostType[ix] = RIO_HOST_INVALID;

    rregHostType[boardProcNumGet()] = hostType;
    RREG_DEBUG(2,("%s: Init Id=%d type=%d\n",(long)__FUNCTION__,boardProcNumGet(),hostType,0,0,0));

    rregIdMax = nId;

    if((rregMq = mq_open("rregMQ",O_RDWR|O_CREAT|O_EXCL,0,&mqattr)) == (mqd_t)(-1))
    {
        RREG_DEBUG(1,("%s: mq_open error=%s (%d)\n",(long)__FUNCTION__,
                (long)strerror(errno),errno,0,0,0));
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
#ifdef PTHREAD_ATTR_UNBREAKABLE
    pthread_attr_setbreakstate(&attr,PTHREAD_ATTR_UNBREAKABLE);
#endif
    schedparam.sched_priority = pri;
    pthread_attr_setschedparam(&attr,&schedparam);
#if OS_VERSION >= 271
    pthread_attr_setname(&attr,"rregThread");
#endif

    if((res = pthread_create(NULL,&attr,(void*)&rregThread,(void *)(long)hostType)))
    {
        RREG_DEBUG(1,("%s: pthread_create error=%s (%d)\n",(long)__FUNCTION__,
                (long)strerror(res),res,0,0,0));
        return -1;
    }

    return 0;
}
