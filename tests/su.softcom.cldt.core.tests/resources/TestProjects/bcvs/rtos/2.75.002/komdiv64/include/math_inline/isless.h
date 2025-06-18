// isless() - функция для проверки условия x < y для чисел с плавающей запятой.
//
// Синтаксис:
//   #include <math.h>
//   int isless( float x, float y );
//   int isless( float x, double y );
//   int isless( double x, float y );
//   int isless( double x, double y );
//
// Возвращаемое значение:
//   Функция возвращает значение x < y.
//
// Особые значения:
//   если аргумент х или у равен  NaN, функция возвращает 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISLESS_H__ ))
#define __MATH_INLINE_ISLESS_H__

#define	isless( X, Y ) ( !isunordered( X, Y ) && ( X < Y ))

#endif // __MATH_INLINE_ISLESS_H__
