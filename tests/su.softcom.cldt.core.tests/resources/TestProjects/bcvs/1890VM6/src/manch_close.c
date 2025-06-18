

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
  manch_close() - разрыв связи с драйвером МКИО
  ВОЗВРАТ:
  0 - норма
  ME_INVALID_PARAMETR - неверный параметр
  DE_DRIVER_SPECIFIC - ошибка:
    - ИУ не проинициализировано
    - неверный дескриптор файла ИУ
    - не удалось закрыть файл ИУ
*/
int
manch_close(
   int device
 )
{
 int ret=0;
 //Номера:
 unsigned nMKIO=device;       //порядковый номер МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле

//Режим ИУ для ioctl()
bt401mode manchMode = {
 .mode=NON,     /* режим ИУ:
                   BC - контроллер канала,
                   RT - оконечное устройство,
                   MT - монитор канала, 
                   NON - пассивное состояние */
//TODO:NON заменить на BC (NON - только для ОС2000)
 .wait=WAIT_40, /* время ожидания ОС в режиме BC:
                   WAIT_40 - 40мкс,
                   WAIT_80 - 80 мкс,
                   WAIT_160 - 160мкс ,
                   WAIT_UNLIM - неограниченно */
 .rep=0,      /* разрешение повтора сообщения при неудачном обмене */
 .dev_n=0,    /* номер ОУ (в режиме RT) */
 .grp_en=0,   /* разрешение приема группповых сообщений (RT) */
 .grp_dif=0,  /* (не реализовано)*/
 .mode_wait=1,/* ???режим ожидания завершения обмена данными*/
};

 //Уазатели на:
 mkioIUDesc_t     * pi; //дескриптор ИУ

 mkio_info("enter");

 //Проверим параметры
 mkio_assert_r(device<=3, ME_INVALID_PARAMETER);
 mkio_assert_r(device>=0, ME_INVALID_PARAMETER); //gmu провеим, что указанное число не отрицательное

 //Захватим ИУ
 pthread_mutex_lock(&manch_mutex);
 pthread_mutex_lock(&mkio_mutex[nMKIO]);

 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 //Проверим состояние описателя ИУ
  //ИУ проинициализировано?
 mkio_assert_g(pi->ini==1, DE_DRIVER_SPECIFIC, Error);
 mkio_assert_g(pi->open==1, DE_DRIVER_SPECIFIC, Error);
  //верный дескриптор файла?
 mkio_assert_g(pi->fd>=0, DE_DRIVER_SPECIFIC, Error);

 //сброс ИУ
 mkio_info("FIORESET");
 ioctl(pi->fd, FIORESET, 0 );
 mkio_info("FIOSETMODE401A");
 ioctl(pi->fd, FIOSETMODE401A, (int)&manchMode);
 mkio_info("FIOSETMODE401A end");
 pi->rgm=MANCH_NONE;
 pi->open=0; //файл для ИУ не открыт

 //Закроем файл ИУ
 mkio_info("close");
 close(pi->fd);

//TODO: снимать ли задачи серверов mkioThread[] mkioRTThread[] ???
//      инициализировать ли соответствующие pthread_once_t ???

Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 pthread_mutex_unlock(&manch_mutex);
 mkio_info("exit");
 return ret;
}

