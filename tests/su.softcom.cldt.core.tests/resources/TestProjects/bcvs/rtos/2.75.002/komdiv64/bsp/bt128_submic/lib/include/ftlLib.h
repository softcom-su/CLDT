/* ftlLib.h - FTL user library header */

#ifndef	__INCftlLibh
#define	__INCftlLibh
#include <ftlDrv.h>

/* function declarations */

STATUS _fshow(FLASH_DESC* flash);
STATUS _fperase(FLASH_DESC* flash, unsigned offset, int bcount);
STATUS _ferase(FLASH_DESC* flash, unsigned sector, unsigned nSectors);
int _fpfree(FLASH_DESC* flash, unsigned offset, unsigned bcount);
int _ffree(FLASH_DESC* flash, unsigned sector, unsigned nSectors);
STATUS _fstore(FLASH_DESC* flash, unsigned offset, void* buf, unsigned bcount);
STATUS _fplock(FLASH_DESC* flash, unsigned offset, unsigned bcount);
STATUS _flock(FLASH_DESC* flash, unsigned sector, unsigned nSectors);
int _fplocked(FLASH_DESC* flash, unsigned offset, unsigned bcount, int cond);
int _flocked(FLASH_DESC* flash, unsigned sector, unsigned nSectors, int cond);
STATUS _fpunlock(FLASH_DESC* flash, unsigned offset, unsigned bcount);
STATUS _funlock(FLASH_DESC* flash, unsigned sector, unsigned nSectors);
STATUS _funlockall(FLASH_DESC* flash);
STATUS _fload(FLASH_DESC* flash, unsigned offset, char* name, int show);

STATUS fshow();
STATUS fperase(uintptr_t offset, int bcount);
STATUS ferase(uintptr_t sector, unsigned nSectors);
int fpfree(uintptr_t offset, unsigned bcount);
int ffree(uintptr_t sector, unsigned nSectors);
STATUS fstore(uintptr_t offset, void* buf, unsigned bcount);
STATUS fplock(uintptr_t offset, unsigned bcount);
STATUS flock(uintptr_t sector, unsigned nSectors);
STATUS fplocked(uintptr_t offset, unsigned bcount, int cond);
STATUS flocked(uintptr_t sector, unsigned nSectors, int cond);
STATUS fpunlock(uintptr_t offset, unsigned bcount);
STATUS funlock(uintptr_t sector, unsigned nSectors);
STATUS funlockall();
STATUS fload(char* name, uintptr_t offset);
void fhelp();

STATUS ftlLibInit();

#endif	/* __INCftlLibh */
