/* ftlDrv.h - FTL driver header */

#ifndef	__INCftlDrvh
#define	__INCftlDrvh

#if OS_VERSION == 206
  #include <inttypes.h>
#endif

#include <blkIo.h>

/* common flash descriptor structure */

typedef struct {
    unsigned	size;		/* flash size in bytes */
    unsigned	nSectors;	/* total number of sectors */
    unsigned	sectorSize;	/* sector size in bytes */
    unsigned	sectorBits;	/* log2 of sector size in bytes */
    FUNCPTR	read;		/* flash read routine */
    FUNCPTR	write;		/* flash write routine */
    FUNCPTR	erase;		/* erase sector routine */
    FUNCPTR	lock;		/* lock sector routine */
    FUNCPTR	unlock;		/* unlock sector routine */
    FUNCPTR	locked;		/* check that sector locked routine */
    FUNCPTR	suspend;	/* suspend program/erase routine */
    FUNCPTR	resume;		/* resume program/erase routine */
    FUNCPTR	show;		/* show flash content routine */
    FUNCPTR	free;		/* check that area clean routine */
} FLASH_DESC;

/* erase sector request */

typedef struct {
    volatile unsigned	state;		/* request state (see below) */
    unsigned		offset;		/* erase sector offset */
    unsigned		timeout;	/* request timeout in ticks */
    unsigned		shift;		/* timeout shift for zero check */
    VOIDFUNCPTR		callback;	/* callback routine */
    uintptr_t		spare;		/* somewere */
} ERASE_REQUEST;

/* hidden definition of FTL device */

typedef struct ftl_dev FTL_DEV;

/* erase request state values */

#define	ERASE_IDLE		1	/* request inactive */
#define	ERASE_Q_FULL		2	/* erase queue full (try later) */
#define	ERASE_SUCCESS		3	/* erase done: success */
#define	ERASE_FAILED		4	/* erase done: failed */
#define	ERASE_TIMEOUT		5	/* erase done: timeout */
#define	ERASE_VERIFY_FAILED	6	/* erase done: sector not clean */
#define	ERASE_IN_PROGRESS	7	/* erase in progress */

/* FTL block device ioctl`s */

#define	FTL_IOCTL_RECLAIM	0x20000000	/* reclaim deleted blocks */
#define	FTL_IOCTL_NFREE		0x20000001	/* get # of free blocks */
#define	FTL_IOCTL_NDELETED	0x20000002	/* get # of deleted blocks */
#define	FTL_IOCTL_PREPARE	0x20000003	/* prepare all erased units */

/* debug print */

#define	FTL_DEBUG(level,msg)	\
{				\
    if(ftlDebug >= (level))	\
	printf msg;		\
}

/* pointer to system flash descriptor */

extern FLASH_DESC* pSysFlashDesc;

/* pointer to system FTL device descriptor */

extern FTL_DEV* pSysFtlDev;

/* debug output level */

extern int ftlDebug;

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

#if OS_VERSION >= 210
FTL_DEV* ftlDevInit(FLASH_DESC* flash, unsigned partitionOffset, int priority);
#else
FTL_DEV* ftlDevInit(FLASH_DESC* flash, unsigned partitionOffset);
#endif
BLK_DEV* ftlBlkDevCreate(FTL_DEV* ftl, unsigned size, unsigned offset);
#if OS_VERSION >= 210
BLK_IO_DEV* ftlBlkDevInstall(FTL_DEV* ftl, unsigned size, unsigned offset,
    char* name);
#endif
STATUS ftlFormat(FLASH_DESC* flash, unsigned offset, unsigned size,
    unsigned nTransferUnits, unsigned reservePercent, int maxEraseReq);
int ftlDrvVersion(int silence);
int ftlShow(FTL_DEV* ftl);

STATUS ftlErase(FLASH_DESC* flash, unsigned offset, unsigned nSectors,
    int maxEraseReq);

int fcln32(unsigned* ptr, unsigned bcount);
int fcmp32(unsigned* p1, unsigned* p2, unsigned bcount);
void fcpy32(unsigned* dst, unsigned* src, unsigned bcount);

#else

FTL_DEV* ftlDevInit();
BLK_DEV* ftlBlkDevCreate();
STATUS ftlFormat();
int ftlDrvVersion();
int ftlShow();

STATUS ftlErase();

int fcln32();
int fcmp32();
void fcpy32();

#endif  /* __STDC__ */

#endif	/* __INCftlDrvh */
