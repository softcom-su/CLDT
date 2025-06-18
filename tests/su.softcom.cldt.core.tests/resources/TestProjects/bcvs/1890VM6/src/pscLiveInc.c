
#include <pciLib.h>
#include "psc.h"

/*
	инкрементирует интегрированный код годности 
*/
void
pscLiveInc(
 unsigned n /*0 - для 1890ВМ6Я
              1 - для 1890ВМ7Я*/
 )
{
  pscLibLiveInc(n);
}
