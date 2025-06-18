/* events.h */
#ifndef _EVENTS_H
#define _EVENTS_H

#include <errno.h>
#include <sys/time.h>
#include <private/events.h>

typedef char            *NAME_TYPE;

typedef NAME_TYPE        EVENT_NAME_TYPE;
typedef int              WAITING_RANGE_TYPE;

enum EVENT_STATE_VALUE_TYPE { DOWN, UP };
typedef enum EVENT_STATE_VALUE_TYPE EVENT_STATE_TYPE;
typedef 
  struct  {
    EVENT_STATE_TYPE  EVENT_STATE;
    WAITING_RANGE_TYPE  WAITING_PROCESSES;
  }                     EVENT_STATUS_TYPE;



#define CREATE_EVENT      evnCreate
#define SET_EVENT         evnSet
#define RESET_EVENT       evnReset
#define WAIT_EVENT        evnWait
#define GET_EVENT_ID      evnGetId
#define GET_EVENT_STATUS  evnGetStatus
#define DESTROY_EVENT     evnDestroy

extern void evnCreate (
      EVENT_NAME_TYPE   EVENT_NAME,
      EVENT_ID_TYPE    *EVENT_ID,
      RETURN_CODE_TYPE *RETURN_CODE
   );
extern void evnSet (
      EVENT_ID_TYPE     EVENT_ID,
      RETURN_CODE_TYPE *RETURN_CODE
   );
extern void evnReset (
      EVENT_ID_TYPE     EVENT_ID,
      RETURN_CODE_TYPE *RETURN_CODE
   );
extern void evnWait (
      EVENT_ID_TYPE     EVENT_ID,
      SYSTEM_TIME_TYPE  TIME_OUT,
      RETURN_CODE_TYPE *RETURN_CODE
   );
extern void evnGetId (
      EVENT_NAME_TYPE   EVENT_NAME,
      EVENT_ID_TYPE    *EVENT_ID,
      RETURN_CODE_TYPE *RETURN_CODE
   );
extern void evnGetStatus (
      EVENT_ID_TYPE       EVENT_ID,
      EVENT_STATUS_TYPE  *EVENT_STATUS,
      RETURN_CODE_TYPE   *RETURN_CODE
   );
extern void evnDestroy (
      EVENT_ID_TYPE     EVENT_ID,
      RETURN_CODE_TYPE *RETURN_CODE
   );

/*средства просмотра событий*/
extern funcptr_t  evnShowRtn;
extern int        evnShowInit( void );
extern void       evnDisplayList( int showType );
extern void       evnShow( EVENT_ID_TYPE EVENT_ID, int list );

#endif /*_EVENTS_H*/
