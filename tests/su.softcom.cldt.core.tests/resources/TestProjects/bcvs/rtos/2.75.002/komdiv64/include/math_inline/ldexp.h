// Файл:
//    "ldexp.h" - содержит inline функцию математической библиотеки.
//       double ldexp( double x, int exp ).
//
// Описание:
//    ldexp() - функция вычисление числа типа double по мантиссе и порядку.
//
// Синтаксис:
//    #include <math.h>
//    double ldexp( double x, int exp );
//
// Возвращаемое значение:
//    Функция возвращает значение x * 2^exp
//
// Особые значения:
//    Если аргумент x равен  NaN, то функция возвращает NaN.
//    Если аргумент exp равен 0, то функция возвращает аргумент x.
//    Если аргумент x равен -0.0 или +0.0, то функция возвращает аргумент x.
//    Если аргумент x равен -Inf или +Inf, то функция возвращает аргумент x.
//    Если в результате было получено слишком большое по абсолютной величине
//       число, то будет возвращено плюс или минус HUGE_VAL в зависимости
//       от знака числа и переменной errno будет присвоено значение [ERANGE].
//    Если в результате преобразования было получено слишком маленькое
//       по абсолютной величине число, которое не может быть представлено в
//       в виде нормализованного числа, то будет возвращено либо ненормализованное
//       число либо 0.0 (если его нельзя представить в виде ненормализованного
//       числа) и переменной errno будет присвоено значение [ERANGE].
//
// Ошибки:
//    [ERANGE].
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.
//
// Примечания:
//    Есть аналогичные функции базирующиеся на ldexp() - scalb(), scalbn(), scalbln()

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_LDEXP_H__ ))
#define __MATH_INLINE_LDEXP_H__

#include <math_inline/copysign.h>
#include <math_inline/ldexp_fast.h>
#include <strings.h> // описание функций fmsbs64_1() ...

#ifdef BT_MATH_INLINE
#define ldexp MATH_INLINE_LDEXP
#define scalbn MATH_INLINE_LDEXP
#define scalbln MATH_INLINE_LDEXP
#endif // BT_MATH_INLINE
static double __inline__ __UNUSED_F MATH_INLINE_LDEXP( double x, int exp )
{
   int n = 0; // поправка к степени для ненормализованного числа
   DBL_INT_UNION X = { x }; // целочисленное представления числа типа double со знаком

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      // x - не число или бесконечность
      return( x );
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   if( exp == 0 )
   {
      return( x );
   }
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // x - ненормализованное число или 0.0
#if( !defined( BT_MATH_NOCHECK ) && !defined( BT_MATH_NORMAL ))
      if( DBL_INT_IS_MANT_EQ_ZERO( X ))
      {
         // x = 0.0
         return( x );
      }
      // x - ненормализованное число
      DBL_INT_CLR_SIGN( X ); // очищаем знак - это самый старший бит
      n = fmsbs64_1( X.u64 ); // ищем старший бит в старшем слове (нумерация с 1)
      n = DBL_INT_MANT_LENGTH - n + 1; // вычисляем величну сдвига
      X.u64 <<= (uint64_t)n; // сдвигаем мантиссу влево - умножаем на 2^n
      X.dbl = MATH_INLINE_COPYSIGN( X.dbl, x ); // восстанавливаем исходный знак
      exp = ( exp < DBL_INT_SUB_MIN_EXP ) ? exp : exp - n;
#else // BT_MATH_NOCHECK && BT_MATH_NORMAL
      return( 0.0 );
#endif // BT_MATH_NOCHECK && BT_MATH_NORMAL
   }
   return( MATH_INLINE_LDEXP_FAST( X.dbl, exp )); // если нужно, то делим на 2^n
}

#endif // __MATH_INLINE_LDEXP_H__
