/*
 * Simple Message Passing
 */
#ifndef _MP_H
#define _MP_H
#include <public/mp/cp2c_mp.h>
#include <channel/channel.h>
#define MAX_NAME    64
struct mp_port_conf {
    char src[MAX_NAME];
    char src_port[MAX_NAME];
    char dst[MAX_NAME];
    char dst_port[MAX_NAME];
#define MP_DIR_RCV  0
#define MP_DIR_SND  1
    /* Number of buffers */
    int nbuf;
    /* Message size */
    unsigned int msgsize;
    /* Cluster size */
    int csize;
    int dst_id;
    int src_id;
    int flow_tag;
    int tag;
};

/*
 * Send / receive buffer
 *
 * buffer states:
 *      dirty: port owns buffer, buffer contains new data
 *              (data not sent yet / already received)
 *      clean: port owns buffer, buffer contains no new data
 *              (data already sent / not received yet)
 *      busy: port doesn't own buffer
 *
 * States transition (receive)
 *      clean -(data receive)-> dirty -(recv_get)-> busy -(recv_put)-> clean
 * States transition: (send)
 *      clean -(send_get)-> busy -(send_put)-> dirty -(data sent)-> clean
 */
struct mp_buf;
int mp_init(int s, int *rmap, int *rid, int *n);
/**
 * Get / set buffer attributes
 */
extern void mp_buf_getattr(struct mp_buf *buf, struct mp_buf_attr *attr);
extern void mp_buf_putattr(struct mp_buf *buf, struct mp_buf_attr *attr);

/**
 * Get buffer mem
 */
extern void *mp_buf_getmem(struct mp_buf *buf);

/**
 * Create send/recv port
 *
 * Return zero on success, nonzero error code else.
 */
extern int mp_send_create(struct mp_port_conf *conf,
    struct mp_send_port **port);
extern int mp_recv_create(struct mp_port_conf *conf,
    struct mp_recv_port **port);
enum {
   MP_IOCTL_SET_SAMPLE =   1,
   MP_IOCTL_SET_MBOX,
   MP_IOCTL_NONBLOCK,
#ifdef MP_INTERNALS
   MP_IOCTL_INIT,
   MP_IOCTL_SEND_CREATE,
   MP_IOCTL_RECV_CREATE,
   MP_IOCTL_SEND_SETBUF,
   MP_IOCTL_RECV_SETBUF,
   MP_IOCTL_DESTROY,
   MP_IOCTL_SEND_CHECK,
   MP_IOCTL_RECV_CHECK,
   MP_IOCTL_WAIT,
#endif
};

extern int  mp_ioctl(int cmd,...);

/**
 * Setup port buffer memory
 */
extern void mp_send_setbuf(struct mp_send_port *port, int n, void *p);
extern void mp_recv_setbuf(struct mp_recv_port *port, int n, void *p);

/**
 * Get / put port buffer.
 *
 * Return zero on success, nonzero error code else
 */
extern int mp_send_get(struct mp_send_port *port, struct mp_buf **buf);
extern int mp_recv_get(struct mp_recv_port *port, struct mp_buf **buf);
extern int mp_send_put(struct mp_send_port *port, struct mp_buf *buf);
extern int mp_recv_put(struct mp_recv_port *port, struct mp_buf *buf);

/**
 * Destroy port
 */
extern int mp_send_destroy(struct mp_send_port *port);
extern int mp_recv_destroy(struct mp_recv_port *port);

extern void mp_send_activate(struct mp_send_port *port);
extern void mp_send_deactivate(struct mp_send_port *port);
extern void mp_recv_activate(struct mp_recv_port *port);
extern void mp_recv_deactivate(struct mp_recv_port *port);

extern int mp_send_check(struct mp_send_port *port);
extern int mp_recv_check(struct mp_recv_port *port);

#endif /* _MP_H */
