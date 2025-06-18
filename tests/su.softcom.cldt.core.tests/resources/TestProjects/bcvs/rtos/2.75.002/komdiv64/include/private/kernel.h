/*  kernel.h  */
/*
  28.05.99 НПЕ Коррекция kernDispatchEnable
*/
#ifndef _KERNEL_H
#define _KERNEL_H

#include <private/clock.h>
#include <private/thread.h>

#ifdef KERNEL

extern queHEAD          kernReadyThreadQue;       /* Очередь потоков, готовых к исполнению */
extern sdlHEAD          kernIntermediateList;     /* Промежуточный список */

extern int              kernStateReady;           /* Состояние системы */
extern volatile int     threadDispatchLevel;      /* Уровень разрешения планирования потоков */
                                                  /* 0 - планирование разрешено, иначе нет */
extern volatile int     threadDispatchNecessary;  /* Требование на выполнение планирования */
extern threadTCB       *threadExecuting;          /* Указатель на текущий поток */
extern unsigned long    kernThreadSlice;          /* Продолжительность активности для round robin */


extern funcptr_t      kernCheckOnExitRtn;
extern vfuncptr_t  kernExitRtn;

#define kernDISPATH_NECESSARY  0x01
#define kernEXEC_SIGNAL        0x02    /* запрос на обработку сигналов                 */

#define kernCHECK_INTERMEDIATE 0x10    /* запрос на проверку промежуточной очереди     */
#define kernEXEC_REQUEST       0x20    /* запросы потока к планировщику                */
#define kernRESOLVE_SIGNAL     0x40
#define kernDPC_JOB            (kernCHECK_INTERMEDIATE|kernEXEC_REQUEST|kernRESOLVE_SIGNAL)

#define threadSetRetunValue(pThread, retValue) pThread->m_wait.m_returnValue=retValue

#define kernDISPATCH_ENABLE()  --threadDispatchLevel
#define kernDISPATCH_DISABLE() ++threadDispatchLevel
#define kernDISPATCH_SET_LEVEL(level) threadDispatchLevel = level
#define kernDISPATCH_CHECK() \
    if ( threadDispatchNecessary && threadDispatchLevel <=0 ) { \
        kernDISPATCH_SET_LEVEL(1); \
        kernDispatchThread(); \
    }

/*------------------- kernDispatchDisable - запрещение планирования -------------------*/
#define kernDispatchDisable kernDISPATCH_DISABLE
/*-------------------- kernDispatchEnable - разрешение планирования -------------------*/
#define kernDispatchEnable() \
    if (threadDispatchLevel <= 1) { \
        kernDISPATCH_SET_LEVEL(0);      \
        if ( threadDispatchNecessary ) { \
            kernDISPATCH_SET_LEVEL(1); \
            kernDispatchThread(); \
        } \
    } else kernDISPATCH_ENABLE();


void    kernDispatchThread();

#define WAIT_FOREVER  -1
#define NOWAIT         0

#define kernSTART_BLOCK_SECTION(thread,que) \
        thread->m_state |= PTHREAD_LOCK_SIGNAL; \
        thread->m_wait.m_retCause = PTHREAD_NOTHING_HAPPENED; \
        thread->m_wait.m_pQue = que;	\
        thread->m_wait.m_parm = 0;

#define kernSTART_BLOCK_FOREVER(que) \
        { \
            kernSTART_BLOCK_SECTION(threadExecuting,que) \
            threadExecuting->m_timerNode.m_Type = WAIT_FOREVER; \
        }
#define kernSTART_BLOCK_FOREVER_THREAD(t,que) \
        { \
            kernSTART_BLOCK_SECTION(t,que) \
            t->m_timerNode.m_Type = WAIT_FOREVER; \
        }

#define kernSTART_BLOCK_TIMESPEC(que, timeout_flag, timeout) \
        { \
            kernSTART_BLOCK_SECTION(threadExecuting,que) \
            threadExecuting->m_timerNode.m_Type = timeout_flag; \
            if ( timeout_flag == TIMER_ABSTIME ) threadExecuting->m_timerNode.m_abstime = *timeout; \
            else threadExecuting->m_timerNode.m_ticktime =  clock_gettick_t(&kernClock) + timeTimespecToTicks(&kernClock, timeout); \
        }

#define kernSTART_BLOCK_TICKS(que, timeout) \
        { \
            kernSTART_BLOCK_SECTION(threadExecuting,que) \
            threadExecuting->m_timerNode.m_Type = 0; \
            threadExecuting->m_timerNode.m_ticktime = clock_gettick_t(&kernClock) + timeout; \
        }


int     kernBlockThread ( unsigned long wait_flags );
int     kernBlockThreadAtCP ( unsigned long wait_flags );

#define kernInsertIntermList(pThread) \
    pThread->m_status |= PTHREAD_INTERMEDIATE; \
    sdlAppend (&kernIntermediateList, &pThread->m_intermNode);  \
    threadDispatchNecessary |= kernCHECK_INTERMEDIATE;

void kernActivateThread(threadTCB *pThread, u_int cause);
int  kernFlushPendQueue ( queHEAD *que, u_int retCause, unsigned long retValue);
void kernThreadSetPriority ( threadTCB *pThread );
void kernInitContext(threadTCB *theThread,int fpu);
void kernDequeueThread(threadTCB *pThread);
void kernThreadSetRequest(threadTCB *pThread, unsigned long req);
void kernThreadClearRequest(threadTCB *pThread, unsigned long req);
/*----------- kernThreadSuspend - установка запроса на приостановку потока ------------*/
#define kernThreadSuspend(pThread) \
    kernThreadSetRequest(pThread, PTHREAD_REQUEST_SUSPEND)
/* kernThreadResume - установка запроса на продолжение работы потока (после приостан.) */
#define kernThreadResume(pThread) \
    kernThreadClearRequest(pThread, PTHREAD_REQUEST_SUSPEND)

/* Процедура обработки отложенных запросов к системному таймеру */
void kernClockDpcRtn(queHEAD *que, int cnt);
/* Процедура обработчика узла потока из очереди к таймеру */
void kernClockThreadHook(threadTCB *pThread);

/* Процедура проверки на завершение работы процесса */
/* 0 - процесс должен быть завершён */
/* 1 - продолжить работу */
int kernCheckOnExit (void);
/* Процедура стандартного завершения работы ядра */
void kernExit (int status);
/* Процедура выдает номер версии ядра */
char *kernVersion (void);
/* Процедура генерации уведомления при наступлении события */
int kernSigNotify(struct sigevent *se, int si_code, void *parm);
/* Процедура наименование символа через хук */
char *kernResolveSymbol (int arg,void *v);
#endif /* KERNEL */

/* Установить значение кванта для режима планирования SCHED_RR*/
void kernSetThreadSlice( unsigned long quant );

void kernInit ( char *pMemBeg, char *pMemEnd, u_int intStackSize );
void kernOsStart( void *(*start_routine)( void * ) );


#endif /*_KERNEL_H*/
