// copysign() - функция копирование знака из числа типа double в другое число типа double.
//
// Синтаксис:
//   #include <math.h>
//   double copysign( double x, double y );
//
// Возвращаемое значение:
//   Возвращаемое значение имеет абсолютное значение аргумента x и знак аргумента y.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_COPYSIGN_H__ ))
#define __MATH_INLINE_COPYSIGN_H__

#ifdef BT_MATH_INLINE 
#define copysign MATH_INLINE_COPYSIGN 
#endif

static double __inline__ __UNUSED_F MATH_INLINE_COPYSIGN( double x, double y )
{

   DBL_INT_UNION X = { x };
   DBL_INT_UNION Y = { y };

   if( DBL_INT_IS_NEG( Y ))
   {
      DBL_INT_SET_SIGN( X ); // устанавливаем знаковый бит
   }
   else
   {
      DBL_INT_CLR_SIGN( X ); // сбрасываем знаковый бит
   }
   return( X.dbl );
}

#endif // __MATH_INLINE_COPYSIGN_H__
