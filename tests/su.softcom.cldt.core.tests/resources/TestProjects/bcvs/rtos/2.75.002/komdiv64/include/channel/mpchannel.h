/* channel/mpchannel.h
 */

#ifndef _CHANNEL_MPCHANNEL_H
#define _CHANNEL_MPCHANNEL_H

#ifdef KERNEL

#include  <semaphore.h>
#include  <syslog.h>
#include  <arch/board.h>
#include  <channel/channel.h>

#define MPCHAN_MODULE_SIGNATURE "mpChannels"

/* Опрос CPU и номера канала по описателю порта в структуре канала (в конфигурации)    */
#define  mpchanCONFIG_CHANNEL(port)    (port)->cp_param

#define MAX_MODULES                512 /* максимальное число модулей в крейте          */

#define MPCHAN_BUFFERSNUMBER       2   /*число буферов, предоставляемое для записи в MP*/

/* Характер запроса к обслуживающему потоку */
typedef enum {
   mpchanREQUEST_REGISTRATION  = 1,     /* обслужить требование регистрации партнера   */
   mpchanREQUEST_IO            = 2,     /* обслужить требование ввода/вывода           */
   mpchanREQUEST_ERROR         = 3,     /* обслужить информацию об ошибке              */
   mpchanREQUEST_CONNECT_CONF  = 4,     /* партнер требует указать максимальное число
                                                                                связей */
   mpchanREQUEST_CONNECT_DATA  = 5,     /* партнер указал максимальное число свзей,
                                           предлагает указать даннные своих логических
                                                                                связей */
   mpchanREQUEST_CONNECT_CHECK = 6,     /* партнер выставил параметры своих активных
                                           логических связей и предлагает проверить их */
   mpchanREQUEST_CONNECT_READY = 7,     /* партнер проверил параметры, указал свои,
                                                                  предлагает проверить */
   mpchanREQUEST_IO_READY      = 8,     /* партнер запустип обмен и требует
                                           сделать то же                               */
   mpchanREQUEST_RECONNECT              /*                                             */
} mpchanREQUEST;


/* Состояние модуля, фиксируемое в собственной таблице */
#define mpchanMODULE_START         1   /* идет инициализация описателей                */
#define mpchanMODULE_INIT          2   /* описатели инициализированы                   */
#define mpchanMODULE_READY         4   /* полностью готов                              */
#define mpchanMODULE_EAGAIN        5   /* партнер не доступен                          */
#define mpchanMODULE_RECONNECT     6   /* в процессе восстановления связи              */
#define mpchanMODULE_REGISTRATION  7    /* модуль должен зарегистрироваться в памяти
                                                                         партнера      */
#define mpchanMODULE_CONNECT       8    /* в процессе установления связи (получил запрос
                                                                       на регистрацию) */
#define mpchanMODULE_CONNECT_CONF  9    /* в процессе установления связи (получил запрос*/
#define mpchanMODULE_CONNECT_DATA  10   /* в процессе установления связи (получил запрос*/
#define mpchanMODULE_CONNECT_CHECK 11   /* в процессе установления связи (получил запрос*/
#define mpchanMODULE_CONNECT_READY 12   /* в процессе установления связи (получил запрос*/

/* ---------- mpchanHEADBUF - описатель буфера в MP (некоторые поля в NBO) ----------- */
typedef
struct mpchan_head_buffer {
  volatile int        mhb_NBOstate;    /* состояние                                    */
  volatile mpoffset_t mhb_NBOoffset;   /* смещение буфера (отправитель)                */
  chanBUFNODE*        mhb_pBufNode;    /* адрес узла буфера (получатель)               */
  chanHEADBUF*        mnb_pBuf;        /* указатель заголовок буфера сообщения (получ.)*/
  u_int               mhb_nMsg;        /* порядковый номер сообщения                   */
  u_int               mhb_size;        /* длина сообщения                              */
} mpchanHEADBUF;

/*----------- Состояния логического (межмодульного) соединения (mpcn_state) -----------*/
enum mpchan_connect_state {
   mpchanCONNECT_START          =   1, /* отправлен запрос на соединение               */
   mpchanCONNECT_CONCORD        =   2, /* в ответ на запрос на соединение отправлено
                                                                             согласие  */
   mpchanCONNECT_COMPLETE       =   3, /* выполнена проверка параметров обмена         */
   mpchanCONNECT_READY          =   4, /* логический канал готов к обмену данными      */
   mpchanCONNECT_ERROR          =   5, /* ошибка при сравнении параметров обмена       */
   mpchanCONNECT_RESTART        =   6  /* отправлен запрос на восстановление связи     */
};

/*---------- mpchanCONNECT - параметры логического (межмодульного) соединения ---------*/
typedef
struct mpchan_connect {
   enum mpchan_connect_state
       mpcn_state;                     /* состояние межмодульного соединения           */
  int  mpcn_mpmode;                    /* тип межмодульного соединения (VME/RIO)       */
  int  mpcn_transfermode;              /* режим передачи - QUEUING/SAMPLING            */
  int  mpcn_direction;                 /* направление направление передачи данных      */
  int  mpcn_link;                      /* номер логического соединения                 */
  int  mpcn_nChannel;                  /* число логических соединений модуля партнера  */
  int  mpcn_size;                      /* max размер сообщения от партнера             */
  int  mpcn_nbuf;                      /* число свободных буферов приемника            */
  int  mpcn_nMsg;                      /* номер сообщения                              */
  int  mpcn_chanId;                    /* ID канала                                    */
  char mpcn_chanName[MAX_NAME_LENGTH]; /* имя канала                                   */
  char mpcn_partName[MAX_NAME_LENGTH]; /* имя псевдопартиции                           */
} mpchanCONNECT;
/*------------- Состояния описателя межмодульного соединения (mpps_state) -------------*/
enum mpchan_pseudo_state {
   mpchanPSEUDO_START     =  0,   /* исходное состояние                           */
   mpchanPSEUDO_INIT      =  1,   /* исходное состояние сконфигурир. соединения   */
   mpchanPSEUDO_ECFGINFO  =  2,   /* информировать об ошибке конфигурации         */
   mpchanPSEUDO_ECONFIG   =  3,   /* ошибка конфигурации                          */
   mpchanPSEUDO_ELINK     =  4,   /* у партнера нет такой логической связи        */
   mpchanPSEUDO_EDIRECT   =  5,   /* у партнера такое же направление              */
   mpchanPSEUDO_ETRANSFER =  6,   /* у партнера отличается transfermode           */
   mpchanPSEUDO_ESIZE     =  7,   /* у партнера другой размер сообщения           */
   mpchanPSEUDO_CONCORD   =  8,   /* получено согласие на обмен                   */
   mpchanPSEUDO_READY     =  9,   /* готовность к обмену                          */
   mpchanPSEUDO_ECFGVME   =  10,  /* настройка VME требует коррекции              */
   mpchanPSEUDO_EINTERNAL =  11,  /* внутренняя ошибка                            */
   mpchanPSEUDO_CONNECT   =  12   /* получено требование соединения               */
};

/*---------------- mpchanPSEUDO - описатель (межмодульной) логической связи -----------*/
typedef
struct mpchan_pseudo {
  enum mpchan_pseudo_state
              mpps_state;              /* состояние (межмодульной) логической связи    */
  int         mpps_cpuPartner;         /* CPU партнера                                 */
  int         mpps_Link;              /* логический номер канала                      */
  struct chan_port*
              mpps_pChanPort;          /*указатель на описатель порта в структ. канала */
  struct mpchan_module*
              mpps_pModule;            /* указатель на описатель модуля-партнера       */
  struct mpchan_connect
              mpps_ownerState;         /* параметры соединения данного модуля          */
  struct mpchan_connect
              mpps_partnerState;       /* параметры соединения модуля-партнера         */
  struct mpchan_pseudo*
              mpps_Partner;            /* описатель логического межмодульного соединения
                                                                    на модуле-партнере */
  u_int       mpps_nMsg;               /*порядковый номер сообщения в данном соединении*/
  u_int       mpps_msgLast;            /*номер последнего отправленного сообщения в
                                                                     данном соединении */
  volatile
  u_int       mpps_reqNotify;          /* флаг, устанавливаемый партнером, когда его
                                       следует извещать о поступлении буфера/сообщения */
  volatile
   int        mpps_nBuf;               /* число свободных буферов известное отправителю*/
                                       /* хранится как у отправителя, так и получателя */
  /* Массив описателей ошибок отправки сообщений
   * 0 - ETIMEDOUT,
   * 1 - EBUSY,
   * 2 - прочее                                                                        */
  struct {
     int err;                          /* номер ошибки                                 */
     int nmsg;                         /* номер сообщения, при отправке которого
                                                                      случилась ошибка */
     int nErr;                         /* число ошибок                                 */
  }           mpps_Err[3];             /* причина невыполненных отсылок                */
//  int         mpps_nErr;               /* число невыполненных отсылок                  */
  volatile
  int         mpps_connect;            /* флаг требования на соединение                */
  mpchanHEADBUF       /* описатели буферов, выделенных получателем для передачи данных */
              mpps_headBuf[MPCHAN_BUFFERSNUMBER];
  struct chan_bufnode
              mpps_bufNodeOut;         /* описатель буфера исходящих служебных         */
  struct {
     volatile int state;
     int request;
  }  mpps_stateOutBuf;                 /* состояние буфера отсылки mpps_pServiceOutBuf */
  struct chan_service
              mpps_msgMove;            /* функция (метод) посылки/приема сообщения     */
  void        (*mpps_msgFunction)(void*,void*); /* чтение/запись сообщения             */
  struct sdl_headn mpps_auxBufList;
  void        *mpps_net;
  u_int       mpps_msgCnt;             /* количество переданных сообщений              */
  u_int       mpps_errCnt;             /* количество непереданных сообщений            */
} mpchanPSEUDO;

#if 0
#define mppsGET_NBUF(p) p->mpps_nBuf.n
#define mppsSET_NBUF(p,v) { assert(v >= 0 ); p->mpps_nBuf.n = v;p->mpps_nBuf.line=__LINE__;}
#define mppsDEC_NBUF(p) { assert(p->mpps_nBuf.n >0 ); p->mpps_nBuf.n--;p->mpps_nBuf.line=__LINE__;}
#define mppsINC_NBUF(p) { p->mpps_nBuf.n++;p->mpps_nBuf.line=__LINE__;}
#define mppsGET_STATE(p) p->mpps_state.n
#define mppsSET_STATE(p,v) { p->mpps_state.n = v;p->mpps_state.line=__LINE__;}
#else
#define mppsGET_NBUF(p)    p->mpps_nBuf
#define mppsSET_NBUF(p,v)  { assert(v >= 0 ); p->mpps_nBuf = v;}
#define mppsDEC_NBUF(p)    { assert(p->mpps_nBuf > 0 ); p->mpps_nBuf--;}
#define mppsINC_NBUF(p)    p->mpps_nBuf++
#define mppsGET_STATE(p)   p->mpps_state
#define mppsSET_STATE(p,v) p->mpps_state = v
#endif

#define mpchanIsReqNotifyUp(psd) (psd)->mpps_reqNotify /* проверка требования об извещ.*/

/* Описатель ошибки (RIO) */
//#define MSG_ERROR_RESPONSE        1    /* дать ответ партнеру на полученное сообщение  */
#define MSG_ERROR_PRINT           2    /* выдать печать об ошибке
                                             (несогласованность с параметрами партнера)*/
/*--------------- msg_error - описание ошибки, которую невозможно связать ---------------
                              с имеющимися логическими связями установленного модуля   */
struct msg_error {
   int                   errcode;      /* код ошибки принятого сообщения               */
   struct chan_hdrbuf    hdrbuf;       /* заголовок ошибочного сообщения               */
   struct mpchan_connect msgConnect;   /* запись ошибочного сообщения от партнера      */
   int                   errFlags;     /* флаги обработки сообщения об ошибке          */
};

struct mpchan_table;

/*--------------------- mpchanMODULE - описатель модуля-партнера ----------------------*/
typedef
struct mpchan_module {
  int           mpm_state;             /* состояние                                    */
  int           mpm_cpu;               /* номер процессора владельца                   */
  struct mpchan_table*
                mpm_pModulesPartner;   /* адрес таблицы связи модуля-партнера          */
  struct mpchan_module*
                mpm_pModule;           /* адрес описателя модуля в таблице партнера    */
//  int           mpm_registTo;          /* регистрация у партнера                       */
//  int           mpm_NBOregistFrom;     /* регистрация от партнера                      */
  volatile int  mpm_requests;          /* локальные флаги запросов                     */
  /* Число соединений данного модуля (в конфигурации ядра) */
  int           mpm_nCfgChannel;
  /* Число соединений  партнера (в конфигурации ядра) */
  int           mpm_NBOnCfgChannelPartner;
  int           mpm_psdActive;         /* число активных логических соединений         */
  struct mpchan_pseudo*                /* указатель на массив описаний логических      */
                mpm_pPseudo;           /* устройств межмодульных соединений            */
  struct mpchan_pseudo**               /* указатель на массив указателей описаний      */
                mpm_pPsdIndex;         /* логических устройств межмодул. соединений,   */
                                       /* связывает индекс соединения партнера с       */
                                       /* описателем межмодульного соединения          */
  struct mpchan_pseudo**               /* указатель на массив указателей опрашиваемых  */
                mpm_pPsdActive;        /* логических устройств межмодул. соединений    */
  int           mpm_IntrSend;          /* число прерываний, выданных модулю            */
  struct msg_error                     /* структура, описывающая ошибку, которую       */
                mpm_error;             /* невозможно связать с имеющимися логическими  */
                                       /* связями установленного модуля                */
  int           mpm_NotifyPartner;     /* число сообщений переданных данному
                                          модулю-партеру в данном сеансе
                                          (используется для уведомления)               */
  struct {
     u_int size;
     struct chan_bufnode *first;
     struct chan_bufnode *last;
     mpchanPSEUDO        *pPseudo;
  }             mpm_inprogressBuf;
  struct mpchan_table *mpm_table;
} mpchanMODULE;

#define mpchanNBOREQUEST(tbl,mod) tbl->mpms_RemoteModules[mod->mpm_cpu].NBOrequest

#define mpchanMOD_TO_PSEUDO(mod)          (mod)->mpm_pPseudo
/* Найти в описателе модуля описатель логической связи nLink */
#define mpchanMOD_TO_PSEUDO_LINK(mod,nLink) \
   (mpchanMOD_TO_PSEUDO(mod) + (nLink-1))
#define mpchanMOD_TO_PSDINDEX(mod)        (mod)->mpm_pPsdIndex
/* Указатель на массив указателей опрашиваемых (активных) межмодульных соединений      */
#define mpchanMOD_TO_PSDACTIVE(mod)       (mod)->mpm_pPsdActive

#define mpchanMODULE_INACCESSIBLE(mod,n)                                      \
{                                                                             \
   (mod)->mpm_state = mpchanMODULE_EAGAIN;                                    \
   syslog( LOG_LAZHEAD|LOG_ERR, errIErrorFmt(IEID_MPCHAN_INACCESIBLE), (mod)->mpm_cpu, n );      \
}
#if 0
#if OS_MAJOR == 3
#define mpchanMODULE_INACCESSIBLE(mod,n)                                      \
{                                                                             \
   (mod)->mpm_NBOstate = htonl(mpchanMODULE_EAGAIN);                          \
   hmPrintf( errIErrorFmt(IEID_MPCHAN_INACCESIBLE), (mod)->mpm_cpu, n );      \
}
#else
#define mpchanMODULE_INACCESSIBLE(mod,n)                                      \
   kprint( errIErrorFmt(IEID_MPCHAN_INACCESIBLE), (mod)->mpm_cpu,n )
#endif
#endif
#define mpchanIS_MODULE_ACCESSIBLE(mod)   ((mod)->mpm_state != mpchanMODULE_EAGAIN)

/* Состояние таблицы межмодульных соединений */
#define mpchanTABLE_START         0    /* исходное состояние                           */
#define mpchanTABLE_INIT          1    /* выделена память и инициализированы
                                          указатели на составные части                 */
#define mpchanTABLE_ECFG          2    /* ошибка конфигурирования                      */
//!!Это значение нигде НЕ используется можно добавить состояние!!не понял


#define  mpchanREQUEST_MODULE     1    /* флаг "запрос на обработку"                   */

struct mpchan_tables {
   sem_t       mpt_semId;              /* семафор потока mpms_threadId                 */
   int         mpt_verbose;
   int         mpt_threadPeriod;       /* период потока mpchanThread                   */
   void        (*mpt_qportPutMessage)(struct chan_bufnode*, struct thread_control*);
   chanBUFNODE*(*mpt_qportGetMessage)(struct chan_port*);
   void        (*mpt_sportPutMessage)(struct chan_bufnode*, struct thread_control*);
   struct mpchan_table *mpt_tables[chanPORT_TYPE_LAST];
};
struct mpchan_service {
   /* Функция инициализации описателя порта в канале */
      //!!Для единообразия лучше использовать тип mpchanTABLE не получится
   void (*mpchanPortInit)(struct mpchan_table*,struct chan_port*);
   /* Функция обработки требования на регистрацию от партнера */
   int (*registration)(struct mpchan_module* pModule);
   /* Функция инициирования процедуры установления связи с партнером */
   void (*connect)(struct mpchan_module* pModule);
   /* Функция обслуживания ввода/вывода */
   void (*serviceIO)(struct mpchan_module* pModule);
   /* Функция выделения памяти для буферов */
   int (*bufAlloc)( int size, int align, anyptr_t res, int verbose );
   /* Функция восстановления связи с партнером */
   void (*reconnect)(struct mpchan_module* pModule);
   int (*configCpu)(struct chan_port *port, int *cpu);
   int (*configPseudo)(struct chan_port *port, mpchanMODULE* pModule, mpchanPSEUDO **p);
};
/* ---------------- mpchanTABLE - таблица связи с модулям-партнерами  ---------------- */
typedef
struct mpchan_table {
  char        mpms_signature[12];      /* MPCHAN_MODULE_SIGNATURE                      */
  /* Функция выделения общей памяти  */
  int         mpms_state;              /* состояние                                    */
  int         mpms_cpu;                /* номер данного модуля в крейте                */
  int        (*mpms_alloc)();          /* функция аллокации памяти                     */
  struct mpchan_module*
              mpms_pModule;            /* адрес описателя собственного модуля в ТС     */
  int         mpms_maxIndex;           /* число опрашиваемых модулей                   */
  int         mpms_nChannel;           /* число MP-каналов модуля по конфигурации      */
  int         mpms_nChanPsd;           /* реальное число межмодульных каналов          */
  volatile
  int         mpms_requestService;     /* запрос к потоку обслуживания                 */
  int         mpms_maxBuffer;          /* максимал. размер приемного буфера для RIO    */
  int         mpms_nBuffers;           /* общее число приемных буферов для RIO         */
  //!!В случае RIO число модулей может быть велико (превышать 256).
  //   Конечно, число модулей-партнеров каждого модуля не столь велико,
  //   но заранее неизвестно. Его нужно сделать объектом конфигурирования.
  //   Желательно избежать прямого доступа к описателю модуля по его RIO-номеру
  //   Один из вариантов - перенумеровать всех партнеров на каждом модуле.
  struct mpchan_module                 /* прямой массив указателей на описатели модулей*/
              * volatile mpms_modules_by_cpu[MAX_MODULES];
  struct mpchan_module                 /* таблица описателей модулей                   */
              *mpms_modules_by_index[MAX_MODULES];
  volatile struct {
      u_int       NBOOffset;           /* смещение описателя модуля в таблице партнера */
      int         NBOrequest;          /* удаленный запрос                             */
  }           mpms_RemoteModules[MAX_MODULES];
  struct mpchan_service
              mpms_service;
  int         mpms_IntrRcv;
  struct msg_error                     /* структура, описывающая ошибку, которую       */
              mpms_error;              /* невозможно связать с имеющимися модулями     */
  int         mpms_bus;                /* используемая шина                            */
  const char *mpms_name;
  int         mpms_send_policy;        /* стратегия отсылки сообщений
                                            0 - одно сообщение лог.связи
                                            1 - по числу межмодульных буферов          */
  int         mpms_notify_policy;      /* стратегия уведомления партнера
                                            0 - по обслуживании каждой лог.связи
                                            1 - по ослуживании всех лог.связ. партнера */
  int         mpms_mpBuffers;          /* число буферов, выделяемое для межмодульного
                                            обмена на лог.связь VME                    */
  int         mpms_maxTransferSize;
  struct socket  *mpms_so;
} mpchanTABLE;

#define mpchanPOLICY_SEND_BUFFERS  1   /* отсылать подряд столько QUEUING-сообщений,
                                          сколько выделено межмодульных буферов        */
#define mpchanPOLICY_NOTIFY_MODULE 1   /* уведомлять партнера после обслуживания всех
                                          связей с ним                                 */
/* Флаги инициализации таблицы связи */
#define mpchanTABLE_QPORT          1   /* поддерживается QUEUING                       */
#define mpchanTABLE_SPORT          2   /* поддерживается SAMLIING                      */
#define mpchanTABLE_VERBOSE        4   /* расширенная диагностика                      */

/* Получить адрес таблицы связей с модулям-партнерами              */
#if OS_MAJOR == 2
extern struct mpchan_tables      *mpchanTablePointer;
#  define mpchanGET_TABLE(type)  mpchanTablePointer?mpchanTablePointer->mpt_tables[type]:0
#  define mpchanGET_TABLES()     mpchanTablePointer
#else
#  define mpchanGET_TABLE(type)  modLinkPage->mlp_tables?modLinkPage->mlp_tables->mpt_tables[type]:0
#  define mpchanGET_TABLES()     modLinkPage->mlp_tables
#endif
/* Получить описатель по номеру модуля */
static __inline__ struct mpchan_module * mpchanCPU_TO_MODULE(mpchanTABLE *table, int cpu)
{
   return table->mpms_modules_by_cpu[cpu];
}
static __inline__ struct mpchan_module * mpchanINDEX_TO_MODULE(mpchanTABLE *table, int i)
{
   return table->mpms_modules_by_index[i];
}

extern int    mpchanInit (  );
extern void   mpchanThread ( void );
extern int    mpchanSetReceiveFunction
    ( chanPORT* pChanPort, void (*f)(void*,void*), void* arg1, void* arg2 );
extern int    mpchanSetSendFunction
    ( chanPORT* pChanPort, void (*f)(void*,void*), void* arg1, void* arg2 );
extern void mpchanReconnect( mpchanTABLE*   pModules );

extern void   mptblShow( void );
extern void   mpchanModShow( unsigned mod );
extern void   psdShow( unsigned chan, int verbose );
extern int    mpchanConfigCPU(struct chan_port *port, int *cpu);
extern int    mpchanConfigPseudo(struct chan_port *pChanPort,  mpchanMODULE* pModule, mpchanPSEUDO **p);
extern int    mpchanLinkRegistration( mpchanMODULE* pModule, mpchanPSEUDO* pPseudo );

#endif /*KERNEL*/

#endif /* _CHANNEL_MPCHANNEL_H */
