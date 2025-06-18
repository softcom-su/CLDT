
/* arinc/sport.h - SAMPLING PORT constant and type definitions and management services */

#ifndef _ARINC_SPORT_H
#define _ARINC_SPORT_H

#include <sys/apextypes.h>

/* Структура параметров функции sportCreate (макрос CREATE_SAMPLING_PORT, ARINC 653)   */
struct st_sportCreate {
  sport_t*  portId;                              /* указатель на идентификатор порта   */
  char*     name;                                /* указатель на имя порта             */
  long      msgsize;                             /* максимальный размер сообщения      */
  int       direct;                              /* направление передачи данных        */
  int64_t   period;                              /* период годности                    */
};

/* Описателя состояния порта  */
typedef  struct sport_status {
    MESSAGE_SIZE_TYPE   MAX_MESSAGE_SIZE;        /* максимальный размер сообщений      */
    PORT_DIRECTION_TYPE PORT_DIRECTION;          /* направление передачи данных        */
    SYSTEM_TIME_TYPE    REFRESH_PERIOD;          /* период обновления порта            */
    VALIDITY_TYPE       LAST_MSG_VALIDITY;       /* годность последнего сообщения      */
} SAMPLING_PORT_STATUS_TYPE;

/*
 * These types are only used in sampling port services.
 */

#define CREATE_SAMPLING_PORT(name,size,direct,period,id,ret) \
  { \
    struct st_sportCreate prm = \
      { id, name, size, direct, period }; \
    *(ret) = sportCreate( &prm ); \
  }
#define WRITE_SAMPLING_MESSAGE(id,ptr,sz,ret) \
  *(ret) = sportWrite( id, ptr, sz )
#define READ_SAMPLING_MESSAGE(id,ptr,sz,valid,ret) \
  *(ret) = sportRead( id, ptr, sz, valid )
#define GET_SAMPLING_PORT_ID(name,id,ret) \
  *(ret) = sportGetId( name, id )
#define GET_SAMPLING_PORT_STATUS(id,status,ret) \
  *(ret) = sportGetStatus( id, status )

extern int sportCreate( struct st_sportCreate* prm );
extern int sportWrite( sport_t portId, MESSAGE_ADDR_TYPE msg_ptr, MESSAGE_SIZE_TYPE msg_len );
extern int sportRead( sport_t portId, MESSAGE_ADDR_TYPE msg_ptr, MESSAGE_SIZE_TYPE *msg_len, enum msg_validity *valid );
extern int sportGetId( char *name, sport_t *portId );
extern int sportGetStatus( sport_t portId, struct sport_status *status );

extern int sportMsgLock ( sport_t portId, MESSAGE_ADDR_TYPE *ptr, MESSAGE_SIZE_TYPE *msg_len, enum msg_validity *valid );
extern int sportMsgUnlock ( sport_t portId, MESSAGE_ADDR_TYPE * ptr );

#endif  /*_ARINC_SPORT_H*/


