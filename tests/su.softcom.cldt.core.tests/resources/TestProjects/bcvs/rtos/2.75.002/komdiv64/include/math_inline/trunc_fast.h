// Описание:
//    MATH_INLINE_TRUNC_FAST() - вспомогательная функция для округления
//      аргумента до ближайшего целого числа типа double, по модулю не большего
//      модуля аргумента (округление в сторону 0).
//
// Синтаксис:
//   #include <math.h>
//   double MATH_INLINE_TRUNC_FAST( double x );
//
// Возвращаемое значение:
//   Если число по модулю больше единицы функция возвращает ближайшее к
//     аргументу целое число, без учета установленного режима округления.
//
// Особые значения.
//   Если аргумент равен  NaN,   функция возвращает  NaN.
//   Если аргумент равен -Inf,   функция возвращает -Inf.
//   Если аргумент равен +Inf,   функция возвращает +Inf.
//   Функцию нельзя использовать для x, меньших 1.0 по модулю.

#if( !defined( __MATH_INLINE_TRUNC_FAST_H__ ))
#define __MATH_INLINE_TRUNC_FAST_H__

#include <math_inline/fabs.h>

static double __inline__ __UNUSED_F MATH_INLINE_TRUNC_FAST( double x )
{
   DBL_INT_UNION X = { x }; // двоичное представление аргумента x со знаком
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление аргумента x без знака
   int64_t trunc_mask;

   if( DBL_INT_GE( ABS_X, DBL_INT_TWO_52 ))
   {
      // Порядок числа больше или равен 52 => x - целое число или NaN, -Inf, +Inf
      return( x );
   }
   // Маска бит мантиссы, соответствующих дробной части числа
   trunc_mask = (int64_t)DBL_INT_MANT_MASK_64 >> (int64_t)DBL_INT_GET_EXP( X );
   X.i64 &= ~trunc_mask; // отсекаем лишние биты для округления до целого
   return( X.dbl );
}

#endif //__MATH_INLINE_TRUNC_FAST_H__
