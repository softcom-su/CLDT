


#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"

/*
	чтение одного слова из кэш MRAM
	ВНИМАНИЕ! Проверка входных параметров не выполняется.
*/
unsigned
mramReadCache(
  unsigned cpu,  //код процессора = 0/1 - MRAM 1890ВМ6Я/1890ВМ7Я
  unsigned wrd   //=0…0x1FF - номер соответствующего слова в кэш
 )
{
 pscAS_t * p=0;
 unsigned off;

 //Смещение и номер соответствующего слова в кэш
 off=(unsigned)&p->RAM2[2+(cpu&1)][wrd&0x1FF];
 wrd=off>>2;

 //Возвращаем результат из кэш
 return pscRead(wrd);
}
