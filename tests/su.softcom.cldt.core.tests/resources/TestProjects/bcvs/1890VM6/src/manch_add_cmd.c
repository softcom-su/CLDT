


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <usr401a.h>

#include "libbcvs.h"
#include "manchester.h"

#include <arch/interrupt.h>

/*
	manch_add_cmd() - добавляет команду в пакет
  ВОЗВРАТ:
  0 - норма
  ME_INVALID_PARAMETR - неверный параметр
*/
int
manch_add_cmd(
   int batch,       //идентификатор пакета
   u_short flag,    //направление передачи MANCH_READ/MANCH_WRITE
   u_short address, //=0-30 - адрес ОУ
   u_short cmd,     //=1-30 - подадрес
   u_short *buf,    //!=0   - слова данных
   u_long length    //=1-32 - количество слов данных
 )
{
 register int ret=0,i;
 mkioBatchDesc_t * p = (void*)batch;
 bt401com * c;

 mkio_info("enter");

//Проверим входные параметры
 mkio_assert_r((batch&3)==0, ME_INVALID_PARAMETER);
 mkio_assert_r(batch!=0    , ME_INVALID_PARAMETER);
 mkio_assert_r(address<=30 , ME_INVALID_PARAMETER);
 mkio_assert_r(cmd!=0      , ME_INVALID_PARAMETER);
 mkio_assert_r(cmd<=30     , ME_INVALID_PARAMETER);
 mkio_assert_r(buf!=0      , ME_INVALID_PARAMETER);
 mkio_assert_r(((((int)buf)&1)==0), ME_INVALID_PARAMETER);
 mkio_assert_r(length!=0   , ME_INVALID_PARAMETER);
 mkio_assert_r(length<=32  , ME_INVALID_PARAMETER);

//Проверим уакзатель на описатель пакета
 mkio_assert_r(p->sgn==0x000BA7C1, ME_INVALID_PARAMETER);
 mkio_assert_r(p->nMKIO<=3       , ME_INVALID_PARAMETER); 

//Захватим пакет и проверим его
 mkio_assert_g(pthread_mutex_lock(&p->mtx)==0, ME_INVALID_PARAMETER, Error);
 mkio_assert_g(p->started==0, ME_INVALID_PARAMETER, Error); //уже запущен
 mkio_assert_g(p->del==0    , ME_INVALID_PARAMETER, Error); //уже удалён

//Добавим команду в цепочку
 i=p->chain.ck_len++; //новая длина пакета
 
 mkio_assert_g(i<MANCH_BATCH_MAX, ME_INVALID_PARAMETER, Error);

 c=&p->chain.ck[i];   //указатель на текущую команду в пакете

 c->com = (flag==MANCH_WRITE) ? (BC_RT) : (RT_BC) ;
 c->rt1 = address; /* адрес ОУ1*/ 
 c->sa1 = cmd;     /* подадрес ОУ1 */ 
 c->size= length;  /* кол-во слов обмена (1..32) */
 c->buf = (void*)buf;/* указатель на буфер обмена*/
 ret=0; //Готово

Error:
 pthread_mutex_unlock(&p->mtx); //освободим пакет
 mkio_info("exit");
 return ret;
}

