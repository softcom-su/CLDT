// Файл "ilogb.h" содержит inline функцию математической библиотеки int ilogb( double x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    ilogb() - функция для вычисление двоичной экспоненты числа типа double.
//
// Синтаксис:
//    #include <math.h>
//    int ilogb( double x );
//
// Возвращаемое значение:
//    Функция возвращает порядок числа типа double.
//
// Особые значения:
//    Если аргумент равен -0.0 или +0.0,
//       то функция возвращает FP_ILOGB0 и устанавливает код ошибки [EDOM].
//    Если аргумент равен  NaN,
//       то функция возвращает FP_ILOGBNAN и устанавливает код ошибки [EDOM].
//    Если аргумент равен -Inf или +Inf,
//       то функция возвращает INT_MAX и устанавливает код ошибки [EDOM].
//
// Ошибки:
//    Если аргумент равен -0.0, +0.0, NaN, -Inf или +Inf,
//       то переменной errno присваивается код ошибки [EDOM].
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_ILOGB_H__ ))
#define __MATH_INLINE_ILOGB_H__

#ifdef BT_MATH_INLINE
#define ilogb MATH_INLINE_ILOGB
#endif

#include <strings.h>                   // описание функций fmsbs64_1() ...

static int __inline__ __UNUSED_F MATH_INLINE_ILOGB( double x )
{
   DBL_INT_UNION X = { x };
   int n;

#if( !( defined( BT_MATH_NOCHECK ) || defined( BT_MATH_NORMAL )))
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      // х не число или равно -Inf или +Inf
      errno = EDOM; // аргумент не принадлежит области определения
      if(( DBL_INT_IS_INF( X )))
      {
         // х равно -Inf или +Inf
         return( INT_MAX );
      }
      // х не число (NaN)
      return( FP_ILOGBNAN );
   }

   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      if( DBL_INT_IS_MANT_EQ_ZERO( X ))
      {
         errno = EDOM;
         return( FP_ILOGB0 );
      }
      DBL_INT_CLR_SIGN( X );  // очищаем знак - это самый старший бит
      n = fmsbs64_1( X.u64 ); // ищем старший бит в старшем слове (нумерация с 1)
      n = DBL_INT_MANT_LENGTH - n + 1; // вычисляем величну сдвига
      return( DBL_INT_EXP_MIN - n );
   }
#else // BT_MATH_NOCHECK || BT_MATH_NORMAL
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      // x равно нулю или ненормализованному числу
      errno = EDOM;
      return( FP_ILOGB0 );
   }
#endif // BT_MATH_NOCHECK || BT_MATH_NORMAL
   return( DBL_INT_GET_EXP( X ));
}

#endif // __MATH_INLINE_ILOGB_H__
