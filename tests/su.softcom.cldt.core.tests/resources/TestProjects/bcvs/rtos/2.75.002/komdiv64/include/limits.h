/* limits.h */

#ifndef _LIMITS_H
#define _LIMITS_H

#include <arch/cputypes.h>
#if  CPU == I80386 
#include <arch/i386/limits.h>
#endif
#if  CPU == R3000 || CPU == R4000
#include <arch/mips/limits.h>
#endif


#define _POSIX_AIO_LISTIO_MAX           2
#define _POSIX_AIO_MAX                  1
#define _POSIX_ARG_MAX                  4096
#define _POSIX_CLOCKRES_MIN             20000000
#define _POSIX_DELAYTIMER_MAX           32
#define _POSIX_HOST_NAME_MAX            255
#define _POSIX_LINK_MAX                 8 
#define _POSIX_MAX_CANON                255
#define _POSIX_MAX_INPUT                255
#define _POSIX_MQ_OPEN_MAX              8
#define _POSIX_MQ_PRIO_MAX              32
#define _POSIX_NAME_MAX                 14
#define _POSIX_OPEN_MAX                 20
#define _POSIX_PATH_MAX                 255
#define _POSIX_PIPE_BUF                 512
#define _POSIX_RTSIG_MAX                8
#define _POSIX_SEM_NSEMS_MAX            256
#define _POSIX_SEM_VALUE_MAX            32767
#define _POSIX_SIGQUEUE_MAX             32
#define _POSIX_SSIZE_MAX                32767
#define _POSIX_STREAM_MAX               8
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS  4
#define _POSIX_THREAD_KEYS_MAX          128
#define _POSIX_THREAD_THREADS_MAX       64
#define _POSIX_TIMER_MAX                32
#define _POSIX_TRACE_EVENT_NAME_MAX     30
#define _POSIX_TRACE_NAME_MAX           8
#define _POSIX_TRACE_SYS_MAX            8
#define _POSIX_TRACE_USER_EVENT_MAX     32
#define _POSIX_TTY_NAME_MAX             9
#define _POSIX_TZNAME_MAX               6
#define _XOPEN_IOV_MAX                  16

#define ARG_MAX                         _POSIX_ARG_MAX

#define AIO_LISTIO_MAX                  16
#define AIO_MAX                         _POSIX_AIO_MAX
#define AIO_PRIO_DELTA_MAX              0

#define ATEXIT_MAX                      32
#define DELAYTIMER_MAX                  _POSIX_DELAYTIMER_MAX
#define HOST_NAME_MAX                   _POSIX_HOST_NAME_MAX
/*#define IOV_MAX                         _XOPEN_IOV_MAX*/
#define	LINK_MAX                        _POSIX_LINK_MAX

#ifndef MQ_OPEN_MAX
#define MQ_OPEN_MAX                     _POSIX_MQ_OPEN_MAX
#endif

#define MQ_PRIO_MAX                     _POSIX_MQ_PRIO_MAX
#define NAME_MAX                        80

#ifndef OPEN_MAX
#define OPEN_MAX                        _POSIX_OPEN_MAX
#endif

#define PATH_MAX                        _POSIX_PATH_MAX
#define PIPE_BUF                        _POSIX_PIPE_BUF
#define PTHREAD_DESTRUCTOR_ITERATIONS   _POSIX_THREAD_DESTRUCTOR_ITERATIONS
#define PTHREAD_KEYS_MAX                _POSIX_THREAD_KEYS_MAX
/* Минимально допустимый размер стека потока в байтах */
#define PTHREAD_STACK_MIN               CPU_THREAD_STACK_MIN
#define RTSIG_MAX                       _POSIX_RTSIG_MAX

#ifndef SEM_NSEMS_MAX
#define SEM_NSEMS_MAX                   _POSIX_SEM_NSEMS_MAX
#endif
#define SEM_VALUE_MAX                   _POSIX_SEM_VALUE_MAX
#define SIGQUEUE_MAX                    _POSIX_SIGQUEUE_MAX
#ifndef STREAM_MAX
#define STREAM_MAX                      20
#endif
#ifndef TIMER_MAX
#define TIMER_MAX                       _POSIX_TIMER_MAX
#endif

#define TRACE_EVENT_NAME_MAX            32 /*_POSIX_TRACE_EVENT_NAME_MAX*/
#define TRACE_NAME_MAX                  32 /*_POSIX_TRACE_NAME_MAX*/
#define TRACE_SYS_MAX                   1
#define TRACE_USER_EVENT_MAX            _POSIX_TRACE_USER_EVENT_MAX
#define TTY_NAME_MAX                    NAME_MAX
#define TZNAME_MAX                      _POSIX_TZNAME_MAX



#define CLOCK_MAX               4

#define MB_LEN_MAX                      1

/* параметры представления чисел */
#if 1
#define CHAR_BIT                        8
#define SCHAR_MAX                       127
#define SCHAR_MIN                       (-128)
#define UCHAR_MAX                       255
#ifdef __CHAR_UNSIGNED__
# define CHAR_MIN	                      0
# define CHAR_MAX	                      UCHAR_MAX
#else
# define CHAR_MIN	                      SCHAR_MIN
# define CHAR_MAX	                      SCHAR_MAX
#endif
#define SHRT_MAX                        32767
#define SHRT_MIN                        (-32768)
#define USHRT_MAX                       65535
#define INT_MAX                         2147483647
#define INT_MIN                         (-2147483647 - 1)
#define UINT_MAX                        ( INT_MAX * 2U + 1 )

#define WORD_BIT                        32

#if __WORDSIZE__ == 64
# define LONG_BIT                       64
# define LONG_MAX	                      9223372036854775807L
# define ULONG_MAX	                    18446744073709551615UL
#else
# define LONG_BIT                       32
# define LONG_MAX	                      2147483647L
# define ULONG_MAX	                    4294967295UL
#endif
# define LONG_MIN	                      (-LONG_MAX - 1L)

#define LLONG_MAX	                      9223372036854775807LL
#define LLONG_MIN	                      (-LLONG_MAX - 1LL)
#define ULLONG_MAX	                    18446744073709551615ULL

#if !defined(_ANSI_SOURCE)
#define	SSIZE_MAX	INT_MAX		/* max value for a ssize_t */

#if !defined(_POSIX_SOURCE)
#define	SIZE_T_MAX	UINT_MAX	/* max value for a size_t */

/* GCC requires that quad constants be written as expressions. */
#define	UQUAD_MAX	((u_quad_t)0-1)	/* max value for a uquad_t */
					/* max value for a quad_t */
#define	QUAD_MAX	((quad_t)(UQUAD_MAX >> 1))
#define	QUAD_MIN	(-QUAD_MAX-1)	/* min value for a quad_t */

#endif /* !_POSIX_SOURCE */
#endif /* !_ANSI_SOURCE */

#endif

#define LOCALE_NAME_MAX         30
#define LOCALE_PARTNAME_MAX     20

#endif /* _LIMITS_H*/

