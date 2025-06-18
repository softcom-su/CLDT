/* sysFlashNvRam.c - system flash block as NVRAM emulations */

#include <system.h>
#include <string.h>
#include <board.h>
#include <compress.h>
#include <ftlDrv.h>
#include <ftlLib.h>

/* whole flash block description */

static unsigned sysNvSector = -1;	/* NVRAM flash sector (from configbrd.h) */
static unsigned sysNvBase;		/* NVRAM flash sector offset in bytes */
static unsigned sysNvSize;		/* NVRAM flash sector size in bytes */
static unsigned sysNvCells;		/* how many cells in NVRAM flash sector */

/******************************************************************************/

/* check that NVRAM cell is free */

static inline int sysNvCellFree(unsigned cell)
{
    return fpfree(sysNvBase + (cell * NV_CELL_BYTES),NV_CELL_BYTES);
}

/******************************************************************************/

/* erase NVRAM block */

static inline STATUS sysNvErase()
{
    return ferase(sysNvSector,1);
}

/******************************************************************************/

/* find last NVRAM cell */

static inline unsigned sysNvFind()
{
    unsigned cell;

    for(cell = sysNvCells - 1; cell; cell--)
    {
	if(!sysNvCellFree(cell))
	    break;
    }

    return sysNvBase + (cell * NV_CELL_BYTES);
}

/******************************************************************************/

/* copy NVRAM to string */

int sysNvGet(unsigned short string[])
{
    return pSysFlashDesc->read(pSysFlashDesc,sysNvFind(),string,NV_CELL_BYTES);
}

/******************************************************************************/

/* strore string in NVRAM */

STATUS sysNvPut(unsigned short string[])
{
    unsigned offset = sysNvFind();

    if(offset == sysNvBase && sysNvCellFree(0))
	goto found;

    offset += NV_CELL_BYTES;

    if(offset == sysNvBase + sysNvSize)
    {
	if(sysNvErase() == -1)
	    return -1;
	offset = sysNvBase;
    }

found:

    return fstore(offset,string,NV_CELL_BYTES);
}

/******************************************************************************/

/* initialize NVRAM */

STATUS sysNvInit()
{
    if(sysNvSector != -1)
	return 0;

    if(!pSysFlashDesc)
	return -1;

    if(FLASH_NVRAM_SECTOR == -1)
	sysNvSector = pSysFlashDesc->nSectors - 1;
    else if(FLASH_NVRAM_SECTOR < pSysFlashDesc->nSectors)
	sysNvSector = FLASH_NVRAM_SECTOR;
    else
	return -1;

    sysNvBase  = sysNvSector << pSysFlashDesc->sectorBits;
    sysNvSize  = pSysFlashDesc->sectorSize;
    sysNvCells = sysNvSize / NV_CELL_BYTES;

    return 0;
}
