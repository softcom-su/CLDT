/* trace/trobject.h */
#ifndef _TRACE_TROBJCT_H
#define _TRACE_TROBJCT_H

#include <sys/types.h>
#include <htrace/trevent.h>

/* Структура параметров функции TraceSetVariable */
struct TraceSetVariable_p {
  void*                    adr;
  char*                    name;
  int                      type;
  int                      size;
  trace_event_set_t*       mask;
//  struct trace_descriptor* traceDescriptor;
};

/* Описатель заказанного для трассирования объекта процесса */
struct trace_object {
   objid_t  objId;                  /* идентификатор              */
   void*    adr;                    /* адрес идентификатора       */
   u_int    type;                   /* класс                      */
   char     name[TRACE_NAME_MAX];
//   struct trace_descriptor*
//            traceDescriptor;
};

#ifdef KERNEL

#include <private/queue.h>

/* Имя события*/
#define  traceTYPENAME_IDEVENT        0x1000
#if 0
#if OS_MAJOR == 2
/* Переход от адреса объекта к идентификатору (совместимость ОС3) */
#  define objTO_ID(p)  (objid_t)p
#  define objID_TO_OBJECT(pClass,objId)  (objHEADER*)objId
#endif
#endif
 /* Описатель протоколируемой переменной */
typedef
volatile struct trace_variable {
  u_int  id;                     /* идентификатор              */
  void*  adr;                    /* адрес                      */
  u_int  type;                   /* тип переменной             */
  u_int  size;                   /* размер переменной          */
  u_int  size_record;            /* размер записи переменной   */
  u_int  arg;                    /* аргумент, передаваемый функции протоколирования */
  char   name[TRACE_NAME_MAX];   /* имя пер-ной/функции        */
} trcVAR_DESCRIPTION;

/* Описатель  событий пользователя процесса */
struct trace_evuser {
   int   id;
   int   send; /* флаг состоявшейся отсылки информации о событии (traceName) */
   char  name[TRACE_EVENT_NAME_MAX];
};

#define traceMAX_VARIABLES    32 /* предельное число переменных */

#define traceMAX_OBJECTS      32 /* предельное число объектов */

struct trace_descriptor;

struct ppe_trace
{
   u_int ppet_pid;               /* ID процесса                                        */
   volatile
   u_int ppet_varCount;          /* число протоколируемых переменных процесса          */
   volatile
   u_int ppet_varSize;           /* общая длина записей протоколирования переменных    */
   u_int ppet_varId;             /* идентификатор последней переменной                 */
//   u_int ppet_addr;             /* адрес события                                    */
   struct traceFilter
         ppet_filter;            /* события, при которых переменные протоколируются    */
   struct trace_variable         /* описатели  переменных процесса                     */
         ppet_variable[traceMAX_VARIABLES];
   u_int ppet_objCount;          /* число объектов процесса                            */
   struct trace_object           /* описатели  объектов процесса                       */
         ppet_object[traceMAX_OBJECTS];
   u_int ppet_evnCount;          /* число открытых событий процесса                    */
   struct trace_evuser           /* описатели  событий пользователя процесса           */
         ppet_events[TRACE_USER_EVENT_MAX+1];
   objid_t                       /* ресурс, которого охидает поток NEXT, ждущий        */
         ppet_objWait;           /* появления в буфере события                         */
   void *                        /* ресурс, которого охидает поток NEXT, ждущий        */
         ppet_objWaitJob;        /* появления в буфере события (вызов в планировщике)  */
   funcptr_t
         ppet_funcFree;          /* функция предоставления потоку NEXT ресурса         */
   vfuncptr_t                    /* функция предоставления потоку NEXT ресурса         */
         ppet_funcFreeJob;       /* (вызов в планировщике)                             */
   struct trace_descriptor*
         ppet_descriptor;        /* адрес описателя потока трассы                      */
   __uint32_t
         ppet_registration;      /* флаг вызова функции в момент регистрации события   */
   traceHEADBUF
         ppet_pBufHeader;
#if 0
   struct trace_region           /* адрес и размер буфера трассы                       */
         ppet_traceBufferRegion;
   struct prsmem_partition*
         ppet_prsmembuffer;      /* адрес описателя раздела сохраняемой памяти
                                                                      под буфер трассы */
   struct prsmem_partition*
         ppet_prsmemlegacy;      /* адрес описателя раздела сохраняемой памяти
                                                                        под завещание  */
#endif
//   int   ppet_tracesize;         /* размер памяти, выделенной под протоколирование     */
   int  ppet_init;               /* флаг инициализации                                 */
};

/* Структура, адрес которой передается функции, вызываемой при регистрации события */
typedef
struct tracefunc_arg {
   int               event;   /* регистрируемое событие              */
   trcHEADER_RECORD* phdr;    /* адрес заголовка записи события      */
   void*             buf;     /* адрес области, предоставляемой для
                                 данных, заносимых функцией протокол.*/
   int               size;    /* размер предоставляемой области      */
   void*             user;    /* указатель, задаваемый пользователем */
} traceFUNC_ARG;

#define traceSET_FUNCARG(p,e,h,b,s,u)  \
   {                                   \
      (p)->event = e;                  \
      (p)->buf = b;                    \
      (p)->size = s;                   \
      (p)->user = u;                   \
      (p)->phdr = h;                   \
   }

/* Проверить тип протоколируемой пер-ной */
#define traceVARIABLE_TYPE_VALIDATE(type) \
  (((type) >= traceTYPE_SHORT) && ((type) < traceTYPE_LAST))

#if OS_MAJOR == 2
extern struct ppe_trace       __ppeTrace;
#endif
u_int    traceSizeVariables( traceHEADBUF pBufHeader, int event );
int      traceSetUserEvent(  const char *event_name );
struct trace_evuser*
         traceGetUserEvent( trace_event_id_t event );
void     traceFPRegisters( void *buf, int size );
void     traceVarFPRegisters( traceFUNC_ARG* p );
void     traceBackTrace( void *buf, int size );
void     traceVarBackTrace( trcVAR_RECORD* pVar, int size );
void     traceVarRegistration( trcVAR_RECORD*  pVar, int size, void* adr );

#ifndef MAX_PERF
   int traceCheckObject( void* adr, const char* name, int class_type, objHEADER* pHdr );
#else
#  define traceCheckObject( adr, name, class_type, pHdr )
#endif

int traceProcessingAuto( pid_t pid );

#endif /*KERNEL*/

#endif /*_TRACE_TROBJCT_H*/
