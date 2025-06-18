// Файл "nextafter.h" содержит inline функцию математической библиотеки
//    double nextafter( double x, double y ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    nextafter() - функция для вычисления следующего представимого
//       числа типа double в заданном направлении.
//
// Синтаксис:
//    #include <math.h>
//    double nextafter( double x, double y );
//
// Возвращаемое значение:
//    Функция возвращает следуещее за x представимое число типа double
//       в направлении заданном числом y.
//	   Если число x равно числу у, то функция возвращает число у.
//
// Особые значения:
//    Если хотя бы один из аргументов равен NaN, то функция возвращает NaN.
//    Если в результате возникает переполнение (overflow),
//       то функция возвращает число  -HUGE_VAL или +HUGE_VAL
//       (в зависмости от знака аргумента x) и устанавливается код ошибки [ERANGE].
//    Если в результате получилось ненормализованное число,
//       то устанавливается код ошибки [ERANGE].
//
// Ошибки:
//    [ERANGE]
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_NEXTAFTER_H__ ))
#define __MATH_INLINE_NEXTAFTER_H__

#ifdef BT_MATH_INLINE
#define nextafter MATH_INLINE_NEXTAFTER
#endif

static double __inline__ __UNUSED_F MATH_INLINE_NEXTAFTER( double x, double y )
{
   DBL_INT_UNION X = { x };

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( isunordered( x, y ))
   {
      // Хотя бы один из аргументов не число
      return( DBL_INT_NAN.dbl ); // возвращаем NaN
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   // Теперь аргументы можно сравнивать
   if( x == y )
   {
      return( y );
   }
   if( x < y )
   {
      // Число x нужно увеличивать
      if( x >= 0.0 )
      {
         // Абсолютную величину х нужно увеличить
         DBL_INT_CLR_SIGN( X ); // гасим знаковый бит для случая x = -0.0
         X.u64++;
      }
      else
      {
         // Абсолютную величину х нужно уменьшить
         X.u64--;
      }
   }
   else
   {
      //  ( x > y ) => число x нужно уменьшить
      if( x <= 0.0 )
      {
         // Абсолютную величину х нужно увеличить
         DBL_INT_SET_SIGN( X ); // выставляем знаковый бит для x = +0.0
         X.u64++;
      }
      else
      {
         // Абсолютную величину х нужно уменьшить
         X.u64--;
      }
   }
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // Ненормализованное число или 0.0
      errno = ERANGE;
   }
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      // х - плюс или минус бесконечность
      errno = ERANGE;
   }
   return( X.dbl );
}
#endif //__MATH_INLINE_NEXTAFTER_H__
