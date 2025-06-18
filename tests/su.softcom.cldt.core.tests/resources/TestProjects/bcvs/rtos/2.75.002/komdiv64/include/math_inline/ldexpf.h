// Файл:
//    "ldexpf.h" - содержит inline функцию математической библиотеки
//       float ldexpf( float x, int exp ).
//
// Описание:
//    ldexpf() - функция вычисление числа типа float по мантиссе и порядку.
//
// Синтаксис:
//    #include <math.h>
//    float ldexpf( float x, int exp );
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
//       число, то будет возвращено плюс или минус HUGE_VALF в зависимости
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
//    Есть аналогичные функции базирующиеся на ldexpf() - scalbf(), scalbnf(), scalblnf()

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_LDEXPF_H__ ))
#define __MATH_INLINE_LDEXPF_H__

#include <math_inline/copysignf.h>
#include <math_inline/ldexpf_fast.h>
#include <strings.h> // описание функций fmsbs1() ...

#ifdef BT_MATH_INLINE
#define ldexpf MATH_INLINE_LDEXPF
#define scalbnf MATH_INLINE_LDEXPF
#define scalblnf MATH_INLINE_LDEXPF
#endif // BT_MATH_INLINE

static float __inline__ __UNUSED_F MATH_INLINE_LDEXPF( float x, int exp )
{
   int n = 0; // поправка к степени для ненормализованного числа
   FLT_INT_UNION X = { x }; // целочисленное представления числа типа float со знаком

#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( FLT_INT_IS_NAN_OR_INF( X ))
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
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // x - ненормализованное число или 0.0
#if( !defined( BT_MATH_NOCHECK ) && !defined( BT_MATH_NORMAL ))
      if( FLT_INT_IS_MANT_EQ_ZERO( X ))
      {
         // x = 0.0
         return( x );
      }
      // x - ненормализованное число
      FLT_INT_CLR_SIGN( X ); // очищаем знак - это самый старший бит
      n = fmsbs1( X.u32 ); // ищем старший бит в старшем слове (нумерация с 1)
      n = FLT_INT_MANT_LENGTH - n + 1; // вычисляем величну сдвига
      X.u32 <<= (uint64_t)n; // сдвигаем мантиссу влево - умножаем на 2^n
      X.flt = MATH_INLINE_COPYSIGNF( X.flt, x ); // восстанавливаем исходный знак
      exp = ( exp < FLT_INT_SUB_MIN_EXP ) ? exp : exp - n;
#else // BT_MATH_NOCHECK && BT_MATH_NORMAL
      return( 0.0 );
#endif // BT_MATH_NOCHECK && BT_MATH_NORMAL
   }
   return( MATH_INLINE_LDEXPF_FAST( X.flt, exp )); // если нужно, то делим на 2^n
}

#endif // __MATH_INLINE_LDEXPF_H__
