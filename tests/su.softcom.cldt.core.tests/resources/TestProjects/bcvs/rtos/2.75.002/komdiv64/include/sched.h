/* sched.h */

#ifndef _SCHED_H
#define _SCHED_H

#include <sys/time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* тактика планирования */

#define SCHED_FIFO      0x1                     /* по приоритетам, внитри приоритета - FIFO */
#define SCHED_RR        0x2                     /* round robin - карусель              */
#define SCHED_OTHER     0x4                     /* не используется                     */

/* максимальный и минимальный POSIX приоритет  */

#define SCHEDULER_MAXIMUM_PRIORITY  (255)       /* максимальный приоритет потока       */
#define SCHEDULER_MINIMUM_PRIORITY  (0)         /* минимальный приоритет потока        */
#define SCHED_FIFO_HIGH_PRI         (255)       /* POSIX highest priority for FIFO     */
#define SCHED_FIFO_LOW_PRI          (1)         /* POSIX lowest  priority for FIFO     */
#define SCHED_RR_HIGH_PRI           (255)       /* POSIX highest priority for RR       */
#define SCHED_RR_LOW_PRI            (1)         /* POSIX lowest  priority for RR       */

struct sched_param /* параметры планирования             */
{
   int sched_priority; /* приоритет                          */
};

extern int sched_setparam(pid_t pid, const struct sched_param * param);
extern int sched_getparam(pid_t pid, struct sched_param * param);
extern int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
extern int sched_getscheduler(pid_t pid);
extern int sched_yield(void);
extern int sched_get_priority_max(int policy);
extern int sched_get_priority_min(int policy);
extern int sched_rr_get_interval(pid_t pid, struct timespec * interval);

#ifdef __cplusplus
}
#endif

#endif /* _SCHED_H */
