// Описание:
//   MATH_INLINE_TRUNCF_FAST() - вспомогательная функция для округления
//     аргумента до ближайшего целого числа типа float, по модулю не большего
//      модуля аргумента (округление в сторону 0).
//
// Синтаксис:
//   #include <math.h>
//   float MATH_INLINE_TRUNCF_FAST( float x );
//
// Возвращаемое значение:
//   Если число по модулю больше единицы функция возвращает ближайшее к
//     аргументу целое число, без учета установленного режима округления.
//
// Особые значения:
//   Если аргумент равен  NaN,   функция возвращает  NaN.
//   Если аргумент равен -Inf,   функция возвращает -Inf.
//   Если аргумент равен +Inf,   функция возвращает +Inf.
//   Функцию нельзя использовать для x, меньших 1.0 по модулю.

#if( !defined( __MATH_INLINE_TRUNCF_FAST_H__ ))
#define __MATH_INLINE_TRUNCF_FAST_H__

#include <math_inline/fabsf.h>

static float __inline__ __UNUSED_F MATH_INLINE_TRUNCF_FAST( float x )
{
   FLT_INT_UNION X = { x }; // двоичное представление аргумента x со знаком
   FLT_INT_UNION ABS_X = { MATH_INLINE_FABSF( x ) }; // двоичное представление аргумента x без знака
   int32_t trunc_mask;

   if( FLT_INT_GE( ABS_X, FLT_INT_TWO_23 ))
   {
      // Порядок числа больше или равен 23 => x - целое число или NaN, -Inf, +Inf
      return( x );
   }
   // Маска бит мантиссы, соответствующих дробной части числа
   trunc_mask = (int32_t)FLT_INT_MANT_MASK >> (int32_t)FLT_INT_GET_EXP( X );
   X.i32 &= ~trunc_mask; // отсекаем лишние биты для округления до целого
   return( X.flt );
}

#endif //__MATH_INLINE_TRUNCF_FAST_H__
