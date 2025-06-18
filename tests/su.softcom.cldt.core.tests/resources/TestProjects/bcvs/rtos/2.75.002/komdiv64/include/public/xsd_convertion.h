#ifndef PUBLIC_XSD_CONVERTION_H_
#define PUBLIC_XSD_CONVERTION_H_

#include <arinc/error.h>

typedef int integer;

#ifndef __BFD_H_SEEN__
typedef int boolean;
#endif

typedef char string[30];
typedef unsigned int MemorySizeType;
typedef char   CALLBACK_TYPE[30];

#define false 0
#define true  1

#define Kb 1024
#define Mb 1024*1024

#define PORT_REF_TYPE             NAME_TYPE
#define REF_NAME_TYPE             NAME_TYPE
#define PARTITION_REF_TYPE        PARTITION_ID_TYPE
#define PARTITION_PORT_TYPE       PARTITION_TYPE
#define PARTITION_LIST_ITEM_TYPE  PARTITION_ID_TYPE
#define SCHEDULE_CHANGE_ACTION_TYPE PARTITION_ERROR_ACTION_TYPE


#define IGNORE 0
#define SHUTDOWN    MODULE_ERROR_ACTION_SHUTDOWN
#define RESET       MODULE_ERROR_ACTION_RESET

#define MODULE      ERROR_LEVEL_MODULE
#define PARTITION   ERROR_LEVEL_PARTITION
#define PROCESS     ERROR_LEVEL_PROCESS
typedef enum {
   DATA = 1,
   CODE = 2,
   PROCDATA = 4
} MEMORY_REQ_TYPE_TYPE;

#endif /* PUBLIC_XSD_CONVERTION_H_ */
