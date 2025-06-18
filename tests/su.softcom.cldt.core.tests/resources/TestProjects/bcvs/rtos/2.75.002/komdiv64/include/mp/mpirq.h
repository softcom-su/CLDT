/* mpirq.h*/
/*
25.04.2002 добавлен mpIRQ_SIGNAL
*/
#ifndef _MPIRQ_H
#define _MPIRQ_H

#include <mp/mpddlist.h>
#include <mp/mpspin.h>

/* Заголовок очереди прерываний*/
typedef struct mpirq_head {
  spin_t             m_spin;
  mpddlHEAD          m_list;
} mpIRQ_HEAD;

union intval {
	int  i;
	void *v;
};

#define mpIRQ_RDY	  0x0  /*прерывание на готовность ЦПУ*/
#define mpIRQ_PKT	  0x1  /*прерывание на обработку пакета*/
#define mpIRQ_OBJ	  0x2  /*прерывание на обработку объекта*/

#define mpINT_TABLE_SIZE	3 /* размер таблицы */

#define mpIRQ_NOACTION      0x0     /* прерывание не требуется */
#define mpIRQ_AVAILABILITY  0x00001 /* прерывание активность*/
#define mpIRQ_SIGNAL        0x00002 /* послать сигнал*/
#define mpIRQ_PKTGET        0x10000 /* код запроса на прерывание - 
                                  "есть сообщения в буфере Ethernet"*/
#define mpIRQ_PKTSEND       0x10001 /* код запроса на прерывание - 
                                 "есть свободное место буфере Ethernet "*/
#define mpIRQ_OBJFREE       0x20000 /* код запроса на прерывание - 
                                 "ресурс доступен"*/
/*#define mpIRQ_CPURDY        0x40000*/ /* код запроса на прерывание - 
                                 "ЦПУ активен?"*/


typedef struct {
	mpddlNODE node;
#define mpIRQ_INQUE    0x1 /* irq в очереди оповещения*/
#define mpIRQ_WAIT     0x2 /* irq в очереди ожидания */
	int flags;
	int toCPU;	 /* номер процессора, которому нужно послать прерывание*/
	int fromCPU; /* номер процессора, посылающего прерывание*/
	int code;	   /* код прерывания (определяет функцию обработки прерывания)*/
	union intval value; /* значение, связанное с прерыванием 
                     (целое или указатель в зависимости от кода прерывания)*/
} mpIRQ;

#define mpIRQ_INIT(irq,cd,tCPU,val) \
		(irq)->flags = 0;             \
		(irq)->toCPU = htonl( tCPU );\
		(irq)->fromCPU = 0;           \
		(irq)->code = htonl( cd );  \
		(irq)->value.i = (int)val
/*надо value приводить к net - для уведомлений это важно*/

int mpIrqSend( mpIRQ* );
int mpIrqCancel( mpIRQ* );
int mpIrqReceive( int *code, union intval *val );

void mpIrqHandler(int p);
int mpIntConnect(int vec,int (*intr)( int code, union intval val ));
int mpObjIntrHandling( int code, union intval value );
int mpRdyIntrHandling( int code, union intval value );

#endif /*_MPIRQ_H*/
