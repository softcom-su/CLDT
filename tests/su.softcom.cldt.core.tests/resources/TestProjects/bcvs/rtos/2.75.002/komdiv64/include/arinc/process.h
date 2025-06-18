/* arinc/process.h */
#ifndef _ARINC_PROCESS_H
#define _ARINC_PROCESS_H

#include <sys/apextypes.h>


/*
 *--------------------------------------------------------------------------
 * PROCESS constant and type definitions and management services
 *--------------------------------------------------------------------------
 */
/*!!TODO!! ARINC 653 требует определить SYSTEM_LIMIT_NUMBER_OF_PROCESSES, а также
  MAX_NUMBER_OF_PROCESSES (через SYSTEM_LIMIT_NUMBER_OF_PROCESSES)*/
/* Максимальное число потоков */
//#define MAX_NUMBER_OF_PROCESSES SYSTEM_LIMIT_NUMBER_OF_PROCESSES
/* Максимальное значение приоритета */
#define MIN_PRIORITY_VALUE       1
/* Максимальное значение приоритета */
#define MAX_PRIORITY_VALUE      63  /*239*/
/* Максимальное уровня блокировки   */
#define MAX_LOCK_LEVEL          16

//typedef APEX_INTEGER WAITING_RANGE_TYPE;
typedef APEX_INTEGER  PRIORITY_TYPE;             /* приоритет потока управления        */

/* Состояние потока */
typedef
  enum {
      DORMANT = 0,                               /* спящий                             */
      READY   = 1,                               /* работоспособен                     */
      RUNNING = 2,                               /* выполняющийся                      */
      WAITING = 3                                /* ожидающий                          */
  } PROCESS_STATE_TYPE;

/* Действия в случае превышения лимита времени */
typedef enum { SOFT = 0, HARD = 1 } DEADLINE_TYPE;

/* Описатель атрибутов потока */
typedef
struct athread_attr {
      PROCESS_NAME_TYPE   NAME;                  /* имя потока                         */
      SYSTEM_ADDRESS_TYPE ENTRY_POINT;           /* адрес точки входа потока           */
      STACK_SIZE_TYPE     STACK_SIZE;            /* размер стека потока                */
      PRIORITY_TYPE       BASE_PRIORITY;         /* начальный приоритет потока         */
      SYSTEM_TIME_TYPE    PERIOD;                /* период потока                      */
      SYSTEM_TIME_TYPE    TIME_CAPACITY;         /* лимит времени работы потока        */
      DEADLINE_TYPE       DEADLINE;    /* действия в случае превышения лимита времени  */
} PROCESS_ATTRIBUTE_TYPE;

/* Структура описателя состояния потока */
typedef  struct process_status {
      PROCESS_ATTRIBUTE_TYPE ATTRIBUTES;         /* описатель атрибутов потока         */
      PRIORITY_TYPE          CURRENT_PRIORITY;   /* текущий приоритет потока           */
      SYSTEM_TIME_TYPE       DEADLINE_TIME;      /* срок завершения работы потока      */
      PROCESS_STATE_TYPE     PROCESS_STATE;      /* состояние потока                   */
      SYSTEM_TIME_TYPE       RELPOINT_TIME;      /* следующая release point            */
} PROCESS_STATUS_TYPE;

#ifndef KERNEL
#define CREATE_PROCESS(attr,id,ret)         \
    *(ret) = athreadCreate( id, attr )
#define GET_PROCESS_ID(name,id,ret)         \
    *(ret) = athreadGetId( name, id )
#define GET_PROCESS_STATUS(id,status,ret)   \
    *(ret) = athreadGetStatus( id, status )
#define SET_PRIORITY(id,prio,ret)           \
    *(ret) = athreadSetPriority( id, prio )
#define SUSPEND_SELF(time,ret)              \
    *(ret) = athreadSuspendSelf( time )
#define SUSPEND(id,ret)                     \
    *(ret) = athreadSuspend( id )
#define RESUME(id,ret)                      \
    *(ret) = athreadResume( id )
#define GET_MY_ID(id,ret)                   \
    *(ret) = athreadGetMyId( id )
#define STOP(id,ret)                        \
    *(ret) = athreadStop( id )
#define STOP_SELF()                         \
    athreadStopSelf()
#define START(id,ret)                       \
    *(ret) = athreadStart( id, 0 )
#define DELAYED_START(id,time,ret)          \
    *(ret) = athreadStart( id, time )
#define LOCK_PREEMPTION(lock,ret)           \
    *(ret) = athreadLockPreemption( lock )
#define UNLOCK_PREEMPTION(lock,ret)         \
    *(ret) = athreadUnlockPreemption( lock )

#define TIMED_WAIT(time,ret)                \
    *(ret) = athreadTimedWait( time )
#define GET_TIME(time,ret)                  \
    *(ret) = athreadGetTime(time)
#define PERIODIC_WAIT(ret)                  \
    *(ret) = athreadPeriodicWait();
#define REPLENISH(time,ret)                 \
    *(ret) = athreadReplenish(time);

extern int  athreadCreate ( pthread_t *threadId, struct athread_attr *attr );
extern int  athreadGetId ( char *name, pthread_t *threadId );
extern int  athreadGetStatus( pthread_t threadId, struct process_status* status );
extern int  athreadSetPriority( pthread_t threadId, int prio );
extern int  athreadSuspendSelf( int64_t time );
extern int  athreadSuspend( pthread_t threadId );
extern int  athreadResume( pthread_t threadId );
extern int  athreadGetMyId( pthread_t *threadId );
extern int  athreadStop( pthread_t threadId );
extern void athreadStopSelf( void );
extern int  athreadStart( pthread_t threadId, int64_t time_out );
extern int  athreadLockPreemption( LOCK_LEVEL_TYPE *lock_level );
extern int  athreadUnlockPreemption( LOCK_LEVEL_TYPE *lock_level );

extern int  athreadTimedWait( int64_t time_out );
extern int  athreadGetTime( int64_t* system_time );
extern int  athreadPeriodicWait( void );
extern int  athreadReplenish( int64_t time );
#else
extern int  __athreadGetTime( int64_t* system_time );
#endif
#endif /*_ARINC_PROCESS_H*/
