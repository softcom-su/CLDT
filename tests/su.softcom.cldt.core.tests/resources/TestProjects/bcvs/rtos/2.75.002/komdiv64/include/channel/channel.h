/* channel/channel.h */

#ifndef _CHANNEL_CHANNEL_H
#define _CHANNEL_CHANNEL_H
struct mp_buf_attr
{
   /* Buffer validity: rw */
   int v;
   /* Msg seq. num: rw */
   int seq;
};

/*
 * СТРУКТУРЫ, ИСПОЛЬЗУЕМЫЕ ФУНКЦИЯМИ УПРАВЛЕНИЯ КАНАЛАМИ
 *
 * Описатель канала (chanDESCRIPTION) содержит следующую информацию:
 *   - указатель на конфиг. параметры канала;
 *   - режим работы (queuing/sampling);
 *   - максимальная длина сообщения;
 *   - количество буферов;
 *   - период обновления (для sampling портов);
 *   - заголовок очередей сообщений;
 *   - число сообщений в очереди;
 *   - заголовок списка свободных буферов (QUEUING).
 *   - описатель порта-отправителя (struct chan_port - chanPORT);
 *   - указатель на массив описателей портов-получателей).
 * Описатели портов создаются в области системных данных модуля функцией modChannelsInit()
 * при инициализации модуля на основе информации,
 * указанной при конфигурировании (структура Connection_Table).
 *
 * Структура Connection_Table содержит только структуру ct_Channel,
 * которая содержит следующую информацию:
 *   - количество каналов;
 *   - указатель на массив описателей каналов, созданных при конфигурировании.
 *
 * Описатель параметров конфигурации канала ChannelType содержит следующую информацию:
 *   - идентификатор канала;
 *   - имя канала;
 *   - описание параметров конфигурации порта-отправителя;
 *   - число портов получателей и указатель на массив
 *     описателей параметров конфигурации портов-получателей.
 *
 * Существует два вида описателей порта.
 * Описатели первого типа (struct chan_port - chanPORT) находятся вместе
 * с соответствующим описателем канала в области системных данных модуля.
 * Описатели второго типа (qportCONTROL или sportCONTROL) находятся
 * в области системных данных соответствующего процесса процесса.
 *
 * Описатель порта в области системных данных модуля входит в состав описателя канала и
 * содержит следующую информацию:
 *   - указатель на описатель канала;
 *   - указатель на конфиг. параметры порта, указанные при конфигурировании канала;
 *   - указатель на конфиг. параметры порта, указанные при конфигурировании процесса;
 *   - направление передачи данных;
 *   - указатель на используемый буфер (для sampling портов);
 *   - заголовок списка буферов, взятых для чтения/записи (для queuing портов);
 *   - указатель на функцию нотификации с двумя параметрами;
 *   - идентификатор процесса.
 *
 * Описатель порта в области системных данных процесса (qportCONTROL или sportCONTROL)
 * содержит следующие данные:
 *   - указатель на описатель порта в области системных данных модуля
 *     (в описателе канала);
 *   - заголовок очереди потоков (для queuing портов).
 *
 * Используемые каналом буфера также находятся в области системных данных модуля.
 *
 * На этапе инициализации модуля создаются описатели каналов, включая описатели портов,
 * и используемые каналом буфера. При этом информация берется из описателей
 * конфигурации каналов. Описатели конфигурационных параметров портов,
 * созданных при конфигурировании процессов, находятся по имени портов
 * (имена портов должны быть уникальными).
 *
 * Функции создания портов CREATE_QUEUING_PORT и CREATE_SAMPLING_PORT
 * просматривают все каналы в поисках порта с указанным именем.
 */

#ifdef KERNEL

#include  <public/difference.h>
#include  <string.h>
#include  <strings.h>
#include  <syslog.h>
#if OS_MAJOR >= 3
#include  <arinc/partition.h>
#else
#  include  <mem.h>
#endif
#include  <netinet/in.h>
#include  <sys/apextypes.h>
#include  <private/ddlist.h>
#include  <private/sdlist.h>
#include  <private/thread.h>
#include  <arch/endian.h>

#if OS_MAJOR >= 3
#else
   extern struct desc_channel*      chanChannels;
   extern struct Connection_Table*  chanConnectionTable;
   extern struct desc_channel*      chanChannels;
   extern struct ModuleConfig       chanModuleConfig;
#endif


/* Конфигурируемые параметры, определяющие свойства канала */
struct chan_options {
   u_int options;   /* опции: печатать сообщения иниц., сбрасывать порт при созданнии  */
   u_int mpbuffers; /* число буферов, используемых для передачи по VME/RIO (на будущее)*/
   u_int mplinks;   /* максимальное число связей с модулем партнером                   */
   u_int period;    /* период служебного потока                                        */
};

#define chanBUFFER_ALIGN   256   /* граница, на которую выравнивается буфер
                                    внутримодульного  канала                */

/* Формат заголовка пересылаемого буфера с сообщением */
/* Тип сообщения из заголовка буфера */
#define chanHEADBUF_TYPE(head)         ntohl(head->chb_type)
/* Тип сообщения из заголовка буфера */
#define chanHEADBUF_CPU(head)          ntohl(head->chb_cpu)
/* Индекс канала из заголовка буфера */
#define chanHEADBUF_CHAN(head)         ntohl(head->chb_connect)
/*-------- Макрос chanHEADBUF_MSG - опрос номера сообщения из заголовка буфера --------*/
//!!Название неудачное, лучше chanHEADBUF_MSGNUM
#define chanHEADBUF_MSGNUM(head)       ntohl(head->chb_nMsg)
/* Длина сообщения из заголовка буфера */
#define chanHEADBUF_SIZE(head)         ntohl(head->chb_sizeMsg)
/* Флаг переполнения из заголовка буфера */
#define chanHEADBUF_OVERFLOW(head)     ntohs(head->chb_overflow)
/* Число буферов из заголовка буфера */
#define chanHEADBUF_NBUF(head)         ntohs(head->chb_nbuf)

struct desc_channel;
struct chan_port;

/* ------------ chanHEADBUF - заголовок буфера сообщения (все поля в NBO) ------------ */
typedef
struct chan_hdrbuf {
    u_int           chb_sizeMsg;           /* размер сообщения                             */
    u_int           chb_type;              /* тип сообщения                                */
    u_int           chb_cpu;               /* номер модуля источника                       */
    u_int           chb_connect;           /* индекс соединения источника                  */
    volatile u_int  chb_nMsg;              /* номер сообщения                              */
    short           chb_overflow;          /* флаг переполнения                            */
    short           chb_nbuf;              /* что думает о числе буферов приемника передатчик */
} chanHEADBUF;
typedef union __attribute__ ((__transparent_union__))
{
  char **c;
  struct chan_hdrbuf **chb;
} anyptr_t;
/* Типы сообщений */
typedef
enum {
   chanHEADBUF_TYPE_DATA,              /* сообщение пользователя                       */
   chanHEADBUF_TYPE_CONNECT,           /* служебный буфер, используемый для соединения */
   chanHEADBUF_TYPE_BUFFER             /* служебный буфер, сообщающий число буферов    */
} chanHEADBUF_TYPE;
/* ------------------------- chanBUFNODE - узел связи буферов ------------------------ */
typedef
struct chan_bufnode {
   union {
     sdlNODE snode;
     ddlNODE dnode;
   } node;
   volatile
   chanHEADBUF*         cb_pHead;/* указатель на заголовок буфера, содержащего сообщение  */
   u_int                cb_pPHead;/* физический адрес указателя на заголовок буфера       */
   u_int                cb_size; /* размер сообщения (нельзя хранить в памяти партнера)   */
   u_int                cb_nMsg; /* номер сообщения (нельзя хранить в памяти партнера)    */
   u_int                cb_flNBO;/* порядок байт(0 - не NBO; 1 - NBO (network byte order) */
   /*!!Для единообразия лучше использовать тип chanDESCRIPTION  здесь этот тип еще не определен*/
   struct desc_channel* cb_pChannel; /* адрес канала                                      */
   volatile u_int       cb_cntRead;  /* счетчик потоков, в текщий момент читающих
                                                  сообщение из этого буфера (SAMPLING) */
   struct {
      struct chan_bufnode *next;
      u_int                copysize;
   }                    cb_parts;
   union {
      struct mp_buf_attr attr;     /* атрибуты буфера (message passing)                 */
      struct sockaddr_in addr;
   }                    cb_add;
   int                  cb_error;
} chanBUFNODE;

/* Получение адреса заголовка буфера по адресу буфера */
#define chanBUF_TO_HEADBUF(p)          (chanHEADBUF*)(p)
/* Получение адреса описателя буфера по адресу узла */
#define chanNODE_TO_BUFNODE(node)      (chanBUFNODE*)(node)
/* Получение адреса узла буфера по адресу описателя буфера */
////!!В чем смысл макроса? ????
#define chanBUFNODE_TO_NODE(node)      (node)
/* Получение адреса описателя канала по адресу описателя буфера */
#define chanBUFNODE_TO_CHANNEL(node)   (node)->cb_pChannel
/* Получение адреса заголовка буфера по адресу узла */
#define chanBUFNODE_TO_HEADBUF(node)   ((chanHEADBUF*)(node)->cb_pHead)
/* Размер заголовка буфера */
#define chanHEADBUFSIZE                sizeof(chanHEADBUF)
/* Получение адреса сообщения по адресу буфера (его содержащего) */
#define chanHEADBUF_TO_DATA(p)         ((char*)(p) + chanHEADBUFSIZE)
/* Получение адреса буфера по адресу сообщения */
#define chanDATA_TO_HEADBUF(p)         ((char*)(p) - chanHEADBUFSIZE)
/* По адресу описателя получить адрес данных */
#define chanBUFNODE_TO_DATA(p)   (chanHEADBUF_TO_DATA((p)->cb_pHead))
/* Указать размер помещенного в буфер сообщения */
#define chanSET_DATASIZE(pBufNode,sz)  pBufNode->cb_parts.copysize = pBufNode->cb_size = sz
/* Поместить адрес узла в буфер сообщения */
#define chanHEADBUF_SET_BUFNODE(head,node)  (head)->chb_type=(u_int)(node)
/* Получение адреса узла из заголовка буфера */
#define chanHEADBUF_TO_BUFNODE(p)      (chanBUFNODE*)(p)->chb_type

/* Режим работы канала */
#define  SAMPLING_MODE    1
#define  QUEUING_MODE     2
#define  SAP_MODE         3
/* режим работы канала определен? */
#define  chanIS_MODE(pChan)           ((pChan)->dc_modeTransfer != 0)
/* режим работы канала определен? */
#define  chanIS_MODE_UNKNOWN(pChan)           ((pChan)->dc_modeTransfer == 0)
/* режим работы канала QUEUING_MODE? */
#define  chanIS_MODE_QUEUING(pChan)   ((pChan)->dc_modeTransfer == QUEUING_MODE)
/* режим работы канала SAMPLING_MODE? */
#define  chanIS_MODE_SAMPLING(pChan)  ((pChan)->dc_modeTransfer == SAMPLING_MODE)
#define  chanIS_MODE_SAP(pChan)       ((pChan)->dc_modeTransfer == SAP_MODE)

#if BYTE_ORDER == BIG_ENDIAN
#define CHAN_SIGNATURE       ('C'<<3*8)+('H'<<2*8)+('A'<<1*8)+'N' /* CHAN */
#else
#define CHAN_SIGNATURE       ('N'<<3*8)+('A'<<2*8)+('H'<<1*8)+'C'
#endif

typedef enum type_port {
   chanPORT_TYPE_STD,         /* стандартный порт                             */
   chanPORT_TYPE_RIO,         /* порт PSEUDO RIO-канала                       */
   chanPORT_TYPE_VME,         /* порт PSEUDO VME-канала                       */
   chanPORT_TYPE_INTR,        /* порт PSEUDO обработчика прерывания           */
   chanPORT_TYPE_NET,         /* порт PSEUDO NET-канала                       */
   chanPORT_TYPE_LAST
} chanPORT_TYPE;

struct mpchan_pseudo;

typedef
struct chan_stat_data {
    unsigned long csd_msgMinSize;      /**< минимальный размер сообщения  */
    unsigned long csd_msgMaxSize;      /**< максимальный размер сообщения */
    unsigned long csd_msgAvgSum;       /**< объём переданных данных       */
    int csd_isStat;                    /**< надо ли собирать статистику по данному каналу */
} chanSTAT;

typedef
struct chan_msg_control {
  unsigned cmc_numCur;
  unsigned cmc_numMax;
  unsigned cmc_size;
  unsigned cmc_freq;
  unsigned cmc_len;
  unsigned cmc_cnt;
} chanMSGCONTROL;

/* ----- Структура chan_service - описатель метода (функции) ----- */
struct chan_service {
   void    (*fun)( void*,void* );         /* функция                                   */
   void*   arg1;                          /* первый пар-р функции                      */
   void*   arg2;                          /* второй пар-р функции                      */
   void    (*flist)( struct chan_port* ); /* функция оповещения для списка портов      */
};
/*--------- Состояния описателя порта в структуре описателя канала (cp_state) ---------*/
typedef enum {
   chanPORT_START,           /* исходное состояние: после выделения памяти             */
   chanPORT_ENOPORT,         /* ошибка при иниц.: порт не найден в процессе            */
   chanPORT_EDIRECT,         /* ошибка при иниц.: одинаковые cp_direction              */
   chanPORT_EMODE,           /* ошибка при иниц.: не совпадают режимы работы           */
   chanPORT_EDIF,            /* ошибка при иниц.: не совпадают параметры               */
   chanPORT_EPSEUDO,         /* 5 ошибка при иниц.: PSEUDO не известно                 */
   chanPORT_E2PSEUDO,        /* ошибка при иниц.: с двух концов PSEUDO                 */
   chanPORT_EPARTNER,        /* ошибка при иниц.: ошибка у партнера                    */
   chanPORT_ECPU,            /* ошибка при иниц.: неверный номер модуля                */
   chanPORT_ENCHAN,          /* ошибка при иниц.: неправильный лог.номер               */
   chanPORT_E2CHAN,          /* 10 ошибка при иниц.: повторная ссылка на лог.номер     */
   chanPORT_EMEM,            /* ошибка при иниц.: нет памяти для буферов               */
   chanPORT_ECFG,            /* ошибка при иниц.: не сконфигурировано RIO/VME          */
   chanPORT_INIT,            /* состоялась инициализация порта                         */
   chanPORT_READY            /* канал обеспечен буферами и готов к работе              */
} chanPORT_STATE;

/*---------------- chanPORT_CONTROL - описатель порта (объект процесса) ---------------*/
typedef
struct  cport_control {
  objHEADER         cport_objHdr;      /* заголовок объекта                            */
  struct chan_port* cport_portChannel; /* указатель на описатель порта в канале        */
  u_int             cport_direct;      /* направление передачи (SOURCE/DESTINATION)    */
  u_int             cport_maxsize;     /* максимальный размер сообщения                */
  queHEAD           cport_waitHead;    /* очередь ждущих потоков                       */
  u_int             cport_nMsg;        /* число положенных в канал/взятых сообщений    */
} chanPORT_CONTROL;

/*-------------- chanPORT - описатель порта в структуре описателя канала --------------*/
typedef
struct chan_port {
  u_int               cp_partitionType;/* тип процесса (partition)                     */
  u_int               cp_state;        /* состояние                                    */
  chanPORT_TYPE       cp_portType;     /* тип порта: std/psd1/psd2...                  */
  u_int               cp_portId;       /* идентификатор порта (как объекта процесса)   */
  struct
  cport_control*      cp_pPort;        /* указатель на описатель порта
                                            (как объекта процесса)                     */
  u_int               cp_param;        /* параметр конфигурации                        */
  pid_t               cp_process;      /* идентификатор процесса                       */
  PORT_DIRECTION_TYPE cp_direction;    /* направление передачи данных                  */
  void*               cp_cfgPort;      /* указатель на описание порта в конфигурации   */
  struct
  mpchan_pseudo*      cp_mpPort;       /* указатель на описатель логической связи      */
  struct
  desc_channel*       cp_channel;      /* указатель на описатель канала                */
  char*               cp_name;         /* указатель на имя порта                       */
  ddlHEAD             cp_bufList;      /* заголовок списка буферов, взятых из канала
                                          для записи/чтения, позволяет вернуть в канал
                                          буфер в особых случаях (STOP,CANCEL)         */
  volatile
  chanBUFNODE*        cp_buffer;       /* адрес буфера, который отправляется по
                                          логическому каналу или читается приемником   */
  u_int               cp_message;      /* флаг наличия в канале сообщения (SAMPLING)   */
  struct chan_service cp_notify;       /* извещение о появлении буфера/сообщения       */
  u_int               cp_flnotify;     /* выполнение запроса на уведомление о появлении
                                          в канале буфера/сообщения (1 - выполнено)    */

  volatile u_int      cp_cntRead;      /* счетчик потоков, в текщий момент читающих
                                          сообщение из этого порта (SAMPLING)          */
  volatile u_int      cp_reqNotify;    /* запрос на уведомление о появлении
                                          в канале буфера/сообщения (1 - требуется)    */
  u_int               cp_reincarnation;/* реинкарнация процесса, устанавливается при
                                          CREATE порта                                 */
  sdlHEAD             cp_portlist;     /* заголовок списка присоединенных
                                             Queuing Port List                         */
  volatile u_int      cp_reqListNotify;/* запрос на уведомление о появлении
                                          в канале буфера/сообщения от
                                          QUEUING PORT LIST                            */
  chanSTAT            cp_stat;          /* данные для сбора статистики                  */
  chanMSGCONTROL      cp_msgControl;
} chanPORT;

/* По указателю на описатель порта получить описатель логической связи       */
#define chanPORT_MPPORT(port)     ((port)->cp_mpPort)

/* Адрес заголовка очереди взятых в обработку буферов    */
#define chanPORT_BUFLIST(canPort)   &(canPort)->cp_bufList

union list_head {
   sdlHEAD  shead;
   ddlHEAD  dhead;
};
struct  sdl_headn {
   sdlHEAD head;
   int     n;
};
/* Выставлено требование извещать порт? */
#define  chanIsReqNotifyUp(port)    ((port)->cp_reqNotify || ((port)->cp_reqListNotify))

/* Дополнительные свойства канала        */
//#undef chanOPTION_NOEMPTY              /* при создании порта не очищать очередь входных сообщений (отмена usermake.def) */
#define  chanOPTION_VERBOSE             1        /* сообщения периода инициализации    */
#define  chanOPTION_EMPTY               2        /* порт приемник пуст при создании    */
#define  chanOPTION_ETIMEDOUT_IGNORE    4        /* игнорировать ошибки ETIMEDOUT при
                                                    при посылке межмодульных сообщений */

/* Состояние описателя канала        */
typedef enum {
   chanCHNL_START,                     /* исходное состояние: после выделения памяти   */
   chanCHNL_INIT,                      /* описатель инициализирован                    */
   chanCHNL_ECFG,                      /* найдены ошибки конфигурирования              */
   chanCHNL_EBUF,                      /* не выделены буфера                           */
   chanCHNL_READY                      /* канал обеспечен буферами и готов к работе    */
} chanCHNL_STATE;

struct _chanTraceConfig {
  int                  on;       /* канал трассировать независимо от маски событий     */
  int                  size;     /* максимальный размер в записи регистрации для
                                            содержимого сообщения                      */
  int                  hdr;      /* протоколирование заголовка сообщения               */
};



/* ------------------------ chanDESCRIPTION - описатель канала ----------------------- */
typedef
struct desc_channel {
  u_int              dc_signature;     /* сигнатура описателя канала                   */
  u_int              dc_id;            /* идентификатор канала                         */
  u_int              dc_state;         /* состояние канала                             */
  u_int              dc_indPsd;        /* номер логической связи                       */
  u_int              dc_modeTransfer;  /* режим канала (QUEUING_MODE/SAMPLING_MODE)    */
  void*              dc_chanConfig;    /* указатель на конфигурацию канала             */
  u_int              dc_cntDest;       /* число портов DESTINATION                     */
  u_int              dc_maxsize;       /* максимальный размер сообщения                */
  u_int              dc_maxmsg;        /* максимальное число сообщений                 */
  volatile u_int     dc_curmsgs;       /* кол-во сообщений в очереди сообщений канала  */
  u_int              dc_overflow;      /* флаг переполнения очереди (QUEUING)          */
  system_time_t      dc_aperiod;       /* срок годности сообщения        (SAMPLING)    */
  int64_t            dc_nanPeriod;     /* срок годности сообщения в нсек               */
  system_time_t      dc_atime;         /* время помещения SAMPLING сообщения в канал   */
  u_int              dc_nMsg;          /* число выведенных в канал сообщений           */
  volatile u_int     dc_nBuf;          /* число свободных буферов в канале             */
  u_int              dc_options;       /* дополнительные (конфигурируемые) свойства    */
  struct timespec    dc_timeMsg;       /* время, за которое положено dc_timeNMsg       */
  u_int              dc_timeNMsg;      /* число выведенных сообщений за dc_timeMsg     */
  sdlHEAD            dc_bufList;       /* заголовок списка свободных буферов канала    */
  union list_head    dc_msgList;       /* заголовок списка сообщений (SDL/DDL)         */
  sdlHEAD            dc_auxMsgList;    /* заголовок вспомогат. очереди сообщений канала*/
  struct sdl_headn   dc_auxBufList;    /* заголовок вспомогат. очереди буферов канала  */

  struct chan_port   dc_srcPort;       /* описатель порта SOURCE                       */
  struct chan_port*  dc_pDstPort;      /* указатель на описатели портов DESTINATION    */
  void*              dc_pBuf;          /* указатель на пул буферов                     */
  u_int              dc_bufSize;       /* размер пула буферов                          */
//  u_int              dc_trace;         /* флаг трассировки независимо от маски событий */
  void             (*dc_ack)(int,struct chan_bufnode *,int); /* функция оповещения     */
  int                dc_mbox;          /* идентификатор  канала (CHAN_MBOX)            */
  struct _chanTraceConfig
                     dc_traceOptions;  /* опции трассировки канала                     */
} chanDESCRIPTION;


/* Адрес заголовка очереди буферов канала                          */
#define chanBUFLIST(chan) (sdlHEAD*)&(chan)->dc_bufList
/* Адрес заголовка очереди сообщений канала                        */
#define chanMSGLIST(chan)           &(chan)->dc_msgList
/* Адрес заголовка вспомогательной очереди сообщений канала        */
#define chanAUXMSGLIST(chan)        &(chan)->dc_auxMsgList
/* Адрес заголовка вспомогательной очереди буферов канала          */
#define chanAUXBUFLIST(chan)        &(chan)->dc_auxBufList

/*----------- Макрос chanSET_OVERFLOW - установка флага переполнения канала -----------*/
#define chanSET_OVERFLOW( pChan )   (pChan)->dc_overflow = 1
/*------------ Макрос chanUNSET_OVERFLOW - сброс флага переполнения канала ------------*/
#define chanUNSET_OVERFLOW( pChan ) (pChan)->dc_overflow = 0
/*------------ Макрос chanIS_OVERFLOW - опрос флага переполнения канала ---------------*/
#define chanIS_OVERFLOW( pChan )    (pChan)->dc_overflow


/* Опрос описателя порта-отправителя по описателю канала                               */
#define chanTO_PORTSRC(chan)      &(chan)->dc_srcPort
/* Опрос описателя первого порта-получателя по описателю канала                        */
#define chanTO_PORTDST(chan)      (chan)->dc_pDstPort
/* Опрос описателя порта-получателя по описателю канала и номеру порта-получателя      */
#define chanTO_PORTDSTJ(chan,j)   ((chan)->dc_pDstPort+j)

/* по указателю на описатель порта найти описатель канала */
#define chanPORT_TO_CHANNEL(port) (port)->cp_channel
/* по указателю на описатель порта найти имя порта */
#define chanPORT_NAME(port)       (port)->cp_name
/* по указателю на описатель порта получить ID порта */
#define chanPORT_ID(port)         (port)->cp_portId
/* по указателю на описатель порта получить ID процесса */
#define chanPORT_PROC(port)       (port)->cp_process
/* по указателю на описатель порта получить направление */
#define chanPORT_DIRECT(port)     (port)->cp_direction
/* по указателю на описатель порта получить конфигурацию порта */
#define chanPORT_CONFIG(port)     (port)->cp_cfgPort

/* Проверка, предназначен ли порт для вывода */
#define chanIS_PORT_SOURCE(port) ((port)->cp_direction == SOURCE)

/* ----- Макрос chanLATEST_MESSAGE - опрос последнего сообщения  ----- */
/*       (выполняется при закрытых прерываниях)          */
#define chanLATEST_MESSAGE(pChan)     \
   chanNODE_TO_BUFNODE(ddlLast( &pChan->dc_msgList.dhead) )

#define chanIS_PORT_STD(port)   \
  ((port)->cp_portType == chanPORT_TYPE_STD)
#define chanIS_PORT_MP(port)   \
  ((port)->cp_portType != chanPORT_TYPE_STD)
/* Порт обработчика прерывания?                          */
#define chanIS_PORT_INTR(port)   \
   ((port)->cp_portType == chanPORT_TYPE_INTR)
/* Проверка, принадлежит ли порт к псевдопартиции        */
#define chanIS_PORT_PRTPSD(port)   \
  ((port)->cp_partitionType == PSEUDO_PARTITION)
/* Проверка, принадлежит ли порт стандартной партиции    */
#define chanIS_PORT_PRTSTD(port)   \
  ((port)->cp_partitionType == STANDARD_PARTITION)

/* Получить свободный буфер                              */
extern chanBUFNODE*  chanGetBuffer( chanPORT* pChanPort, threadTCB* pThread );
/* Вернуть в канал освободившийся буфер                  */
extern void          chanPutBuffer ( chanBUFNODE* pBufNode, threadTCB* pThread );
/* Указать размер данных в сообщении */
extern void          chanSetMessageSize( chanBUFNODE* pBufNode, int size );
/* Получить размер данных в сообщении */
extern int           chanGetMessageSize( chanBUFNODE* pBufNode );
/* Получить адрес данных в буфере */
extern void*         chanDataAddress( chanBUFNODE* pBufNode );

extern int           modChannelsInit( struct chan_options* pChanOptions );
extern int           modChanBuffersInit( struct chan_options* pChanOptions );
extern chanPORT*     modPseudoSource( char *name );
extern chanPORT*     modPseudoDestination( char *name );

extern chanPORT*     modSPort ( char *name );
extern chanPORT*     modQPort( pid_t pid, const char *name );
extern chanPORT*     modSapPort( pid_t pid, const char *name );

#define mpchanNPETRACE()  //syslog(0,"%s %d\n",__PRETTY_FUNCTION__,__LINE__)

extern void cnlsShow( unsigned chan );
extern void portShow( unsigned chan );

extern void traceChanConfigSetting( void );

#endif /*KERNEL*/

#endif /* _CHANNEL_CHANNEL_H */
