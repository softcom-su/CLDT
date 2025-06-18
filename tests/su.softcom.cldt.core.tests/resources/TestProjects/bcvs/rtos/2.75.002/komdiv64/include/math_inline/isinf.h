// isinf() - функция для проверки, является ли аргумент бесконечностью (-Inf или +Inf).
//
// Синтаксис:
//   #include <math.h>
//   int isinf( float x );
//   int isinf( double x );
//
// Возвращаемое значение:
//   Возвращается ненулевое значение, если аргумент x равен -Inf или +Inf.
//   В других случаях возвращается 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1 2004 г.



#if( !defined( __MATH_INLINE_ISINF_H__ ))
#define __MATH_INLINE_ISINF_H__

static __inline__ int __UNUSED_F ISINF_FLOAT_INLINE( float x )
{
	FLT_INT_UNION X = { x };
	if( FLT_INT_IS_INF( X ))    // двоичная проверка NaN, -Inf и +Inf
	{
		return( 1 );
	}
	return( 0 );
}

static __inline__ int __UNUSED_F ISINF_DOUBLE_INLINE( double x )
{
	DBL_INT_UNION X = { x };
	if( DBL_INT_IS_INF( X ))    // двоичная проверка -Inf и +Inf
	{
		return( 1 );
	}
	return( 0 );
}

#define isinf( X ) (( sizeof( X ) == sizeof( float )) ? ISINF_FLOAT_INLINE( X ) : ISINF_DOUBLE_INLINE( X ))

#endif // __MATH_INLINE_ISINF_H__
