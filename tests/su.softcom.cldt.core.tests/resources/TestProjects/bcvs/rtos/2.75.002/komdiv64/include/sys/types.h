#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#if !defined(__ASSEMBLER__)

#include <public/difference.h>
#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef     unsigned char   u_char;
typedef     unsigned short  u_short;
typedef     unsigned int    u_int;
typedef     unsigned long   u_long;
typedef     unsigned char   unchar;
typedef     unsigned short  ushort;
typedef     unsigned int    uint;
typedef     unsigned long   ulong;
typedef     unsigned int    useconds_t;

/* SYSV_SOURCE */
typedef     unsigned char   uchar_t;
typedef     unsigned short  ushort_t;
typedef     unsigned int    uint_t;
typedef     unsigned long   ulong_t;
typedef     char *          caddr_t;
typedef     unsigned short  __uint16_t;
typedef     unsigned char   __uint8_t;
typedef     unsigned long   __uint32_t;
typedef     long            __int32_t;
typedef     int64_t         __int64_t;
#define QUAD_INT
typedef     uint64_t        u_quad_t;
typedef     long            daddr_t;    /* disk address */
typedef     int64_t         quad_t;
typedef     quad_t         *qaddr_t;
typedef     u_int32_t       fixpt_t;    /* fixed point number */
typedef     __int32_t       register_t;
typedef     quad_t          rlim_t;     /* resource limit */
typedef     int             module_t;
typedef     unsigned int    vm_offset_t;
typedef     __int64_t       vm_ooffset_t;
typedef     unsigned int    vm_pindex_t;
typedef     unsigned int    vm_size_t;

typedef     struct vm_page *vm_page_t;
typedef     int             boolean_t;

typedef     int             (*funcptr_t)()   ;
typedef     void            (*vfuncptr_t)()   ;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef     unsigned long long  system_time_t;

#define ARINC_TIME_MAX (system_time_t)0x7fffffffffffffffLL


#ifdef _MSC_VER
typedef     __int64             tick_t;
#else
typedef     long long           tick_t;
#endif /* _MSC_VER */


typedef     struct thread_attr  pthread_attr_t;
typedef     struct cond_attr    pthread_condattr_t;
struct cndControl;
typedef     struct cndControl   *pthread_cond_t;
typedef     unsigned int        pthread_key_t;
typedef     struct mutex_attr   pthread_mutexattr_t;
struct mtxControl;
typedef     struct mtxControl   *pthread_mutex_t;
typedef     struct thread_once  pthread_once_t;
typedef     struct thrdControl *pthread_t;
typedef     unsigned long       mpoffset_t;  /*смещение в общей памяти*/

typedef     objid_t             qport_t;
typedef     objid_t             sport_t;

/* POSIX_SOURCE */

struct mem_pool;
typedef     struct mem_pool *memPOOL_ID;

typedef     u_long dev_t;
typedef     u_long udev_t;

#define     dev2udev(a) (a)
#define     umajor(a) (a>>16)
#define     uminor(a) (a&0xffff)
#define     makeudev(major,minor) (((major)<<16)|(minor))


typedef     unsigned short  gid_t;
typedef     unsigned long   ino_t;
typedef     unsigned long   mode_t;
typedef     unsigned long   nlink_t;
typedef     long            off_t;
typedef     long            pid_t;
typedef     long            koff_t;

/* Fragment stddef.h 2.7.2.1 */
#ifndef _GCC_SIZE_T
#define _GCC_SIZE_T
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ long unsigned int
#endif
typedef __SIZE_TYPE__ size_t;
#endif  /* _GCC_SIZE_T */

typedef     long            ssize_t;
typedef     unsigned short  uid_t;

typedef int  clock_t;
typedef int  clockid_t;
typedef long time_t;
typedef int  timer_t;
typedef struct
{
  void    *ma_sem;
  unsigned ma_type;
}  sem_t;
struct mqpPxControl;
typedef struct mqpPxControl * mqd_t;

typedef struct trace_attribute    trace_attr_t;
typedef struct trace_descriptor  *trace_id_t;
typedef int                       trace_event_id_t;
typedef struct traceFilter        trace_event_set_t;


typedef int bus_space_handle_t; /* bus space handle */
typedef int bus_space_tag_t;    /* bus space tag */
typedef int bus_size_t;

typedef int  rune_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
/*
 * Useful stuff.
 */
#define __CONCAT(a,b)   a ## b

#endif /*  !defined(__ASSEMBLER__) */
#endif /* _SYS_TYPES_H */

