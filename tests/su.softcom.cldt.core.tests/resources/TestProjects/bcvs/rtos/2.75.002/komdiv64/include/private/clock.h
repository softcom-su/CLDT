#ifndef _PRIVATE_CLOCK_H
#define _PRIVATE_CLOCK_H

#ifdef KERNEL
#include <time.h>
#include <limits.h>
#include <arch/endian.h>
#include <arch/board.h>
#include <sys/time.h>
#include <sys/apextypes.h>
#include <private/queue.h>

typedef union {
    tick_t      tt;
    es_longlong es;
} timeout_clock_t;

typedef struct clock_wait_node
{
    queHOOK_NODE        m_clockNode;    /* узел для вкл-я в очередь к часам */
    int                 m_Type;         /* тип времени ожидания (TIMER_ABSTIME) */
    union {                             /* время ожидания в очереди к часам */
        struct timespec abstime;        /* абсолютное время */
        tick_t          ticktime;       /* номер тика часов от момента их запуска */
    } m_time;
    int                 m_overtime;     /* превышает ли время ожидания ULONG_MAX */
    ddlNODE             m_abstimeNode;  /* узел для включения в список элементов с абс. временем */
    #define m_abstime   m_time.abstime
    #define m_ticktime  m_time.ticktime
} clockWAIT_NODE;

struct clockCONTROL {
    objHEADER           m_objHdr;
    struct timespec     m_BaseTime;         /* Базовое время */
    struct timespec     m_BaseTimeAdd;      /* Базовое время */
    struct timespec     m_Res;              /* Разрешение часов */
    u_long              m_Ticks;            /* Число тиков  */
    union {
        tick_t  tt;
        es_longlong es;
    }                   m_longTicks;        /* Число тиков от начала работы */
    u_long              m_HZ;               /*  Число тиков в секунду       */
    u_long              m_NS;               /* Число наносекунд в тике      */
    queHEAD             m_Que;              /* Очередь к таймеру            */
    ddlHEAD             m_abstimeList;      /* Очередь  abstime узлов       */
};

extern struct clockCONTROL Clocks[CLOCK_MAX];
extern objCONTROL clockObjControl;

#define clockCHECK(clock_id) \
    if( clock_id>CLOCK_MAX || objCHECK(&clockObjControl,&Clocks[clock_id].m_objHdr)) errSET_AND_RETURN(EINVAL);

/* Макросы для системных часов */
#define kernClock    Clocks[CLOCK_REALTIME]
/* Очередь к системному таймеру */
#define kernTimerQue Clocks[CLOCK_REALTIME].m_Que
/* Список элементов в очереди к системному таймеру по абс времени */
#define kernTimerList Clocks[CLOCK_REALTIME].m_abstimeList
/* Счётчик тиков системных часов */
#define kernTicks    Clocks[CLOCK_REALTIME].m_Ticks
/* Разрешение системных часов */
#define kernHZ       Clocks[CLOCK_REALTIME].m_HZ

int     clockPxInit( );
int     clockInit(clockid_t clock_id, int clock_hz);
void    clockHard(clockid_t clock_id);
void    clockQueDpcRtn(queHEAD *que, int cnt);

int     clockInsert(struct clockCONTROL *pClock, clockWAIT_NODE *pNode, tick_t *overtime );
#define clockREMOVE(pClock,pNode)                           \
{                                                           \
    queExtract ( &(pClock)->m_Que, (queNODE *) pNode);      \
    if ((pNode)->m_Type==TIMER_ABSTIME ) {                  \
        ddlExtract(&(pNode)->m_abstimeNode);                \
    }                                                       \
}

tick_t  clock_gettime_t (struct clockCONTROL *theClock, struct timespec *tp);
tick_t  clock_gettick_t (struct clockCONTROL *theClock);

#define NANOSECONDS_PER_SECOND  1000000000L

#define TIMESPEC_EQ(a,b)      \
    ((a).tv_sec == (b).tv_sec && (a).tv_nsec == (b).tv_nsec)
#define TIMESPEC_GT(a,b)      \
    ((a).tv_sec  > (b).tv_sec ||    \
    ((a).tv_sec == (b).tv_sec && (a).tv_nsec > (b).tv_nsec))
#define TIMESPEC_LT(a,b)      \
    ((a).tv_sec  < (b).tv_sec ||    \
    ((a).tv_sec == (b).tv_sec && (a).tv_nsec < (b).tv_nsec))
#define TIMESPEC_ISCLEAR(a)    \
    ((a).tv_sec == 0 && (a).tv_nsec == 0)
#define TIMESPEC_COPY(a,b)     \
    (a).tv_sec = (b).tv_sec; (a).tv_nsec = (b).tv_nsec
#define TIMESPEC_SET(a,sec,nsec)     \
    (a).tv_sec = sec; (a).tv_nsec = nsec
#define TIMESPEC_ADD(a,b)     \
    (a).tv_sec += (b).tv_sec; (a).tv_nsec += (b).tv_nsec;\
    if( (a).tv_nsec >= NANOSECONDS_PER_SECOND) {\
        (a).tv_nsec -= NANOSECONDS_PER_SECOND;\
        (a).tv_sec ++; }

#define TIMESPEC_SUB(a,b)     \
    (a).tv_sec -= (b).tv_sec; (a).tv_nsec -= (b).tv_nsec;\
    if( (a).tv_nsec < 0) {\
        (a).tv_nsec += NANOSECONDS_PER_SECOND;\
        (a).tv_sec --; }

#define TIMESPEC_ISVALID(a)    \
    ( (a).tv_sec>=0 && (a).tv_nsec >= 0 && (a).tv_nsec < NANOSECONDS_PER_SECOND)
#define TIMESPEC_CLEAR(a)      \
    (a).tv_sec = 0; (a).tv_nsec = 0

#define NSEC_TO_TIMESPEC(nsec,time) \
  (time).tv_sec = nsec / NANOSECONDS_PER_SECOND; \
  (time).tv_nsec = nsec % NANOSECONDS_PER_SECOND

#define TIMESPEC_TO_NSEC(time,nsec) \
  nsec = (time).tv_sec * NANOSECONDS_PER_SECOND + (time).tv_nsec

int timeTimespecSubstract (
    const struct timespec *start,
    const struct timespec *end,
    struct timespec *diff
);

int timeTimespecAdd (
    struct timespec       *tim,
    const struct timespec *diff
);

tick_t timeTicksTo(register struct clockCONTROL *theClock,
                   const struct timespec *from,
                    const struct timespec *to);

void timeTicksToTimespec(register struct clockCONTROL *theClock,
    tick_t ticks,struct timespec *tim);

tick_t timeTimespecToTicks(register struct clockCONTROL *theClock,
    const struct timespec *tim);

#define TIMESPEC_TO_SYSTEM_TIME(tp) ((SYSTEM_TIME_TYPE)(tp)->tv_sec*NANOSECONDS_PER_SECOND+(tp)->tv_nsec)
#define SYSTEM_TIME_TO_TIMESPEC(tp,st) (tp)->tv_sec=st/NANOSECONDS_PER_SECOND; (tp)->tv_nsec=st%NANOSECONDS_PER_SECOND

u_long clock_gettick();

/* Опрос текущего времени */
static inline system_time_t clockGetRealTime( void )
{
   return boardClockGetTime();
}

#endif

#endif
