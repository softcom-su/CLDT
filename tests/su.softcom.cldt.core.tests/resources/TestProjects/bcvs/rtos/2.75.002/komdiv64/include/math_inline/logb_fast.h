// Файл "logb_fast.h" содержит inline функцию математической библиотеки
//    double MATH_INLINE_LOGB_FAST( double x ),
//    в случае, если не используются inline версиия этой функции.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//     MATH_INLINE_LOGB_FAST() - функция для вычисление двоичной экспоненты числа типа double.
//
// Синтаксис.
//    #include <math.h>
//    double MATH_INLINE_LOGB_FAST( double x );
//
// Возвращаемое значение.
//   Функция возвращает значение двоичной экспоненты числа типа double.
//
// Особые значения.
//    Если аргумент x равен -0.0 или +0.0,
//    то функция возвращает -HUGE_VAL и устанавливает код ошибки [ERANGE]
//
// Ошибки.
//    [ERANGE]
//
// Cтандарт:
//    Функция не соответствует стандарту POSIX 1003.1, 2004 г.,
//
// Примечание.
//    Функция не делает никаких проверок аргументов,
//       но зато вычисляется с максимальной скоростью.
//    Проверка, что аргумент - конечное число в области определения
//       функции, должна быть сделана до вызова функции.

#if( !defined( __MATH_INLINE_LOGB_FAST_H__ ))
#define __MATH_INLINE_LOGB_FAST_H__

#include <strings.h> // описание функций fmsbs64_1() ...

static double __inline__ __UNUSED_F MATH_INLINE_LOGB_FAST( double x )
{
   DBL_INT_UNION X = { x };
   int n;

   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
#if( !defined( BT_MATH_NOCHECK )&&( !defined( BT_MATH_NORMAL )))
      if( DBL_INT_IS_ZERO( X ))
      {
         errno = ERANGE;
         return( -HUGE_VAL );
      }
      DBL_INT_CLR_SIGN( X ); // очищаем отрицательный знак - это самый старший бит
      n = fmsbs64_1( X.u64 );// ищем старший бит в старшем слове (нумерация с 1)
      n = DBL_INT_MANT_LENGTH - n + 1;
      return( (double)( DBL_INT_EXP_MIN - n ));
#else // BT_MATH_NORMAL && BT_MATH_NOCHECK
      errno = ERANGE;
      return( -HUGE_VAL );
#endif // BT_MATH_NORMAL && BT_MATH_NOCHECK
   }
   return( (double)( DBL_INT_GET_EXP( X )));
}

#endif //__MATH_INLINE_LOGB_FAST_H__
