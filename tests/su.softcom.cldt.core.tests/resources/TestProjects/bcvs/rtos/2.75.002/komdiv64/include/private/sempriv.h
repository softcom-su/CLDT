/* sempriv.h */

#ifndef _SEMPRIV_H
#define _SEMPRIV_H

#include   <hash.h>
#include   <private/thread.h>
#include   <private/queue.h>
#include   <private/mutex.h>
#include   <private/kernel.h>



struct semPxControl
{
    objHEADER           m_objHdr;
    struct 	semControl *m_psem;   /*указатель на описатель семафора*/
    int                 m_named;
    unsigned long       m_idn;    /* идентификатор узла в таблице имен
                                   * используется только в семафорах MP
                                   */
    int                 m_attach; /* число открытий */
};

#define errSET_AND_RETURN_PX( err ) \
					 {	errno =  err ;	return (sem_t *) -1;  }

extern objCONTROL_ID      semPxObjControlId;


#ifdef __cplusplus
extern "C" {
#endif

#define GET_PNT_SEM( sem )  (sem)? (semCONTROL_ID)sem->m_psem : NULL

#define mpSEM_TABLE_RTN_INIT      0
#define mpSEM_TABLE_RTN_DSTR      1
#define mpSEM_TABLE_RTN_WAIT      2
#define mpSEM_TABLE_RTN_TRYWAIT   3
#define mpSEM_TABLE_RTN_TIMEDWAIT 4
#define mpSEM_TABLE_RTN_POST      5
#define mpSEM_TABLE_RTN_OPEN      6
#define mpSEM_TABLE_RTN_CLOSE     7
#define mpSEM_TABLE_RTN_GETVALUE  8
/*размер таблицы указателей на функции MP*/
#define mpSEM_TABLE_RTN_SIZE  9

/*значение mpSemPxControlOff до инициализации*/
#define MPSEM_NOT_INIT  0xffffffff
/*
#define SEM_IS_MP(sem) \
   ( (mpoffset_t)((objHEADER *)sem)->m_pClass == mpSemPxControlOff )
*/
/*является ли (sem_t)sem семафором в общей памяти*/
#define SEM_TYPE_MP     0xffffffff
#define SEM_TYPE_NOTMP  0x00000000
#define SEM_IS_MP(sem) \
   ( (sem)->ma_type == SEM_TYPE_MP )


enum {                     /* типы семафоров */
	SEM_TY_BINARY =       0,
	SEM_TY_COUNTING =     1,
	SEM_TY_MUTEX =        2,
	SEM_TY_PSHARED =      4,
	SEM_TY_POSIX =        8,
	SEM_TY_POSIX_NAMED =  16,
	SEM_TY_MP =           32
};

enum {                      /* атрибуты */
	SEM_Q_FIFO =            0,
	SEM_Q_PRIORITY =        1,
	SEM_DELETE_SAFE =       4
};

enum {
	SEM_SHOW_HEAD = 1,  /* вывести заголовок */
  SEM_SHOW_LIST = 2,  /* выводится список */
	SEM_SHOW_WAIT = 4   /* вывести ожидающих/семафоры с ожидающими */
};
enum {
	SEM_SHOW_LIST_WAIT = 1  /* список семафоров, имеющих непустой список
                           * ждущих потоков
                           */
};

/* по sem_t* получить адрес дескриптора */
#define objTO_SEMCONTROL(objId) (struct semPxControl*)((objId)->ma_sem)
/* семафор именованный? */
#define SEM_IS_NAMED(sem)   ((sem)->m_named == SEM_TY_POSIX_NAMED)

typedef struct semControl
{
   objHEADER   m_objHdr;
   u_short     m_type;
   u_short     m_attr;
   volatile int m_count;
   sem_t       m_desc;        /*указатель на дескриптор семафора*/
   queHEAD     m_waitHead;
} semCONTROL;

typedef  semCONTROL  *semCONTROL_ID;

/* Структура параметров функций sem_open */
struct sem_open_p {
  const char*   name;
  int           oflag;
  mode_t        mode;
  unsigned int  value;
  int           event;
};


extern objCONTROL_ID  semObjControlId;
extern hshCONTROL_ID  semHashNames;    /*таблица имен семафоров*/

int semInit( int num );
int semInitPrivate	(
      semCONTROL_ID  pSem,
      u_short        type,
      u_short        attr,
      unsigned int   count,
      void*          sem
);

/* ----------- semThreadRelease -------------
 * Функция semThreadRelease - освобождение потока, ждущего семафора
 *
 * Аргументы:
 *   pSem - адрес описателя семафора.
 */
static __inline__ void semThreadRelease( semCONTROL_ID  pSem )
{
   queNODE *node;
   threadTCB *pThread;
   queHEAD *que = &pSem->m_waitHead;

   while ((node = queExtract(que, NULL)) != NULL)
   {
      /* получаем TCB потока */
      pThread = (threadTCB*) queGetNodeOwner(node);
      pThread->m_status &= ~PTHREAD_PENDED;
      if (pThread->m_wait.m_retCause == PTHREAD_NOTHING_HAPPENED)
      {
         /* ничего еще не произошло - освобождаем */
         kernActivateThread(pThread, PTHREAD_WAIT_OK);
         return;
      }
   }
   /* не оказалось потока, который можно освободить */
   ++pSem->m_count;
}

int  semQueueCounter(	queHEAD *que, int counter );

/*средства просмотра семафоров*/
extern int (*semPxShowRtn)();
extern int (*semShowRtn)();

extern int    semPxInit( int n );
extern void   semPxDisplayList(int showType, int (func_p)(const char *fmt,...));
extern void   semShow( struct semPxControl *sem, int list );
extern int    semPxShowInit( void );

#ifdef __cplusplus
}
#endif

#endif /* _SEMPRIV_H */
