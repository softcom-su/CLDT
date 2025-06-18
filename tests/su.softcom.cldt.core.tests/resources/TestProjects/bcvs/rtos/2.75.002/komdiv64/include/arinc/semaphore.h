/* arinc/semaphore.h */
#ifndef _ARINC_SEMAPHORE_H
#define _ARINC_SEMAPHORE_H

#include <sys/apextypes.h>

//#define MAX_NUMBER_OF_SEMAPHORES SYSTEM_LIMIT_NUMBER_OF_SEMAPHORES
typedef unsigned int        SEMAPHORE_ID_TYPE;   /* идентификатор семафора             */
typedef objid_t             asem_t;
/* Максимальное значение счетчика семафора */
#define MAX_SEMAPHORE_VALUE SEM_VALUE_MAX        /* 32767                              */
typedef NAME_TYPE           SEMAPHORE_NAME_TYPE; /* имя семафора                       */
typedef APEX_INTEGER        SEMAPHORE_VALUE_TYPE;/* значение счетчика семафора         */
/* Описатель состояния семафора */
typedef
  struct semaphore_status {
    SEMAPHORE_VALUE_TYPE CURRENT_VALUE;          /* текущее значение счетчика семафора */
    SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE;          /* макс. значение счетчика семафора   */
    WAITING_RANGE_TYPE   WAITING_PROCESSES;      /* число потоков, в очереди к семафору*/
  } SEMAPHORE_STATUS_TYPE;

/* Структура параметров функции asemCreate (макрос CREATE_SEMAPHORE, ARINC 653)   */
struct st_asemCreate {
  asem_t*   semId;           /* указатель на идентификатор семафора                    */
  char*     name;            /* указатель на имя семафора                              */
  long      value;           /* начальное значение счетчика семафора                   */
  long      maxvalue;        /* максимальное значение счетчика семафора                */
  int       discip;          /* дисциплина обслуживания очереди потоков                */
};

#define CREATE_SEMAPHORE(name,val,valmax,discip,id,ret) \
  { \
    struct st_asemCreate prm = \
      { id, name, val, valmax, discip }; \
    *(ret) = asemCreate( &prm ); \
  }
#define WAIT_SEMAPHORE(id,time,ret) \
    *(ret) = asemWait(id,time)
#define SIGNAL_SEMAPHORE(id,ret) \
    *(ret) = asemSignal(id)
#define GET_SEMAPHORE_ID(name,id,ret) \
    *(ret) = asemGetId(name,id)
#define GET_SEMAPHORE_STATUS(id,s,ret) \
    *(ret) = asemGetStatus(id,s)

extern int asemCreate( struct st_asemCreate* prm );
extern int asemWait( asem_t semId, int64_t timeout );
extern int asemSignal( asem_t semId );
extern int asemGetId( char *name, asem_t *semId );
extern int asemGetStatus( asem_t semId, struct semaphore_status *s );

#endif /*_ARINC_SEMAPHORE_H*/
