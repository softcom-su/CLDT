// isfinite() - функция для проверки, является ли аргумент конечным числом
//
// Синтаксис:
//   #include <math.h>
//	  int isfinite( float x );
//	  int isfinite( double x );
//
// Возвращаемое значение:
//	  Если аргумент x не число (NaN), -Inf или +Inf, то возвращается 0.
//	  B противном случае возвращается ненулевое значение.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISFINITE_H__ ))
#define __MATH_INLINE_ISFINITE_H__

static __inline__ int __UNUSED_F ISFINITE_FLOAT_INLINE( float x )
{
	FLT_INT_UNION X = { x };
	if( FLT_INT_IS_NAN_OR_INF( X )) // двоичная проверка NaN, -Inf и +Inf
   {
      return( 0 );
   }
   return( 1 );
}

static __inline__ int __UNUSED_F ISFINITE_DOUBLE_INLINE( double x )
{
	DBL_INT_UNION X = { x };
	if( DBL_INT_IS_NAN_OR_INF( X )) // двоичная проверка NaN, -Inf и +Inf
   {
      return( 0 );
   }
   return( 1 );
}

#define isfinite( X ) (( sizeof( X ) == sizeof( float )) ? ISFINITE_FLOAT_INLINE( X ) : ISFINITE_DOUBLE_INLINE( X ))

// определение для совместимости со старыми версиями:
#define finite( X ) isfinite( X )

#endif // __MATH_INLINE_ISFINITE_H__
