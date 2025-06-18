// Файл:
//    "ldexp_fast_subnormal.c" - содержит вспомогательную inline функцию математической библиотеки
//       MATH_INLINE_LDEXP_FAST_SUBNORMAL( double x, int exp ).
//
// Описание:
//    MATH_INLINE_LDEXP_FAST_SUBNORMAL() - функция быстрого вычисления числа типа double
//       по мантиссе и порядку
//
// Синтаксис:
//    #include <math.h>
//    double MATH_INLINE_LDEXP_FAST_SUBNORMAL( double x, int exp );
//
// Возвращаемое значение:
//    Функция возвращает значение x * 2^exp
//
// Особые значения:
//    Если в результате было получено слишком большое по абсолютной величине
//       число, то будет возвращено плюс или минус HUGE_VALF в зависимости
//       от знака числа и переменной errno будет присвоено значение [ERANGE].
//    Если в результате преобразования было получено слишком маленькое
//       по абсолютной величине число, то будет возвращено ненормализованное
//       число и переменной errno будет присвоено значение [ERANGE].
//
// Ошибки:
//    [ERANGE].
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

#if( !defined( __MATH_INLINE_LDEXP_FAST_SUBNORMAL_H__ ))
#define __MATH_INLINE_LDEXP_FAST_SUBNORMAL_H__

static double __inline__ __UNUSED_F MATH_INLINE_LDEXP_FAST_SUBNORMAL( double x, int exp_new )
{
   // Декларация переменных для целочисленного представления числа типа double:
   DBL_INT_UNION X = { x };
   int n;
   int exp_old = DBL_INT_GET_EXP_WITH_BIAS( X );// смещенная экспонента ( exp+bias )
   if( exp_new > ( DBL_INT_EXP_MAX + DBL_INT_EXP_BIAS - exp_old ))
   {
      // Переполнение порядка результата
      errno = ERANGE;
      if( DBL_INT_IS_NEG( X ))
      {
         return( -HUGE_VAL );
      }
      return( +HUGE_VAL );
   }
   if( exp_new < ( DBL_INT_SUB_MIN_EXP + DBL_INT_EXP_BIAS - exp_old ))
   {
      // Потеря точности (число меньше чем минимальное неномализованное число)
      errno = ERANGE;
      X.u64 &= DBL_INT_SIGN_MASK_64;
      return( X.dbl );
   }
   if( exp_new < ( DBL_INT_EXP_MIN + DBL_INT_EXP_BIAS - exp_old ))
   {
      // Результат ненормализованное число
      n = DBL_INT_EXP_MIN + DBL_INT_EXP_BIAS - exp_new - exp_old;
      DBL_INT_CLR_SIGN( X ); // сбрасываем знаковый бит чтобы не мешал сдвигу
      DBL_INT_SET_EXP_WITH_BIAS( X, 1 ); // устанавливаем скрытый бит мантиссы для корректного сдвига
      X.u64 >>= (uint64_t)n; // сдвиг вправо - деление ненормализованного числа на 2^n
      errno = ERANGE; // непредставимое ненормализованное число (при свдиге теряются биты)
      // Представимое ненормализованное число
      if( x < 0.0 )
      {
         return( -X.dbl );
      }
      return( X.dbl );
   }
   // Результат конечное нормализованное число
   DBL_INT_SET_EXP_WITH_BIAS( X, exp_old + exp_new ); // устанавливаем нужную степень
   return( X.dbl );
}

#endif // __MATH_INLINE_LDEXP_FAST_SUBNORMAL_H__
