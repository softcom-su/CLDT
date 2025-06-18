

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "libbcvs.h"
#include "manchester.h"

/*
	manch_notify_detach() - удаление функции обратного вызова
*/
int
manch_notify_detach(
   int device,   //идентификатор связи с МКИО
   int cookie    //от manch_notify_attach()
 )
{
 //Проверим параметры
 mkio_assert_r(device==cookie, ME_INVALID_PARAMETER);

 return manch_notify_attach(device,0,0);
}

