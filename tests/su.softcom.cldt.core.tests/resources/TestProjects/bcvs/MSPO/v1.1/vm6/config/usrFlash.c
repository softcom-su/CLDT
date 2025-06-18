/* usrFlash.c - example of flash initialization */

#include "ftlLib.h"
#include "iflashLib.h"
#include "flashLib.h"

/* priority of thread that poll sector erase or -1 for synchronous erase */

#define	ERASE_THREAD_PRI	(-1)

/* system flash descriptor */

FLASH_DESC* pSysFlashDesc = NULL;

/* size of system flash */

unsigned sysFlashSize = 0x2000000;

/******************************************************************************/

/* CPRIO64 intel flash library handler */

long sysFtlHandler(int boardId, int cmd, unsigned offset)
{
    switch(cmd)
    {
	case BRD_GET_PTR:
	    if(offset >= sysFlashSize)
		return (long)NULL;
	    return FLASH_BASE + offset;

	case BRD_SHOW: 
	    printf("CPRIO64: flash based at %p, sector %dK, size %dM\n\r",
		(int*)FLASH_BASE,pSysFlashDesc->sectorSize/0x400,
		sysFlashSize/0x100000);
	    return 0;
    }

    return 0;
}

/******************************************************************************/

/* create flash descriptor (if need set MODE_WRITE_VERIFY|MODE_ERASE_VERIFY) */

STATUS sysFlashInit()
{
    //*(volatile unsigned*)(SYSTEM_REGS+0x140) |= 0x1;	// enable 2 chip mode

    if(!(pSysFlashDesc = iflashInit(1,sysFtlHandler,0,
	 ERASE_THREAD_PRI,MODE_WRITE_VERIFY|MODE_ERASE_VERIFY|MODE_ISA)))
    {
	return -1;
    }

    sysFlashSize = pSysFlashDesc->size;

    flashShowInit();
    flash_base = (unsigned*)FLASH_BASE;
    flash_size = pSysFlashDesc->size;
    flash_nsec = pSysFlashDesc->nSectors;
    flash_ssec = pSysFlashDesc->sectorSize;

#ifdef	FLASH_NVRAM_SECTOR
    if(!nv_init)
    {
	nv_init = (FUNCPTR)sysNvInit;
	nv_get  = (FUNCPTR)sysNvGet;
	nv_put  = (FUNCPTR)sysNvPut;
    }
#endif	/* FLASH_NVRAM_SECTOR */

    return 0;
}
