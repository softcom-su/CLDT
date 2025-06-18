/* nandTest.c - NAND flash test */

#include <stdio.h>
#include <board.h>

#define	MODE_IRQ		1	/* wait irq for write and erase */

/* oob data */

typedef struct {
    unsigned char blockStatus;
    unsigned char pageStatus;
    unsigned char tagByte0;
    unsigned char tagByte1;
    unsigned char ecc[24];
    unsigned	  tags[9];
} SPARE;

/* bad blocks table */

static unsigned bbt[NAND_BLOCKS_PER_CHIP*NAND_N_CHIPS/32];

static unsigned rw_data[2048/4];	/* read-write data buffers */
static unsigned ck_data[2048/4];	/* check data buffers */
static unsigned rw_oob[64/4];		/* read-write OOB data buffers */
static unsigned ck_oob[64/4];		/* check  OOB data buffers */

static int nmode;			/* current test mode (see above) */

static unsigned first, last;		/* first and last blocks */

static const unsigned char column_parity_table[] = {
0x00, 0x55, 0x59, 0x0c, 0x65, 0x30, 0x3c, 0x69, 0x69, 0x3c, 0x30, 0x65, 0x0c, 0x59, 0x55, 0x00, 
0x95, 0xc0, 0xcc, 0x99, 0xf0, 0xa5, 0xa9, 0xfc, 0xfc, 0xa9, 0xa5, 0xf0, 0x99, 0xcc, 0xc0, 0x95, 
0x99, 0xcc, 0xc0, 0x95, 0xfc, 0xa9, 0xa5, 0xf0, 0xf0, 0xa5, 0xa9, 0xfc, 0x95, 0xc0, 0xcc, 0x99, 
0x0c, 0x59, 0x55, 0x00, 0x69, 0x3c, 0x30, 0x65, 0x65, 0x30, 0x3c, 0x69, 0x00, 0x55, 0x59, 0x0c, 
0xa5, 0xf0, 0xfc, 0xa9, 0xc0, 0x95, 0x99, 0xcc, 0xcc, 0x99, 0x95, 0xc0, 0xa9, 0xfc, 0xf0, 0xa5, 
0x30, 0x65, 0x69, 0x3c, 0x55, 0x00, 0x0c, 0x59, 0x59, 0x0c, 0x00, 0x55, 0x3c, 0x69, 0x65, 0x30, 
0x3c, 0x69, 0x65, 0x30, 0x59, 0x0c, 0x00, 0x55, 0x55, 0x00, 0x0c, 0x59, 0x30, 0x65, 0x69, 0x3c, 
0xa9, 0xfc, 0xf0, 0xa5, 0xcc, 0x99, 0x95, 0xc0, 0xc0, 0x95, 0x99, 0xcc, 0xa5, 0xf0, 0xfc, 0xa9, 
0xa9, 0xfc, 0xf0, 0xa5, 0xcc, 0x99, 0x95, 0xc0, 0xc0, 0x95, 0x99, 0xcc, 0xa5, 0xf0, 0xfc, 0xa9, 
0x3c, 0x69, 0x65, 0x30, 0x59, 0x0c, 0x00, 0x55, 0x55, 0x00, 0x0c, 0x59, 0x30, 0x65, 0x69, 0x3c, 
0x30, 0x65, 0x69, 0x3c, 0x55, 0x00, 0x0c, 0x59, 0x59, 0x0c, 0x00, 0x55, 0x3c, 0x69, 0x65, 0x30, 
0xa5, 0xf0, 0xfc, 0xa9, 0xc0, 0x95, 0x99, 0xcc, 0xcc, 0x99, 0x95, 0xc0, 0xa9, 0xfc, 0xf0, 0xa5, 
0x0c, 0x59, 0x55, 0x00, 0x69, 0x3c, 0x30, 0x65, 0x65, 0x30, 0x3c, 0x69, 0x00, 0x55, 0x59, 0x0c, 
0x99, 0xcc, 0xc0, 0x95, 0xfc, 0xa9, 0xa5, 0xf0, 0xf0, 0xa5, 0xa9, 0xfc, 0x95, 0xc0, 0xcc, 0x99, 
0x95, 0xc0, 0xcc, 0x99, 0xf0, 0xa5, 0xa9, 0xfc, 0xfc, 0xa9, 0xa5, 0xf0, 0x99, 0xcc, 0xc0, 0x95, 
0x00, 0x55, 0x59, 0x0c, 0x65, 0x30, 0x3c, 0x69, 0x69, 0x3c, 0x30, 0x65, 0x0c, 0x59, 0x55, 0x00, 
};


/******************************************************************************/

/* calculate how many ones in byte */

static int yaffs_CountBits(unsigned char x)
{
	int r = 0;
	while(x)
	{
		if(x & 1) r++;
		x >>= 1;
	}
	return r;
}

/******************************************************************************/

/* calclulate 256 byte data ECC */

static void yaffs_ECCCalculate(const unsigned char *data, unsigned char *ecc)
{
	unsigned int i;
	
	unsigned char col_parity = 0;
	unsigned char line_parity = 0;
	unsigned char line_parity_prime = 0;
	unsigned char t;
	unsigned char b;
	
	for(i = 0; i < 256; i++)
	{
		b = column_parity_table[*data++];
		col_parity ^= b;

		if(b & 0x01) // odd number of bits in the byte
		{
			line_parity ^= i;
			line_parity_prime ^= ~i;
		}
		
	}
	
	ecc[2] = (~col_parity) | 0x03;
	
	t = 0;
	if(line_parity       & 0x80) t |= 0x80;
	if(line_parity_prime & 0x80) t |= 0x40;
	if(line_parity       & 0x40) t |= 0x20;
	if(line_parity_prime & 0x40) t |= 0x10;
	if(line_parity       & 0x20) t |= 0x08;
	if(line_parity_prime & 0x20) t |= 0x04;
	if(line_parity       & 0x10) t |= 0x02;
	if(line_parity_prime & 0x10) t |= 0x01;
	ecc[1] = ~t;
	
	t = 0;
	if(line_parity       & 0x08) t |= 0x80;
	if(line_parity_prime & 0x08) t |= 0x40;
	if(line_parity       & 0x04) t |= 0x20;
	if(line_parity_prime & 0x04) t |= 0x10;
	if(line_parity       & 0x02) t |= 0x08;
	if(line_parity_prime & 0x02) t |= 0x04;
	if(line_parity       & 0x01) t |= 0x02;
	if(line_parity_prime & 0x01) t |= 0x01;
	ecc[0] = ~t;

#if CONFIG_YAFFS_ECC_WRONG_ORDER
	// Swap the bytes into the wrong order
	t = ecc[0];
	ecc[0] = ecc[1];
	ecc[1] = t;
#endif 
}

/******************************************************************************/

/* check and corect 256 byte data ECC */

static int yaffs_ECCCorrect(unsigned char *data, unsigned char *read_ecc, const unsigned char *test_ecc)
{
	unsigned char d0, d1, d2; // deltas 

	d0 = read_ecc[0] ^ test_ecc[0];
	d1 = read_ecc[1] ^ test_ecc[1];
	d2 = read_ecc[2] ^ test_ecc[2];
	
	
	
	if((d0 | d1 | d2) == 0)
	{
		// no error
		return 0;
	}
	
	if( ((d0 ^ (d0 >> 1)) & 0x55) == 0x55 &&
	    ((d1 ^ (d1 >> 1)) & 0x55) == 0x55 &&
	    ((d2 ^ (d2 >> 1)) & 0x54) == 0x54)
	{
		// Single bit (recoverable) error in data

		unsigned byte;
		unsigned bit;

#if CONFIG_YAFFS_ECC_WRONG_ORDER
		// swap the bytes to correct for the wrong order
		unsigned char t;
		
		t = d0;
		d0 = d1;
		d1 = t;
#endif
		
		bit = byte = 0;
		
		
		if(d1 & 0x80) byte |= 0x80;
		if(d1 & 0x20) byte |= 0x40;
		if(d1 & 0x08) byte |= 0x20;
		if(d1 & 0x02) byte |= 0x10;
		if(d0 & 0x80) byte |= 0x08;
		if(d0 & 0x20) byte |= 0x04;
		if(d0 & 0x08) byte |= 0x02;
		if(d0 & 0x02) byte |= 0x01;

		if(d2 & 0x80) bit |= 0x04;
		if(d2 & 0x20) bit |= 0x02;
		if(d2 & 0x08) bit |= 0x01;

		data[byte] ^= (1 << bit);
		
		return 1;
	}
	
	if((yaffs_CountBits(d0)+yaffs_CountBits(d1)+yaffs_CountBits(d2)) == 1)
	{
		// Reccoverable error in ecc
		
		read_ecc[0] = test_ecc[0];
		read_ecc[1] = test_ecc[1];
		read_ecc[2] = test_ecc[2];
		
		return 1;
	}
	
	// Unrecoverable error
	
	return -1;
	    

}

/******************************************************************************/

/* calclulate data ECC and write it to oob */

static void necc(void* data, void* oob)
{
    SPARE* spare = oob; unsigned ix;

    for(ix = 0; ix < 8; ix++)
	yaffs_ECCCalculate(data+ix*256,spare->ecc+ix*3);
}

/******************************************************************************/

/* correct ecc error if able */

static int neccCorrect(void* data, void* oob)
{
    unsigned char ecc[3]; unsigned err = 0, ix;

    SPARE* spare = (void*)oob;

    for(ix = 0; ix < 8; ix++)
    {
	yaffs_ECCCalculate(data+ix*256,ecc);
	err |= yaffs_ECCCorrect(data+ix*256,spare->ecc+ix*3,ecc);
    }
    return err;
}

/******************************************************************************/

/* create bad block table */

static int bbtCreate(char* msg, int doErase)
{
    unsigned ix, block, lastPercent = -1, thisPercent, pass, res, nBad = 0;

    memset(bbt,0,sizeof(bbt));

    for(block = first; block < last; block++)
    {
	thisPercent = ((block - first) * 100) / (last - first);

	if(thisPercent != lastPercent)
	{
	    printf("\r%s%i%c",msg,thisPercent,'%'); fflush(stdout);
	    lastPercent = thisPercent;
	}

	if(nandIsBad(block*NAND_PAGES_PER_BLOCK,&res) == -1)
	    goto fail;

	if(res && !doErase)
	    goto bad;

	if(nandIsBad(block*NAND_PAGES_PER_BLOCK+1,&res) == -1)
	    goto fail;

	if(res && !doErase)
	    goto bad;

	pass = 0;

recheck:
	for(ix = 0; ix < NAND_PAGES_PER_BLOCK; ix++)
	{
	    if(nandIsFree(block*NAND_PAGES_PER_BLOCK+ix,&res) == -1)
		goto fail;

	    if(!res)
	    {
		if(pass || nandErase(block*NAND_PAGES_PER_BLOCK) == -1)
		    goto mark_bad;

		pass = 1; goto recheck;
	    }
	}

	continue;

mark_bad:

	printf("mark block=%i bad, pass=%i\n\r",block,pass);
	memset(rw_oob,0,64);
	nandWrite(block*NAND_PAGES_PER_BLOCK,NULL,rw_oob);
	nandWrite(block*NAND_PAGES_PER_BLOCK+1,NULL,rw_oob);

bad:
	bbt[block / 32] |= (1 << (block % 32));
	nBad++;
    }

    printf("\r%s       ",msg); printf("\r%sOK (%d bad blocks)\n\r",msg,nBad);
    return 0;

fail:
    printf("ERROR\n\r");
    return -1;
}

/******************************************************************************/

/* return non-zero if block is damaged */

static inline int blkIsBad(unsigned block)
{
    return bbt[block / 32] & (1 << (block % 32));
}

/******************************************************************************/

/* fill chunk data pattern */

static void nfillData(unsigned* ptr, unsigned chunk)
{
    unsigned val, ix;

    for(ix = 0; ix < 2048/4; ix++)
    {
	val = (ix & 0x1ff) | (chunk << 9);
	ptr[ix] = val;
    }
}

/******************************************************************************/

/* fill chunk OOB data pattern for first write */

static void nfillOOB1(unsigned* ptr, unsigned chunk)
{
    SPARE* s = (void*)ptr;

    s->tags[0] = chunk;
    s->tags[1] = ~chunk;
    s->tags[2] = 0x12345678;
    s->tags[3] = 0x00000001;
    s->tags[4] = 0x80000000;
    s->tags[5] = 0xffffffff;
    s->tags[6] = 0xffffffff;
    s->tags[7] = 0xffffffff;
    s->tags[8] = 0xffffffff;
    s->tagByte0 = 0x55;
    s->tagByte1 = 0xaa;
    s->pageStatus = 0xff;
    s->blockStatus = 0xff;
}

/******************************************************************************/

/* fill chunk OOB data pattern for second write */

static void nfillOOB2(unsigned* ptr, unsigned chunk)
{
    SPARE* s = (void*)ptr;
    nfillOOB1(ptr,chunk);
    s->pageStatus = 0;
    s->tags[5] = 0x09abcdef;
    s->tags[6] = 0x6d3a770c;
    s->tags[7] = 0x55555555;
    s->tags[8] = 0xaaaaaaaa;
}

/******************************************************************************/

/* call rtn for each chunk */

static int nloop(char* msg, FUNCPTR rtn)
{
    unsigned block, page, chunk;
    unsigned lastPercent = -1, thisPercent;
    unsigned startTick, endTick, ms;

    startTick = tickGet();

    for(block = first; block < last; block++)
    {
	thisPercent = ((block - first) * 100) / (last - first);

	if(thisPercent != lastPercent)
	{
	    printf("\r%s%i%c",msg,thisPercent,'%'); fflush(stdout);
	    lastPercent = thisPercent;
	}

	if(blkIsBad(block))
	    continue;

	for(page = 0; page < NAND_PAGES_PER_BLOCK; page++)
	{
	    chunk = page+block*NAND_PAGES_PER_BLOCK;

	    if(rtn(chunk) == -1)
		goto fail;
        }
    }

    endTick = tickGet();
    ms = (endTick - startTick) * 1000 / boardClkRateGet();

    printf("\r%s       ",msg); printf("\r%sOK (%d.%03d sec)\n\r",msg,ms / 1000,ms % 1000);
    return 0;

fail:
    printf("ERROR\n\r");
    return -1;
}

/******************************************************************************/

/* first write pattern routine for nloop() */

static int nwriteRtn1(unsigned chunk)
{
    nfillData((void*)rw_data,chunk);
    nfillOOB1((void*)rw_oob,chunk);
    necc(rw_data,rw_oob);

    if(nandWrite(chunk,rw_data,rw_oob) == -1)
    {
        printf("chunk=%i ",chunk);
	return -1;
    }
    return 0;
}

/******************************************************************************/

/* update oob pattern routine for nloop() */

static int nwriteRtn2(unsigned chunk)
{
    nfillOOB2((void*)rw_oob,chunk);

    if(nandWrite(chunk,NULL,rw_oob) == -1)
    {
        printf("chunk=%i ",chunk);
	return -1;
    }
    return 0;
}

/******************************************************************************/

/* erase routine for nloop() */

static int neraseRtn(unsigned chunk)
{
    if(chunk % NAND_PAGES_PER_BLOCK)
	return 0;

    if(nandErase(chunk) == -1)
    {
        printf("chunk=%i ",chunk);
	return -1;
    }
    return 0;
}

/******************************************************************************/

/* compare pattern routine */

static int ncmp(unsigned* ck, unsigned* rw, int nLongs)
{
    int ix; int res = 0;

    for(ix = 0; ix < nLongs; ix++)
    {
	if(ck[ix] != rw[ix])
	{
	    printf("ix=%#x: write=0x%08x read=0x%08x\n",
		ix,ck[ix],rw[ix]);
	    res = -1;
	}
    }
    return res;
}

/******************************************************************************/

/* verify data and oob routine */

static int nverifyRtn(unsigned chunk, VOIDFUNCPTR dataRtn, VOIDFUNCPTR oobRtn)
{
    int res;
    dataRtn(ck_data,chunk);
    oobRtn(ck_oob,chunk);
    necc(ck_data,ck_oob);

    if(nandRead(chunk,rw_data,rw_oob) == -1)
    {
	printf("read 1 chunk=%i ",chunk);
	return -1;
    }
    res = neccCorrect(rw_data,rw_oob);
    if(res > 0)
    {
	printf("Correct 1: chunk=%i ",chunk);
    }
    else if(res < 0)
    {
	printf("Unrecoverable error: chunk=%i ",chunk);
	return -1;
    }
    if(ncmp((void*)ck_data,(void*)rw_data,2048/4))
    {
	printf("ncmp data 1 chunk=%i ",chunk);
    }
    if(ncmp((void*)ck_oob,(void*)rw_oob,64/4))
    {
	printf("ncmp oob 1 chunk=%i ",chunk);
	return -1;
    }
    if(nandRead(chunk,rw_data,NULL) == -1)
    {
	printf("read data chunk=%i ",chunk);
	return -1;
    }
    if(nandRead(chunk,NULL,rw_oob) == -1)
    {
	printf("read oob chunk=%i ",chunk);
	return -1;
    }
    res = neccCorrect(rw_data,rw_oob);
    if(res > 0)
    {
	printf("Correct 2: chunk=%i ",chunk);
    }
    else if(res < 0)
    {
	printf("Unrecoverable error 2: chunk=%i ",chunk);
    }
    if(ncmp((void*)ck_data,(void*)rw_data,512/4))
    {
	printf("ncmp data 2 chunk=%i ",chunk);
	return -1;
    }
    if(ncmp((void*)ck_oob,(void*)rw_oob,16/4))
    {
        printf("ncmp oob 2 chunk=%i ",chunk);
        return -1;
    }
    return 0;
}

/******************************************************************************/

/* verify first pattern routine for nloop() */

static int nverifyRtn1(unsigned chunk)
{
    return nverifyRtn(chunk,nfillData,nfillOOB1);
}

/******************************************************************************/

/* verify updated pattern routine for nloop() */

static int nverifyRtn2(unsigned chunk)
{
    return nverifyRtn(chunk,nfillData,nfillOOB2);
}

/******************************************************************************/

/* verify chunk is free routine for nloop() */

static int nfreeRtn(unsigned chunk)
{
    int res;
    if(nandIsFree(chunk,&res) == -1 || !res)
    {
	printf("nfree: chunk=%i ",chunk);
	return -1;
    }
    return 0;
}

/******************************************************************************/

/* single test pass */

static int npass(char* msg)
{
    printf("********************************************************\n");
    printf("%s\n\r",msg);

    if(nloop("Write chunk pattern ....            ",(FUNCPTR)nwriteRtn1) == -1)
	return -1;
    if(nloop("Verify chunk pattern ....           ",(FUNCPTR)nverifyRtn1) == -1)
	return -1;
    if(nloop("Update chunk OOB pattern ....       ",(FUNCPTR)nwriteRtn2) == -1)
	return -1;
    if(nloop("Verify updated chunk pattern ....   ",(FUNCPTR)nverifyRtn2) == -1)
	return -1;
    if(nloop("Erase ....                          ",(FUNCPTR)neraseRtn) == -1)
	return -1;
    if(nloop("Verify ....                         ",(FUNCPTR)nfreeRtn) == -1)
	return -1;
    return 0;
}

/******************************************************************************/

/* flash test */

/*
    _first - starting block for test area
    nBlock - size of test area in blocks, if 0 then whole flash
    nmask - tests mask, if 0 then nmask=1
	0x01 - 1X, poll busy
	0x02 - 1X, wait irq
    doErase - if TRUE erase bad blocks before tests.
*/

int nandTest(unsigned _first, unsigned nBlocks, int nmask, int doErase)
{
    int res = 0;

    if(nandInit() == -1)
	return -1;

    if(!nBlocks)
    {
	_first = 0; nBlocks = nandBlocksPerChip*NAND_N_CHIPS;
    }

    if(!nmask)
	nmask = 1;

    first = _first;
    last = first + nBlocks;
    
    nmode = 0;

    nandWritePoll = nandErasePoll = TRUE;

    if(bbtCreate("Create Bad Block Table ....  ",doErase) == -1)
	goto fail;

    if(!(nmask & 0x01))
	goto pass2;

    if(npass("PASS1: POLL BUSY") == -1)
	goto fail;

pass2:
    if(!(nmask & 0x02))
	goto done;
    nmode = MODE_IRQ;
    nandWritePoll = nandErasePoll = FALSE;
    if(npass("PASS2: WAIT IRQ") == -1)
	goto fail;

done:
    return nandReset();

fail:
    res = -1;
    goto done;
}
