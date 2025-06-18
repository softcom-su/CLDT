

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <usr401a.h>
#include "libbcvs.h"



#include <board.h>
char *boardModel(void);


unsigned bcvsInit(void)
{

  OPOLOG("\n\n");
  OPOLOG("========================\n");
  OPOLOG("БЦВС. Процессор 1890ВМ6Я\n");
  OPOLOG("------------------------\n");
  OPOLOG("boadrModel:%s\n",boardModel());
  OPOLOG("========================\n\n");


 return 0;
}

