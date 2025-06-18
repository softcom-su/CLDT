/*  channel/qportsap.h
 */

#ifndef _CHANNEL_QPORTSAP_H
#define _CHANNEL_QPORTSAP_H
//#ifndef KERNEL
//#  define KERNEL
//#endif
#include    <version.h>
#include    <sys/apextypes.h>
#include    <sys/objclasses.h>
#include    <channel/channel.h>
//#if OS_MAJOR == 2
//#  include    <private/objcore.h>
//#endif

/* Структура параметров функции qportSapCreate (макрос CREATE_SAP_PORT, ARINC 653)    */
struct st_qportSapCreate {
  enum obj_classes         class;      /* класс порта:
                                             CLASS_QPORTCONTROL/CLASS_QSAPCONTROL      */
  const char*              name;       /* указатель на имя объекта                     */
  MESSAGE_SIZE_TYPE        maxsize;    /* максимальный размер сообщения                */
  MESSAGE_SIZE_TYPE        maxmsg;     /* максимальное число сообщений                 */
  PORT_DIRECTION_TYPE      direct;     /* направление передачи                         */
  QUEUING_DISCIPLINE_TYPE  discip;     /* дисциплина обслуживания очереди потоков      */
  objid_t*                 portId;     /* указатель на идентификатор объекта           */
};

struct st_qportSapSend {
  enum obj_classes         class;      /* класс порта                                  */
  objid_t                  portId;     /* идентификатор порта                          */
  MESSAGE_ADDR_TYPE        pMsg;       /* адрес данных                                 */
  MESSAGE_SIZE_TYPE        length;     /* размер сообщения                             */
  int64_t                  timeout;    /* время ожидания                               */
  SAP_ADDRESS_TYPE*        sapDstAdr;  /* адрес получателя                             */
};

struct st_qportSapReceive {
  enum obj_classes         class;      /* класс порта                                  */
  objid_t                  portId;     /* указатель на имя объекта                     */
  int64_t                  timeout;    /* время ожидания                               */
  MESSAGE_ADDR_TYPE        pBuf;       /* адрес приемного буфера                       */
  MESSAGE_SIZE_TYPE*       length;     /* адрес для приема размера сообщения           */
  SAP_ADDRESS_TYPE*        sapSrcAdr;  /* адрес для приема отправителя                 */
};

struct st_qportSapGetId {
  enum obj_classes         class;      /* класс порта                                  */
  const char*              name;       /* указатель на имя объекта                     */
  objid_t*                 portId;     /* указатель на идентификатор объекта           */
};

struct st_sapGetStatus {
  objid_t                  portId;     /* идентификатор порта                           */
  SAP_PORT_STATUS_TYPE*    status;     /* адрес для приема состояния порта              */
};

struct st_qportSapClear {
  enum obj_classes         class;      /* класс порта                                  */
  objid_t                  portId;     /* указатель на идентификатор объекта           */
};

/* Идентификаторы функций вызываемых через SYSCALL */
enum sap_syscall {
   sapSYSCALL_CREATE,
   sapSYSCALL_SEND,
   sapSYSCALL_RECEIVE,
   sapSYSCALL_GETID,
   sapSYSCALL_GETSTATUS,
   qportSYSCALL_GETSTATUS,
   qportSYSCALL_CLEAR,
   sapSYSCALL_LAST
};

struct chan_options;
int sapSysCall( enum sap_syscall fId, void* arg1, void* arg2 );
int mpchanNetTableInit ( struct chan_options* pChanOptions );

#ifdef KERNEL

/* -------- sapCONTROL - описатель queuing-портов (объект процесса) --------- */
typedef
struct  sap_control {
  objHEADER          sap_objHdr;     /* заголовок объекта                            */
  struct chan_port*  sap_portChannel;/* указатель на описатель порта в канале        */
  u_int              sap_direct;     /* напрвление передачи                          */
  u_int              sap_maxsize;    /* максимальный размер сообщения                */
  queHEAD            sap_waitHead;   /* заголовок очереди блокирования потоков       */
  u_int              sap_nMsg;       /* число положенных в канал/взятых сообщений    */
  u_int              sap_discipline; /* дисциплина потоков                           */
  u_int              sap_maxmsg;     /* максимальное число сообщений                 */
  u_int              sap_class;      /* класс объекта
                                             CLASS_QPORTCONTROL/CLASS_QSAPCONTROL    */
  SAP_ADDRESS_TYPE   sap_address;
} sapCONTROL;



#  if OS_MAJOR == 2
extern objCONTROL  objQSapControl;
#  endif

#endif   /* KERNEL */

#if OS_MAJOR == 2
struct st_qportGetStatus;
int PortSapCreate( struct st_qportSapCreate* prm );
int PortSapSend( struct st_qportSapSend* prm );
int PortSapReceive( struct st_qportSapReceive* prm );
int PortSapGetId( struct st_qportSapGetId* prm );
int QPortGetStatus( struct st_qportGetStatus* prm );
int PortSapClear( struct st_qportSapClear* prm );
int PortSapGetStatus( struct st_sapGetStatus* prm );
#endif

#endif /* _CHANNEL_QPORTSAP_H */
