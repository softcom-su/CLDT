
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
#include <mqueue.h>


/*@
	mkioBatchNotify() - вызов функций обратного вызова для сообщений пакета
*/
void
mkioBatchNotify(
  PNOTIFYFUNC    func,/*указатель на функцию обратного вызова*/
  mkioBatchDesc_t * p /*указатель на описатель пакета*/
 )
{
 int i;
 u_long  cmd,length;
 u_short * buf;
 bt401com * pc; //указатель на описатель команды
 u_short ks,res;

 if( (func!=0) && (p!=0) ) {
    mkio_info("check result");
    //Для каждого сообщения в пакете
    for(i=0;i<p->chain.ck_len;i++) { 
       pc=&p->chain.ck[i];          
       //Определим направление передачи СД
       if((pc->com==RT_BC)||(pc->com==RT_BC_CHAN_A)||(pc->com==RT_BC_CHAN_B)) {
          //Чтение из ОУ:
          buf   =(void*)pc->buf;
          length=pc->size; //< 7- 0> - количество принятых слов данных
          ks=MAKECOMMAND(pc->rt1,MANCH_READ,pc->sa1,pc->size);
       }else{ //Запись в ОУ:
          buf   =0;
          length=0;        //< 7- 0> - количество принятых слов данных
          ks=MAKECOMMAND(pc->rt1,MANCH_WRITE,pc->sa1,pc->size);
       }
          
       //Проверим результат:
       res=pc->res;         
       if( (length==0)                && //Выдача в ОУ         и
           ((res&(1<<12)) ==0 )       && //нет флага ошибки    и 
           ((res&(1<<10)) ==0 )       && //нет ошибки формата  и         
           ((u_short)pc->aw1!=0xFFFF) ){ //есть ответное слово
          return; //нормальная выдача в ОУ - не вызывать func
       }
       //Здесь: чтение из ОУ или выдача в ОУ с ошибкой:
       //Вызываем func()
       cmd=(res<<16)|ks;   //<15- 0> - выдававшееся командное слово
                           //<31-16> - результат: 0 - норма, иначе - ошибка
                           //< 7- 0> - количество принятых слов данных
       length|=i<<8;       //<15- 8> - номер команды в пакете
       length|=pc->aw1<<16;//<31-16> - полученное ответное слово
                           //          (=0xFFFFF - нет ответного слова)
       func((u_long)p, cmd, buf, length);
    }
 }
}
