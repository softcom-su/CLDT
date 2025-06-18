#ifndef _PRIVATE_TIMER_H
#define _PRIVATE_TIMER_H

#include <sys/signal.h>
#include <private/objcore.h>
struct clockCONTROL;
/*================================= Таймеры  ========================*/
struct tmrCONTROL {
    objHEADER             m_objHdr;     
    struct clockCONTROL  *m_clock;       /* Часы */
    clockid_t             m_clockid;     /* Часы */
    struct  sigevent      m_sigevent;       /* sigvent из timer_create*/
    struct  itimerspec    m_itimerspec;     /* itimrspec из timer_settime */
    int                   m_Started;    
    struct sigINFO_NODE  *m_pInfoNode;
    sdlHEAD               m_pInfoQue;
    clockWAIT_NODE        m_waitNode; 
    volatile int          m_c_overrun,m_l_overrun;
};
extern objCONTROL_ID tmrObjControlId;

#endif
