/*  mutex.h  */

#ifndef _PRIVATE_MUTEX_H
#define _PRIVATE_MUTEX_H

#include   <pthread.h>
#include   <private/objcore.h>
#include   <private/queue.h>

enum {
	MTX_SHOW_HEAD =         1,  /* вывести заголовок */
  MTX_SHOW_LIST =         2,  /* выводится список */
	MTX_SHOW_WAIT =         4,  /* вывести ожидающих/мьютексы с очередью */
	MTX_SHOW_LOCKED =       8   /* вывести мьютексы с владельцем */
};

enum {
	MTX_SHOW_LIST_WAIT = 1, /* список мьютексов, имеющих владельца */
	MTX_SHOW_LIST_LOCKED    /* список мьютексов, имеющих непустой список
                           * ждущих потоков
                           */
};


typedef struct  mtxControl {
	objHEADER     m_objHdr;
	objHEADER    *m_owner;
	int           m_recur;    /*счетчик рекурсивных захватов*/
	struct mutex_attr m_attr;
	queHEAD       m_waitHead;
	int           m_condCount;
	sdlNODE       m_ownerNode;
} mtxCONTROL;

typedef  mtxCONTROL  *mtxCONTROL_ID;

/* атрибуты мьютекса инициализированы? */
#define  mtxATTR_IS_INIT(attr) ((attr)->ma_init == TRUE)

#define  mtxPROTOCOL( mutex )       (mutex)->m_attr.ma_protocol
#define  mtxPRIOCEILING( mutex )    (mutex)->m_attr.ma_prioceiling
#define  mtxPROPERTY( mutex )       (mutex)->m_attr.ma_type
#define  mtxOWNER( mutex )          (mutex)->m_owner
/* мьютекс с протоколом PTHREAD_PRIO_PROTECT? */
#define  mtxIS_PROTECT( mutex ) \
( (mtxPROTOCOL(mutex) == PTHREAD_PRIO_PROTECT) )
/* мьютекс с протоколом PTHREAD_PRIO_INHERIT? */
#define  mtxIS_INHERIT( mutex ) \
( (mtxPROTOCOL(mutex) == PTHREAD_PRIO_INHERIT) )
/* мьютекс рекурсивный? */
#define  mtxIS_RECURSIVE( mutex ) \
( (mtxPROPERTY(mutex) == PTHREAD_MUTEX_RECURSIVE) )
/* мьютекс NORMAL? */
#define  mtxIS_NORMAL( mutex ) \
( (mtxPROPERTY(mutex) == PTHREAD_MUTEX_NORMAL) )
#define  mtxIS_LOCKED( mutex )  ( (mutex)->m_owner != NULL )
#define  mtxTHREAD_IS_OWNER( mutex ) \
( (pthread_t)((mutex)->m_owner) == threadExecuting )
/*мьютекс статической инициализации?*/
#define  mtxIS_INITIALIZER(mutex) *(mutex) == &__mutexInitializer

/* назначить поток владельцем мьютекса */
#define  mtxASSIGN_OWNER(mtx,th) \
  { \
		  mtxOWNER(mtx) = (objHEADER*) th; /* исп. поток становится влад. */\
		  mtx->m_recur = 1;     /* мьютекс захвачен 1 раз */ \
		  /* Поместим мьютекс в список мьютексов, захваченных потоком */\
		  sdlAppend ( &(th)->m_mtxList, &mtx->m_ownerNode ); \
  }
/* Проверка корректности приоритета потока в случае протокола PTHREAD_PRIO_PROTECT */
#define  mtxCHECK_PROTECT(mtx) \
  (!mtxIS_PROTECT(mtx))? 0 :   \
  (threadExecuting->m_curPriority>mtxPRIOCEILING(mtx))? EINVAL : 0
#if 0
#define  mtxCHECK_PROTECT(mtx) \
  if( mtxIS_PROTECT(mtx) ) {                                                              \
    if ( threadExecuting->m_curPriority > mtxPRIOCEILING(mtx) )                           \
    {                                                                                     \
	    kernDispatchEnable ();           /* разрешение планирования                      */ \
	    return EINVAL;                   /* приритет потока выше приоритета мьютекса     */ \
    }                                                                                     \
  }
#endif
/* повысить приоритет потоку, владеющему мьютексом,
 * если ставим в очередь поток с большим приоритетом, а мьютекс с
 * протоколом PTHREAD_PRIO_INHERIT
 */
#define  mtxCHECK_INHERIT(mtx) \
	if( mtxIS_INHERIT(mtx) ) {                                                             \
    threadTCB*    thread_holder;         /* указатель на описатель потока управления  */  \
    /* Протокол PTHREAD_PRIO_INHERIT - наследование приоритета */                         \
		thread_holder = (threadTCB*)mtxOWNER( mtx );       /* владелец мьютекса        */   \
    if ( threadExecuting->m_curPriority > thread_holder->m_curPriority ) {                \
			  /* Повышаем приоритет владельцу мьютекса */                                    \
        thread_holder->m_inhPriority = threadExecuting->m_curPriority ;                   \
        kernThreadSetPriority( thread_holder );                                           \
    }                                                                                     \
	}

/*способы захвата мьютекса*/
#define  mtxNOWAIT       (struct timespec*)-1    /*trylock*/
#define  mtxWAIT_FOREVER (struct timespec*)0     /*ждать*/
/*получить мьютекс по узлу, который служит для включения его в список владельца*/
#define mtxOWNNODE_TO_HDR(pNode) ( (objHEADER *) ((unsigned char*)pNode - ITEM_OFFSET(mtxCONTROL, m_ownerNode)) )

//extern  objCONTROL  mtxObjControl;
extern  objCONTROL_ID  mtxObjControlId;


extern int  mtxPxInit( void );
extern void mtxSupplyFirst( struct mtxControl *mutex );
extern int  mtxLockInternal (	mtxCONTROL *mtx	);
extern int  mtxUnlockInternal( mtxCONTROL *mtx );

void mtxUnlockCompletion( struct mtxControl *mutex );
/*int  pthread_mutexattr_setrecur( pthread_mutexattr_t *attr,	int recur );*/


/*средства просмотра мьютексов*/
extern funcptr_t mtxShowRtn;
extern int   mtxShowInit( void );
extern void  mtxDisplayList(int showType, int (func_p)(const char *fmt,...));
extern void  mtxShow( pthread_mutex_t mutex, int list );

#endif /* _PRIVATE_MUTEX_H */
