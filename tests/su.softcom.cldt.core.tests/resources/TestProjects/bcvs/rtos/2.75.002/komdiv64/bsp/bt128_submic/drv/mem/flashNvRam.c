/* flashNvRam.c - common flash block as NVRAM emulations library */

#include <system.h>
#include <string.h>
#include <board.h>

#ifdef	INCLUDE_NVRAM_COMPRESS
#include <compress.h>
#endif

/* globals */

FUNCPTR	nv_init;	/* NVRAM initialization routine */
FUNCPTR	nv_get;		/* NVRAM read routine */
FUNCPTR	nv_put;		/* NVRAM write routine */

/******************************************************************************/

/* make sum correcting word in last word */

static void nv_sum(unsigned short buf[])
{
    unsigned int sum = 0, ix;

    for(ix = 0; ix < NV_CELL_SUM; ix++)
         sum += buf[ix];

    buf[NV_CELL_SUM] = (-sum - NV_CELL_WORDS) & 0xffff;
}

/******************************************************************************
*
* sysNvLineSize - get number of bytes the string will occupy in NVRAM
*
* NVRAM contains compressed boot string. Compress ratio depends
* on string contents. In the best case string can be almost twice longer
* then in the worst.
*
* RETURNS: Number of bytes the string will occupy in NVRAM or
*          -1 if string is too long
*          If argument is NULL, returns minimum posible value.
*          BOOT_LINE_SIZE defines maximum posible size
*
* SEE ALSO: sysNvRamSet(), sysNvRamGet()
*/

int sysNvLineSize(char string[])
{
#ifdef	INCLUDE_NVRAM_COMPRESS
    unsigned short buf[NV_CELL_WORDS];
#endif

    if(!string)
        return 1;

#ifdef	INCLUDE_NVRAM_COMPRESS
    return nv_compress(buf,string);
#else
    return strlen(string) + 1;
#endif
}

/******************************************************************************
*
* sysNvRamCheck - checks sum of NVRAM content
*
* This routine calculates first check sum of NVRAM and compares it with zero
*
* RETURNS: TRUE - zero sum, FALSE - otherwise
*
*/

int sysNvRamCheck()
{
    unsigned short buf[NV_CELL_WORDS]; unsigned short val;

    if(!nv_init || nv_init() == -1)
	return FALSE;
	
    nv_get(buf); val = buf[NV_CELL_SUM]; nv_sum(buf);

    return  (val == buf[NV_CELL_SUM]) ? TRUE : FALSE;
}

/******************************************************************************
*
* sysNvRamCheckSum - get second check sum of NVRAM content
*
* Second check sum can be used for additional control of NVRAM software
* integrity and version
*
* RETURNS: check sum
*
*/

unsigned char sysNvRamCheckSum(void)
{
    unsigned char buf[NV_CELL_BYTES]; int ix; unsigned char sum = 0;

    if(!nv_init || nv_init() == -1)
	return 0;

    nv_get((unsigned short*)buf);

    for(ix = 0; ix < NV_CELL_BYTES; ix++)
	sum += buf[ix];

    return sum;
}

/******************************************************************************
*
* sysNvRamGet - get the contents of non-volatile RAM
*
* This routine copies the contents of non-volatile memory into a specified
* string.  The string will be terminated with an EOS.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamSet()
*/

STATUS sysNvRamGet
    (
    char* string,	/* where to copy NVRAM */
    int maxLen,		/* maximum number of bytes to copy */
    int offset		/* half-words offset into NVRAM */
    )
{
    unsigned short buf[NV_CELL_WORDS], val;

#ifdef	INCLUDE_NVRAM_COMPRESS
    char ubuf[BOOT_LINE_SIZE];
#endif

    if(!nv_init || nv_init() == -1)
	{ errno = ENODEV; return -1;
        }
    if(offset < 0 || offset >= NV_CELL_WORDS || maxLen < 0)
      { errno = EINVAL; return -1;
      }

    nv_get(buf);

    /* validate NVRAM checksum */

    val = buf[NV_CELL_SUM]; nv_sum(buf);

    if (val != buf[NV_CELL_SUM])
      { errno = EIO; return -1;
      }

#ifdef	INCLUDE_NVRAM_COMPRESS
    nv_uncompress(buf+offset,ubuf);
    strncpy(string,ubuf,maxLen);
#else
    strncpy(string,(char*)(buf+offset),maxLen);
#endif

    string[maxLen] = '\0';

    return 0;
}

/*******************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine copies a specified string into non-volatile RAM.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamGet()
*/

STATUS sysNvRamSet
    (
    char* string,	/* string to be copied into NVRAM */
    int maxLen,		/* maximum number of bytes to copy */
    int offset		/* half-words offset into NVRAM */
    )
{
    unsigned short buf[NV_CELL_WORDS];
    unsigned short cmp[NV_CELL_WORDS];
    int len;

#ifdef	INCLUDE_NVRAM_COMPRESS
    unsigned short str[BOOT_LINE_SIZE / 2]; int ix;
#endif

    if(!nv_init || nv_init() == -1)
      { errno = ENODEV;	return -1;
      }
    if(maxLen > (len = strlen(string)))
	maxLen = len;

    if(offset < 0 || offset >= NV_CELL_WORDS || maxLen < 0)
      { errno = EINVAL; return -1;
      }
#ifdef	INCLUDE_NVRAM_COMPRESS
    if((len = nv_compress(str,string)) < 0)
	return -1;
#else
    len = strlen(string) + 1;
#endif

    len >>= 1;

    if(offset + len > NV_CELL_END)
      { errno = EFBIG;	return -1;
      }
    nv_get(buf);

#ifdef	INCLUDE_NVRAM_COMPRESS
    for(ix = 0; ix < len; ix++)
	buf[ix+offset] = str[ix];
#else
	strcpy((char*)(buf+offset),string);
#endif

    buf[NV_CELL_END] = 0; nv_sum(buf);

    if(nv_put(buf) == -1)
	return -1;

    nv_get(cmp);

    if (memcmp(cmp,buf,NV_CELL_BYTES)==0) return 0;

    errno = EBADMSG; return -1;
}

