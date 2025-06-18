
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


void free_batch(mkioBatchDesc_t *p); //gmu: эта функция служебная, используется только в mkioBatchServer

mqd_t mkio_mq;

/*
	mkioBatchServer() - один поток многопотокового серевера МКИО-КК
*/
void mkioBatchServer(void)
{
 ssize_t siz; 
 mkioBatchDesc_t * p;
 unsigned ret;


 for(;;) {
 Contin:
    mkio_info("wait");
    //Принимаем пакет
    siz=mq_receive(mkio_mq, (char *)&p, sizeof(p), NULL);

    //Проверим результаты приёма
    mkio_info("recieve");
    mkio_assert_g(siz==4, 0, Contin);
    mkio_assert_g(p!=0, 0, Contin);
    mkio_assert_g(p->sgn==0x000BA7C1, 0, Contin);
    mkio_assert_g(p->nMKIO<=3, 0, Contin);

    //Запустим принятый пакет
    mkio_info("mkioBatchSend");
    mkioBatchSend(p);
 }
}


pthread_t mkioThread[MKIO_SERVERS];

static pthread_attr_t attr;
static struct sched_param shparam = {.sched_priority = 101}; //?????

struct mq_attr mqa = {
    .mq_flags  = 0,
    .mq_maxmsg =2*MKIO_SERVERS, //???макс.кол-во сообщений в очереди
    .mq_msgsize= 4,
    .mq_curmsgs= 0,
 };


/*
	mkioBatchServer_start() - запуск многопотокового серевера МКИО-КК
*/
void mkioBatchServer_start(void)
{
 int i;

 //Открываем очередь на чтение и запись
 mkio_info("open /mkio_batch (RW)");
 mkio_mq=mq_open("/mkio_batch", O_CREAT|O_RDWR,0,&mqa);
 if(mkio_mq==((mqd_t)-1)) {
    return;
 }

 //Инициализация аттрибутов 
 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
 pthread_attr_setfpustate(&attr, PTHREAD_FPU_DISABLE);
 pthread_attr_setschedparam(&attr, &shparam);  

 //Создание потоков
 mkio_info("create mkioBatchServer's");
 for(i=0;i<MKIO_SERVERS;i++) {       
    //pthread_attr_setschedparam(&attr, &shparam);  //gmu:  каждому свой приоритет
    pthread_create(&mkioThread[i], &attr, (void*)&mkioBatchServer, 0);   
    //shparam.sched_priority +=1; 
 }

}



/*
  free_batch - освобождает место под пакет в массиве mkioBatchDesc
  Функция сбрасывает бит соответствующий p в массиве mkioBatchDesc_busy.
  Если указанный в параметрах адрес не определяет ни один из элементов массива,
  то функция ничего не делает.
*/
void 
free_batch(mkioBatchDesc_t *p)
{
   int i;
   for(i=0;i<1024;i++)
   {
      if(p==&mkioBatchDesc[i])
      {       
        mkioBatchDesc_busy[i/32] &= ~(1<<(i%32)); // сбрасываем соответсвующий бит        
     //   printf(" %d : mkioBatchDesc_busy[%d] = 0x%x \n",i,i/32,mkioBatchDesc_busy[i/32]);
        break;
      }
   }
}
 
