// Файл "nextafterf.h" содержит inline функцию математической библиотеки
//    float nextafterf( float x, float y ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    nextafterf() - функция для вычисления следующего представимого
//       числа типа float в заданном направлении.
//
// Синтаксис:
//    #include <math.h>
//    float nextafterf( float x, float y );
//
// Возвращаемое значение:
//    Функция возвращает следуещее за x представимое число типа float
//       в направлении заданном числом y.
//	   Если число x равно числу у, то функция возвращает число у.
//
// Особые значения:
//    Если хотя бы один из аргументов равен NaN, то функция возвращает NaN.
//    Если в результате возникает переполнение (overflow),
//       то функция возвращает число  -HUGE_VALF или +HUGE_VALF
//       (в зависмости от знака аргумента x) и устанавливается код ошибки [ERANGE].
//    Если в результате получилось ненормализованное число,
//       то устанавливается код ошибки [ERANGE].
//
// Ошибки:
//    [ERANGE]
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_NEXTAFTERF_H__ ))
#define __MATH_INLINE_NEXTAFTERF_H__

#ifdef BT_MATH_INLINE
#define nextafterf MATH_INLINE_NEXTAFTERF
#endif

static float __inline__ __UNUSED_F MATH_INLINE_NEXTAFTERF( float x, float y )
{
   FLT_INT_UNION X = { x };

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( isunordered( x, y ))
   {
      // Хотя бы один из аргументов не число
      return( FLT_INT_NAN.flt ); // возвращаем NaN
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
         FLT_INT_CLR_SIGN( X ); // гасим знаковый бит для случая x = -0.0
         X.u32++;
      }
      else
      {
         // Абсолютную величину х нужно уменьшить
         X.u32--;
      }
   }
   else
   {
      //  ( x > y ) => число x нужно уменьшить
      if( x <= 0.0 )
      {
         // Абсолютную величину х нужно увеличить
         FLT_INT_SET_SIGN( X ); // выставляем знаковый бит для x = +0.0
         X.u32++;
      }
      else
      {
         // Абсолютную величину х нужно уменьшить
         X.u32--;
      }
   }
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // Ненормализованное число или 0.0
      errno = ERANGE;
   }
   if( FLT_INT_IS_NAN_OR_INF( X ))
   {
      // х - плюс или минус бесконечность
      errno = ERANGE;
   }
   return( X.flt );
}
#endif //__MATH_INLINE_NEXTAFTERF_H__
