
#include <pciLib.h>
#include "psc.h"

/*
	чтение слова из PSC
*/
unsigned
pscRead (
 unsigned nWord /*номер слова в адресном пространстве блоков*/
 )              /*TLM и RAM2 при доступе к ним через PCI    */
{
 return PSC_READ(nWord);
}
