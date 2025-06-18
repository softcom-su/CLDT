// Файл "rintf.h" содержит inline функцию математической библиотеки float rintf( float x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    rintf() - функция для округления аргумента до ближайшего целого числа типа float,
//       с учетом установленного по умолчанию режима округления.
//
// Синтаксис:
//    #include <math.h>
//    float rintf( float x );
//
// Возвращаемое значение:
//    Функция возвращает ближайшее к аргументу целое число типа float,
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

#if( !defined( __MATH_INLINE_RINTF_H__ ))
#define __MATH_INLINE_RINTF_H__

#ifdef BT_MATH_INLINE
#define rintf MATH_INLINE_RINTF
#endif

#include <math_inline/fabs.h>

static float __inline__ __UNUSED_F MATH_INLINE_RINTF( float x )
{
   FLT_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака

   if( FLT_INT_GE( ABS_X, FLT_INT_TWO_23 )) // безопасное двоичное сравнение чисел (включая NaN)
   {
      // x - целое число (так как порядок больше 23 (длины мантисы)), либо NaN, либо Inf
      return( x );
   }
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( ABS_X ))
   {
      // х - ненормализованное число или 0
      if( x == 0.0 )
      {
         return( x );
      }
      // х - ненормализованное число,
      //     заменяем ненормализованное число на маленькое нормализованное число
      x = copysign( FLT_MIN, x );
   }
#if( CPU == I80386 )
   __asm __volatile ( "frndint" : "=t" (x) : "0" (x) );
   return( x );
#else
   {
      volatile float y1;
      float y = 0x1.0p23;             // y = 2^23
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

#endif //__MATH_INLINE_RINTF_H__
