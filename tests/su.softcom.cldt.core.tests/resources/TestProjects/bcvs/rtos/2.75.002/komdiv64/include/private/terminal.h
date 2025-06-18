#ifndef _PRIVATE_TERMINAL_H
#define _PRIVATE_TERMINAL_H

#include <termios.h>
#include <sys/time.h>
#include <private/commdrv.h>

struct  vc_data;

typedef struct term_devhdr {
    stdDEV                  te_dev;                 /* Общая часть всех устройств !!!*/
    /*  Описать и можно пользоваться */
    struct  termios         te_ti;
    int     (*te_ioctl)     (ioFILE *, int cmd,int arg);
    int     (*te_putchar)   (struct term_devhdr *td,char chr);
    int     (*te_checkread) (struct term_devhdr *td);
    int     (*te_getchar)   (struct term_devhdr *td,char *ch);
    /* Лучше не трогать  */
    void                    *te_sc;         /*  для slip и ppp      */
    struct  rngbuf          *te_rb;         /*  кольцевые буфера    */
    struct  rngbuf          *te_wb;         /*  кольцевые буфера    */
    struct  rngbuf          *te_echo;       /*  кольцевые буфера    */
    struct  aiocb           *te_wcb;        /*  текущая заявка      */
    volatile size_t         te_wcount;      /*  число байтов выведенных в текущей заявке            */
    struct  selinfo         te_siread,te_siwrite,te_siex;           /* для селекта                  */
    struct vc_data          *te_attached;                           /* виртуальный терминал         */
    struct callout          te_callout;                             /* для тайм-аута устройства     */
    int                     te_int;
    int                     te_flags;               /* флаги (см ниже)  */
    int                     te_cbrktime;            /* время в тиках кгда нужно выполнить TIOCCBRK */
#define TD_TIMEOUT 0x1
#define TD_ZEROOK  0x2
#define TD_XOFF    0x4
#define TD_BRK     0x8
#define TD_STOPPED ( TD_XOFF | TD_BRK )
#define TD_STOPSENT 0x10
#define TD_NORTS      0x20
} termDEV;

int     termDevInit(termDEV *td,char *devname,ioDRV *dev_drv,int rdSize,int wrSize,int (*startio)(ioDEV *));
int     termDevFree(termDEV *td);
int     termStartio(termDEV *dev);
int     termDestroy(struct io_dev *);
int     termSubmit(ioFILE *fcb,struct aiocb *cb);
int     termCancel(struct file *file,int fildes,struct aiocb *cb);

#define TERM_OK     0
#define TERM_QUIT   -1
#define TERM_CONT   -2

#endif /* ! _PRIVATE_TERMINAL_H*/
