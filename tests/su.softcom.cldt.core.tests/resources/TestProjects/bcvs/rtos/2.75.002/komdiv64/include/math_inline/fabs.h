// Описание:
//   fabs() - функция вычисление абсолютной величины числа типа double
//
// Синтаксис:
//   #include <math.h>
//   double fabs( double x );
//
// Возвращаемое значение:
//   Функция возвращает абсолютное значение аргумента типа double
//
// Особые значения:
//   Eсли аргумент равен -0.0, функция возвращает +0.0
//   Eсли аргумент равен +0.0, функция возвращает +0.0
//   Eсли аргумент равен  NaN, функция возвращает  NaN
//   Eсли аргумент равен -Inf, функция возвращает +Inf
//   Eсли аргумент равен +Inf, функция возвращает +Inf
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#ifdef BT_MATH_INLINE 
#define fabs MATH_INLINE_FABS 
#endif // BT_MATH_INLINE

#if( !defined( __MATH_INLINE_FABS_H__ ))
#define __MATH_INLINE_FABS_H__

static double __inline__ __UNUSED_F MATH_INLINE_FABS( double x )
{
   DBL_INT_UNION X = { x };

   DBL_INT_CLR_SIGN( X ); // всегда сбрасываем знаковый бит
   return( X.dbl );
}

#endif // __MATH_INLINE_FABS_H__

