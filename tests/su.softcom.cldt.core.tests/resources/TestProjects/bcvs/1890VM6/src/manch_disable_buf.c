

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
	manch_disable_buf() - отмена буфера чтения по подадресу МКИО-ОУ
*/
int
manch_disable_buf(
   int device, 
   u_char subaddress
 )
{
 return manch_enable_buf(device, subaddress,0);
}

