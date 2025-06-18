/* trace/trevent.h */
#ifndef _TRACE_TREVENT_H
#define _TRACE_TREVENT_H

#ifndef __ASSEMBLER__

#include  <limits.h>

/* Определения границ групп событий */
#define traceServiceGroupBegin      traceServiceEmpty
#define traceServiceGroupEnd        (traceInterruptEntry - 1)

#define traceInterruptGroupBegin    traceInterruptEntry
#define traceInterruptGroupEnd      traceInterruptExit

#define traceExceptionGroupBegin    traceExceptionCase
#define traceExceptionGroupEnd      traceExceptionCase

#define traceProcessGroupBegin      traceWindowSwitchEvent
#define traceProcessGroupEnd        (traceThreadSwitchEvent - 1)

#define traceThreadSwitchGroupBegin traceThreadSwitchEvent
#define traceThreadSwitchGroupEnd   (traceThreadStatusCreate - 1)

#define traceThreadStatusGroupBegin traceThreadStatusCreate
#define traceThreadStatusGroupEnd   (traceSigGeneration - 1)

#define traceSignalGroupBegin       traceSigGeneration
#define traceSignalGroupEnd         (traceMemPoolCreate - 1)

#define traceMemoryGroupBegin       traceMemPoolCreate
#define traceMemoryGroupEnd         (traceFunctionReturn - 1)

#define traceReturnGroupBegin       traceFunctionReturn
#define traceReturnGroupEnd         traceFunctionReturn

#define tracePthreadGroupBegin      tracePthreadCancel
#define tracePthreadGroupEnd        (tracePause - 1)

#define traceSigWaitGroupBegin      tracePause
#define traceSigWaitGroupEnd        (traceSemGetvalue - 1)

#define traceSemaphoreGroupBegin    traceSemGetvalue
#define traceSemaphoreGroupEnd      (traceMutexDestroy - 1)

#define traceMutexGroupBegin        traceMutexDestroy
#define traceMutexGroupEnd          (traceCondBroadcast - 1)

#define traceCondGroupBegin         traceCondBroadcast
#define traceCondGroupEnd           (traceMqClose - 1)

#define traceMqGroupBegin           traceMqClose
#define traceMqGroupEnd             (traceTimerSleep - 1)

#define traceTimerGroupBegin        traceTimerSleep
#define traceTimerGroupEnd          (traceIOOpen - 1)

#define traceIOGroupBegin           traceIOOpen
#define traceIOGroupEnd             (traceASetprio - 1)

#define traceAProcGroupBegin        traceASetprio
#define traceAProcGroupEnd          (traceEventCreate - 1)

#define traceEventGroupBegin        traceEventCreate
#define traceEventGroupEnd          (traceQPortCreate - 1)

#define traceQPortGroupBegin        traceQPortCreate
#define traceQPortGroupEnd          (traceSPortCreate - 1)

#define traceSPortGroupBegin        traceSPortCreate
#define traceSPortGroupEnd          (traceASemCreate - 1)

#define traceASemGroupBegin         traceASemCreate
#define traceASemGroupEnd           (traceBbrdCreate - 1)

#define traceBbrdGroupBegin         traceBbrdCreate
#define traceBbrdGroupEnd           (traceBufCreate - 1)

#define traceBufGroupBegin          traceBufCreate
#define traceBufGroupEnd            (traceChanNotifyMessage - 1)

#define traceChanGroupBegin         traceChanNotifyMessage
#define traceChanGroupEnd           (traceChanMPSend - 1)

#define traceMpChanGroupBegin       traceChanMPSend
#define traceMpChanGroupEnd         (traceFunction1 - 1)

#define traceFunctionGroupBegin     traceFunction1
#define traceFunctionGroupEnd       (traceQListCreate - 1)

#define traceQListGroupBegin        traceQListCreate
#define traceQListGroupEnd          (traceSapPortCreate - 1)

#define traceSapPortGroupBegin      traceSapPortCreate
#define traceSapPortGroupEnd          430

/* События программы пользователя */

#define TRACE_EVENT_USER_MIN    481 /* минимальный номер пользовательского события */

#define traceUser1  TRACE_EVENT_USER_MIN

#define traceUserGroupBegin traceUser1
#define traceUserGroupEnd   (traceUser1+TRACE_USER_EVENT_MAX)

#define POSIX_TRACE_UNNAMED_USEREVENT MAX_EVENT_ID

/* Предельные значения ID события */
#define MIN_EVENT_ID  traceServiceGroupBegin
#define MAX_EVENT_ID  traceUserGroupEnd

/* Проверить идентификатор события */
#define traceEVENT_VALIDATE(id) \
  ((id) >= MIN_EVENT_ID && (id) <= MAX_EVENT_ID)

/* Определим общее число событий с запасом (на 640) */
//#define traceEVENT_COUNT  640
/* Соответственно число слов (по 32 бита) */
//#define traceEVENT_PARTS  20
#define traceEVENT_COUNT  (MAX_EVENT_ID - MIN_EVENT_ID + 1)

#define tracePART_FILTER_SIZE    32

/* Номер части фильтра из идентификатора события */
#define traceEVENTID_TO_PART(id)  ((id-1)>>5)
/* Число слов (4 байта) в массиве событий */
#define tracePART_FILTER_COUNT  (traceEVENTID_TO_PART(MAX_EVENT_ID)+1)
/* Позиция в части фильтра из идентификатора события */
#define traceEVENTID_TO_POSITION(id)  ((id-1)%tracePART_FILTER_SIZE)
/* Событие установлено в SET? */
#define traceEVENTSET_ISMEMBER(id,set) \
  ((set)->parts[traceEVENTID_TO_PART(id)] & (1<<traceEVENTID_TO_POSITION(id)))

/* Массив событий: каждый бит - событие (1-регистрировать)*/
struct traceFilter {
  int parts[tracePART_FILTER_COUNT];
};

/* Объединение 2-х массив событий */
#define traceFILTER_ADD(f1,f2,fd) \
  { \
    int part; \
    for( part=0; part < tracePART_FILTER_COUNT; part++ ) \
      (fd)->parts[part] = (f1)->parts[part] | (f2)->parts[part]; \
  }
/* Убрать регистрацию событий, установленных в f2 */
#define traceFILTER_SUB(f1,f2,fd) \
  { \
    int part; \
    for( part=0; part < tracePART_FILTER_COUNT; part++ ) \
      (fd)->parts[part] = (f1)->parts[part] & ~(f2)->parts[part]; \
  }
/* Копирование фильтра событий f в fd */
#define traceFILTER_SET(f,fd) \
  { \
    int part; \
    for( part=0; part < tracePART_FILTER_COUNT; part++ ) \
      (fd)->parts[part] = (f)->parts[part]; \
  }

/* Проверяем идентификатор события */
#define traceIS_EVENTID_VALID(id)  \
  (id>=MIN_EVENT_ID && id<=MAX_EVENT_ID)

/* Принадлежность идентификатора события */
/* Идентификатор события traceServiceGroup ? */
#define traceIS_EVENTID_SERVICE(id) \
  (id>=traceServiceGroupBegin && id<=traceServiceGroupEnd)
/* Идентификатор события traceProcessGroup ? */
#define traceIS_EVENTID_PROCESS(id) \
  (id>=traceProcessGroupBegin && id<=traceProcessGroupEnd)
/* Идентификатор события traceThreadSwitchGroup ? */
#define traceIS_EVENTID_THREADSWITCH(id) \
  (id>=traceThreadSwitchGroupBegin && id<=traceThreadSwitchGroupEnd)
/* Идентификатор события traceInterruptGroup ? */
#define traceIS_EVENTID_INTERRUPT(id) \
  (id>=traceInterruptGroupBegin && id<=traceInterruptGroupEnd)
/* Идентификатор события traceThreadStatusGroup ? */
#define traceIS_EVENTID_THREADSTATUS(id) \
  (id>=traceThreadStatusGroupBegin && id<=traceThreadStatusGroupEnd)
/* Идентификатор события traceSignalGroup ? */
#define traceIS_EVENTID_SIGNAL(id) \
  (id>=traceSignalGroupBegin && id<=traceSignalGroupEnd)
/* Идентификатор события =traceExceptionGroup ? */
#define traceIS_EVENTID_EXCEPTION(id) \
  (id>=traceExceptionGroupBegin && id<=traceExceptionGroupEnd)
/* Идентификатор события traceMemoryGroup ? */
#define traceIS_EVENTID_MEMORY(id) \
  (id>=traceMemoryGroupBegin && id<=traceMemoryGroupEnd)
/* Идентификатор события traceReturnGroup ? */
#define traceIS_EVENTID_RETURN(id) \
  (id>=traceReturnGroupBegin && id<=traceReturnGroupEnd)
/* Идентификатор события tracePthreadGroup ? */
#define traceIS_EVENTID_PTHREAD(id) \
  (id>=tracePthreadGroupBegin && id<=tracePthreadGroupEnd)
/* Идентификатор события traceSigWaitGroup ? */
#define traceIS_EVENTID_SIGWAIT(id) \
  (id>=traceSigWaitGroupBegin && id<=traceSigWaitGroupEnd)
/* Идентификатор события traceSemaphoreGroup ? */
#define traceIS_EVENTID_SEMAPHORE(id) \
  (id>=traceSemaphoreGroupBegin && id<=traceSemaphoreGroupEnd)
/* Идентификатор события traceMutexGroup ? */
#define traceIS_EVENTID_MUTEX(id) \
  (id>=traceMutexGroupBegin && id<=traceMutexGroupEnd)
/* Идентификатор события traceCondGroup ? */
#define traceIS_EVENTID_COND(id) \
  (id>=traceCondGroupBegin && id<=traceCondGroupEnd)
/* Идентификатор события traceMqGroup ? */
#define traceIS_EVENTID_MQ(id) \
  (id>=traceMqGroupBegin && id<=traceMqGroupEnd)
/* Идентификатор события traceTimerGroup ? */
#define traceIS_EVENTID_TIMER(id) \
  (id>=traceTimerGroupBegin && id<=traceTimerGroupEnd)
/* Идентификатор события traceIOGroup ? */
#define traceIS_EVENTID_IO(id) \
  (id>=traceIOGroupBegin && id<=traceIOGroupEnd)
/* Идентификатор события traceAProcGroup ? */
#define traceIS_EVENTID_APROC(id) \
  (id>=traceAProcGroupBegin && id<=traceAProcGroupEnd)
/* Идентификатор события traceEventGroup ? */
#define traceIS_EVENTID_EVENT(id) \
  (id>=traceEventGroupBegin && id<=traceEventGroupEnd)
/* Идентификатор события traceQPortGroup ? */
#define traceIS_EVENTID_QPORT(id) \
  (id>=traceQPortGroupBegin && id<=traceQPortGroupEnd)
/* Идентификатор события traceSPortGroup ? */
#define traceIS_EVENTID_SPORT(id) \
  (id>=traceSPortGroupBegin && id<=traceSPortGroupEnd)
/* Идентификатор события traceChanGroup */
#define traceIS_EVENTID_CHAN(id) \
  (id>=traceChanGroupBegin && id<=traceChanGroupEnd)
/* Идентификатор события traceMpChanGroup */
#define traceIS_EVENTID_MPCHAN(id) \
  (id>=traceMpChanGroupBegin && id<=traceMpChanGroupEnd)
/* Идентификатор события traceASemGroup ? */
#define traceIS_EVENTID_ASEM(id) \
  (id>=traceASemGroupBegin && id<=traceASemGroupEnd)
/* Идентификатор события traceBbrdGroup ? */
#define traceIS_EVENTID_BBRD(id) \
  (id>=traceBbrdGroupBegin && id<=traceBbrdGroupEnd)
/* Идентификатор события traceBufGroup ? */
#define traceIS_EVENTID_BUF(id) \
  (id>=traceBufGroupBegin && id<=traceBufGroupEnd)
/* Идентификатор события traceQListGroupId ? */
#define traceIS_EVENTID_QLIST(id) \
  (id>=traceQListGroupBegin && id<=traceQListGroupEnd)
/* Идентификатор события traceSapPortGroupId ? */
#define traceIS_EVENTID_SAPPORT(id) \
  (id>=traceSapPortGroupBegin && id<=traceSapPortGroupEnd)
/* Идентификатор события traceFunctionGroup ? */
#define traceIS_EVENTID_FUNCTION(id) \
  (id>=traceFunctionGroupBegin && id<=traceFunctionGroupEnd)
/* Идентификатор события пользователя? */
#define traceIS_EVENTID_USER(id) \
  (id>=traceUserGroupBegin && id<=traceUserGroupEnd)
/* Идентификатор события представляет группу событий? */
#define traceIS_EVENTID_COMPLEX(id) \
  (id > tracePREF_COMPLEX && id < LastGroupId)
/* Иидентификатор группы пользовательских событий? */
#define traceIS_EVENTID_USERGROUP(id) \
  (id==traceUserGroupId)

#endif /* ASSEMBLER */

#endif /*_TRACE_TREVENT_H*/
