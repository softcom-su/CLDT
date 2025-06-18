

#include <stdio.h>
#include <pciLib.h>
#include "psc.h"
#include <pthread.h>


pthread_mutex_t mram_mutex_rd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mram_mutex_wr = PTHREAD_MUTEX_INITIALIZER;

volatile unsigned mram_flag;

/*
	инициализация контроллера MRAM
	Устанавливает в исходное состояние аппаратуру контроллера MRAM.
	Вызывает pscInit().
	ВНИМАНИЕ! Содержимое MRAM не изменяет.
 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        ENODEV        - pscInit(): устройство не обнаружено на шине PCI
        ENOMEM        - pscInit(): недостаточно памяти в области PCI
        EADDRNOTAVAIL - неверный код процессора
*/
int
mramInit(
  unsigned cpu //= 0/1 код MRAM процессора 1890ВМ6Я/1890ВМ7Я
 )
{
 int res;

 unsigned nRegWrd;

 if(cpu>=2) {
    errno = EADDRNOTAVAIL;
    return -1;
 }

 res=pscInit();
 if(res!=0) return res;

 nRegWrd=0x1002+cpu; // 0x1002 или 0x1003

 pscWrite(nRegWrd,0);
 pscWrite(nRegWrd+2,0);

 mram_flag=0;
  
 return 0;
}
