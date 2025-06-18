#ifndef _HEALTH_MONITOR_H
#define _HEALTH_MONITOR_H
#include <public/errrep.h>
#include <private/dataset.h>

/* Состояния системы */
typedef enum {
   STATE_INIT_PARTOS  =     1,   /* инициализация процесса операционной системой */
   STATE_INIT_PARTUSER,          /* инициализация процесса прикладной программой */
   STATE_INTERRUPT_HANDLER,      /* обработчик прерываний                        */

/* Выполнение функций ОС, влияющих на работу нескольких процессов  */
   STATE_OS_MOD,
/* Выполнение функций ОС, влияющих на работу только одного (текущего) процесса         */
   STATE_OS_PART,
   STATE_ERROR_HANDLER,          /* поток обработчик ошибок                      */

   STATE_USER,                   /* поток управления                             */
   STATE_MAX
} SYSTEM_STATE_TYPE;


/* НЕЛЬЗЯ  МЕНЯТЬ ПОРЯДОК (см. syscall.S) */
typedef enum {
/* Причина вызова Health Monitor     */
   CALL_CAUSE_NORMAL = 0,              /* ошибка, найденная средствам ОС               */
   CALL_CAUSE_EXCEPTION,               /* исключительная ситуация                      */
   CALL_CAUSE_GOOD_SYSCALL,            /* syscall                                      */
   CALL_CAUSE_APPLICATION,             /* вызов функции RAISE_APPLICATION_ERROR        */
   CALL_CAUSE_INVALID_SYSCALL          /* ошибочный syscall                            */
} CALL_CAUSE_TYPE;

#ifndef __ASSEMBLER__
extern const struct error_codes {
   CERROR_CODE_TYPE ec_CEID;           /* код ошибки, используемый при конфигурировании*/
   int              ec_signo;          /* номер сигнала                                */
   int              ec_sicode;         /* код причины сигнала                          */
} const __errCodes[];


#include <sys/types.h>

//typedef int ErrorIDType;

 /*------ Структура health_monitor_param - структура параметров Health Monitor'a ------*/
struct health_monitor_param {
    struct thread_control   *hmp_Thread;     /* - адрес потока управления
                                                                       0 текущий поток */
    CALL_CAUSE_TYPE   hmp_CallCause;   /* call_cause  - причина вызова:
                                        CALL_CAUSE_EXCEPTION   - исключительная ситуация;
                                        CALL_CAUSE_SYSCALL     - ошибочный syscall;
                                        CALL_CAUSE_NORMAL      - ошибка, найденная ОС;
                                        CALL_CAUSE_APPLICATION - вызов функции
                                                              RAISE_APPLICATION_ERROR; */
    SYSTEM_STATE_TYPE hmp_SystemState; /* состояние системы                            */
    CERROR_CODE_TYPE  hmp_CEID;        /* идентификатор ошибки
                                                 (используемый при конфигурировании)   */
    const char       *hmp_Message;     /* сообщение                                    */
    int               hmp_VecNum;      /* номер вектора прерывания                     */
    void             *hmp_Regs;
};

void HealthMonitor( const struct health_monitor_param *hmp );
/*----- Макрос HEALTH_MONITOR_D - заполнение структуры параметров Health Monitor'a ----*/
#define HEALTH_MONITOR_CALL(thread, call_cause, system_state, error_id, message ) { \
    struct health_monitor_param hmp = {                  \
        .hmp_Thread           = (thread),                \
        .hmp_CallCause        = (call_cause),            \
        .hmp_SystemState      = (system_state),          \
        .hmp_CEID             = (error_id),              \
        .hmp_Message          = (message),               \
        .hmp_Regs             = 0};                      \
    HealthMonitor(&hmp);                                 \
}
typedef void (*hm_callback_t)(int,int);

/* Описатель callback-функции        */
struct hm_callback {
   const char    *hc_name;             /* имя callback-функции                         */
   hm_callback_t  hc_func;             /* указатель на callback-функцию                */
};

#define HM_CALLBACK(a) \
static void a (int system_state, int error_id); \
static struct hm_callback __ct ##a = { #a, a }; \
DATA_SET(hm_callbacks, __ct ##a ); \
static void a (int system_state, int error_id)



int hmInit(int prio);
void hmReportError(const char *action, const char *error_level,
      const struct health_monitor_param *hmp);
struct thread_control;
void *cpuGetFailedAddress(struct thread_control *t);
void cpuPrintRegs( struct thread_control *t );
struct pt_regs;
void cpuPrintRegsR( struct pt_regs *r );
void *cpuGetCallSyscallAddress(struct thread_control *t);
#endif
#endif /* !_HEALTH_MONITOR_H */
