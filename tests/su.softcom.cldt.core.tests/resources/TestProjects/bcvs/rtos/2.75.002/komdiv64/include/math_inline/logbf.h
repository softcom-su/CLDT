// Файл "logbf.h" содержит inline функцию математической библиотеки float logbf( float x ).

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    logbf() - функция для вычисление двоичной экспоненты числа типа float.
//
// Синтаксис:
//    #include <math.h>
//    float logbf( float x );
//
// Возвращаемое значение:
//    Функция возвращает значение двоичной экспоненты числа типа float.
//
// Особые значения:
//    Если аргумент x равен -0.0 или +0.0,
//       то функция возвращает -HUGE_VALF и устанавливает код ошибки [ERANGE].
//    Если аргумент x равен  NaN, то функция возвращает NaN
//    Если аргумент равен -Inf или +Inf, то функция возвращает +Inf.
//
// Ошибки:
//    [ERANGE]
//
// Cтандарт:
//    Функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_LOGBF_H__ ))
#define __MATH_INLINE_LOGBF_H__

#ifdef BT_MATH_INLINE 
#define logbf MATH_INLINE_LOGBF
#endif

#include <math_inline/logbf_fast.h>

static float __inline__ __UNUSED_F MATH_INLINE_LOGBF( float x )
{
#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   FLT_INT_UNION X = { x };

   if( FLT_INT_IS_NAN_OR_INF( X ))
   {
        // х - не число (NaN) или равно -Inf или +Inf
        if( FLT_INT_IS_MANT_EQ_ZERO( X ))
        {
            // х равно -Inf или +Inf
            FLT_INT_CLR_SIGN( X ); // очищаем знаковый разряд
            return( X.flt );
        }
        return( x ); // х - не число (NaN)
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   return( MATH_INLINE_LOGBF_FAST( x ));
}

#endif //__MATH_INLINE_LOGBF_H__
