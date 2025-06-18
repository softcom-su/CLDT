// isgreater() - функция для проверки условия x > y для чисел с плавающей запятой
//
// Синтаксис:
//  #include <math.h>
//	 int isgreater( float x, float y );
//  int isgreater( float x, double y );
//	 int isgreater( double x, float y );
//	 int isgreater( double x, double y );
//
// Возвращаемое значение:
//	 Функция возвращает значение условия x > y.
//
// Особые значения:
//	 Если аргумент х или у равен  NaN, функция возвращает 0.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISGREATER_H__ ))
#define __MATH_INLINE_ISGREATER_H__

#define isgreater( X, Y ) ( !isunordered( X, Y )  && (( X ) > ( Y )))

#endif // __MATH_INLINE_ISGREATER_H__
