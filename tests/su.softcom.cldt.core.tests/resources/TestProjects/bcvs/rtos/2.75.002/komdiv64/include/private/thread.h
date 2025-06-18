/* thread.h */
/*
  19.05.99 НПЕ в threadUnlock() вставлен kernDISPATCH_CHECK()
  22.08.2000 nkigor в thread_context вставлен m_tryList
*/
#ifndef _THREAD_H
#define _THREAD_H

#include <sched.h>
#include <setjmp.h>
#include <pthread.h>
#ifdef KERNEL
#include <private/objcore.h>
#include <private/queue.h>
#include <private/clock.h>
#include <private/mutex.h>
#include <private/ios.h>
#include <sys/signal.h>

#include <arch/cpu.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KERNEL

/* Флаги текущего состояния потока */
#define PTHREAD_READY           0x01 /* Поток в очереди готовых */
#define PTHREAD_INTERMEDIATE    0x02 /* Поток в промежуточном списке (для включения в очередь готовых) */
#define PTHREAD_PENDED          0x04 /* Поток в очереди к ресурсу  */
#define PTHREAD_DELAYED         0x08 /* Поток в очереди к сис. таймеру */
#define PTHREAD_WAITED          0x10 /* Поток ожидает свершения события */

#define PTHREAD_RUNNING         (PTHREAD_READY|PTHREAD_INTERMEDIATE) /* Выполняющийся поток */
/* Поток в состоянии ожидания */
#define PTHREAD_BLOCKED         (PTHREAD_PENDED|PTHREAD_DELAYED|PTHREAD_WAITED|PTHREAD_PENDEDMP|PTHREAD_PENDEDS|PTHREAD_WAITEDRD)

#define PTHREAD_SUSPENDED       0x20 /* Выполнение потока приостановлено на неопр. время */
#define PTHREAD_PENDEDMP        0x40 /* Поток в очереди к ресурсу в общей памяти */
#define PTHREAD_PENDEDS         0x80 /* Ожидает сигнала                          */

#define PTHREAD_TERMINATION   0x0100 /* Поток находится в процессе завершения своей работы */
#define PTHREAD_WAITEDIO      0x0200 /* Поток ожидает завершения ввода/вывода */
#define PTHREAD_WAITEDSS      0x0400 /* Поток ожидает сигнала после sigsuspend */
#define PTHREAD_WAITEDRD      0x0800

/* Флаги пар-ров функционирования нити */
#define PTHREAD_INITIALIZED         0x01 /* Поток проинициализирован */
#define PTHREAD_DETACHED            0x02 /* Поток не связан с другими, т.е. не должен уведомлять
                                          * кого-либо (передавать код завершения) о своём окончании */
#define PTHREAD_ASYNCHRONOS         0x04 /* Поток допускает ассинхронное удаление
                                          * в противном случае только в точке анулирования */
#define PTHREAD_SYSTEM              0x08 /* Системный поток (не поддерживается)        */
#define PTHREAD_SAFE                0x10 /* Поток не может быть удален                 */
#define PTHREAD_CANCELLATION_POINT  0x20 /* Поток в точке анулирования потока          */
#define PTHREAD_LOCK_SIGNAL         0x40 /* Флага запрета обработки всех сигналов      */
#define PTHREAD_UNBREAKABLE         0x80 /* Поток не прерывается отладчиком */
#define PTHREAD_DEAD              0x0100 /* Поток завершен и ждёт join */
#define PTHREAD_SUSPEND_SAFE      0x0200 /* Поток не может быть приостановлен */
#define PTHREAD_MP                0x0400 /* Поток использует общую память */
#define PTHREAD_USER_STACK        0x0800 /* Поток использует предоставленный стек */
#define PTHREAD_AUTOSTART         0x1000 /* Поток после создания активизируется */
#define PTHREAD_NOIEEE754         0x2000

/* Флаги причины возврата потока из состояния ожидания */
#define PTHREAD_NOTHING_HAPPENED    0x00 /* ни чего не произошло */
#define PTHREAD_WAIT_OK             0x01 /* ожидание завершилось успешно */
#define PTHREAD_WAIT_TIMEOUT        0x02 /* прервано по таймауту */
#define PTHREAD_WAIT_SIGNAL         0x04 /* прервано приходом сигнала */
#define PTHREAD_WAIT_CANCEL         0x08 /* пришёл запрос на анулирование нити */
#define PTHREAD_WAIT_DESTROY        0x10 /* объект ожидания был разрушен, ожидание прервано */

/* Флаги запросов из других потоков и прерываний */
#define PTHREAD_REQUEST_CANCEL      0x01 /* запрос на удаление потока                  */
#define PTHREAD_REQUEST_SUSPEND     0x02 /* запрос на приостановку потока              */
#define PTHREAD_REQUEST_SLICEOUT    0x04 /* запрос на вытеснение потока
                                              по исчерпанию выделенного кванта времени */

/* Максимальное количество hooks на переключение */
#define THREAD_SWITCH_HOOKS   32
/* Максимальное количество hooks на удаление */
#define THREAD_REMOVE_HOOKS   32
/* Максимальное количество hooks на создание потока */
#define THREAD_CREATE_HOOKS   4


typedef struct thread_debug_info
{
    int     ldbState;
    int     ldbCause;
    void   *brRes;  /* элемент из списка остановов, с которого продолжить */
    void   *brCause;/* элемент из списка, который вызвал останов */
    void   *addr;   /* адрес останова */
    void   *regs;
} threadDB_INFO;

typedef struct thread_context
{
    int           m_errorNo;	    /* errorNo для нити */
    struct io_fd  m_stdFD[3];	    /* дескрипторы файлов для stdin,stdout,stderr  */
    void         *m_stdF[3];	    /* дескрипторы файлов для stdin,stdout,stderr  */
    threadREGS    m_regs;         /* регистры процессора */
    void         *m_fpContext;	  /* указатель на контекст сопроцессора */
/* nkigor 22.08.2000 */
    void         *m_tryList;	    /* try-catch для нити */
    void         *m_stackAddr;	  /* Адес стека сохранения */
    int           m_fpSticky;
    int           m_isrExit;      /* поток прерван прерыванием*/
} threadCONTEXT;

typedef struct thread_waited_block
{
    queHEAD     *m_pQue;         /* очередь, в которую включена нить в ожидании        */
    u_long       m_parm;         /* дополнительные данные об предмете ожидания         */
    volatile u_int m_retCause;   /* код причины завершения ожидания                    */
    u_long       m_retValue;     /* возвращаемое значение после ожидания               */
    u_long       m_info;
} threadWAITED;

typedef struct
{
    void       *tht_pointer;    /* используется для передачи параметров функции
                                   вызываемой при регистрации события трассировки      */
//    int         m_size;       /*размер буфера в словах(4б)*/
//    int         m_recfill;    /*использовано слов в буфере стека вызовов*/
//    int         m_nesting;    /*глубина стека вызовов*/
} threadTRACE;

/* Описатель завещания, которое следует выполнить при экстренном прекращении деятельности */
struct thread_testament {
   void     (*function)(u_long,u_long);/* функция, которую следует выполнить           */
   u_long   arg1;                      /* первый аргумент                              */
   u_long   arg2;                      /* второй аргумент                              */
};
/* Обслуживание завещания потока */
/* Проверка наличия у потока завещания */
#define threadIsTestament(thread)                           \
   ( thread->m_testament.function != NULL )
/* Передать завещание от одного потока другому */
#define threadMoveTestament(from,to)                        \
{                                                           \
   assert( threadIsTestament(from) );                       \
   to->m_testament = from->m_testament;                     \
}


typedef struct thrdControl
{
    objHEADER	 m_objHdr;      /* заголовок нити как объекта */

    quePARM_NODE    m_readyNode;   /* узел для вкл-я в очередь готовых */
    sdlNODE         m_intermNode;  /* узел для включения в промежуточный список к очереди готовых */
    quePARM_NODE    m_pendNode;    /* узел для вкл-я в очередь ожидания */
    clockWAIT_NODE  m_timerNode;   /* узел для вкл-я в очередь к сис. таймеру */

    threadWAITED    m_wait;        /* объект ожидания */

    void      *(*m_startPoint)(void *); /* точка старта */
    void        *m_startArg;    /* параметр для старта */

    void        *m_stackAddr;    /* Адрес стека */
    size_t       m_stackSize;    /* Размер стека */

    volatile u_long m_status;    /* текущее состояние нити */
    u_long       m_state;        /* параметры функционирования нити */
    u_long       m_request;      /* флаги запросов к потоку из других потоков */

    u_long       m_curPriority;  /* текущий приоритет */
    u_long       m_regPriority;  /* приоритет по умолчанию */
    u_long       m_inhPriority;  /* приоритет наследуемый */
    int          m_schedpolicy;  /* политика планирования работы нити */
    struct sched_param m_schedparam; /* параметры для планирования */

    u_short      m_mutexCnt2;  /* число захваченных нитью семафоров взаимного исключения не используется*/
    u_short      m_lockCnt;    /* счетчик вложенности выданых запретов диспетчеризации */
    queHEAD      m_join;       /* очередь нитей ждущих окончания данной */

    int          m_sigProcessing; /* 0 или номер сигнала, если находимся в его обработчике */
    sigset_t     m_sigBlocked; /* маска блокируемых сигналов */
    volatile sigset_t m_sigPending; /* маска сигналов ожидающих обработки  */
    sdlHEAD      m_sigInfoPending; /* информация для задержанных сигналов */

    u_long       m_threadSlice; /* промежуток времени для RR */
    u_long       m_threadTicks; /* общее число тиков работы нити */

    sdlHEAD      m_cleanupStack; /* */
    int          m_exitCode;	/* код завершения, передается exit () */

    threadCONTEXT m_context; /* контекст нити */

    void         *m_stackAddrOrig;  /* Начальный адрес предоставленного стека */
    void         *m_mempoolAddr;  	/* Адрес описателя пула памяти */

    threadDB_INFO m_debugInfo; /* информация для отладчика */
    void        **m_dataKeys;
    void         *m_pRPC;

    void         *m_mptcb;  /*указатель на TCB в общей памяти*/
    int           m_User;
    sdlHEAD       m_mtxList; /*список захваченных мьютексов*/
    threadTRACE   m_traceInfo; /*сведения трассировки*/
    volatile struct thread_testament
                  m_testament;
} threadTCB;

/*------------------- threadSetTestament - установить завещание потока ------------------
 * Аргументы:
 *   pThread - указатель на описатель потока;
 *   fun     - указатель на функцию;
 *   a1      - первый аргумент;
 *   a2      - второй аргумент.
 * Описатель потока нужен для обработки извещения как задание планировщику
 */
static inline void threadSetTestament( threadTCB *thread, void (*fun)(), void* a1, void* a2 )
{
//   assert( thread->m_testament.function == NULL );
   thread->m_testament.arg1 = (u_long)a1;
   thread->m_testament.arg2 = (u_long)a2;
   thread->m_testament.function =
      (void (*)(u_long,u_long))fun;
}
/*------------------- threadUnsetTestament - отменить завещание потока ------------------
 * Аргумент:
 *   pThread - указатель на описатель потока;
 */
static inline void threadUnsetTestament( threadTCB *thread )
{
//   assert( thread->m_testament.function );
   thread->m_testament.function = NULL;
}

extern objCONTROL_ID threadControlId;

#define threadIS_CURRENT( theThread )   (theThread==threadExecuting)
#define threadSET_STATE( thread, state )  (thread)->m_state=(state)
#define threadSET_STATUS( thread, status )  (thread)->m_status=(status)

#define threadIS_PRIORITY_VALID( prio ) \
	 ( prio >= SCHEDULER_MINIMUM_PRIORITY && prio <= SCHEDULER_MAXIMUM_PRIORITY )
#define threadSET_PRIORITY( thread, prio )  (thread)->m_curPriority=(u_long)(prio)
#define threadPRI_POSIX_TO_CORE(prio) (u_long)(prio)
#define threadPRI_CORE_TO_POSIX(prio) (int)(prio)
/*является ли поток владельцем мьютекса*/
#define threadMUTEX(th) sdlIs_Not_empty(&(th)->m_mtxList)
/*подтверждение запроса на CANCEL в особых случаях (MP)*/
/*может, добавить еще TERMINATE*/
#define threadCANCEL_ACKNOWLEDGE(thread) \
  if( (thread->m_request & PTHREAD_REQUEST_CANCEL)  \
    && !(thread->m_state & PTHREAD_SAFE) ) \
      thread->m_wait.m_retCause |= PTHREAD_WAIT_CANCEL

int         threadInitModule(void);
threadTCB  *threadAllocate ( u_long stackSize );
int         threadRemove (threadTCB * theThread);
void        threadInitContext(threadTCB *theThread);
int         threadSetPriority ( threadTCB *thread, int prio );
threadTCB  *threadGet( pthread_t thread, int *err );
void       *threadStackAlloc( threadTCB *pThread, size_t nSize );
void        threadIoStdSet(pthread_t thread, int fd, int newfd);
int         threadIoStdGet(pthread_t thread, int fd);

/* поддержка hooks */
enum typeThreadHooks {
  TYPE_HOOK_THREAD_SWITCH = 1,
  TYPE_HOOK_THREAD_REMOVE,
  TYPE_HOOK_THREAD_CREATE
};
#define     threadSwitchHookAdd(func)       threadSetHook( TYPE_HOOK_THREAD_SWITCH, func )
#define     threadSwitchHookDelete(func)    threadUnsetHook( TYPE_HOOK_THREAD_SWITCH, func )
#define     threadRemoveHookAdd(func)       threadSetHook( TYPE_HOOK_THREAD_REMOVE, func )
#define     threadRemoveHookDelete(func)    threadUnsetHook( TYPE_HOOK_THREAD_REMOVE, func )
#define     threadCreateHookAdd(func)       threadSetHook( TYPE_HOOK_THREAD_CREATE, func )
#define     threadCreateHookDelete(func)    threadUnsetHook( TYPE_HOOK_THREAD_CREATE, func )
#define     threadIS_TYPEHOOK_VALID(type) \
  (    type == TYPE_HOOK_THREAD_SWITCH \
    || type == TYPE_HOOK_THREAD_REMOVE \
    || type == TYPE_HOOK_THREAD_CREATE \
  )
/*списки hooks*/
extern funcptr_t threadSwitchHooks[];
extern funcptr_t threadRemoveHooks[];
extern funcptr_t threadCreateHooks[];

int         threadSetHook( int type, funcptr_t funcHook );
int         threadUnsetHook( int type, funcptr_t funcHook );

void fileDestroyF(void **f);
int fileCreateF(void **f);



#define threadLock() ++threadExecuting->m_lockCnt

#define threadUnlock() \
    if ( --threadExecuting->m_lockCnt <= 0 ) { \
        threadExecuting->m_lockCnt = 0; \
        kernDISPATCH_CHECK(); \
    }
/* Если поток стоит в очереди к ресурсу, меняем его положение в очереди
 * в соответствии с новым приоритетом.
 * Если ресурс - мьютекс с протоколом PTHREAD_PRIO_INHERIT , то
 * при повышении приоритета надо рассмотреть владельца.
 */
static __inline threadTCB* __UNUSED_F
ThreadPriorityPended( threadTCB* pThread, u_long prio )
{
    queHEAD   *pQue;
    objHEADER *pObj;
    if ( pThread->m_status & PTHREAD_PENDED )
    {
      /*  Поток неработоспособен и находится в очереди */
      pQue = pThread->m_wait.m_pQue;       /* очередь, в которой находится поток */
      /* Изменение положения потока в очереди к ресурсу */
      /* !!!Здесь бы надо не вытаскивать и вставлять - это при понижении
       * приоритета неправильно
       */
      queLOCK(pQue);
      queUpdateParm( pQue, (queNODE*)&pThread->m_pendNode, prio );
      queUNLOCK_DPC(pQue);
//      pQue->m_functions->m_updateRtn( pQue, (queNODE *) &pThread->m_pendNode, prio );
      pObj = pQue->m_owner;      /* владелец очереди, в которой находится поток  */
      if( !pObj )
          return NULL;

      if ( objCHECK( mtxObjControlId, pObj ) == 0
          && mtxPROTOCOL((mtxCONTROL *)pObj) == PTHREAD_PRIO_INHERIT )
      {
        /* Поток стоит в очереди к мьютексу, с протоколом PTHREAD_PRIO_INHERIT
          * владельцу, возможно следует повысить приоритет
          */
        pThread = (threadTCB *)(mtxOWNER((mtxCONTROL*)pObj)); /* владелец мьютекса */
        if( pThread->m_curPriority < prio ) {
          pThread->m_inhPriority = prio;
          return pThread;
        }
      }
    }
    return NULL;
}
#define PTHREAD_SUSPEND_ENABLE	0 
#define PTHREAD_SUSPEND_DISABLE 1 

void pthread_testsuspend( void );
int pthread_setsuspendstate( int  state, int *oldstate );

/* Дополнительный флаг возможность выполнения операций с плавающей арифметики */
#define PTHREAD_FPU_ENABLE  0 /* Нить может работать с сопроцессором */
#define PTHREAD_FPU_DISABLE 1 /* Работа с сопроцессором запрещена */

/* Дополнительный флаг устанавливающий должна ли нить быть активизирована при создании */
#define PTHREAD_CREATE_ACTIVE    0 /* Нить будет активизирована при создании */
#define PTHREAD_CREATE_SUSPENDED 1 /* Нить не будет активизирована, для её запуска надо выполнить RESUME */
#define PTHREAD_CREATE_DORMANT   2 /* Нить не будет активизирована, для её запуска надо выполнить START */

/* Дополнительный флаг использования нитью общей памяти */
#define PTHREAD_MP_ENABLE  0 /* Нить может использовать MP */
#define PTHREAD_MP_DISABLE 1 /* Нить не использует MP */

/* Дополнительный флаг поведения нити в точке останова */
#define PTHREAD_BREAK_ENABLE  0 /* Нить может останавливаться */
#define PTHREAD_BREAK_DISABLE 1 /* Нить не может останавливаться */

/* Дополнительный флаг допустимости трассировки нити */
#define PTHREAD_TRACE_ENABLE  1 /* Нить может трассироваться */
#define PTHREAD_TRACE_DISABLE 2 /* Нить не может трассироваться */

#else

typedef struct thrdControl  threadTCB;

#endif /* KERNEL */
int         threadSuspend(threadTCB *theThread);
int         threadResume(threadTCB *theThread);



#ifdef __cplusplus
}
#endif

#endif /* _THREAD_H */
