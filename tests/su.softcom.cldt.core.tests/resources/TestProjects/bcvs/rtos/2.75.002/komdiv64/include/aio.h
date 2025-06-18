/*
 *  Copyright 1998
 */

#ifndef  _AIO_H
#define  _AIO_H

#include <signal.h>
#include <sys/time.h>
/* aio_cancel() return values */
#define  AIO_ALLDONE          1  /* Все операции были завершены до начала aio_cancel */
#define  AIO_CANCELED         2  /* Операции либо завершились либо канселированы    */
#define  AIO_NOTCANCELED         3  /* По крайней мере один из запросов не м.б. удален */

/* lio_listio()  modes */

#define  LIO_WAIT          4  /* wait for completion */
#define  LIO_NOWAIT           5  /* don't wait for completion */

/* lio_listio() operations */
/* lio op codes            */
#define  LIO_READ   1                  /* read operation                               */
#define  LIO_WRITE  2                  /* write operation                              */
#define  LIO_NOP    3                  /* no transfer operation                        */
#if OS_MAJOR == 2
#ifdef KERNEL
#include <private/aio.h>
/* Блок асинхронного ввода/вывода  */
struct aiocb  {
    struct aiosys aiosys;
#else
struct aiocb  {
    int         aiosys[15];
#endif
    int        aio_fildes;             /* file descriptor                              */
    off_t      aio_offset;             /* file offset                                  */
    void *      aio_buf;               /* location of buffer                           */
    size_t     aio_nbytes;             /* length of transfer                           */
    int        aio_reqprio;            /* request priority offset                      */
    struct sigevent  aio_sigevent;     /* signal number and value                      */
    int        aio_lio_opcode;         /* operation to be performed                    */
    int        __fdflags;
    off_t      __offset;
};
#else
/* Блок асинхронного ввода/вывода  */
struct aiocb  {
    int              aio_fildes;       /* file descriptor                              */
    off_t            aio_offset;       /* file offset                                  */
    void            *aio_buf;          /* location of buffer                           */
    size_t           aio_nbytes;       /* length of transfer  (return status)          */
    int              aio_reqprio;      /* request priority offset                      */
    struct sigevent  aio_sigevent;     /* signal number and value                      */
    int              aio_lio_opcode;   /* operation to be performed                    */
    int              aio_error;        /* код ошибки (error status)                    */
    int              __fdflags;
    int              __offset;
};
#endif
ssize_t aio_return(struct aiocb *cb);
int aio_error(const struct aiocb *aiocbp);


int aio_read(struct aiocb *);
int aio_write(struct aiocb *);
int lio_listio(int mode, struct aiocb * const list[],int nent, struct sigevent * sig);
int aio_suspend(const struct aiocb * const list[],int, const struct timespec *);
int aio_cancel(int,struct aiocb *);
int aio_fsync(int,struct aiocb *);

#endif      /* _AIO_H */
