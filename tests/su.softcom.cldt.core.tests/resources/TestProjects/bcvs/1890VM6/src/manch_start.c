

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
#include <mqueue.h>



/*
	manch_start() - запуск пакета МКИО
  ВОЗВРАТ:
  0  - норма;
  -1 - ошибка, переменной errno присвоен код ошибки mq_send().
*/
int
manch_start(
   int batch      //идентификатор пакета от manch_create_batch()
 )
{
 int ret;
 mkio_info("enter");

 //TODO:проверить пакет???
 ret=mq_send(mkio_mq, (void*)&batch,4,0);

 mkio_info("exit");
 return ret;
}

