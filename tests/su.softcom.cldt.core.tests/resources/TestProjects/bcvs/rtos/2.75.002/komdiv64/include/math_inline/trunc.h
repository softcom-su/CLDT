// Описание:
//   trunc() - функция для округления аргумента до ближайшего целого числа типа double,
//     по модулю не большего модуля аргумента (округление в сторону 0).
//
// Синтаксис:
//   #include <math.h>
//   double trunc( double x );
//
// Возвращаемое значение:
//   Функция возвращает ближайшее целое число типа double,
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

#if( !defined( __MATH_INLINE_TRUNC_H__ ))
#define __MATH_INLINE_TRUNC_H__

#ifdef BT_MATH_INLINE
#define trunc MATH_INLINE_TRUNC 
#endif

#include <math_inline/fabs.h>
#include <math_inline/trunc_fast.h>

static double __inline__ __UNUSED_F MATH_INLINE_TRUNC( double x )
{
   DBL_INT_UNION X     = { x }; // двоичное представление аргумента x со знаком
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака

   if( DBL_INT_LT( ABS_X, DBL_INT_ONE ))
   {
      // |x| < 1
      X.u64 &= DBL_INT_SIGN_MASK_64; // оставляем только знаковый разряд
      return( X.dbl );               // возвращаем -0.0 или +0.0
   }
   return( MATH_INLINE_TRUNC_FAST( x ));
}

#endif //__MATH_INLINE_TRUNC_H__
