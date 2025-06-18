/* trrecord.h - структура записей */

/*
  07.04.2003 в заголовок записи добавлен адрес события
*/
#ifndef _TRACE_TRRECORD_H
#define _TRACE_TRRECORD_H

#include <arch/trace.h>

#define trace_func_ptr        uint32_t  /* поле записи указывает на функцию   */
#define trace_ptr             uint32_t  /* поле записи содержит адрес         */

#define TRACE_NAMESIZE        12          /* отводится под имя объекта          */

#define traceRECORD_ALIGN     sizeof(int) /* Выравнивание записей               */

/* Заголовок записи, входящей в серию */
typedef
struct hrlist {
    unsigned short type_record; /* тип записи    */
    unsigned short size_record; /* размер записи */
} trcHEADER_RECORD_LIST;

/* Заголовок блока трассировки */
#define tracePSEUDOBLOCK_NUMBER  0xFFFFFFFF
typedef
struct trace_block_header {
  trcHEADER_RECORD_LIST hdr;
  int nblock;
} traceBLOCK_HEADER, trcSERVICE_BLOCKHEAD;

#if OS_MAJOR >= 3
typedef
struct {
    int  itime[2];               /* число тиков       */
} trcTIME;
typedef
struct {
    int  itimeout[2];            /* число тиков       */
} trcTIMEOUT;
#else
typedef struct {
    unsigned int   ticks;        /* число тиков       */
    unsigned int   counter;      /* значение счетчика */
} trcTIME;
typedef
struct {
    unsigned int   ticks;        /* число тиков       */
} trcTIMEOUT;
#endif

/* Заголовок первой записи серии или одиночной */
typedef struct trcHeaderRecord {
    struct hrlist  header_list; /* тип и размер записи      */
    trcTIME        time_record; /* время записи             */
    unsigned int   procId;      /* идентификатор процесса   */
                                /* байт 0   id процесса
                                 * байт 1   вектор прерывания
                                 * байт 2   запрет планирования
                                 */
    unsigned int   threadId;    /* идентификатор потока     */
    unsigned int   adrevent;    /* адрес события            */
    unsigned int   coderet;     /* код завершения           */
} trcHEADER_RECORD;


/* Получить выровненный размер */
#define traceGET_RECORDSIZE(size) \
  (UADDR_ALIGN_UP( size, traceRECORD_ALIGN ))

/* Установить тип архитектуры целевой ЭВМ */
enum {
  cpuR3000 = 1,
  cpuI80386,
  cpuR4000
};
#if CPU == R3000
#  define traceSET_CPUTYPE( rec )                                    \
       (rec)->cputype = ( cpuR3000 )
#  define traceGET_CPUTYPE()  cpuR3000
#elif CPU == I80386
#  define traceSET_CPUTYPE( rec )                                    \
      (rec)->cputype = ( cpuI80386 )
#  define traceGET_CPUTYPE()  cpuI80386
#elif CPU == R4000
#  define traceSET_CPUTYPE( rec )                                    \
      (rec)->cputype = ( cpuR4000 )
#  define traceGET_CPUTYPE()  cpuR4000
#else
#error CPU type is not recognized!
#endif

/* Установить значение счетчика таймера */
#define traceSET_TIMER_FREQ( rec, tf )                               \
    (rec)->timer_freq = ( (unsigned int)(tf))
/* Установить число тиков в секунду таймера */
#define traceSET_TIMER_HZ( rec, hz )                                 \
    (rec)->timer_hz = ( (unsigned short)(hz))

/* Заполнение заголовка записи trcHEADER_RECORD */
/* Установить тип записи */
#define traceSET_RECORD_TYPE( hdr, type )                            \
    ((trcHEADER_RECORD_LIST*)hdr)->type_record = ((unsigned short)(type))
/* Установить размер записи */
#define traceSET_RECORD_SIZE( hdr, size )                            \
    ((trcHEADER_RECORD_LIST*)hdr)->size_record = (unsigned short)(size)
/* Установить тип и размер записи */
#define traceSET_TYPESIZE( hdr, type, size )                         \
   {                                                                 \
      traceSET_RECORD_TYPE( (hdr), type );                           \
      traceSET_RECORD_SIZE( (hdr), size );                           \
   }
/* Установить время записи */
#define traceSET_RECORD_TIME( hdr )   \
    traceGetTime(&(hdr)->time_record)
/* Установить идентификатор процесса */
#if OS_MAJOR >= 3
#define traceSET_IDPROC( hdr )                                       \
  {                                                                  \
    ((trcHEADER_RECORD*)hdr)->procId = curpid;                       \
  }
#else
#define traceSET_IDPROC( hdr )                                       \
  {                                                                  \
    ((trcHEADER_RECORD*)hdr)->procId = 0;                            \
  }
#endif
/* Установить идентификатор потока */
#define traceSET_IDTHREAD( hdr )   \
    ((trcHEADER_RECORD*)hdr)->threadId = \
    (threadExecuting==NULL)? 0 : (unsigned int)objTO_ID(threadExecuting)
/* Установить адрес события */
#define traceSET_ADREVENT( hdr, adr )   \
    ((trcHEADER_RECORD*)hdr)->adrevent = (unsigned int)(long)(adr)
/* Установить код завершения */
#define traceSET_CODERET( hdr, rt )   \
    ((trcHEADER_RECORD*)(hdr))->coderet = (unsigned int)(rt)
/* Установить все, помимо типа, размера и времени */
#define traceSET_REMAINDER(hdr,adr,coderet)   \
  if( intNest() ) {                           \
     ((trcHEADER_RECORD*)hdr)->procId = 0;    \
     ((trcHEADER_RECORD*)hdr)->threadId = 0;  \
  } else {                                    \
     traceSET_IDPROC( hdr );                  \
     traceSET_IDTHREAD( hdr );                \
  }                                           \
  traceSET_ADREVENT( hdr, adr );              \
  traceSET_CODERET( hdr, coderet )

/* Указать размер списка записей в trcBEGIN_RECORD */
#define  traceSET_LIST_SIZE(r,s) ((trcBEGIN_RECORD*)r)->size=s
/* Занести в запись short */
#define traceSET_RECORD_SHORT(rec,name,value) \
  (rec)->name=((short)value)
/* Занести в запись int */
#define traceSET_RECORD_INT(rec,name,value) \
  (rec)->name=((int)(long)value)
/* Занести в запись float */
#define traceSET_RECORD_FLOAT(rec,name,value) \
  (rec)->name=((float)value)
/* Занести в запись указатель */
#define traceSET_RECORD_POINTER(rec,name,pnt)   \
  (rec)->name=((trace_ptr)(long)pnt)
/* Занести в запись int64_t */
#define traceSET_RECORD_INT64(rec,name,value) \
  memmove( (void*)&(rec)->name, (void*)&value, 8 )
/* Занести в запись время в trace_timespec */
#define   traceSET_RECORD_TIMESPEC(rec,name,time) \
  if( time ) {                             \
    (rec)->name.sec = (time)->tv_sec;      \
    (rec)->name.nsec = (time)->tv_nsec;    \
  } else {                                 \
    (rec)->name.sec = 0;                   \
    (rec)->name.nsec = 0;                  \
  }
/* Занести в запись адрес */
#define traceSET_RECORD_ADR(p,adrv)   \
    (p)->adr = (void*)((int)(adrv))

/* Указать пустую запись - заполняет остаток буфера, может использоваться для
 * выравнивания последующей записи
 */
#define traceSET_RECORD_EMPTY(p,s)  \
    traceSET_TYPESIZE( (p), traceServiceEmpty,s)
/* Отметить запись как недействительную */
#define  traceSET_BADRECORD(hdr) \
    traceSET_RECORD_TYPE( (trcHEADER_RECORD*)hdr, traceServiceBad )

/* Прочитать тип записи */
#define  traceGET_RECORD_TYPE(r,type) \
    type = ((trcHEADER_RECORD_LIST*)(r))->type_record
/* Прочитать размер записи */
#define  traceGET_RECORD_SIZE(r,size) \
    size = ((trcHEADER_RECORD_LIST*)(r))->size_record
/* Прочитать время записи */
#define  traceGET_RECORD_TIME(r,time) \
    time = ((trcHEADER_RECORD*)(r))->time_record
/* Прочитать идентификатор процесса */
#define  traceGET_RECORD_IDPROC(r,id) \
    id = ((trcHEADER_RECORD*)(r))->procId
/* Прочитать идентификатор потока */
#define  traceGET_RECORD_IDTHREAD(r,id) \
    id = ((trcHEADER_RECORD*)(r))->threadId
/* Прочитать вектор прерывания */
#define  traceGET_RECORD_INTVECTOR(r,v) \
    v = 0/*((trcHEADER_RECORD*)(r))->threadId*/
/* Прочитать адрес события */
#define  traceGET_RECORD_ADREVENT(r,adr) \
    adr = (void*)((trcHEADER_RECORD*)(r))->adrevent
/* Прочитать размер списка записей в trcBEGIN_RECORD */
#define  traceGET_LIST_SIZE(r) ((trcBEGIN_RECORD*)(r))->size
/* Прочитать код возврата */
#define  traceGET_RECORD_RETURNCODE(r,rt) \
    rt = (int)((trcHEADER_RECORD*)(r))->coderet

/* Запись представления */
enum {
  boLITTLE_ENDIAN = 0,
  boBIG_ENDIAN
};
/* Флаги событий и состояний в записи представления */
#define traceFL_SBL_OVERFLOW     1  /* переполнение выделенных служебных блоков        */
#define traceFL_OVERFLOW         2  /* переполнение буфера                             */
#define traceFL_OVERRUN          4  /* перезапись блоков при циклическом заполнении    */

typedef
struct {
    trcHEADER_RECORD  hdr;
    int               byte_order;               /* порядок байт                        */
    int               streamfulpolicy;          /* режим записи в буфер                */
    int               logfulpolicy;             /* режим записи в файл                 */
    int               blksize;                  /* размер блока записи                 */
    int               streamsize;               /* размер буфера                       */
    int               logsize;                  /* размер файла                        */
    int               cputype;                  /* архитектура                         */
    unsigned int      timer_freq;               /* загружаемое в таймер значение       */
    unsigned short    timer_hz;                 /* число тиков в секунду таймера       */
    char              version[TRACE_NAME_MAX];  /* версия средств протоколирования     */
    char              name[TRACE_NAME_MAX];     /* имя протокола                       */
    struct  trace_timespec  createtime;         /* время создания протокола            */
    int               maxdatasize;              /* максим.размер данных пользователя   */
    int               cpu;                      /* номер модуля                        */
    int               serviceBlocks;            /* число блоков служебной информации
                                                  в начале LOG файла                   */
    char              date[28];                 /* дата и время создания               */
    char              modelname[8];             /* имя модуля                          */
    int               flags;                    /* флаги событий и состояний           */
    int               errpc;                    /* адрес команды, в момент выполнения
                                                   аппаратного RESET (заполняется при
                                                   выгрузке сохраненного буфера трассы */
} trcPRESENT_RECORD, trcSERVICE_PRESENT;

typedef
struct {
    trcHEADER_RECORD_LIST  hdr;
} trcSERVICE_EMPTY, trcSERVICE_BAD,
  trcSERVICE_ERROR,
  trcSERVICE_OVERFLOW_LOG, trcSERVICE_OVERRUN_LOG;

/* Именование объекта */
typedef struct {
    trcHEADER_RECORD hdr;
    int   id;           /* идентификатор объекта                              */
    int   adr;          /* адрес объекта                                      */
    int   type;         /* тип объекта (для переменной - способ интерпритации */
    char  name[TRACE_NAME_MAX];
    int   adrId;        /* адрес идентификатора объекта                       */
} trcNAME_RECORD, trcSERVICE_NAME;

/* Начало группы записей */
typedef struct {
    trcHEADER_RECORD hdr;
    int  size;  /*размер группы записей в буфере*/
} trcBEGIN_RECORD, trcSERVICE_BEGIN;

/* Начало протоколирования */
typedef struct {
    trcHEADER_RECORD    hdr;
    int                 traceMask[20];  /* маска событий*/
} trcSTART_RECORD, trcSERVICE_START;

/* Конец протоколирования */
typedef struct {
    trcHEADER_RECORD hdr;
} trcSTOP_RECORD, trcSERVICE_STOP;

/* Возобноление протоколирования при освобожд. буфера */
typedef struct {
    trcHEADER_RECORD hdr;
} trcRESUME_RECORD, trcSERVICE_RESUME;

/* Начало синхронизации */
typedef struct {
    trcHEADER_RECORD hdr;
} trcFLUSHSTART_RECORD, trcSERVICE_FLUSH_START;

/* Конец синхронизации */
typedef struct {
    trcHEADER_RECORD hdr;
} trcFLUSHSTOP_RECORD, trcSERVICE_FLUSH_STOP;

/* Переполнение буфера */
typedef struct {
    trcHEADER_RECORD hdr;
} trcOVERFLOW_RECORD, trcSERVICE_OVERFLOW;

/* Переполнение служебных блоков */
typedef struct {
    trcHEADER_RECORD hdr;
} trcSERVICE_BLOCK_OVERFLOW;

/* Перекрытие невыведенных записей */
typedef struct {
    trcHEADER_RECORD hdr;
} trcOVERRUN_RECORD, trcSERVICE_OVERRUN;

/* Описание окна расписания */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    short       idSchedule;
    short       idWnd;
    __uint32_t  duration[2];
    __uint32_t  procId;         /* исполняемый в окне процесс              */
} trcSCHEDULE_RECORD, trcSERVICE_SCHEDULE;

/* Конец протоколирования */
typedef struct {
    trcHEADER_RECORD hdr;
} trcSHUTDOWN_RECORD, trcSERVICE_SHUTDOWN;

/* Перезагрузка модуля */
typedef struct {
    trcHEADER_RECORD hdr;
} trcREBOOT_RECORD, trcSERVICE_REBOOT;

/* Запись завещания */
typedef struct {
   trcHEADER_RECORD_LIST hdrlst;
   int         event;   /* идентификатор события                           */
   int         nblock;  /* номер блока, в который помещена запись события,
                           породившего завещание                           */
   int         offset;  /* смещение в буфере событий записи, породившей
                           завещание                                       */
} trcSERVICE_TESTAMENT;

/* Атрибуты трассы */
typedef struct {
    trcHEADER_RECORD hdr;
    struct trace_attribute attr;
} trcATTR_RECORD, trcSERVICE_ATTR;

/* Статус трассы */
typedef struct {
    trcHEADER_RECORD hdr;
    struct posix_trace_status_info status;
} trcSTATUS_RECORD, trcSERVICE_STATUS;

/* Свойства очереди сообщений traceServiceMqp */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор очереди                              */
    trace_ptr   adrId;          /* адрес идентификатор очереди                        */
    trace_ptr   adr;            /* адрес описателя очереди                            */
    __uint16_t  procId;         /* принадлежит процессу                               */
    __uint16_t  maxmsg;
    int         msgsize;
    char        name[TRACE_NAME_MAX];
}  trcSERVICE_MQP;

/* Свойства семафоров traceServiceSem */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор доски обявлений ARINC                */
    trace_ptr   adrId;          /* адрес идентификатор доски обявлений ARINC          */
    trace_ptr   adr;            /* адрес описателя доски обявлений ARINC              */
    __uint16_t  procId;         /* принадлежит процессу                               */
    __uint16_t  discipline;     /* дисциплина ожидания                                */
    __uint16_t  count;          /* начальное значение счетчика                        */
    __uint16_t  countmax;       /* максимальное значение счетчика                     */
    char        name[TRACE_NAME_MAX];
} trcSERV_SEM, trcSERVICE_SEM;

/* Свойства мьютекса traceServiceMutex */
typedef struct {
    trcHEADER_RECORD hdr;
} trcSERV_MTX, trcSERVICE_MUTEX;

/* Свойства буфера traceServiceBuf */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор очереди сообщений ARINC              */
    trace_ptr   adrId;          /* адрес идентификатор очереди сообщений ARINC        */
    trace_ptr   adr;            /* адрес описателя очереди сообщений ARINC            */
    __uint16_t  procId;         /* принадлежит процессу                               */
    __uint16_t  maxmsg;
    __uint32_t  msgsize;
    char        name[TRACE_NAME_MAX];
} trcSERV_BUF, trcSERVICE_BUF;

/* Свойства доски объявлений traceServiceBbrd */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор доски обявлений ARINC                */
    trace_ptr   adrId;          /* адрес идентификатор доски обявлений ARINC          */
    trace_ptr   adr;            /* адрес описателя доски обявлений ARINC              */
    __uint32_t  procId;         /* принадлежит процессу                               */
    __uint32_t  msgsize;
    char        name[TRACE_NAME_MAX];
} trcSERV_BBRD, trcSERVICE_BBRD;

/* Свойства порта SAMPLING traceServiceSPort */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор порта                                */
    trace_ptr   adrId;          /* адрес идентификатора порта                         */
    trace_ptr   adr;            /* адрес описателя порта                              */
    __uint32_t  procId;         /* принадлежит процессу                               */
    __uint16_t  chanId;         /* идентификатор канала                               */
    __uint16_t  direct;         /* направление передачи данных                        */
    __uint32_t  msgsize;        /* максимальный размер сообщения                      */
    char        name[TRACE_NAME_MAX];
} trcSERV_SPORT, trcSERVICE_SPORT;

/* Свойства порта QUEUING traceServiceQPort */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор порта                                */
    trace_ptr   adrId;          /* адрес идентификатора порта                         */
    trace_ptr   adr;            /* адрес описателя порта                              */
    __uint16_t  procId;         /* принадлежит процессу                               */
    __uint16_t  chanId;         /* идентификатор канала                               */
    __uint16_t  discipline;     /* дисциплина ожидания                                */
    __uint16_t  direct;         /* направление передачи данных                        */
    __uint32_t  maxmsg;         /* максимальное число сообщений                       */
    __uint32_t  msgsize;        /* максимальный размер сообщения                      */
    char        name[TRACE_NAME_MAX];
} trcSERV_QPORT, trcSERVICE_QPORT;

/* Свойства порта SAP traceServiceQSap */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор порта                                */
    trace_ptr   adrId;          /* адрес идентификатора порта                         */
    trace_ptr   adr;            /* адрес описателя порта                              */
    __uint16_t  procId;         /* принадлежит процессу                               */
    __uint16_t  chanId;         /* идентификатор канала                               */
    __uint16_t  discipline;     /* дисциплина ожидания                                */
    __uint16_t  direct;         /* направление передачи данных                        */
    __uint32_t  maxmsg;         /* максимальное число сообщений                       */
    __uint32_t  msgsize;        /* максимальный размер сообщения                      */
    char        name[TRACE_NAME_MAX];
} trcSERV_QSAP, trcSERVICE_QSAP;

/* Свойства списка портов QLIST traceServiceQList */
typedef struct {
    trcHEADER_RECORD hdr;
    __uint32_t  id;             /* идентификатор списка портов                        */
    trace_ptr   adrId;          /* адрес идентификатора списка портов                 */
    trace_ptr   adr;            /* адрес описателя списка портов                      */
    __uint32_t  procId;         /* принадлежит процессу                               */
    __uint16_t  discipline;     /* дисциплина ожидания                                */
    __uint16_t  maxport;        /* максимальное число портов в списке                 */
    char        name[TRACE_NAME_MAX];
} trcSERV_QLIST, trcSERVICE_QLIST;

/* Изменении фильтра */
typedef struct {
    trcHEADER_RECORD hdr;
    struct traceFilter mask;
} trcFILTER_RECORD, trcSERVICE_FILTER;

/* Запись дополнения, которая может быть сформирована в функции
 * пользователя при регистрации события
 */
typedef struct {
   trcHEADER_RECORD hdr;
   int      event;   /* событие, породившее дополнение */
} trcSERVICE_ADD;

/* Выполнение функции (traceReturnGroup), возможно,
 * следует регистрировать и успешное завершение функции
 *???надо бы добавить вектор
 */
typedef struct {
   trcHEADER_RECORD_LIST hdr;
   int    event;     /* событие/функция*/
} trcFUNC_RETURN, trcFUNCTION_RETURN;

/* Внешнее событие */
typedef struct {
    trcHEADER_RECORD    hdr;
    __uint32_t          proc;
    trace_func_ptr      f;
    int                 next_pid;
    int                 disp_level;
    int                 proc_switch;
} trcPROCEVENT_RECORD, trcPROCESS_EVENT;

/* Старт модуля */
typedef struct {
    trcHEADER_RECORD hdr;
} trcMODSTART_RECORD, trcMOD_START_EVENT;

/* Инициализация процесса */
typedef struct {
    trcHEADER_RECORD hdr;
} trcPROCINIT_RECORD, trcPROCESS_INIT_EVENT;

/* Переключение окон */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    window;
} trcWINDOW_RECORD, trcWINDOW_SWITCH_EVENT;

/* Переключение процессов */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    proc;
} trcPROCESS_RECORD, trcPROCESS_SWITCH_EVENT;

/* выбор расписания */
typedef struct {
    trcHEADER_RECORD hdr;
    int    id;
} trcSETSCHEDULE_RECORD, trcSCHEDULE_EVENT;

/* Переключение потоков */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    heir;              /* поток получает управление              */
    short  priority;          /* приоритет thread(старший байт), heir   */
    short  schedpolicy;       /* политика планирования heir             */
    int    pid;               /* pid потока приемника                   */
#if  CPU == R3000 || CPU == R4000
    u_int  cp0_sr;            /* регистр состояния текущего потока      */
    u_int  cp0_srHeir;        /* регистр состояния потока наследника    */
#else
    u_int  eflags;            /* флаги текущего потока                  */
    u_int  eflagsHeir;        /* флаги потока наследника                */
#endif
} trcSWITCH_RECORD, trcTHREAD_SWITCH_EVENT;

/* Переход системы в состояние простоя */
typedef struct {
    trcHEADER_RECORD hdr;
} trcSYSTEM_IDLE, trcSYSTEM_IDLE_EVENT;
/* Выход из состояния простоя */
#define trcSYSTEM_ACTIVE trcSYSTEM_IDLE

/* Выполнение планировщика */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    dispatchNecessary;
} trcTHREAD_DISPATCH;

/* Изменение состояния потока управления (traceThreadStatusGroup) */
/* Переход потока в состояние блокирования */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int        state;         /* флаги состояния m_state                */
    int        cause_details; /* дополнительная информация              */
    trcTIMEOUT timeout;       /* максимальное время ожидания в тиках    */
} trcTHREAD_BLOCK, trcTHREAD_STATUS_BLOCK;
/* Уничтожение потока */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    code_exit;         /*его код завершения*/
} trcTHREAD_EXIT, trcTHREAD_STATUS_EXIT;
/* Переход потока в работоспособное состояние */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    thread;        /* активированный поток */
    int    cause;         /* код причины*/
    int    state;         /* флаги состояния m_state*/
} trcTHREAD_ACTIVATE, trcTHREAD_STATUS_ACTIVATE;
/* Отказ потока от использования процессора (без потери работоспособности) SchedYield() */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
} trcSCHED_YIELD, trcTHREAD_STATUS_SCHED_YIELD;
/* Отказ потока от использования процессора (без потери работоспособности - истек квант времени) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
} trcTIME_QUANTUM, trcTHREAD_STATUS_TIME_QUANTUM;
/* Вызов функций threadSuspend, threadResume */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    thread;
} trcTHREAD_SUSPEND, trcTHREAD_RESUME, trcTHREAD_STATUS_RESUME, trcTHREAD_STATUS_SUSPEND, trcASUSPEND, trcARESUME;
/* Вызов функций athreadSuspendSelf */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    __uint32_t  time[2];    /* время ожидания               */

} trcTHREAD_SUSPENDSELF, trcASUSPEND_SELF;
/* Вызов функций athreadStop, athreadStopSelf */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int    thread;
} trcTHREAD_STOP, trcTHREAD_STATUS_STOP, trcASTOP;
/* Вызов функций athreadStart */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int         thread;
    __uint32_t  time[2];
} trcTHREAD_START, trcTHREAD_STATUS_START, trcASTART;
/* Вызов функций athreadLockPreemption, athreadUnlockPreemption */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int       pLvl;
    int       lvl;
} trcTHREAD_PREEMPTION, trcTHREAD_STATUSLOCK, trcTHREAD_STATUS_LOCK, trcALOCK_PREEMPTION,
  trcTHREAD_STATUSUNLOCK, trcTHREAD_STATUS_UNLOCK, trcAUNLOCK_PREEMPTION;

/* Вызов функций athreadTimedWait */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    __uint32_t  time[2];    /* время ожидания               */
} trcTHREAD_TIMEDWAIT, trcTHREAD_STATUS_TIMEDWAIT, trcATIMEDWAIT;
/* Вызов функций traceThreadPeriodicwait */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
} trcTHREAD_PERIODICWAIT, trcTHREAD_STATUS_PERIODICWAIT, trcAPERIODICWAIT;

/* Вызов функций athreadReplenish */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    __uint32_t  time[2];
} trcTHREAD_REPLENISH, trcTHREAD_STATUS_REPLENISH, trc_AREPLENISH, trcAREPLENISH;
/* Вызов функции release poit */
typedef struct {
    trcHEADER_RECORD hdr;
    int       thread;
} trcRELEASEPOINT, trcTHREAD_STATUS_RELEASE_POINT, trcARELEASE_POINT;
/* Вызов функции athreadDeadlinePoint */
typedef struct {
    trcHEADER_RECORD hdr;
    int       thread;
} trcDEADLINEPOINT, trcTHREAD_STATUS_DEADLINE_POINT, trcADEADLINE_POINT;
/* Вызов функции assert() c ошибочным условием */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
} trcASSERT, trcTHREAD_STATUS_ASSERT;


/* Порождение потока */
/* Атрибуты потока */
struct trace_thread_attr {
    trace_ptr   entryPoint; /* адрес головной функции потока                     */
    int         arg;        /* аргумент головной функции потока (указатель)      */
    int         word_attr;  /* слово атрибутов порожденного потока в описателе   */
    int         mempool;    /* используемый по умолчанию пул памяти              */
    trace_ptr   kstackaddr; /* адрес стека системного                            */
    int         kstacksize; /* размер стека (в байтах)                           */
    trace_ptr   stackaddr;  /* адрес стека пользователя                          */
    int         stacksize;  /* размер стека (в байтах)                           */
    int         schedpolicy;/* политика планирования                             */
    int         priority;   /* приоритет                                         */
    int         sigBlocked; /* маска сигналов m_sigBlocked                       */
    char        name[32];   /* имя потока                                        */
};
/* Дополнительные атрибуты потока ARINC */
struct trace_thread_attr_arinc {
   int    period;        /* период выполнения для периодического потока          */
   int    time_capacity; /* ограничение на время вполнения потока                */
   int    deadline;      /* характер реакции на превышение времени выполнения    */
   int    delay;         /* задержка запуска                                     */
};
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr   pId;     /* указатель на идентификатор (аргумент функции)        */
    trace_ptr   pAttr;   /* указатель на описатель атрибутов (аргумент функции)  */
    int         id;      /* идентификатор созданного потока                      */
    struct trace_thread_attr
                attr;    /* атрибуты потока                                      */
    struct trace_thread_attr_arinc
                attrA;   /* дополнительные атрибуты ARINC                        */
} trcTHREAD_CREATE, trcTHREAD_STATUS_CREATE;
/* Свойства потока traceServiceThread */
typedef struct {
    trcHEADER_RECORD hdr;
    objid_t id;             /* идентификатор потока                               */
    struct trace_thread_attr
           attr;           /* атрибуты потока                                    */
    struct trace_thread_attr_arinc
           attrA;          /* дополнительные атрибуты ARINC                      */
    int    for_pid;        /* поток принадлежит процессу                         */
    uint32_t adr;            /* адрес описателя потока                             */
} trcSERV_THREAD, trcSERVICE_THREAD;

/* Вход в прерывание (вызов функции обработки прерываний) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  trace_ptr         epc;        /* адрес прерывания                              */
#if  CPU == R3000 || CPU == R4000
  u_int             cp0_sr;     /* регистр состояния текущего потока             */
#endif
#if  CPU == I80386
  u_int             eflags;
#endif
  trace_func_ptr    function;   /* адрес функции обработки прерывания            */
} trcINT_ENTRY, trcINTERRUPT_ENTRY;
/* Выход из прерывания (из функции обработки прерываний) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int               level;      /* уровень вложенности прерываний (до выхода)    */
  int               vector;     /* вектор прерывания, к обработке которого
                                                                 возвращаемся    */
  trace_ptr         epc;        /* адрес прерывания                              */
#if  CPU == R3000 || CPU == R4000
  u_int             cp0_sr;     /* регистр состояния текущего потока             */
#endif
#if  CPU == I80386
  u_int             eflags;
#endif
} trcINT_EXIT, trcINTERRUPT_EXIT;

/* Исключение */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    code;                      /* код исключительной ситуации
                                       (номер соответствующего сигнала)          */
  int    type;                      /* вид обработки исключительной ситуации     */
  int    level;                     /* уровень вложенности прерываний            */
  char   regs[TRACE_SIZEREGS];      /* регистры процессора                       */
} trcEXCEPTION, trcEXCEPTION_CASE;


/* функции работы с семафорами traceSemaphoreGroup*/
/* Вызов функции sem_post */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       counter;    /* счетчик семафора после выполнения функции */
} trcSEM_POST, trcDELAYED_SEM_POST, trcDELAYED_MP_SEM_POST;
/* Вызов функции sem_wait */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       counter;    /* счетчик семафора после выполнения функции */
} trcSEM_WAIT, trcSEM_TRYWAIT,
  trcMP_SEM_WAIT, trcMP_SEM_TRYWAIT;
/* Вызов функции sem_timedwait */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       counter;    /* счетчик семафора после выполнения функции */
  struct trace_timespec timeout;  /* максимальное время ожидания           */
} trcSEM_TIMEDWAIT, trcMP_SEM_TIMEDWAIT;
/* Вызов функции sem_getvalue */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       counter;    /* счетчик семафора после выполнения функции */
  int       threads;    /* число потоков, ждущих семафор             */
} trcSEM_GETVALUE, trcMP_SEM_GETVALUE;
/* Вызов функции sem_unlink */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int   attach;         /* флаги процессов, работающих с семафором   */
  char  name[TRACE_NAMESIZE]; /* имя семафора                        */
} trcSEM_UNLINK, trcMP_SEM_UNLINK;
/* Вызов функции sem_close */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       attach;     /* флаги процессов, работающих с семафором   */
  int       unlink;     /* признак, указывающий была ли выполнена
                           функция sem_unlink()                      */
} trcSEM_CLOSE, trcMP_SEM_CLOSE;
/* Вызов функции sem_open */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       attach;     /* флаги процессов, работающих с семафором   */
  unsigned  oflag;      /* флаги открытия                            */
  unsigned  mode;       /* режим                                     */
  unsigned  value;      /* начальное значение счетчика               */
  char      name[TRACE_NAMESIZE]; /*имя семафора                     */
} trcSEM_OPEN, trcMP_SEM_OPEN;
/* Вызов функции sem_destroy */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
} trcSEM_DESTROY, trcMP_SEM_DESTROY;
/* Вызов функции sem_init */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       pshared;    /* семафор разделяемый процессами            */
  unsigned  value;      /* начальное значение счетчика               */
} trcSEM_INIT, trcMP_SEM_INIT;

/* функции работы с мьтексами traceMutexGroup*/
/* Вызов функции pthread_mutex_init */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  unsigned  attr;
  short     priority; /*приоритет*/
  short     protocol; /*протокол*/
  int       pshared;
} trcMUTEX_INIT;
/* Вызов функции pthread_mutex_init */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
  int       cond;       /* используется переменной условия           */
} trcMUTEX_DESTROY;
/* Вызов функции pthread_mutex_lock */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
} trcMUTEX_LOCK;
/* Вызов функции pthread_mutex_trylock */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
} trcMUTEX_TRYLOCK, trcMUTEX_TRY_LOCK;
/* Вызов функции pthread_mutex_timedlock */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
  struct trace_timespec timeout;
} trcMUTEX_TIMEDLOCK;
/* Вызов функции pthread_mutex_unlock */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
} trcMUTEX_UNLOCK;
/* Вызов функции pthread_mutex_setprioceiling */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /*владелец мьютекса до выполнения функции*/
  int       prioceiling;
} trcMUTEX_SETPRIOCEILING;
/* Вызов функции pthread_mutex_getprioceiling */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMtx;     /* адрес описателя мьютекса                  */
  unsigned  mtxId;      /* идентификатор мьютекса                    */
  int       owner;      /* владелец мьютекса до выполнения функции   */
  int prioceiling;
} trcMUTEX_GETPRIOCEILING;

/*функции работы с условными переменными traceCondGroup*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrCnd;
  unsigned  cndId;
  int       pshared;
} trcCOND_INIT;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrCnd;
  unsigned  cndId;
  int       mutex;      /* указатель на описатель мьютекса*/
} trcCOND_DESTROY;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrCnd;
  unsigned  cndId;
  int mutex;      /* указатель на описатель мьютекса*/
  struct trace_timespec timeout; /* максимальное время ожидания*/
} trcCOND_WAIT,
  trcCOND_TIMEDWAIT;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrCnd;
  unsigned  cndId;
} trcCOND_SIGNAL,
  trcCOND_BROADCAST;

/* функции работы с очерядями сообщений traceMqGroup*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMq;
  unsigned  mqId;
  unsigned  attr;
  int       oflag;
  int       msg_nbuf;             /*число буферов*/
  int       msg_len;              /*длина сообщения*/
  char      name[TRACE_NAMESIZE]; /*имя очереди*/
} trcMQ_OPEN, trcMP_MQ_OPEN;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMq;
  unsigned  mqId;
  int       attach;   /* число открытий этой очереди (mq_open()-mq_close)*/
  int       unlink;   /* признак, указывающий была ли выполнена функция mq_unlink().*/
} trcMQ_CLOSE, trcMP_MQ_CLOSE;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int   attach;   /* число открытий этой очереди (mq_open()-mq_close)*/
  char  name[TRACE_NAMESIZE];
} trcMQ_UNLINK, trcMP_MQ_UNLINK;
  /* mq_send(), mq_timedsend() */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMq;
  unsigned  mqId;
  int   msg_len;    /* длина сообщения                */
  int   msg_prio;   /* приоритет сообщения            */
  int   msg_ptr;    /* указатель на текст сообщения   */
  int   msg_cur;    /* сообщений в очереди            */
  struct trace_timespec timeout;
} trcMQ_SEND, trcMQ_TIMEDSEND,
  trcMP_MQ_SEND, trcMP_MQ_TIMEDSEND;
  /*mq_receive(), mq_timedreceive()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrMq;
  unsigned  mqId;
  int       msg_len;    /* длина сообщения                */
  int       msg_prio;   /* приоритет сообщения            */
  int       msg_ptr;    /* указатель на текст сообщения   */
  int       msg_cur;    /* сообщений в очереди            */
  struct trace_timespec
            timeout;
  int       bufsize;    /* размер приемного буфера        */
} trcMQ_RECEIVE, trcMQ_TIMEDRECEIVE,
  trcMP_MQ_RECEIVE, trcMP_MQ_TIMEDRECEIVE;
   /* mq_notify()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned          adrMq;
  unsigned          mqId;
  int               notify;     /* способ уведомления
                                   (NONE, SIGNAL, SIGTHR, THREAD, FUNCTION) */
  int               sig_num;    /* номер (и имя) сигнала                    */
  int               sig_val;    /* значения сигнала                         */
  int               sig_thread; /* поток, которому был послан сигнал        */
  trace_func_ptr    func;       /* функция уведомления (если используется
                                   способ уведомления FUNCTION)             */
} trcMQ_NOTIFY, trcMP_MQ_NOTIFY;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned          mqId;
  int               maxmsg;
  int               msgsize;
  int               flags;
  int               curmsg;
} trcMQ_SETATTR, trcMP_MQ_SETATTR;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned          mqId;
  int               maxmsg;
  int               msgsize;
  int               flags;
  int               curmsg;
} trcMQ_GETATTR, trcMP_MQ_GETATTR;
/* Распределение памяти traceMemoryGroup */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int       poolID;         /* идентификатор созданного пула памяти     */
  int       poolAddr;       /* адрес буфера                             */
  int       poolSize;       /* длина буфера                             */
  int       flags;
} trcPOOL_CREATE, trcMEM_POOL_CREATE,
  trcMPMEM_POOL_CREATE,trcMP_MEM_POOL_CREATE,
  trcPOOL_ADD, trcMEM_POOL_ADD,
  trcMPMEM_POOL_ADD, trcMP_MEM_POOL_ADD;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int       poolID;    /* идентификатор пула памяти */
} trcPOOL_DELETE, trcMEM_POOL_DELETE;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    poolID;    /* идентификатор пула памяти                */
  int    memAddr;   /* указатель на полученный участок памяти   */
  int    memSize;   /* длина участка                            */
  int    memAlign;  /* выравнивание                             */
} trcMEM_ALLOC, trcMEM_POOL_ALLOC,
  trcMPMEM_POOL_ALLOC, trcMP_MEM_POOL_ALLOC;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    poolID;    /* идентификатор пула памяти                */
  int    memAddr;   /* указатель на освобождаемый участок памяти*/
} trcMEM_FREE, trcMEM_POOL_FREE,
  trcMPMEM_POOL_FREE, trcMP_MEM_POOL_FREE;

/* Функции работы с часами и таймерами traceTimer*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int clock_id;
  int timer_id;     /* идентификатор таймера                    */
  int notify;       /* способ уведомления
                       (NONE, SIGNAL, SIGTHR, THREAD, FUNCTION) */
  int sig_num;      /* номер (и имя) сигнала                    */
  int sig_val;      /* значения сигнала                         */
  int sig_thread;   /* поток, которому будет послан сигнал      */
  int func;         /* функция уведомления                      */
} trcTIMER_CREATE;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int timer_id;     /* идентификатор таймера*/
  int proc;        /* процесс, которому он принадлежит */
} trcTIMER_EXPIRE;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int timer_id;     /* идентификатор таймера*/
} trcTIMER_DELETE;

typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int timer_id;     /* идентификатор таймера*/
  int flags;        /* флаги*/
  struct trace_timespec it_interval; /*период срабатывания таймера*/
  struct trace_timespec it_value;    /* время срабатывания таймера*/
} trcTIMER_SETTIME;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int timer_id;     /* идентификатор таймера*/
  int counter;      /* число избыточных срабатываний таймера*/
} trcTIMER_GETOVERRUN;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int       clock_id;   /* идентификатор часов*/
  struct trace_timespec time; /*установленное время в секундах и наносекундах*/
} trcCLOCK_SETTIME, trcTIMER_CLOCK_SETTIME;
/*приостановка на время*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int time;             /* максимальное время ожидания в секундах */
  int remaining;        /* осталось*/
} trcSLEEP, trcTIMER_SLEEP;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int       clock_id;   /* идентификатор часов*/
  int       flags;
  struct trace_timespec timeout;    /* максимальное время ожидания в секундах и наносекундах*/
  struct trace_timespec remaining;  /* осталось*/
} trcCLOCK_NANOSLEEP, trcTIMER_NANOSLEEP;

/* Порождение и доставка сигналов traceSignalGroup */
typedef struct {  /*вызов функции обработки сигнала*/
  trcHEADER_RECORD_LIST hdr;
  int    signo;   /* номер (и имя) сигнала*/
  int    value;   /* значение сигнала (и имя)*/
  int    code;    /* код причины порождения сигнала*/
  int    handler; /* адрес или имя функции*/
} trcSIG_CATCHFUNC, trcSIG_CATCH_FUNC;
typedef struct {/*возврат управления из функции обр.сигн.*/
  trcHEADER_RECORD_LIST hdr;
  int    signo;   /* номер (и имя) сигнала*/
} trcSIG_CATCHFUNCRET, trcSIG_CATCH_FUNC_RET;
/* доставка сигнала */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;  /* идентификатор потока, которому был доставлен сигнал*/
  int    signo;   /* номер (и имя) сигнала*/
  int    value;   /* значение сигнала (и имя)*/
  int    code;    /* код причины порождения сигнала*/
} trcSIG_DELIVERY;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    signo;     /* номер (и имя) сигнала*/
  int    value;     /* значение сигнала (и имя)?????*/
  int    code;      /* код причины порождения сигнала*/
  int    recipient; /* идентификатор потока, которому посылается сигнал*/
} trcSIG_GENERATION;

typedef struct {  /*запрос на удаление потока*/
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока, получившего запрос на удаление*/
} trcSIG_THREAD_CANCEL;

/* функции ожидания сигналов traceSigWaitGroup*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned int  mask;       /* маска сигналов*/
  struct trace_timespec timeout;  /* максимальное время ожидания*/
} trcSIGWAIT,
  trcSIGTIMEDWAIT, trcSIGWAITINFO;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned int  mask; /*маска сигналов*/
} trcSIGSUSPENDED, trcSIGSUSPEND;

/* функции управления потоками  traceThreadGroup*/
typedef struct {    /*pthread_cancel*/
  trcHEADER_RECORD_LIST hdr;
  int    threadCancel; /*удаляемый поток*/
} trcTHREAD_CANCEL, trcPTHREAD_CANCEL;
typedef struct {    /*pthread_join*/
  trcHEADER_RECORD_LIST hdr;
  int    threadJoin; /*ожидаемый поток*/
} trcTHREAD_JOIN, trcPTHREAD_JOIN;
typedef struct {    /*pthread_sigmask*/
  trcHEADER_RECORD_LIST hdr;
  unsigned  mask; /*новая маска*/
} trcTHREAD_SIGMASK, trcPTHREAD_SIGMASK;
typedef struct {    /*pthread_setschedparam*/
  trcHEADER_RECORD_LIST hdr;
  int    threadSched;    /*поток меняющий пар-ры планирования*/
  short  priority;      /*приоритет*/
  short  schedpolicy;   /*политика планирования*/
} trcTHREAD_SETSCHEDPARAM, trcPTHREAD_SETSCHEDPARAM;
typedef struct {    /*pthread_setschedprio*/
  trcHEADER_RECORD_LIST hdr;
  int    threadSched;    /*поток меняющий приоритет*/
  int    priority;      /*приоритет*/
} trcTHREAD_SETSCHEDPRIO, trcPTHREAD_SETSCHEDPRIO;
typedef struct {    /*pthread_setcancelstate */
  trcHEADER_RECORD_LIST hdr;
  int    state;         /**/
} trcTHREAD_SETCANCELSTATE, trcPTHREAD_SETCANCELSTATE;
typedef struct {    /*pthread_setcanceltype */
  trcHEADER_RECORD_LIST hdr;
  int    type;         /**/
} trcTHREAD_SETCANCELTYPE, trcPTHREAD_SETCANCELTYPE;
typedef struct {    /*pthread_detach*/
  trcHEADER_RECORD_LIST hdr;
  int    threadDetach;  /*поток, у которого изменяется статус*/
} trcTHREAD_DETACH, trcPTHREAD_DETACH;

/* ввод/вывод */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
} trcIO_CLOSE;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    fildes2;   /* дескриптор файла, используемый для записи / дескриптор-дубликат*/
} trcIO_DUP;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    fildes2;   /* дескриптор файла, используемый для записи / дескриптор-дубликат*/
} trcIO_DUP2;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    fildes2;   /* дескриптор файла, используемый для записи / дескриптор-дубликат*/
} trcIO_PIPE;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    mode;      /* режим доступа к файлу (типа mode_t)*/
  char   path[NAME_MAX];    /* */
} trcIO_MKFIFO;     /* mkfifo()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    nbyte;     /* количество прочтенных байтов*/
  int    offset;    /* смещение в файле (для регулярных файлов)*/
  int    buf;       /* адрес буфера*/
} trcIO_RW, trcIO_READ, trcIO_WRITE;         /* read() и write()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    whence;    /* вид позиционирования (SET, CUR, END)*/
  int    offset;    /* смещение в файле (для регулярных файлов)*/
} trcIO_LSEEK;
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
} trcIO_FSYNC, trcIO_FDATASYNC;         /* fdatasync*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    fildes;    /* дескриптор файла*/
  int    mode;      /* режим доступа к файлу (типа mode_t)*/
  int    oflag;     /* флаги*/
  char   path[NAME_MAX];    /* */
} trcIO_OPEN;       /* creat() и open()*/

/* Вызов функции athreadSetPriority (SET_PRIORITY) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;        /* поток меняющий приоритет                  */
  int    priority;      /* устанавливаемый приоритет                 */
} trcAPROC_SETPRIO, trcASETPRIO;

/* Вызов функции partSetMode (SET_PARTITION_MODE) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    mode;          /* устанавливаемый режим                     */
} trcAPROC_SETMODE, trcSET_MODE_EVENT;

/* Вызов функции asemCreate (CREATE_SEMAPHORE) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int      value;
  int      maxvalue;
  int       discip;
  char      name[TRACE_NAMESIZE];
} trcASEM_CREATE;
/* Вызов функции asemSignal (SIGNAL_SEMAPHORE) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrSem;     /* адрес описателя семафора                  */
  unsigned  semId;      /* идентификатор семафора                    */
  int       counter;    /*счетчик семафора после выполнения функции  */
} trcASEM_POST, trcASEM_SIGNAL;
/* Вызов функции asemWait (WAIT_SEMAPHORE) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned      adrSem;     /* адрес описателя семафора                  */
  unsigned      semId;      /* идентификатор семафора                    */
  int           counter;    /*счетчик семафора после выполнения функции  */
  __uint32_t    timeout[2]; /* время ожидания                   */
} trcASEM_WAIT, trcA_SEM_WAIT;

/* Вызов функции bufCreate (CREATE_BUFFER) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrBuf;     /* адрес описателя буфера           */
  unsigned  bufId;      /* идентификатор буфера             */
  int      msgsize;    /* максимальный размер сообщения    */
  int      maxmsg;     /* максимальное число сообщений     */
  int      discip;     /* дисциплина обслуживания          */
  char      name[TRACE_NAMESIZE];
} trcABUF_CREATE, trcBUF_CREATE;
/* Вызов функции bufSend (SEND_BUFFER) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrBuf;     /* адрес описателя буфера           */
    unsigned    bufId;      /* идентификатор буфера             */
    unsigned    msg;        /* адрес сообщения                  */
    int        len;        /* длина сообщения                  */
    __uint32_t  timeout[2]; /* время ожидания                   */
    int      curmsg;       /* текущее число сообщений          */
} trcABUF_SEND, trcBUF_SEND;
/* Вызов функции bufReceive (RECEIVE_BUFFER) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrBuf;     /* адрес описателя буфера           */
    unsigned    bufId;      /* идентификатор буфера             */
    unsigned    msg;        /* адрес приемного буфера           */
    int        len;        /* длина сообщения                  */
    __uint32_t  timeout[2]; /* время ожидания                   */
    int        curmsg;     /* текущее число сообщений          */
} trcABUF_RECEIVE, trcBUF_RECEIVE;
/* Перенос сообщения из буфера ждущего потока в свободный буфер */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr   adrBuf;     /* адрес описателя буфера           */
    __uint32_t  bufId;      /* идентификатор буфера             */
    __uint32_t  threadId;   /* идентификатор потока источника   */
    trace_ptr   threadBuf;  /* адрес сообщения                  */
    __uint32_t  len;        /* длина сообщения                  */
    trace_ptr   buf;        /* адрес свободного буфера          */
} trcBUF_FROM_THREAD;
/* Перенос сообщения из буфера ждущего потока в свободный буфер */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr   adrBuf;     /* адрес описателя буфера           */
    __uint32_t  bufId;      /* идентификатор буфера             */
    __uint32_t  threadId;   /* идентификатор потока источника   */
    trace_ptr   threadBuf;  /* адрес сообщения                  */
    __uint32_t  len;        /* длина сообщения                  */
    trace_ptr   msg;        /* адрес сообщения                  */
} trcBUF_SEND_DIRECT;
/* Перенос сообщения из буфера ждущего потока в буфер потока приемника */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr   adrBuf;     /* адрес описателя буфера           */
    __uint32_t  bufId;      /* идентификатор буфера             */
    __uint32_t  threadId;   /* идентификатор потока источника   */
    trace_ptr   threadBuf;  /* адрес сообщения                  */
    __uint32_t  len;        /* длина сообщения                  */
    trace_ptr   msg;        /* адрес приемного буфера           */
} trcBUF_RECEIVE_DIRECT;
/* Освобождение буфера потоком, возможен перенос сообщения из буфера
 *  ждущего потока в свободный буфер, который может быть отдан
 *  потоку приемнику
 */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr   adrBuf;     /* адрес описателя буфера           */
    __uint32_t  bufId;      /* идентификатор буфера             */
    __uint32_t  threadId;   /* идентификатор потока источника   */
    __uint32_t  threadId2;  /* идентификатор потока приемника   */
    trace_ptr   buf;        /* адрес свободного буфера          */
} trcBUF_RELEASE_BUFFER;

/* Вызов функции evnCreate (CREATE_EVENT) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrEvn;     /* адрес описателя события          */
  unsigned  evnId;      /* идентификатор события            */
  char      name[TRACE_NAMESIZE];   /* имя события          */
} trcAEVN_CREATE, trcEVENT_CREATE;
/* Вызов функции evnSet/evnReset (SET_EVENT/RESET_EVENT) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
  unsigned  adrEvn;     /* адрес описателя события          */
  unsigned  evnId;      /* идентификатор события            */
} trcAEVN_SET, trcAEVN_RESET, trcEVENT_SET, trcEVENT_RESET;
/* Вызов функции evnWait (WAIT_EVENT) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
  unsigned      adrEvn;     /* адрес описателя события          */
  unsigned      evnId;      /* идентификатор события            */
  __uint32_t    timeout[2]; /* время ожидания                   */
} trcAEVN_WAIT, trcEVENT_WAIT;

/* Вызов функции bbrdCreate (CREATE_BLACKBOARD) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrBbrd;    /* адрес описателя доски объявлений */
  unsigned  bbrdId;     /* идентификатор доски объявлений   */
  int      len;        /* максимальная длина объявления    */
  char      name[TRACE_NAMESIZE];   /* имя доски объявлений */
} trcABBRD_CREATE, trcBBRD_CREATE;
/* Вызов функции bbrdDisplay (DISPLAY_BLACKBOARD) */
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  unsigned  adrBbrd;    /* адрес описателя доски объявлений */
  unsigned  bbrdId;     /* идентификатор доски объявлений   */
  unsigned  msg;        /* адрес объявления                 */
  unsigned  len;        /* длина объявления                 */
} trcABBRD_DISPLAY, trcBBRD_DISPLAY;
/* Вызов функции bbrdRead (READ_BLACKBOARD) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned      adrBbrd;          /* адрес описателя доски объявлений */
    unsigned      bbrdId;           /* идентификатор доски объявлений   */
    unsigned      msg;              /* адрес приемного буфера           */
    unsigned      len;              /* длина сообщения                  */
    __uint32_t    timeout[2];   /* время ожидания                   */
} trcABBRD_READ, trcBBRD_READ;
/* Вызов функции bbrdClear (CLEAR_BLACKBOARD) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrBbrd;  /* адрес описателя доски объявлений */
    unsigned  bbrdId;   /* идентификатор доски объявлений   */
} trcABBRD_CLEAR, trcBBRD_CLEAR;

/* Вызов функции qportCreate (CREATE_QUEUING_PORT) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта                     */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  chanId;   /* идентификатор канала                      */
    int      msgsize;  /* максимальный размер сообщения             */
    int      maxmsg;   /* максимальное число сообщений              */
    int       direct;   /* направление передачи данных               */
    int       discip;   /* дисциплина обслуживания очереди потоков   */
    char      name[TRACE_NAMESIZE]; /* имя порта                     */
} trcQPORT_CREATE, trcSAPPORT_CREATE;
/* Вызов функции qportSend (SEND_QUEUING_MESSAGE) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned      adrPort;      /* адрес описателя порта               */
    unsigned      portId;       /* идентификатор порта                 */
    unsigned      chanId;       /* идентификатор канала                */
    unsigned      msg;          /* адрес сообщения                     */
    unsigned      len;          /* длина сообщения                     */
    __uint32_t    timeout[2];   /* время ожидания                      */
} trcQPORT_SEND;
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrPort;        /* адрес описателя порта               */
    unsigned    portId;         /* идентификатор порта                 */
    unsigned    chanId;         /* идентификатор канала                */
    unsigned    msg;            /* адрес сообщения                     */
    unsigned    len;            /* длина сообщения                     */
    __uint32_t  timeout[2];     /* время ожидания                      */
    unsigned    UDP_Port_Number;/* адрес сообщения                     */
    unsigned    IP_Address;     /* dst IP address                      */
} trcSAPPORT_SEND;
/* Вызов функции qportReceive (RECEIVE_QUEUING_MESSAGE) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrPort;    /* адрес описателя порта               */
    unsigned    portId;     /* идентификатор порта                 */
    unsigned    chanId;     /* идентификатор канала                */
    unsigned    buf;        /* адрес приемного буфера              */
    unsigned    len;        /* длина сообщения                     */
    __uint32_t  timeout[2]; /* время ожидания                      */
} trcQPORT_RECEIVE;
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrPort;        /* адрес описателя порта               */
    unsigned    portId;         /* идентификатор порта                 */
    unsigned    chanId;         /* идентификатор канала                */
    unsigned    buf;            /* адрес приемного буфера              */
    unsigned    len;            /* длина сообщения                     */
    __uint32_t  timeout[2];     /* время ожидания                      */
    unsigned    UDP_Port_Number;/* адрес сообщения                     */
    unsigned    IP_Address;     /* dst IP address                      */
} trcSAPPORT_RECEIVE;
/* Вызов функции qportClear (CLEAR_QUEUING_PORT) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  nMsg;     /* число удаленных сообщений           */
} trcQPORT_CLEAR;
/* Вызов функции sportCreate (CREATE_SAMPLING_PORT) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrPort;  /* адрес описателя порта               */
    unsigned    portId;   /* идентификатор порта                 */
    unsigned    chanId;   /* идентификатор канала                */
    int        msgsize;  /* максимальный размер сообщения       */
    int         direct;   /* направление передачи данных         */
    __uint32_t  period[2];/* период годности                     */
    char        name[TRACE_NAMESIZE]; /* имя порта               */
} trcSPORT_CREATE;
/* Вызов функции sportWrite (WRITE_SAMPLING_MESSAGE) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  msg;      /* адрес буфера                        */
    int      len;      /* длина сообщения                     */
} trcSPORT_WRITE;
/* Вызов функции sportRead (READ_SAMPLING_MESSAGE) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  msg;      /* адрес приемного буфера              */
    int       len;      /* длина сообщения                     */
    int       valid;    /* сообщение актуально                 */
} trcSPORT_READ;
/* Взять буфер из канала */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  buf;      /* адрес описателя сообщения (узла)    */
    unsigned  curmsg;
} trcCHAN_GETBUFFER, trcCHAN_GET_BUFFER;
/* Положить буфер/сообщение в канал */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала                   */
    unsigned  buf;      /* адрес описателя сообщения (узла)       */
    unsigned  notify;   /* есть требование на уведомление         */
    unsigned  flnotify; /* уведомление партнера уже инициировано  */
    unsigned  curmsg;
} trcCHAN_PUTBUFFER, trcCHAN_PUTMESSAGE, trcCHAN_PUT_BUFFER, trcCHAN_PUT_MESSAGE;
/* Уведомление о появлении буфера/сообщения */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала                   */
    unsigned  buf;      /* адрес описателя сообщения (узла)       */
    unsigned  flnotify; /* уведомление партнера уже инициировано  */
} trcCHAN_NOTBUFFER, trcCHAN_NOTMESSAGE, trcCHAN_NOTIFY_BUFFER, trcCHAN_NOTIFY_MESSAGE;
/* Чтение сообщения из канала */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала             */
    unsigned  buf;      /* адрес описателя сообщения (узла) */
    unsigned  curmsg;
} trcCHAN_GETMESSAGE, trcCHAN_GET_MESSAGE;
/* Посылка сообщения модулю партнеру */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор/индекс канала      */
    unsigned  buf;      /* адрес описателя сообщения (узла) */
    short int cpu;      /* модуль получатель                */
    short int link;     /* номер логической связи           */


    short int type;     /* тип сообщения                    */
    short int state;    /* состояние соединения             */
    short     nBufs;
    short     line;
    unsigned  nMsg;     /* # сообщения                      */
    unsigned  size;     /* размер сообщения                 */
} trcCHAN_MPSEND, trcCHAN_MP_SEND_INIT, trcCHAN_MP_SEND;
/* Посылка сообщения модулю партнеру */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала             */
    unsigned  buf;      /* адрес описателя сообщения (узла) */
    unsigned  port;
    unsigned  addr;
    unsigned  nMsg;     /* # сообщения                      */
    unsigned  size;     /* размер сообщения                 */
} trcCHAN_NETSEND, trcCHAN_NET_SEND;
/* Получение сообщения от модуля партнера */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала                            */
    unsigned  buf;      /* адрес описателя сообщения (узла)                */
    short int cpu;      /* модуль источник                                 */
    short int link;     /* номер логической связи                          */
    unsigned  pHdr;     /* адрес заголовка сообщения                       */
    unsigned  nMsg;     /* # сообщения                                     */
    unsigned  type;     /* тип сообщения: регистрация/число буферов/данные */
    unsigned  size;     /* размер сообщения                                */
    unsigned  nBuf;
} trcCHAN_MPRECEIVE, trcCHAN_MP_RECEIVE;
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала                            */
    unsigned  nBuf;
    unsigned  curmsgs;
    unsigned  nDrvBuf;
    unsigned  flnotify;
} trcCHAN_MPBUFS, trcCHAN_MP_BUFS;
/* Посылка числа доступных буферов */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала */
    short int cpu;      /* модуль партнер */
    short int link;     /* номер логической связи */
    unsigned  nBuf;     /* число имеющихся буферов */
    unsigned  notify;   /* требование от партнера на уведомление */
} trcCHAN_MPSENDBUF, trcCHAN_MP_SEND_BUF;
/* Получение извещения о результате отсылки сообщения модулю партнеру */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  chanId;   /* идентификатор канала             */
    unsigned  buf;      /* адрес описателя сообщения (узла) */
    short int cpu;      /* модуль получатель                */
    short int link;     /* номер логической связи           */
} trcCHAN_MPSENDEND, trcCHAN_MP_SEND_END;

/* Посылка требования спец.потоку */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    __uint32_t  cpu;      /* cpu                                 */
    __uint32_t  state;    /* состояние                           */
    __uint32_t  request;  /* требование                          */
} trcCHAN_MPCHANREQ, trcCHAN_THREAD_REQUEST;

/* Посылка требования модулю, получение требования от модуля */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    __uint32_t  cpu;      /* cpu                                 */
    __uint32_t  state;    /* состояние                           */
    __uint32_t  request;  /* требование                          */
} trcCHAN_MPREQUEST;

/* Захват сообщения из канала */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned    adrPort;  /* адрес описателя порта               */
    unsigned    portId;   /* идентификатор порта                 */
    unsigned    chanId;   /* идентификатор канала                */
    unsigned    buf;      /* адрес приемного буфера              */
    unsigned    len;      /* длина сообщения                     */
    __uint32_t  timeout[2];/*                                     */
} trcQPORT_MSGLOCK, trcQPORT_MSG_LOCK;
/* Освобождение захваченного сообщения */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  msg;      /* адрес сообщения                     */
    unsigned  node;     /* адрес узла                          */
} trcQPORT_MSGUNLOCK, trcQPORT_MSG_UNLOCK;
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  buf;      /* адрес приемного буфера              */
    unsigned  len;      /* длина сообщения                     */
    int       valid;    /* сообщение актуально                 */
} trcSPORT_MSGLOCK, trcSPORT_MSG_LOCK;
/* Освобождение захваченного сообщения */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrPort;  /* адрес описателя порта               */
    unsigned  portId;   /* идентификатор порта                 */
    unsigned  chanId;   /* идентификатор канала                */
    unsigned  msg;      /* адрес сообщения                     */
    unsigned  node;     /* адрес узла                          */
} trcSPORT_MSGUNLOCK, trcSPORT_MSG_UNLOCK;

/* Вызов функции QportListCreate (CREATE_QUEUING_PORT_LIST) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  listsize; /* размер списка                             */
    int       discip;   /* дисциплина обслуживания очереди потоков   */
    char      name[TRACE_NAMESIZE]; /* имя порта                     */
} trcQLIST_CREATE;

/* Вызов функции QportListAdd (ADD_PORT_TO_QUEUING_PORT_LIST) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  pBuf;     /* адрес буфера                              */
    unsigned  action;   /* активность порта                          */
} trcQLIST_ADD;

/* Вызов функции QportListAction (SET_PORT_ACTION_IN_QUEUING_PORT_LIST) */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  action;   /* активность порта                          */
} trcQLIST_ACTION;

typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  action;   /* активность порта                          */
    unsigned  pBuf;     /* адрес буфера                              */
} trcQLIST_STATUS;

typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  length;   /* размер сообщения                          */
    unsigned  nMsg;     /* сообщений в канале                        */
} trcQLIST_RECEIVE;

typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned  adrList;  /* адрес описателя списка                    */
    unsigned  listId;   /* идентификатор списка                      */
    unsigned  portId;   /* идентификатор порта                       */
    unsigned  reqList;  /* требование в описателе списка             */
    unsigned  numPort;  /* порядковый номер порта в списке           */
} trcQLIST_NOTIFY, trcQLIST_NOTIFY_MESSAGE;

/* Системные события и события клиента */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    unsigned int  adr;        /* адрес представляемого поля данных   */
    int           size;       /* размер представляемого поля         */
    int           truncate;   /* усечение записи                     */
} trcUSER_RECORD, trcUSER, trcFUNCTION;

/* Запись значения переменной */
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    trace_ptr     adr;        /* адрес переменной                    */
} trcVAR_RECORD, trcSERVICE_VAR;


struct trc_aio_sigev {     /* информация об оповещении сигналом*/
  int    sig_notify; /* способ уведомления (NONE, SIGNAL, SIGTHR, THREAD, FUNCTION)*/
  int    sig_signo;  /* номер (и имя) сигнала*/
  int    sig_value;  /* значения сигнала*/
  int    sig_thread; /* поток, которому был послан сигнал*/
};
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    fildes;    /* дескриптор файла*/
  int    aiocb;     /* адрес структуры aiocb*/
  int    offset;    /* позиция в файле*/
  int    nbyte;     /* количество передаваемых байтов*/
  int    reqprio;   /* смещение приоритета запроса*/
  int    buf;
  struct trc_aio_sigev sigev;
} trcAIO_RW;     /* aio_read(), aio_write()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    fildes;    /* дескриптор файла*/
  int    aiocb;     /* адрес структуры aiocb*/
  int    offset;    /* позиция в файле*/
  int    nbyte;     /* количество передаваемых байтов*/
  int    reqprio;   /* смещение приоритета запроса*/
  struct trc_aio_sigev sigev;
  int    buf;
  int    op;      /* вид синхронизации файла (O_DSYNC или O_SYNC)*/
} trcAIO_FSYNC;     /* aio_fsync()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    fildes;    /* дескриптор файла*/
  int    aiocb;     /* адрес структуры aiocb*/
  int    offset;    /* позиция в файле*/
  int    nbyte;     /* количество передаваемых байтов*/
  int    reqprio;   /* смещение приоритета запроса*/
  struct trc_aio_sigev sigev;
  int    buf;
  int    fd_arg;        /*пар-тр fildes*/
} trcAIO_CANCEL;  /* aio_cancel()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    fildes;    /* дескриптор файла*/
  int    aiocb;     /* адрес структуры aiocb*/
  int    offset;    /* позиция в файле*/
  int    nbyte;     /* количество передаваемых байтов*/
  int    reqprio;   /* смещение приоритета запроса*/
  int    state;     /* возвращенное состояние*/
  int    buf;
} trcAIO_ERROR;  /* aio_error()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    fildes;    /* дескриптор файла*/
  int    aiocb;     /* адрес структуры aiocb*/
  int    offset;    /* позиция в файле*/
  int    nbyte;     /* количество передаваемых байтов*/
  int    reqprio;   /* смещение приоритета запроса*/
  int    buf;
  int    count;     /* число переданных байтов*/
} trcAIO_RETURN; /* aio_return()*/
typedef struct {
  trcHEADER_RECORD_LIST hdr;
  int    thread;    /* идентификатор потока управления*/
  int    ret;       /* код ошибки*/
  int    nent;      /* число aiocb*/
  struct trace_timespec timeout;  /* максимальное время ожидания*/
  /* массив указателей aiocb*/
} trcAIO_SUSPEND; /* aio_suspend()*/


/*изменение таблицы символов*/
typedef struct {
    trcHEADER_RECORD_LIST hdr;
    int   thread;     /* threadExecuting*/
    int   ret;
    int   ptr;
    int   operation;  /*операция*/
    char  name[TRACE_NAME_MAX];
    char  type[TRACE_NAME_MAX];
    int   module;
} trcCHANGE_SYMTBL;

/* Запись пустая? */
#define  traceIS_RECORD_EMPTY(r)  \
  (((trcHEADER_RECORD_LIST*)(r))->type_record==traceServiceEmpty)
/* Запись испорчена? */
#define  traceIS_RECORD_BAD(r)  \
  (((trcHEADER_RECORD_LIST*)(r))->type_record==traceServiceBad)
/* Запись заголовка блока? */
#define  traceIS_RECORD_BLOCKHEAD(r)  \
  (((trcHEADER_RECORD_LIST*)(r))->type_record==traceServiceBlockhead)
/* Запись начала группы? */
#define  traceIS_RECORD_BEGIN(r)  \
  (((trcHEADER_RECORD_LIST*)(r))->type_record==traceServiceBegin)

#endif /*_TRACE_TRRECORD_H*/
