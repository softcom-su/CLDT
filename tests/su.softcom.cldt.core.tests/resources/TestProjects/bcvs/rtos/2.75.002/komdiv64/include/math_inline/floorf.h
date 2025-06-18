// Описание:
//   floorf() - функция вычисления наибольшего целого числа типа float,
//     меньшего или равного аргументу (округление в сторону -Inf).
//
// Синтаксис:
//   #include <math.h>
//   float floorf( float x );
//
// Возвращаемое значение:
//   Функция возвращает значение наибольшего целого числа типа float,
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

#if( !defined( __MATH_INLINE_FLOORF_H__ ))
#define __MATH_INLINE_FLOORF_H__

#ifdef BT_MATH_INLINE 
#define floorf MATH_INLINE_FLOORF
#endif

#include <math_inline/fabsf.h>
#include <math_inline/truncf_fast.h>

static float __inline__ __UNUSED_F MATH_INLINE_FLOORF( float x )
{
   FLT_INT_UNION X     = { x }; // двоичное представление аргумента x со знаком
   FLT_INT_UNION ABS_X = { MATH_INLINE_FABSF( x ) }; // двоичное представление аргумента x без знака
   FLT_INT_UNION Y;

   if( FLT_INT_LT( ABS_X, FLT_INT_ONE ))
   {
      // |x| < 1
      if( FLT_INT_IS_ZERO( X ))
      {
         // x равно +0.0 или -0.0
         return( x );
      }
      if( FLT_INT_IS_NEG( X ))
      {
         // x принадлежит интервалу (-1,0)
         return( -1.0 );
      }
      // x принадлежит интервалу (0,+1)
      return( +0.0 );
   }
   Y.flt = MATH_INLINE_TRUNCF_FAST( x ); // отбрасываем дробную часть
   if( FLT_INT_EQ( X, Y ))
   {
      // х - целое число, или NaN, или -Inf, или +Inf
      return( x );
   }
   if( Y.flt < 0.0 )
   {
      // Y - отрицательное число => округляем к -Inf
      Y.flt -= 1.0;
   }
   return( Y.flt );
}

#endif // __MATH_INLINE_FLOORF_H__
