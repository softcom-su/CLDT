// Файл:
//    "ldexpf_fast.h" - содержит вспомогательную inline функцию математической библиотеки
//       float MATH_INLINE_LDEXPF_FAST( float x, int exp ).
//
// Описание:
///   MATH_INLINE_LDEXPF_FAST() - функция быстрого вычисления числа типа float
//       по мантиссе и порядку.
//
// Синтаксис:
//    #include <math.h>
//    #include <math_inline/ldexpf_fast.h>
//    float MATH_INLINE_LDEXPF_FAST( float x, int exp );
//
// Возвращаемое значение:
//    Функция возвращает значение x * 2^exp
//
// Особые значения:
//    Если в результате было получено слишком большое по абсолютной величине
//       число, то будет возвращено плюс или минус HUGE_VALF в зависимости
//       от знака числа и переменной errno будет присвоено значение [ERANGE].
//    Если в результате преобразования было получено слишком маленькое
//       по абсолютной величине число, которое не может быть представлено в
//       в виде нормализованного числа, то будет возвращено либо ненормализованное
//       число, либо 0.0 (если его нельзя представить в виде ненормализованного
//       числа) и переменной errno будет присвоено значение [ERANGE].
//
// Ошибки:
//    [ERANGE].
//
// Cтандарт:
//    Функция не соответствует стандарту POSIX 1003.1, 2004 г.
//
// Примечания:
//    Функция предназначена для использования
//       внутри других функций (ldexpf(), scalbf(), atan2f()), где должна проводиться
//       предварительная проверка корректности аргументов.
//    Предполагается, что x - конечное число не равное бесконечности.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_LDEXPF_FAST_H__ ))
#define __MATH_INLINE_LDEXPF_FAST_H__

static float __inline__ __UNUSED_F MATH_INLINE_LDEXPF_FAST( float x, int exp_new )
{
   FLT_INT_UNION X = { x }; // целочисленное представление числа типа float
   FLT_INT_UNION X0 = { 0.0 };
   int n, sign;
   int exp_old = FLT_INT_GET_EXP_WITH_BIAS( X );// смещенная экспонента (exp+bias)

   if( exp_new > ( FLT_INT_EXP_MAX + FLT_INT_EXP_BIAS - exp_old ))
   {
      // Переполнение порядка результата
      errno = ERANGE;
      if( FLT_INT_IS_NEG( X ))
      {
         return( -HUGE_VALF );
      }
      return( +HUGE_VALF );
   }
   if( exp_new < ( FLT_INT_EXP_MIN + FLT_INT_EXP_BIAS - exp_old ))
   {
      // Результат ненормализованное число
      sign = X.u32 & FLT_INT_SIGN_MASK;
      FLT_INT_SET_EXP_WITH_BIAS( X, 1 );
      n = FLT_INT_EXP_MIN + FLT_INT_EXP_BIAS - exp_new - exp_old + 1;
      if( n < 0 || n > FLT_INT_EXP_MAX )
      {
         n = FLT_INT_EXP_MAX;
      }
      FLT_INT_SET_EXP_WITH_BIAS( X0, n );
      X0.u32 |= sign;
      X.flt += X0.flt;
      if( X.u32 == 0x01800000 || X.u32 == 0x81800000 )
      {
         FLT_INT_SET_EXP_WITH_BIAS( X, 1 );
      }
      else
      {
         FLT_INT_SET_EXP_WITH_BIAS( X, 0 );
      }
      errno = ERANGE; // ненормализованное число
      return( X.flt );
   }
   // Результат конечное нормализованное число
   FLT_INT_SET_EXP_WITH_BIAS( X, exp_old + exp_new ); // устанавливаем нужную степень
   return( X.flt );
}

#endif // __MATH_INLINE_LDEXPF_FAST_H__
