// signbit() - функция для прверки знака из числа типа double.
//
// Синтаксис.
//   #include <math.h>
//   int signbit( float x );
//   int signbit( double x );
//
// Возвращаемое значение.
//   Если аргумент x отрицателен, возвращается ненулевое значение.
//   В противном случае возвращается 0.
//
// Cтандарт.
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_SIGNBIT_H__ ))
#define __MATH_INLINE_SIGNBIT_H__

static __inline__ int __UNUSED_F SIGNBIT_FLOAT_INLINE( float x )
{
   // декларация переменных для двоичного представления чисел типа float:
   FLT_INT_UNION X = { x };
   return( FLT_INT_SIGN( X ) ? 1 : 0 );
}

static __inline__ int __UNUSED_F SIGNBIT_DOUBLE_INLINE( double x )
{
   // декларация переменных для двоичного представления чисел типа double:
   DBL_INT_UNION X = { x };
   return( DBL_INT_SIGN( X ) ? 1 : 0 );
}

#define signbit( X ) (( sizeof( X ) == sizeof( float )) ? SIGNBIT_FLOAT_INLINE( X ) : SIGNBIT_DOUBLE_INLINE( X ))

#endif //__MATH_INLINE_SIGNBIT_H__
