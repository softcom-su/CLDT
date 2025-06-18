/* arinc/bbrd.h */
#ifndef _ARINC_BBRD_H
#define _ARINC_BBRD_H

#include <sys/apextypes.h>


typedef unsigned int  BLACKBOARD_ID_TYPE;        /* идентификатор доски объявлений     */
typedef objid_t       bbrd_t;
typedef NAME_TYPE     BLACKBOARD_NAME_TYPE;      /* имя доски объявлений               */
typedef enum { EMPTY = 0, OCCUPIED = 1 } EMPTY_INDICATOR_TYPE;/* состояние доски объяв.*/

/* Описатель состояния доски объявлений */
typedef
  struct bbrd_status {
    EMPTY_INDICATOR_TYPE EMPTY_INDICATOR;        /* состояние доски объявлений         */
    MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE;          /* максимальный размер сообщения      */
    WAITING_RANGE_TYPE WAITING_PROCESSES; /* число потоков в очереди к доске объявлений*/
  } BLACKBOARD_STATUS_TYPE;

#define CREATE_BLACKBOARD(name,msz,id,ret) \
  *(ret) = bbrdCreate(id,name,msz)
#define DISPLAY_BLACKBOARD(id,ptr,sz,ret) \
  *(ret) = bbrdDisplay(id,ptr,sz);
#define READ_BLACKBOARD(id,time,ptr,sz,ret) \
  *(ret) = bbrdRead(id,ptr,sz,time);
#define CLEAR_BLACKBOARD(id,ret) \
  *(ret) = bbrdClear(id);
#define GET_BLACKBOARD_ID(name,id,ret) \
  *(ret) = bbrdGetId(name,id);
#define GET_BLACKBOARD_STATUS(id,s,ret) \
  *(ret) = bbrdGetStatus(id,s)

extern int bbrdCreate( bbrd_t *bbrdId, char *name, MESSAGE_SIZE_TYPE maxsize );
extern int bbrdDisplay( bbrd_t bbrdId, u_char* msg_ptr, MESSAGE_SIZE_TYPE msg_len );
extern int bbrdRead( bbrd_t bbrdId, u_char* buf_ptr, MESSAGE_SIZE_TYPE *len, int64_t timeout );
extern int bbrdClear( bbrd_t bbrdId );
extern int bbrdGetId( char *name, bbrd_t *bbrdId );
extern int bbrdGetStatus( bbrd_t bbrdId, struct bbrd_status* status );

#endif /*_ARINC_BBRD_H*/
