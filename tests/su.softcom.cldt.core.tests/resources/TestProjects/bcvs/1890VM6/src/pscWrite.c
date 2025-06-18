
#include <pciLib.h>
#include "psc.h"

/*
	запись слова в PSC
*/
void
pscWrite(
  unsigned nWord, /*номер слова в адресном пространстве блоков*/
                  /*TLM и RAM2 при доступе к ним через PCI    */
  unsigned Word  /*записываемое слово                         */
 )
{
   PSC_WRITE(nWord,Word);
}
