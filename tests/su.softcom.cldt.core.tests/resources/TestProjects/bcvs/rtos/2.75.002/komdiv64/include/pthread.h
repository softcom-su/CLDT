/*  pthread.h  */

#ifndef _PTHREAD_H
#define _PTHREAD_H

#include <sched.h>
#include <limits.h>
#include <sys/objclasses.h>
#include <private/pthread.h>

#define pthread_mutexattr_default NULL
#define pthread_condattr_default NULL
#define pthread_attr_default NULL

/* Атрибут мютекса, определяющий приоритет захватившего его потока*/
enum mutexproto_t {
    PTHREAD_PRIO_NONE,
    PTHREAD_PRIO_INHERIT,
    PTHREAD_PRIO_PROTECT
};
/* Атрибут мютекса, определяющий его поведение при lock*/
enum {
  PTHREAD_MUTEX_ERRORCHECK,
  PTHREAD_MUTEX_RECURSIVE,
  PTHREAD_MUTEX_NORMAL
};
#define PTHREAD_MUTEX_DEFAULT PTHREAD_MUTEX_ERRORCHECK

enum {
  PTHREAD_PROCESS_PRIVATE,
  PTHREAD_PROCESS_SHARED
};


/* Переменной используемой для динамической пакетной инициализации.
 * static pthread_once_t once = PTHREAD_ONCE_INIT; */

#define PTHREAD_ONCE_INIT  { 1, 0, PTHREAD_MUTEX_INITIALIZER }

/* Макрос определяющий код завершения работы нити
   при её принудительном анулировании */

#define PTHREAD_CANCELED    ((void *)0xFFFFFFFF)

/******************************************************************
 * Параметры атрибутов, определяющих свойства нити при её создании*
 *****************************************************************/

/* Связывание одной нити с другой */
#define PTHREAD_CREATE_JOINABLE 0 /* связанная нить */
#define PTHREAD_CREATE_DETACHED 1 /* развязанная нить */

/* Наследование параметров планирования */
#define PTHREAD_INHERIT_SCHED   0 /* наследовать параметры от родительской нити */
#define PTHREAD_EXPLICIT_SCHED  1 /* использовать значения из структуры атрибутов при создании */

/* Свойства нити для планировщика */
#define PTHREAD_SCOPE_PROCESS 0 /* область планирования нити - процесс */
#define PTHREAD_SCOPE_SYSTEM  1 /* область планирования нити система */

/* 18.2.2 Возможность принудительного завершения нити */
#define PTHREAD_CANCEL_ENABLE  0 /* Удаление нити разрешено */
#define PTHREAD_CANCEL_DISABLE 1 /* Удаление нити запрещено */

/* 18.2.2 Типы завершения нити */
#define PTHREAD_CANCEL_DEFERRED 0 /* Отложенное до момента обращения к pthread_testcancel */
#define PTHREAD_CANCEL_ASYNCHRONOUS 1 /* Асинхронное, в любой момент времени */

/* Дополнительный флаг возможность выполнения операций с плавающей арифметики */
#define PTHREAD_FPU_ENABLE  0 /* Нить может работать с сопроцессором */
#define PTHREAD_FPU_DISABLE 1 /* Работа с сопроцессором запрещена */

/* Дополнительный флаг устанавливающий должна ли нить быть активизирована при создании */
#define PTHREAD_CREATE_ACTIVE 0    /* Нить будет активизирована при создании */
#define PTHREAD_CREATE_SUSPENDED 1 /* Нить не будет активизирована, для её запуска надо выполнить RESUME */
#define PTHREAD_CREATE_DORMANT   2 /* Нить не будет активизирована, для её запуска надо выполнить START */

/* Дополнительный флаг использования нитью общей памяти */
#define PTHREAD_MP_ENABLE  0 /* Нить может использовать MP */
#define PTHREAD_MP_DISABLE 1 /* Нить не использует MP */

/* Дополнительный флаг поведения нити в точке останова */
#define PTHREAD_BREAK_ENABLE  0 /* Нить может останавливаться */
#define PTHREAD_BREAK_DISABLE 1 /* Нить не может останавливаться */

/* Дополнительный флаг допустимости трассировки нити */
#define PTHREAD_TRACE_ENABLE  1 /* Нить может трассироваться */
#define PTHREAD_TRACE_DISABLE 2 /* Нить не может трассироваться */


/*****************************************
 *  Функции атрибутов создания мютексов  *
 *****************************************/
#define GETCONDNAME(attr) ((attr)->ca_size<sizeof(*attr)?(attr)->ca_name.nameptr:(attr)->ca_name.name)
struct cond_attr {
    int         ca_init;
    size_t      ca_size;
    clockid_t   ca_clockid;
    int         ca_shared;
    union {
        const char *nameptr;
        char        name[NAME_MAX+1];
    } ca_name;
};
#define GETMTXNAME(attr) ((attr)->ma_size<sizeof(*attr)?(attr)->ma_name.nameptr:(attr)->ma_name.name)

struct mutex_attr {
    u_long      ma_init;
    size_t      ma_size;
    u_long      ma_prioceiling;            /* приоритет                                 */
    int         ma_protocol;               /* протокол                                  */
    int         ma_type;
    int         ma_shared;
    union {
        const char *nameptr;
        char        name[NAME_MAX+1];
    }           ma_name;
};
//!!Проверка неадекватная -- что имеется ввиду?
#define GETTHRNAME(attr) ((attr)->ta_size<sizeof(*attr)?(attr)->ta_name.nameptr:(attr)->ta_name.name)

struct othread_attr
{
    int         ta_init;
    int         ta_size;
    void       *ta_stackaddr;
    size_t      ta_stacksize;
    int         ta_contentionscope;
    int         ta_inheritsched;
    int         ta_schedpolicy;            /* политика планирования                     */
    struct sched_param
                ta_schedparam;             /* параметры планирования                    */
    int         ta_flags;
    objid_t     ta_mempool_id;             /* пул памяти потока                         */
    union {
        const char *nameptr;
        char        name[NAME_MAX+1];
    }           ta_name;
};

/* Структура thread_once определяет тип pthread_once_t */
struct thread_once
{
    int    to_init;
    int    to_init_executed;
    pthread_mutex_t to_mutex;
};



/*****************************************
 *  Функции атрибутов создания мютексов  *
 *****************************************/

int pthread_mutexattr_init_s(   pthread_mutexattr_t *attr,size_t size );
#define pthread_mutexattr_init( attr ) pthread_mutexattr_init_s(attr,sizeof(pthread_mutexattr_t))

int pthread_mutexattr_destroy( pthread_mutexattr_t *attr );

int pthread_mutexattr_getpshared( const pthread_mutexattr_t *attr, int *pshared );

int pthread_mutexattr_setpshared( pthread_mutexattr_t *attr, int pshared );

int pthread_mutexattr_settype( pthread_mutexattr_t *attr, int type );

int pthread_mutexattr_gettype( const pthread_mutexattr_t *attr, int *type );


/* 13.6.1 Mutex Initialization Scheduling Attributes */

int pthread_mutexattr_setprotocol( pthread_mutexattr_t *attr, int protocol );

int pthread_mutexattr_getprotocol( const pthread_mutexattr_t *attr, int *protocol );

int pthread_mutexattr_setprioceiling( pthread_mutexattr_t *attr, int prioceil );

int pthread_mutexattr_getprioceiling( const pthread_mutexattr_t *attr, int *prioceil );

/************************************
 *  Функции для работы с мьютексами  *
 ************************************/

int pthread_mutex_init( pthread_mutex_t *mutex, const pthread_mutexattr_t *attr );

int pthread_mutex_destroy( pthread_mutex_t *mutex );

int pthread_mutex_lock( pthread_mutex_t *mutex );

int pthread_mutex_trylock( pthread_mutex_t *mutex );

int pthread_mutex_timedlock(pthread_mutex_t *mutex,
                  const struct timespec *abs_timeout);
int pthread_mutex_unlock( pthread_mutex_t *mutex );


int pthread_mutex_setprioceiling( pthread_mutex_t *mutex,
    int prioceiling, int  *old_ceiling );

int pthread_mutex_getprioceiling( pthread_mutex_t *mutex,
    int *prioceiling );





/***********************************************************
 *  11.4.1 Функции атрибутов создания условных переменных  *
 ***********************************************************/

int pthread_condattr_init_s( pthread_condattr_t *attr,size_t size );
#define pthread_condattr_init( attr ) pthread_condattr_init_s(attr,sizeof(pthread_condattr_t))

int pthread_condattr_destroy( pthread_condattr_t *attr );

int pthread_condattr_getpshared( const pthread_condattr_t *attr, int *pshared );

int pthread_condattr_setpshared( pthread_condattr_t *attr, int pshared );

int pthread_condattr_getname(pthread_condattr_t *attr, char *name);

int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock_id);
int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);

/************************************************
 *  Функции для работы с условными переменными  *
 ************************************************/

int pthread_cond_init( pthread_cond_t *cond, const pthread_condattr_t *attr );

int pthread_cond_destroy( pthread_cond_t *mutex );

/* 11.4.3 Broadcasting and Signaling a Condition */

int pthread_cond_signal( pthread_cond_t *cond );

int pthread_cond_broadcast( pthread_cond_t *cond );

/* 11.4.4 Waiting on a Condition */

int pthread_cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex );

int pthread_cond_timedwait( pthread_cond_t *cond, pthread_mutex_t *mutex,
    const struct timespec *abstime );


/********************************************************
 *  17.1 Функции для работы с Thread-Specific даннымии  *
 ********************************************************/

/* 17.1.1 Создание ключа для Thread-Specific данного */
int pthread_key_create( pthread_key_t *key, void (*destructor)( void * ) );

/* 17.1.3 Удаление ключа Thread-Specific данного */
int pthread_key_delete( pthread_key_t key );

/* 17.1.2 Занесение значения  Thread-Specific данного */
int pthread_setspecific( pthread_key_t key, const void *value );

/* 17.1.2 Получение значения  Thread-Specific данного */
void *pthread_getspecific( pthread_key_t key );
/********************************************
 *  16.2.1 Функции атрибутов создания нити  *
 ********************************************/

/* Инициализация атрибутов умолчательными значениями */
int pthread_attr_init_s( pthread_attr_t  *attr,size_t size );
#define pthread_attr_init( attr ) pthread_attr_init_s( attr,sizeof(pthread_attr_t) )

/* Уничтожение атрибутов */
int pthread_attr_destroy( pthread_attr_t  *attr );

/* Получение размера стека */
int pthread_attr_getstacksize( const pthread_attr_t *attr,
    size_t *stacksize );

/* Установка размера стека */
int pthread_attr_setstacksize( pthread_attr_t *attr,
    size_t stacksize );

/* Получение адреса начала стека */
int pthread_attr_getstackaddr( const pthread_attr_t *attr,
    void **stackaddr );

/* Установка адреса начала стека */
int pthread_attr_setstackaddr( pthread_attr_t *attr,
    void *stackaddr );

/* Получение значения атрибута связанности нити */
int pthread_attr_getdetachstate( const pthread_attr_t *attr,
    int *detachstate );

/* Установка значения атрибута связанности нити */
int pthread_attr_setdetachstate( pthread_attr_t *attr,
    int detachstate );

/* Получение значения атрибута доступности сопроцессора */
int pthread_attr_getfpustate( const pthread_attr_t *attr,
    int *fpustate );

/* Установка значения атрибута доступности сопроцессора */
int pthread_attr_setfpustate( pthread_attr_t *attr,
    int fpustate );

/* Получение значения атрибута активизации потока при создании */
int pthread_attr_getactivatestate( const pthread_attr_t *attr,
    int *activatestate );

/* Установка значения атрибута активизации потока при создании */
int pthread_attr_setactivatestate( pthread_attr_t *attr,
    int activatestate );

/* Получение значения атрибута прерывания выполнения */
int pthread_attr_getbreakstate(
    const pthread_attr_t *attr, int *state );

/* Установка значения атрибута прерывания выполнения */
int pthread_attr_setbreakstate( pthread_attr_t *attr, int state );

/* Получение значения атрибута трассировки */
int pthread_attr_gettracestate(
    const pthread_attr_t *attr, int *state );

/* Установка значения атрибута трассировки */
int pthread_attr_settracestate( pthread_attr_t *attr, int state );

/* Атрибуты, для планировщика */

/* Установка параметров планирования */
int pthread_attr_setschedparam( pthread_attr_t *attr,
        const struct sched_param  *param );

/* Получение параметров планирования */
int pthread_attr_getschedparam( const pthread_attr_t *attr,
    struct sched_param    *param );



int pthread_attr_setscope( pthread_attr_t *attr,
    int contentionscope );

int pthread_attr_getscope( const pthread_attr_t *attr,
    int *contentionscope );

int pthread_attr_setinheritsched( pthread_attr_t *attr,
    int inheritsched );

int pthread_attr_getinheritsched( const pthread_attr_t *attr,
    int *inheritsched );

int pthread_attr_setschedpolicy( pthread_attr_t *attr,
    int policy );

int pthread_attr_getschedpolicy( const pthread_attr_t *attr,
    int *policy );

/* Функция pthread_attr_getmempool - опрос атрибута "пул памяти" потока управления */
int pthread_attr_getmempool( const pthread_attr_t *attr, objid_t *pool );
/* Функция pthread_attr_setmempool - установка атрибута "пул памяти" потока управления */
int pthread_attr_setmempool( pthread_attr_t *attr, objid_t pool );


/*****************************
 *  Функции работы с нитями  *
 *****************************/


/* 16.2.2 Создание нити */
int pthread_create( pthread_t *thread,
  const pthread_attr_t   *attr,
  void                 *(*start_routine)( void * ),
  void                   *arg );

/* 16.2.3 Связывание одной нити с другой,
   текущая нить приостанавливается до завершения другой */
int pthread_join( pthread_t   thread, void  **value_ptr );

/* 16.2.4 Перевод нити в категорию несвязанных */
int pthread_detach( pthread_t thread );

/* 16.2.5 Завершение работы нити */
void pthread_exit( void *value_ptr );

/* 16.2.6 Получение идентификатора текущей нити */
pthread_t pthread_self( void );

/* 16.2.7 Сравнение идентификаторов нитей */
int pthread_equal( pthread_t t1, pthread_t t2 );

/* 16.2.8 Динамическая пакетная инициализация */
int pthread_once( pthread_once_t *once_control, void (*init_routine)(void) );
/*  once_control - статическая переменная, проинициализированная
 *  значением  PTHREAD_ONCE_INIT;  */

/* 18.2.1 Принудительное завершение нити */
int pthread_cancel( pthread_t  thread );

/* 18.2.2 Установка состояния допустимости принудительного завершения текущей нити */
int pthread_setcancelstate( int state, int *oldstate );

/* 18.2.2 Установка для текущей нити типа её принудительного завершения */
int pthread_setcanceltype( int  type, int *oldtype );

/* 18.2.2 Определяет точку где допускается принудительное завершение нити */
void pthread_testcancel( void );

/* 18.2.3 Обработчики завершения работы нити */

typedef struct thread_cleanup_handler
{
    struct thread_cleanup_handler *m_next_handler;
    void   (*m_routine)( void * );
    void   *m_arg;
} threadCLEANUP_HANDLER;

extern void threadCleanupPush (threadCLEANUP_HANDLER *handler);
extern threadCLEANUP_HANDLER *threadCleanupPop (void);

/* Макрос pthread_cleanup_push() - регистрация функции свертывания в стеке */
#define pthread_cleanup_push( rtn, arg ) \
    { \
        threadCLEANUP_HANDLER _thread_stack_handler; \
        _thread_stack_handler.m_routine = (void(*)(void *))(rtn); \
        _thread_stack_handler.m_arg = (void *)(arg); \
        threadCleanupPush (&_thread_stack_handler);

/* Макрос pthread_cleanup_pop() - извлечение функции свёртывания из стека */
#define pthread_cleanup_pop( execute ) \
        threadCleanupPop(); \
        if ( execute ) (*_thread_stack_handler.m_routine)( _thread_stack_handler.m_arg ); \
    }

/*  Текущие атрибуты планирования работы нити */

/* Получение текущих атрибутов планирования нити */
int pthread_getschedparam( pthread_t thread,
        int *policy, struct sched_param *param );

/* Установка атрибутов планирования нити */
int pthread_setschedparam( pthread_t thread,
        int policy, struct sched_param *param );

/* Установка приоритета планирования нити */
int pthread_setschedprio( pthread_t thread,	int prio );






#ifndef _POSIX_SOURCE

extern funcptr_t arincInitThreadRtn;

/* осуществлена инициализация поддержки ARINC? */
#define arincIS_INITIALIZE (arincInitThreadRtn != NULL)

#define PTHREAD_SUSPEND_ENABLE	0 /* Приостановка нити разрешена */
#define PTHREAD_SUSPEND_DISABLE 1 /* Приостановка нити запрещена */
/*
void pthread_testsuspend( void );
int pthread_setsuspendstate( int  state, int *oldstate );
*/
/* Установка значения атрибута системного потока */
#define PTHREAD_CREATE_USER     0 /* нить пользовательская */
#define PTHREAD_CREATE_SYSTEM   1 /* нить системная */
int pthread_attr_setsystem( pthread_attr_t *attr, int state );

#define PTHREAD_CREATE_NORMAL       0
#define PTHREAD_CREATE_ERRHANDLER   1 /* error handler */
int pthread_attr_seterrhandler( pthread_attr_t *attr, int state );

//#define PTHREAD_CREATE_POSIX         0
//#define PTHREAD_CREATE_ARINC         1
int pthread_attr_setproperties( pthread_attr_t *attr, int state );
int pthread_attr_setname( pthread_attr_t *attr, const char *name );
int pthread_attr_getname( pthread_attr_t *attr, char *name );
int pthread_mutexattr_setname( pthread_mutexattr_t *attr, const char *name );
int pthread_mutexattr_getname( pthread_mutexattr_t *attr, char *name );
int pthread_condattr_setname( pthread_condattr_t *attr, const char *name );
int pthread_condattr_getname( pthread_condattr_t *attr, char *name );


int pthread_attr_getstack(const pthread_attr_t * attr, void **stackaddr, size_t * stacksize);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);

#define PTHREAD_TYPE_SYSTEM     0
#define PTHREAD_TYPE_USER       1

#endif /* _POSIX_SOURCE */


#endif

