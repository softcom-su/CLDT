/* amdFlash.h - AMD flash FTL support library header */

#ifndef	__INCamdFlashh
#define	__INCamdFlashh

#include <semaphore.h>
#include <pthread.h>
#include <ftlDrv.h>

/* flash descriptor mode bits definitions */

#define	MODE_ERASE_VERIFY	0x001	/* checking sector after erase */
#define	MODE_WRITE_VERIFY	0x002	/* checking written data */
#define	MODE_SYNC_ERASE		0x008	/* synchronous erase */
#define	MODE_4X8		0x080	/* flash chips organization is 4x8 (autodetected) */
#define	MODE_QUERY		0x100	/* use CFI query (autodetected) */
#define	MODE_UNIFORM		0x200	/* uniform sectors (autodetected) */
#define	MODE_RESIDENT		0x400	/* image flash resident */

/* AMD boot block structure */

typedef struct {
    int			bottom;		/* TRUE if bottom boot sector, else top */
    int			nBlocks;	/* how many erase zones */
    unsigned		blkSize[0];	/* sizes os erase zones in bytes */
} AMD_BOOT_BLOCK;

/* AMD flash parameters */

typedef struct {
    unsigned char	mnfID;		/* manufacturer code or 0 */
    unsigned char	devID;		/* device code or 0 */
    char*		name;		/* device name */
    int			programm;	/* programm timeout in us */
    int			suspend;	/* erase suspend timeout in us or -1 */
    int			erase;		/* erase timeout in sec */
    unsigned		chipSize;	/* size of one chip in bytes */
    unsigned		sectorSize;	/* size of one chip sector in bytes */
    AMD_BOOT_BLOCK*	bootBlock;	/* boot block structure if any */
} AMD_FLASH_DESC;

/* function declarations */

int amdFlashVersion(int silence);

FLASH_DESC* amdFlashInit(unsigned nChips, volatile unsigned* base,
    unsigned mode, AMD_FLASH_DESC flash[]);

#endif	/* __INCamdFlashh */
