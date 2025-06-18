/* iflashLib.h - intel flash FTL support library header */

#ifndef	__INCiflashLibh
#define	__INCiflashLibh

#include <semaphore.h>
#include <pthread.h>
#include <ftlDrv.h>

/* intel flash descriptor mode bits definitions */

#define	MODE_ERASE_VERIFY	0x001	/* checking sector after erase */
#define	MODE_WRITE_VERIFY	0x002	/* checking written data */
#define	MODE_VPP_SWITCH		0x004	/* disable Vpp after program/erase */
#define	MODE_SYNC_ERASE		0x008	/* synchronous erase */
#define	MODE_4X8		0x080	/* flash chips organization is 4x8 (autodetected)*/
#define	MODE_ISA		0x100	/* ISA 8 bit flash */

/* board handler commands */

#define	BRD_VPP_HIGH		1	/* write enable on */
#define	BRD_VPP_LOW		2	/* write enable off */
#define	BRD_WP_HIGH		3	/* write protect on */
#define	BRD_WP_LOW		4	/* write protect off */
#define	BRD_SHOW		5	/* show some about board */
#define	BRD_GET_PTR		6	/* convert offset to local address */
#define	BRD_PUT_PTR		7	/* release offset to local address */

/* intel flash region descriptor */

typedef struct {
    unsigned		state;		/* region erase state (see iflashLib.c) */
    ERASE_REQUEST*	request;	/* connected sector erase request */
} IFLASH_REGION;

/* intel flash descriptor structure */

typedef struct {
    FLASH_DESC		common;		/* common flash descriptor */
    unsigned char	query[64];	/* query structure */
    FUNCPTR		board;		/* board dependent handler */
    int			boardId;	/* board descriptor */
    unsigned		mode;		/* MODE_ definitions (see above) */
    unsigned		nRegions;	/* total number of regions */
    unsigned		regionSize;	/* region size in bytes */
    unsigned		regionBits;	/* log2 of region size in bytes */
    unsigned		nBuffers;	/* how many write buffers */
    IFLASH_REGION*	regions;	/* regions descriptors (see above) */
    sem_t		eraseKickSem;	/* for kick ifl_eraseThread */
    pthread_mutex_t	eraseMuteSem;	/* for mute ifl_eraseThread */
    unsigned		eraseCount;	/* active erase requests count */
    unsigned		vppCount;	/* current Vpp ON count */
    int			chipsPerRegion;	/* log2 of chips per region */
} IFLASH_DESC;

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

int iflashLibVersion(int silence);
FLASH_DESC* iflashInit(unsigned nChips, FUNCPTR board, int boardId,
    unsigned eraseTaskPri, unsigned mode);

#else

int iflashLibVersion();
FLASH_DESC* iflashInit();

#endif  /* __STDC__ */

#endif	/* __INCiflashLibh */
