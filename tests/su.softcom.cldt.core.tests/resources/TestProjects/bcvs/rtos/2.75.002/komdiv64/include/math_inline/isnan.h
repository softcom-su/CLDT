// isnan() - проверка, является ли аргумент не числом (NaN - Not a Number)
//
// Синтаксис:
//   #include <math.h>
//   int isnan( float x );
//   int isnan( double x );
//
// Возвращаемое значение:
//   Возвращается ненулевое значение, если аргумент x является не числом (NaN).
//   В других случаях возвращается 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISNAN_H__ ))
#define __MATH_INLINE_ISNAN_H__

static __inline__ int __UNUSED_F ISNAN_FLOAT_INLINE( float x )
{
	FLT_INT_UNION X = { x };
	if( FLT_INT_IS_NAN( X )) // двоичная проверка NaN, -Inf и +Inf
	{
		return( 1 );
	}
	return( 0 );
}

static __inline__ int __UNUSED_F ISNAN_DOUBLE_INLINE( double x )
{
	DBL_INT_UNION X = { x };
	if( DBL_INT_IS_NAN( X )) // двоичная проверка NaN, -Inf и +Inf
	{
		return( 1 );
	}
	return( 0 );
}

#define isnan( X ) (( sizeof( X ) == sizeof( float )) ? ISNAN_FLOAT_INLINE( X ) : ISNAN_DOUBLE_INLINE( X ))

#endif // __MATH_INLINE_ISNAN_H__

