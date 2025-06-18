// isgreaterequal() - функция для проверки x >= y чисел с плавающей запятой.
//
// Синтаксис:
//   #include <math.h>
//   int isgreaterequal( float x, float y );
//   int isgreaterequal( float x, double y );
//   int isgreaterequal( double x, float y );
//   int isgreaterequal( double x, double y );
//
// Возвращаемое значение:
//	 Функция возвращает значение x >= y.
//
// Особые значения:
//	 Если аргумент х или у равен  NaN, функция возвращает 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISGREATEREQUAL_H__ ))
#define __MATH_INLINE_ISGREATEREQUAL_H__

#define isgreaterequal( X, Y ) ( !isunordered( X, Y )  && ( X >= Y ))

#endif // __MATH_INLINE_ISGREATEREQUAL_H__
