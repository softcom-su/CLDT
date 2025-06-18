/* hash.h */

#ifndef _HASH_H
#define _HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include   <pthread.h>
#include   <private/mutex.h>
#include   <private/objcore.h>
#include   <private/ddlist.h>

#define ID_NODE /*идентификатор - адрес узла*/

enum {
	HSH_SHOW_HEAD =         1,
  HSH_SHOW_ELEM =         4
};

#define CRC32_GEN	          0x04c11db7L  /* CCITT */
#define CRC32_MSB	          0x80000000L
#define NUM_HASH_DEFAULT	  4

enum {
	HSH_FIND_DATA    = 1,
	HSH_FIND_NAME    = 2,    /* совпадение имени                              */
	HSH_FIND_NODE    = 4,    /* совпадение имени и типа                       */
	HSH_FIND_PROTECT = 8,
	HSH_NAME_EXCL    = 16,   /* не допускается дублирование имен              */
	HSH_NODE_EXCL    = 32,   /* не допуск.дублир.пары имя/данное              */
	HSH_MTX_NOINIT   = 64    /* инициализация мьтекса многопоточности         */
};
/* Получить индекс в HASH */
#define hshGET_INDEX( idn )  ((unsigned int)( idn & 0xFFFF ))
#define hshGET_NUM( idn )    ( idn>>16 )           /* получить номер в HASH  */
#define hshPUT_IDENT( index, num )  ( (unsigned long)index | (num<<16) )

#define errSET_AND_RETURN_HSH( err ) \
				 { errno =  err;	return (hshCONTROL_ID)-1; }
#define errSET_AND_RETURN_HSHNODE( err ) \
				 { errno =  err;	return (hshNODE*)-1; }
#define HASHIDN_FAILED     0

#define hshGET_KEY(node)   (node)->m_idn
#define hshGET_DATA(node)  (node)->m_data
#define hshGET_NAME(node)  (node)->m_pstr
#define hshGET_OBJ(node)   (node)->m_pObj

typedef unsigned long  hashidn_t;
typedef unsigned long  hashdata_t;

typedef struct hshnode {
	ddlNODE         m_node;   /* обслуживает цепочку элементов с одним ключом */
	hashidn_t       m_idn;    /* идентификатор в таблице                      */
	char           *m_pstr;   /* адрес имени                                  */
	hashdata_t      m_data;   /* данные, связанные с объектом                 */
	void           *m_pObj;   /* помещаемый объект                            */
} hshNODE;

typedef struct {
	objHEADER        m_objCore;
	int              m_type;     /* режим занесения элементов                 */
	unsigned int	  m_nodes;    /* элементов в hash                          */
	unsigned int	  m_mask;
	unsigned long    m_count;    /* счетчик размещенных узлов                 */
	unsigned int     m_locate;   /* текущее число записей в таблице           */
	pthread_mutex_t  m_mutex;    /* семафор                                   */
	ddlHEAD         *m_pHshTbl;
} hshCONTROL;

typedef  hshCONTROL  *hshCONTROL_ID;

extern objCONTROL_ID  hshObjControlId;

extern int              hshInit( void );
extern int              hshDestroy( hshCONTROL_ID pHshCtrl );
extern hshCONTROL_ID    hshCreate ( int type, int size_hash );
extern hshNODE*         hshFindName (
                            hshCONTROL_ID p_hshHead,
                            char*         pName
                        );
extern hshNODE *hshFindNameAndData (
		hshCONTROL_ID  p_hshHead,
		char          *pName,
		unsigned long  data
	);
/* Вставить в таблицу элемент */
extern hshNODE *hshInsert (
		hshCONTROL_ID  p_hshHead,
		char *pName,
		unsigned long data,
		void *pObj
	);
/* Вставить в таблицу элемент с именем, для кот.не нужно сохранять */
extern hshNODE *hshInsertConstName (
		hshCONTROL_ID  p_hshHead,
		char *pName,
		unsigned long data,
		void *pObj
	);
extern int  hshRemoveName (
		hshCONTROL_ID  p_hshHead,
		char          *pName
	);
extern int  hshRemoveNameAndData (
		hshCONTROL_ID  p_hshHead,
		char          *pName,
		unsigned long data
	);

hshNODE *hshFindUnprotect (
		hshCONTROL_ID  p_hshHead,
		char          *pName,
		unsigned long  data,
		int mode
	);

#ifdef ID_NODE
# define hshFindIdUnprotect(p_hshHead,idn)    idn
#else
extern hshNODE *hshFindIdUnprotect (
		hshCONTROL_ID  p_hshHead,
		unsigned long idn
	);
#endif
#ifdef ID_NODE
# define hshFindId(p_hshHead,idn)    idn
#else
extern hshNODE *hshFindId (
		hshCONTROL_ID  p_hshHead,
		unsigned long idn
	);
#endif

#ifdef  ID_NODE
extern int  hshRemoveId (
		hshCONTROL_ID  p_hshHead,
    hshNODE *idn
	);
#else
extern int  hshRemoveId (
		hshCONTROL_ID  p_hshHead,
    unsigned long  idn
	);
#endif

extern int  hshRemove (
		hshCONTROL_ID  p_hshHead,
		char          *pName,
		unsigned long  data,
		int flags
	);
extern int  hshRemoveNode (
    hshCONTROL_ID  p_hshHead,
    hshNODE *node
    );

extern hshNODE *hshFindNameNext ( hshNODE *node );

extern void     hshShowInit( void );
extern int      hshLock (	hshCONTROL_ID  p_hshHead );
extern int      hshUnlock (	hshCONTROL_ID  p_hshHead );
extern hshNODE *hshEach (	hshCONTROL_ID  p_hshHead, funcptr_t func, void *arg );
extern void     hshShow( hshCONTROL_ID  p_hshHead, int showType );

#ifdef __cplusplus
}
#endif

#endif  /* _HASH_H */

