// Файл "nearbyint.h" содержит inline функцию математической библиотеки double nearbyint( double x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    nearbyint() - функция для округления аргумента до ближайшего целого числа типа double,
//       с учетом установленного по умолчанию режима округления.
//
// Синтаксис:
//    #include <math.h>
//    double nearbyint( double x );
//
// Возвращаемое значение:
//    Функция возвращает ближайшее к аргументу целое число типа double,
//       с учетом установленного режима округления.
//
// Особые значения:
//    Если аргумент равен -0.0, функция возвращает -0.0
//    Если аргумент равен +0.0, функция возвращает +0.0
//    Если аргумент равен  NaN, функция возвращает  NaN
//    Если аргумент равен -Inf, функция возвращает -Inf
//    Если аргумент равен +Inf, функция возвращает +Inf
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_NEARBYINT_H__ ))
#define __MATH_INLINE_NEARBYINT_H__

#ifdef BT_MATH_INLINE
#define nearbyint MATH_INLINE_NEARBYINT
#endif

#include <math_inline/fabs.h>

static double __inline__ __UNUSED_F MATH_INLINE_NEARBYINT( double x )
{
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака

   if( DBL_INT_GE( ABS_X, DBL_INT_TWO_52 )) // безопасное двоичное сравнение чисел (включая NaN)
   {
      // x - целое число (так как порядок больше 52 (длины мантисы)), либо NaN, либо Inf
      return( x );
   }
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( ABS_X ))
   {
      // х - ненормализованное число или 0.0
      if( DBL_INT_IS_MANT_EQ_ZERO( ABS_X ))
      {
         return( x );
      }
      // х - ненормализованное число,
      //     заменяем ненормализованное число на маленькое нормализованное число
      x = copysign( DBL_MIN, x );
   }
#if( CPU == I80386 )
   __asm __volatile ( "frndint" : "=t" (x) : "0" (x) );
   return( x );
#else
   {
      volatile double y1;
      double y = 0x1.0p52;             // y = 2^52
      if( x < 0.0 )
      {
         y1 = x - y;                   // аппаратное округление до целого
         y1 += y;
      } else
      {
         y1 = x + y;                      // аппаратное округление
         y1 -= y;
      }
      return( copysign( y1, x ));
   }
#endif
}

#endif //__MATH_INLINE_NEARBYINT_H__
