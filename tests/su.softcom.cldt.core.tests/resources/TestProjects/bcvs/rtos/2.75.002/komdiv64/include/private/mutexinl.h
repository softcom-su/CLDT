/*  private/mutexinl.h  */

#ifndef _PRIVATE_MUTEXINL_H
#define _PRIVATE_MUTEXINL_H

#include   <private/mutex.h>
#include   <private/kernel.h>


/* если мьютекс инициализирован статически, производится
 * реальная инициализация
 */
static __inline int __UNUSED_F mtxInitializer( pthread_mutex_t *mutex )
{
   if( mtxIS_INITIALIZER(mutex) )
   {
      if( pthread_mutex_init( mutex, NULL ) != 0 )
         return ENOMEM;
      return 0;
   }
   return EINVAL;
}

/*----------- mtxGetObject --------------
 * Функция mtxGetObject - получение адреса мьютекса (объекта)
 *                        по адресу идентификатора
 * Аргумент:
 *   rmutex - адрес идентификатора мьютекса.
 * Возвращаемые значения.
 * Функция возвращает указательна описатель мьютекса или NULL,
 * если обнаружены ошибки:
 *  - аргумент указывает на некорректный идентификатор мьютекса.
 *
 * При возврате управления планирование запрещено, если mutex существует
 */
static __inline __UNUSED_F mtxCONTROL* __UNUSED_F mtxGetObject ( pthread_mutex_t *rmutex )
{
   mtxCONTROL* mtx; /* указатель на описатель мьютекса                        */
   int rt;

   kernDispatchDisable (); /* запрет планирования                                    */
   if( (rt=mtxInitializer( rmutex )) == 0 )
   return *rmutex; /*успешная инициализация*/
   /*либо инициализация не прошла (ENOMEM), либо не стат.иниц.(EINVAL)*/
   if( rt == ENOMEM )
   {
      kernDispatchEnable (); /* разрешение планирования            */
      return NULL; /* некорректный идентификатор         */
   }
   /*проверяем мьютекс*/
   if( rmutex == NULL || *rmutex == NULL )
   {
      kernDispatchEnable (); /* разрешение планирования            */
      return NULL; /* некорректный идентификатор         */
   }
   mtx = *rmutex;
   if ( objCHECK( mtxObjControlId, mtx ) != 0 )
   {
      kernDispatchEnable (); /* разрешение планирования            */
      return NULL; /* некорректный идентификатор         */
   }
   return mtx;
}

#endif /* _PRIVATE_MUTEXINL_H */
