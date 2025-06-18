// Файл "sqrtf.h" содержит inline версию функции математической библиотеки float sqrtf( float x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    sqrtf() - функция вычисление квадратного корня числа типа float
//
// Синтаксис:
//    #include <math.h>
//    float sqrtf( float x );
//
// Возвращаемое значение:
//    Функция возвращает значение квадратного корня числа типа float
//
// Особые значения:
//    Eсли аргумент x равен -0.0,   функция возвращает -0.0
//    Eсли аргумент x равен +0.0,   функция возвращает +0.0
//    Eсли аргумент x отрицательный функция возвращает NaN и выставляет код ошибки [EDOM]
//    Eсли аргумент x равен  NaN,   функция возвращает NaN
//    Eсли аргумент x равен -Inf,   функция возвращает NaN и выставляет код ошибки [EDOM]
//    Eсли аргумент x равен +Inf,   функция возвращает аргумент
//
// Ошибки:
//    [EDOM]
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.
//
// Примечания:
//    Если в тексте программы определена константа
//       BT_MATH_INLINE -  представлять математические функции в ввиде inline функций
//    и одна из констант
//       BT_MATH_NORMAL  - аргументы являются конечными нормализованными числами (включая 0.0)
//       BT_MATH_NOCHECK - аргументы являются конечными нормализованными числами (включая 0.0)
//                       из области определения функции
//    то вместо функции sqrtf() будет вызываться вызываться inline функция MATH_INLINE_SQRTF.
//    Функция MATH_INLINE_SQRTF не соответствует полностью стандарту POSIX,
//    так как не обрабатывает все возможные значения аргументов,
//    но зато работает с большей скоростью.

#if( !defined( __MATH_INLINE_SQRTF_H__ ))
#define __MATH_INLINE_SQRTF_H__

#ifdef BT_MATH_INLINE 
#define sqrtf MATH_INLINE_SQRTF
#endif

#include <math_inline/sqrtf_fast.h>

static float __inline__ __UNUSED_F MATH_INLINE_SQRTF( float x )
{
#if( !defined( BT_MATH_NOCHECK ))
   FLT_INT_UNION X = { x }; // двоичное представление аргумента x со знаком

   if( FLT_INT_GE( X, FLT_INT_PLUS_INF )) // проверка диапазона чисел x=[+Inf,+NaN,-Zero,-Sub,-Norm,-Inf,-NaN]
   {
#if( !defined( BT_MATH_NORMAL ))
      // x - NaN (c любым знаком), +Inf или отрицательное число (включая -0.0 и -Inf)
      if( FLT_INT_IS_NAN_OR_INF( X ))
      {
         // x - не число или равно -Inf или +Inf
         if( FLT_INT_IS_MANT_NE_ZERO( X ))
         {
            // x - не число
            return( x ); // возвращаем NaN
         }
         if( FLT_INT_IS_NEG( X ))
         {
            // x = [ -Inf ]
            errno = EDOM;
            return( FLT_INT_NAN.flt ); // возвращаем +NaN
         }
         // x = [ +Inf ]
         return( x );
      }
#endif // BT_MATH_NORMAL
      // x - конечное отрицательное число или -0.0
      if( FLT_INT_EQ( X, FLT_INT_MINUS_ZERO ))
      {
         // x =  -0.0
         return( x );
      }
      // x - строго отрицательное число
      errno = EDOM;
      return( FLT_INT_NAN.flt ); // возвращаем [ +NaN ]
   }
   // x  - неотрицательное конечное число (включая +0.0)
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X )) // x = [ +Zero, +Sub ]
   {
      // x - ненормализованное число или +0.0
      return( +0.0 );
   }
#endif // BT_MATH_NOCHECK
   // x - конечное нормализованное положительное число
   return( MATH_INLINE_SQRTF_FAST( x ));
}

#endif //__MATH_INLINE_SQRTF_H__
