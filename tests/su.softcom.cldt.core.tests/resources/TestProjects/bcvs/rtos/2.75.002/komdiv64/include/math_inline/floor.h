// Описание:
//   floor() - функция вычисления наибольшего целого числа типа double,
//     меньшего или равного аргументу (округление в сторону -Inf).
//
// Синтаксис:
//   #include <math.h>
//   double floor( double x );
//
// Возвращаемое значение:
//   Функция возвращает значение наибольшего целого числа типа double,
//     меньшего или равного аргументу.
//
// Особые значения:
//   Если аргумент равен -0.0, функция возвращает -0.0.
//   Если аргумент равен +0.0, функция возвращает +0.0.
//   Если аргумент равен  NaN, функция возвращает  NaN.
//   Если аргумент равен -Inf, функция возвращает -Inf.
//   Если аргумент равен +Inf, функция возвращает +Inf.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FLOOR_H__ ))
#define __MATH_INLINE_FLOOR_H__

#ifdef BT_MATH_INLINE 
#define floor MATH_INLINE_FLOOR 
#endif

#include <math_inline/fabs.h>
#include <math_inline/trunc_fast.h>

static double __inline__ __UNUSED_F MATH_INLINE_FLOOR( double x )
{
   DBL_INT_UNION X     = { x }; // двоичное представление аргумента x со знаком
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака
   DBL_INT_UNION Y;

   if( DBL_INT_LT( ABS_X, DBL_INT_ONE ))
   {
      // |x| < 1
      if( DBL_INT_IS_ZERO( X ))
      {
         // x равно +0.0 или -0.0
         return( x );
      }
      if( DBL_INT_IS_NEG( X ))
      {
         // x принадлежит интервалу (-1,0)
         return( -1.0 );
      }
      // x принадлежит интервалу (0,+1)
      return( +0.0 );
   }
   Y.dbl = MATH_INLINE_TRUNC_FAST( x ); // отбрасываем дробную часть
   if( DBL_INT_EQ( X, Y ))
   {
      // х - целое число, или NaN, или -Inf, или +Inf
      return( x );
   }
   if( Y.dbl < 0.0 )
   {
      // Y - отрицательное число => округляем к -Inf
      Y.dbl -= 1.0;
   }
   return( Y.dbl );
}

#endif // __MATH_INLINE_FLOOR_H__
