/* traceevent.h */
#ifndef _TRACE_TRACEEVENT_H
#define _TRACE_TRACEEVENT_H

#include  <limits.h>
#include  <sys/objclasses.h>

/* Идентификаторы записей/событий */
enum {
/* служебные события/записи */
  traceServiceEmpty = 1,        /* пустая                                              */
  traceServiceName,             /* имя                                                 */
  traceServiceStart,            /* начать протоколирование                             */
  traceServiceStop,             /* остановить протоколирование                         */
  traceServiceBlockhead,        /* 5 заголовок блока                                   */
  traceServiceVar,              /* значение переменной                                 */
  traceServiceBegin,            /* начало посл.записей                                 */
  traceServiceBad,              /* отмена записи                                       */
  traceServiceOverflow,         /* произошло переполнение                              */
  traceServiceOverrun,          /* 10 произошла перезапись (при переполнении)          */
  traceServiceResume,           /* возобновление после переполнения                    */
  traceServiceFlushStart,       /* начало синхронизации                                */
  traceServiceFlushStop,        /* конец синхронизации                                 */
  traceServiceFilter,           /* изменился фильтр                                    */
  traceServiceError,            /* 15 произошла ошибка                                 */
  traceServicePresent,          /* запись представления                                */
  traceServiceOverflowLog,      /* произошло переполнение файла протокола              */
  traceServiceOverrunLog,       /* произошла перезапись файла протокола (при переполнении) */
  traceServiceSchedule,         /* расписание окон                                     */
  traceServiceShutdown,         /* 20 закрыть поток трассы                             */
  traceServiceAttr,             /* атрибуты протокола                                  */
  traceServiceStatus,           /* статус протокола                                    */

  traceServiceThread,           /* свойства потока                                     */
  traceServiceMutex,            /* свойства мьютекса                                   */
  traceServiceSem,              /* 25 свойства семафора                                */
  traceServiceMqp,              /* свойства очереди сообщений                          */
  traceServiceBuf,              /* свойства буфера                                     */
  traceServiceSPort,            /* свойства порта SAMPLING                             */
  traceServiceQPort,            /* свойства порта QUEUING                              */
  traceServiceBlockOverflow,    /* 30 переполнение служебных блоков                    */
  traceServiceReboot,           /* перезагрузка модуля                                 */
  traceServiceADD,              /* запись дополнения                                   */
  traceServiceBbrd,             /* свойства доски объявлений                           */
  traceServiceQSap,             /* свойства порта SAP                                  */
  traceServiceQList,            /* свойства списка портов                              */

/* события прерываний */
  traceInterruptEntry = 51,     /* при вызове функции обработки прерываний             */
  traceInterruptExit,           /* при завершении обработки прерываний                 */

/* исключительные ситуации */
  traceExceptionCase,

/* управление процессами */
  traceWindowSwitchEvent = 55,  /* переключение окна                                   */
  traceScheduleEvent,           /* выбор расписания                                    */
  traceSystemIdleEvent,         /* простой                                             */
  traceProcessSwitchEvent,      /* переключение процесса                               */
  traceProcessEvent,            /* внешнее событие процесса                            */
  traceModStartEvent,           /* 60 процессы загружены, старт                        */
  traceProcessInitEvent,        /* начало инициализации процесса                       */
  traceSetModeEvent,            /* установить состояние процесса                       */

/* события переключения потоков */
  traceThreadSwitchEvent = 66,  /* переход управл.к дугому потоку                      */
  traceThreadDispatch,          /* вызов диспетчера                                    */

/* изменение состояния потока управления */
  traceThreadStatusCreate = 71, /* порождение потоков                                  */
  traceThreadStatusExit,        /* завершение потоков                                  */
  traceThreadStatusActivate,    /* переход потока в работоспособное состояние          */
  traceThreadStatusBlock,       /* переход потока в неработоспособное состояние        */
  traceThreadStatusSchedYield,  /* 75 отказ потока от использ.ЦПУ                      */
  traceThreadStatusTimeQuantum, /* отказ потока от использ.ЦПУ (без потери работоспособ.) */
  traceThreadStatusSuspend,
  traceThreadStatusResume,
  traceThreadStatusStart,       /* не используется                                     */
  traceThreadStatusStop,        /* 80 не используется                                  */
  traceThreadStatusLock,        /* не используется                                     */
  traceThreadStatusUnlock,      /* не используется                                     */
  traceThreadStatusTimedwait,   /* не используется                                     */
  traceThreadStatusPeriodicwait,/* не используется                                     */
  traceThreadStatusReplenish,   /* 85 не используется                                  */
  traceThreadStatusReleasePoint,/* не используется                                     */
  traceThreadStatusAssert,      /* assert                                              */

/* порождение и доставка сигналов */
  traceSigGeneration = 96,      /* генерация сигнала                                   */
  traceSigDelivery,             /* доставка сигнала                                    */
  traceSigCatchFunc,            /* вызов функции обработки сигнала                     */
  traceSigCatchFuncRet,         /* возврат управления из функции обр.сигн.             */
  traceSigThreadCancel,         /* 100 получение запроса на удаление (не используется) */

/*распределение памяти traceMemoryGroup */
  traceMemPoolCreate = 106,
  traceMpMemPoolCreate,
  traceMemPoolAdd,
  traceMpMemPoolAdd,
  traceMemPoolDelete,           /* 110 */
  traceMemPoolAlloc,
  traceMpMemPoolAlloc,
  traceMemPoolFree,
  traceMpMemPoolFree,

/* возврат из функции traceReturnGroup */
  traceFunctionReturn = 121,    /* не используется */

/* функции управления потоками  traceThreadGroup */
  tracePthreadCancel = 125,
  tracePthreadJoin,
  tracePthreadSigmask,
  tracePthreadSetschedparam,
  tracePthreadSetschedprio,
  tracePthreadSetcancelstate,   /* 130 */
  tracePthreadSetcanceltype,
  tracePthreadDetach,

/* функции ожидания сигналов traceSigWaitGroup */
  tracePause = 136,    /* не используется */
  traceSigsuspend,
  traceSigtimedwait,
  traceSigwait,
  traceSigwaitinfo,             /* 140 */

/* функции работы с семафорами traceSemaphoreGroup*/
  traceSemGetvalue = 146,
  traceMpSemGetvalue,
  traceSemOpen,
  traceMpSemOpen,
  traceSemClose,                /* 150 */
  traceMpSemClose,
  traceSemUnlink,
  traceMpSemUnlink,
  traceSemInit,
  traceMpSemInit,               /* 155 */
  traceSemDestroy,
  traceMpSemDestroy,
  traceSemPost,
  traceMpSemPost,
  traceDelayedSemPost,          /* 160 */
  traceDelayedMpSemPost,
  traceSemWait,
  traceMpSemWait,
  traceSemTimedwait,
  traceMpSemTimedwait,          /* 165 */
  traceSemTrywait,
  traceMpSemTrywait,

/* функции работы с мьтексами traceMutexGroup*/
  traceMutexDestroy = 171,
  traceMutexInit,
  traceMutexSetprioceiling,
  traceMutexGetprioceiling,
  traceMutexLock,               /* 175 */
  traceMutexTryLock,
  traceMutexTimedlock,
  traceMutexUnlock,

/*функции работы с условными переменными traceCondGroup*/
  traceCondBroadcast = 181,    /* не используется */
  traceCondDestroy,
  traceCondInit,
  traceCondSignal,
  traceCondTimedwait,           /* 185 */
  traceCondWait,

/* функции работы с очередями сообщений traceMqGroup */
  traceMqClose = 191,
  traceMpMqClose,
  traceMqNotify,
  traceMqOpen,
  traceMpMqOpen,                /* 195 */
  traceMqReceive,
  traceMqTimedreceive,
  traceMpMqReceive,
  traceMpMqTimedreceive,
  traceMqSend,                  /* 200 */
  traceMqTimedsend,
  traceMpMqSend,
  traceMpMqTimedsend,
  traceMqSetattr,
  traceMqGetattr,               /* 205 */
  traceMpMqSetattr,
  traceMpMqGetattr,
  traceMqUnlink,
  traceMpMqUnlink,

/* функции работы с часами и таймерами traceTimerGroup */
  traceTimerSleep = 216,
  traceTimerNanosleep,
  traceTimerClockSettime,
  traceTimerExpire,
  traceTimerCreate,             /* 220 */
  traceTimerDelete,
  traceTimerGetoverrun,
  traceTimerSettime,

/* функции ввода/вывода traceIOGroup */
  traceIOOpen = 226,
  traceIOClose,
  traceIOMkfifo,
  traceIOLseek,
  traceIODup,                   /* 230 */
  traceIODup2,
  traceIOPipe,
  traceIORead,
  traceIOWrite,
  traceIOFdatasync,             /* 235 */
  traceIOFsync,

/* ARINC */
/* управление процессами traceAProcessGroup */
  traceASetprio = 241,
  traceASuspendSelf,
  traceASuspend,
  traceAStop,
  traceAStart,                  /* 245 */
  traceALockPreemption,
  traceAUnlockPreemption,
  traceATimedwait,
  traceAPeriodicwait,
  traceAReleasePoint,           /* 250 */
  traceADeadlinePoint,
  traceAReplenish,
  traceAResume,

/* управление событиями traceEventGroup */
  traceEventCreate = 261,
  traceEventSet,
  traceEventReset,
  traceEventWait,

/* функции обслуживания портов traceQPortGroup */
  traceQPortCreate = 271,
  traceQPortSend,
  traceQPortReceive,
  traceQPortNotify,             /* не используется */
  traceQPortGetId,              /* 275 не используется */
  traceQPortMsgLock,
  traceQPortMsgUnlock,
  traceQPortGetStatus,          /* не используется */
  traceQPortClear,

/* функции обслуживания портов traceSPortGroup */
  traceSPortCreate = 281,
  traceSPortWrite,
  traceSPortRead,
  traceSPortGetId,              /* не используется */
  traceSPortMsgLock,            /* 285 */
  traceSPortMsgUnlock,
  traceSPortGetStatus,          /* не используется */

/* функции обслуживания семафоров ARINC traceASemGroup */
  traceASemCreate = 291,
  traceASemWait,
  traceASemSignal,
  traceASemGetId,               /* не используется */
  traceASemGetStatus,           /* 295  не используется */

/* функции обслуживания доски объявлений traceBbrdGroup */
  traceBbrdCreate = 306,
  traceBbrdDisplay,
  traceBbrdRead,
  traceBbrdClear,
  traceBbrdGetId,               /* 300  не используется */
  traceBbrdGetStatus,           /* не используется */

/* функции обслуживания сообщений ARINC traceBufGroup */
  traceBufCreate = 316,
  traceBufSend,
  traceBufReceive,
  traceBufGetId,                /* не используется */
  traceBufGetStatus,            /* 320  не используется */
  traceBufSendDirect,
  traceBufReceiveDirect,
  traceBufReleaseBuffer,
  traceBufFromThread,

/* события обслуживания каналов traceChanGroup */
  traceChanNotifyMessage = 326,
  traceChanNotifyBuffer,
  traceChanPutMessage,
  traceChanGetMessage,          /* не используется */
  traceChanPutBuffer,           /* 330 */
  traceChanGetBuffer,

/* события обслуживания межпроцессорных каналов traceMpChanGroup */
  traceChanMPSend,
  traceChanMPReceive,
  traceChanThreadRequest,
  traceChanMPSendEnd,           /* 335 */
  traceChanMPSendBuf,
  traceChanMPBufs,
  traceChanNetSend,
  traceChanMPRequest,           /* посылка требования модулю,
                                   получение требования       */

/* события выполнения системных функций....*/
  traceFunction1 = 341,
  traceFunction2,
  traceFunction3,
  traceFunction4,
  traceFunction5,               /* 345 */
  traceFunction6,
  traceFunction7,
  traceFunction8,
  traceFunction9,
  traceFunction10,              /* 350 */
  traceFunction11,
  traceFunction12,
  traceFunction13,
  traceFunction14,
  traceFunction15,              /* 355 */
  traceFunction16,
  traceFunction17,
  traceFunction18,
  traceFunction19,
  traceFunction20,              /* 360 */
  traceFunction21,
  traceFunction22,
  traceFunction23,
  traceFunction24,
  traceFunction25,              /* 365 */
  traceFunction26,
  traceFunction27,
  traceFunction28,
  traceFunction29,
  traceFunction30,              /* 370 */
  traceFunction31,
  traceFunction32,
  traceFunction33,
  traceFunction34,
  traceFunction35,              /* 375 */
  traceFunction36,
  traceFunction37,
  traceFunction38,
  traceFunction39,
  traceFunction40,              /* 380 */
  traceFunction41,
  traceFunction42,
  traceFunction43,
  traceFunction44,
  traceFunction45,              /* 385 */
  traceFunction46,
  traceFunction47,
  traceFunction48,
  traceFunction49,
  traceFunction50,              /* 390 */
  traceFunction51,
  traceFunction52,
  traceFunction53,
  traceFunction54,
  traceFunction55,              /* 395 */
  traceFunction56,
  traceFunction57,
  traceFunction58,
  traceFunction59,
  traceFunction60,              /* 400 */
  traceFunction61,
  traceFunction62,
  traceFunction63,
  traceFunction64,

/* функции обслуживания портов traceSPortGroup */
  traceQListCreate = 410,
  traceQListAdd,
  traceQListAction,
  traceQListStatus,
  traceQListReceive,
  traceQListNotifyMessage,

  /* функции обслуживания портов traceSapPortGroup */
  traceSapPortCreate = 420,
  traceSapPortSend,
  traceSapPortReceive

};

#if !defined(POSIX_SOURCE)

/*комплексные события - составляющие некоторые подмножества событий*/

#define tracePREF_COMPLEX 0x1000 /*префикс составного события*/
/*получить из ID события #ссылки*/
#define traceCOMPLEX_ID_TO_REFER(id)  ((id)&~tracePREF_COMPLEX)
/*идентификаторы комплексных событий*/
#define  traceServiceGroupId        (traceServiceGroupBegin       | tracePREF_COMPLEX)
#define  traceProcGroupId           (traceProcessGroupBegin       | tracePREF_COMPLEX)
#define  traceThreadSwitchGroupId   (traceThreadSwitchGroupBegin  | tracePREF_COMPLEX)
#define  traceInterruptGroupId      (traceInterruptGroupBegin     | tracePREF_COMPLEX)
#define  traceThreadStatusGroupId   (traceThreadStatusGroupBegin  | tracePREF_COMPLEX)
#define  traceSignalGroupId         (traceSignalGroupBegin        | tracePREF_COMPLEX)
#define  traceExceptionGroupId      (traceExceptionGroupBegin     | tracePREF_COMPLEX)
#define  traceMemoryGroupId         (traceMemoryGroupBegin        | tracePREF_COMPLEX)
#define  traceReturnGroupId         (traceReturnGroupBegin        | tracePREF_COMPLEX)
#define  traceThreadGroupId         (tracePthreadGroupBegin       | tracePREF_COMPLEX)
#define  traceSigWaitGroupId        (traceSigWaitGroupBegin       | tracePREF_COMPLEX)
#define  traceSemaphoreGroupId      (traceSemaphoreGroupBegin     | tracePREF_COMPLEX)
#define  traceMutexGroupId          (traceMutexGroupBegin         | tracePREF_COMPLEX)
#define  traceCondGroupId           (traceCondGroupBegin          | tracePREF_COMPLEX)
#define  traceMqGroupId             (traceMqGroupBegin            | tracePREF_COMPLEX)
#define  traceTimerGroupId          (traceTimerGroupBegin         | tracePREF_COMPLEX)
#define  traceIOGroupId             (traceIOGroupBegin            | tracePREF_COMPLEX)
#define  traceAProcGroupId          (traceAProcGroupBegin         | tracePREF_COMPLEX)
#define  traceEventGroupId          (traceEventGroupBegin         | tracePREF_COMPLEX)
#define  traceQPortGroupId          (traceQPortGroupBegin         | tracePREF_COMPLEX)
#define  traceSPortGroupId          (traceSPortGroupBegin         | tracePREF_COMPLEX)
#define  traceChanGroupId           (traceChanGroupBegin          | tracePREF_COMPLEX)
#define  traceMpChanGroupId         (traceMpChanGroupBegin        | tracePREF_COMPLEX)
#define  traceASemGroupId           (traceASemGroupBegin          | tracePREF_COMPLEX)
#define  traceBbrdGroupId           (traceBbrdGroupBegin          | tracePREF_COMPLEX)
#define  traceBufGroupId            (traceBufGroupBegin           | tracePREF_COMPLEX)
#define  traceFunctionGroupId       (traceFunctionGroupBegin      | tracePREF_COMPLEX)
#define  traceQListGroupId          (traceQListGroupBegin         | tracePREF_COMPLEX)
#define  traceSapPortGroupId        (traceSapPortGroupBegin       | tracePREF_COMPLEX)
#define  traceUserGroupId           (traceUserGroupBegin          | tracePREF_COMPLEX)
/* Общее число групп событий */
#define  traceEVENT_GROUP_NUMBER 28

#endif /*!defined(POSIX_SOURCE)*/

extern  int *traceComplexRefer[];

  #include "htrace/trevent.h"

#endif /*_TRACE_TRACEEVENT_H*/
