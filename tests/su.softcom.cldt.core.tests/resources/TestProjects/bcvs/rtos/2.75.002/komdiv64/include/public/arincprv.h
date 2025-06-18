/* public/arincprv.h */

#ifndef _PUBLIC_ARINCPRV_H
#define _PUBLIC_ARINCPRV_H

#ifdef KERNEL

#include  <public/difference.h>
#include  <string.h>
#include  <strings.h>
#include  <assert.h>
#include  <sys/apextypes.h>
#include  <private/thread.h>
#if OS_MAJOR >= 3
#  include  <private/kern_libc.h>
#  include  <private/copymem.h>
#  define   ARINC_TIME_MAX TIME_MAX
#else
#  include  <ctype.h>
#endif

/* Проверка, указывает ли аргумент бесконечное время */
#define arncTIME_IS_INFINITE(t)    ((t)<=INFINITE_TIME_VALUE)
/* Проверка, указывает ли аргумент нулевое время */
#define arncTIME_IS_NULL(t)        ((t)==0)
/* Проверка, указывает ли аргумент допустимое время */
#define arncTIME_IS_VALID(t)       ( t != ARINC_TIME_MAX )
/* Преобразование времени из формата ARINC в тики */
#define arncTIME_TO_TICKS(t) \
  SYSTEM_TIME_TO_TICKS( curproc->p_Clocks[CLOCK_REALTIME]->pc_ModClock, t )
/* Преобразование времени, указанного в тиках, в формат ARINC */
#define arncTICKS_TO_TIME(t) \
  (curproc->p_Clocks[CLOCK_REALTIME]->pc_ModClock->mc_Res * t)

/* Конфигурационные макроорределения, определяющие старт периодического потока
 * после DEADLINE
 */
#define START_NEW_PERIOD    0   /* если момент старта текущего периода пропущен,
                                   поток стартует в следующем периоде          */
#define START_CUR_PERIOD    1   /* если момент старта текущего периода пропущен,
                                   поток не прерывает выполнения               */

/* ---------- athreadBlockCheck -------------------
 * Функция athreadBlockCheck  – проверка допустимости блокирования
 *                              потока в ожидании ресурса
 * Аргумент:
 *   timeout         - максимальное время ожидания.
 * Возвращаемые значения:
 *   [NO_ERROR]	     - блокирование возможно;
 *   [INVALID_MODE]	 - значение аргумента time_out не равно 0,
 *	                   в то время как запрещено планирование
 *	                   или вызывающий поток является потоком обработки ошибок;
 *	 [NOT_AVAILABLE] - аргумент time_out равен 0;
 *	 [INVALID_PARAM] - аргумент time_out имеет некорректное значение.
 */
static __inline int __UNUSED_F athreadBlockCheck( int64_t timeout )
{

   if( arncTIME_IS_NULL(timeout) )
      return NOT_AVAILABLE; /* аргумент time_out равен 0                       */

#if OS_MAJOR >= 3
   if( procIS_ARINC && !procMODE_IS_NORMAL() ) {
         /* Поток нельзя устанавливать в очередь на этапе инициализации */
         return INVALID_MODE;
   }
   if( aprocIS_LOCK_PREEMPTION() )
      return INVALID_MODE;  /* планирование запрещено                          */

   if( athreadIS_ERRORHANDLER(threadExecuting) )
      return INVALID_MODE;  /* текущий поток является потоком обработки ошибок */
#endif
   return NO_ERROR;
}

/* ---------- arincNameCopy -------------------
 * Функция arincNameCopy  – получение имени, отвечающего требованиям ARINC
 * Аргумент:
 *   from - указатель на имя;
 *   to   - указатель на буфер, принимающий преобразованное имя.
 */
static __inline__ void arincNameCopy( const char *from, char *to )
{
   int i;
   for(i=0; *from && (i < MAX_NAME_LENGTH); to++,from++,i++ )
   {
      *to = kern_toupper(*from);
   }
   *to = '\0';
}

/* ---------- arincNameCheck -------------------
 * Функция arincNameCheck  – проверка имени
 * Аргумент:
 *   name      - указатель на имя;
 *   newname   - указатель на буфер, принимающий преобразованное имя.
 * Возвращаемые значения:
 *  Функция возвращает 0, если имя отвечает требованиям стандарта.
 *
 *   Размер предоставляемого буфера должен быть больше MAX_NAME_LENGTH.
 */
static __inline int __UNUSED_F arincNameCheck( char* name, char *newname )
{
   if( name == NULL )
      return EFAULT;

#if OS_MAJOR >= 3
   if( memcheck_s(curpid,(caddr_t)name,MAX_NAME_LENGTH, msflagsREADABLE) )
      return EFAULT;
#endif
   if( name[0] == '\0' )
      return EINVAL;

   arincNameCopy( name, newname );
   return 0;
}


#endif /*KERNEL*/

#endif /* _PUBLIC_ARINCPRV_H */
