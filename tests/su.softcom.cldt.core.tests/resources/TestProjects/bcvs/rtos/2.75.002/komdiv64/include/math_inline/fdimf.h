// fdimf() - функция вычисления положительной разности двух чисел типа float.
//
// Синтаксис:
//   #include <math.h>
//   float fdim( float x, float y );
//
// Возвращаемое значение:
//   Если число x больше числа у, то функция возвращает
//   положительную разность двух чисел: (x-y).
//   Если число х меньше либо равно у, функция возвращает число +0.
//
// Особые значения:
//   Если хотя бы один из аргументов равен NaN, то функция возвращает NaN.
//
// Ошибки:
//   Если в результат (x-y) положительный и возникает переполнение (overflow),
//   то функция возвращает HUGE_VAL и переменной errno присваивает код ошибки [ERANGE].
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FDIMF_H__ ))
#define __MATH_INLINE_FDIMF_H__

#if( defined( BT_MATH_INLINE )) 
#if( defined( BT_MATH_NOCHECK ) || defined( BT_MATH_NORMAL ))

#define fdimf MATH_INLINE_FDIMF

#endif // BT_MATH_NORMAL || BT_MATH_NOCHECK
#endif // BT_MATH_INLINE
static float __inline__ __UNUSED_F MATH_INLINE_FDIMF( float x, float y )
{
   if( x <= y )
   {
        // Неположительная разность: ( x - y ) <= 0.0
        return( +0.0 );
   }
#if( !defined( BT_MATH_NOCHECK ))
   if(( y < 0 ) && ( x > y + FLT_MAX ))
   {
      // Переполнение: ( x - y ) > FLT_MAX
      errno = ERANGE;
      return( HUGE_VAL );
   }
#endif // BT_MATH_NOCHECK
   return( x - y );         // возвращаем положительную разность
}

#endif // __MATH_INLINE_FDIMF_H__

