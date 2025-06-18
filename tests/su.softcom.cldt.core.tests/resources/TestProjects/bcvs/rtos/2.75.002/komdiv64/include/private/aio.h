#ifndef 	_PRIVATE_AIO_H
#define  	_PRIVATE_AIO_H

#include	<private/ecb.h>
#include	<private/queue.h>
#include	<sys/systm.h>
#include	<sys/signal.h>
#include	<sys/callout.h>

struct aiocb;
struct que_head;
struct aiosys   {
/*  Реализация  */
	quePARM_NODE		sy_node;						/* Для включения в приоритетный список	*/
	unsigned int		sy_flags;						/* Флаги из fd							*/
	volatile size_t		sy_count;						/* Уже переданная длина 				*/
	volatile int		sy_error; 						/* Код ошибки							*/
    volatile int        sy_state;                        /* Состояние aio операции               */
	struct que_head 	*sy_que;						/* Заголовок очереди в которой стоит aiocb */
    ecbNODE				sy_ecbn;                        /* Указатель на ecb блок для ожидания      */
	struct sigevent		*sy_lioSe;						/* Указатель на sigevent из lio_listio	*/
    void                *sy_ext;                        /* указатель на дополнительную информацию */
    struct	callout_handle  sy_callout;                 /* тайм-аут операции */
};
enum aio_state {ioAIO_QUEUED=1,ioAIO_LOCKED,ioAIO_COMPLETE,ioAIO_RETURNED};
/* Блок асинхронного ввода/вывода  */



#endif
