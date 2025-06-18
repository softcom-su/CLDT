/* arinc/error.h */
#ifndef _ARINC_ERROR_H
#define _ARINC_ERROR_H

#include <sys/apextypes.h>

/*
 *--------------------------------------------------------------------------
 * ERROR constant and type definitions and management services
 *--------------------------------------------------------------------------
 */
/* Максимальный размер сообщения об ошибке */
#define MAX_ERROR_MESSAGE_SIZE 64

typedef APEX_INTEGER ERROR_MESSAGE_SIZE_TYPE;            /* размер сообщения об ошибке */
typedef APEX_BYTE ERROR_MESSAGE_TYPE[MAX_ERROR_MESSAGE_SIZE];   /* сообщение об ошибке */

/* Коды ошибок пользовательских потоков в соответствии с ARINC 653 */
typedef
  enum {
    DEADLINE_MISSED   = 0,             /* нарушение сроков                             */
    APPLICATION_ERROR = 1,             /* ошибка, обнаруженная прикладной программой   */
    NUMERIC_ERROR     = 2,             /* ошибка при выполнении арифметических действий*/
    ILLEGAL_REQUEST   = 3,             /* недопустимое обращение к ОС                  */
    STACK_OVERFLOW    = 4,             /* переполнение стека                           */
    MEMORY_VIOLATION  = 5,             /* нарушение защиты памяти,                     */
                                       /* попытка выполнения привилегированной команды */
    HARDWARE_FAULT    = 6,             /* аппаратная ошибка                            */
    POWER_FAIL        = 7              /* сбой питания                                 */
  } ERROR_CODE_TYPE;

/* Уровни ошибок                               */
typedef
  enum {
     ERROR_LEVEL_MODULE,               /* ошибка уровня модуля                         */
     ERROR_LEVEL_PARTITION,            /* ошибка уровня процесса                       */
     ERROR_LEVEL_PROCESS               /* ошибка уровня потока управления              */
  } ERROR_LEVEL_TYPE;

/* Способы обработки ошибок уровня модуля      */
typedef
  enum {
    MODULE_ERROR_ACTION_IGNORE = 0,    /* игнорировать ошибку                          */
    MODULE_ERROR_ACTION_SHUTDOWN,      /* останов модуля                               */
    MODULE_ERROR_ACTION_RESET,         /* рестарт модуля                               */
  } MODULE_ERROR_ACTION_TYPE;
/* Способы обработки ошибок уровня процесса    */
typedef
  enum {
    PARTITION_ERROR_ACTION_IGNORE = 0,   /* игнорировать ошибку                          */
    PARTITION_ERROR_ACTION_COLD_START=1, /* холодный (ре)старт процесса                  */
    PARTITION_ERROR_ACTION_WARM_START=2, /* горячий (ре)старт процесса                   */
    PARTITION_ERROR_ACTION_IDLE = 4      /* останов процесса                             */
  } PARTITION_ERROR_ACTION_TYPE;

/* Описатель ошибки - структура, используемая потоком обработкиком ошибок
           для получения информации об ошибке (заполняеется функцией GET_ERROR_STATUS) */
typedef
  struct error_status {
    ERROR_CODE_TYPE         ERROR_CODE;          /* код ошибки                         */
    ERROR_MESSAGE_TYPE      MESSAGE;             /* сообщение об ошибке                */
    ERROR_MESSAGE_SIZE_TYPE LENGTH;              /* размер сообщения об ошибки         */
    PROCESS_ID_TYPE         FAILED_PROCESS_ID;   /* идентификатор ошибочного потока    */
    SYSTEM_ADDRESS_TYPE     FAILED_ADDRESS;      /* адрес команды, вызвавшей ошибку    */
  } ERROR_STATUS_TYPE;


#define CREATE_ERROR_HANDLER(p,sz,ret) \
  *(ret) = aerrorHandlerCreate(p,sz)
#define REPORT_APPLICATION_MESSAGE(msg,sz,ret) \
  *(ret) = aerrorApplicationMessage(msg,sz)
#define GET_ERROR_STATUS(status,ret) \
  *(ret) = aerrorGetStatus(status)
#define RAISE_APPLICATION_ERROR(code,msg,sz,ret) \
  *(ret) = aerrorApplicationRaise(code,msg,sz)

#ifndef KERNEL
extern int aerrorHandlerCreate( SYSTEM_ADDRESS_TYPE startPoint, STACK_SIZE_TYPE stacksize );
extern int aerrorGetStatus( ERROR_STATUS_TYPE  *status );
extern int aerrorApplicationRaise( ERROR_CODE_TYPE ercode, MESSAGE_ADDR_TYPE msg, ERROR_MESSAGE_SIZE_TYPE length );
extern int aerrorApplicationMessage( MESSAGE_ADDR_TYPE msg, ERROR_MESSAGE_SIZE_TYPE length );
#else
extern int __aerrorHandlerCreate( SYSTEM_ADDRESS_TYPE startPoint, STACK_SIZE_TYPE stacksize );
extern int __aerrorGetStatus( ERROR_STATUS_TYPE  *status );
extern int __aerrorApplicationRaise( ERROR_CODE_TYPE ercode, MESSAGE_ADDR_TYPE msg, ERROR_MESSAGE_SIZE_TYPE length );
extern int __aerrorApplicationMessage( MESSAGE_ADDR_TYPE msg, ERROR_MESSAGE_SIZE_TYPE length );
#endif

#endif  /*_ARINC_ERROR_H*/

