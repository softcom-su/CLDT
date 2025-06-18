/* channel/extradebug.h
 */

#ifndef _CHANNEL_EXTRADEBUG_H
#define _CHANNEL_EXTRADEBUG_H

#ifdef EXTRA_DEBUG

/* События */
#define extradebugCHAN_GET_BUF   1
#define extradebugCHAN_PUT_MSG   2

/* Флаги заданий */
#define extradebugSTART          1
#define extradebugSTOP           2
#define extradebugPROCRESTART    3
#define extradebugTHREADRESTART  4

/* Структура связи */
struct s_extradebug {
   u_int monId;
   u_int procId;     /* исследуемый процесс */
   u_int threadId;   /* исследуемый поток   */
   u_int eventId;       /* событие */
   u_int objId;       /* объект */
   u_int taskId;       /* задание */
};

void extradebugSetMon ( void );
void extradebugSetEvent ( u_int event, u_int obj, u_int task );

#define extradebugRESTART_CHAN_GET_BUF(obj)
#ifdef STOP_CHAN_GET_BUF
/* STOP,START потоку, взявшему буфер */
#undef extradebugRESTART_CHAN_GET_BUF(obj)
#  define extradebugRESTART_CHAN_GET_BUF(obj) \
      extradebugTask ( extradebugCHAN_GET_BUF, obj, extradebugTHREADRESTART )
#endif

#ifdef RESTART_CHAN_GET_BUF
/* STOP,START потоку, взявшему буфер */
#undef extradebugRESTART_CHAN_GET_BUF(obj)
#  define extradebugRESTART_CHAN_GET_BUF(obj) \
      extradebugTask ( extradebugCHAN_GET_BUF, obj, extradebugPROCRESTART )
#endif

#ifdef STOP_CHAN_PUT_MSG
/* STOP потоку, кладущему сообщение */
#  define extradebugSTOP_CHAN_PUT_MSG() \
      extradebugTask ( u_int event, extradebugSTOP )
#else
#  define extradebugSTOP_CHAN_PUT_MSG()
#endif

#define extradebugSTOP_CHAN_GET_MSG()  /* STOP потоку, берущему сообщение */
#define extradebugSTOP_CHAN_NTF_MSG()  /* STOP потоку, извещающему о сообщении */
#define extradebugSTOP_CHAN_NTF_BUF()  /* STOP потоку, извещающему о буфере */

#else /*EXTRA_DEBUG*/

#  define extradebugRESTART_CHAN_GET_BUF(obj)
#  define extradebugSTOP_CHAN_PUT_BUF()
#  define extradebugSTOP_CHAN_PUT_MSG()
#  define extradebugSTOP_CHAN_GET_MSG()
#  define extradebugSTOP_CHAN_NTF_MSG()
#  define extradebugSTOP_CHAN_NTF_BUF()

#endif /*EXTRA_DEBUG*/

#endif /* _CHANNEL_EXTRADEBUG_H */
