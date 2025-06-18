

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include "mram.h"

/*
	запуск процесса записи/чтени€ из/в кэш в/из MRAM
*/
void
mramStart(
  unsigned reg, //=0x1002Е0x1005 - номер регистра управлени€ MRAM
  unsigned wrd, //=0Е0xFFFFF - номер слова в MRAM
  unsigned len, //=1Е512 - количество слов
  unsigned cyc  //=0/1 - однократный/циклический процесс
 )
{
 psc_mram_reg_t w;

 if(len==0) return;

 w.wrd=0;
 w.fld.sMRAMAW=wrd;
 w.fld.sMRAMNW=len; //0 - это 512
 if(cyc==0) { //ќднократный
    w.fld.START=1;
 }else{       //÷иклический
    w.fld.LOOP =1;
 }
 pscWrite(reg,w.wrd);
 
 return;
}
