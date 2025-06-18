

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"
#include <pthread.h>

/*
	запись массива слов в MRAM
	Для предотвращения одновременного доступа разных потоков к MRAM
 использует мьютекс mram_mutex_wr.
 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        EADDRNOTAVAIL - неверный код процессора
        EINVAL - неверное значение from,to или size
        ENOMEM - выход конца массива за процессорную границу MRAM
        EBUSY  - ошибка: нет готовности MRAM
*/
int
mramWrite(
  unsigned cpu,  //код процессора = 0/1 - MRAM 1890ВМ6Я/1890ВМ7Я
  void    *from, //указатель на записываемый массив
  unsigned to,   //=0…0xFFFFF - номер первого слова массива в MRAM
  unsigned size  //=0,4,8,…,0x400000 - размер массива в байтах
 )
{
 unsigned * ufr=from;
 int ret=0;

 unsigned reg=0x1002+(cpu&1); //0x1002 или 0x1003;
 unsigned len;
 int i;

 //Проверим параметры
 if(cpu>=2) {
    errno = EADDRNOTAVAIL;
    return -1;
 }
 if((((int)from|size)&3)!=0) {
    errno = EINVAL;
    return -1;
 }
 if(to>0xFFFFF) {
    errno = EINVAL;
    return -1;
 }
 if(size>0x400000) {
    errno = EINVAL;
    return -1;
 }
 if(((to*4)+size)>0x400000) { //Выход конца массива за процессорную границу
    errno = ENOMEM;
    return -1;
 }

 //Ждём возможности
 pthread_mutex_lock(&mram_mutex_wr); //Захватываем MRAM
 mramStop(reg);                      //Останавливаем (и не ждём здесь) //gmu: возможно здесь это лишнее

 //Перепишем массив ОЗУ->кэш->MRAM
 mram_flag=0;
 while(size>0) {
    len=min(512,size/4);           //Очередной кусок до 512 слов
    mramWaitReady(reg);            //Ожидание остановки
    mramStop(reg);                 //gmu: перед стартом надо сбросить
    if(mram_flag!=0) break;        //ошибка --> выход
    for(i=0; i<len; i++) {         //Копируем ОЗУ->кэш
       mramWriteCache(cpu,i,ufr[i]);
    }
    mramStart(reg,to,len,0);       //Копируем кэш->MRAM
    ufr +=len;
    to  +=len;
    size-=len*4;
 }

 //Анализ результата
 if(mram_flag!=0) {
    mramStop(reg);
    errno = EBUSY;
    ret=-1;
 }else{ //Ожидание полной остановки
    mramWaitReady(reg); //Не отдаим мьютекс, пока не закончится запись
 }

 //Освободим MRAM для других потоков
 pthread_mutex_unlock(&mram_mutex_wr);

 return ret;
}
