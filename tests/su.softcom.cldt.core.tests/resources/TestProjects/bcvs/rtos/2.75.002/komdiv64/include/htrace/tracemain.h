/* tracemain.h */
#ifndef _TRACE_TRACEMAIN_H
#define _TRACE_TRACEMAIN_H

#include <version.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <limits.h>
#include <arinc/partition.h>
#include <sys/types.h>
#include <sys/objclasses.h>
#include <htrace/traceevent.h>

#ifdef __cplusplus
extern "C" {
#endif

struct posix_trace_event_info {
  trace_event_id_t  posix_event_id;          /* id события              */
  pid_t             posix_pid;               /* id процесса             */
  void             *posix_prog_address;      /* адрес события           */
  int               posix_truncation_status; /* признак усечения записи */
  struct timespec   posix_timestamp;         /* время события           */
  pthread_t         posix_thread_id;         /* id потока               */
  int               interrupt_vector;        /* вектор прерывания       */
  int               return_code;             /* вектор прерывания       */
};

struct posix_trace_status_info {
  int  posix_stream_status;         /* состояние трассы                 */
  int  posix_stream_full_status;    /* переполнение трассы              */
  int  posix_stream_overrun_status; /* потеря записей трассы            */
  /*c log*/
  int  posix_stream_flush_status;   /* выполняется flush трассы         */
  int  posix_stream_flush_error;    /* ошибка flush трассы              */
  int  posix_log_full_status;       /* переполнение в log               */
  int  posix_log_overrun_status;    /* потеря записей в log             */
};
struct trace_timespec {
   int sec;
   int nsec;
};

struct trace_attribute {
  u_long  signature;                /* сигнатура                                 */
  char    version[TRACE_NAME_MAX];  /* версия средств протоколирования           */
  int     inherited;                /* наследование трассы
                                       (при порождении процесса)                 */
  int     logfulpolicy;             /* действия при переполнении файла трассы    */
  int     streamfulpolicy;          /* действия при переполнении буфера трассы   */
  int     maxdatasize;              /* максимальный размер данных при протоколировании
                                       событий прикладной программы              */
  int     streamsize;               /* минимальный размер буфера трассы          */
  int     logsize;                  /* максимальный размер файла трассы          */
  struct  trace_timespec
          clockres;                 /* разрешение часов                          */
  struct  trace_timespec
          createtime;               /* время создания протокола                  */
  char    name[TRACE_NAME_MAX];     /* имя протокола                             */
  int     savebuf;                  /* если в момент создания потока буфер не
                                       пуст, отгрузить его содержимое в файл     */
};

/* Возможное значение второго аргумента функции posix_trace_eventset_fill() */
#define POSIX_TRACE_ALL_EVENTS  0
/* Значение атрибута log-full-policy;
 * запись в файл производится без учета ограничений на длину файла
 * (атрибут log-max-size игнорируется)
 * (см. функцию posix_trace_attr_setlogfullpolicy())
 */
#define POSIX_TRACE_APPEND       4

/*POSIX_TRACE_CLOSE_FOR_CHILD*/

/* Тип события "во время протоколирования обнаружена ошибка" */
#define POSIX_TRACE_ERROR  traceServiceError
/* Тип события "изменение фильтра трассируемых событий"
 * (см. функцию posix_trace_set_filter())
 */
#define POSIX_TRACE_FILTER  traceServiceFilter

/* Значение атрибута stream-full-policy "регулярный вывод в LOG"
 * (см. функцию posix_trace_attr_setstreamfullpolicy())
 */
#define POSIX_TRACE_FLUSH        3
/* Тип события "начало синхронизации протокола",
 * (см. функцию posix_trace_set_filter())
 */
#define POSIX_TRACE_FLUSH_START traceServiceFlushStart
/* Тип события "конец синхронизации протокола",
 * (см. функцию posix_trace_set_filter())
 */
#define POSIX_TRACE_FLUSH_STOP  traceServiceFlushStart
/* Значение статуса протокола "идет flush"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_FLUSHING     1
/* Значение статуса протокола (posix_stream_full_status/posix_log_full_status)
 * "буфер заполнен" (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_FULL       1
/* Значение атрибутов (log_full_policy/stream_full_policy) "пишем циклически"
 * (см. функции posix_trace_attr_setlogfullpolicy(),
 * posix_trace_attr_setstreamfullpolicy())
 */
#define POSIX_TRACE_LOOP         1
/* Значение статуса протокола (posix_stream_overrun_status, posix_log_overrun_status)
 * "не было потерянных записей"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_NO_OVERRUN 0

/* Значение статуса протокола "flush не идет"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_NOT_FLUSHING 0

/* Значение статуса протокола (posix_stream_full_status, posix_log_full_status)
 * "в буфере есть место"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_NOT_FULL   0
/* Описание события трассы (posix_truncation_status) "запись не усекалась"
 * (усечение длины клиентских данных)
 */
#define POSIX_TRACE_NOT_TRUNCATED     0
/* Тип события "переполнение протокола" */
#define POSIX_TRACE_OVERFLOW    traceServiceOverflow
/* Значение статуса протокола (posix_stream_overrun_status, posix_log_overrun_status)
 * "были потеряны записи"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_OVERRUN    1
/* Тип события "восстановление протоколирования после переполнения" */
#define POSIX_TRACE_RESUME     traceServiceResume
/* Значение статуса протокола posix_stream_status "идет протоколирование"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_RUNNING    1
/* Тип события "пуск протоколирования" (см. функцию posix_trace_start()) */
#define POSIX_TRACE_START       traceServiceStart
/* Тип события "останов протоколирования" (см. функцию posix_trace_stop()) */
#define POSIX_TRACE_STOP        traceServiceStop
/* Значение статуса протокола posix_stream_status "протоколирование остановлено"
 * (см. функцию posix_trace_get_status())
 */
#define POSIX_TRACE_SUSPENDED          0
#define POSIX_TRACE_SYSTEM_EVENTS      1  /* системные события                */
/* описание события трассы - posix_truncation_status*/
#define POSIX_TRACE_TRUNCATED_READ     2  /* запись была усечена при чтении   */
#define POSIX_TRACE_TRUNCATED_RECORD   1  /* запись была усечена              */
/* Предопределенное событие пользователя */
#define POSIX_TRACE_UNNAMED_USER_EVENT  traceUserGroupEnd
/* Значение атрибутов (log-full-policy,stream-full-policy)
 * (см. функцию posix_trace_attr_setlogfullpolicy(),
 * posix_trace_attr_setstreamfullpolicy())
 */
#define POSIX_TRACE_UNTIL_FULL         2  /* пока есть место                  */

/* Возможное значение второго аргумента функции posix_trace_eventset_fill() */
#define POSIX_TRACE_WOPID_EVENTS       2

#define POSIX_TRACE_SET_EVENTSET       0
#define POSIX_TRACE_ADD_EVENTSET       1
#define POSIX_TRACE_SUB_EVENTSET       2

/* Типы трассируемых переменных (способ интерпретации при показе) */
enum traceTypeVariable {
  traceTYPE_SHORT = 0xFF,
  traceTYPE_INT,
  traceTYPE_PNT,
  traceTYPE_FLOAT,
  traceTYPE_DOUBLE,  /*5*/
  traceTYPE_STR,
  traceTYPE_MEM,
  traceTYPE_FUNC,
  traceTYPE_FP,
  traceTYPE_BACKTRACE,
  traceTYPE_LAST
};

/* Условный адрес переменной traceTYPE_FP */
#define traceADDR_TYPE_FP           0xfffffff0
/* Условный адрес переменной traceTYPE_BACKTRACE */
#define traceADDR_TYPE_BACKTRACE    0xfffffff1
/* Условный адрес переменной, отображающей содержание канального сообщения */
#define traceADDR_TYPE_CHANMSG      0xfffffff2

#ifndef INCLUDE_NOTRACE
  /* интерфейс POSIX */
  /* удаление описателя атрибутов протокола */
  int posix_trace_attr_destroy( trace_attr_t *attr );
  /* получить разрешение часов*/
  int posix_trace_attr_getclockres( const trace_attr_t *attr,
     struct timespec *resolution );
  /* получить время создания трассы*/
  int posix_trace_attr_getcreatetime( const trace_attr_t *attr,
     struct timespec *createtime );
  /* опрос атрибута "версия средств создания трассы*/
  int posix_trace_attr_getgenversion( const trace_attr_t *attr,
         char *genversion );
  /* получить действия по заполнению log файла */
  int posix_trace_attr_getlogfullpolicy( const trace_attr_t *attr,
       int *logpolicy );
  /* получить макс.размер файла протокола */
  int posix_trace_attr_getlogsize( const trace_attr_t *attr,
    size_t *size );
  /* опрос атрибута "максимальный размер пользовательских данных" */
  int posix_trace_attr_getmaxdatasize( const trace_attr_t *attr,
         size_t *maxdatasize );
  /* опрос атрибута "максимальный размер записи о системном событии" */
  int posix_trace_attr_getmaxsystemeventsize( const trace_attr_t *attr,
         size_t *eventsize );
  /* опрос атрибута "максимальный размер записи о событии прикладной программы */
  int posix_trace_attr_getmaxusereventsize( const trace_attr_t *attr,
         size_t data_len, size_t *eventsize );
  /* получить имя трассы*/
  int posix_trace_attr_getname( const trace_attr_t *attr,
         char *tracename );
  /* получить атрибут "действия при переполнении буфера протокола" */
  int posix_trace_attr_getstreamfullpolicy( const trace_attr_t *attr,
       int *streampolicy );
  /* получить размер буфера протокола */
  int  posix_trace_attr_getstreamsize( const trace_attr_t *attr,
    size_t *size );
  /* инициализация описателя атрибутов протокола */
  int posix_trace_attr_init( trace_attr_t *attr );
  /* установить действия по заполнению log файла */
  int posix_trace_attr_setlogfullpolicy( trace_attr_t *attr,
       int logfulpolicy );
  /* установить макс.размер файла протокола */
  int posix_trace_attr_setlogsize( trace_attr_t *attr, size_t size );
  /* установка атрибута "максимальный размер пользовательских данных" */
  int posix_trace_attr_setmaxdatasize( trace_attr_t *attr,
         size_t maxdatasize );
  /* установка атрибута "имя протокола" */
  int posix_trace_attr_setname( trace_attr_t *attr,
       const char *tracename );
  /* установка атрибута "действия при переполнении буфера протокола" */
  int posix_trace_attr_setstreamfullpolicy( trace_attr_t *attr,
       int streampolicy );
  /* установка атрибута "размер буфера протокола" */
  int  posix_trace_attr_setstreamsize( trace_attr_t *attr, size_t size );
  /* очистка протокола */
  int posix_trace_clear( trace_id_t trid );
  /* создание протокола */
  int posix_trace_create( pid_t pid, const trace_attr_t *attr,
    trace_id_t *trid );
  /* создание протокола (с записью в файл) */
  int posix_trace_create_withlog( pid_t pid,
    const trace_attr_t *attr, int fildes, trace_id_t *trid );
  /* протоколирование события прикладной программы */
  void posix_trace_event( trace_event_id_t event_id,
                      const void *data_ptr, size_t data_len );
  /* получить имя события прикладной программы */
  int posix_trace_eventid_get_name( trace_id_t trid,
         trace_event_id_t event, char *event_name );
  /* откыть идентификатор события с указанным именем */
  int posix_trace_eventid_open( const char *event_name,
       trace_event_id_t *event );

  /* манипуляции над маской событий */
  int posix_trace_eventset_add( trace_event_id_t event_id,
         trace_event_set_t *set );
  int posix_trace_eventset_del( trace_event_id_t event_id,
         trace_event_set_t *set );
  int posix_trace_eventset_empty( trace_event_set_t *set );
  int posix_trace_eventset_fill( trace_event_set_t *set, int what );
  int posix_trace_eventset_ismember(trace_event_id_t event_id,
         const trace_event_set_t *set, int *ismember );
  /* синхронизация протокола - буфер в LOG */
  int posix_trace_flush( trace_id_t trid );
  /* получить атрибуты трассы */
  int posix_trace_get_attr( trace_id_t trid, trace_attr_t *attr );
  /* получить маску трассы */
  int posix_trace_get_filter( trace_id_t trid, trace_event_set_t *set );
  /* получить состояние трассы */
  int posix_trace_get_status( trace_id_t trid,
       struct posix_trace_status_info *statusinfo );
  /* дать следующее событие */
  int posix_trace_getnext_event( trace_id_t trid,
       struct posix_trace_event_info *event,
       void *data, size_t num_bytes,
       size_t *data_len, int *unavailable );
  /* установить маску трассы */
  int posix_trace_set_filter( trace_id_t trid,
       const trace_event_set_t *set, int how );
  int posix_trace_shutdown( trace_id_t trid );
  /* запуск протоколирования */
  int posix_trace_start( trace_id_t trid );
  /* останов протоколирования */
  int posix_trace_stop ( trace_id_t trid );
  /* дать следующее событие */
  int posix_trace_timedgetnext_event( trace_id_t trid,
       struct posix_trace_event_info *event,
       void *data, size_t num_bytes,
       size_t *data_len, int *unavailable,
       const struct timespec *abs_timeout );
  /* дать следующее событие */
  int posix_trace_trygetnext_event( trace_id_t trid,
       struct posix_trace_event_info *event,
       void *data, size_t num_bytes,
       size_t *data_len, int *unavailable);

#  if !defined(POSIX_SOURCE)

#define TRACE_CHANNEL_CONFIG_SIZE   8

  struct traceChanIdConfig {
    int             on;       /* канал трассировать независимо от маски событий        */
    int             id;       /* идентификатор канала                                  */
    int             size;     /* максимальный размер в записи регистрации для
                                         содержимого сообщения                         */
    int             hdr;      /* протоколирование заголовка сообщения                  */
  };
  struct traceChanConfig {
     const char     channame[PATH_MAX];/* имя файла конфигурации каналов               */
     int            size;
     vfuncptr_t     traceChanConfigSetting; /* функция устанавливает в канале признак трассировки */
     struct traceChanIdConfig  traceChanIdOptions[TRACE_CHANNEL_CONFIG_SIZE];
  };

  struct trace_region {
      void* pRegion;
      void* pRegionKernel;
      size_t size;
  };

#define TRACE_PROCESS_MAX   8 /* максимальное число трассируемых процессов             */

  struct  traceBufHeader;

  struct traceProcIdConfig {
    int             on;       /* канал трассировать независимо от маски событий        */
    int             id;       /* идентификатор процесса                                */
  };

/* Флаги инициализации памяти буфра */
#define TRACEBUF_NOT_FOUND      0x01    /* сохраняемая облать отсутствует              */
#define TRACEBUF_ERROR_OPEN     0x02    /*                                             */
#define TRACEBUF_ERROR_SIZE     0x04    /* размер мал                                  */
#define TRACEBUF_NO_ACTION      0x08    /* процесс не протоколируется                  */

  /* Параметры трассы процесса */
  struct traceMemConfig {
     int    status;              /* результаты инициализации сохраняемой памяти        */
     int    tracesize;           /* размер памяти, выделенной под протоколирование     */
     struct trace_region         /* адрес и размер буфера трассы                       */
            traceBufferRegion;
     struct prsmem_partition*
            prsmembuffer;        /* адрес описателя раздела сохраняемой памяти
                                                                      под буфер трассы */
     struct prsmem_partition*
            prsmemlegacy;        /* адрес описателя раздела сохраняемой памяти
                                                                        под завещание  */
     struct prsmem_partition*
            prsmemoption;        /* адрес описателя раздела общих данных процессов     */
     int    fd;                  /* дескриптор log файла                               */
     char   filename[PATH_MAX];  /* имя файла                                          */
  };

  /* Описатель, используемый служебным потоком */
  struct tracesender_proc {
      int    signature;           /* сигнатура                                          */
      struct  traceBufHeader*     /* адрес заголовка буфера, выставляемый процессом,    */
             pBufHeaderWrite;     /*            по которому процесс пишет в него        */
      struct  traceBufHeader*     /* адрес заголовка того же буфера, по которому его    */
             pBufHeaderRead;      /*                        могут читать все процессы   */
      int    unloadStatus;        /* флаги состояния трассы процесса                    */
      char   filename[PATH_MAX];  /* имя файла                                          */
      int    fd;                  /* дескриптор log файла                               */
      int    status;              /* флаги состояния трассы процесса                    */
      int    logstatus;           /* флаги состояния LOG трассы процесса                */
      int    logError;            /* ошибка при записи в LOG                            */
      int    lockBufService;      /* блокирование записи в служебные блоки              */
      int    lockBufOverride;     /* блокирование записи в уже занятые блоки (при LOOP) */
      uint32_t
             logblocks;           /* число блоков, записанных в файл                    */
      uint32_t
             logblock;            /* номер блока, записываемого в файл                  */
  };

  struct tracesender {
      objid_t     sender;         /* идентификатор потока traceSender                   */
      struct thread_control*
                  senderTCB;      /* адрес описателя потока sender                      */
      sem_t       semaphore;
      int         logpriority;
      int         loghighpriority;/* приоритет для экстренного закрытия трассы          */
  };

  /* Параметры трассы общие для всех процессов */
  struct trace_options {
      uint32_t    to_signature;       /* сигнатура                                    */
      uint32_t    to_single;          /* буфер трассы один на все пргоцессы           */
      uint32_t    to_fileIndex;       /* текущий индекс LOG                           */
      uint32_t    to_requestFlags;    /* флаги требований которые следует выполнить   */
      uint32_t    to_nproc;           /* число трассируемых процессов                 */
      uint32_t                        /* cписок трассируемых процессов                */
                  to_listproc[MAX_NUMBER_OF_PARTITIONS];
      struct tracesender
                  to_sender;          /* описатель данных, используемых traceSender   */
      struct tracesender_proc         /* описатели, используемые traceSender
                                         для обслуживания трассируемых процессов      */
                  to_proc[MAX_NUMBER_OF_PARTITIONS];
  };

  struct tracefunc_arg;

  struct trace_config {
     int         signature;     /* сигнатура                                           */
     int         savebuf;       /* флаг сохранения буфера                              */
     int         serviceBlocks; /* число служебных блоков                              */
     int         fullpolicy;    /* политика заполнения буфера                          */
     int         logfullpolicy; /* политика заполнения файла                           */
     int         maxdatasize;   /* максимальный размер данных пользователя             */
     int         logsize;       /* размер файла                                        */
     int         tracecreate;   /* создать поток трассы на этапе иницииализации ОС     */
     int         tracelog;      /* поток трассы создать с LOG                          */
     int         tracestart;    /* запустить регистрацию после создания потока трассы  */
     int         tracethreads;  /* тип потоков, события с которыми регистрируются      */
     int         tracevariables;/* максимальное число протоколируемых переменных
                                                                             (адресов) */
     int         traceobjects;  /* максимальное число протоколируемых объектов         */
     int         logpriority;   /* приоритет потока, выгружающего трассу в LOG         */
     int         loghighpriority;   /* приоритет потока при экстренном завершении
                                                                      протоколирования */
     int         traceInterruptIgnore; /* игнорирование прерывания регистрации события */
     int         excPolicy;     /* действия при регистрации исключения                 */
     int         exceptionFlags;
     int         show;          /* средства показа описателя трассы                    */
     char        tracename[TRACE_NAME_MAX];   /* имя трассы                            */
     char        logname[PATH_MAX];           /* имя файла завещания                   */

     int         logrotate;     /* максимальное значение индекса в имени файла         */
     struct traceFilter
                 tracemask;     /* маска событий                                       */
     vfuncptr_t  shell;         /* адрес функции osShell                               */
     void        (*traceFunction)( struct tracefunc_arg* );
     int         traceFunctionSize;    /* размер предоставляемой ей памяти             */
//     funcptr_t   traceSetOptions;  /* адрес функции, выполняемой после создания трассы */
     int         configload;    /* загрузка конфигурации из файла инструментальной эвм */
     int         previous;      /* регистрация событий на этапе инициализации ОС       */
     const char  configname[PATH_MAX]; /* имя файла конфигурации                       */
     const char  maskname[PATH_MAX];   /* имя файла маски событий                      */
     const char  varmaskname[PATH_MAX];/* имя файла маски протоколирования переменных  */
     struct traceChanConfig
                 traceChanOptions;
     struct traceProcIdConfig          /* идентификаторы трассируемых процессов        */
                 traceConfigProcesses[TRACE_PROCESS_MAX];
     funcptr_t   prsmemPartFind;       /* адрес функции поиска раздела сохр. памяти    */
     funcptr_t   prsmemPartOpen;       /* адрес функции поиска раздела сохр. памяти    */
     struct trace_options*             /* адрес общих данных с точки зрения процесса   */
                 pOptions[MAX_NUMBER_OF_PARTITIONS];
     struct traceMemConfig             /* информация о памяти, выделенной процессам    */
                 procConfiguration[MAX_NUMBER_OF_PARTITIONS];
     char              date[28];       /* дата и время создания протокола               */
  };


  struct tracefunc_arg;

  /* Назначение имени объекту трассировки */
  int traceName( objid_t id, void *adr, const char *name, int type );
  int traceObjectNameOnAddress( void *adr, const char *name, int clId );
#  if OS_MAJOR >= 3
  int traceNameOnId( objid_t id, const char *name );
#  endif
  /* Установить объект трассируемым независимо от маски событий */
  int traceSetObjectOnName( const char* name, int class_type );
  int traceSetObjectOnAddress( void* adr, int class_type );

  /* Установить протоколируемую переменную */
  int traceSetVariable
    ( void *adr, char *name, int type, int size, trace_event_set_t *mask );
  /* Зарегистрировать протоколируемую функцию */
  int traceFunctionRegistration
               ( void (*Func)(struct tracefunc_arg*), char *name, int size );
  /* Установить протоколируемую функцию для текущего процесса */
  int traceSetFunction( void *arg );
  /* Установить маску событий на протоколирование переменных и вывов функций */
  int traceSetMaskVariable( trace_event_set_t *mask );

#  endif  /* ! POSIX_SOURCE */

#  if defined(KERNEL) /*&& !defined(__POSIX__)*/
#     include <htrace/trrecord.h>
#     include <htrace/trace.h>
#     include <htrace/trobject.h>
#     include <htrace/trregistration.h>
#     include <htrace/tracestream.h>
#  endif


#else
  /* INCLUDE_NOTRACE */

  /* интерфейс POSIX */
  #define  posix_trace_attr_destroy(attr)                             0
  #define  posix_trace_attr_getclockres( attr, resolution )           0
  #define  posix_trace_attr_getcreatetime( attr, createtime )         0
  #define  posix_trace_attr_getgenversion( attr, genversion )         0
  #define  posix_trace_attr_getlogfullpolicy( attr, logpolicy )       0
  #define  posix_trace_attr_getlogsize( attr, size )                  0
  #define  posix_trace_attr_getmaxdatasize( attr, maxdatasize )       0
  #define  posix_trace_attr_getmaxsystemeventsize( attr, eventsize )  0
  #define  posix_trace_attr_getmaxusereventsize                       \
                                          ( attr, len, eventsize )    0
  #define  posix_trace_attr_getname( attr, tracename )                0
  #define  posix_trace_attr_getstreamfullpolicy( attr, streampolicy ) 0
  #define  posix_trace_attr_getstreamsize( attr, size )               0
  #define  posix_trace_attr_init(attr)                                0
  #define  posix_trace_attr_setlogfullpolicy( attr, logfulpolicy )    0
  #define  posix_trace_attr_setlogsize( attr, size )                  0
  #define  posix_trace_attr_setmaxdatasize( attr, maxdatasize )       0
  #define  posix_trace_attr_setname( attr, tracename )                0
  #define  posix_trace_attr_setstreamfullpolicy( attr, streampolicy ) 0
  #define  posix_trace_attr_setstreamsize( attr, size )               0
  #define  posix_trace_clear( trid )                                  0
  #define  posix_trace_create( pid, attr, trid )                      0
  #define  posix_trace_create_withlog( pid, attr, fildes, trid )      0
  #define  posix_trace_event( event_id, data_ptr, data_len )
  #define  posix_trace_eventid_get_name( trid, event, event_name )    0
  #define  posix_trace_eventid_open( event_name, event)               0
/*
  #define  posix_trace_eventtypelist_getnext_id()       0
  #define  posix_trace_eventtypelist_rewind( trid )     0
*/
  #define  posix_trace_eventset_add(id,set)                           0
  #define  posix_trace_eventset_del(id,set)                           0
  #define  posix_trace_eventset_empty(set)                            0
  #define  posix_trace_eventset_fill(set,what)                        0
  #define  posix_trace_eventset_ismember(id,set,m)                    0
  #define  posix_trace_flush( trid )                                  0
  #define  posix_trace_get_attr( trid, attr )                         0
  #define  posix_trace_get_filter( trid, set )                        0
  #define  posix_trace_get_status( trid, statusinfo )                 0
  #define  posix_trace_set_filter( trid, set, how )                   0
  #define  posix_trace_shutdown( trid )                               0
  #define  posix_trace_start( trid )                                  0
  #define  posix_trace_stop( trid )                                   0
  #define  posix_trace_getnext_event( trid, event, data,              \
                     num_bytes, data_len, unavailable )               0
  #define  posix_trace_timedgetnext_event( trid, event, data,         \
                    num_bytes, data_len, unavailable, abs_timeout )   0
  #define  posix_trace_trygetnext_event( trid, event, data,           \
                    num_bytes, data_len, unavailable )                0


  #define  traceName(id,adr,name,type)                                0
  #define  traceNameOnId(id,name)                                     0
  #define  traceObjectNameOnAddress(adr,name,clId)                    0
  #define  traceSetVariable(adr,name,type,size,mask)                  0
//  #define  traceSetName(name,type)                      0

//  #define  traceUnsetVariable(adr)                      0
//  #define  traceUnsetFunction(adr)                      0
//  #define  traceUnsetObject(obj,type)                   0

#  ifdef KERNEL
#     include <htrace/trace.h>
#  endif

#endif   /* ! INCLUDE_NOTRACE */


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_TRACEMAIN_H*/
