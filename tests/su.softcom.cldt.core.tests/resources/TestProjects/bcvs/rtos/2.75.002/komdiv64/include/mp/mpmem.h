/* mpmem.h*/

#ifndef _MPMEM_H
#define _MPMEM_H

#include <sys/types.h>

/*идентификатор пула общей памяти, который содержит общую память,
  находящуюся на ведущем процессоре
*/
#define  mpMEMPOOL_MASTER  mpMemPoolMaster
/*идентификатор пула общей памяти, который содержит общую память,
  находящуюся на данной процессорной плате
*/
#define  mpMEMPOOL_SELF    mpMemPoolSelf


#define      SLAVE_POOL_CREATE       1 /*slave создает собственный пул памяти*/
#define      SLAVE_POOL_ADD          2 /*slave расширяет пул ведущего ЦПУ*/

#define MP_ALIGNED  16
#define mpmemALIGN_UP(adr) \
  (((int) (adr) + (MPHEAP_ALLOC_ALIGNMENT - 1)) & ~(MPHEAP_ALLOC_ALIGNMENT - 1))
#define mpmemALIGN_DOWN(adr)	ADDR_ALIGN_DOWN(adr, MPHEAP_ALLOC_ALIGNMENT)
#define mpmemALIGNED(adr)		  ADDR_ALIGNED(adr, MPHEAP_ALLOC_ALIGNMENT)

/*mpMEMBLOCK_CHECK*/

struct mpmem_pool;
typedef struct mpmem_pool  mpMEMPOOL, *mpMEMPOOL_ID;

#ifdef KERNEL

#include <semaphore.h>
#include <mp/mpsdlist.h>
#include <mp/mpspin.h>
#include <mp/mpobjcore.h>

enum {
   MPMEM_SHOW_HEAD =         1,
   MPMEM_SHOW_FREE =         2,
   MPMEM_SHOW_REGION =       4
};

struct mpmem_pool
{
    objHEADER       objHdr;
    u_long           flags;	      /* Флаги */
    mpsdlHEAD       regionList;	  /* Список областей, включённых в пул */
    mpsdlHEAD       freeList;	    /* Список свободных блоков */
    sem_t           sem;	        /* Управляющий мютекс */
    u_long           minBlock;	    /* Размер минимально выделяемого блока */
/*    pthread_spinlock_t  m_lock; */    /* spin */
    /* Статистика */
    u_long totalSize;	    /* размер пула (байт) */
    u_long curAllocBlk;	  /* текущ.число выделенных блоков */
    u_long curAllocMem;	  /* распределено байтов */
    u_long blocksCnt;		  /* общее число выделенных блоков */
    u_long memCnt;	        /* общее число байтов */
};

typedef
struct mpmem_block_hdr
{
    struct mpmem_block_hdr *bh_pPrev;
    unsigned long		      bh_size;
    union {
        sdlNODE           node;
        void		         *pthread;
    } bh_ref;
#define bh_nodeRef      bh_ref.node
#define bh_threadRef    bh_ref.pthread
} mpmemHEADER;

/* укажем для всех типов модулей одну границу (по 206-му) */
#define MPHEAP_ALLOC_ALIGNMENT 8

#define mpmemBLK_USED    1  /* indicates block is in use */
#define mpmemBLK_FREE    0  /* indicates block is free */

#define mpMEMBLK_SIZE(pHdr)    ( ntohl(pHdr->bh_size) & ~mpmemBLK_USED )
#define mpMEMIS_BLK_USED(pHdr) ( (ntohl(pHdr->bh_size) & mpmemBLK_USED) == mpmemBLK_USED )
#define mpMEMIS_BLK_FREE(pHdr) ( (ntohl(pHdr->bh_size) & mpmemBLK_USED) == mpmemBLK_FREE )
#define mpMEMNEXT_HDR(pHdr)    ( (mpmemHEADER *) ((u_long) (pHdr) + mpMEMBLK_SIZE(pHdr)) )
#define mpMEMPREV_HDR(pHdr)    ( ((pHdr)->bh_pPrev == NULL)? \
    (mpmemHEADER *)NULL : (mpmemHEADER *)mpAdrLocal((u_long)(pHdr)->bh_pPrev) )
#define mpmemHEADER_SIZE       sizeof(mpmemHEADER)

/* преобразование block <-> header */
#define mpmemHDR_TO_BLK(pHdr) ( (unsigned char  *) ((unsigned char  *)pHdr + mpmemHEADER_SIZE ) )
#define mpmemBLK_TO_HDR(pBlk) ( (mpmemHEADER *) ((unsigned char  *)pBlk - mpmemHEADER_SIZE ) )

/* зкуобразование header <-> "node" */
#define mpmemHDR_TO_NODE(pHdr) ( &((mpmemHEADER *) pHdr)->bh_nodeRef )
#define mpmemNODE_TO_HDR(pNode) ( (mpmemHEADER *) ((unsigned char  *)pNode - ITEM_OFFSET(mpmemHEADER, bh_nodeRef)) )

/* MemPool flags */
#define mpmemCTRL_SEMAPHORE        0x01  /* use semaphore */
#define mpmemBLOCK_CHECK           0x02  /* check block */
#define mpmemALLOC_ERROR_SUSPEND   0x20  /* suspend task if alloc error */
#define mpmemFREE_ERROR_SUSPEND    0x100  /* suspend task if free error */

extern u_int   mpMemDefAlignment;
extern mpOBJCONTROL *mpMemControlId; /*описатель класса*/
extern mpoffset_t mpMemControlOff;  /*смещение описателя*/
extern mpMEMPOOL_ID  mpPoolDefault;    /*лок.адрес пула общей памяти, используемого по умолчанию*/
extern mpOBJCONTROLLOC_ID	  mpMemControlLocId; /*локальный описатель класса пулов*/
//extern mpMEMPOOL_ID  mpMemPoolSelf;

int  mpMemInitModule ( unsigned char  *pBuf, u_long bufSize );
void mpMemSetPoolDefault( mpMEMPOOL_ID poolId );
void mpPoolInitSem( mpMEMPOOL_ID poolId );
mpMEMPOOL *mpMemInit( int flags, size_t objsize );
int mpMemShowInit( void );

#endif /*KERNEL*/


extern mpMEMPOOL_ID  mpMemPoolMaster;
extern mpMEMPOOL_ID  mpMemPoolSelf;

mpMEMPOOL_ID
      mpMemPoolCreate ( unsigned char *pBuf, u_long bufSize );
void  mpMemPoolInit ( mpMEMPOOL_ID pPool, unsigned char *pBuf, u_long bufSize, u_long flags, u_long minBlock );
int   mpMemPoolAdd ( mpMEMPOOL_ID poolId, unsigned char *pBuf, u_long bufSize );
void *mpMemAlloc ( mpMEMPOOL_ID poolId, u_long nBytes, u_int alig );
int   mpMemFree ( mpMEMPOOL_ID poolId, void *pBlock );

int   mpmem( int format );
int   mppool( mpMEMPOOL_ID poolId, int format );

#endif /*_MPMEM_H*/
