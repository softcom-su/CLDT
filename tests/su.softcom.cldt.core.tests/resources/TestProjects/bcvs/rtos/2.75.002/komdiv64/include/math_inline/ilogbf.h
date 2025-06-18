// Файл "ilogbf.h" содержит inline функцию математической библиотеки int ilogbf( float x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    ilogbf() - функция для вычисление двоичной экспоненты числа типа float.
//
// Синтаксис:
//    #include <math.h>
//    int ilogbf( float x );
//
// Возвращаемое значение:
//    Функция возвращает порядок числа типа float.
//
// Особые значения:
//    Если аргумент равен -0.0 или +0.0,
//       то функция возвращает FP_ILOGBF0 и устанавливает код ошибки [EDOM].
//    Если аргумент равен  NaN,
//       то функция возвращает FP_ILOGBFNAN и устанавливает код ошибки [EDOM].
//    Если аргумент равен -Inf или +Inf,
//       то функция возвращает INT_MAX и устанавливает код ошибки [EDOM].
//
// Ошибки:
//    Если аргумент равен -0.0, +0.0, NaN, -Inf или +Inf,
//       то переменной errno присваивается код ошибки [EDOM].
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ILOGBF_H__ ))
#define __MATH_INLINE_ILOGBF_H__

#ifdef BT_MATH_INLINE
#define ilogbf MATH_INLINE_ILOGBF
#endif

#include <strings.h> // описание функций fmsbs1() ...

static int __inline__ __UNUSED_F MATH_INLINE_ILOGBF( float x )
{
   FLT_INT_UNION X = { x };
   int n;

#if( !( defined( BT_MATH_NOCHECK ) || defined( BT_MATH_NORMAL )))
   if( FLT_INT_IS_NAN_OR_INF( X ))
   {
      // х не число или равно -Inf или +Inf
      errno = EDOM; // аргумент не принадлежит области определения
      if(( FLT_INT_IS_INF( X )))
      {
         // х равно -Inf или +Inf
         return( INT_MAX );
      }
      // х не число (NaN)
      return( FP_ILOGBNAN );
   }

   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      if( FLT_INT_IS_MANT_EQ_ZERO( X ))
      {
         errno = EDOM;
         return( FP_ILOGB0 );
      }
      FLT_INT_CLR_SIGN( X ); // очищаем знак - это самый старший бит
      n = fmsbs1( X.u32 );   // ищем старший бит в старшем слове (нумерация с 1)
      n = FLT_INT_MANT_LENGTH - n + 1; // вычисляем величну сдвига
      return( FLT_INT_EXP_MIN - n );
   }
#else // BT_MATH_NOCHECK || BT_MATH_NORMAL
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // x равно нулю или ненормализованному числу
      errno = EDOM;
      return( FP_ILOGBF0 );
   }
#endif // BT_MATH_NOCHECK || BT_MATH_NORMAL
   return( FLT_INT_GET_EXP( X ));
}

#endif // __MATH_INLINE_ILOGBF_H__
