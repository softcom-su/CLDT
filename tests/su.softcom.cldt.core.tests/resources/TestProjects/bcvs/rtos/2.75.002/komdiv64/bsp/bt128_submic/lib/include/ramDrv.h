/* ramDrv.h - header for ramDrv: block device on memory */

#ifndef	__INCramDrvh
#define	__INCramDrvh

#include <blkIo.h>

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

BLK_DEV *ramDevCreate(char* ramAddr, int bytesPerBlk, int blksPerTrack,
    int nBlocks, int blkOffset);

  #if OS_VERSION >= 210
     BLK_IO_DEV* ramDevInstall(char* ramAddr, int bytesPerBlk, int blksPerTrack,
                    int	nBlocks, int blkOffset, char* name);
  #endif
#else

BLK_DEV *ramDevCreate();
  #if OS_VERSION >= 210
     BLK_IO_DEV* ramDevInstall();
  #endif

#endif  /* __STDC__ */

#endif	/* __INCramDrvh */
