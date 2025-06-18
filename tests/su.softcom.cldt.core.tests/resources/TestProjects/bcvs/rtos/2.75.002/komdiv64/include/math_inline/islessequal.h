// islessequal() - функция для проверки x <= y чисел с плавающей запятой.
//
// Синтаксис:
//   #include <math.h>
//   int islessequal( float x, float y );
//   int islessequal( float x, double y );
//   int islessequal( double x, float y );
//   int islessequal( double x, double y );
//
// Возвращаемое значение:
//   Функция возвращает значение x <= y
//
// Особые значения:
//   Если аргумент х или у равен  NaN, функция возвращает 0.
//
// Cтандарт
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISLESSEQUAL_H__ ))
#define __MATH_INLINE_ISLESSEQUAL_H__

#define islessequal( X, Y ) ( !isunordered( X, Y ) && (( X ) <= ( Y )))

#endif // __MATH_INLINE_ISLESSEQUAL_H__
