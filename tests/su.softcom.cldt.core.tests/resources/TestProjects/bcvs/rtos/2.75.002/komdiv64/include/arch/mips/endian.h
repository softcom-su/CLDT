#ifndef _MIPS_ENDIAN_H
#define _MIPS_ENDIAN_H

#define	BYTE_ORDER	BIG_ENDIAN 

#ifndef __ASSEMBLER__
typedef struct {
	long	hi;
	unsigned long lo;
} es_longlong; /* endian safe long-long */
#endif
#ifndef mips
#define mips 1
#endif
/*
 * Define the order of 32-bit words in 64-bit words.
 */
#define	_QUAD_HIGHWORD 0
#define	_QUAD_LOWWORD  1


#endif /*! _MIPS_ENDIAN_H */
