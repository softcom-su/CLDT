
#include <stdio.h>
#include <pciLib.h>
#include "psc.h"


//extern pscDesc_t pscDesc; //описатель PSC

/*
	инициализация PSC
 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        ENODEV - устройство не обнаружено на шине PCI
        ENOMEM - недостаточно памяти в области PCI
*/
int
pscInit(void)
{
  return pscLibInit();
}
