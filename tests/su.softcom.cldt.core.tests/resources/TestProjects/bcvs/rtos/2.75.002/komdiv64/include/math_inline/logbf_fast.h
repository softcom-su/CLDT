// Файл "logbf_fast.h" содержит inline функцию математической библиотеки
//    float MATH_INLINE_LOGBF_FAST( float x ),
//    в случае, если не используются inline версиия этой функции.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

// Описание:
//    MATH_INLINE_LOGBF_FAST() - функция для вычисление двоичной экспоненты числа типа float.
//
// Синтаксис:
//    #include <math.h>
//    float MATH_INLINE_LOGBF_FAST( float x );
//
// Возвращаемое значение:
//    Функция возвращает значение двоичной экспоненты числа типа float.
//
// Особые значения:
//    Если аргумент x равен -0.0 или +0.0,
//       то функция возвращает -HUGE_VALF и устанавливает код ошибки [ERANGE]
//
// Ошибки:
//    [ERANGE]
//
// Cтандарт:
//    Функция не соответствует стандарту POSIX 1003.1, 2004 г.,
//
// Примечания:
//    Функция не делает никаких проверок аргументов,
//       но зато вычисляется с максимальной скоростью.
//    Проверка, что аргумент - конечное число в области определения
//       функции, должна быть сделана до вызова функции.

#if( !defined( __MATH_INLINE_LOGBF_FAST_H__ ))
#define __MATH_INLINE_LOGBF_FAST_H__

#include <strings.h> // описание функций fmsbs1() ...

static float __inline__ __UNUSED_F MATH_INLINE_LOGBF_FAST( float x )
{
   FLT_INT_UNION X = { x };
   int n;

   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
#if( !defined( BT_MATH_NOCHECK )&&( !defined( BT_MATH_NORMAL )))
      if( FLT_INT_IS_ZERO( X ))
      {
         errno = ERANGE;
         return( -HUGE_VALF );
      }
      FLT_INT_CLR_SIGN( X ); // очищаем отрицательный знак - это самый старший бит
      n = fmsbs1( X.u32 );// ищем старший бит в старшем слове (нумерация с 1)
      n = FLT_INT_MANT_LENGTH - n + 1;
      return( (float)( FLT_INT_EXP_MIN - n ));
#else // BT_MATH_NORMAL && BT_MATH_NOCHECK
      errno = ERANGE;
      return( -HUGE_VALF );
#endif // BT_MATH_NORMAL && BT_MATH_NOCHECK
   }
   return( (float)( FLT_INT_GET_EXP( X )));
}

#endif //__MATH_INLINE_LOGBF_FAST_H__
