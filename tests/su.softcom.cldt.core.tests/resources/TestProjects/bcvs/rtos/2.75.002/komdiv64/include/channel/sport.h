/*  channel/sport.h */

#if !defined _CHANNEL_SPORT_H && defined KERNEL
#define _CHANNEL_SPORT_H

#include <channel/channel.h>


/* ---------- sportCONTROL - описатель sampling-порта (объект процесса) ---------- */
typedef
struct  sport_control {
    objHEADER           sport_objHdr;      /* заголовок объекта                        */
    struct chan_port*   sport_portChannel; /* указатель на описатель порта в канале    */
    u_int               sport_direct;      /* направление передачи (SOURCE/DESTINATION)*/
    u_int               sport_maxsize;     /* максимальный размер сообщения            */
    queHEAD             sport_waitHead;    /* заголовок списка потоков, ждущих записи  */
    u_int               sport_nMsg;        /* число положенных в канал/прочитанных сообщений */
//    volatile u_int      sport_number;      /* номер записываемого сообщения(не исп.?)  */
} sportCONTROL;

/* ---------- макрос sportIS_EMPTY - проверка, пуст ли sampling-канал ---------- */
#define sportIS_EMPTY(chanPort)      ((chanPort)->cp_message == 0)
/* Проверка доступности канала для записи */
//#define sportIS_AVAILABLE(chanPort) ddlIs_empty(&(chanPort)->cp_bufList)
/* Проверка направления передачи данных */
#define sportCHECK_DIRECTION(d)  ((d == SOURCE) || (d == DESTINATION))
/* Проверка, предназначен ли порт для вывода */
#define sportIS_OUTPUT(pPort)       ((pPort)->sport_direct == SOURCE)
/* Поиск в таблице имен в порта по имени */
#if OS_MAJOR == 3
#  define sportNAME_FIND(name,port)  \
     { \
       hshNODE      *node; \
       if( hshFindUnprotect(objNamesLocal,name,CLASS_SPORTCONTROL,&node) == 0 ) { \
         port = (sportCONTROL*)hshGET_OBJ(node); \
       } \
       else \
         port = NULL; \
     }
#else
#  define sportNAME_FIND(name,port)  \
      port=(sportCONTROL*)objHashNamesFind( (char*)name, CLASS_SPORTCONTROL )
   extern objCONTROL*  sportObjControlId;
#endif

#define SPORT_RETURN(e)   \
  {                       \
  rt = e;                 \
  goto sport_return;      \
  }

/* Получить буфер */
#define sportGetBuffer(pChanPort,thread) chanGetBuffer(pChanPort,thread)
/* Регистрация функции извещения порта о появлении сообщения */
#define sportSetNotify(chanPort,fun,arg1,arg2) \
      chanSetNotification(chanPort,fun,arg1,arg2)

/* Положить сообщение */
extern void sportPutMessage( chanBUFNODE* pBufNode, threadTCB* pThread );
/* Получить адрес актуального сообщения */
extern chanBUFNODE*  sportGetMessage ( chanPORT* pChanPort );
/* Освободить буфер (сообщение прочитано) */
extern void          sportFreeMessage ( chanPORT* pChanPort );


#endif /* _CHANNEL_SPORT_H */
