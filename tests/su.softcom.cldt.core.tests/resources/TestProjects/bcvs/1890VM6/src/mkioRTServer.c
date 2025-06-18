
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include <usr401a.h>

//#include <mqueue.h>

#include "libbcvs.h"
#include "manchester.h"

u_short *mkioRTbuf[4][32];

void 
mkioRTServer(int *arg)
{
 int i;
 int nMKIO =  *(arg); //gmu: в качестве агумента передается не переменная а ее адрес
 unsigned nMod =(nMKIO>>1)&1; //модуль МКИО
 unsigned nIU  =nMKIO&1;      //ИУ на модуле
 int ks; //командное слово/тайм-аут в тиках
 u_short sa,cw,tr,sd;
 int ret;
 int er;

 u_short buf[64];
 u_short * pbuf;

 bt401block rs;
 bt401open  ou;


 mkioIUDesc_t * pi=&mkioDrvDesc.Module[nMod].IUDesc[nIU]; //дескриптор ИУ

 PCALLBACKFUNC func;

/*
 SA_MODE_DOUBLE: в локальной памяти контроллера и передатчику и приемнику
 выделяются по два буфера. Один буфер является активным и используется
 аппаратурой ОУ для приема/передачи в/из канала, другой - заполняется/читается
 программой. КК может читать данные порциями, меньшими, чем размер одного
 буфера. "Недочитанные" данные пропадают.
*/
 mkio_info("FIOOPENSA");
 for(i=1; i<31; i++) {
    ou.sa = i;
    ou.mode_rd = SA_MODE_DOUBLE;
    ou.mode_td = SA_MODE_DOUBLE;
    ou.size_rd = 32;
    ou.size_td = 32;
    ret = ioctl(pi->fd, FIOOPENSA, (int)&ou);     
 }

 //Буфер чтения для всех подадресов ОУ один и тот же
 mkio_info("FIOCLRRDBUF");
 ret = ioctl(pi->fd, FIOCLRRDBUF,0); 

 //Буферы записи для всех подустройств(???) ОУ разные.
 mkio_info("FIOCLRWRTBUF");
 for(i=1;i<31;i++) {
    ret = ioctl(pi->fd, FIOCLRWRTBUF,i);//????   
 }

 //записывать в буфер чтения команды "передать данные"
  mkio_info("FIORTSAVEALLCOM");
  ret = ioctl(pi->fd, FIORTSAVEALLCOM,1); 
 //Не устанавливать занятость ОУ???
  mkio_info("FIOBUSYSET");
  ret = ioctl(pi->fd, FIOBUSYSET, 0);    
 for(;;) {
    mkio_info("wait");
    ks=-1; //ждать вечно   
    er=ioctl(pi->fd, FIOWAITCOM, (int)&ks);    
    if(er!=0)  continue;  //ничего не получено
    mkio_info("recieve");     
    //Что-то получено:
    rs.buf=buf; //буфер для получения данных
    er = ioctl (pi->fd, FIOGETBLOCK, &rs); //вынуть из буфера
    sa=rs.sa;    //подадрес
    cw=rs.size;  //длина

    //-----------------------------------------------------
    //ЗАПЛАТКА: ППМ НИИСИРАН не всегда верно формирует КС!
    //  Вставим в КС правильный подадрес:
    ks= (ks&0xFC1F) | ((sa&0x1f)<<5);
    //-----------------------------------------------------

    if( (sa==0) && (cw==0) ) continue; //буфер чтения пуст
    if( sa==0 ) {
       ks=rs.buf[0]; // "передать данные" / "команда управления"
       if(cw==2) sd=rs.buf[1];  //СД полученное с командой управления
    }
    //Анализ полученного ks
    tr=(ks>>10)&0x01; //направление передачи
    //printf("ks = 0x%x \n",ks);  //gmu: для отладки 
 
    sa = ((ks>>5)&0x1f); //gmu: без этого не получится анализировать команды чтения

    if(sa==0x00) continue; //это команда управления
    if(sa==0x1f) continue; //это команда управления
    if(tr==1) { //КК<-ОУ  (Чтение из ОУ)
       mkio_info("read OY");      
       pbuf=mkioRTbuf[nMKIO][sa];      

       if((unsigned)pbuf!=0) { //Буфер зарегестрирован (func не вызывать)
          //Перепишем данные из зарегистрированного буфера в буфер ИУ
          mkio_info("buf enable");  //gmu 
          rs.buf =pbuf;
          rs.sa  =sa;
          rs.size=32;
          ret=ioctl(pi->fd, FIOSETBLOCK, (int)&rs);          
          continue; //???
       }
       mkio_info("buf disable");  //gmu
    }else {mkio_info("write OY");}

    func=pi->rt_func;
    if(func==0) continue;
    //Вызов функции обратного вызова
    ret=func(ks, &buf[0], cw);
    if(ret==0) continue; //Ничего не делать
    if(ret>0) {
       //????????Сделать Trigger на proxy????????
       continue;
    }    
    //Здесь ret<0
    if(tr==1) { //Было КК<-ОУ: подготовим новые данные от func для КК
       cw=-ret; //количество слов в buf от func 
       rs.buf =buf;
       rs.sa  =sa;
       rs.size=cw;
       mkio_info("write buf");  //gmu 
       ioctl(pi->fd, FIOSETBLOCK, (int)&rs);
    }
 }
}




pthread_t mkioRTThread[4];
static pthread_attr_t attr;
static struct sched_param shparam = {.sched_priority = 102}; //?????


///int mkioRT_num=2; //???



void mkioRTServer0_start(void)
{
 mkioRTServer_start(0);
}

void mkioRTServer1_start(void)
{
 mkioRTServer_start(1);
}

void mkioRTServer2_start(void)
{
 mkioRTServer_start(2);
}

void mkioRTServer3_start(void)
{
 mkioRTServer_start(3);
}



int mkioRTn[4]={0,1,2,3};

/*
	mkioRTServer_start() - запуск серевера МКИО-ОУ
*/
void mkioRTServer_start(int i)
{
//--- int i=mkioRT_num;
 if(mkioRTThread[i]!=0) return; //поток уже создан

 //Инициализация аттрибутов 
 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
 pthread_attr_setfpustate(&attr, PTHREAD_FPU_DISABLE);
 pthread_attr_setschedparam(&attr, &shparam);

 //Создание потока
 mkio_info("create mkioRTServer");
 pthread_create(&mkioRTThread[i], &attr, (void*)&mkioRTServer, &mkioRTn[i]);

}



