#ifndef _ERRTR_H
#define _ERRTR_H

#ifndef  __ASSEMBLER__
struct errtrEvent {
  int event;
  int thread;
  int value;
};

extern void *specTrace;
extern int specTraceSize;

extern int errtrSize;
extern struct errtrEvent * volatile errtrPnt;
extern struct errtrEvent * volatile errtrPnt0;
extern struct errtrEvent * volatile errtrPnt2;

extern int errtrUsSize;
extern void *errtrUsPnt0;
extern void *errtrUsPnt;

extern char *intInterruptStack;
/*extern int   intStackSize;*/
extern volatile int InterruptStack1;
extern volatile int InterruptStack2;

extern volatile int errtrError;

#ifdef TRACE_TREVIAL

#define eventSIZE  3

#define traceGET_SP(field)\
	__asm __volatile ("sw $29,%0": : "m" (field) )


#define errtrALLOC(ev,th,val)  \
  if( errtrPnt ) { \
    int lvl;  \
    struct errtrEvent *pnt; \
    lvl=intLock();  \
    if( errtrSize < sizeof(struct errtrEvent) ) { \
      errtrPnt = NULL; /*останавливаем трассу*/\
      intUnlock(lvl); \
    } else {\
      pnt = errtrPnt; \
      (char*)errtrPnt += sizeof(struct errtrEvent);  \
      errtrSize -= sizeof(struct errtrEvent);  \
      intUnlock(lvl); \
      pnt->event = ev;  \
      pnt->thread = (int)th; \
      pnt->value = (int)val; \
    } \
  }

#define errtrUSALLOC(p,v,s)  \
  if( errtrUsPnt0 && (v) && (s) ) { \
    if( p ) { \
      bcopy( (v) , (p), s ); \
    } else { \
      bcopy( (v) , errtrUsPnt, s ); \
      errtrUsPnt = (void*)((unsigned)errtrUsPnt + s); \
    } \
  }

#else /* TRACE_TREVIAL */

#define traceGET_SP(field)
#define errtrALLOC(ev,th,val)
#define errtrUSALLOC(p,v,s)
#define errtrENTRY
#define errtrEXIT

#endif /* TRACE_TREVIAL */

enum {
  ev_sem_wait = 1,
  ev_sem_wait_cnt,
  ev_sem_wait_blk,
  ev_sem_wait_blk2,
  ev_sem_wait_blk3, /* 5 */
  ev_sem_wait_blk4,
  ev_sem_wait_rls,
  ev_sem_wait_rls2,
  ev_sem_wait_rls3,
  ev_sem_wait_rls4, /* 10/a */
  ev_sem_wait_rls5,
  ev_activ,         /* 12/c */
  ev_activ2,        /* 13/d */
  ev_activ3,
  ev_activ4,        /* 15/f */
  ev_sem_wait_int,
  ev_sem_wait_err,  /* 17/11 */
  ev_dispatch,      /* 18/12 */
  ev_dispatch2,     /* 19/13 */
  ev_dispatch3,     /* 20/14 */
  ev_suspend,       /* 21/15 */
  ev_resume,     
  ev_insert,    
  ev_sem_wait_err2, /* 24/18 */
  ev_suspend2,      /* 25/19 */
  ev_suspend3,      /* 26/1a */
  ev_suspend4,      /* 27/1b */
  ev_sem_wait_blk5, /* 28/1c */
  ev_sem_post,    
  ev_sem_post1,    
  ev_sem_post2,     /* 31/1f */
  ev_sem_post3,    
  ev_int1,          /* 33/21 */
  ev_int2,    
  ev_int3,          /* /23 */    
  ev_int4,          /* /24 */
  ev_int5,    
  ev_int6,    
  ev_int7,    
  ev_int8,    
  ev_int9,    
  ev_int10          /* 42/2a  */
};

void errtrInit( void );

#else /*__ASSEMBLER__*/

.extern errtrPnt         4 
#ifdef TRACE_TREVIAL

#define errtrENTRY \
        lw	a0,errtrPnt; \
        li	v0,1; \
        beq	a0,zero,errtrentry; \
        sw	v0,0(a0); \
        sw	t0,4(a0); \
        sw	sp,8(a0); \
        addiu   a0,a0,12; \
        sw	a0,errtrPnt; \
errtrentry:

#define errtrEXIT \
        lw 	t2,errtrPnt; \
        li	t1,2; \
        beq	t2,zero,errtrexit; \
        lw	t0,intNestLevel; \
        sw	t1,0(t2); \
        sw	t0,4(t2); \
        sw	sp,8(t2); \
        addiu   t2,t2,12; \
        sw	t2,errtrPnt; \
errtrexit:  
#else /*TRACE_TREVIAL*/
#define errtrENTRY
#define errtrEXIT
#endif  /*TRACE_TREVIAL*/
#endif  /*__ASSEMBLER__*/

#endif /* _ERRTR_H */
