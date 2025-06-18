#ifndef _OC3K_COMPAT_H
#define _OC3K_COMPAT_H
#include <sys/time.h>
/* res  = a - b */
static __inline__ void timersub(struct timeval *a, struct timeval *b,
              struct timeval *res)
{
    res->tv_sec = a->tv_sec - b->tv_sec;
    res->tv_usec = a->tv_usec - b->tv_usec;
    if( res->tv_usec < 0 )
    {
        res->tv_usec += 1000000;
        res->tv_sec--;
    }
}
#endif /* _OC3K_COMPAT_H */
