/* cacheLib.h - cache library header */

#ifndef	__INCcacheLibh__
#define	__INCcacheLibh__

#include <version.h>

#if OS_VERSION >= 210
#ifdef	KERNEL
#include <sys/buf.h>
#endif
#endif

#include <system.h>

#if CPU == R4000
#define	CACHE_ALIGN_SIZE	32
#endif

#if CPU == R3000
#define	CACHE_ALIGN_SIZE	4
#endif

#define	CACHE_ROUND_UP(x)	ADDR_ALIGN_UP(x, CACHE_ALIGN_SIZE)
#define	CACHE_ROUND_DOWN(x)	ADDR_ALIGN_DOWN(x, CACHE_ALIGN_SIZE)
#define	CACHE_ALIGNED(x)	ADDR_ALIGNED(x, CACHE_ALIGN_SIZE)

/* function declarations */

void* cacheMalloc(int nBytes);
void* _cacheMalloc(int nBytes, int alignment);
void cacheFree(void* ptr);
void* cacheAlignMalloc(int nBytes);
void* _cacheAlignMalloc(int nBytes, int alignment);
void cacheAlignFree(void* ptr);
#if OS_VERSION >= 210
int cacheDataFlush(void* ptr, int nBytes);
int cacheDataInvalidate(void* ptr, int nBytes);
int cacheDataClear(void* ptr, int nBytes);
int cacheInstInvalidate(void* ptr, int nBytes);
int cacheTextUpdate(void* ptr, int nBytes);
int cacheDataLock(void* ptr, int nBytes);
int cacheInstLock(void* ptr, int nBytes);
int cacheDataUnlock(void* ptr, int nBytes);
int cacheInstUnlock(void* ptr, int nBytes);
void cacheInit();
void tlbEntrySet(unsigned mask_ix, unsigned hi, unsigned lo0, unsigned lo1);
#else
void cacheLibInit();
#endif

#if OS_VERSION >= 210
#ifdef	KERNEL
void cacheBufsInvalidate(struct buf* bp, unsigned nBytes);
void cacheBufsFlush(struct buf* bp, unsigned nBytes);
#endif
#endif

#if OS_VERSION >= 300
void cacheInit2(long poolBase, unsigned poolSize);
#endif

#endif	/* __INCcacheLibh__ */
