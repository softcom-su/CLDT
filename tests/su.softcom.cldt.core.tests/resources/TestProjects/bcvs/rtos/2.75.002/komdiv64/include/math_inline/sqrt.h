// Файл "sqrt.h" содержит inline версию функции математической библиотеки double sqrt( double x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    sqrt() - функция вычисление квадратного корня числа типа double
//
// Синтаксис:
//    #include <math.h>
//    double sqrt( double x );
//
// Возвращаемое значение:
//    Функция возвращает значение квадратного корня числа типа double
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
//    то вместо функции sqrt() будет вызываться вызываться inline функция MATH_INLINE_SQRT.
//    Функция MATH_INLINE_SQRT не соответствует полностью стандарту POSIX,
//    так как не обрабатывает все возможные значения аргументов,
//    но зато работает с большей скоростью.

#if( !defined( __MATH_INLINE_SQRT_H__ ))
#define __MATH_INLINE_SQRT_H__

#ifdef BT_MATH_INLINE 
#define sqrt MATH_INLINE_SQRT
#endif

#include <math_inline/sqrt_fast.h>

static double __inline__ __UNUSED_F MATH_INLINE_SQRT( double x )
{
#if( !defined( BT_MATH_NOCHECK ))
   DBL_INT_UNION X = { x }; // двоичное представление аргумента x со знаком

   if( DBL_INT_GE( X, DBL_INT_PLUS_INF )) // проверка диапазона чисел x=[+Inf,+NaN,-Zero,-Sub,-Norm,-Inf,-NaN]
   {
#if( !defined( BT_MATH_NORMAL ))
      // x - NaN (c любым знаком), +Inf или отрицательное число (включая -0.0 и -Inf)
      if( DBL_INT_IS_NAN_OR_INF( X ))
      {
         // x - не число или равно -Inf или +Inf
         if( DBL_INT_IS_MANT_NE_ZERO( X ))
         {
            // x - не число
            return( x ); // возвращаем NaN
         }
         if( DBL_INT_IS_NEG( X ))
         {
            // x = [ -Inf ]
            errno = EDOM;
            return( DBL_INT_NAN.dbl ); // возвращаем +NaN
         }
         // x = [ +Inf ]
         return( x );
      }
#endif // BT_MATH_NORMAL
      // x - конечное отрицательное число или -0.0
      if( DBL_INT_EQ( X, DBL_INT_MINUS_ZERO ))
      {
         // x =  -0.0
         return( x );
      }
      // x - строго отрицательное число
      errno = EDOM;
      return( DBL_INT_NAN.dbl ); // возвращаем [ +NaN ]
   }
   // x  - неотрицательное конечное число (включая +0.0)
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X )) // x = [ +Zero, +Sub ]
   {
      // x - ненормализованное число или +0.0
      return( +0.0 );
   }
#endif // BT_MATH_NOCHECK
   // x - конечное нормализованное положительное число
   return( MATH_INLINE_SQRT_FAST( x ));
}

#endif //__MATH_INLINE_SQRT_H__
