/* mpspin.h*/
#ifndef _MPSPIN_H
#define _MPSPIN_H

#include <sys/types.h>

typedef mpoffset_t spin_t;
/*#define pthread_spinlock_t spin_t */

#define SPIN_FAILED  ((spin_t)-1)
/*#define SPIN_INITDEFER  ((spin_t)0) */

/* захват спина с запретом прерываний */
int spinIntLock( spin_t spin, int* lvl );

/* освобождение спина с разрешением прерываний */
int spinIntUnlock( spin_t spin, int lvl );

/* опрос размера спина */
int spinGetSize( void );

/* получение спина из списка */
void *spinGet( void );

/*  возвращение спина в список */
int spinFree( void *p );

/*--------------------------------------------------*/
/*BSP may call the following OS functions in spin support
  functions:
*/

/* инициализация модуля: создание списка свободных спинов
   n - число спинов 
*/
int mpSpinInit( int n );

/* открытие спина */
spin_t spinOpen( void );

/* закрытие спина */
int spinClose( spin_t );


#endif /*_MPSPIN_H*/
