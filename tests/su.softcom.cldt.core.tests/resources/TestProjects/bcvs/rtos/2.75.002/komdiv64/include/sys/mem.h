#ifndef _SYS_MEM_H
#define _SYS_MEM_H

memPOOL_ID memPoolCreate ( void *pBuf, size_t bufSize );
memPOOL_ID memPoolCreateFlags ( void *pBuf, size_t bufSize, unsigned long uFlags );
int        memPoolDelete ( memPOOL_ID pPool );
void       memPoolInit ( memPOOL_ID pPool, void *pBuf, size_t bufSize,
                  unsigned long flags, unsigned long minBlock );
int        memPoolAdd ( memPOOL_ID poolId, void *pBuf, size_t bufSize );
void*      memAlloc ( memPOOL_ID poolId, size_t nBytes, int alig );
int        memFree ( memPOOL_ID poolId, void *pBlock );

int        memShow( int format );
int        pool( memPOOL_ID poolId, int format );
enum {
   MEM_SHOW_HEAD =         1,
   MEM_SHOW_FREE =         2,
   MEM_SHOW_REGION =       4
};
typedef struct mempool_status
{
    int         total_size;
    int         alloc_bytes;
    int         alloc_blk;
    int         free_bytes;
    int         free_blk;
    int         max_free_blk;
} memSTATUS;
int memGetStatus(memPOOL_ID pPool, memSTATUS  *info);

extern memPOOL_ID   memSysPoolId;

#endif  /* _SYS_MEM_H */
