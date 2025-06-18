
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

void free_batch(mkioBatchDesc_t *p);


/*@
	mkioBatchSend() - выдача одного пакета в МКИО
*/
int
mkioBatchSend(
  mkioBatchDesc_t * p /*указатель на описатель пакета*/
 )
{
 unsigned nMKIO,nMod,nIU;
 mkioIUDesc_t     * pi; //дескриптор ИУ

 //Проверим указатель
 mkio_assert_r(p!=0, -1);
 mkio_assert_r(p->sgn==0x000BA7C1, -2);
 mkio_assert_r(p->nMKIO<=3, -3);
 mkio_assert_r(p->chain.ck_len<=MANCH_BATCH_MAX,-4);


 //Захватим этот пакет
 if(pthread_mutex_lock(&p->mtx)!=0) { //Неверный мьютекс:??
    pthread_mutex_unlock(&p->mtx);   //удалим его??
    free(p);                        //освободим память пакета??
    //free_batch(p);
    return -10;                       //не будем его обрабатывать???
 }

 //Проверим возможность и необходимость запуска пакета
 if( (p->del!=0) && (p->started==0) ) {
    //Пакет не запущен и требует удаления
    p->started=1;                  //чтобы другие не удаляли
    p->del=1;                      //мы его удалим
    p->sgn=0xFFFFFFFF;             //испортим сигнатуру для удаленного пакета
    pthread_mutex_unlock(&p->mtx);//удалим мьютекс       
    mkio_info("free");
    free(p);                     //освободим память пакета
    //free_batch(p);
    return -11;                    //не запускаем
 }
 if(p->started!=0) { //Этот пакет уже кем-то запущен:
    pthread_mutex_unlock(&p->mtx); //освободим пакет
    return -12;                    //кто запустил - тот и удалит
 }

 //Пакет можно и нужно запускать
 p->started=1;   //мы запускаем (захвачен нами)
 p->del=1;       //мы его и удалим
 nMKIO= p->nMKIO;
 nMod = (nMKIO>>1)&1;
 nIU  = nMKIO&1;
 pi   = &mkioDrvDesc.Module[nMod].IUDesc[nIU];

 /////////////////////////
 //Инициализация и запуск ЦК
 mkioBatchCK(pi, p);
 //Анализ результатов функциями обратного вызова
 mkioBatchNotify(pi->bc_func, p);
 /////////////////////////

 //Освобождение памяти
 mkio_info("free");
 //Мы запускали - мы и удалим (p->started не изменять??)
 p->sgn=0xFFFFFFFF;             //испортим сигнатуру для удаленного пакета
 pthread_mutex_unlock(&p->mtx);//удалим мьютекс!!
 free(p);     //освободим память пакета
 //free_batch(p);

 return 0;
}
