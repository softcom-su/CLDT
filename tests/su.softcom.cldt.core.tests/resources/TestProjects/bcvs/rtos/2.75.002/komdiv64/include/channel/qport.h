/*  channel/qport.h
 */

//#if !defined _CHANNEL_QPORT_H && defined KERNEL
#ifndef _CHANNEL_QPORT_H
#define _CHANNEL_QPORT_H

//struct qport_status;

struct st_qportGetStatus {
  objid_t                portId;     /* идентификатор порта                        */
  struct qport_status*   status;     /* адрес для приема состояния порта           */
};

#ifdef KERNEL
#include  <private/queue.h>
#include  <private/objcore.h>
#include  <channel/channel.h>

#define QPORT_RETURN(rt)  \
  {                       \
  qport_ret = rt;         \
  goto qport_return;      \
  }
#define QPORT_RETURN0(rt) \
  {                       \
  qport_ret = rt;         \
  goto qport_return0;     \
  }


/* ---------- qportCONTROL - описатель queuing-порта (объект процесса) ---------- */
typedef
struct  qport_control {
  objHEADER          qport_objHdr;     /* заголовок объекта                            */
  struct chan_port*  qport_portChannel;/* указатель на описатель порта в канале        */
  u_int              qport_direct;     /* направление передачи (SOURCE/DESTINATION)    */
  u_int              qport_maxsize;    /* максимальный размер сообщения                */
  queHEAD            qport_waitHead;   /* очередь ждущих потоков                       */
  u_int              qport_nMsg;       /* число положенных в канал/взятых сообщений    */
  u_int              qport_discipline; /* дисциплина очереди потоков                   */
  u_int              qport_maxmsg;     /* максимальный число сообщений                 */
  u_int              qport_class;      /* класс объекта                                */
} qportCONTROL;


/* Поиск в таблице имен */
#if OS_MAJOR >= 3
#  define qportNAME_FIND(name,port)                                                 \
      {                                                                             \
       hshNODE      *node;                                                          \
       if( hshFindUnprotect(objNamesLocal,name,CLASS_QPORTCONTROL,&node) == 0 ) {   \
         port = (qportCONTROL*)hshGET_OBJ(node);                                    \
       }                                                                            \
       else                                                                         \
         port = NULL;                                                               \
     }
#else
#  define qportNAME_FIND(name,port)                                                 \
      port=(qportCONTROL*)objHashNamesFind( (char*)name, CLASS_QPORTCONTROL )
   extern objCONTROL*  qportObjControlId;
#endif

/* Проверка дисциплины обслуживания очереди потоков */
#define qportCHECK_DISCIPLINE(d)  ((d == FIFO) || (d == PRIORITY))
/* Проверка направления передачи данных */
#define qportCHECK_DIRECTION(d)   ((d == SOURCE) || (d == DESTINATION))
/* Проверка, предназначен ли порт для вывода */
#define qportIS_OUTPUT(pPort) ((pPort)->qport_direct == SOURCE)

/* Регистрация функции извещения порта о появлении буфера/сообщения */
#define qportSetNotify(chanPort,fun,arg1,arg2)                                      \
      chanSetNotification(chanPort,fun,arg1,arg2)
/* Получить буфер */
#define qportGetBuffer(pChanPort,thread) chanGetBuffer(pChanPort,thread)
/* Указать размер данных в сообщении */
//#define qportSetSize(pBufNode,size)    chanSetMessageSize(pBufNode,size)
/* Получить размер данных в сообщении */
//#define qportGetSize(pBufNode,size)    chanGetMessageSize(pBufNode)
/* Получить адрес данных в буфере */
#define qportDataAddress(pBufNode)     (void*)chanBUFNODE_TO_DATA(pBufNode)

/* Получить сообщение */
extern chanBUFNODE* qportGetMessage( chanPORT* pChanDstPort );
extern chanBUFNODE* qportGetMessageWithoutSafe( chanPORT* pChanDstPort );
/* Положить сообщение */
extern void         qportPutMessage ( chanBUFNODE* pBufNode, threadTCB* pThread );

#endif   /* KERNEL */
#endif /* _CHANNEL_QPORT_H */
