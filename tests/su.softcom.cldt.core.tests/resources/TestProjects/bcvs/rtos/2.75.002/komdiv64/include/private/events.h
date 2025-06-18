/*  private/events.h  */

#ifndef _PRIVATE_EVENTS_H
#define _PRIVATE_EVENTS_H

typedef struct evnControl *event_t;


#ifdef KERNEL

#include <private/queue.h>


#define evnSET_EVENT(ev)     (ev)->m_state=UP
#define evnRESET_EVENT(ev)   (ev)->m_state=DOWN
#define evnIS_UP(ev)         (ev)->m_state==UP
#define evnIS_TRYWAIT(time)  \
  ( (time) && (time)->tv_sec==0 && (time)->tv_nsec==0 )



enum {
	EVN_SHOW_HEAD =         1,
	EVN_SHOW_WAIT =         4,
	EVN_SHOW_LOCKED =       8
};

enum {
	EVN_SHOW_LIST_WAIT = 0,
	EVN_SHOW_LIST_SHORT
};

typedef struct  evnControl {
	objHEADER     m_objHdr;
	queHEAD       m_waitHead;
	volatile int  m_state;
} evnCONTROL, *evnCONTROL_ID;

extern  objCONTROL_ID  evnObjControlId;

extern  int evnInit( void );

/*средства просмотра событий*/
extern funcptr_t  evnShowRtn;
extern int        evnShowInit( void );
extern void       evnDisplayList( int showType );
extern void       evnShow( event_t evn, int list );

#else /*!KERNEL*/

struct evnControl;

#endif /*KERNEL*/

#endif /* _PRIVATE_EVENTS_H */
