// Описание:
//    roundf() - функция для округления аргумента до ближайшего целого числа
//       типа float, без учета установленного режима округления.
//
// Синтаксис:
//   #include <math.h>
//   float roundf( float x );
//
// Возвращаемое значение:
//   Функция возвращает ближайшее к аргументу целое число типа float,
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

#if( !defined( __MATH_INLINE_ROUNDF_H__ ))
#define __MATH_INLINE_ROUNDF_H__

#ifdef BT_MATH_INLINE 
#define roundf MATH_INLINE_ROUNDF
#endif

#include <math_inline/fabsf.h>

static float __inline__ __UNUSED_F MATH_INLINE_ROUNDF( float x )
{
   FLT_INT_UNION X = { x }; // двоичное представление аргумента x со знаком
   FLT_INT_UNION ABS_X = { MATH_INLINE_FABSF( x ) }; // двоичное представление аргумента x без знака
   int32_t trunc_mask;

   if( FLT_INT_GE( ABS_X, FLT_INT_TWO_23 ))
   {
      // Порядок числа больше или равен 23 => x - целое число или NaN, -Inf, +Inf
      return( x );
   }
   if( FLT_INT_LT( ABS_X, FLT_INT_HALF ))
   {
      // |x|<0.5
      X.u32 &= FLT_INT_SIGN_MASK;// оставляем только знаковый разряд
      return( X.flt );// возвращаем -0.0 или +0.0
   }
   if( x > 0.0 )
   {
      X.flt += 0.5;
   }
   else
   {
      X.flt -= 0.5;
   }
   // Маска бит мантиссы, соответствующих дробной части числа
   trunc_mask = (int32_t)FLT_INT_MANT_MASK >> (int32_t)FLT_INT_GET_EXP( X );
   X.i32 &= ~trunc_mask;// отсекаем лишние биты для округления до целого
   return( X.flt );
}

#endif //__MATH_INLINE_ROUNDF_H__
