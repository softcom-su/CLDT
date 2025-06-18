// Файл:
//    "scalb.h" - содержит inline функцию математической библиотеки.
//       double scalb( double x, double n ).
//
// Описание:
//    scalb() - функция вычисление числа типа double по мантиссе и порядку
//
// Синтаксис:
//    double scalb( double x, double n );
//
// Возвращаемое значение:
//    Функция возвращает значение x * 2^n (т.к. FLT_RADIX = 2).
//
// Особые значения:
//    Если аргумент x или n равен NaN, то функция возвращает NaN.
//    Если аргумент n равен -0.0 или +0.0, то функция возвращает аргумент x.
//    Если аргумент x равен -Inf или +Inf, а аргумент n не равен -Inf
//       то функция возвращает аргумент x.
//    Если аргумент x равен -0.0 или +0.0, а аргумент n не равен +Inf,
//       то функция возвращает аргумент x.
//    Если аргумент x равен -0.0 или +0.0, а аргумент n равен +Inf,
//       то функция возвращает NaN и переменной errno будет присвоено значение [EDOM].
//    Если аргумент x равен -Inf или +Inf, а аргумент n равен -Inf
//       то функция возвращает NaN и переменной errno будет присвоено значение [EDOM].
//    Если в результате было получено слишком большое по абсолютной величине число,
//       то будет возвращено плюс или минус HUGE_VAL в зависимости от знака числа и
//       переменной errno будет присвоено значение [ERANGE].
//    Если в результате преобразования было получено слишком маленькое
//       по абсолютной величине число, которое не может быть представлено в
//       в виде нормализованного числа, то будет возвращено либо ненормализованное
//       число либо 0.0 (если его нельзя представить в виде ненормализованного
//       числа) и переменной errno будет присвоено значение [ERANGE].
//    Если exp не является целым числом, не равно NaN или бесконечности,
//       то результат не определен.
//
// Ошибки:
//    [EDOM], [ERANGE].
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_SCALB_H__ ))
#define __MATH_INLINE_SCALB_H__

#include <math_inline/copysign.h>
#include <math_inline/ldexp_fast.h>
#include <strings.h> // описание функций fmsbs64_1() ...

#ifdef BT_MATH_INLINE
#define scalb MATH_INLINE_SCALB
#endif // BT_MATH_INLINE

static double __inline__ __UNUSED_F MATH_INLINE_SCALB( double x, double n )
{
   DBL_INT_UNION X = { x }; // целочисленное представления числа типа double со знаком
   DBL_INT_UNION N = { n }; // целочисленное представления числа типа double со знаком
   int n_sub = 0; // целочисленная поправка к степени для ненормализованного числа

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( isunordered( x, n ))
   {
      // Хотя бы один из аргументов не число
      return( DBL_INT_NAN.dbl ); // возвращаем NaN
   }
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      // x = [ -Inf, +Inf ]
      if( DBL_INT_EQ( N, DBL_INT_MINUS_INF ))
      {
         // n равен -Inf
         errno = EDOM;
         return( DBL_INT_NAN.dbl );
      }
      return( x );
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   // x не равно NaN или бесконечности
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( N ))
   {
      // n равно плюс или минус нулю
      return( x );
   }
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // x - ненормализованное число или 0.0
#if( !defined( BT_MATH_NOCHECK ) && !defined( BT_MATH_NORMAL ))
      if( DBL_INT_IS_MANT_EQ_ZERO( X ))
      {
         // x = 0.0
         if( DBL_INT_EQ( N, DBL_INT_PLUS_INF ))
         {
            // n равен +Inf
            errno = EDOM;
            return( DBL_INT_NAN.dbl );
         }
         return( x );
      }
      // x - ненормализованное число
      DBL_INT_CLR_SIGN( X ); // очищаем знак - это самый старший бит
      n_sub = fmsbs64_1( X.u64 ); // ищем старший бит в старшем слове (нумерация с 1)
      n_sub = DBL_INT_MANT_LENGTH - n_sub + 1; // вычисляем величну сдвига
      n -= n_sub; // новое целочисленное значение степени с учетом поправки
      X.u64 <<= (uint64_t)n_sub; // сдвигаем мантиссу влево - умножаем на 2^n
      X.dbl = MATH_INLINE_COPYSIGN( X.dbl, x ); // восстанавливаем исходный знак
#else // BT_MATH_NOCHECK && BT_MATH_NORMAL
      return( 0.0 );
#endif // BT_MATH_NOCHECK && BT_MATH_NORMAL
   }
   if( n > 2 * DBL_INT_EXP_MAX )
   {
      // Заведомо происходит переполнение порядка - слишком большое число
      errno = ERANGE;
      if( DBL_INT_IS_NEG( X ))
      {
          // x < 0
          return( -HUGE_VAL );
      }
      return( +HUGE_VAL );
   }
   if( n < (-2) * DBL_INT_EXP_MAX - 53 )
   {
      // Потеря точности (число меньше чем минимальное неномализованное число)
      return( MATH_INLINE_LDEXP_FAST( X.dbl, INT_MIN ));
   }
   return( MATH_INLINE_LDEXP_FAST( X.dbl, (int)n ));
}

#endif //__MATH_INLINE_SCALB_H__
