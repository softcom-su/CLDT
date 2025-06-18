// Файл:
//    "ldexp_fast.h" - содержит вспомогательную inline функцию математической библиотеки
//       double MATH_INLINE_LDEXP_FAST( double x, int exp ).
//
// Описание:
///   MATH_INLINE_LDEXP_FAST() - функция быстрого вычисления числа типа double
//       по мантиссе и порядку.
//
// Синтаксис:
//    #include <math.h>
//    #include <math_inline/ldexp_fast.h>
//    double MATH_INLINE_LDEXP_FAST( double x, int exp );
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
//       внутри других функций (ldexp(), scalb(), atan2()), где должна проводиться
//       предварительная проверка корректности аргументов.
//    Предполагается, что x - конечное число не равное бесконечности.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_LDEXP_FAST_H__ ))
#define __MATH_INLINE_LDEXP_FAST_H__

static double __inline__ __UNUSED_F MATH_INLINE_LDEXP_FAST( double x, int exp_new )
{
   DBL_INT_UNION X = { x }; // целочисленное представление числа типа double
   DBL_INT_UNION X0 = { 0.0 };
   int n, sign;
   int exp_old = DBL_INT_GET_EXP_WITH_BIAS( X );// смещенная экспонента (exp+bias)

   if( exp_new > ( DBL_INT_EXP_MAX + DBL_INT_EXP_BIAS - exp_old ))
   {
      /* Переполнение порядка результата */
      errno = ERANGE;
      if( DBL_INT_IS_NEG( X ))
      {
         return( -HUGE_VAL );
      }
      return( +HUGE_VAL );
   }
   if( exp_new < ( DBL_INT_EXP_MIN + DBL_INT_EXP_BIAS - exp_old ))
   {
      /* Результат ненормализованное число   */
      sign = X.u32.h & DBL_INT_SIGN_MASK_32;
      DBL_INT_SET_EXP_WITH_BIAS( X, 1 );
      n = DBL_INT_EXP_MIN + DBL_INT_EXP_BIAS - exp_new - exp_old + 1;
      if( n < 0 || n > DBL_INT_EXP_MAX )
      {
         n = DBL_INT_EXP_MAX;
      }
      DBL_INT_SET_EXP_WITH_BIAS( X0, n );
      X0.u32.h |= sign;
      X.dbl += X0.dbl;
      if( X.u32.h == 0x00300000 || X.u32.h == 0x80300000 )
      {
         DBL_INT_SET_EXP_WITH_BIAS( X, 1 );
      }
      else
      {
         DBL_INT_SET_EXP_WITH_BIAS( X, 0 );
      }
      errno = ERANGE;                  // ненормализованное число
      return( X.dbl );
   }
   // Результат конечное нормализованное число
   DBL_INT_SET_EXP_WITH_BIAS( X, exp_old + exp_new ); // устанавливаем нужную степень
   return( X.dbl );
}

#endif // __MATH_INLINE_LDEXP_FAST_H__
