// frexp() - функция вычисление мантиссы и порядка числа типа double
//
// Синтаксис:
//   #include <math.h>
//   double frexp( double x, int *exp );
//
// Возвращаемое значение.
//   Функция возвращает значение мантиссы аргумента в интервале [1/2,1)
//   и порядок аргумента по основанию 2
//
// Особые значения:
//   если аргумент равен -0.0, функция возвращает -0.0, порядок равен 0
//   если аргумент равен +0.0, функция возвращает +0.0, порядок равен 0
//   если аргумент равен  NaN, функция возвращает  NaN, порядок не определен
//   если аргумент равен -Inf, функция возвращает аргумент, порядок не определен
//   если аргумент равен +Inf, функция возвращает аргумент, порядок не определен
//
// Ошибки: [EDOM]
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FREXP_H__ ))
#define __MATH_INLINE_FREXP_H__

#ifdef BT_MATH_INLINE
#define frexp MATH_INLINE_FREXP

#include <strings.h> // описание функций fmsbs64_1() ...
static double __inline__ __UNUSED_F MATH_INLINE_FREXP( double x, int* pexp )
{
   DBL_INT_UNION X = { x };
   int n = 0;

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      // аргумент равен NAN, -Inf или +Inf
      return( x );
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // х - ненормализованное число или 0.0
#if( !defined( BT_MATH_NOCHECK ) && !defined( BT_MATH_NORMAL ))
      if( DBL_INT_IS_ZERO( X ))
      {
         // х = 0.0
         (*pexp) = 0;
         return( x );
      }
      // x - ненормализованное число
      DBL_INT_CLR_SIGN( X );           // очищаем отрицательный знак - это самый старший бит
      n = fmsbs64_1( X.u64 );          // ищем старший бит в старшем слове (нумерация с 1)
      n = DBL_INT_MANT_LENGTH - n + 1; // вычисляем величну сдвига
      X.u64 <<= (uint64_t)n;           // сначала домножаем мантиссу на 2^n
      (*pexp) = (int)( DBL_INT_EXP_MIN - n + 1 ); // делим число 2^n
#else // BT_MATH_NOCHECK && BT_MATH_NORMAL
      X.u64 &= DBL_INT_SIGN_MASK_64;   // возвращаем 0.0 со знаком x
      return( X.dbl );
#endif // BT_MATH_NOCHECK && BT_MATH_NORMAL
   }
   else
   {
      // Выделяем порядок аргумента по основанию 2
      (*pexp) = (int)DBL_INT_GET_EXP( X ) + 1;   // порядок мантиссы равен -1
   }
   // Устанавливаем порядок мантиссы аргумента:
   DBL_INT_SET_EXP( X, -1 ); // мантисса находится в [0.5, 1.0) => порядок равен -1
   return MATH_INLINE_COPYSIGN( X.dbl, x );
}

#endif // BT_MATH_INLINE
#endif // __MATH_INLINE_FREXP_H__
