/*  private/cond.h  */

#ifndef _PRIVATE_COND_H
#define _PRIVATE_COND_H

#include    <pthread.h>
#include    <arch/endian.h>


#if BYTE_ORDER == BIG_ENDIAN
#  define CNDATTR_SIGNATURE       ('A'<<3*8)+('C'<<2*8)+('N'<<1*8)+'D' /* ACND */
#else
#  define CNDATTR_SIGNATURE       ('D'<<3*8)+('N'<<2*8)+('C'<<1*8)+'A'
#endif

enum
{
   CND_SHOW_HEAD = 1,      /* вывести заголовок                         */
   CND_SHOW_LIST = 2,      /* выводится список                          */
   CND_SHOW_WAIT = 4       /* вывести ожидающих/переменные с очередью   */
};

enum
{
   CND_SHOW_LIST_WAIT = 1  /* список перменных, имеющих непустой список
                              ждущих потоков                            */
};


typedef struct  cndControl {
	objHEADER           m_objHdr;
	pthread_mutex_t     m_pmutex;
	pthread_condattr_t  m_attr;
	queHEAD             m_waitHead;
} cndCONTROL;


typedef  cndCONTROL  *cndCONTROL_ID;

/* Атрибуты условной переменной инициализированы? */
#define  cndATTR_IS_INIT(attr)                                 \
   ((u_long)(attr)->ca_init == (u_long)CNDATTR_SIGNATURE)
/* Установить признак инициализации атрибутов */
#define  cndATTR_SET_INIT(attr)                                \
   (attr)->ca_init = (u_long)CNDATTR_SIGNATURE
#define  cndATTR_UNSET_INIT(attr)                              \
   (attr)->ca_init = (u_long)0

/* Мьютекс статической инициализации? */
#define  cndIS_INITIALIZER(cond)                               \
   *(cond) == &__condInitializer

#define COND_INIT_DONE( cond )                                 \
   if( cndIS_INITIALIZER(cond) )                               \
   { /* Имеем статическую инициализацию */                     \
       int rt;                                                 \
       rt = pthread_cond_init( cond, NULL );                   \
       if( rt != 0 ) return rt;                                \
   }

extern  objCONTROL      cndObjControl;
extern  objCONTROL_ID   cndObjControlId;
extern  funcptr_t       cndShowRtn;


extern int     cndPxInit( void );
/* Средства просмотра переменных условия */
extern int     cndShowInit( void );
extern void    cndDisplayList( int showType );
extern void    cndShow( pthread_cond_t cond, int list );

#endif /* _PRIVATE_COND_H */
