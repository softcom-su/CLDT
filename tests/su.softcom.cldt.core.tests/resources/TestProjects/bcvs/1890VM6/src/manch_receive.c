

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
	manch_receive() - чтение информационных слов из ОУ
*/
int
manch_receive(
   int device,      //идентификатор связи с МКИО
   u_short address, //=0-30 - адрес ОУ
   u_short cmd,     //=1-30 - подадрес
   u_short *buf,    //!=0   - буфер для слов данных
   u_long length    //=1-32 - количество слов данных
 )
{
 int ret;
 mkio_info("enter");
 ret=manch_trans(device,address,cmd,buf,length,MANCH_READ);
 mkio_info("exit");
 return ret;
}

