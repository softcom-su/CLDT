
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "libbcvs.h"
#include "manchester.h"

/*
	manch_notify_attach() - подключение функции обратного вызова к серверу МКИО
*/
int
manch_notify_attach(
   int device,        //идентификатор связи с МКИО
   PNOTIFYFUNC func,  //функция обратного вызова
   unsigned ds        //не используется
 )
{
 int ret=device;
 mkioIUDesc_t     * pi; //дескриптор ИУ

 //Номера:
 unsigned nMKIO=device;       //порядковый номер МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле
 mkio_info("enter");
 ds=ds;

 //Проверим параметры
 mkio_assert_r(device<=3, ME_INVALID_PARAMETER);
 mkio_assert_r(device>=0, ME_INVALID_PARAMETER);

 //Захватим ИУ
 mkio_assert_g(pthread_mutex_lock(&mkio_mutex[nMKIO])==0, 
               DE_DRIVER_SPECIFIC, Error);

 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 //Проверим состояние описателя ИУ
  //ИУ проинициализировано?
 mkio_assert_g(pi->ini==1, DE_DRIVER_SPECIFIC, Error);
  //подключена другая функция?
 if(func!=0) {
    mkio_assert_g(pi->bc_func==0, DE_DRIVER_SPECIFIC, Error);
 }

 pi->bc_func=func;
//ret=device;

Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 mkio_info("exit");
 return ret;
}

