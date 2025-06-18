// Описание:
//   truncf() - функция для округления аргумента до ближайшего целого числа типа float,
//     по модулю не большего модуля аргумента (округление в сторону 0).
//
// Синтаксис:
//   #include <math.h>
//   float truncf( float x );
//
// Возвращаемое значение:
//   Функция возвращает ближайшее целое число типа float,
//   модуль которого не больше модуля аргумента.
//
// Особые значения:
//   Eсли аргумент равен -0.0, функция возвращает -0.0.
//   Eсли аргумент равен +0.0, функция возвращает +0.0.
//   Eсли аргумент равен  NaN, функция возвращает  NaN.
//   Eсли аргумент равен -Inf, функция возвращает -Inf.
//   Eсли аргумент равен +Inf, функция возвращает +Inf.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_TRUNCF_H__ ))
#define __MATH_INLINE_TRUNCF_H__

#ifdef BT_MATH_INLINE
#define truncf MATH_INLINE_TRUNCF
#endif

#include <math_inline/fabsf.h>
#include <math_inline/truncf_fast.h>

static float __inline__ __UNUSED_F MATH_INLINE_TRUNCF( float x )
{
   FLT_INT_UNION X     = { x }; // двоичное представление аргумента x со знаком
   FLT_INT_UNION ABS_X = { MATH_INLINE_FABSF( x ) }; // двоичное представление аргумента x без знака

   if( FLT_INT_LT( ABS_X, FLT_INT_ONE ))
   {
      // |x| < 1
      X.u32 &= FLT_INT_SIGN_MASK; // оставляем только знаковый разряд
      return( X.flt );               // возвращаем -0.0 или +0.0
   }
   return( MATH_INLINE_TRUNCF_FAST( x ));
}

#endif //__MATH_INLINE_TRUNCF_H__
