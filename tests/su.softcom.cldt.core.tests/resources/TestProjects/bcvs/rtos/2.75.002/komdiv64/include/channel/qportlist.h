/*  channel/qportlist.h
 */

#ifndef _CHANNEL_QPORTLIST_H
#define _CHANNEL_QPORTLIST_H

#include    <version.h>
#include    <sys/apextypes.h>
#include    <private/queue.h>
#include    <private/sdlist.h>
#include    <channel/channel.h>

/* Структура параметров функции qportCreateList (макрос CREATE_QUEUING_PORT_LIST, ARINC 653)    */
struct st_qportListCreate {
  char*                    name;       /* указатель на имя объекта                     */
  LIST_SIZE_TYPE           listsize;   /* максимальный число портов в списке           */
  objid_t*                 portListId; /* указатель на идентификатор списка портов     */
  QUEUING_DISCIPLINE_TYPE  discip;     /* дисциплина обслуживания очереди потоков      */
};

struct st_qportListAdd {
  objid_t                  portListId; /* идентификатор списка портов                  */
  qport_t                  portId;     /* идентификатор порта                          */
  MESSAGE_ADDR_TYPE        pBuf;       /* адрес приемного буфера                       */
  PORT_ACTION_TYPE         action;     /* состояние порта                              */
};

struct st_qportListAction {
  objid_t                  portListId; /* идентификатор списка портов                  */
  qport_t                  portId;     /* идентификатор порта                          */
  PORT_ACTION_TYPE         action;     /* состояние порта                              */
};

struct st_qportListStatus {
  objid_t                  portListId; /* идентификатор списка портов                  */
  qport_t                  portId;     /* идентификатор порта                          */
  PORT_LIST_STATUS_TYPE*   status;     /* адрес приемника состояния порта              */
};

struct st_qportListReceive {
  objid_t                  portListId; /* идентификатор списка портов                  */
  int64_t                  timeout;    /* время ожидания                               */
  qport_t*                 portId;     /* адрес для идентификатора порта               */
  MESSAGE_SIZE_TYPE*       length;     /* адрес для размера сообщения                  */
  MESSAGE_RANGE_TYPE*      msg;        /*  адрес для числа сообщений в очереди         */
};

/* Идентификаторы функций вызываемых через SYSCALL */
enum qlist_syscall {
   qlistSYSCALL_CREATE,
   qlistSYSCALL_ADD,
   qlistSYSCALL_ACTION,
   qlistSYSCALL_STATUS,
   qlistSYSCALL_RECEIVE,
   qlistSYSCALL_LAST
};

#if OS_MAJOR == 2
int QportListCreate( struct st_qportListCreate* par );
#endif

int qlistSysCall( enum qlist_syscall fId, void* arg1, void* arg2 );

#ifdef KERNEL

#define qlistUNUSED     0xFFFFFFFF     /* к списку не было обращения                   */

typedef
struct  qlist_port {
   sdlNODE  qlport_node;               /* узел присоединения к описателю порта         */
   volatile
   u_int    qlport_number;             /* порядковый номер порта в списке              */
   objid_t  qlport_portId;             /* идентификатор порта                          */
   void*    qlport_buf;
   volatile
   u_int    qlport_action;             /* состояние порта                              */
   struct qlist_control*
            qlport_qlist;              /* адрес описателя списка портов                */
   struct qport_control*
            qlport_port;               /* адрес описателя порта                        */
   struct chan_port*
            qlport_chanport;           /* адрес описателя порта в канале               */
   struct desc_channel*
            qlport_channel;            /* адрес описателя канала                       */
} qlistPORT;

/* -------- qlistCONTROL - описатель списка queuing-портов (объект процесса) --------- */
typedef
struct  qlist_control {
  objHEADER          qlist_objHdr;     /* заголовок объекта                            */
  u_int              qlist_maxport;    /* максимальное число портов в списке           */
  u_int              qlist_curport;    /* действительное число портов в списке         */
  u_int              qlist_discipline; /* максимальный размер сообщения                */
  volatile
  u_int              qlist_nReceive;   /* счетчик одновременных обращений к списку     */
  u_int              qlist_lastPort;   /* порядковый номер порта, из которого получено
                                                               последнее сообщение     */
  u_int              qlist_numPort;    /* порядковый номер порта, рассматриваемый
                                                                        потоком        */
#if 0
//  volatile
//  u_int              qlist_buffer;     /* флаг полученного буфера с сообщением         */
//  volatile
  u_int              qlist_msgPort;    /* порядковый номер порта, в который было
                                          положено сообщение во время просмотра списка */
#endif
  struct qlist_port* qlist_Port;       /* описатель порта списка (1-ый элемент массива)*/
  queHEAD            qlist_waitHead;   /* заголовок очереди блокирования потоков       */
} qlistCONTROL;

#if OS_MAJOR == 2
extern objCONTROL*  qlistObjControlId;

int   QportListCreate( struct st_qportListCreate* prm );
int   QportListAdd( struct st_qportListAdd* prm );
int   QportListSetAction( struct st_qportListAction* prm );
int   QportListStatus( struct st_qportListStatus* prm );
int   QportListReceive( struct st_qportListReceive* prm );
#endif

#endif   /* KERNEL */


#endif /* _CHANNEL_QPORTLIST_H */
