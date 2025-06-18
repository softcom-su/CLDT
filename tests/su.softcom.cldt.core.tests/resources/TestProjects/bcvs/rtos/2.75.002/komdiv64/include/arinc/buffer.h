/* arinc/buffer.h */
/*
 *--------------------------------------------------------------------------
 * BUFFER constant and type definitions and management services 
 *--------------------------------------------------------------------------
 */
#ifndef _ARINC_BUFFER_H
#define _ARINC_BUFFER_H

#include <sys/apextypes.h>

typedef unsigned int  BUFFER_ID_TYPE;       /* идентификатор очереди сообщений         */
typedef NAME_TYPE BUFFER_NAME_TYPE;         /* имя очереди сообщений                   */

/* Структура параметров функции bufCreate (макрос CREATE_BUFFER, ARINC 653)   */
struct st_bufCreate {
  BUFFER_ID_TYPE *bufId;     /* указатель на идентификатор буфера                      */
  char*     name;            /* указатель на имя буфера                                */
  long      msgsize;         /* максимальный размер сообщения                          */
  long      maxmsg;          /* максимальное число сообщений                           */
  int       discip;          /* дисциплина обслуживания очереди потоков                */
};

/* Структура описателя состояния буфера */
typedef
  struct buffer_status {
    MESSAGE_RANGE_TYPE NB_MESSAGE;          /* текущее количество сообщений в очереди  */
    MESSAGE_RANGE_TYPE MAX_NB_MESSAGE;      /* макс. количество сообщений в очереди    */
    MESSAGE_SIZE_TYPE  MAX_MESSAGE_SIZE;    /* максимальный размер сообщения           */
    WAITING_RANGE_TYPE WAITING_PROCESSES;   /* текущее количество сообщений в очереди  */
  } BUFFER_STATUS_TYPE;

#define CREATE_BUFFER(name,size,msg,discip,id,ret) \
  { \
    struct st_bufCreate prm = \
      { id, name, size, msg, discip }; \
    *(ret) = bufCreate( &prm ); \
  }
#define SEND_BUFFER(id,ptr,sz,time,ret) \
  *(ret) = bufSend(id,ptr,sz,time)
#define RECEIVE_BUFFER(id,time,ptr,sz,ret) \
  *(ret) = bufReceive(id,ptr,sz,time)
#define GET_BUFFER_ID(name,id,ret) \
  *(ret) = bufGetId(name,id)
#define GET_BUFFER_STATUS(id,s,ret) \
  *(ret) = bufGetStatus(id,s)

extern int bufCreate( struct st_bufCreate* prm );
extern int bufSend( BUFFER_ID_TYPE bufId, MESSAGE_ADDR_TYPE msg_ptr, MESSAGE_SIZE_TYPE msg_len, SYSTEM_TIME_TYPE timeout );
extern int bufReceive( BUFFER_ID_TYPE bufId, MESSAGE_ADDR_TYPE buf_ptr, MESSAGE_SIZE_TYPE * msg_len, SYSTEM_TIME_TYPE timeout );
extern int bufGetId( BUFFER_NAME_TYPE name, BUFFER_ID_TYPE *bufId );
extern int bufGetStatus( BUFFER_ID_TYPE bufId, BUFFER_STATUS_TYPE * status );

#endif /* _ARINC_BUFFER_H */


