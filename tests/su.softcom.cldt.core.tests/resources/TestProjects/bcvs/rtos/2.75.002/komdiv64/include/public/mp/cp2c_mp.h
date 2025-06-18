/*
 * cp2c_mp.h
 *
 *  Created on: 18.11.2010
 *      Author: nkigor
 */

#ifndef CP2C_MP_H_
#define CP2C_MP_H_
#include <semaphore.h>
#include <sys/queue.h>
struct mp_send_port;
struct mp_recv_port;
#define MAX_NAME 64
#define SAMPLE_PORT 0x12345678
struct mp_buf;
struct mp_buf_attr;

struct cp2c_mp_port_conf
{
   int nbuf;
   int msgsize;
   int rio_id;
   int mbox;
   sem_t sem_id;
   char name[MAX_NAME];
};

void mp_buf_getattr(struct mp_buf *buf, struct mp_buf_attr *attr);
void mp_buf_putattr(struct mp_buf *buf, struct mp_buf_attr *attr);
void *mp_buf_getmem(struct mp_buf *buf);


#endif /* CP2C_MP_H_ */
