/* mptcb.h*/
#ifndef _MPTCB_H
#define _MPTCB_H

#include <mp/mpmem.h>

#ifdef KERNEL

#include <mp/mpddlist.h>
#include <mp/mpque.h>
#include <mp/mpirq.h>
#include <mp/mpobjcore.h>
#include <private/thread.h>
#include <private/mempool.h>

#define TCB_SHOW_HEAD      1

#define MPTCB_GET_QUERES(mptcb)  (mpQUEHEAD*)mpAdrLocal((mpoffset_t)(mptcb)->m_que)
#define MPTCB_GET_RESOURCE(mptcb)  (objHEADER*)mpAdrLocal      \
      ((mpoffset_t)(MPTCB_GET_QUERES(mptcb))->m_owner)

#define mpIRQ_TO_TCB(irq) ( (mpTCBCONTROL *) ((u_char *)irq - ITEM_OFFSET(mpTCBCONTROL, m_irq)) )
/*mpTCB представляет текущий процессор?*/
#define mpThreadIsLocal(mptcb) ( (mptcb)->m_irq.toCPU == mpPSN )

/*доступ потока к общей памяти*/
#define mpTHREAD_ACCESS(thread)  ((thread)->m_mptcb != NULL)

typedef struct mpTcbControl
{ /*узел очереди к ресурсу - узел IRQ*/
	objHEADER     m_objCore;
  mpoffset_t    m_que;     /*ожидаемый ресурс - очередь к нему*/
  mpIRQ         m_irq;     /*блок запроса на прерывание*/ 
} mpTCBCONTROL, *mpTCBCONTROL_ID;

extern mpOBJCONTROL *mpTcbControlId; /*описатель класса*/
extern mpoffset_t    mpTcbControlOff;  /*смещение описателя*/
/*адрес локального описателя класса*/
extern mpOBJCONTROLLOC_ID	  mpTcbControlLocId;

extern void mpTcbInsert( mpQUEHEAD *que );
extern mpTCBCONTROL *mpThreadRelease( mpQUEHEAD *que );
extern funcptr_t mpDequeueThreadRtn; /*отрабатывает получение управления после ожидания ресурса*/
extern mpTCBCONTROL* (*mpTcbGetRtn)( threadTCB *theThread ); /*получить mpTCB*/
extern void (*mpTcbFreeRtn)( void ); /*освобождение mpTCB*/
extern void mptcb( mpTCBCONTROL *mptcb );

extern funcptr_t mpTcbShowRtn;
extern funcptr_t mpTcbResourceRtn;

#endif /*KERNEL*/

extern int mpTcbInit( mpMEMPOOL_ID pool, int num );
extern int mpTcbShowInit( void );

#endif /*_MPTCB_H*/
