/* romFlashNvRam.c - ROM flash block as NVRAM emulations on BT206 */

/*
modification history
--------------------
*/

#include <system.h>
#include <string.h>
#include <errno.h>
#include <board.h>

/* whole flash block description */

#define	NV_BASE		K1_TO_TLB(0xbfc70000)	/* NVRAM flash block base */
#define	NV_SIZE		0x10000			/* NVRAM flash block size */

#define	NV_CELLS	(NV_SIZE / NV_CELL_BYTES)

/******************************************************************************/

/* 0 - nothing, 1 - OK, 2 - ERROR, 3 - protected */

int romNvInitStatus = 0;

/******************************************************************************/

/* check that NVRAM cell is free */

static inline int romNvCellFree(unsigned cell)
{
    unsigned* ptr = (unsigned *)(NV_BASE + (cell * NV_CELL_BYTES)), ix, sum;

    for(ix = 0, sum = 0xffffffff; ix < NV_CELL_LONGS; ix += 8, ptr += 8)
    {
	sum &= ptr[0];
	sum &= ptr[1];
	sum &= ptr[2];
	sum &= ptr[3];
	sum &= ptr[4];
	sum &= ptr[5];
	sum &= ptr[6];
	sum &= ptr[7];

	if(sum != 0xffffffff)
	    return FALSE;
    }
    return TRUE;
}

/******************************************************************************/

/* check that BSP part of NVRAM free */

static inline STATUS romNvFree()
{
    unsigned cell;

    for(cell = 0; cell < NV_CELLS; cell++)
	if(!romNvCellFree(cell))
	    return -1;

    return 0;
}

/******************************************************************************/

/* issue flash command */

inline void romNvWrite(unsigned offset, unsigned char value)
{
    *(volatile unsigned char*)(NV_BASE + offset) = value;
}

/******************************************************************************/

/* waiting for programm complete */

static inline STATUS romNvWritePoll(unsigned offset, unsigned char value)
{
    int ix; unsigned char c, vbit = value & 0x80;

    /* poll programm complete condition */

    for(ix = 0; ix < 300; ix++)
    {
	c = *(volatile unsigned char*)(NV_BASE + offset);

	/* check DQ7 */

	if((c & 0x80) == vbit)
	    break;

	/* check DQ5 */

	if((c & 0x20) == 0)
	    continue;

	/* additional read before break */

        c = *(volatile unsigned char*)(NV_BASE + offset);
        break;
    }

    /* reset to read array mode */

    romNvWrite(0,0xf0);

    /* read and verify programmed byte */

    c = *(volatile unsigned char*)(NV_BASE + offset);

    return (c == value) ? 0 : (-1);
}

/******************************************************************************/

/* waiting for erase complete */

static inline void romNvErasePoll()
{
    int ix; unsigned char c;

    /* poll erase complete condition */

    for(ix = 0; ix < 3000000; ix++)
    {
	c = *(volatile unsigned char*)(NV_BASE);

	/* check DQ7 and DQ5 */

	if((c & 0x80) == 0x80 || (c & 0x20))
	    break;
    }

    /* switch back to read array mode */

    romNvWrite(0,0xf0);
}

/******************************************************************************/

/* program byte in flash */

inline STATUS romNvProgram(unsigned offset, unsigned char value)
{
    romNvWrite(0x555,0xaa);
    romNvWrite(0x2aa,0x55);
    romNvWrite(0x555,0xa0);
    romNvWrite(offset,value);
    return romNvWritePoll(offset,value);
}

/******************************************************************************/

/* erase NVRAM block */

inline STATUS romNvErase()
{
    romNvWrite(0x555,0xaa);
    romNvWrite(0x2aa,0x55);
    romNvWrite(0x555,0x80);
    romNvWrite(0x555,0xaa);
    romNvWrite(0x2aa,0x55);
    romNvWrite(0x000,0x30);
    romNvErasePoll();
    return romNvFree();
}

/******************************************************************************/

/* find last NVRAM cell */

static inline uintptr_t romNvFind()
{
    unsigned cell;

    for(cell = NV_CELLS - 1; cell; cell--)
	if(!romNvCellFree(cell))
	    break;

    return NV_BASE + (cell * NV_CELL_BYTES);
}

/******************************************************************************/

/* copy NVRAM to string */

long romNvGet(unsigned short string[])
{
    memcpy((char*)string,(char*)romNvFind(),NV_CELL_BYTES); return 0;
}

/******************************************************************************/

/* strore string in NVRAM */

long romNvPut(unsigned short string[])
{
    uintptr_t cell = romNvFind();
    unsigned char* s = (unsigned char*)string;
    unsigned ix, offset;

    if(romNvInitStatus != 1)
	return -1;

    /* no dirty cell found: check first cell */

    if(cell == NV_BASE && romNvCellFree(0))
	goto found;
    
    cell += NV_CELL_BYTES;

    /* no more free cell: erase whole sector */

    if(cell == NV_BASE + NV_SIZE)
    {
	if(romNvErase() == -1)
	    return -1;

	cell = NV_BASE;
    }

found:

    offset = cell - NV_BASE;

    for(ix = 0; ix < NV_CELL_BYTES; ix++)
	if(romNvProgram(offset+ix,s[ix]) == -1)
	    return -1;

    if(memcmp((char*)cell,s,NV_CELL_BYTES))
	return -1;

    return 0;
}

/******************************************************************************/

/* check write ability */

long romNvInit()
{
    unsigned w1, w2; unsigned char c;

    if(romNvInitStatus)
	goto done;

    /* get word from flash */

    w1 = *(volatile unsigned*)NV_BASE;

    /* switch to autoselect mode */

    romNvWrite(0x555,0xaa);
    romNvWrite(0x2aa,0x55);
    romNvWrite(0x555,0x90);

    /* get ID's and write protect status */

    w2 = *(volatile unsigned*)NV_BASE;
    c = *(volatile unsigned char*)(NV_BASE + 2);

    if(w1 == w2)		/* cannot get ID ? */
    {
	romNvInitStatus = 2;
    }
    else if(c == 0x00)		/* write available */
    {
	romNvInitStatus = 1;
	nv_init = romNvInit;
	nv_get  = romNvGet;
	nv_put  = romNvPut;
    }
    else if(c == 0x01)		/* sector protected ? */
    {
	romNvInitStatus = 2;
    }
    else			/* invalid write protect status */
    {
	romNvInitStatus = 2;
    }

    /* switch back to read array mode */

    romNvWrite(0,0xf0);

done:
    return (romNvInitStatus != 2) ? 0 : (-1);
}
