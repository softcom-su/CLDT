#ifndef PUBLIC_ERREP_H
#define PUBLIC_ERREP_H

#include <version.h>
#include <arinc/error.h>
/* Внутренние идентификаторы ошибок (IEID_*)  */
typedef enum
{
   IEID_USER_STACK_OVERFLOW = 1,       /* переполнение пользов. стека потока           */
   IEID_SYSTEM_STACK_OVERFLOW,         /* переполнение системного стека потока         */
   IEID_INTERRUPT_STACK_OVERFLOW,      /* переполнение стека обработчика прерываний    */
   IEID_INVALID_USER_SYSCALL,          /* недопустимый номер syscall                   */
   IEID_MODPOSTPROCEVENT_ELIST,        /* список свободных описателей внеш.событий пуст*/
   IEID_OBJECT_INIT_ERROR,             /* ошибка инициализации процесса                */
   IEID_INIT_SYSPOOL_ERROR,            /* ошибка инициализации системного пула памяти  */
   IEID_THREAD_CREATE_ERROR,           /* ошибка при порождении корневого потока       */
   IEID_NOMEMORY_PROCDATA,             /* нет памяти в области сис. данных процесса    */
   IEID_CHECK_POOL,          /* 10 */  /* некорректный пул памяти                      */
   IEID_DOUBLE_SCHEDULE,               /* более одного расписания, использ. при старте */
   IEID_SYSTEM_SYSCALL,                /* syscall в привилегированном режиме           */
   IEID_NOINIT_SCHEDULE,     /* не указано расписание, используемое при старте системы */
   IEID_SECTION_ERROR,                 /* cекция ELF-файла не может быть загружена     */
   IEID_OPEN_ERROR,                    /* ошибка открытия ELF-файла                    */
   IEID_GETELFHDR_ERROR,               /* некорректный заголовок ELF-файла             */
   IEID_LOAD_THREAD_ERROR,             /* ошибка при порождении потока загрузки        */
   IEID_INVALID_MODULE_CALLBACK,       /* callback-функция модуля не найдена           */
   IEID_INVALID_PARTITION_CALLBACK,    /* callback-функция процесса не найдена         */
   IEID_INVALID_PARTITION_MEMSIZE, /* 20 */ /* некорректный размер сегмента PROCDATA   */
   IEID_INVALID_PARTITION_MEMSIZE_PID, /* запрос памяти для несуществующего процесса   */
   IEID_INVALID_PARTITION_CONFIG,      /* Invalid or duplicate partition identifier    */
   IEID_INVALID_PARTITION_SCHEDULE,    /* недопустимый идентификатор процесса          */
   IEID_PARTITION_SCHEDULE_TO_LONG,    /* слишком много процессов в расписании         */
   IEID_DUPLICATE_PARTITION_SCHEDULE,  /* процесс указан несколько раз в расписании    */
   IEID_TLBMOD,                        /* TLB modification exception                   */
   IEID_TLBL,                          /* TLB exception (load or instr. fetch)         */
   IEID_TLBS,                          /* TLB exception (store)                        */
   IEID_ADEL,                          /* address error exception (load/instr. fetch)  */
   IEID_ADES,                /* 30 */  /* address error exception (store)              */
   IEID_IBE,                           /* bus error exception (instruction fetch)      */
   IEID_DBE,                           /* bus error exception (data ref: load or store)*/
   IEID_RI,                            /* reserved instruction exception               */
   IEID_CPU,                           /* coprocessor unusable exception               */
   IEID_TR,                            /* trap exception                               */
   IEID_IWE,                           /* Instruction Watch Exception                  */
   IEID_DWE,                           /* Data Watch Exception                         */
   IEID_MCHECK,                        /* Machine Check Exception                      */
   IEID_OV,                            /* Integer overflow                             */
   IEID_FP_EXC_E,            /* 40 */  /* Unimplemented operation                      */
   IEID_FP_EXC_V,                      /* Invalid floating-point operation             */
   IEID_FP_EXC_Z,                      /* Floating-point divide by zero                */
   IEID_FP_EXC_O,                      /* Floating-point overflow                      */
   IEID_FP_EXC_U,                      /* Floating-point underflow                     */
   IEID_FP_EXC_I,                      /* Floating-point inexact result                */
   IEID_BP,                            /* Integer divide by zero                       */
   IEID_NOFLAG,                        /* нет окна начала периода процесса             */

   HM_FMT_ERROR,
   HM_FMT_ERRL,
   HM_FMT_ADDRESS,           /* 50 */
   HM_FMT_SYSTEM_STATE,
   HM_FMT_THREAD,
   HM_FMT_PARTITION,
   HM_FMT_DISPL,
   HM_FMT_LOCKL,
   HM_FMT_IV,
   HM_FMT_SCHEDULE,
   HM_FMT_WINID,
   HM_FMT_ACTION,
   HM_FMT_SHUTDOWN,          /* 60 */

   IEID_NOMODMEM_CACHED_SYSDATA,        /* недостаточно памяти модуля                   */
   IEID_NOMODMEM_CHAN_BUFFER_DATA,      /* недостаточно памяти модуля                   */
   IEID_NOMODMEM_USER_DATA,             /* недостаточно памяти модуля                   */
   IEID_MPCHAN_TABLE_INIT,             /* недостаточно памяти модуля                   */

   IEID_CHAN_INIT_BUFFERS,             /* ошибка при инициализации буферов канала      */

   IEID_CHAN_NOPORT,                   /* порт не описан ни в одном процессе           */
   IEID_CHAN_INVDIR,                   /*неверное направление передачи данных для порта*/
   IEID_CHAN_INVPORTMODE,              /*неверный режим работы порта (queuing/sampling)*/
   IEID_CHAN_DIFFCONF1,                /* параметры портов канала не согласованы       */
   IEID_CHAN_DIFFCONF2,                /* параметры портов канала не согласованы       */
   IEID_CHAN_DIFFCONF3,                /* параметры портов канала не согласованы       */
   IEID_CHAN_INVCHANTYPE,              /* для канала не определено направление передачи
                                            (все порты принадлежат к PSEUDO-partition) */
   IEID_CHAN_INVCONF,             /* 70 */
   IEID_CHAN_INVPRM,                   /* Параметры канала (длина сообщения, число сообщений) слишком велики: канал %s(%d), Partition Name %s */
   IEID_CHAN_PSDINV,                   /* недопустимая PSEUDO: канал %s(%d), %s, Pseudo_Partition Name %s                          */
   IEID_CHAN_DBLPSD,                   /* канал имеет с обеих сторон PSEUDO            */
   IEID_CHAN_SGETMSG,                  /* Ошибка чтения SAMPLING сообщения             */

   IEID_MPCHAN_INACCESIBLE,            /* Модуль %d не доступен (%d) */
   IEID_MPCHAN_INVCPU,                 /* Неверный номер модуля (платы): канал %s(%d), Pseudo_Partition Name %s, PhysicalAddress %d */
   IEID_MPCHAN_INVLINK,                /* Неправильный номер логической связи: канал %s(%d), Pseudo_Partition Name %s, PhysicalAddress %d    */
   IEID_MPCHAN_INVLINK1,               /* У модуля сконфигурировано меньше логических связей: канал %s(%d), Pseudo_Partition Name %s, PhysicalAddress %d */
   IEID_MPCHAN_INVLINK2,               /* От модуля %d получено требование регистрации логической связи %d (ID=%d), отсутствующей на данном модуле */
   IEID_MPCHAN_INVLINK3,               /* Модуль %d, канал %s(%d), имя Pseudo_Partition имя %s, PhysicalAddress %d требует MPCHAN_MAX_NUMBER>=%d */
   IEID_MPCHAN_INVLINK4,               /* Номер логической связи превышает указанное в конфигурации значение: канал %s(%d), Pseudo_Partition Name %s, PhysicalAddress %d */
   IEID_MPCHAN_DBLLINK,                /* Логическая связь используется несколькими каналами: канал %s(%d), Pseudo_Partition Name %s, PhysicalAddress %d */
   IEID_MPCHAN_NOLINK,                 /* "The module partner %d does not have the logical link %d: channel %s(%d)" */
   IEID_MPCHAN_NOSHMEM,
   IEID_MPCHAN_SHMEMER,
   IEID_MPCHAN_SHSENDER,
   IEID_MPCHAN_INTRE,
   IEID_MPCHAN_INVREG,
   IEID_MPCHAN_INVMSG,                 /* Сообщение искажено (RIO) */
   IEID_MPCHAN_MEMALLOC,
   IEID_MPCHAN_NOTSUP,
   IEID_MPCHAN_BUSADR,                 /* Ошибка при вызове boardLocalToBusAdrs */

   IEID_SOFT_DEADLINE,                 /* превышении сроков работы потока управления   */

   IEID_TLBALLOC,
   IEID_MKPS_CLOCKENOMEM,
   IEID_MKPS_CLOCKERROR,
   IEID_MKPS_ERROR,
   IEID_SYSIO_ERROR,
   IEID_SYSIO_ERROR1,
   IEID_SEGMENTS_CROSSING,
   IEID_INVALID_PARTITION_ALLOC,
   IEID_INVALID_WINDOW_SIZE,
   IEID_READ_ERROR,
   IEID_MPCHAN_SAP_INVLINK,
   IEID_MPCHAN_SAP_DBLLINK,
   IEID_MPCHAN_SAP_INVLINK4,
   IEID_MPCHAN_THREAD_CREATE_ERROR,
   IEID_MPCHAN_SAP_INVNAME,

   IEID_CHAN_USEDPORT,
   IEID_CHAN_SAP_INVPSD,
   IEID_CHAN_SAP_STD,
   IEID_CHAN_DIFFSIZE,
   IEID_CHAN_DIFFREFRESH,
   IEID_CHAN_SAP_INVMSGSIZE,
   IEID_MPCHAN_NOTSUP1,
   IEID_CHAN_DIFFNUMBER,
   IEID_CACHE_ERROR,
   IEID_LAST
} IERROR_CODE_TYPE;

/* Идентификаторы ошибок (Error_ID), используемые при конфигурировании       */
typedef enum {
   CEID_DEADLINE_MISSED   =      1, /* превышение лимита времени                    */
   CEID_APPLICATION_ERROR ,         /* ошибка, обнаруженная прикладной программой   */
   CEID_NUMERIC_ERROR,              /* ошибка при выполнении арифметических действий*/
   CEID_ILLEGAL_REQUEST,            /* недопустимое обращение к ОС                  */
   CEID_STACK_OVERFLOW,             /* переполнение стека                           */
   CEID_MEMORY_VIOLATION,           /* нарушение защиты памяти,                     */
                                    /* попытка выполнения привилегированной команды */
   CEID_HARDWARE_FAULT,             /* сбой оборудования (аппаратная ошибка)        */
   CEID_POWER_FAIL,                 /* сбой питания                                 */

   CEID_MODPOSTPROCEVENT_ELIST,     /* список свободных описателей внеш.событий пуст*/
   CEID_PROCINIT_ERROR,             /* ошибка инициализации процесса                */
   CEID_NOMEMORY_PROCDATA,          /* нет памяти в области системных данных проц.  */
   CEID_PARTLOAD_ERROR,             /* ошибка при загрузке процесса                 */

   CEID_ASSERT,                     /* самодиагностика                              */

   CEID_CONFIG_ERROR,               /* ошибка конфигурации                          */
   CEID_CHECK_POOL,                 /* ошибка при проверке пула памяти              */
   CEID_UNHANDLED_INT,               /* неинициализированное прерывание              */
   CEID_LAST
} CERROR_CODE_TYPE;

#define ERROR_IEID_TO_CEID(IEID) __errCodes[IEID].ec_CEID;

void errPrintf( IERROR_CODE_TYPE,...);

const char *errIErrorFmt(IERROR_CODE_TYPE err_id);

#if OS_MAJOR >= 3
void errKprint( IERROR_CODE_TYPE ieid,...);

struct bufptr {
   char *buf;
   size_t size;
};
#define CREATE_BUFPTR(a) char bbuf[a]; struct bufptr buf = { bbuf, sizeof(bbuf) }
void errSPrintf( struct bufptr *buf, IERROR_CODE_TYPE ieid,...);
//const char *errIErrorFmt(IERROR_CODE_TYPE err_id);
const char *errAErrorFmt(ERROR_CODE_TYPE err_id);
const char *errCErrorFmt(CERROR_CODE_TYPE err_id);
#endif   /* OS_MAJOR == 3 */

#endif /* PUBLIC_ERREP_H */
