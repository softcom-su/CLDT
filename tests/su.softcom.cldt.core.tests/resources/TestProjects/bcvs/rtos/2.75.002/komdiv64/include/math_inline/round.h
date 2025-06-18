// Описание:
//    round() - функция для округления аргумента до ближайшего целого числа
//       типа double, без учета установленного режима округления.
//
// Синтаксис:
//   #include <math.h>
//   double round( double x );
//
// Возвращаемое значение:
//   Функция возвращает ближайшее к аргументу целое число типа double,
//     без учета установленного режима округления.
//
// Особые значения:
//   Если аргумент равен -0.0, функция возвращает -0.0
//   Если аргумент равен +0.0, функция возвращает +0.0
//   Если аргумент равен  NaN, функция возвращает  NaN
//   Если аргумент равен -Inf, функция возвращает -Inf
//   Если аргумент равен +Inf, функция возвращает +Inf
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ROUND_H__ ))
#define __MATH_INLINE_ROUND_H__

#ifdef BT_MATH_INLINE 
#define round MATH_INLINE_ROUND 
#endif

#include <math_inline/fabs.h>

static double __inline__ __UNUSED_F MATH_INLINE_ROUND( double x )
{
   DBL_INT_UNION X = { x }; // двоичное представление аргумента x со знаком
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака
   int64_t trunc_mask;

   if( DBL_INT_GE( ABS_X, DBL_INT_TWO_52 ))
   {
      // Порядок числа больше или равен 52 => x - целое число или NaN, -Inf, +Inf
      return( x );
   }
   if( DBL_INT_LT( ABS_X, DBL_INT_HALF ))
   {
      // |x|<0.5
      X.u64 &= DBL_INT_SIGN_MASK_64;// оставляем только знаковый разряд
      return( X.dbl );// возвращаем -0.0 или +0.0
   }
   if( x > 0.0 )
   {
      X.dbl += 0.5;
   }
   else
   {
      X.dbl -= 0.5;
   }
   // Маска бит мантиссы, соответствующих дробной части числа
   trunc_mask = (int64_t)DBL_INT_MANT_MASK_64 >> (int64_t)DBL_INT_GET_EXP( X );
   X.i64 &= ~trunc_mask;// отсекаем лишние биты для округления до целого
   return( X.dbl );
}

#endif //__MATH_INLINE_ROUND_H__
