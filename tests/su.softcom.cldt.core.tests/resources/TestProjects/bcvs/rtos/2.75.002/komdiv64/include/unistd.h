/*
 *    Copyright  1998
 *    UNISTD.H
 */

#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/types.h>

#define _POSIX_VERSION    200112L
/*
#define _POSIX2_VERSION   200112L
*/

#define _POSIX_ASYNCRONOUS_IO                 _POSIX_VERSION
/* #define _POSIX_FSYNC */
#define _POSIX_JOB_CONTROL                    _POSIX_VERSION
#define _POSIX_MESSAGE_PASSING                _POSIX_VERSION
#define _POSIX_NO_TRUNC                       -1

#define _POSIX_PRIORITIZED_IO                 _POSIX_VERSION
#define _POSIX_PRIORITY_SCHEDULING            _POSIX_VERSION
#define _POSIX_REALTIME_SIGNALS               _POSIX_VERSION
#define _POSIX_SEMAPHORES                     _POSIX_VERSION
#define _POSIX_TIMERS                         _POSIX_VERSION
#define _POSIX_THREADS                        _POSIX_VERSION

#define _POSIX_THREAD_ATTR_STACKADDR          _POSIX_VERSION
#define _POSIX_THREAD_ATTR_STACKSIZE          _POSIX_VERSION
#define _POSIX_THREAD_PRIORITY_SCHEDULING     _POSIX_VERSION

#define _POSIX_THREAD_PRIO_INHERIT            _POSIX_VERSION
#define _POSIX_THREAD_PRIO_PROTECT            _POSIX_VERSION

#define _POSIX_TIMEOUTS                       _POSIX_VERSION

#define _POSIX_THREADS                        _POSIX_VERSION
#define _POSIX_TRACE                          _POSIX_VERSION
#define _POSIX_TRACE_LOG                      _POSIX_VERSION
#define _POSIX_TRACE_EVENT_FILTER             _POSIX_VERSION

#define _POSIX_VDISABLE                       0

/* константы, определяемые для функции sysconf() */
#define SYSCONF_MIN   1
enum {
  _SC_2_C_BIND = SYSCONF_MIN,
  _SC_2_C_DEV,
  _SC_2_CHAR_TERM,
  _SC_2_FORT_DEV,
  _SC_2_FORT_RUN,
  _SC_2_LOCALEDEF,
  _SC_2_PBS,
  _SC_2_PBS_ACCOUNTING,
  _SC_2_PBS_CHECKPOINT,
  _SC_2_PBS_LOCATE,
  _SC_2_PBS_MESSAGE,
  _SC_2_PBS_TRACK,
  _SC_2_SW_DEV,
  _SC_2_UPE,
  _SC_2_VERSION,
  _SC_ADVISORY_INFO,
  _SC_AIO_LISTIO_MAX,
  _SC_AIO_MAX,
  _SC_AIO_PRIO_DELTA_MAX,
  _SC_ARG_MAX,
  _SC_ASEM_INIT_MAX,
  _SC_ASYNCHRONOUS_IO,
  _SC_ATEXIT_MAX,
  _SC_BARRIERS,
  _SC_BBRD_INIT_MAX,
  _SC_BBRD_BUF_MAX,
  _SC_BC_BASE_MAX,
  _SC_BC_DIM_MAX,
  _SC_BC_SCALE_MAX,
  _SC_BC_STRING_MAX,
  _SC_BUFFERS_MAX,
  _SC_CHILD_MAX,
  _SC_CLC_TCK,
  _SC_CLOCK_SELECTION,
  _SC_CONDITION_VARIABLES,
  _SC_CND_INIT_MAX,
  _SC_SCND_INIT_MAX,
  _SC_COLL_WEIGHTS_MAX,
  _SC_CPUTIME,
  _SC_DELAYTIMER_MAX,
  _SC_ARINC_EVENTS_MAX,
  _SC_EXPR_NEST_MAX,
  _SC_FILE_LOCKING,
  _SC_FSYNC,
  _SC_GETGR_R_SIZE_MAX,
  _SC_GETPW_R_SIZE_MAX,
  _SC_HOST_NAME_MAX,
  _SC_HSHTABLES_MAX,
  _SC_SHSHTABLES_MAX,
  _SC_IOV_MAX,
  _SC_IPV6,
  _SC_JOB_CONTROL,
  _SC_LINE_MAX,
  _SC_LOGIN_NAME_MAX,
  _SC_MAPPED_FILES,
  _SC_MEMLOCK,
  _SC_MEMLOCK_RANGE,
  _SC_MEMORY_PROTECTION,
  _SC_MEMPOOL_CHECK,
  _SC_MEMPOOL_MAX,
  _SC_SMEMPOOL_MAX,
  _SC_MESSAGE_PASSING,
  _SC_MONOTONIC_CLOCK,
  _SC_MQ_OPEN_MAX,
  _SC_MQ_PRIO_MAX,
  _SC_MQ_ATTR,
  _SC_MTX_INIT_MAX,
  _SC_SMTX_INIT_MAX,
  _SC_MULTI_PROCESS,
  _SC_NGROUPS_MAX,
  _SC_NO_TRUNC,
  _SC_OBJNAMES_MAX,
  _SC_OBJSNAMES_MAX,
  _SC_OPEN_MAX,
  _SC_PAGE_SIZE,
  _SC_PAGESIZE,
  _SC_PRIORITIZED_IO,
  _SC_PRIORITY_SCHEDULING,
  _SC_RAW_SOCKETS,
  _SC_RE_DUP_MAX,
  _SC_READER_WRITER_LOCKS,
  _SC_REALTIME_SIGNALS,
  _SC_REGEXP,
  _SC_REGEX_VERSION,
  _SC_RTSIG_MAX,
  _SC_SAVED_IDS,
  _SC_SEM_NSEMS_MAX,
  _SC_SSEM_NSEMS_MAX,
  _SC_SEM_VALUE_MAX,
  _SC_SEMAPHORES,
  _SC_SHARED_MEMORY_OBJECTS,
  _SC_SHELL,
  _SC_SIGQUEUE_MAX,
  _SC_SPAWN,
  _SC_SPIN_LOCKS,
  _SC_SPORADIC_SERVER,
  _SC_SPORT_MAX,
  _SC_SPORT_BUFFERS,
  _SC_QPORT_MAX,
  _SC_QPORTLIST_MAX,
  _SC_QPORTSAP_MAX,
  _SC_MPCHANNEL_MAX,
  _SC_DEADLINE_START,
  _SC_SS_REPL_MAX,
  _SC_STREAM_MAX,
  _SC_SYMLOOP_MAX,
  _SC_SYNCHRONIZED_IO,
  _SC_THREAD_ATTR_STACKADDR,
  _SC_THREAD_ATTR_STACKSIZE,
  _SC_THREAD_CPUTIME,
  _SC_THREAD_DESTRUCTOR_ITERATIONS,
  _SC_THREAD_KEYS_MAX,
  _SC_THREAD_PRIO_INHERIT,
  _SC_THREAD_PRIO_PROTECT,
  _SC_THREAD_PRIORITY_SCHEDULING,
  _SC_THREAD_PROCESS_SHARED,
  _SC_THREAD_SAFE_FUNCTIONS,
  _SC_THREAD_SPORADIC_SERVER,
  _SC_THREAD_STACK_MIN,
  _SC_THREAD_THREADS_MAX,
  _SC_THREADS,
  _SC_TIMEOUTS,
  _SC_TIMER_MAX,
  _SC_TIMERS,
  _SC_TRACE,
  _SC_TRACE_EVENT_FILTER,
  _SC_TRACE_EVENT_NAME_MAX,
  _SC_TRACE_INHERIT,
  _SC_TRACE_LOG,
  _SC_TRACE_NAME_MAX,
  _SC_TRACE_SYS_MAX,
  _SC_TRACE_USER_EVENT_MAX,
  _SC_TTY_NAME_MAX,
  _SC_TYPED_MEMORY_OBJECTS,
  _SC_TZNAME_MAX,
  _SC_V6_ILP32_OFF32,
  _SC_V6_ILP32_OFFBIG,
  _SC_V6_LP64_OFF64,
  _SC_V6_LPBIG_OFFBIG,
  _SC_XBS5_ILP32_OFF32,
  _SC_XBS5_ILP32_OFFBIG,
  _SC_XBS5_LP64_OFF64,
  _SC_XBS5_LPBIG_OFFBIG,
  _SC_XOPEN_CRYPT,
  _SC_SYSLOGD_PRIO,
  _SC_SYSLOG_BUF_SIZE,
  _SC_SYSLOG_BUF_COUNT,
  _SC_LOGBOOK_MAX,
  _XOPEN_ENH_I18N,
  _XOPEN_LEGACY,
  _XOPEN_REALTIME,
  _XOPEN_REALTIME_THREADS,
  _XOPEN_SHM,
  _XOPEN_STREAMS,
  _XOPEN_UNIX,
  _XOPEN_VERSION,
  _SC_VERSION
};
#define SYSCONF_MAX   _SC_VERSION
#define sysconfIS_VALID(par) \
  (((par) >= SYSCONF_MIN) && ((par) <= SYSCONF_MAX))

/* константы, определяемые для функции confstr() */
#define CONFSTR_MIN   SYSCONF_MAX + 1
enum {
  _CS_PATH = CONFSTR_MIN,
  _CS_POSIX_V6_ILP32_OFF32_CFLAGS,
  _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,
  _CS_POSIX_V6_ILP32_OFF32_LIBS,
  _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,
  _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,
  _CS_POSIX_V6_ILP32_OFFBIG_LIBS,
  _CS_POSIX_V6_LP64_OFF64_CFLAGS,
  _CS_POSIX_V6_LP64_OFF64_LDFLAGS,
  _CS_POSIX_V6_LP64_OFF64_LIBS,
  _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,
  _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,
  _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,
  _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS
};
#define CONFSTR_MAX   _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS
#define sconfstrIS_VALID(par) \
  (((par) >= CONFSTR_MIN) && ((par) <= CONFSTR_MAX))

/* константы, определяемые для функции pathconf() */
#define PATHCONF_MIN   CONFSTR_MAX + 1
#include <private/unistd.h>
#define PATHCONF_MAX   _PC_SYNC_IO
#define pathconfIS_VALID(par) \
  (((par) >= PATHCONF_MIN) && ((par) <= PATHCONF_MAX))


#define STDIN_FILENO    0   /* standard input file descriptor */
#define STDOUT_FILENO   1   /* standard output file descriptor */
#define STDERR_FILENO   2   /* standard error file descriptor */

#ifdef __cplusplus
extern "C" {
#endif

extern char **environ; /*переменные окружения*/

#ifdef _LARGEFILE64_SOURCE
#define off_t off64_t
#define lseek lseek64
#define truncate  truncate64
#define ftruncate ftruncate64
#endif

int           access( const char *path, int mode );
unsigned int  alarm( unsigned int seconds );
int           chdir ( const char * path );
int           close( int filedes );
int           dup( int fildes );
int           dup2( int fildes, int fildes2 );
void          _exit( int status) ;
int           fdatasync( int fildes );
long          fpathconf( int filedes, int name );
int           fsync( int fildes );

char         *getcwd( char * buf, size_t size );
gid_t         getegid( void );
uid_t         geteuid( void );
gid_t         getgid( void );
int           gethostname( char *name, size_t namelen );
pid_t         getpid( void );
uid_t         getuid( void );

int           link ( const char * existing, const char * newn );
off_t         lseek( int filedes, off_t offset, int whence );
long          pathconf( const char *path, int name );
int           pause( void );
int           pipe( int fildes[2] );
ssize_t       read( int filedes, void *buf, size_t nbyte );
unsigned int  sleep( unsigned int seconds );
void          sync( void );
long          sysconf( int name );
int           unlink ( const char * path );
ssize_t       write( int filedes, const void *buf, size_t nbyte );
char         *ttyname(int fd);
int           ttyname_r(int fd, char *buf, size_t buflen);

int           truncate (const char *path, off_t length);
int           ftruncate(int fildes, off_t length);

int           getopt(int argc, char * const argv[], const char *optstring);
extern        char *optarg;
extern int    optind, opterr, optopt;


#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
#define X_OK    1       /* Test for execute permission.  */
#define F_OK    0       /* Test for existence.  */

#ifdef __cplusplus
}
#endif

#endif  /* _UNISTD_H */
