
#include <board.h>
#include <string.h>
#include <time.h>
#include <pciLib.h>
#include "tlmBuf.h"


void _bcopy(unsigned from, unsigned to, unsigned size );

//Номер ОЗУ кодируется в младшем бите:
// 0 - ОЗУ1
// 1 - ОЗУ2
//все остальные биты игнорируются

/*
	инициализация телеметрии
	Вызывает pscInit().
*/
int
tlmBufInit(
    int n //=0,1 - номер ОЗУ1,ОЗУ2
)
{
 int res;
// n&=1;

 res=pscInit();
 if(res!=0) return res;

 return 0;
}



/*
	запись заданного массива в текущий свободный банк
*/
void
tlmBufWrite(
    int    n,   /*=0,1 - номер ОЗУ1,ОЗУ2*/
    void * adr, /*адрес начала массива (не обязан быть кратен 4)*/
    size_t size /*размер массива в байтах*/
)
{
 unsigned i;
 unsigned * p;
 psc_1000h_t w0;
 psc_1001h_t w1;
 unsigned *adr_mas = (unsigned*)adr;
 n&=1;

 p=&pscDesc.cpuAdr->TLM[n][0];//Начало ОЗУ телеметрии

 //Копируем байты(!) в ОЗУ1,ОЗУ2
 // memcpy(p, adr, size);
// _bcopy((unsigned)adr,(unsigned)p,size); //gmu: закомментировано, чтобы 2 раза не обращаться по PCI в ОЗУ

 //Переставляем байты в словах(!)
 for(i=0;i<((size+3)>>2);i++) {
 //  p[i]=CT_LE_L(p[i]);
   p[i]=CT_LE_L(adr_mas[i]); // gmu: переворачиваем сразу, чтобы 2 раза не обращаться по PCI в ОЗУ
 }

 if(n==0) {
    w0.wrd=pscRead(0x1000);   
    w0.sA=(size+3)>>2;  /*длина в словах*/    
    pscWrite(0x1000,w0.wrd);  
 }else{
    w1.wrd=pscRead(0x1001);   
    w1.sB=(size+3)>>2;  /*длина в словах*/    
    pscWrite(0x1001,w1.wrd);
 }

}


/*
	чтения признака готовности телеметрии к выдаче
*/
int
tlmBufIsReady(
    int n      /*=0,1 - номер ОЗУ1,ОЗУ2*/
)
{
 int ret=0;
 psc_1000h_t w0;
 psc_1001h_t w1;

 n&=1;

 if(n==0) {
     w0.wrd=pscRead(0x1000);
     ret=w0.sTLMREADY;
 }else{
     w1.wrd=pscRead(0x1001);
     ret=w1.sTLMREADY;
 }

 return ret;
}



/*
	ожидание готовности к выдаче телеметрии
	Периодически засыпает вызовом nanosleep() на заданное время, пока
 не появится готовность в tlmBufIsReady(). 
*/
void
tlmBufWaitReady(
    int n,         /*=0,1 - номер ОЗУ1,ОЗУ2*/
    unsigned delay /*интервал опроса готовности в мкросекундах*/
)
{
 int rdy;
// n&=1;
 rdy=tlmBufIsReady(n); //готовность к выдаче
 while(rdy==0) {   
    udelay(delay); 
    rdy=tlmBufIsReady(n);
 }
}

//gmu: инкремент в функции запуска телеметрии убран по просьбе заказчиков (от 29.02.16)
/*
	запуск выдачи телеметрии
	Одновременно с запуском инкрементирует интегрированный код годности.
*/
int
tlmBufStart(
    int n,      /*=0,1 - номер ОЗУ1,ОЗУ2*/
    size_t size /*размер массива в байтах*/
 )
{
 psc_1000h_t w0;
 psc_1001h_t w1;
 n&=1;

 if(n==0) {
    w0.wrd=pscRead(0x1000);
  //  w0.sVM6LIVEINC++; //gmu: убран по просьбе заказчиков
    w0.sA=(size+3)>>2;  /*длина в словах*/
    w0.sTLMSTARTPCI|=1; /*биты для запуска*/
    pscWrite(0x1000,w0.wrd);

    w0.sTLMSTARTPCI=0; /* сбрасываем признак sTLMSTARTPCI = 0*/ //gmu
    pscWrite(0x1000,w0.wrd);  //gmu
 }else{
    w1.wrd=pscRead(0x1001);
  //  w1.sVM7LIVEINC++;    //gmu: убран по просьбе заказчиков
    w1.sB=(size+3)>>2;  /*длина в словах*/
    w1.sTLMSTARTPCI|=1; /*биты для запуска*/
    pscWrite(0x1001,w1.wrd);

    w1.sTLMSTARTPCI=0; /* сбрасываем признак sTLMSTARTPCI = 0*/ //gmu
    pscWrite(0x1001,w1.wrd);  //gmu
 }

 return 0;
}

/*
	выдача телеметрии
	Вызывает tlmBufWrite(), tlmBufWaitReady(n,10) и tlmBufStart().
*/
void
tlmBufSend(
    int    n,   /*=0,1 - номер ОЗУ1,ОЗУ2*/
    void * adr, /*адрес начала массива*/
    size_t size /*размер массива в байтах*/
 )
{
// n&=1;
 tlmBufWrite(n, adr, size); //переписываем массив
 tlmBufWaitReady(n, 10);    //ждём готовности с интервалом опроса 10мкс
 tlmBufStart(n, size);      //запускаем выдачу
}


//////////////////////////////////////////////////////////////////////////////////////////////
#define UNALIGNED(x) (((unsigned)(x) & (sizeof (unsigned) - 1)))
    
void
_bcopy ( unsigned from,  /* откуда */
         unsigned to,    /* куда   */
         unsigned size   /* длина области в байтах */
 )
{
  char *src = (char*)from;
  char *dst = (char*)to;
  unsigned  *aligned_src;
  unsigned  *aligned_dst;

  while ( UNALIGNED(src) || UNALIGNED(dst) ) {
     if ( size==0 ) return;
     size--;
     *dst++ = *src++;
  }

  aligned_dst = (unsigned *)dst;
  aligned_src = (unsigned *)src;

  while (size >= 16) {
      size -= 16;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
  }

  while (size >= 4) {
      size -= 4;
      *aligned_dst++ = *aligned_src++;
  }

  dst = (char*)aligned_dst;
  src = (char*)aligned_src;
  while (size>0) {
    size-=1;
    *dst++ = *src++;
  }
}

/*
	запись заданного массива в текущий свободный банк, с указанного смещения
        Примечание. Количество пакетов в управляющие регистры не заносится.
*/
void
tlmBufWritePiece(
    int    n,   /*=0,1 - номер ОЗУ1,ОЗУ2*/
    unsigned * adr, /*адрес начала массива */
    size_t size,    /*размер массива в байтах (должен быть кратен 4)*/
    unsigned offset /*смещение в байтах от начала ОЗУ (должно быть кратно 4)*/
)
{
 unsigned i;
 unsigned * p; 
 n&=1;

 p=&pscDesc.cpuAdr->TLM[n][0];//Начало ОЗУ телеметрии
 p+=offset/4; // устанавливаем смещение

 //Копируем байты(!) в ОЗУ1,ОЗУ2, переставляем байты в словах(!)
 for(i=0;i<((size+3)>>2);i++) {
    p[i]=CT_LE_L(adr[i]); // gmu: переворачиваем сразу, чтобы 2 раза не обращаться по PCI в ОЗУ
 }
}

/*
     фунция устанавливает количество пкетов в ОЗУ1/ОЗУ2
*/
void
tlmSetCount(  
    int    n,     /*=0,1 - номер ОЗУ1,ОЗУ2*/
    size_t count /* количество пакетов */)
{
   psc_1000h_t w0;
   psc_1001h_t w1;

    count = (count>=1024)?0:count;

    if(n==0) {
      w0.wrd=pscRead(0x1000);   
      w0.sA=count;  /*длина в словах*/    
      pscWrite(0x1000,w0.wrd);  
    }else{
      w1.wrd=pscRead(0x1001);   
      w1.sB=count;  /*длина в словах*/    
      pscWrite(0x1001,w1.wrd);
    }
}

