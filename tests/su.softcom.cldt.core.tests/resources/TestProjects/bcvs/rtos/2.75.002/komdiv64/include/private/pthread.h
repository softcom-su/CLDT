#ifndef _PRIVATE_PTHREAD_H
#define _PRIVATE_PTHREAD_H
struct thread_attr
{
    int     ta_init;
    size_t  ta_size;
    void   *ta_stackaddr;
    size_t  ta_stacksize;
    int     ta_contentionscope;
    int     ta_inheritsched;
    int     ta_schedpolicy;             /* политика планирования работы нити */
    struct  sched_param ta_schedparam;  /* параметры для планирования */
    int     ta_detachstate;
    int     ta_fpustate;                /* будет ли поток использовать сопроцессор */
    int     ta_activatestate;           /* будет ли поток активизирован после создания */
    int     ta_mpstate;                 /* использует ли поток общую память */
    int     ta_breakstate;              /* остановка в контрольных точках */
    int     ta_tracestate;              /* трассируемость потока */
    void   *ta_mempool;                 /* пул памяти потока */
    int     ta_system;                  /* поток системный */
    union {
        const char *nameptr;
        char        name[NAME_MAX+1];
    }       ta_name;
};
/*****************************************
 * Макросы для статической инициализации *
 ****************************************/

/* Мьютекса. Например:
 * pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 */
extern struct mtxControl __mutexInitializer;
#define PTHREAD_MUTEX_INITIALIZER &__mutexInitializer
/* Условной переменной. Например:
 * pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
 */
extern struct cndControl __condInitializer;
#define PTHREAD_COND_INITIALIZER &__condInitializer

/* Установка доступа нити к объектам в общей памяти */
int pthread_setmpstate( pthread_t  thread );
/* Получение значения атрибута использования нитью общей памяти */
int pthread_attr_getmpstate(
    const pthread_attr_t *attr, int *mpstate );

/* Установка значения атрибута использования нитью общей памяти */
int pthread_attr_setmpstate( pthread_attr_t *attr, int mpstate );



#endif
