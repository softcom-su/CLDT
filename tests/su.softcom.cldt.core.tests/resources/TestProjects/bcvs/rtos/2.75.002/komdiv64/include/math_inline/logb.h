// Файл "logb.h" содержит inline функцию математической библиотеки double logb( double x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    logb() - функция для вычисление двоичной экспоненты числа типа double.
//
// Синтаксис:
//    #include <math.h>
//    double logb( double x );
//
// Возвращаемое значение:
//    Функция возвращает значение двоичной экспоненты числа типа double.
//
// Особые значения:
//    Если аргумент x равен -0.0 или +0.0,
//       то функция возвращает -HUGE_VAL и устанавливает код ошибки [ERANGE].
//    Если аргумент x равен  NaN, то функция возвращает NaN
//    Если аргумент равен -Inf или +Inf, то функция возвращает +Inf.
//
// Ошибки:
//    [ERANGE]
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_LOGB_H__ ))
#define __MATH_INLINE_LOGB_H__

#ifdef BT_MATH_INLINE 
#define logb MATH_INLINE_LOGB 
#endif

#include <math_inline/logb_fast.h>

static double __inline__ __UNUSED_F MATH_INLINE_LOGB( double x )
{
#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   DBL_INT_UNION X = { x };

   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
        // х - не число (NaN) или равно -Inf или +Inf
        if( DBL_INT_IS_MANT_EQ_ZERO( X ))
        {
            // х равно -Inf или +Inf
            DBL_INT_CLR_SIGN( X ); // очищаем знаковый разряд
            return( X.dbl );
        }
        return( x ); // х - не число (NaN)
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   return( MATH_INLINE_LOGB_FAST( x ));
}

#endif //__MATH_INLINE_LOGB_H__
