#ifndef _PRIVATE_COMMDRV_H
#define _PRIVATE_COMMDRV_H

#include <private/mutex.h>
#include <private/ios.h>
#include <private/resource.h>
#include <private/stddrv.h>

typedef struct {
	ioDEV			dev_dev;
	queHEAD			dev_rque,dev_wque;                  /* Очереди aiocb					*/
	pthread_mutex_t dev_mutex;							/* Mutex							*/
	resDESCR		dev_res;
} stdDEV;


#define devLOCK(dh) pthread_mutex_lock(&(dh)->dev_mutex);resLock(&(dh)->dev_res); 

#define devUNLOCK(dh) resUnlock(&(dh)->dev_res); pthread_mutex_unlock(&(dh)->dev_mutex);

int drv1qDestroy(ioDEV *devcb);
int drv2qDestroy(ioDEV *devcb);
int drv2qPrioDevIns(stdDEV * devcb,const char *devname,ioDRV *drvaddr,int (*startio)(ioDEV *));
int drv2qFifoDevIns(stdDEV * devcb,const char *devname,ioDRV *drvaddr,int (*startio)(ioDEV *));
int drv2qSubmit(ioFILE *fcb,struct aiocb *cb);
int drv2qClose(ioFILE *file,int fildes);
int drv2qCancel(ioFILE *file,int fildes,struct aiocb *cb);
int drv2qFsync(ioFILE *file,int,struct aiocb *cb);

#endif /* ! _PRIVATE_COMMDRV_H */
