/* Тестирование системных событий */
#ifndef _PRIVATE_OSTEST_H
#define _PRIVATE_OSTEST_H

#include <public/difference.h>
#include <limits.h>
#if OS_MAJOR == 3
#  include <arinc/partition.h>
#else
//#  define   MAX_NUMBER_OF_PARTITIONS     1
#endif
#include <sys/types.h>
#include <sys/systm.h>
#if OS_MAJOR == 3
#  include <sys/proc.h>
#endif
#include <private/bindmodule.h>

#define oststEVENT_ID   traceFunction64   /* событие выполнения теста в трассе */

#define oststMAX_OBJECTS   5

#define oststMAX_CYCLE     10 /* ограничение на число циклов */

/* Идентификаторы тестов */
enum id_ostst
{
      oststQUEDDL_EXTRACT = 1,
      oststDDL_EXTRACT,
      oststDDL_INSERT,
      oststQUE_LIST,
      oststQUE_HEAD,            /* 5 */
      oststSDL_LIST,
      oststQUE_READY,
      oststQUE_CLOCK,
      oststTCB,
      oststSEM,                  /* 10 */
      oststSSEM,
      oststMTX,
      oststSMTX,
      oststCND,
      oststSCND,                 /* 15 */
      oststMQ,
      oststASEM,
      oststAMQ,
      oststAEVENT,
      oststABBRD,                /* 20 */
      oststQPORT,
      oststSPORT,
      oststNODE,
      oststUSER,             /* тест выполняется вызываемой функцией (oststUserFunction) */
      oststLAST
};

typedef struct ostst_mask {
   enum id_ostst oststmask[oststLAST];
}  oststset_t;

/* Структура передаваемых тесту параметров */
struct par_ostest {
   u_int    options;
   int      bufsize;       /* размер буфера, использованного в тесте (слов) */
   int      test;          /* id подтеста, в котором обнаружена ошибка      */
   u_int    restriction;   /* ограничение                                   */
   void*    obj1;
   void*    obj2;
};
/* Описатель теста */
struct s_ostest {
   /* Тестирующая функция */
   int         (*oststFunction)(void*, struct par_ostest*);
   int         (*oststFunctionStore)(void*, struct par_ostest*);
   int         id;         /* идентификатор теста                           */
   u_int       status;     /* текущее состояние теста:
                            1 байт - идентификатор теста
                            2 байт - идентификатор подтеста
                            3 байт - код ошибки теста/подтеста
                            4 байт - число ошибок, обнаруженных в тесте
                                                                            */
   int         maxbufsize; /* размер буфера,требуемого для протокола (слов) */
   u_int       options;    /* условия выполнения теста                      */
   u_int       restriction;/* ограничение (число проверяемых узлов/
                                           число проверяемых объектов       */
   void*       pClass;     /* адрес описателя класса объекта                */
   u_int       objCount;
   struct obj_ostest {     /* проверяемые объекты                           */
      u_int objId;         /* идентификатор объекта                         */
      u_int object;        /* адрес описателя                               */
      char  name[NAME_MAX];
   }  objects[oststMAX_OBJECTS];
   u_int       nTest;      /* число выполненных тестов                      */
   int         nActive;    /* номер в списке активных тестов                */
   int         debugError; /* идентификатор эмулируемой ошибки              */
};

/* Описатель тестов */
struct ss_ostest {
   int   init;       /* флаг готовности тестов к обслуживанию  */
   int   active;     /* флаг выполнения теста(ов)              */
   int   lock;       /* флаг блокирования выполнения теста(ов) */
   struct s_ostest   /* описатели отдельных тестов             */
         tests[oststLAST];
   struct s_ostest*  /* список активных тестов                 */
         active_tests[oststLAST];
   struct ostst_mask
         mask;       /* маска тестов                           */
};

enum err_ostst
{
      oststE_OWNER = 1, /* ошибка владельца                                      */
      oststE_FOUND,     /* ошибка поиска в списке узла                           */
      oststE_SIGNATURE, /* ошибка сигнатуры                                      */
      oststE_LIST,      /* ошибка связности списка                               */
      oststE_CNULL,     /* 5 постоянный элемент заголовка DDL не 0               */
      oststE_ADDRESS,   /* адрес имеет недопустимое значение                     */
      oststE_SEM,       /* ошибка в описателе семафора                           */
      oststE_MTX,       /* ошибка в описателе мьютекса                           */
      oststE_CND,       /* ошибка в описателе переменной условия                 */
      oststE_MQ,        /* 10 ошибка в описателе очереди сообщения               */
      oststE_TCB,       /* ошибка в описателе потока                             */
      oststE_CLOCK,     /* ошибка в описателе часов                              */
      oststE_READY,     /* ошибка в описателе очереди потоков READY              */
      oststE_AEVENT,    /* ошибка в описателе события                            */
      oststE_ASEM,      /* 15 ошибка в описателе семафора                        */
      oststE_AMQ,       /* ошибка в описателе буфера                             */
      oststE_ABBRD,     /* ошибка в описателе доски объявлений                   */
      oststE_QPORT,     /* ошибка в описателе порта                              */
      oststE_SPORT,     /* ошибка в описателе порта                              */
      oststE_TESTID,    /* 20 ошибка идентификатора теста                        */
      oststE_OBJECT,    /* ошибка задания объекта тестирования                   */
      oststE_BUFSIZE,   /* недостаточно места для вывода результатов             */
      oststE_ACCESS,    /* объект не доступен для проверки (очередь захвачена)   */
      oststE_NODE,      /* ошибка содержимого узла связи                         */
      oststE_SUPPORT    /* не поддерживается подсистема                          */
};
/* Опции проверки */
#define  OSTEST_OPT_ERROR     1     /* информировать только об ошибках                       */
#define  OSTEST_OPT_TRACE     2     /* результат теста выводить в трассу                     */
#define  OSTEST_OPT_SYSLOG    4     /* результат теста выводить в SYSLOG                     */
#define  OSTEST_OPT_QUELIST   8     /* проверять проверять связность списка DDL              */
#define  OSTEST_OPT_KPRINT    0x20  /* ррезультат теста выводить KPRINT                      */
#define  OSTEST_OPT_OWNER     0x40  /* проверить владельца                                   */
#define  OSTEST_OPT_QUEHEAD   0x80  /* проверить заголовок DDL (c_null)                      */
#define  OSTEST_OPT_QUE       0x100 /* выполнить проверку очереди                            */
#define  OSTEST_OPT_NODE      0x200 /* выполнить проверку узла связи                         */
#define  OSTEST_OPT_REBOOT    0x400 /* реакция на обнаружение ошибки - REBOOT                */
#define  OSTEST_OPT_SHUTDOWN  0x800 /* реакция на обнаружение ошибки - posix_trace_shutdown  */
#define  OSTEST_OPT_ACCESS    0x1000/* информировать о невыполнении теста                    */

#define oststOPT_DEFAULT   OSTEST_OPT_ERROR|OSTEST_OPT_TRACE|OSTEST_OPT_REBOOT

/* Содержимое буфера:
 * 1 слово - status теста,
 * Остальное содержимое определяется спецификой теста.
 * Структура слова status:
 *    1 байт - идентификатор теста,
 *    2 байт - идентификатор подтеста,
 *    3 байт - код ошибки теста/подтеста,
 *    4 байт - число ошибок, обнаруженных в тесте.
 */

/* Размер буфера (слов), для протоколирования теста. Не включает 1-ое слово. */
#define oststSIZE_QUEDDL_EXTRACT    8
#define oststSIZE_DDL_EXTRACT       6
#define oststSIZE_DDL_INSERT        6
#define oststSIZE_QUE_HEAD          7
#define oststSIZE_QUE_LIST          9
#define oststSIZE_SDL_LIST          5
#define oststSIZE_SEM               4
#define oststSIZE_MTX               8
#define oststSIZE_CND               3
#define oststSIZE_MQ                7
#define oststSIZE_TCB               7
#define oststSIZE_CLOCK             1 + 4
#define oststSIZE_QUEREADY          1
#define oststSIZE_TCBNODE           4

#define oststSIZE_ASEM              5
#define oststSIZE_AMQ               5
#define oststSIZE_AEVENT            3
#define oststSIZE_ABBRD             5
#define oststSIZE_QPORT             3
#define oststSIZE_SPORT             2

#define oststSIZE_USER              10

/* Первый параметр вызова oststSyaCall */
enum ostst_syscall
{
   oststSYSCALL_INIT = 1,  /* инициализация тестов процесса          */
   oststSYSCALL_ACTIVE,    /* у процесса есть выполняемые тесты      */
   oststSYSCALL_OBJECT,    /* добавить тестируемый объект            */
   oststSYSCALL_OBJNAME,   /* добавить тестируемый объект по имени   */
   oststSYSCALL_DEBUG,     /* вызов функций отладки                  */
   oststSYSCALL_LAST
};

#ifndef  MAX_PERF
/* Выполнение всех запрошенных тестов процесса */
#if OS_MAJOR == 3

#define oststCALL_PROCESS()
#define oststCHECK_OBJNAME(tstId,name,objId,pObj)
/*
#define oststCALL_PROCESS()                                                   \
   if( modLinkPage->mlp_Ostest.ostst_CallProcess)                             \
      modLinkPage->mlp_Ostest.ostst_CallProcess()
#define oststCHECK_OBJNAME(tstId,name,objId,pObj)                             \
   if( modLinkPage->mlp_Ostest.ostst_CheckObjectName)                         \
      modLinkPage->mlp_Ostest.ostst_CheckObjectName(tstId,name,objId,pObj)
*/
#else

#define oststCALL_PROCESS()
#define oststCHECK_OBJNAME(tstId,name,objId,pObj)
/*
#define oststCALL_PROCESS()                                                   \
   if( pOstestDescription && pOstestDescription->ostst_CallProcess )          \
      pOstestDescription->ostst_CallProcess()
#define oststCHECK_OBJNAME(tstId,name,objId,pObj)                             \
   if( pOstestDescription && pOstestDescription->ostst_CheckObjectName)       \
      pOstestDescription->ostst_CheckObjectName(tstId,name,objId,pObj)
*/
#endif
/* Выполнение теста без изменения потока управления. oststCallTest
 * возвращает EACTION, когда тест для указанного объекта не выполнялся
 */
#define oststCALL(id,obj1,obj2)                                               \
   (modLinkPage->mlp_Ostest.ostst_CallTest)?                                  \
         ((int (*)(enum id_ostst,void*,void*))modLinkPage->mlp_Ostest.ostst_CallTest)(id,obj1,obj2) : 0

/* Выполнение теста, в случае ошибки:
 *  выход из функции, в которой вызван тест без возврата значения
 */
#define oststCALL_RET_VOID(id,obj1,obj2)     \
   {                                         \
      int rt;                                \
      rt = oststCALL( id, obj1, obj2 );      \
      if( rt && (rt != EACTION) )            \
         return;                             \
   }
/* Выполнение теста, в случае ошибки:
 *  выход из функции, в которой вызван тест с возвратом значения
 */
#define oststCALL_RET_VAL(id,obj1,obj2,val)  \
   {                                         \
      int rt;                                \
      rt = oststCALL( id, obj1, obj2 );      \
      if( rt && (rt != EACTION) )            \
      return val;                            \
   }

/* Требуется выполнить тест/тесты? */
#define  oststIsExecute(p)                   \
   ( !p->lock && p->active )

#if OS_MAJOR == 2
struct _desc_ostest {
   funcptr_t  ostst_CallTest;       /* адрес функции, вызываемой для проверки объекта
                                     * (oststCallTest)                                          */
   vfuncptr_t ostst_CallProcess;    /* адрес функции, вызываемой для проверки объектов процесса
                                     * (oststCallProcess)                                       */
   funcptr_t  ostst_CheckObjectName;/* адрес функции, которая проверяет требуется ли вновь
                                     * созданный именованный объект тестировать
                                     * (oststCheckObjectName)                                   */
   void *t[MAX_NUMBER_OF_PARTITIONS];
};

   /* Включены средства подддежки? */
   #define  oststIsSupport()  pOstestDescription
#else
   /* Включены средства подддежки? */
   #define  oststIsSupport()  modLinkPage->mlp_Ostest.ostst_CallTest
#endif

/* Получить доступ к тесту */
void oststShowInit( void );
int  oststInit( void );
int  oststStart( void );
int  oststStop( void );
int  oststObject( enum id_ostst id, unsigned obj );
int  oststObjectName( enum id_ostst id, const char* name );
int  oststTestOptions( unsigned opt );
int  oststGetSize( enum id_ostst id, int* pSize );
/* Обработка маски системных тестов */
int  oststMaskEmpty( void );
int  oststMaskFill( void );
int  oststMaskAdd( enum id_ostst id );
int  oststMaskDel( enum id_ostst id );

int  oststSetRestriction( enum id_ostst id, void* restriction );
int  oststGetObjects( enum id_ostst id, struct obj_ostest** objects );

int  oststQueDdlExtractFunction( void* buf, struct par_ostest* parm );
int  oststDdlExtractFunction( void* buf, struct par_ostest* parm );
int  oststDdlInsertFunction( void* buf, struct par_ostest* parm );
int  oststSemFunction( void* buf, struct par_ostest* parm );
int  oststMtxFunction( void* buf, struct par_ostest* parm );
int  oststCndFunction( void* buf, struct par_ostest* parm );
int  oststMqFunction( void* buf, struct par_ostest* parm );
int  oststTcbFunction( void* buf, struct par_ostest* parm );
int  oststReadyFunction( void* buf, struct par_ostest* parm );
int  oststClockFunction( void* buf, struct par_ostest* parm );

int  oststASemFunction( void* buf, struct par_ostest* parm );
int  oststAEventFunction( void* buf, struct par_ostest* parm );
int  oststAMqFunction( void* buf, struct par_ostest* parm );
int  oststABbrdFunction( void* buf, struct par_ostest* parm );
int  oststQPortFunction( void* buf, struct par_ostest* parm );
int  oststSPortFunction( void* buf, struct par_ostest* parm );
int  oststUserFunction( void* buf, struct par_ostest* parm );

int  oststCheckObjectName( enum id_ostst oststId, const char* name, unsigned objId, void* pObj );

#else /* MAX_PERF */
#  define oststCALL_PROCESS()
#  define oststCALL(id,obj1,obj2)
#  define oststCALL_RET_VOID(id,obj1,obj2)
#  define oststCALL_RET_VAL(id,obj1,obj2,val)
#  define oststCHECK_OBJNAME(tstId,name,objId,pObj)
#endif /* MAX_PERF */

//extern int oststError;  /* в одном из тестов обнаружена ошибка */

int  oststLock( void );    /* запретить выполнение тестов */
int  oststUnlock( void );  /* разрешить выполнение тестов */

/* Инициирование выполнения теста */
int oststFunction( struct s_ostest* p, void* obj1, void* obj2 );
/* Инициирование выполнения теста из функции, выполняемой в событии трассы */
int oststFunctionEvent( void* buf, int bufsize, enum id_ostst id );
/* Вызов функции обслуживания */
int oststSysCall( enum ostst_syscall fId, void *arg1, void* arg2 );

int oststSetOptions( void );

#endif /* _PRIVATE_OSTEST_H */
