// isnormal() - проверка, является ли аргумент конечным нормализованным числом не равным 0.
//
// Синтаксис:
//   #include <math.h>
//   int isnormal( float x );
//   int isnormal( double x );
//
// Возвращаемое значение:
//   Если аргумент является конечным нормализованным числом не равным 0.0,
//   то возвращается ненулевое значение. В противном случае возвращается 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISNORMAL_H__ ))
#define __MATH_INLINE_ISNORMAL_H__

static __inline__ int __UNUSED_F ISNORMAL_FLOAT_INLINE( float x )
{
	FLT_INT_UNION X = { x };
	if( FLT_INT_IS_NAN_OR_INF( X )) // двоичная проверка NaN, -Inf и +Inf
	{
		return( 0 );
	}
	if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X )) // двоичная проверка на ноль или ненормализованное число
   {
      return( 0 );
   }
   return( 1 );
}

static __inline__ int __UNUSED_F ISNORMAL_DOUBLE_INLINE( double x )
{
	DBL_INT_UNION X = { x };
	if( DBL_INT_IS_NAN_OR_INF( X )) // двоичная проверка NaN, -Inf и +Inf
	{
		return( 0 );
	}
	if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X )) // двоичная проверка на ноль или ненормализованное число
   {
      return( 0 );
   }
   return( 1 );
}

#define isnormal( X ) (( sizeof( X ) == sizeof( float )) ? ISNORMAL_FLOAT_INLINE( X ) : ISNORMAL_DOUBLE_INLINE( X ))

#endif // __MATH_INLINE_ISNORMAL_H__

