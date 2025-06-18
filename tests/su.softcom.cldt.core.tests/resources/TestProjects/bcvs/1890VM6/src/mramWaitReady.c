

#include "psc.h"
#include "mram.h"

#include <time.h>
#include <board.h>

/*
	ожидание готовности MRAM
	ВНИМАНИЕ! Маску прерывний не изменяет. Мьютекс mram_mutex не 
 использует. Пауза 25мкс между опросами организованы вызовом udelay().
 ВОЗВРАТ:
  0 - норма, есть готовность
 -1 - ошибка, готовности не дождались; флаг mram_flag установлен
*/
int
mramWaitReady(
  unsigned reg //=0x1002…0x1005 - номер регистра управления MRAM

)
{
 int i;
 int ret=0; //норма
 psc_mram_reg_t w;
 udelay(1); // gmu: 1 мкс, пауза для достоверной проверки ready

 for(i=0;i<600;i++) { //~15мс (600 = 15мс/25мкс)
    w.wrd=pscRead(reg);
    if(w.fld.READY!=0) break;
    udelay(25); 
 }

 if(w.fld.READY==0) {
    mram_flag|=1;
    ret=-1; //ошибка
 }

 return ret;
}
