/*  arinc/qportlist.h
 */

#ifndef _ARINC_QPORTLIST_H
#define _ARINC_QPORTLIST_H

#include <sys/apextypes.h>

typedef APEX_INTEGER          LIST_SIZE_TYPE;

typedef
enum {
   INACTIVE = 0,
   ACTIVE   = 1
} PORT_ACTION_TYPE;

typedef
struct qportlist_status {
    PORT_ACTION_TYPE      PORT_ACTION;           /* состояние порта                    */
    MESSAGE_ADDR_TYPE     MESSAGE_ADDR;          /* адрес буфера                       */
} PORT_LIST_STATUS_TYPE;

#define CREATE_QUEUING_PORT_LIST(name,size,id,disc,ret)                          \
  *(ret) = qportListCreate( name, size, id, disc )
#define ADD_PORT_TO_QUEUING_PORT_LIST(listId,portid,buf,action,ret)              \
  *(ret) = qportListAdd( listId, portid, buf, action )
#define SET_PORT_ACTION_IN_QUEUING_PORT_LIST(listId,portid,action,ret)           \
  *(ret) = qportListSetAction( listId, portid, action )
#define GET_QUEUING_PORT_LIST_STATUS(listId,portid,status,ret)                   \
  *(ret) = qportListStatus( listId, portid, status )
#define RECEIVE_MESSAGE_FROM_QUEUING_PORT_LIST(listId,time,portid,size,msg,ret)  \
  *(ret) = qportListReceive( listId, time, portid, size, msg )

extern int qportListCreate( const char* name, LIST_SIZE_TYPE listsize,
      objid_t* qportlistId, QUEUING_DISCIPLINE_TYPE discip );
extern int qportListAdd( objid_t qportlistId, qport_t qportId,
      MESSAGE_ADDR_TYPE buf, PORT_ACTION_TYPE action );
extern int qportListSetAction( objid_t qportlistId, qport_t qportId,
      PORT_ACTION_TYPE action );
extern int qportListStatus( objid_t qportlistId, qport_t qportId,
      PORT_LIST_STATUS_TYPE* status );
extern int qportListReceive( objid_t qportlistId, int64_t timeout,
      qport_t* qportId, MESSAGE_SIZE_TYPE* length, MESSAGE_RANGE_TYPE* msg );
#endif /* _ARINC_QPORTLIST_H */
