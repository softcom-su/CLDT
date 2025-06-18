

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

int
manch_read_buf(
   int device, 
   u_char subaddress,
   u_long off,
   u_short *buf,
   u_long length
 )
{
 return -ENOSYS; //Не реализована
}

