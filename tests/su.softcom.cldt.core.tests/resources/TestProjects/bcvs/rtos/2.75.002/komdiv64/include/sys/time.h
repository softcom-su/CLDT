/* sys/time.h */
#ifndef  _SYS_TIME_H
#define  _SYS_TIME_H

#define CLOCK_REALTIME              0
#define CLOCK_PROCESS_CPUTIME_ID    1 /* часы процессорного времени процесса */
#define CLOCK_THREAD_CPUTIME_ID     2 /* часы процессорного времени потока управления */
#define CLOCK_MONOTONIC            -1

#define TIMER_ABSTIME         1
#define TIMER_TICKTIME        2

#include <strings.h>
#include <sys/types.h>
#include <sys/signal.h>

struct timeval
{
   time_t tv_sec; /* секунды */
   long tv_usec; /* микросекунды */
};

struct timespec
{
   time_t tv_sec; /* секунды */
   long tv_nsec; /* наносекунды */
};

struct itimerspec
{
   struct timespec it_interval;
   struct timespec it_value;
};

struct tm
{
   int tm_sec;
   int tm_min;
   int tm_hour;
   int tm_mday;
   int tm_mon;
   int tm_year;
   int tm_wday; /* дней от воскресенья (0-6)  */
   int tm_yday; /* дней от начала года (0-365)*/
   int tm_isdst;
};

#ifndef TIMEZONE_DEFINED
#define TIMEZONE_DEFINED
struct timezone
{
   int tz_minuteswest; /* minutes west of Greenwich */
   int tz_dsttime; /* type of dst correction */
};
#endif

#define SECSPERMIN      60
#define MINSPERHOUR     60
#define HOURSPERDAY     24
#define DAYSPERWEEK     7
#define DAYSPERNYEAR    365
#define DAYSPERLYEAR    366
#define SECSPERHOUR     (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY      ((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR     12

#define TM_SUNDAY       0
#define TM_MONDAY       1
#define TM_TUESDAY      2
#define TM_WEDNESDAY    3
#define TM_THURSDAY     4
#define TM_FRIDAY       5
#define TM_SATURDAY     6

#define TM_JANUARY      0
#define TM_FEBRUARY     1
#define TM_MARCH        2
#define TM_APRIL        3
#define TM_MAY          4
#define TM_JUNE         5
#define TM_JULY         6
#define TM_AUGUST       7
#define TM_SEPTEMBER    8
#define TM_OCTOBER      9
#define TM_NOVEMBER     10
#define TM_DECEMBER     11

#define TM_YEAR_BASE    1900

#define EPOCH_YEAR      1970
#define EPOCH_WDAY      TM_THURSDAY

#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

#define ASCTIME_BUFFER          "DAY MON DD HH:MM:SS YYYY\n\0" /*используемый в asctime*/
#define GET_ZONENAME( fl )    tzname[ (fl)? 1 : 0]
/*стандартное смещение к UTC*/
#define GET_ZONE_STD_OFFSET(poff)   {tzset();  *(poff)=loc_time_pnt->stdoffset;}

struct timeLocale
{
   char *daysNames[2 * DAYSPERWEEK];char *monthsNames[2*MONSPERYEAR];
   char *frmDateTime;
   char *frmDate;
   char *frmTime;
   char *am;
   char *pm;
   /*
    char *zoneName[3];
    char *Isdst[2];
    */
};
extern struct timeLocale timeLocale;

extern int calcDaysFromEpoch(int year, int day);
extern int julday(int year, int mon, int day);
extern int timeToTm(const time_t timer, struct tm *tmtime);
extern int timeLocaleOffset(struct tm *tptr, long *offset);
extern int timeGetWeekDay(int year, int yday);
extern void timeSetLocaleInfo(char *lang);

#define   FD_SETSIZE   256
typedef long fd_mask;
#define NFDBITS   (sizeof(fd_mask) * NBBY)   /* bits per mask 32 */
#define   howmany(x, y)   (((x)+((y)-1))/(y))
#define   NBBY   8      /* number of bits in a byte */

#ifdef KERNEL

#include <private/ecb.h>
#include <private/objcore.h>
#include <private/ddlist.h>

/*
 * Used to maintain information about processes that wish to be
 * notified when I/O becomes possible.
 */

typedef struct
{
   objHEADER sn_obj;
#define SN_DRV	1
   volatile int sn_flags;
   ddlNODE sn_local; /* Нода для включения в список selscan      */
   ddlNODE sn_drv; /* Нода для включения в список к драйверу   */
   ecbNODE *sn_ecb; /* То, на что нужно выдать post             */
} selNODE;


struct selscan {
   ddlHEAD ss_list;
   ecbNODE ss_node;
};

struct selinfo
{
   ddlHEAD si_list; /* Список структур selNODE */
};
#define siInit(a) ddlInitEmpty(&(a).si_list)

int selInit();
void selrecord(struct selinfo *sip,int arg);
int selwakeup(struct selinfo *sip);
#else
struct selinfo
{
   int a[3];
};
#endif /* KERNEL */

typedef struct fd_set
{
   fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)]; /* 256/32=8 long */
} fd_set;

#define   FD_SET(n, p)   ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define   FD_CLR(n, p)   ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define   FD_ISSET(n, p)   ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define   FD_COPY(f, t)   bcopy(f, t, sizeof(*(f)))
#define   FD_ZERO(p)      bzero(p, sizeof(*(p)))

int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int pselect(int nfds, fd_set *, fd_set *, fd_set *, const struct timespec *,
      const sigset_t *);
extern struct timezone tz;

int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);

/* Free Bsd Additions */
#define wall_cmos_clock 0     /* wall   CMOS clock assumed if != 0 */
extern int adjkerntz;         /* local offset   from GMT in seconds */

#endif /*_SYS_TIME_H*/

