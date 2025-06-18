// islessgreater() - функция для проверки x != y чисел с плавающей запятой.
//
// Синтаксис:
//   #include <math.h>
//   int islessgreater( float x, float y );
//   int islessgreater( float x, double y );
//   int islessgreater( double x, float y );
//   int islessgreater( double x, double y );
//
// Возвращаемое значение:
//   Функция возвращает значение x != y
//
// Особые значения:
//	 Если аргумент х или у равен  NaN, функция возвращает 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISLESSGREATER_H__ ))
#define __MATH_INLINE_ISLESSGREATER_H__

#define islessgreater( X, Y ) ( !isunordered( X, Y ) && (( X < Y ) || ( X > Y )))

#endif // __MATH_INLINE_ISLESSGREATER_H__
