

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include <usr401a.h>

#include "libbcvs.h"
#include "manchester.h"

/*
   Запись информации в аппаратный буфер
   Функция записывает данные в аппаратный буфер, после чего информация
   становится доступнойдля чтения по указанному подадресу
   Возвращаемое значение:
   0 - норма,
   ME_INVALID_PARAMETER - неправильно задан параметр
   DE_DRIVER_SPECIFIC - не проинициализировано ИУ
   -1 - ошибка при исполнении команды FIOSETBLOCK функцией ioctl

*/
int
manch_write_buf(
   int device,        //
   u_char subaddress, //подадрес
   u_long off,        //=0!
   u_short *buf,      //указатель на область памяти в которой находится записываемая информация
   u_long length      //длина передаваемых данных
 )
{
 int res;
 mkioIUDesc_t     * pi; //дескриптор ИУ

 bt401block bl;


 //Номера:
 unsigned nMKIO=device;       //порядковый номер МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле

 //Проверим параметры
 mkio_assert_r(device<=3     , ME_INVALID_PARAMETER);
 mkio_assert_r(device>=0     , ME_INVALID_PARAMETER);
 mkio_assert_r(subaddress!=0 , ME_INVALID_PARAMETER);
 mkio_assert_r(subaddress<=30, ME_INVALID_PARAMETER);
 mkio_assert_r(buf!=0, ME_INVALID_PARAMETER);
 mkio_assert_r(off==0, ME_INVALID_PARAMETER);
 mkio_assert_r(length!=0 , ME_INVALID_PARAMETER);
 mkio_assert_r(length<=32, ME_INVALID_PARAMETER);

 //Проверим состояние описателя ИУ
 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
  //ИУ проинициализировано?
 mkio_assert_r(pi->ini==1, DE_DRIVER_SPECIFIC);
  //верный дескриптор файла?
 mkio_assert_r(pi->fd>=0, DE_DRIVER_SPECIFIC);

 bl.buf =buf;
 bl.size=length;
 bl.sa  =subaddress;
 res=ioctl(pi->fd, FIOSETBLOCK, (int)&bl );

 return res;
}
