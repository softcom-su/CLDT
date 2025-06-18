// Описание:
// fabsf() - функция вычисление абсолютной величины числа типа float
//
// Синтаксис:
//   #include <math.h>
//   float fabsf( float x );
//
// Возвращаемое значение:
//   Функция возвращает абсолютное значение аргумента типа float
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
#define fabsf MATH_INLINE_FABSF
#endif // BT_MATH_INLINE

#if( !defined( __MATH_INLINE_FABSF_H__ ))
#define __MATH_INLINE_FABSF_H__

static float __inline__ __UNUSED_F MATH_INLINE_FABSF( float x )
{
   FLT_INT_UNION X = { x };

   FLT_INT_CLR_SIGN( X ); // всегда сбрасываем знаковый бит
   return( X.flt );
}

#endif // __MATH_INLINE_FABSF_H__

