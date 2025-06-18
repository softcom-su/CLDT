/* mempool.h */

#ifndef _PRIVATE_MEMPOOL_H
#define _PRIVATE_MEMPOOL_H


#ifdef __cplusplus
extern "C" {
#endif


#ifdef KERNEL

#include <string.h>
#include <private/objcore.h>
#include <private/sysdef.h>
#include <arch/cpu.h>

typedef struct mem_pool             /* memPOOL */
{
    objHEADER       mp_objHdr;
    unsigned long   mp_flags;	      /* Флаги */
    sdlHEAD         mp_regionList;	/* Список областей, включённых в пул */
    sdlHEAD         mp_freeList;	  /* Список свободных блоков */
    pthread_mutex_t mp_sem;	        /* Управляющий мютекс */
    unsigned long   mp_minBlock;	  /* Размер минимально выделяемого блока */
    /* Статистика */
    unsigned long mp_totalSize;	    /* total size of pool in bytes */
    unsigned long mp_curAllocBlk;	  /* current # of blocks allocated */
    unsigned long mp_curAllocMem;	  /* current # bytes allocated */
    unsigned long mp_blocksCnt;		  /* total # of blocks allocated */
    unsigned long mp_memCnt;	      /* total # of bytes allocated */
} memPOOL;

typedef struct mem_block_hdr	/* memHEADER */
{
    struct mem_block_hdr *bh_pPrev;
    unsigned long		bh_size;
    union {
        sdlNODE     node;
        void		*pthread;
    } bh_ref;
#if CPU == 	R4000
	void	*dummy;
#endif
#define bh_nodeRef      bh_ref.node
#define bh_threadRef    bh_ref.pthread
} memHEADER;

/* constants used in the size field of each block to
 *  indicate when a block is free or in use.
 */
#define memBLK_USED    1  /* indicates block is in use */
#define memBLK_FREE    0  /* indicates block is free */

#define memBLK_SIZE(pHdr) ( pHdr->bh_size & ~memBLK_USED )
#define memIS_BLK_USED(pHdr) ( (pHdr->bh_size & memBLK_USED) == memBLK_USED )
#define memIS_BLK_FREE(pHdr) ( (pHdr->bh_size & memBLK_USED) == memBLK_FREE )
#define memHEADER_SIZE       sizeof(memHEADER)
/* Опрос размера блока                                   */
#define memBLK_GET_SIZE memBLK_SIZE

/* macros for getting to next and previous blocks */
#define memNEXT_HDR(pHdr) ( (memHEADER *) ((unsigned long) (pHdr) + memBLK_SIZE(pHdr)) )
#define memPREV_HDR(pHdr) ( (pHdr)->bh_pPrev )

/* macros for converting block <-> header */
#define memHDR_TO_BLK(pHdr) ( (unsigned char  *) ((unsigned char  *)pHdr + memHEADER_SIZE ) )
#define memBLK_TO_HDR(pBlk) ( (memHEADER *) ((unsigned char  *)pBlk - memHEADER_SIZE ) )

/* macros for converting header <-> "node" */
#define memHDR_TO_NODE(pHdr) ( &((memHEADER *) pHdr)->bh_nodeRef )
#define memNODE_TO_HDR(pNode) ( (memHEADER *) ((unsigned char  *)pNode - ITEM_OFFSET(memHEADER, bh_nodeRef)) )

int  memInitModule ( unsigned char   *pBuf, unsigned long bufSize );
void memInitMutex ( memPOOL_ID poolId );

int  getAllocBlk(memPOOL_ID poolId);
void memShowInit();
int  memShowPool ( int nn, memPOOL_ID poolId, int type, int (func_p)(const char *fmt,...) );
int  memShow ( int type );

#define memSYS_ALLOC(nBytes) memAlloc(NULL, (unsigned long) nBytes, 0)
#define memSYS_FREE(pBlock) memFree(NULL, (unsigned char  *)pBlock)

/* MemPool flags */
#define memCTRL_SEMAPHORE        0x01  /* use semaphore */
#define memBLOCK_CHECK           0x02  /* check block */
#define memALLOC_ERROR_LOG       0x10  /* log msg if alloc error */
#define memALLOC_ERROR_SUSPEND   0x20  /* suspend task if alloc error */
#define memALLOC_ERROR_ERRNO     0x40
#define memFREE_ERROR_LOG        0x80  /* log msg if free error */
#define memFREE_ERROR_SUSPEND    0x100  /* suspend task if free error */
#define memFREE_ERROR_ERRNO      0x200
#define memALLOC_CHECK           0x400  /* проверять блок при выделении памяти */
#define memFREE_CHECK            0x800  /* проверять блок при сдаче памяти */
#define memALLOC_CLEAR           0x1000 /* очищать блок при выделении памяти */
/*
#define memERROR_NONE            0x00
#define memERROR_SUSPEND   (memBLOCK_CHECK|memALLOC_ERROR_SUSPEND | memFREE_ERROR_SUSPEND)
#define memERROR_ERRNO     (memBLOCK_CHECK|memALLOC_ERROR_ERRNO | memFREE_ERROR_ERRNO)
*/
/* MemPool alignment macros */
#define memALIGN_UP(adr)	ADDR_ALIGN_UP(adr, HEAP_ALLOC_ALIGNMENT)
#define memALIGN_DOWN(adr)	ADDR_ALIGN_DOWN(adr, HEAP_ALLOC_ALIGNMENT)
#define memALIGNED(adr)		ADDR_ALIGNED(adr, HEAP_ALLOC_ALIGNMENT)

/* Проверка принадлежности адреса области пула, заданной заголовком */
#define memREGION_ADDRESS(adr,pRegHdr) \
  ( ((unsigned long)pRegHdr < (unsigned long)adr) && \
  (((unsigned long)pRegHdr+pRegHdr->bh_size) > (unsigned long)adr) )

/* -------------- memIsBlockValid ---------------
 * Проверка состояния блока памяти.
 * Ссылка в следующем блоке памяти должна указывать на текущий.
 * Ссылка в предыдущем блоке также.
 */
static __inline__ int __UNUSED_F memIsBlockValid
  ( memHEADER *pHdr, memHEADER *pRegHdr )
{
  memHEADER*  pNextHdr;
  memHEADER*  pPrevHdr;

  pNextHdr = memNEXT_HDR (pHdr);
  pPrevHdr = memPREV_HDR (pHdr);
  if( !memREGION_ADDRESS(pNextHdr,pRegHdr) || !memREGION_ADDRESS(pPrevHdr,pRegHdr) )
    return 0;   /* адрес след.блока или предыдущего не лежит в области */
  if( (pHdr != memNEXT_HDR(pPrevHdr)) || (pHdr != memPREV_HDR(pNextHdr)) )
    return 0; /* ссылка следующего или предыдущего заголовка не *
               * указывает на текущий                           */
  return 1;
}

/* -------------- memClear ---------------
 * Очистка сдаваемого блока
 */
static __inline__ void memClear( void* pBlock, unsigned long size )
{
  memset( pBlock, 0xaa, size );
  memset( pBlock, 0x55, size );
}


extern unsigned long memDefFlags;     /* флаги, назначаемые пулу по умолчанию */
extern objCONTROL_ID memControlId;    /* указатель на описатель системного пула */

#define _memAlloc memAlloc
#define _memFree memFree

#endif /* KERNEL */

#ifdef __cplusplus
}
#endif
#endif /* _PRIVATE_MEMPOOL_H */
