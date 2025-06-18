

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




//Описатель драйвера МКИО
mkioDrvDesc_t mkioDrvDesc;


pthread_mutex_t manch_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mkio_mutex[4] = {
 PTHREAD_MUTEX_INITIALIZER,
 PTHREAD_MUTEX_INITIALIZER,
 PTHREAD_MUTEX_INITIALIZER,
 PTHREAD_MUTEX_INITIALIZER,
};


pthread_once_t mkioBatchServer_once_control = PTHREAD_ONCE_INIT;//управляющая переменная для запуска функции mkioBatchServer

//управляющие переменные для запуска функции mkioRTServer0-mkioRTServer3
pthread_once_t mkioRTServer0_once_control = PTHREAD_ONCE_INIT;
pthread_once_t mkioRTServer1_once_control = PTHREAD_ONCE_INIT;
pthread_once_t mkioRTServer2_once_control = PTHREAD_ONCE_INIT;
pthread_once_t mkioRTServer3_once_control = PTHREAD_ONCE_INIT;

pthread_once_t mkioDrvInit_once_control = PTHREAD_ONCE_INIT;// управляющая переменная для инициализации драйвера

void mkioDrvInit(void);

/*
	manch_open() - установка связи с драйвером МКИО

 ВОЗВРАТ:
  >=0                 - норма: идентификатор связи с МКИО
  ME_INVALID_PARAMETR - неверные параметры
  ME_INVALID_EXCHANGE - не удалость выполнить без ошибок 
                        bt401Drv(), bt401Init(),bt401DevCreate(), open(), ioctl()   //gmu добавила bt401DevCreate
  DE_DRIVER_SPECIFIC  - дескриптор файла для ioctl() меньше 0
  DE_ALREADY_USED     - недопустимая попытка изменения режима работы МКИО
*/
int
manch_open(
   int device, /*=0,1 - номер устройства (модуля МКИО)*/
   int chanel, /*=0,1 - номер ИУ на устройстве device*/
   u_char rgm  /*=MANCH_BC/MANCH_MT/MANCH_RT... - режим работы МКИО*/
 )
{
 int ret=0;

 //Номера:
 unsigned nMod =device;        //модуля МКИО
 unsigned nIU  =chanel;        //ИУ на модуле
 unsigned nMKIO=(nMod<<1)|nIU; //порядковый номер МКИО

 //Уазатели на:
 mkioDrvDesc_t    * pd; //дескриптор драйвера
 mkioModuleDesc_t * pm; //дескриптор модуля
 mkioIUDesc_t     * pi; //дескриптор ИУ
 char             * pn; //имя файла ИУ

//Режим ИУ для ioctl()
bt401mode manchMode = {
 .mode=NON,     /* режим ИУ:
                   BC - контроллер канала,
                   RT - оконечное устройство,
                   MT - монитор канала, 
                   NON - пассивное состояние */
 .wait=WAIT_40, /* время ожидания ОС в режиме BC:
                   WAIT_40 - 40мкс,
                   WAIT_80 - 80 мкс,
                   WAIT_160 - 160мкс ,
                   WAIT_UNLIM - неограниченно */
 .rep=0,      /* разрешение повтора сообщения при неудачном обмене */
 .dev_n=0,    /* номер ОУ (в режиме RT) */
 .grp_en=1,   /* разрешение приема группповых сообщений (RT) =1 21.11.2013 по просьбе Реутово*/
 .grp_dif=0,  /* (не реализовано)*/
 .mode_wait=1,/* ???режим ожидания завершения обмена данными*/
};
 mkio_info("enter");

 //Проверим параметры
 mkio_assert_r(nMod<=1 ,ME_INVALID_PARAMETER);
 mkio_assert_r(nIU <=1 ,ME_INVALID_PARAMETER);
 mkio_assert_r(rgm <=MANCH_RGM_MAX, ME_INVALID_PARAMETER);

 mkio_assert_g(pthread_mutex_lock(&manch_mutex)==0,
               DE_DRIVER_SPECIFIC, Error);
 mkio_assert_g(pthread_mutex_lock(&mkio_mutex[nMKIO])==0,
               DE_DRIVER_SPECIFIC, Error);

 pthread_once(&mkioBatchServer_once_control, mkioBatchServer_start);
 mkio_assert_g(mkio_mq!=((mqd_t)-1),ME_INVALID_PARAMETER,Error);
 
 
 //Инициализация драйвера
 pd = &mkioDrvDesc;
 pthread_once(&mkioDrvInit_once_control, mkioDrvInit); //динамическая инициализация (функция mkioDrvInit вызывается только 1 раз)
 mkio_assert_g(pd->err==0, ME_INVALID_EXCHANGE, Error);


 //Инициализация модуля
 pm = &mkioDrvDesc.Module[nMod];
 if(pm->ini==0) {
    pm->nMod=nMod;
    pm->offset=0;      //=0 - для модуля БТМ23-401???
    pm->mod=1;         //=1 - для модуля БТМ23-401???тип модуля???
    pm->dev=nMod+1;    //=1,2 - номер PMC-слота, на который установлен модуль???
    pm->interrupt=nMod+1;//=1,2 - номер линии прерывания шины PCI (INTB,INTC)  //gmu: для bcvs 
    /////////////////////////////////////////////////////////////////////////
    mkio_info("bt401Init");   
    pm->module = bt401Init(pm->dev, pm->interrupt, pm->offset, 0, pm->mod);//
    mkio_assert_g(pm->module>=0, ME_INVALID_EXCHANGE, Error);
    pm->IUDesc[0].ini=0;
    pm->IUDesc[1].ini=0;
    pm->ini=1;
 };


 //Инициализация ИУ
 pi = &mkioDrvDesc.Module[nMod].IUDesc[nIU];
 if(pi->ini==0) {
    //Модуль не проинициализирован - ошибка
    mkio_assert_g(pm->module>=0, ME_INVALID_EXCHANGE, Error);
    pi->nMod =nMod;      //номер модуля МКИО
    pi->nIU  =nIU;       //номер ИУ на модуле
    pi->nMKIO=nMKIO;     //порядковый номер МКИО
    pi->rgm  =MANCH_NONE;//режим работы не задан
    pn=&pi->name[0];

    // каждому ИУ присваивается имя /dev/mcn, где n порядковый номер МКИО (/dev/mc0.../dev/mc3)
    // вне зависимости от того какой режим работы МКИО задан в параметрах функци manch_open
    *pn++='/'; *pn++='d'; *pn++='e'; *pn++='v'; *pn++='/'; *pn++='m';  *pn++='c';
    *pn++='0'+nMKIO;
    *pn=0;

    pi->rdBufSize=1024;  //размер внутреннего кольцевого буфера чтения
    pi->wrtSubBufSize=33;//размер внутренних кольцевых буферов записи 

    /////////////////////////////////////////////////////
    mkio_info("bt401DevCreate");
    pd->err = bt401DevCreate(&pi->name[0], pm->module, nIU,  //Проверить возврат??  //gmu: ?? 3 параметр не номер ИУ
                    pi->rdBufSize, pi->wrtSubBufSize); //

    mkio_assert_g(pd->err==0,ME_INVALID_EXCHANGE, Error);
    
    pi->ini=1;
 }

   // После инициализации необходимо открыть файлы на созданных устройствах
 if(pi->open==0){ 
    /////////////////////////////////////////////////////
    pi->fd = open(&pi->name[0], O_RDWR, 0); //дескриптор файла ИУ    
    mkio_assert_g(pi->fd>=0, ME_INVALID_EXCHANGE, Error); 

    pi->open=1;  
  }

 //Выбор режима работы ИУ
 if(rgm==MANCH_NONE) { //перевод в пассивное состояние
    manchMode.mode=BC; //NON - для внутренних целей ОС2000!!!;
    mkio_info("MANCH_NON(BC)");
 }else{
    //Для этого ИУ уже был задан другой режим?
    //(Изменять можно только режим MANCH_NONE)
    mkio_assert_g(pi->rgm==MANCH_NONE, DE_ALREADY_USED, Error);
    if(rgm==MANCH_BC) {
       manchMode.mode = BC;
       mkio_info("MANCH_BC");
    }else if(rgm==MANCH_MT) {
       manchMode.mode = MT;
       mkio_info("MANCH_MT");
    }else{
       manchMode.dev_n = rgm; //АОУ
       mkio_info("MANCH_RT");      
       manchMode.mode = RT;
    }
 } //TODO выяснить почему изменять можно только режим MANCH_NONE

 //Установка режима работы ИУ
 mkio_assert_g(pi->fd>=0, DE_DRIVER_SPECIFIC, Error);
 mkio_info("FIORESET");
 ioctl(pi->fd, FIORESET, 0 ); //сброс ИУ
 mkio_info("FIOSETMODE401A");
 mkio_assert_g(ioctl(pi->fd, FIOSETMODE401A, (int)&manchMode)==0,
               ME_INVALID_EXCHANGE, Error);
 pi->rgm=rgm;
 ret=nMKIO;

 if(manchMode.mode==RT) {
    switch(nMKIO) {
       case 0: pthread_once(&mkioRTServer0_once_control, mkioRTServer0_start);
               break;
       case 1: pthread_once(&mkioRTServer1_once_control, mkioRTServer1_start);
               break;
       case 2: pthread_once(&mkioRTServer2_once_control, mkioRTServer2_start);
               break;
       case 3: pthread_once(&mkioRTServer3_once_control, mkioRTServer3_start);
               break;
    }
 }

Error:
 pthread_mutex_unlock(&mkio_mutex[nMKIO]);
 pthread_mutex_unlock(&manch_mutex);
 mkio_info("exit");
 return ret;
}

//----------------------------------------------------------------------------

/*
   Функция для начальной инициализация драйвера и структур данных
   Вызывает функцию bt401Drv для начальной инициализации драйвера
   
*/
void mkioDrvInit(void)
{
   mkioDrvDesc_t    * pd; //дескриптор драйвера
   pd = &mkioDrvDesc;

   //////////////////////
   mkio_info("bt401Drv");
   pd->err=bt401Drv(); //
   ////////////////////// 
   pd->Module[0].ini=0; //не инициализирован
   pd->Module[1].ini=0; //не инициализирован
   pd->ini=1; 
}



