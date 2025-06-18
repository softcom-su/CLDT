

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"

/*
	запись одного слова в кэш MRAM
*/
void
mramWriteCache(
  unsigned cpu,  //код процессора = 0/1 - MRAM 1890ВМ6Я/1890ВМ7Я
  unsigned wrd,  //=0…0x1FF - номер соответствующего слова в кэш
  unsigned val   //записывемое значение
 )
{
 pscAS_t * p=0;
 unsigned off;

 //Смещение и номер слова в кэш
 off=(unsigned)&p->RAM2[0+(cpu&1)][wrd&0x1FF];
 wrd=off>>2;

 //Запись слова в кэш
 pscWrite(wrd,val);
}
