

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
	manch_send() - передача информационных слов в ОУ
 ВОЗВРАТ:
  0     - норма;
  иначе - ошибка (от manch_trans()).
*/
int
manch_send(
   int device,      //идентификатор связи с МКИО
   u_short address, //=0-30 - адрес ОУ
   u_short cmd,     //=1-30 - подадрес
   u_short *buf,    //!=0   - слова данных
   u_long length    //=1-32 - количество слов данных
 )
{
 int ret;
 mkio_info("enter");
 ret=manch_trans(device,address,cmd,buf,length,MANCH_WRITE);
 mkio_info("exit");
 return ret;
}
