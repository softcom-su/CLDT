/*  arinc/qportsap.h
 */

#ifndef _ARINC_QPORTSAP_H
#define _ARINC_QPORTSAP_H

#include <sys/apextypes.h>

#define MAX_NUMBER_OF_SAP_PORTS 1024            /* Implementation dependent */
                                                /* Partition scope          */
typedef NAME_TYPE       SAP_PORT_NAME_TYPE;

#define SAP_ADDR_TYPE   APEX_BYTE

#if 0
typedef struct sapaddr_type
{
    APEX_BYTE             Addr_Length;           /* structure size          */
    APEX_BYTE             Addr_Type;             /* address type            */
    APEX_UNSIGNED_SHORT   UDP_Port_Number;       /* src/dst UDP port number */
    APEX_UNSIGNED         IP_Address;            /* src/dst IP address      */
    APEX_BYTE             Addr_Spare[8];         /* structure padding       */
} SAP_ADDRESS_TYPE;
#else
typedef struct sapaddr_type
{
    APEX_BYTE             ADDR_LENGTH;           /* structure size          */
    SAP_ADDR_TYPE         ADDR_TYPE;             /* address type            */
    APEX_UNSIGNED_SHORT   UDP_Port_Number;       /* src/dst UDP port number */
    APEX_UNSIGNED         IP_Address;            /* src/dst IP address      */
    APEX_BYTE             Addr_Spare[8];         /* structure padding       */
} SAP_ADDRESS_TYPE;
#endif


#define SAP_ADDR_TYPE_UNKNOWN  0                 /* unknown address type    */
#define SAP_ADDR_TYPE_INET     1                 /* type is IP address type */

/* structure returned by SAP port status query */
typedef
struct qsapport_status {
   MESSAGE_RANGE_TYPE      NB_MESSAGE;           /* состояние порта         */
   MESSAGE_RANGE_TYPE      MAX_NB_MESSAGE;       /* адрес буфера            */
   MESSAGE_SIZE_TYPE       MAX_MESSAGE_SIZE;
   PORT_DIRECTION_TYPE     PORT_DIRECTION;
   WAITING_RANGE_TYPE      WAITING_PROCESSES;
   SAP_ADDRESS_TYPE        CURRENT_DEST_ADDR;
   SAP_ADDRESS_TYPE        CURRENT_SRC_ADDR;
} SAP_PORT_STATUS_TYPE;

#define CREATE_SAP_PORT(name,size,msg,drc,disc,id,ret)                  \
  *(ret) = sapCreate( name, size, msg, drc, disc, id )
#define SEND_SAP_MESSAGE(sapId,buf,l,time,adr,ret)                      \
  *(ret) = sapSend( sapId, buf, l, time, adr )
#define RECEIVE_SAP_MESSAGE(sapId,time,buf,l,adr,ret)                   \
  *(ret) = sapReceive( sapId, time, buf, l, adr )
#define GET_SAP_PORT_ID(name,portid,ret)                                \
  *(ret) = sapGetId( name, portid )
#define GET_SAP_PORT_STATUS(portid,status,ret)                          \
  *(ret) = sapGetStatus( portid, status )

extern int sapCreate( const char* name, MESSAGE_SIZE_TYPE msgsize,
            MESSAGE_RANGE_TYPE maxmsg, PORT_DIRECTION_TYPE direct,
            QUEUING_DISCIPLINE_TYPE discip, objid_t* portId );
extern int sapSend( objid_t portId, MESSAGE_ADDR_TYPE pBuf,
            MESSAGE_SIZE_TYPE length, SYSTEM_TIME_TYPE timeout, SAP_ADDRESS_TYPE sapDstAdr );
extern int sapReceive( objid_t portId, SYSTEM_TIME_TYPE timeout, MESSAGE_ADDR_TYPE pBuf,
            MESSAGE_SIZE_TYPE* length, SAP_ADDRESS_TYPE* sapSrcAdr );
extern int sapGetId( const char* name, objid_t* portId );
extern int sapGetStatus( objid_t portId, SAP_PORT_STATUS_TYPE* status );

#endif /* _ARINC_QPORTSAP_H */
