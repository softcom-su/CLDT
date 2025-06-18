// isunordered() - функция для проверки являются аргументы упорядоченными или нет.
//
// Синтаксис:
//   #include <math.h>
//   int isunordered( float x, float y );
//   int isunordered( float x, double y );
//   int isunordered( double x, float y );
//   int isunordered( double x, double y );
//
// Возвращаемое значение:
//   Функция возвращает 1 если аргументы х или у не упорядоченны и 0 в противоположном случае
//   (слово упорядоченные подразумевает, что их можно сравнивать между собой - больше меньше или равно).
//
// Особые значения:
//   если аргумент х или у равен  NaN, функция возвращает 1.
//
// Cтандарт:
//   Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ISUNORDERED_H__ ))
#define __MATH_INLINE_ISUNORDERED_H__

#define isunordered( X, Y ) (( isnan( X ) || isnan( Y )))

#endif //__MATH_INLINE_ISUNORDERED_H__
