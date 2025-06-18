/* arinc/event.h */
#ifndef _ARINC_EVENT_H
#define _ARINC_EVENT_H

#include <sys/apextypes.h>

/* EVENT constant and type definitions and management services */
typedef APEX_INTEGER  EVENT_ID_TYPE;             /* идентификатор описателя событий    */
typedef NAME_TYPE     EVENT_NAME_TYPE;           /* имя описателя событий              */
/* Состояния описателя событий */
typedef enum { DOWN = 0, UP = 1 } EVENT_STATE_TYPE;

/* Описатель состояния события */

typedef
  struct event_status {
    EVENT_STATE_TYPE EVENT_STATE;                /* текущее состояние описателя события*/
    WAITING_RANGE_TYPE WAITING_PROCESSES;        /* число потоков, ожидающих события   */
  } EVENT_STATUS_TYPE;

#define CREATE_EVENT(name,id,ret) \
    *(ret) = evnCreate(id,name)
#define SET_EVENT(id,ret) \
    *(ret) = evnSet(id)
#define RESET_EVENT(id,ret) \
    *(ret) = evnReset(id)
#define WAIT_EVENT(id,it,ret)  \
    *(ret) = evnWait(id,it)
#define GET_EVENT_ID(name,id,ret)  \
    *(ret) = evnGetId(name,id)
#define GET_EVENT_STATUS(id,status,ret) \
    *(ret) = evnGetStatus(id,status)

extern int evnCreate ( EVENT_ID_TYPE *event, char *name );
extern int evnSet ( EVENT_ID_TYPE event );
extern int evnReset ( EVENT_ID_TYPE event );
extern int evnWait ( EVENT_ID_TYPE event, int64_t timeout );
extern int evnGetId ( char *name, EVENT_ID_TYPE *event );
extern int evnGetStatus ( EVENT_ID_TYPE event, struct event_status *status );


#endif /*_ARINC_EVENT_H*/
