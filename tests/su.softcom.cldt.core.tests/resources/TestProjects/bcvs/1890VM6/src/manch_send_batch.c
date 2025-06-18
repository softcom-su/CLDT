
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
    manch_send_batch() - передача пакета в МКИО
 ВОЗВРАТ:
  0     - норма;
  иначе - ошибка:
     -1 - вызов manch_start() окончился неудачно,
          переменной errno присвоен код ошибки mq_send();
     -2 - не удалось создать пакет вызовом manch_create_batch();
     -3 - не удалось добавить сообщение в пакет вызовом manch_add_cmd();
     инача - код возврата от manch_start().
*/
int
manch_send_batch(int device,PMANCH_COMMAND buf, u_short count)
{
//int manch_send_batch(int device,MANCH_COMMAND * buf, u_short count)
//int manch_send_batch(int device,void * buf, u_short count)

 //Создание пакета
 int batch=manch_create_batch(device);

 if(batch==-1) {
    return -2; //не удалось создать пакет
 }

 //добавляем все команды в пакет
 if(mkioBatchInit(batch,buf,count)!=0) {
    return -3; //не удалось добавить сообщение в пакет
 }

 //запускаем пакет
 return manch_start(batch);
}




