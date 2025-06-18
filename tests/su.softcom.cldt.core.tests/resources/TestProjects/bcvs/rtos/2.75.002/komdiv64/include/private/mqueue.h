#ifndef _PRIVATE_MQUEUE_H
#define _PRIVATE_MQUEUE_H

#include   <hash.h>
#include   <private/objcore.h>
#include   <private/queue.h>
#include   <private/q_prio32.h>

typedef struct {
    quePARM_NODE node;
    int prio;
    size_t size;
} mqpBUFFER;

typedef struct mqpControl {
    objHEADER       m_objHdr;
//    unsigned long m_idn;                  /* идентификатор узла в таблице имен   */
    long            m_attr;                 /*                                     */
    size_t          m_msgsize;              /* макс.длина сообщения                */
    long            m_maxmsg;               /* макс.число сообщений                */
    long            m_curmsgs;              /* число сообщений в очереди           */
    long            m_attach;
    struct sigevent m_notification;
    mqd_t           m_mqdes;                /*описатель, потребовавший уведомления */
    void*           m_pBuffers;
    quePRIO32_HEAD  m_pending;              /* сообщения очереди                   */
    sdlHEAD         m_intermed;             /* очередь для сообщений промежуточная */
    ddlHEAD         m_inactive;             /* свободные буфера                    */
    queHEAD         m_waitHead;             /* ждущие сообщений из очереди         */
    queHEAD         m_waitHeadBuf;          /* ждущие буферов                      */
    volatile u_int  m_safe;                 /* защита от удаления                  */
} mqpCONTROL;

typedef struct mqpPxControl
{
   objHEADER            m_objHdr;
   struct 	mqpControl *m_pmq;
   int                  m_oflag;
} mqpPXCONTROL;

#define mqMQD_TO_MQ(mqd)   (mqd)->m_pmq

/* Структура параметров функций mq_open, mq_receive и mq_timedreceive */
struct mq_open_p {
  const char*    name;
  int            oflag;
  mode_t         mode;
  const struct mq_attr*
                 attr;               /* указатель на описатель атрибутов
                                                                   очереди сообщений */
  int            check;              /* флаг проверки входных аргументов             */
  void          *eventAdr;           /* адрес вызова (используется при трассировке   */
  int            event;              /* traceMqOpen/traceMpMqOpen                    */
};

/* Структура параметров функций mq_open, mq_receive и mq_timedreceive */
struct mq_receive_p {
  mqd_t          mqdes;              /* дескриптор очереди сообщений (идентификатор) */
  char          *msg_ptr;            /* указатель на буфер для приема сообщения      */
  size_t         msg_len;            /* длина буфера                                 */
  unsigned int  *msg_prio;           /* приоритет сообщения                          */
  const struct   timespec            /* ограничение на время ожидания                */
                *abs_timeout;
  size_t         msgsize;            /* размер полученного сообщения                 */
  void          *eventAdr;           /* адрес вызова (используется при трассировке   */
  int            event;              /* traceMqReceive/traceMpMqReceive              */
};

/* Структура параметров функции функций mq_send() и mq_timedsend() */
struct mq_send_p {
  mqd_t          mqdes;              /* дескриптор очереди сообщений (идентификатор) */
  const char    *msg_ptr;            /* указатель на оправляемое сообщения           */
  size_t         msg_len;            /* длина сообщения                              */
  unsigned int   msg_prio;           /* приоритет сообщения                          */
  const struct   timespec            /* ограничение на время ожидания                */
                *abs_timeout;
  void          *eventAdr;           /* адрес вызова (используется при трассировке   */
  int            event;              /* traceMqSend/traceMpMqSend                    */
};


typedef  mqpCONTROL  *mqpCONTROL_ID;
typedef  mqpCONTROL   mqCONTROL;

#define errSET_AND_RETURN_MQP( err )    \
        {                               \
            errSET( err );              \
            return (mqd_t) -1;          \
        }

#define mqpMOVE_TO_BUF( pBuf, msg_ptr, msg_len, msg_prio )          \
        (pBuf)->size = msg_len;                                     \
        (pBuf)->prio = msg_prio;                                    \
        bcopy( msg_ptr, (char*)(pBuf) + sizeof(mqpBUFFER), msg_len )
/*		memmove( (char*)(pBuf) + sizeof(mqpBUFFER), msg_ptr, msg_len )*/

#define mqpINSERT_MSG( pMsg, pBuf, msg_prio )                                               \
        queInsert	( &(pMsg)->m_pending, (queNODE*)(pBuf), msg_prio );                     \
        pMsg->m_curmsgs++;                                                                  \
        if( (pMsg->m_mqdes && pMsg->m_curmsgs == 1) && queIsEmpty( &pMsg->m_waitHead ) )    \
                mqNotify( pMsg )

#define MQ_DESTROY         0x40        /* запрос на освобожд. памяти, занятой буферами */

/* Проверка, имеется ли запрос на освобождение памяти, занятой буферами      */
#define mqIS_DESTROY(mq)  ((mq)->m_attr & MQ_DESTROY)
/* Установка защита очереди от удаления (от освобождения памяти)             */
#define mqSAFE_SET(mq)    (mq)->m_safe++
/* Проверка, установлена ли защита очереди от удаления (от освобождения памяти)        */
#define mqIS_SAFE(mq)     ((mq)->m_safe != 0)
/*
 * Снятие защиты очереди от удаления (от освобождения памяти)
 * Если ранее был сделан запрос на удаление, то память не была осбобождена.
 * В этом случае освобождение памяти производится здесь.
 * Значение res зависит от того, была ли уничтожена очередь.
 */
#define mqSAFE_UNSET(mq)                                            \
  if( (--(mq)->m_safe == 0) && mqIS_DESTROY(mq) ) {                 \
      objRemove(mqpObjControlId, (objHEADER*) mq);                  \
  }

#define mpMQP_TABLE_RTN_OPEN         0
#define mpMQP_TABLE_RTN_CLOSE        1
#define mpMQP_TABLE_RTN_TIMEDSEND    2
#define mpMQP_TABLE_RTN_TIMEDRECEIVE 3
#define mpMQP_TABLE_RTN_SETATTR      4
/*размер таблицы указателей на функции MP*/
#define mpMQP_TABLE_RTN_SIZE  5

/*значение mpSemPxControlOff до инициализации*/
#define MPMQP_NOT_INIT  0xffffffff
#define MQP_IS_MP(mqp) \
   ( (mpoffset_t)((objHEADER *)mqp)->m_pClass == mpMqpPxControlOff )


extern objCONTROL_ID  mqpObjControlId;
extern objCONTROL_ID  mqpPxObjControlId;
extern hshCONTROL_ID  mqpHashNames;    /*таблица имен очередей*/
extern int mqpAtomicMove; /*длина сообщения пересылаемого разом*/

void mqpSetMqPrio( queCONTROL_ID quePrio );
#if 1
enum {
    MQ_SHOW_HEAD = 1,  /* вывести заголовок */
    MQ_SHOW_LIST = 2,  /* выводится список */
    MQ_SHOW_WAIT = 4   /* вывести ожидающих/очереди с ожидающими */
};
enum {
    MQ_SHOW_LIST_WAIT = 1   /* список очередей, имеющих непустой список
                             * ждущих потоков
                             */
};
#endif
extern funcptr_t  mqShowRtn;
extern funcptr_t  mqPxShowRtn;


#endif /* _PRIVATE_MQUEUE_H */
