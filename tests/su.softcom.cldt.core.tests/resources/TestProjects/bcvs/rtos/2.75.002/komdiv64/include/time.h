/* time.h */
#ifndef _TIME_H
#define	_TIME_H

#include <sys/types.h>
#include <sys/time.h>
#include <sys/signal.h>
/* C89, C99, POSIX.1-2001.  POSIX requires that CLOCKS_PER_SEC equals 1000000 independent of the actual resolution. */
#define CLOCKS_PER_SEC 1000000

extern int daylight;
extern long int timezone;
extern char *tzname[];

char     *asctime(const struct tm *);
char     *asctime_r(const struct tm *, char *);
clock_t  clock(void);
int      clock_getres(clockid_t, struct timespec *);
int      clock_gettime(clockid_t, struct timespec *);
int      clock_settime(clockid_t, const struct timespec *);
int      clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *requested_time,
                        struct timespec *remaining_time);
char     *ctime(const time_t *);
char     *ctime_r(const time_t *, char *);
double   difftime(time_t, time_t);
struct tm *getdate(const char *);
struct tm *gmtime(const time_t *);
struct tm *gmtime_r(const time_t *, struct tm *result);
struct tm *localtime(const time_t *);
struct tm *localtime_r(const time_t *, struct tm *);
time_t     mktime(struct tm *);

int  nanosleep(const struct timespec *requested_time,
            struct timespec *remaining);

size_t strftime(char *, size_t, const char *, const struct tm *);
/*char   *strptime(const char *,const char *,struct tm *);*/
time_t time(time_t *);

int timer_create(clockid_t clock_id, struct sigevent *evp, timer_t *ptimer);
int timer_delete(timer_t timerid);
int timer_gettime(timer_t timerid, struct itimerspec *value);
int timer_getoverrun(timer_t timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value,
      struct itimerspec *ovalue);
void tzset(void);
int clock_getcpuclockid(pid_t pid, clockid_t *clock_id);
int clockAdjust( system_time_t D, int Efmax, int Esmax, int Etsh );

#endif /* _TIME_H */
