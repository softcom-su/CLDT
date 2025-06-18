// copysignf() - функция копирование знака из числа типа float в другое число типа float.
//
// Синтаксис:
//   #include <math.h>
//   float copysign( float x, float y );
//
// Возвращаемое значение:
//   Возвращаемое значение имеет абсолютное значение аргумента x и знак аргумента y.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_COPYSIGNF_H__ ))
#define __MATH_INLINE_COPYSIGNF_H__

#ifdef BT_MATH_INLINE 
#define copysignf MATH_INLINE_COPYSIGNF
#endif

static float __inline__ __UNUSED_F MATH_INLINE_COPYSIGNF( float x, float y )
{

   FLT_INT_UNION X = { x };
   FLT_INT_UNION Y = { y };

   if( FLT_INT_IS_NEG( Y ))
   {
      FLT_INT_SET_SIGN( X ); // устанавливаем знаковый бит
   }
   else
   {
      FLT_INT_CLR_SIGN( X ); // сбрасываем знаковый бит
   }
   return( X.flt );
}

#endif // __MATH_INLINE_COPYSIGNF_H__
