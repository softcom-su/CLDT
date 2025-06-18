/* blkIo.h - block device I/O header file */

#ifndef __INCblkIoh
#define __INCblkIoh

#include <version.h>

/* IDE, SCSI and etc. block devices ioctl's */

#define	FIOPOWERDOWN	1000	/* power down all attached devices and chip */
#define	FIOPOWERUP	1001	/* wakeup from power down */
#define	FIOSTOPUNIT	1002	/* stop device (standby) */
#define	FIOSTARTUNIT	1003	/* wakeup from standby */
#define	FIOLASTSESSION	1004	/* get offset of CDROM last session */
#define	FIODOORLOCK	1005	/* prevent medium removal */
#define	FIODOORUNLOCK	1006	/* allow medium removal */
#define	FIOEJECT	1007	/* eject the media */
#define	FIOREADYCHANGED	1008	/* device ready status changed */

#if OS_VERSION >= 210 && defined(KERNEL)

#include <pthread.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <private/ios.h>
#include <sys/disklabel.h>
#include <sys/diskslice.h>

/* standard block device queue descriptor */

typedef struct {
    pthread_mutex_t		bq_mutex;	/* buf queue mutex */
    pthread_cond_t		bq_cond;	/* buf queue condition variable */
    struct buf_queue_head	bq_queue_head;	/* head of buf queue */
    pthread_t			bq_thread;	/* queue thread id */
    int				bq_priority;	/* queue thread priority or -1 */
    int				bq_ix;		/* queue index for the same driver */
} BLK_QUEUE;

/* standard block device descriptor for system I/O */

struct blk_io_dev {
    struct blk_dev*	bd;	/* pointer to block device */
    BLK_QUEUE*		queue;	/* block device queue or NULL */
    unsigned long	offset;	/* block offset from real device */
    char*		name;	/* device name */
    int			slices;	/* TRUE if slices used */
    dev_t		major;	/* device major number */
    struct disklabel	dl;	/* for OS communication */
    struct diskslices*	ds;	/* partitions */
    int			nRef;	/* reference count */
};

#endif	/* OS_VERSION >= 210 && defined(KERNEL) */

#if OS_VERSION >= 210
typedef struct blk_io_dev BLK_IO_DEV;
#endif

typedef struct blk_dev {	/* BLK_DEV */
    FUNCPTR		bd_blkRd;		/* function to read blocks */
    FUNCPTR		bd_blkWrt;		/* function to write blocks */
    FUNCPTR		bd_ioctl;		/* function to ioctl device */
    FUNCPTR		bd_reset;		/* function to reset device */
    FUNCPTR		bd_statusChk;		/* function to check status */
    int			bd_removable;		/* removable medium flag */
    unsigned long	bd_nBlocks;		/* number of blocks on device */
    unsigned long	bd_bytesPerBlk;		/* number of bytes per block */
    unsigned long	bd_blksPerTrack;	/* number of blocks per track */
    unsigned long	bd_nHeads;		/* number of heads */
    int			bd_retry;		/* retry count for I/O errors */
    int			bd_mode;		/* O_RDONLY |O_WRONLY| O_RDWR */
    int			bd_readyChanged;	/* dev ready status changed */
} BLK_DEV;

#if OS_VERSION >= 210 && defined(KERNEL)

/* globals */

extern ioDRV blkDevDrv;		/* standard block device driver operations */
extern dev_t blkDevNum;		/* current free block device number */
extern BLK_IO_DEV* blkDevs[256];/* array of major block devices */

/* function declarations */

int blkDevIoctl(ioFILE* file, int cmd, uintptr_t arg);
int blkDevStartegy(ioDEV* unused, struct buf* bp);
void blkChainDone(struct buf* bp, int result);
struct buf* blkChainBind(BLK_QUEUE* bq, unsigned long limit,
    unsigned long* pSize);
STATUS blkQueueInit(BLK_QUEUE* bq, int priority, void* thread, void* arg,
    int debug, int ix);
dev_t blkDevNumGet(void);
STATUS blkDevIns(BLK_IO_DEV* io, char* name, BLK_QUEUE* bq, int offset,
    int usePart, int debug);
int blkDevInit(BLK_IO_DEV* io);

#endif	/* OS_VERSION >= 210 && defined(KERNEL) */

#endif /* __INCblkIoh */
