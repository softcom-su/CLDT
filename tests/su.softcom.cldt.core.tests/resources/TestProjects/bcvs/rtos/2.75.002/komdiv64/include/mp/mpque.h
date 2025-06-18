/* mpque.h*/
#ifndef _MPQUE_H
#define _MPQUE_H

#ifdef KERNEL

#include <mp/mpobjcore.h>
#include <mp/mpddlist.h>
#include <mp/mpspin.h>

typedef struct mpque_head {
    objHEADER  *m_owner;    /* ссылка на объект которому принадлежит очередь */
    spin_t      m_spin;
/*    pthread_spinlock_t m_spin; */
    mpddlHEAD   m_waitHead; /* заголовок списка элементов очереди*/
} mpQUEHEAD;

#define mpQueueAppend( que, mptcb ) \
  (mptcb)->m_irq.flags = 0;         \
  (mptcb)->m_que = (mpQUEHEAD*) mpAdrOffset( (que) );  \
  mpddlAppend ( &(que)->m_waitHead, &(mptcb)->m_nodewait )

#define mpQueueExtract( que, node ) \
    mpddlExtract ( &que->m_waitHead, node )

#define mpQueueFirst(que) (mpddlFirst(&(que)->m_waitHead))
#define mpQueueIsEmpty(que) (mpQueueFirst(que) == NULL)

/*заголовок списка спинов*/
extern mpQUEHEAD *mpSpinHead;

void mpQueueInit( mpQUEHEAD *que, objHEADER  *m_owner );

#endif /*KERNEL*/

#endif /*_MPQUE_H*/
