


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
	manch_hint_attach() - подключение функции обратного вызова к МКИО-ОУ
*/
int
manch_hint_attach(
   int device,        //идентификатор связи с МКИО
   PCALLBACKFUNC func,//функция обратного вызова
   unsigned ds        //не используется
 )
{
 int ret=device;
 mkioIUDesc_t     * pi; //дескриптор ИУ

 //Номера:
 unsigned nMKIO=device;       //порядковый номер МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле

 ds=ds;
 mkio_info("enter");

 //Проверим параметры
 mkio_assert_r(device<=3, ME_INVALID_PARAMETER);
 mkio_assert_r(device>=0, ME_INVALID_PARAMETER); //gmu

 //Захватим ИУ
 mkio_assert_g(pthread_mutex_lock(&mkio_mutex[nMKIO])==0, 
               DE_DRIVER_SPECIFIC, Error);

 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 //Проверим состояние описателя ИУ
  //ИУ проинициализировано?
 mkio_assert_g(pi->ini==1, DE_DRIVER_SPECIFIC, Error);
  //подключена другая функция?
 mkio_assert_g(pi->rt_func==0, DE_DRIVER_SPECIFIC, Error);

 pi->rt_func=func;
//ret=device;

Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 mkio_info("exit");
 return ret;
}

