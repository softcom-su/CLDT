

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"
#include <pthread.h>



/*
	Чтение массива слов из MRAM
	Для предотвращения одновременного доступа разных потоков к MRAM
 использует мьютекс mram_mutex_rd.

 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        EADDRNOTAVAIL - неверный код процессора
        EINVAL - неверное значение from, to или size
        ENOMEM - выход конца массива за процессорную границу MRAM
        EBUSY  - ошибка: нет готовности MRAM
*/
int
mramRead(
  unsigned cpu,   //код процессора = 0/1 - MRAM 1890ВМ6Я/1890ВМ7Я
  unsigned from,  //=0…0xFFFFF - номер первого слова массива в MRAM
  void    *to,    //указатель на область для считанного массива
  unsigned size   //=0,4,8,…,0x400000 - размер массива в байтах
 )
{
 unsigned * uto=to;
 int ret=0;

 unsigned reg=0x1004+cpu; //0x1004 или 0x1005
 unsigned len;
 int i;

 //Проверим параметры
 if(cpu>=2) {
    errno = EADDRNOTAVAIL;
    return -1;
 }
 if(((size|((int)to))&3)!=0) {
    errno = EINVAL;
    return -1;
 }
 if(from>0xFFFFF) {
    errno = EINVAL;
    return -1;
 }
 if(size>0x400000) {
    errno = EINVAL;
    return -1;
 }
 if(((from*4)+size)>0x400000) { //Выход конца массива за процессорную границу
    errno = ENOMEM;
    return -1;
 }


 //Ждём возможности
 pthread_mutex_lock(&mram_mutex_rd);//Захватываем MRAM
 mramStop(reg);                     //Останавливаем
 mramWaitReady(reg);                //Ожидание полной остановки

 //Перепишем массив MRAM->кэш->ОЗУ
 mram_flag=0;
 while(size>0) {
    len=min(512,size/4);           //Очередной кусок до 512 слов
    mramStart(reg,from,len,0);     //Копируем MRAM->кэш
    mramWaitReady(reg);            //Ожидание окончания копирования
    mramStop(reg);                 //gmu: перед следующим стартом надо сбросить
    if(mram_flag!=0) break;        //ошибка --> выход
    for(i=0; i<len; i++) {         //Копируем кэш->ОЗУ
       uto[i]=mramReadCache(cpu,i);
    }
    uto +=len;
    from+=len;
    size-=len*4;
 }

 //Анализ результата
 if(mram_flag!=0) {
    mramStop(reg);
    errno = EBUSY;
    ret=-1;
 }

 //Освободим MRAM для других потоков
 pthread_mutex_unlock(&mram_mutex_rd);

 return ret;
}
