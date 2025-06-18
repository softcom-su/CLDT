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
STATUS fperase(unsigned offset, int bcount);
STATUS ferase(unsigned sector, unsigned nSectors);
int fpfree(unsigned offset, unsigned bcount);
int ffree(unsigned sector, unsigned nSectors);
STATUS fstore(unsigned offset, void* buf, unsigned bcount);
STATUS fplock(unsigned offset, unsigned bcount);
STATUS flock(unsigned sector, unsigned nSectors);
STATUS fplocked(unsigned offset, unsigned bcount, int cond);
STATUS flocked(unsigned sector, unsigned nSectors, int cond);
STATUS fpunlock(unsigned offset, unsigned bcount);
STATUS funlock(unsigned sector, unsigned nSectors);
STATUS funlockall();
STATUS fload(unsigned offset, char* name);
void fhelp();

STATUS ftlLibInit();

#endif	/* __INCftlLibh */
