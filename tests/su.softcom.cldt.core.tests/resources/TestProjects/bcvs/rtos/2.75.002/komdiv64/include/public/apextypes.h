
/* sys/apextypes.h - Global constant and type definitions */

#ifndef PUBLIC_SYS_APEXTYPES_H
#define PUBLIC_SYS_APEXTYPES_H

#include <errno.h>
#include <sys/types.h>
#include <public/difference.h>
/*
 *---------------------------
 * Domain limits
 *---------------------------
 *
 * These values define the domain limits and are implementation dependent.
 */
#define SYSTEM_LIMIT_NUMBER_OF_PARTITIONS      32 /* module scope */
//#define SYSTEM_LIMIT_NUMBER_OF_MESSAGES       512 /* module scope */
//#define SYSTEM_LIMIT_MESSAGE_SIZE            8192 /* module scope */
//#define SYSTEM_LIMIT_NUMBER_OF_PROCESSES      128 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_SAMPLING_PORTS 512 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_QUEUING_PORTS  512 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_BUFFERS        256 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_BLACKBOARDS    256 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_SEMAPHORES     256 /* partition scope */
//#define SYSTEM_LIMIT_NUMBER_OF_EVENTS         256 /* partition scope */
/*
 * Base APEX types
 *
 * The actual size of these base types is system specific and the
 * sizes must match the sizes used by the implementation of the
 * underlying Operating System.
 */
typedef unsigned char APEX_BYTE;                 /* 8-bit unsigned                     */
typedef __uint16_t    APEX_UNSIGNED_SHORT;       /* 16-bit unsigned integer            */
typedef __int32_t     APEX_INTEGER;              /* 32-bit signed                      */
typedef __uint32_t    APEX_UNSIGNED;             /* 32-bit unsigned                    */
typedef int64_t       APEX_LONG_INTEGER;         /* 64-bit signed                      */
typedef double        APEX_FLOAT;

/*
 * General APEX types
 */
/* Коды возврата функций */
typedef
  enum {
      NO_ERROR = 0,                    /* request valid and operation performed        */
      NO_ACTION = EACTION,             /* status of system unaffected by request       */
      NOT_AVAILABLE = EAVAILABLE,      /* resource required by request unavailable     */
      INVALID_PARAM = EINVAL,          /* invalid parameter specified in request       */
      INVALID_CONFIG = ECONFIG,        /* parameter incompatible with configuration    */
      INVALID_MODE = EMODE,            /* request incompatible with current mode       */
      TIMED_OUT = ETIMEDOUT            /* time-out tied up with request has expired    */
  } RETURN_CODE_TYPE;

/* Максимальная длина имени */
#define MAX_NAME_LENGTH 30

typedef char              NAME_TYPE[MAX_NAME_LENGTH]; /* имя                           */
typedef void              (* SYSTEM_ADDRESS_TYPE);    /* адрес                         */
typedef int               STACK_SIZE_TYPE;
typedef APEX_BYTE *       MESSAGE_ADDR_TYPE;     /* адрес сообщения                    */
typedef APEX_INTEGER      MESSAGE_SIZE_TYPE;     /* размер сообщения                   */
typedef APEX_INTEGER      MESSAGE_RANGE_TYPE;    /* количество сообщений               */
typedef APEX_INTEGER      WAITING_RANGE_TYPE;    /*количество потоков, ждущих сообщений*/
typedef APEX_LONG_INTEGER SYSTEM_TIME_TYPE;      /* системное время                    */

/* Направление передачи данных */
typedef enum { SOURCE = 0, DESTINATION = 1 } PORT_DIRECTION_TYPE;
/* Дисциплина обслуживания очереди потоков */
typedef enum { FIFO = 0, PRIORITY = 1 }      QUEUING_DISCIPLINE_TYPE;
/* Годность сообщения */
typedef enum msg_validity { INVALID = 0, VALID = 1 }      VALIDITY_TYPE;

/* Бесконечное время ожидания */
#define INFINITE_TIME_VALUE -1LL

typedef int BOOLEAN;

typedef APEX_INTEGER  WINDOW_ID_TYPE;            /* идентификатор окна                 */
typedef APEX_INTEGER  PARTITION_ID_TYPE;         /* идентификатор процесса (partition) */
typedef NAME_TYPE     PARTITION_NAME_TYPE;       /* имя процесса (partition)           */
typedef APEX_INTEGER  LOCK_LEVEL_TYPE;           /* уровень блокировки                 */
typedef APEX_INTEGER  CHANNEL_ID_TYPE;
typedef NAME_TYPE     ENTRY_POINT_TYPE;
typedef NAME_TYPE     FILE_NAME_TYPE;
typedef NAME_TYPE     PROCESS_NAME_TYPE;         /* имя потока управления              */
typedef unsigned int  PROCESS_ID_TYPE;           /* идентификатор потока управления    */

//typedef unsigned int  QUEUING_PORT_ID_TYPE;      /* идентификатор порта с очередью     */
typedef NAME_TYPE     QUEUING_PORT_NAME_TYPE;    /* имя порта с очередью               */
//typedef unsigned int  SAMPLING_PORT_ID_TYPE;     /* идентификатор порта без очередей   */
typedef NAME_TYPE     SAMPLING_PORT_NAME_TYPE;   /* имя порта без очередей             */
typedef NAME_TYPE     QUEUING_PORT_LIST_NAME_TYPE; /* имя списка портов с очередью */

#endif /* PUBLIC_SYS_APEXTYPES_H */

