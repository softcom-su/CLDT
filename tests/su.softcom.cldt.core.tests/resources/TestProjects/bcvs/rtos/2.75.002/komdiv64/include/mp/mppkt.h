#ifndef _MP_MPPKT_H
#define _MP_MPPKT_H
#include <mp/mpirq.h>
/* Заголовок пакета в кольцевом буфере */
struct pkt_head {
	int length;
};

#define mpPKTBUF_ADDR(head) (char *)((char *)head+sizeof(struct pkt_head))
#define mpPKTBUF_MIN_SIZE	  2048
#define mpPKT_ALIGNMENT			4

#define mpPKT_MTU			(mpPKTBUF_MIN_SIZE - sizeof(struct pkt_head) )	  
#define mpPKT_MINSIZE		sizeof(struct pkt_head)+sizeof(int)

/* Заголовки приемных кольцевых очередей */

struct pkt_buf {
	mpIRQ IRQout;     /* блок запроса на прерывание процессора, выводящего данные (пишущего процессора);*/
	mpIRQ IRQinp;     /* блок запроса на прерывание процессора, вводящего данные (читающего процессора);*/
	int	  pfree;      /* указатель на свободную часть буфера;*/
	int	  pbusy;      /* указатель на занятую часть буфера;*/
	int   waterline;  /* запрашиваемое свободное место;*/
	int	  length;     /* длина буфера;*/
	char  buff[mpPKTBUF_MIN_SIZE];		/* буфер передачи данных.*/
}; 

int mpPktInit(int size,int prm,int vector);

void *mpPktGetFree(int cputo,int size);
/* 
	Получение локального адреса занятого пакета 
	в буфере собственного процессора
 */
void *mpPktGet(int cpufrom);
/* 
	Освобождение места в собственном буфере
 */
int mpPktFree(int cpufrom);
/* Запуск передачи пакета */
int mpPktSend(int cputo);
#endif /* !_MP_MPPKT_H */