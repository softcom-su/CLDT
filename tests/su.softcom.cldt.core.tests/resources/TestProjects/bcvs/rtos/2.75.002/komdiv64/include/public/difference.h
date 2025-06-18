#ifndef PUBLIC_DIFFERENCE_H
#define PUBLIC_DIFFERENCE_H

#include <version.h>

#if OS_MAJOR == 2
//#  define LOG_LAZHEAD 0
//#  define LOG_BSDFMT  0
#  define MAX_NUMBER_OF_PARTITIONS     1

#  define MODCALL(a)      { a; }
#  define  procIS_ARINC (0)
#  define  PTHREAD_STATE_WAITEDRD  PTHREAD_WAITEDRD
#  define  M_SYSLOG  0

#  define PTHREAD_CAUSE_WAIT_OK        PTHREAD_WAIT_OK
#  define PTHREAD_CAUSE_WAIT_DESTROY   PTHREAD_WAIT_DESTROY

#  define SYSPROC_PID        0        /* pid главного системного процесса             */
#  define curpid             0        /* pid текущего процесса                        */
#  define procIS_SYSTEM      1
#  define procIS_PRIVILEGED  1

#  define objid_t            void*
#  define thread_control     thrdControl

#  define kern_strcasecmp    strcasecmp
#  define kern_toupper       toupper
#  define syscall_syslog     syslog

#  define MEMCHECK_R(p)      0
#  define MEMCHECK_W(p)      0

#  define PTHREAD_UNSET_PENDED(t)   (t)->m_status &= ~PTHREAD_PENDED

//#  define CLASS_SEMCONTROL          CLASS_SEMCONTROL
#  define CLASS_SSEMCONTROL         CLASS_SEMCONTROL

#  define CLASS_CNDCONTROL          CLASS_CNDCONTROLPX
#  define CLASS_SCNDCONTROL         CLASS_CNDCONTROLPX
#  define CLASS_MTXCONTROL          CLASS_MTXCONTROLPX
#  define CLASS_SMTXCONTROL         CLASS_MTXCONTROLPX

/* Переход от адреса объекта к идентификатору */
#  define objTO_ID(p)  (objid_t)p
#  define objID_TO_OBJECT(pClass,objId)  (objHEADER*)objId

typedef void*  QUEUING_PORT_ID_TYPE;      /* идентификатор порта с очередью     */
typedef void*  SAMPLING_PORT_ID_TYPE;     /* идентификатор порта без очередей   */
typedef void*  SAP_PORT_ID_TYPE;          /* идентификатор порта SAP            */
typedef void*  QUEUING_PORT_LIST_ID_TYPE;

#  ifdef INCLUDE_TRACE

      /* Сигнатура описателя буфера */
#     define traceSIGNATURE        "TRACE 2.0"

#     define TRACE_FUNC_PROCEVENT(func,funcname)

      extern volatile int           _traceEffect;  /*триггер трассировки */
      extern volatile int           traceIntEvent;

      extern int traceThreadSys;
      extern int traceThreadUser;

      struct thrdControl;
      extern void         tracePropertyThread( int pid, struct thrdControl* thread );

#     define traceEffectGMask       traceGMask
#     define traceEffect()          _traceEffect
      /* Установить флаг регистрации событий - включить трассировку */
#     define traceSetEffect()       _traceEffect=1
      /* Снять флаг регистрации событий - выключить трассировку */
#     define traceUnsetEffect()     _traceEffect=0
      /* Флаг регистрации событий установлен ? */
#     define traceIsEffect()        _traceEffect

#  else
#     define traceUnsetEffect()
#     define tracePropertyThread(pid,thread)
#     define TRACE_FUNC_PROCEVENT(func,funcname)
#  endif /* INCLUDE_TRACE */

      void memDi(unsigned int *a,unsigned int len);
#   define PCORE()
#   define cpuCoreGet() 0
#   define MAX_CORES 1
#else
/* OS_MAJOR != 2 */

#   include <sys/types.h>

typedef unsigned int  QUEUING_PORT_ID_TYPE;      /* идентификатор порта с очередью     */
typedef unsigned int  SAMPLING_PORT_ID_TYPE;     /* идентификатор порта без очередей   */
typedef unsigned int  SAP_PORT_ID_TYPE;          /* идентификатор порта SAP            */
typedef unsigned int  QUEUING_PORT_LIST_ID_TYPE;

#  define PTHREAD_UNSET_PENDED(t)   (t)->m_state &= ~PTHREAD_STATE_PENDED

#  ifdef INCLUDE_TRACE

#       if OS_MAJOR >= 4
#           define traceSIGNATURE       "TRACE 4.0"
#           define traceEffect()       modLinkPage->mlp_traceEffect
#           define traceSetEffect()    MODCALL(modLinkPageInternal->mlp_traceEffect = 1;);
#           define traceUnsetEffect()  MODCALL(modLinkPageInternal->mlp_traceEffect = 0;);
#           define traceIsEffect()     modLinkPage->mlp_traceEffect

#       else
            /* Сигнатура описателя буфера */
#           define traceSIGNATURE        "TRACE 3.0"
            /* Проверка состояния флаг регистрации событий */
#           define traceEffect()       modLinkPage->mlp_traceEffect
            /* Установить флаг регистрации событий - включить трассировку */
#           define traceSetEffect()    MODCALL(modLinkPageInternal->mlp_traceEffect = 1;);
            /* Снять флаг регистрации событий - выключить трассировку */
#           define traceUnsetEffect()  MODCALL(modLinkPageInternal->mlp_traceEffect = 0;);
            /* Можно регистрировать событие? */
#           define traceIsEffect()     modLinkPage->mlp_traceEffect

#       endif

#     define TRACE_FUNC_PROCEVENT(func,funcname)                        \
         static const struct func_procevent const trcfunc_##funcname =  \
         {(void *)func,#funcname};                                      \
            DATA_SET(trcfunc,trcfunc_##funcname)
     struct thread_control;
     extern void         tracePropertyThread( int pid, struct thread_control* thread );
//     extern int          tracePpeInit( void );
#  else
#     define traceUnsetEffect()
#     define tracePropertyThread(pid,thread)
#     define TRACE_FUNC_PROCEVENT(func,funcname)
#  endif /* INCLUDE_TRACE */

     void memDi(unsigned int *a, pid_t pid, unsigned int len, int check);
#define THREAD_SUSPEND(a,b) threadSuspend(a,b)
#if OS_MAJOR == 3
   #define PCORE()
   #define CUR_PROC_PID modLinkPage->mlp_CurProcPid
   #define PROC_DISPATCH_LEVEL modLinkPage->mlp_DispatchLevel
   #define MAX_CORES 1
   #define cpuCoreGet() 0
   #define MULTICORE_ADD(field, value )  field += value
   #define MULTICORE_SWAP(field, value ) field = value
#else
   #define PCORE() struct core *pcore = &modLinkPage->mlp_cores[cpuCoreGet()]
   #define CUR_PROC_PID pcore->c_CurProcPid
   #define PROC_DISPATCH_LEVEL   pcore->c_DispatchLevel
   #define MULTICORE_ADD(field, value ) cpuLDADD((paddr_t)(long)&field, value )
   #define MULTICORE_SWAP(field, value ) cpuSWAP((paddr_t)(long)&field, (rreg)(long)value )
#endif


#endif
#endif   /* PUBLIC_DIFFERENCE_H  */
