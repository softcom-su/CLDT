/* mpmqp.h*/

#ifndef _MPMQP_H
#define _MPMQP_H

#include <mp/mpddlist.h>
#include <mp/mpmem.h>
#include <mp/mpque.h>
#include <mp/mpdic.h>
#include <mqueue.h>

#ifdef KERNEL

typedef struct  mpMqpControl {
	objHEADER       m_objHdr;
	mpDIC_NODE     *m_dicnode;    /* узел в таблице имен */
	size_t          m_msgsize;    /* макс.длина сообщения */
	long            m_maxmsg;     /* макс.число сообщений */
	long            m_curmsgs;    /* число сообщений в очереди */
	long            m_attach;
	void		       *m_pBuffers;
  mpddlHEAD       m_pending;    /* сообщения очереди*/
  mpddlHEAD       m_inactive;   /* свободные буфера */
	mpQUEHEAD       m_waitHead;   /* ждущие сообщений из очереди */
	mpQUEHEAD       m_waitHeadBuf;/* ждущие буферов */
} mpMQPCONTROL;

typedef  mpMQPCONTROL  *mpMQPCONTROL_ID;

#define mpMQP_PRIO_CHECK( msg_prio )  ( (msg_prio) >= MQ_PRIO_MAX )
#define mpmqpMOVE_TO_BUF( pBuf, msg_ptr, msg_len, msg_prio )   \
		(pBuf)->size = htonl(msg_len);                             \
		(pBuf)->prio = htonl(msg_prio);                            \
		bcopy( msg_ptr, (char*)(pBuf) + sizeof(mqpBUFFER), msg_len )
/*вставить сообщение в конец очереди (спин уже захвачен)*/
/*извещение не выполняется*/
#define mpmqpINSERT_MSG( pMsg, pBuf, msg_prio )         \
    mpddlAppend ( &pMsg->m_pending, (mpddlNODE*)pBuf ); \
		pMsg->m_curmsgs = htonl(ntohl(pMsg->m_curmsgs)+1)

extern funcptr_t mpMqpTableRtn[];  /*таблица функций mqpmp*/

extern mpOBJCONTROL *mpMqpControlId;   /*описатель класса*/
extern mpOBJCONTROL *mpMqpPxControlId;   /*описатель класса неименованных*/

extern mpoffset_t    mpMqpControlOff;  /*смещение описателя*/
extern mpoffset_t    mpMqpPxControlOff;

extern mpOBJCONTROLLOC_ID	  mpMqpControlLocId;
extern mpOBJCONTROLLOC_ID	  mpMqpPxControlLocId;

extern funcptr_t mpMqpShowRtn;
extern funcptr_t mpMqpPxShowRtn;
extern int (*mpMqpGetIdRtn)( char *s );
extern void mpmq( mpMQPCONTROL_ID pMsg, int list );

#endif /*KERNEL*/

extern int mpMqpPxInit( mpMEMPOOL_ID pool, int num );
extern int mpMqpShowInit( void );


#endif /*_MPMQP_H*/
