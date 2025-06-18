/*  mqueue.h  */

#ifndef _MQUEUE_H
#define _MQUEUE_H

#include  <fcntl.h>
#include  <sys/types.h>
/* Используемые по умолчанию атрибуты очереди сообщений */
#define MQ_DFLT_FLAGS   O_RDWR    /* флаги (разрешены отправка и приема сообщений)     */
#define MQ_DFLT_MAXSIZE 16        /* максимальный размер сообщения                     */
#define MQ_DFLT_MAXMSG	20        /* максимальное количество сообщений                 */

struct sigevent;
struct timespec;
struct proc;
/* Описатель атрибутов очереди сообщений */
struct mq_attr {
	long  mq_flags;               /* флаги очереди сообщений                           */
	long  mq_maxmsg;              /* максимальное количество сообщений                 */
	long  mq_msgsize;             /* максимальный размер сообщения                     */
	long  mq_curmsgs;             /* количество сообщений в очереди в настоящий момент */
};


extern int mq_close(	mqd_t  mqdes );

extern int mq_getattr(
	mqd_t           mqdes,
	struct mq_attr *mqstat
);



extern int mq_notify(
	mqd_t                  mqdes,
	const struct sigevent *notification
);

extern mqd_t mq_open(
	const char *name,
	int         oflag,
	...
);

extern ssize_t mq_receive(
	mqd_t         mqdes,
	char         *msg_ptr,
	size_t        msg_len,
	unsigned int *msg_prio
);

extern ssize_t mq_timedreceive(
	mqd_t         mqdes,
	char         *msg_ptr,
	size_t        msg_len,
	unsigned int *msg_prio,
  const struct timespec *abs_timeout
);

extern int mq_send(
	mqd_t         mqdes,
	const char   *msg_ptr,
	size_t        msg_len,
	unsigned int  msg_prio
);

extern int mq_timedsend(
	mqd_t         mqdes,
	const char   *msg_ptr,
	size_t        msg_len,
	unsigned int  msg_prio,
  const struct timespec *abs_timeout
);

extern int mq_setattr(
	mqd_t                 mqdes,
	const struct mq_attr *mqstat,
	struct mq_attr       *omqstat
);

extern int mq_unlink( const char *name );


extern int mq_timedsend(
	mqd_t                  mqdes,
	const char            *msg_ptr,
	size_t                 msg_len,
	unsigned int           msg_prio,
	const struct timespec *timeout
);

extern ssize_t mq_timedreceive(
	mqd_t         mqdes,
	char         *msg_ptr,
	size_t        msg_len,
	unsigned int *msg_prio,
	const struct  timespec *timeout
);

#endif /* _MQUEUE_H */

