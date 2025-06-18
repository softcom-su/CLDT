/* arinc/partition.h */
#ifndef _ARINC_PARTITION_H
#define _ARINC_PARTITION_H

#include <version.h>
#include <sys/apextypes.h>

/*
 *--------------------------------------------------------------------------
 * PARTITION constant and type definitions and management services
 *--------------------------------------------------------------------------
 */

/* Максимальное число разделов (partition) */
#if OS_MAJOR >= 3
#  define MAX_NUMBER_OF_PARTITIONS SYSTEM_LIMIT_NUMBER_OF_PARTITIONS
#endif

typedef  enum {
    SAMPLING_TRANSFER = 0,
    QUEUING_TRANSFER = 1
} TRANSFER_MODE_TYPE;

/* Режим работы (состояние) процесса */
typedef  enum {
    COLD_START = 1,          /* холодный старт (рестарт)                               */
    WARM_START = 2,          /* горячий старт (рестарт)                                */
    NORMAL = 3,              /* рабочий режим                                          */
    IDLE = 4                /* простой                                                */
} OPERATING_MODE_TYPE;

 /* Тип процесса, используемый при конфигурировании портов */
typedef enum {
   STANDARD_PARTITION,       /* стандартный (пользовательский или системный) процесс   */
   PSEUDO_PARTITION          /* псевдопроцесс                                          */
} PARTITION_TYPE;

typedef enum {
    LEVEL_A,
    LEVEL_B,
    LEVEL_C
} CRITICALITY_TYPE;

/* Способ запуска процесса */
typedef  enum {
    NORMAL_START         = 0,/* нормальный запуск (включение питания)                  */
    PARTITION_RESTART    = 1,/* рестарт процесса функцией SET_PARTITION_MODE()         */
    HM_MODULE_RESTART    = 2,/* рестарт модуля HM-монитором при обработке ошибки       */
    HM_PARTITION_RESTART = 3 /* рестарт процесса HM-монитором при обработке ошибки     */
} START_CONDITION_TYPE;

/* Описатель состояния процесса  */
typedef  struct partition_status {
    PARTITION_ID_TYPE    IDENTIFIER;             /* идентификатор процесса             */
    SYSTEM_TIME_TYPE     PERIOD;                 /* период                             */
    SYSTEM_TIME_TYPE     DURATION;               /* длина периода                      */
    LOCK_LEVEL_TYPE      LOCK_LEVEL;	         /* уровень блокировки                 */
    OPERATING_MODE_TYPE  OPERATING_MODE;         /* режим работы (состояние) процесса  */
    START_CONDITION_TYPE START_CONDITION;        /* режим запуска                      */
  } PARTITION_STATUS_TYPE;
typedef int SCHEDULE_ID_TYPE;
typedef NAME_TYPE SCHEDULE_NAME_TYPE;
/* Описатель состояния расписания окон */
typedef struct schedule_status {
   SYSTEM_TIME_TYPE TIME_OF_LAST_SCHEDULE_SWITCH;/* время последней смены расписания   */
   SCHEDULE_ID_TYPE CURRENT_SCHEDULE;            /* используемое сейчас расписание     */
   SCHEDULE_ID_TYPE NEXT_SCHEDULE;               /* следующее расписание               */
} SCHEDULE_STATUS_TYPE;

typedef NAME_TYPE module_name_type;
typedef int       module_id_type;

typedef struct _module_status_type {
   module_name_type  name;          /* имя модуля; */
   module_id_type    identifier;    /*  идентификатор модуля. */
} module_status_type;

#ifdef KERNEL
int  partSuspend( pid_t pid );
int  partResume(pid_t pid);
void partRestart(int mode, int StartCondition);
int  __partSetMode( int mode );
#else
#define SET_PARTITION_MODE(mode,ret)   \
    *(ret) = partSetMode(mode);
#define GET_PARTITION_STATUS(status,ret)   \
    *(ret) = partGetStatus(status);


#define SET_MODULE_SCHEDULE(schedule_id,ret) \
    *(ret) = partSetSchedule(schedule_id)
#define GET_MODULE_SCHEDULE_STATUS(status,ret) \
    *(ret) = partGetScheduleStatus(status)
#define GET_MODULE_SCHEDULE_ID(schedule_name,schedule_id,ret) \
    *(ret) = partGetScheduleId(schedule_name,schedule_id)

#define get_module_status(module_status,ret) \
    *(ret) = partGetModuleStatus(module_status)

extern int  partSetMode( int mode );
extern int  partGetStatus( struct partition_status* status );
void        partRestart(int mode, int StartCondition);
extern int  partSetSchedule(int  schedule_id);
extern int  partGetScheduleStatus(struct schedule_status *status);
extern int  partGetScheduleId(char *schedule_name, int *schedule_id);
extern int  partGetModuleStatus(module_status_type *module_status);
#endif

#endif /*_ARINC_PARTITION_H*/
