/* arinc/qport.h - QUEUING PORT constant and type definitions and management services */

#ifndef _ARINC_QPORT_H
#define _ARINC_QPORT_H

#include <sys/apextypes.h>
/* Структура параметров функции qportCreate (макрос CREATE_QUEUING_PORT, ARINC 653)    */
struct st_qportCreate {
  qport_t*  portId;              /* указатель на идентификатор порта                   */
  char*     name;                /* указатель на имя порта                             */
  long      msgsize;             /* максимальный размер сообщения                      */
  long      maxmsg;              /* максимальное число сообщений                       */
  PORT_DIRECTION_TYPE  direct;   /* направление передачи данных                        */
  QUEUING_DISCIPLINE_TYPE discip;/* дисциплина обслуживания очереди потоков            */
};


/* Описатель состояния порта с очередью сообщений */
typedef  struct qport_status {
    MESSAGE_RANGE_TYPE    NB_MESSAGE;            /* количество сообщений в очереди     */
    MESSAGE_RANGE_TYPE    MAX_NB_MESSAGE;        /* макс. число сообщений в очереди    */
    MESSAGE_SIZE_TYPE     MAX_MESSAGE_SIZE;      /* максимальный размер сообщений      */
    PORT_DIRECTION_TYPE   PORT_DIRECTION;        /* направление передачи данных        */
    WAITING_RANGE_TYPE    WAITING_PROCESSES;     /* число потоков в очереди            */
} QUEUING_PORT_STATUS_TYPE;

#define CREATE_QUEUING_PORT(name,size,msg,direct,discip,id,ret)   \
   *(ret) = qportsapCreate( name, size, msg, direct, discip, id )

#define SEND_QUEUING_MESSAGE(id,msg,size,timeout,ret)             \
         *(ret) = qportsapSend( id, msg, size, timeout )
#define RECEIVE_QUEUING_MESSAGE(id,timeout,msg,pSize,ret)         \
         *(ret) = qportsapReceive( id, timeout, msg, pSize )
#define GET_QUEUING_PORT_ID(name,id,ret)                          \
         *(ret) = qportsapGetId( name, id )
#define GET_QUEUING_PORT_STATUS(id,status,ret)                    \
         *(ret) = qportsapGetStatus( id, status )
#define CLEAR_QUEUING_PORT(id,ret)                                \
         *(ret) = qportsapClear( id )
extern int qportsapCreate( const char* name, MESSAGE_SIZE_TYPE msgsize,
            MESSAGE_RANGE_TYPE maxmsg, PORT_DIRECTION_TYPE direct,
            QUEUING_DISCIPLINE_TYPE discip, objid_t* portId );
extern int qportsapSend( objid_t portId, MESSAGE_ADDR_TYPE pBuf,
            MESSAGE_SIZE_TYPE length, int64_t timeout );
extern int qportsapReceive( objid_t portId, int64_t timeout, MESSAGE_ADDR_TYPE pBuf,
            MESSAGE_SIZE_TYPE* length );
extern int qportsapGetId( const char* name, objid_t* portId );
extern int qportsapGetStatus( objid_t portId, QUEUING_PORT_STATUS_TYPE* status );
extern int qportsapClear( qport_t portId );

#endif  /*_ARINC_QPORT_H*/


