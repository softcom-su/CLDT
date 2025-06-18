/* crc32.h - CRC-32 related code header */

#ifndef	__INCcrc32h
#define	__INCcrc32h

/* function declarations */

void crcInit(void);
unsigned long crcGet(unsigned char *buf, int len);

#endif	/* __INCcrc32h */
