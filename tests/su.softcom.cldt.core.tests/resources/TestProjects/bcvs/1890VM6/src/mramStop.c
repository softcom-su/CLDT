

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"

/*
	останов текущего процесса записи/чтени€ MRAM
	ќбнул€ет биты START и LOOP заданного регистра.
*/
void
mramStop(
  unsigned int reg //=0x1002Е0x1005 - номер регистра управлени€ MRAM
 )
{
 psc_mram_reg_t w;

 //ќбнуление START и LOOP
 w.wrd=pscRead(reg);
 w.fld.START=0;
 w.fld.LOOP=0;
 pscWrite(reg,w.wrd);
 
 return;
}
