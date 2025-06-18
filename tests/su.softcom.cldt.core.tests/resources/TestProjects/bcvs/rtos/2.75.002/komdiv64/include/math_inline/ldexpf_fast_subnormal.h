// Файл:
//    "ldexpf_fast_subnormal.c" - содержит вспомогательную inline функцию математической библиотеки
//       MATH_INLINE_LDEXPF_FAST_SUBNORMAL( float x, int exp ).
//
// Описание:
//    MATH_INLINE_LDEXPF_FAST_SUBNORMAL() - функция быстрого вычисления числа типа float
//       по мантиссе и порядку
//
// Синтаксис:
//    #include <math.h>
//    float MATH_INLINE_LDEXPF_FAST_SUBNORMAL( float x, int exp );
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
//       внутри других функций (ldexpf(), scalbf(), atan2f()), где должна проводиться
//       предварительная проверка корректности аргументов.
//    Предполагается, что x - конечное число не равное бесконечности.

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */

#if( !defined( __MATH_INLINE_LDEXPF_FAST_SUBNORMAL_H__ ))
#define __MATH_INLINE_LDEXPF_FAST_SUBNORMAL_H__

static float __inline__ __UNUSED_F MATH_INLINE_LDEXPF_FAST_SUBNORMAL( float x, int exp_new )
{
   // Декларация переменных для целочисленного представления числа типа float:
   FLT_INT_UNION X = { x };
   int n;
   int exp_old = FLT_INT_GET_EXP_WITH_BIAS( X );// смещенная экспонента ( exp+bias )

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
   if( exp_new < ( FLT_INT_SUB_MIN_EXP + FLT_INT_EXP_BIAS - exp_old ))
   {
      // Потеря точности (число меньше чем минимальное неномализованное число)
      errno = ERANGE;
      X.u32 &= FLT_INT_SIGN_MASK;
      return( X.flt );
   }
   if( exp_new < ( FLT_INT_EXP_MIN + FLT_INT_EXP_BIAS - exp_old ))
   {
      // Результат ненормализованное число
      n = FLT_INT_EXP_MIN + FLT_INT_EXP_BIAS - exp_new - exp_old;
      FLT_INT_CLR_SIGN( X ); // сбрасываем знаковый бит чтобы не мешал сдвигу
      FLT_INT_SET_EXP_WITH_BIAS( X, 1 ); // устанавливаем скрытый бит мантиссы для корректного сдвига
      X.u32 >>= (uint32_t)n; // сдвиг вправо - деление ненормализованного числа на 2^n
      errno = ERANGE; // непредставимое ненормализованное число (при свдиге теряются биты)
      // Представимое ненормализованное число
      if( x < 0.0 )
      {
         return( -X.flt );
      }
      return( X.flt );
   }
   // Результат конечное нормализованное число
   FLT_INT_SET_EXP_WITH_BIAS( X, exp_old + exp_new ); // устанавливаем нужную степень
   return( X.flt );
}

#endif // __MATH_INLINE_LDEXPF_FAST_SUBNORMAL_H__
