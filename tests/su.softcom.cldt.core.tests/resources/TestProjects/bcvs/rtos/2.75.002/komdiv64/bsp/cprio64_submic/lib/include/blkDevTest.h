#ifndef __INCblkDevTesth
#define __INCblkDevTesth

#define	BDS_TIME	0x01
#define	BDS_WRITE	0x02
#define	BDS_RANDOM	0x04
#define	BDS_BUFFER	0x08
#define	BDS_K0_TO_K1	0x10

/* function declarations */

void blkDevSpeed(BLK_DEV* pDev, unsigned xfer, int mode, unsigned stop);

STATUS blkDevTest(BLK_DEV* pDev, unsigned blkSize, unsigned blkCount,
    unsigned char* buf, int ignore);

STATUS blkDevTestShow(BLK_DEV* pDev, unsigned blkSize, unsigned blkCount,
    unsigned char* buf, int ignore, char* name);

int blkDevExtTest(char* name, BLK_DEV* bd, unsigned secPerBlk, unsigned nBlocks,
    unsigned pattern, unsigned wrOrder, unsigned rdOrder, unsigned* buf,
    unsigned options, unsigned offset);

#endif /* __INCblkDevTesth */
