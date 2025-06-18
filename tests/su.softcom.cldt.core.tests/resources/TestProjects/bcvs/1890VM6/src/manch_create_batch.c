
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <usr401a.h>
#include <pthread.h>

#include "libbcvs.h"
#include "manchester.h"

mkioBatchDesc_t* assign_storage_batch(void); // gmu: служебная функция для работы manch_create_batch
mkioBatchDesc_t mkioBatchDesc[1024]; //gmu: зарезервируем место для создания пакетов
unsigned mkioBatchDesc_busy[32];     //gmu: для определения занятости места под пакет в mkioBatchDesc


/*
  manch_create_batch() - создаёт пакет
  ВОЗВРАТ:
  -1    - ошибка;
  иначе - идентификатор созданного пакета.
*/
int
manch_create_batch(
   int device
 )
{
 mkioBatchDesc_t * p;

 mkio_info("enter");
 mkio_assert_r(device<=3, -1);
 mkio_assert_r(device>=0, -1); //gmu:  нужно проверять, что параметр не отрицательное число 

 //Выделим память для пакета
// mkio_assert_r((p=malloc(sizeof(mkioBatchDesc_t))!=0), -1);

 p=malloc(sizeof(mkioBatchDesc_t)); // last_work
// p = assign_storage_batch(); //gmu: вариация вместо malloc
 mkio_assert_r(p!=0, -1);

 p->nMKIO= device;
 p->mtx  = PTHREAD_MUTEX_INITIALIZER;
 p->chain.ck_len=0; //длина пакета
 p->sgn  = 0x000BA7C1;
 p->started = 0; //gmu: это поле тоже надо инициализировать 
 p->del=0;	 //gmu

 mkio_info("exit");
 return (int)p;
}

/*
    assign_storage_batch - выбирает свободное место в массиве mkioBatchDesc
    Функция анализирует mkioBatchDesc_busy, определяет свободное место в массиве mkioBatchDesc
    возвращает адрес для размещения дескриптора пакета.
    ВОЗВРАТ:
    0      - нет места в массиве;
    иниче  - адрес дескриптора пакета. 
 
    Примечание:
    Массив mkioBatchDesc_busy содержит 32 слова, каждый бит в этом слове 
    определяет номер в массиве  mkioBatchDesc. О-е значение бита означает,
    что дескриптор с этим номером не занят и может быть использован.
*/
mkioBatchDesc_t*
assign_storage_batch(void)
{
  int i,j;
  
  // анализируем mkioBatchDesc_busy
  for(i=0;i<32;i++)
  {
     if(mkioBatchDesc_busy[i]!=0xffffffff) 
     {
         for(j=0;j<32;j++)
         {
           if(((mkioBatchDesc_busy[i]>>j)&1)==0) // номер не занят, можно занимать 
           {             
              mkioBatchDesc_busy[i]|=(1<<j); // устанавливаем признак занятости
  //            printf(" create_batch mkioBatchDesc[%d] \n",i*32+j);
  //            printf(" mkioBatchDesc_busy[%d] = 0x%x \n",i,mkioBatchDesc_busy[i]);           
              return &mkioBatchDesc[i*32+j];
           }
         }
     }
  }

 return 0;
}


