/* trace/trace.h */
#ifndef _TRACE_TRACE_H
#define _TRACE_TRACE_H

#include <stdlib.h>
#include <arch/endian.h>
#include <public/difference.h>
#include <stdarg.h>
#include <arinc/partition.h>

#define traceREGION_MINIMUM   40960*5 /* минимальный размер выделяемый под трассу */

#ifndef __ASSEMBLER__

#undef DPRINT

#if 1
#define DPRINT(a) kprint a;
#else
#define DPRINT(a)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Имя раздела сохраняемой памяти, используемого под буфер трассы */
#define TRACEBUF              "TRACEBUF"

/* Имя раздела сохраняемой памяти, используемого под завещание трассы */
#define TRACE_LEGACY          "TRACELEGACY"

/* Размер логического блока в буфере регистрации событий */
#define TRACE_BLOCKSIZE       8192

#include <htrace/trrecord.h>

/* Идентификаторы функций вызываемых через SYSCALL */
enum trace_syscall {
//   trceSYSCALL_PROCESSING,
   trceSYSCALL_SETFUNC, // = 1,
   trceSYSCALL_SETVAR,
   trceSYSCALL_SETVARMASK,
   trceSYSCALL_SETOBJECT,
   trceSYSCALL_GETNEXT,
//   trceSYSCALL_NAMEEVENT,
   trceSYSCALL_CREATE,
   trceSYSCALL_BACKTRACE,
   trceSYSCALL_SETFILTER,
   trceSYSCALL_ATTRINIT,
   trceSYSCALL_LAST
};

extern int traceSysCall( enum trace_syscall fId, void *arg1, void* arg2 );

/* Приоритет обслуживающего потока */
#define traceSENDER_PRIORITY       1

#define TRACE_FLAGS_EXC_FP                1 /* протоколировать регистры FP в
                                               событии traceExceptionCase        */
#define TRACE_FLAGS_EXC_CALLSTACK         2 /* протоколировать стек вызовов в
                                               событии traceExceptionCase        */
#define TRACE_FLAGS_PREVIOUS_START        4 /* протоколировать стек вызовов в
                                               событии traceExceptionCase        */

/* Состояние файла трассы */
#define traceLOGSTATUS_EMPTY        0x01
#define traceLOGSTATUS_FULL         0x02
#define traceLOGSTATUS_MIDLE        0x04
#define traceLOGSTATUS_OVERRUN      0x08
#define traceLOGSTATUS_ERROR        0x10
#define traceLOGSTATUS_ERROR_IGNORE 0x20
#define traceLOGSTATUS_OPEN         0x40
#define traceLOGSTATUS_CLOSE        0x80

/* Режимы трассировки потоков */
#define traceTHREADS_NONE      0  /*не трассиовать потоки*/
#define traceTHREADS_USER      1  /*поток пользователя*/
#define traceTHREADS_SYS       2  /*системные потоки*/
#define traceTHREADS_ALL       3  /*системные потоки и пользователя*/

/* Состояние буфера трассы */
#define traceBUFSTATUS_EMPTY       0x1
#define traceBUFSTATUS_FULL        0x2
#define traceBUFSTATUS_MIDLE       0x4
#define traceBUFSTATUS_OVERRUN     0x8
#define traceBUFSTATUS_FLUSHING   0x10  /* идет освобождение буфера          */
#define traceBUFSTATUS_OPEN       0x20  /* протоколирование открыто (create) */
#define traceBUFSTATUS_SHUTDOWN   0x40  /* идет завершение                   */
#define traceBUFSTATUS_LOG        0x80  /* трасса с LOG                      */
#define traceBUFSTATUS_AUTO      0x100  /* трасса открыта при инициализации  */
#define traceBUFSTATUS_LOCK      0x200  /* запрещена запись в буфер          */
#define traceBUFSTATUS_CLEAR     0x400  /* очистка буфера                    */
//#define traceBUFSTATUS_WRITE     0x800  /* идет запись в LOG                 */
#define traceBUFSTATUS_PREVIOUS 0x1000  /* старт на этапе инициализации ОС   */
#define traceBUFSTATUS_ARINC    0x2000  /* буфер принадлежит ARINC процессу  */

/* Состояние буфера трассы в отношении выгрузки старого состояния */
#define traceBUFUNLOAD_CONFIG      0x1  /* выгрузка задана в конфигурации               */
#define traceBUFUNLOAD_CHECK       0x2  /* выполнена проверка старого состояния буфера  */
#define traceBUFUNLOAD_ADDRESS     0x4  /* адрес буфера ие совпадает                    */
#define traceBUFUNLOAD_FAIL        0x8  /* буфер испорчен                               */
#define traceBUFUNLOAD_OPEN        0x10  /* буфер открыт                                */
#define traceBUFUNLOAD_EMPTY       0x20 /* буфер пуст                                   */
#define traceBUFUNLOAD_YES         0x40 /* буфер исправен и содержит информацию         */
#define traceBUFUNLOAD_DUMP        0x80 /* ввиду ошибок в буфере выводить DUMP          */
#define traceBUFUNLOAD_REBOOT      0x100 /* предыдущий сеанс закончился выставлением
                                           требования REBOOT                            */
#define traceBUFUNLOAD_SERVE       0x200/* процедура обслуживания старого буфера выпол. */

#define traceBUFUNLOAD_ER_SIGNATURE     0x0400  /* ошибка в сигнатуре                   */
#define traceBUFUNLOAD_ER_BLKSIZE       0x0800  /* ошибка в размере блока               */
#define traceBUFUNLOAD_ER_CPUTYPE       0x1000  /* ошибка в cputype                     */
#define traceBUFUNLOAD_ER_SIGNINIT      0x2000  /* ошибка в signatureInit               */
#define traceBUFUNLOAD_ER_FLUSH         0x4000  /* политика POSIX_TRACE_FLUSH           */
#define traceBUFUNLOAD_ER_OPTSIGNATURE  0x8000  /* ошибка в сигнатуре trace_options     */
#define traceBUFUNLOAD_ER_SENSIGNATURE  0x10000 /* ошибка в сигнатуре tracesender_proc  */

/* Выставленные требования */
#define traceREQUEST_SHUTDOWN   0x01    /* закрыть поток трассы              */
#define traceREQUEST_FORCE      0x02    /* принять экстренные меры           */
#define traceREQUEST_REBOOT     0x04    /* перезагрузить модуль              */
#define traceREQUEST_FLUSH      0x08    /* выгрузить буфер в файл            */
#define traceREQUEST_CLOSE      0x10    /* закрыть файл трассы               */
#define traceREQUEST_UNLOAD     0x20    /* выгрузить буфер                   */
//#define traceREQUEST_CLEAR      0x40    /* очистить буфер                   */

/* Действия после регистрации исключения */
#define TRACE_EXCEPTION_NONE       0x0  /* оставить без внимания             */
#define TRACE_EXCEPTION_STOP       0x1  /* остановить регистрацию событий    */
#define TRACE_EXCEPTION_SHUTDOWN   0x2  /* закрыть поток трассы              */
#define TRACE_EXCEPTION_REBOOT     0x3  /* перезагрузить модуль              */

#if 0
/* Буфер доступен для записи? */
#define traceIsUnlock(traceBufHeader) \
  ((traceBufHeader) && !(traceBufHeader->status & traceBUFSTATUS_LOCK))
/* Запретить запись в буфер */
#define traceLock(traceBufHeader) traceBufHeader->status |= traceBUFSTATUS_LOCK
/* Разрешить запись в буфер */
#define traceUnlock(traceBufHeader) traceBufHeader->status &= ~traceBUFSTATUS_LOCK
#endif

#if BYTE_ORDER == BIG_ENDIAN
   /* Сигнатура, указывающая на инициализацию буфера */
#  define TRACE_INIT_SIGNATURE   ('b'<<3*8)+('I'<<2*8)+('n'<<1*8)+'I' /* bInI */
   /* Сигнатура, требования принудительной перезагрузки модуля */
#  define TRACE_REBOOT_SIGNATURE ('r'<<3*8)+('E'<<2*8)+('b'<<1*8)+'O' /* rEbO */
/* Сигнатура описателя конфигурации */
#  define traceCFGSIGNATURE      ('t'<<3*8)+('r'<<2*8)+('C'<<1*8)+'f' /* trCf */
/* Сигнатура описателя trace_options */
#  define traceOPTIGNATURE       ('t'<<3*8)+('r'<<2*8)+('O'<<1*8)+'p' /* trOp */
/* Сигнатура описателя tracesender_proc */
#  define traceSENDERSIGNATURE   ('t'<<3*8)+('r'<<2*8)+('S'<<1*8)+'e' /* trSe */
#else
#  define TRACE_INIT_SIGNATURE   ('I'<<3*8)+('n'<<2*8)+('I'<<1*8)+'b'
#  define TRACE_REBOOT_SIGNATURE ('O'<<3*8)+('b'<<2*8)+('E'<<1*8)+'r'
#  define traceCFGSIGNATURE      ('f'<<3*8)+('C'<<2*8)+('r'<<1*8)+'t'
#  define traceOPTIGNATURE       ('p'<<3*8)+('O'<<2*8)+('t'<<1*8)+'t' /* trOp */
#  define traceSENDERSIGNATURE   ('e'<<3*8)+('S'<<2*8)+('t'<<1*8)+'t' /* trSe */
#endif

/* Резервируется в блоке для размещения служебных записей */
#define BLOCK_RESERVE   (sizeof(trcSTOP_RECORD) \
        +sizeof(trcSTART_RECORD)                \
        +sizeof(trcRESUME_RECORD)               \
        +sizeof(trcOVERFLOW_RECORD)             \
        +sizeof(trcFLUSHSTART_RECORD)           \
        +sizeof(trcFLUSHSTOP_RECORD)            \
        +sizeof(trcSHUTDOWN_RECORD)             \
        +sizeof(trcREBOOT_RECORD))

struct tracefunc_arg;

/* Описатель обрабатываемых прерываний */
#define TRACE_VECTOR_MAX      4  /* максимальная обрабатываемая вложенность         */

struct trace_context {
   int      trv_count;                       /* уровень вложенности прерывания      */
   struct {
      objid_t  thread_id;                    /* текущий поток                       */
      unsigned system;                       /* ARINC/POSIX                         */
   } trv_context[MAX_NUMBER_OF_PARTITIONS];  /* описатели процессов                 */
   int trv_vector[TRACE_VECTOR_MAX];         /* массив векторов прерываний          */
};

/* Описатель события */
struct trace_event_property {
   long        event;         /* идентификатор события                  */
   const char* shortname;     /* короткое имя                           */
   const char* eventname;     /* полное имя                             */
   long        recsize;       /* размер записи протоколирования события */
   short       hdrsize;       /* размер заголовка записи:
                                  0 - trcHEADER_RECORD_LIST,
                                  1 - trcHEADER_RECORD                  */
   short       registration;  /* допустимость регистрации переменных    */
};

/* Описатель функции, выполняемой как внешнее событие процесса */
struct func_procevent {
   void*       adrfunc;          /* адрес функции                                   */
   const char  namefunc[20];     /* имя функции                                     */
};

/* Описатель, используемый при обработке стека вызовов */
struct trace_backtrace {
   int      tb_nesting;          /* число записанных окон                           */
   int      tb_finish;           /* флаг конца просмотра                            */
   struct   {
      void *callAdrs;
      void *funcAdrs;
   } tb_data[1];
};

  /*  */
  struct pproc_ext;
  void traceProcStub( void );
  /* Настройка протоколирования переменных и объектов */
  int  traceSetOptions( void );
  /* Получить глобальную маску событий */
  trace_event_set_t *traceGetGlobalMask( void );

#ifdef KERNEL

#include <private/queue.h>

  /* Описатель буфера трассы */
  typedef
  struct  traceBufHeader
  {
    char      signature[12];/* сигнатура                                             */
    volatile int
              status;       /* состояние буфера */
    int       blocksize;    /* размер блока буфера                                   */
    int       cputype;      /* архитектура                                           */
    unsigned  timer_freq;   /* загружаемое в таймер значение                         */
    int       timer_hz;     /* число тиков в секунду таймера                         */
    int       fd;           /* дескриптор log файла                                  */
    objid_t   sender;       /* идентификатор потока traceSender                      */
    queHEAD   quewait;      /* очередь ожидания завершения SHUTDOWN                  */
  //  objid_t*  sem;          /* семафор, синхронизирующий процедуры create/shutdown   */
    int       logpriority;  /* приоритет traceSender                                 */
    volatile int
              offTo;        /* куда заносим событие                                  */
    volatile int
              offFrom;      /* откуда читаем                                         */
    volatile __uint32_t
              offReserve;   /* куда заносим событие при переполнении буфера          */
    int       block;        /* текущий блок в буфере                                 */
    int       maxblock;     /* максимальный номер текущего блока в буфере            */
    volatile int
              tail;         /* в текущем блоке осталось свободным                    */
    int       blocks;       /* #текущего с начала протоколирования                   */
    int       flushblock;   /* #блока с начала протоколирования, которым заканчивается
                                                                            FLUSH    */
    int       logblock;     /* #блока в LOG, в который будем писать                  */
    int       logblocks;    /* # блока, который ставим в заголовок блока             */
    int       maxlogblock;  /* максимальный номер текущего блока в log файле         */
    int       logstatus;    /* состояние log файла                                   */
    int       flush_error;  /* результат flush                                       */
    int       testament;    /* завещание которое следует отработать                  */
    volatile u_int          /* прерывание начатой регистрации - циклический счетчик  */
              traceInterrupt;
    int       traceInterruptIgnore;  /* игнорировать прерывание регистрации          */
    int       size;         /* размер памяти, выделенной под протоколирование        */
    int       bufsize;      /* размер буфера регистрации событий                     */
    int       logsize;      /* размер файла                                          */
    int       fullpolicy;   /* политика заполнения буфера                            */
    int       logfullpolicy;/* политика заполнения файла                             */
    int       maxdatasize;  /* максимальный размер данных пользователя               */
    int       flags;        /* флаги, управляющие процессом регистрации              */
    int       tracethreads; /* протоколируемые потоки                                */
    struct trace_timespec
              createtime;   /* время создания трассы                                 */
    struct trace_timespec
              clockres;     /* разрешение часов                                      */
    char      version[TRACE_NAME_MAX+8];      /* версия средств протоколирования     */
    char      name[TRACE_NAME_MAX];           /* имя протокола                       */
    char      logname[PATH_MAX];              /* имя файла протокола                 */
    const struct trace_event_property*
              traceEventProperties;
    int       logError;            /* код ошибки вывода в файл                        */
    int       offToPresent;        /* куда пишем в blockPresent                       */
    int       offFromPresent;      /* откуда читаем blockPresent                      */
    int       tailPresent;         /* осталось в blockPresent                         */
    int       statusPresent;       /* состояние блока                                 */
    struct traceFilter
              filter;              /* действующий фильтр событий                      */
    int       saveBuffer;          /* флаг сохранения буфера                          */
    /* Функция, которая может быть вызвана при регистрации события */
    void      (*traceFunction)( struct tracefunc_arg* );
    int       traceFunctionSize;   /* размер предоставляемой ей памяти                */
    int       serviceBlockNum;     /* число служебных блоков в начале файла           */
    int       logrotate;           /* запущен receiver                                */
    int       processOpen;         /* флаги процессов, открывших поток трассы         */
    int       waitNextFlags;       /* флаги процессов, ждущих появления события    `  */
    int       requestFlags;        /* флаги требований которые следует выполнить   `  */
    int       signatureInit;       /* сигнатура, указывающая на инициализацию буфера  */
    int       loghighpriority;     /* приоритет для экстренного закрытия трассы       */
  //  int       reqReboot;           /* сигнатура требования перезагузки модуля         */
    void*     tracestream;         /* адрес описателя в предыдущем сеансе             */
    int       excPolicy;           /* действия при регистрации исключения             */
    struct thread_control*
              senderTCB;           /* адрес описателя потока sender                   */
    void*     prsmemTrace;         /* адрес описателя раздела сохраняемой памяти,
                                      используемой под описатель и буфер трассы       */
    void*     prsmemPart;          /* адрес описателя раздела сохраняемой памяти,
                                      используемой для записи завещания               */
    struct trace_context
              intvectors;          /* описатель обрабатываемых прерываний             */
    int       configload;          /* загрузка конфигурации из файла                  */
    const char*                    /* инструментальной эвм                            */
              configname;          /* имя файла конфигурации                          */
    trace_attr_t
              attr;                /* атрибуты трассы                                 */
    pid_t     pid;
    struct trace_config*
              traceConfig;         /* описатель конфигурации трассы                   */
    struct trace_options*
              traceOptions;        /* описателя общих данных процессов                */
    struct tracesender_proc*       /* адрес описателя трассы процесса для чтения из   */
              traceProcRead;       /*                                 любого процесса */
    struct tracesender_proc*       /* адрес описателя трассы процесса для записи      */
              traceProcWrite;      /*                         (для главного процесса) */
    int       offServiceBlocks;    /* смещение к началу служебных блоков от заголовка */
    char      buf;                 /* начало буфера событий                           */
  } *traceHEADBUF;

  typedef struct traceBufHeader   bufheader_t;

#if OS_MAJOR == 2
      /* кольцевой буфер записей трассировки */
    extern volatile bufheader_t*    traceRingBuf;
#endif



  /* создание служебного  потока записи протокола в файл */
  int traceSenderCreate( traceHEADBUF pBufHeader );
  /* стартовая функция потока записи протокола в файл */
  void traceSender( void  );
  /* инициализация буфера трассы */
  int traceBufferInit( pid_t pid, traceHEADBUF* buf, const trace_attr_t *attr );
  /* сброс буфера */
  void traceBufferClear( traceHEADBUF buf );
  /* открыть файл трассировки*/
  int  traceFileOpen( const char *path, int flags );
//  int  traceNewFileOpen( const char *path, const char* ext, char* newpath, int rotate );
  int traceNewFileOpen( const char *path, const char* ext, char* newpath, int rotate, pid_t pid );
  /* принудительная выгрузка буфера трассировки */
//  int  traceUnload( const char *filename, int flags );
  /* принудительная выгрузка буферов трассировки */
  void  traceUnload( void );
  /* байтов в буфере трассировки */
  int  traceUnloadSize( void );
  /* поиск объекта в списке трассируемых */
  int  traceIsObject( void *objId );
  /* поиск имени в списке трассируемых */
  int  traceIsName( const char *name );

  /* Выгрузка буфера трассы, если он сохранился */
  /* Dump области, предоставленной под буфер трассы */
  extern int          traceBufferDump( traceHEADBUF pBufHeader, const char *filename, int rotate );
  /* сохранение буфера трассировки предыдущей сессии */
  extern int          traceBufferSave( traceHEADBUF pBufHeader, const char *filename, int rotate );
//  extern void         traceSetObjectRegistration( objid_t objId );
  extern void         traceShowInit( void );
  extern void         traceShowBuffer( void );

#ifdef INCLUDE_TRACE

/* Трассировать прикладные потоки? */
#define traceIS_THREAD_USER    (traceThreadUser == objFLAGS_TRACING)
/* Трассировать системные потоки? */
#define traceIS_THREAD_SYS     (traceThreadSys == objFLAGS_TRACING)

/* Установить атрибут трассировки при создании сист.потока */
#if 0
#define   traceSET_ATTR_SYSTHREAD(attr)                           \
   if(traceIS_THREAD_SYS)                                         \
     pthread_attr_settracestate( attr, PTHREAD_TRACE_ENABLE );    \
   else                                                           \
     pthread_attr_settracestate( attr, PTHREAD_TRACE_DISABLE )
#endif

/* Завершить трассу */
#define traceSET_SHUTDOWN(pBufHeader)                             \
pBufHeader->status |= traceBUFSTATUS_SHUTDOWN
/* Завершить трассу */
#define traceSET_FLUSH(pBufHeader)                                \
pBufHeader->status |= traceBUFSTATUS_FLUSHING

/* Состояние буфера трассы */
#define traceIsEmpty(traceBufHeader)                              \
  (traceBufHeader->status & traceBUFSTATUS_EMPTY)
#define traceIsFull(traceBufHeader)                               \
  (traceBufHeader->status & traceBUFSTATUS_FULL)
#define traceIsOverrun(traceBufHeader)                            \
  (traceBufHeader->status & traceBUFSTATUS_OVERRUN)
#define traceIsOpen(traceBufHeader)                               \
  (traceBufHeader->status & traceBUFSTATUS_OPEN)
/* Было требование flush? */
#define traceIsFlush(traceBufHeader)                              \
  (traceBufHeader->status & traceBUFSTATUS_FLUSHING)
/* Трасса завершается? */
#define traceIsShutdown(traceBufHeader)                           \
  (traceBufHeader->status & traceBUFSTATUS_SHUTDOWN)
/* Описатель буфера инициализирован? (поток открывался) */
#define traceIsInit(traceBufHeader)                               \
 (traceBufHeader->signatureInit == TRACE_INIT_SIGNATURE)
/* Трасса имеет LOG? */
#define traceIsLog(traceBufHeader)                                \
 (traceBufHeader->status & traceBUFSTATUS_LOG)
#define tracelogIsOverrun(traceBufHeader)                         \
 (traceBufHeader->traceProcRead->logstatus & traceLOGSTATUS_OVERRUN)
#   define tracelogIsEmpty(traceBufHeader)                        \
        (traceBufHeader->traceProcRead->logstatus & traceLOGSTATUS_EMPTY)
#   define tracelogIsFull(traceBufHeader)                         \
        (traceBufHeader->traceProcRead->logstatus & traceLOGSTATUS_FULL)
#   define traceSetFlagStatus(traceBufHeader,fl)                  \
         traceBufHeader->status |= fl
#   define traceUnsetFlagStatus(traceBufHeader,fl)                \
         traceBufHeader->status &= ~(fl)
#   define traceCheckFlagStatus(traceBufHeader,fl)                \
         (traceBufHeader->status & fl)
#   define tracelogSetError(traceBufHeader,err)                   \
         traceBufHeader->traceProcWrite->logError = err
#   define traceGetToBuf(traceBufHeader)                          \
         (&traceBufHeader->buf + (traceBufHeader->to - &traceBufHeader->traceProc->traceBufHeader->buf))
#   define traceLogBlocks(traceBufHeader)                         \
         traceBufHeader->traceProcWrite->logblocks
#   define traceLogBlock(traceBufHeader)                          \
         traceBufHeader->traceProcWrite->logblock
#   define traceSetFlagLogStatus(traceBufHeader,fl)               \
         traceBufHeader->logstatus |= fl
#   define traceUnsetFlagLogStatus(traceBufHeader,fl)             \
         traceBufHeader->logstatus &= ~(fl)
#   define traceCheckFlagLogStatus(traceBufHeader,fl)             \
         (traceBufHeader->traceProcWrite->logstatus & fl)
#   define traceGetLogStatus(traceBufHeader)                      \
         traceBufHeader->traceProc->logstatus
#   define traceGetRequest(traceBufHeader)                        \
        traceGetOptions()->to_requestFlags


/* Код возврата при блокировании потока на объекте */
#define traceSTATE_PENDED  -1

/* Проверить идентификатор события */
#define traceEVENT_VALIDATE(id) \
  ((id) >= MIN_EVENT_ID && (id) <= MAX_EVENT_ID)


/* Установить,снять флаг трассировки */
#define traceSET_OBJECT(obj)     objSET_TRACESTATE(obj)

/* проверить флаг трассировки объекта */
#define traceOBJECT(obj)        (obj && objIS_TRACING(obj))
#define traceIS_OBJECT(obj)     traceOBJECT(obj)
#define traceTHREAD(t)          traceOBJECT(t)
/*#define traceTHREAD(t)    (t && t->m_objHdr.m_status == traceTHREAD_YES)*/


#define   traceSET_TIMESPEC_ZERO(name) \
    recbuf->name.tv_sec = 0; \
    recbuf->name.tv_nsec = 0
#define   traceSET_TIMESPEC(name,time) \
  if( time ) { \
    recbuf->name.tv_sec = (time)->tv_sec; \
    recbuf->name.tv_nsec = (time)->tv_nsec; \
  } else { \
    traceSET_TIMESPEC_ZERO(name); \
  }
#define   traceSET_TIMEOUT(time)  traceSET_TIMESPEC( timeout,time)



#if OS_MAJOR == 2
  extern trace_event_set_t      traceGMask;   /*глобальная маска трассируемых событий*/
  extern struct trace_options*   oc2_traceOptions;
#endif

/* Описатели регистрируемых событий */
extern const struct trace_event_property traceGroupProperties[];
extern const struct trace_event_property traceEventProperties[];

/* Получить адрес массива описателей событий */
extern const struct trace_event_property* traceEventPropertiesGet( void );
extern const struct trace_event_property* traceGroupPropertiesGet( void );
/* Информация о существующих объектах */
extern void         traceSetObjectsNameAndProperty( void );
/* Дать имена функциям, представленным в записях адресами */
extern void         traceSetFunctionsName( void );

/* Получить место в буфере под событие */
void* traceAlloc( traceHEADBUF pBufHeader, size_t size );
/* Посредник обработчика прерывания */
void traceIntFunc( long arg, int vector, struct pt_regs* pSave, vfuncptr_t func );

void traceEvent( trace_event_id_t event_id,
            const void *data_ptr, size_t data_len );
//int  tracePpeInit( struct trace_descriptor* traceDescriptor );
int  tracePpeInit( void );
struct trace_config* traceGetConfig( void );
/* Создание потока трассы по данным конфигурирования */
//int  TraceProcessing( struct trace_descriptor* traceDescriptor );
int traceGetAddress( pid_t pid, struct trace_region* pRegion );
/* Проверка наличия данных предыдущего сеанса в буфере */
/*static __inline__*/ int traceIsOldData( traceHEADBUF pBufHeader, int print );

//void traceSetRequest( int request );
int  traceRequestShutdown( void );
int  traceShutdown( void );

void traceRequestReboot( void );
void traceReboot( void );
void traceSetStop( void );
int  traceWriteAdd( int event, trcHEADER_RECORD* pHdr, void* p, int size );
//void traceValidationMessage( const char* format, va_list args );

void traceShowInit( void );
/* Выполнение директив конфигурации трассы */
//int traceProcessing( int (*traceSetOptions)(void) );
/* Именование стандартных объектов */
int traceObjectName( objid_t id, void* adrId, void *adr, const char *name, int cl_type );
/* Опрос статуса протокола */
int traceGetStatusInfo( struct posix_trace_status_info *statusinfo );
/* Опрос атрибутов протокола */
int traceGetAttrInfo( trace_attr_t *attr );
/* Загрузка файла конфигурации */
int traceLoadConfiguration( void );
/* Обработка файла конфигурации */
//int traceConfigProcessing( int (*traceSetOptions)(void) );
/* Раннее начало регистрации событий */
int tracePreviousStart( void );
/* Проверка некоторых полей описателя буфера */
//void traceCheckOldBuffer( pid_t pid );
//int traceCheckOldBuffer( traceHEADBUF pBufHeader, int print );
/* Опрос версии трассы */
void traceGetVersion( char* buf );
/*  Вывод маски событий в служебные блоки */
void traceServiceMask( void );
/*  Перемещение точки вывода данных из буфера */
void traceChangeFrom( traceHEADBUF pBufHeader, size_t size );
/* Вывод записи представления */
//int traceSendPresent( traceHEADBUF pBufHeader );
/* Вывод записей расписания */
void traceScheduleRecords( traceHEADBUF pBufHeader );
/* Формирование в служебном блоке записи представления */
void traceSetPresentRecord( traceHEADBUF pBufHeader );
/* Извещение процесса о закрытии трассы */
void traceProcNotifyShutdown( pid_t pid );
/* Извещение процесса о конце синхронизации трассы */
void traceProcNotifyFlushStop( pid_t pid );
/* Запустить регистрацию событий */
int traceStart( void );
/* Остановить регистрация событий */
int traceStop( void );
/* Сброс трассы */
void traceClear( pid_t pid );
/* Установить требование на REBOOT или SHUTDOWN */
void traceSetRequest( traceHEADBUF pBufHeader, int request );

#if OS_MAJOR >= 4
#   include <private/module.h>
/* Выполнить функцию в контексте процесса */
void tracePostProcEvent( pid_t pid, fptr_t tracefunc, long arg1, long arg2 );
/* Установить флаги состояния трассы процесса */
void traceProcSetStatus( pid_t pid, int flags );
/* Снять флаги состояния трассы процесса */
void traceProcUnsetStatus( pid_t pid, int flags );
/* Установить флаги состояния LOG трассы процесса */
void traceProcSetLogStatus( pid_t pid, int flags );
/* Снять флаги состояния LOG трассы процесса */
void traceProcUnsetLogStatus( pid_t pid, int flags );
/* Извещение процесса о событии */
void traceProcNotify( pid_t pid, vfuncptr_t tracefunc, int arg );
/* Извещение процессов о событии */
int traceNotify( fptr_t tracefunc );
#endif

int traceFilesOpen( void );
bufheader_t* traceBufCfgAddressForWrite( pid_t pid );
bufheader_t* traceBufCfgAddressForRead( pid_t pid );

struct sport_control;
struct qport_control;
struct sap_control;
struct qlist_control;
void tracePropertySPort( int procId, struct sport_control* pSPort );
void tracePropertyQPort( int procId, struct qport_control* pQPort );
void tracePropertyQSap( int procId, struct sap_control* pSap );
void tracePropertyQList( int procId, struct qlist_control* pList );

#   if OS_MAJOR != 2
    struct obj_header;
    struct thread_control;
    struct mq_control;
    struct buffer_control;
    struct bbrd_control;
    struct sport_control;
    struct qport_control;
    struct  sap_control;
    struct qlist_control;

    void tracePropertySem( int pricId, struct obj_header* pHdr );
    void tracePropertyMq( int pricId, struct mq_control* pMq );
    void tracePropertyBuf( int pricId, struct buffer_control* pBuf );
    void tracePropertyBbrd( int pricId, struct bbrd_control* pBbrd );
#   endif

#else
   /* ! INCLUDE_TRACE */

//#  define   traceSET_ATTR_SYSTHREAD(attr)
//#  define   tracePropertyThread(thread,pid)
//#  define   traceSetObjectRegistration(id )
#  define   traceEvent(id,adr,size )
#  define   traceObjectName(id,adrId,adr,name,type)

#endif /* ! INCLUDE_TRACE */
#endif /*KERNEL*/

#ifdef __cplusplus
}
#endif

#endif /*__ASSEMBLER__ */
#endif /*_TRACE_TRACE_H*/

