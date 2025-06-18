

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <usr401a.h>
#include <pthread.h>

#include "libbcvs.h"
#include "manchester.h"



/*
	manch_trans() - обмен информационными словами с ОУ
*/
int
manch_trans(
   int device,      //идентификатор связи с МКИО
   u_short address, //=0-31 - адрес ОУ
   u_short cmd,     //=1-30 - подадрес
   u_short *buf,    //!=0   - слова данных
   u_long length,   //=1-32 - количество слов данных
   u_char np        //направление передачи:
		    //   MANCH_WRITE - KK->ОУ (manch_send)
 )                  //   MANCH_READ  - КК<-ОУ (manch_recive)
{
 int ret=0, er=0, os;

 //Номера:
 unsigned nMKIO=device;       //МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле

 //Уазатели на:
 mkioIUDesc_t     * pi; //дескриптор ИУ

bt401transf tr = {
  .buf=buf,     /* указатель на буфер обмена */
  .rt1=address, /* адрес ОУ1 (от 0 до 30) */
  .sa1=cmd,     /* адрес подустройства 1 (от 1 до 30) */
  .rt2=0,       /* адрес ОУ2 (для обмена данными типа ОУ-ОУ) */
  .sa2=0,       /* адрес подустройства 2 (для обмена данными типа ОУ-ОУ)*/ 
  .size=length, /* количество слов обмена (не более 320) */ 
  .delay=0,     /* величина псевдозадержки после выполнения каждой команды обмена (1 = 10 мкс) */ 
 };

 mkio_info("enter");

 //Проверим параметры
 mkio_assert_r(nMod<=1    , ME_INVALID_PARAMETER);
 mkio_assert_r(nIU <=1    , ME_INVALID_PARAMETER);
 mkio_assert_r(address<=30, ME_INVALID_PARAMETER);
 mkio_assert_r(cmd!=0     , ME_INVALID_PARAMETER);
 mkio_assert_r(cmd<=30    , ME_INVALID_PARAMETER);
 mkio_assert_r(buf!=0     , ME_INVALID_PARAMETER);
 mkio_assert_r(((((int)buf)&1)==0), ME_INVALID_PARAMETER);
 mkio_assert_r(length!=0  , ME_INVALID_PARAMETER);
 mkio_assert_r(length<=32 , ME_INVALID_PARAMETER);
 mkio_assert_r(np<=1      , ME_INVALID_PARAMETER);

 //запретим manch_close()
 mkio_assert_g(pthread_mutex_lock(&mkio_mutex[nMKIO])==0,
               HE_SPECIFIC_7, 
               Error);

 //Проверим описатель ИУ
 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 mkio_assert_g(pi->ini==1, HE_SPECIFIC_0, Error);
 mkio_assert_g(pi->fd >=0, HE_SPECIFIC_1, Error);

 //Выдача сообщения в МКИО
 er=0; ///------
 if(np==MANCH_WRITE) {
    mkio_info("???FIOSEND");
    er=ioctl(pi->fd, FIOSEND, (int)&tr);
 }else{ //MANCH_READ?
    mkio_info("???FIORECEIVE ");
    er=ioctl(pi->fd, FIORECEIVE , (int)&tr);
 }
 if(er!=0) { //Есть ошибка обмена:
    mkio_info("FIOGETSTATUSIO0");
    os=ioctl(pi->fd,FIOGETSTATUSIO,0); //Читаем ответное слово
    //printf("os= 0x%x\n",os); //gmu: для отладки
    if(os==0) { //Нет ОС
       mkio_info("no answer word");
       ret=HE_SPECIFIC_2; //???
       goto Error;
    }else{
       if((os&(RW_BUSY|RW_ABERR|RW_GET_CONTROL|RW_RTERR))!=0) {
          mkio_info("invalid answer word");
          ret=HE_SPECIFIC_3; //???
          goto Error;
       }
    }
 }
// ret=0;

Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 mkio_info("exit");
 return ret;
}
