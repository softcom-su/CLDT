/* prsmem/prsmemprv.h - поддержка сохраняемой памяти */

#ifndef _PRSMEM_PRSMEM_H
#define _PRSMEM_PRSMEM_H

#if OS_MAJOR == 3
#  include <sys/proc.h>
#endif
#include <arch/endian.h>

/* Максимальное число разделов */
#define PRSMEM_MAX_PATITION   8

#if BYTE_ORDER == BIG_ENDIAN
   /* Сигнатура */
#  define PRSMEM_SIGNATURE   ('p'<<3*8)+('R'<<2*8)+('s'<<1*8)+'M' /* pRsM */
#else
#  define PRSMEM_SIGNATURE   ('M'<<3*8)+('s'<<2*8)+('R'<<1*8)+'p'
#endif

#  define prsmem_t            unsigned
#if OS_MAJOR == 2
#  define PRSMEM_ALIGN        1024*8
#else
#  define PRSMEM_ALIGN        4
#endif


#define PRSMEM_ALLOC_AUTO  1        /* областиь сохранения размещена автоматически  */
#define PRSMEM_ALLOC_MP    2        /* областиь сохранения размещена в общей памяти */
#define PRSMEM_ALLOC_ADR   3        /* областиь сохранения указана в конфигураторе  */

enum prsmemOpenFlags {
   prsmemFLAGS_EMPTY = 1, /* очистить раздел сохраняемой области    */
   prsmemFLAGS_FORMAT= 2  /* записи форматировать                   */
};

/* Флаги состояния раздела */
#define prsmemSTATE_EMPTY            0x0001 /* рвздел памяти пуст                   */
#define prsmemSTATE_FORMAT           0x0002 /* записи в разделе форматированы       */
#define prsmemSTATE_EMEMORY          0x0004 /* не получена память под раздел        */
#define prsmemSTATE_OPEN             0x0008 /* раздел открыт                        */
#define prsmemSTATE_ESIGN_START      0x0100 /* сигнатура не найдена в начале раздела
                                               области сохранения                   */
#define prsmemSTATE_ESIGN_END        0x0200 /* сигнатура не найдена в конце раздела
                                               области сохранения                   */
/* Идентификаторы вызываемых через SYSCALL функций */
enum prsmemSysCallEnum {
   prsmemSYSCALL_OPEN,
   prsmemSYSCALL_EMPTY,   /* очистить сохраняемую область               */
   prsmemSYSCALL_PUT,     /* положить данные в сохраняемую область      */
   prsmemSYSCALL_GET,     /* получить данные из сохраняемой области     */
   prsmemSYSCALL_REWIND,  /* вернуться для чтения в начало области      */
   prsmemSYSCALL_STATUS,  /* дать сведения о сохраняемой области        */
   prsmemSYSCALL_LSEEK,   /* позиционироваться в  сохраняемой области   */
   prsmemSYSCALL_LAST
};

/* Структура параметров вызова функции prsmemSysCall  */
struct prsmem_status;
struct st_prsmemSysCallPar {
  enum prsmemSysCallEnum    funId;      /* идентификатор функции                        */
  prsmem_t                  id;
  union {
      struct {
          pid_t                    pid;
          const char*              name;       /* имя раздела сохраняемой памяти               */
          enum prsmemOpenFlags     flags;
      } open;
      struct {
          void*                    addr;
          size_t                   size;
      } move;
      struct {
          off_t                    offset;
          int                      whence;
      } lseek;
      struct {
          struct prsmem_status*    status;
      } status;
  } fun;
};

/* Входные параметры конфигурирования области сохранения */
struct prsmem_partconf {
  int                  active;
  pid_t                pid;
  const char*          name;           /* имя                              */
  long                 size;           /* размер                           */
};

/* Входные параметры конфигурирования области сохранения */
struct prsmem_config {
   int      pc_allocation;
   void    *pc_addres;
   struct prsmem_partconf const *pc_parts;
   /* Функция запроса общей памяти, указатель инициализируется, если
    * ППМ поддерживает общую память
    */
   int      (*pc_shmem)(int cpu, int type, volatile void** ptr, size_t* size);
};

/* Состояние раздела области сохранения */
typedef
struct prsmem_status {
//  void*  prsms_start;         /* адрес начала данных раздела                         */
  size_t prsms_size;          /* общий размер раздела                         */
  size_t prsms_size_effect;   /* доступный размер раздела                     */
  size_t prsms_size_tail;     /* свободно                                     */
  u_int  prsms_state;         /* информация о состоянии                       */
  size_t prsms_maxsize;       /* наибольший размер записи                     */
  size_t prsms_nRec;          /* число записей                                */
} prsmemSTATUS;

/* Заголовок раздела области сохранения */
typedef
struct prsmem_partition {
  u_int  prsmp_signature;     /* сигнатура                                    */
  u_int  prsmp_start_offset;  /* адрес начала данных раздела                  */
  size_t prsmp_size;          /* общий размер раздела                         */
  size_t prsmp_size_effect;   /* доступный размер раздела                     */
  u_int  prsmp_id;            /* идентификатор                                */
  const char*
         prsmp_name;
  pid_t  prsmp_pid;
  u_int  prsmp_state;         /* информация о состоянии                       */
  u_int  prsmp_open;          /* счетчик открытий                             */
  size_t prsmp_size_tail;     /* свободно                                     */
  u_int  prsmp_current_offset;/* текущее положение                            */
  size_t prsmp_maxsize;       /* наибольший размер записи                     */
  size_t prsmp_nRec;          /* число записей                                */
} prsmemPART;

/* Заголовок области сохранения */
typedef
struct prsmem_main {
  u_int  prsm_signature;      /* сигнатура                                    */
  u_int  prsm_allocation;     /* способ размещения                            */
  void*  prsm_start;          /* указатель на начало области сохранения       */
  size_t prsm_size;           /* общий размер сохраняемой области             */
  u_int  prsm_state;          /* информация о состоянии                       */
  u_int  prsm_partitions;     /* число разделов                               */
  struct prsmem_partition*    /* адреса описателей разделов                   */
         prsm_partition[PRSMEM_MAX_PATITION];
  const struct prsmem_config*
         prsm_config;         /* адрес конфигурации                           */
  struct prsmem_partition     /* предыдущее состояние описателей разделов     */
         prsm_oldpartition[PRSMEM_MAX_PATITION];
} prsmemMAIN;

/* Заголовок записи в завещании */
typedef
struct prsmem_record {
   u_short  recsize;    /* размер записи                 */
   u_short  size;       /* размер данных в записи        */
} prsmemHDRREC;

static __inline__ void *__prsmemPartAddress( prsmemPART* pPart, int offset )
{
   return (char *)pPart + offset;
}

#if OS_MAJOR == 2
int  prsmemGetInfo( prsmemMAIN* pMain );
#endif

int   _prsmemInit( pid_t pid );
int   _prsmemPartOpen( struct st_prsmemSysCallPar* pParams );
void  prsmemShowInit( void );
int   prsmemPartFind(pid_t pid, const char* name, prsmemPART** pPart);
int   prsmemPartOnId( prsmem_t id, prsmemPART** pPart );
void  prsmemPhysGet(pid_t pid, unsigned long long *pa, int *size);
int   prsmemGetSize(pid_t pid);

struct module_description;
int   prsmemPhysInit( struct module_description *md );
const struct prsmem_config*
      prsmemGetConfig(void);
prsmemMAIN*
      prsmemGetAddressByPid( pid_t pid );

//int   prsmemSysCall( enum prsmemSysCallEnum fId, void *arg1, void* arg2, void* arg3 );
int   prsmemSysCall( struct st_prsmemSysCallPar* pParams );

int   prsmemPartWrite( prsmem_t id, void* addr, size_t size );
int   prsmemPartRead( prsmem_t id, void* addr, size_t* size );
int   prsmemPartLseek( prsmem_t id, off_t offset, int whence );
int   prsmemPartRewind( prsmem_t id );
int   prsmemPartStatus( prsmem_t id, prsmemSTATUS* status );
int   prsmemPartEmpty( prsmem_t id );
int   prsmemPartOpen( const char* name, prsmem_t* pId, enum prsmemOpenFlags flags );

#endif   /* _PRSMEM_PRSMEM_H */
