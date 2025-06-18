

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
	manch_send_com() - послать команду управления интерфейсом

	Для команд управления со словом данны передаваемое слово должно`
 быть размещено в buf[0]; полученное слово данных будет резмещено в buf[0];
 ответное слово будет резмещено в buf[0] только для команды управления`
 "Передать ответное слово".

 ВОЗВРАТ:
 0     - норма;
 иначе - ошибка.
*/
int
manch_send_com(
   int device,      //идентификатор связи с МКИО
   u_short address, //=0-31 - адрес ОУ
   u_short cod,     //=0-31 - код команды управления c режимом управления 0
                    //=32-63 - код команды управления c режимом управления 0x1F (0x20|(код команды))
   u_short *buf     //!=0   - указатель на слово данных команды
 )
{
 int ret=0, er=0;

 //Номера:
 unsigned nMKIO=device;       //МКИО
 unsigned nMod =(nMKIO>>1)&1; //модуля МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле

 //Уазатели на:
 mkioIUDesc_t     * pi; //дескриптор ИУ

 bt401ctrl ku = {
   .com=cod,
   .dev=address,
   .data=0
 };

 //Проверим параметры
 mkio_assert_r(nMod<=1    , ME_INVALID_PARAMETER);
 mkio_assert_r(nIU <=1    , ME_INVALID_PARAMETER);
 mkio_assert_r(address<=31, ME_INVALID_PARAMETER);
 mkio_assert_r(cod<=63    , ME_INVALID_PARAMETER);  // gmu: изменены границы 

 if(buf!=0) {
   mkio_assert_r(((((int)buf)&1)==0), ME_INVALID_PARAMETER);
   ku.data=buf[0];
 }

 //запретим manch_close()
 mkio_assert_g(pthread_mutex_lock(&mkio_mutex[nMKIO])==0,
               HE_SPECIFIC_0, 
               Error);

 //Проверим описатель ИУ
 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 mkio_assert_g(pi->ini==1, HE_SPECIFIC_1, Error);
 mkio_assert_g(pi->fd >=0, HE_SPECIFIC_2, Error);

 //Выдача команды управления в МКИО
 er=ioctl(pi->fd, FIOSENDCOM, (int)&ku);
 if(er!=0) {
    ret=HE_SPECIFIC_3;
 }

 if(buf!=0) {
    switch(cod&0x1F) {//Вернём ОС или полученное слово данных  // gmu: добавлена маска 0x1F
       case GIVEOS:      buf[0]=ku.data; break;
       case GIVEVECWORD: buf[0]=ku.data; break;
       case GIVELASTCOM: buf[0]=ku.data; break;
       case GIVEVSK:     buf[0]=ku.data; break;
       case LOCKi:       buf[0]=ku.data; break;
       case UNLOCKi:     buf[0]=ku.data; break;
    }
 }


Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 mkio_info("exit");
 return ret;
}
