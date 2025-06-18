// Файл "sqrt_fast.h" содержит inline функции математической библиотеки
//    double MATH_INLINE_SQRT_FAST( double x ).
//
// ============================
// Copyright (C) 2008 NIISI RAN
// ============================
//
// Описание:
//    MATH_INLINE_SQRT_FAST() - функция вычисление квадратного корня числа
//       типа double без предварительных проверок значения аргумента.
//
// Синтаксис:
//    #include <math.h>
//    #include <math_inline/sqrt_fast.h>
//    double MATH_INLINE_SQRT_FAST( double x );
//
// Возвращаемое значение:
//    Функция возвращает значение квадратного корня числа типа double
//
// Особые значения:
//    Если аргумент равен -0.0, функция возвращает -0.0
//    Если аргумент равен +0.0, функция возвращает +0.0
//
// Cтандарт:
//    Функция не соответствует стандарту POSIX 1003.1, 2004 г.
//
// Примечания:
//    Функция  не делает никаких проверок аргументов,
//       но зато вычисляется с максимальной скоростью.
//    Проверка, что аргумент - конечное нормализованное число
//       в области определения функции, должна быть выполнена
//       перед вызовом функции.

#if( !defined( __MATH_INLINE_SQRT_FAST_H__ ))
#define __MATH_INLINE_SQRT_FAST_H__

// Таблица начальных приближений sqrt(x):
extern const int32_t MATH_INLINE_SQRT_TABLE[];

// Аппаратно независимая функция функция извлечения квадратного корня числа типа double:
static double __inline__ __UNUSED_F MATH_INLINE_SQRT_FAST_NOFPU( double x )
{
   DBL_INT_UNION X = { x };
   int32_t x_h;
   double y;

   if( x == 0.0 ) // проверка, чтобы избежать деления на ноль и вернуть правильный результат
   {
      return( x );
   }
   // начальное приближение sqrt(x) - 8 значащих бит:
   x_h = X.i32.h >> 1;
   x_h += 0x1ff80000L;
   X.i32.h = x_h - MATH_INLINE_SQRT_TABLE[( x_h >> 15 ) & 31];
   y = X.dbl;
   // аппроксимация sqrt(x) - 17 значащих бит:
   y = ( y + x / y ) * 0.5;
   // аппроксимация sqrt(x) - 35 значащих бит:
   y = ( y + x / y ) * 0.5;
   // аппроксимация sqrt(x) - до последнего разряда:
   x = y - ( y - x / y ) * 0.5;
   return( x );
}

// Аппаратно зависимая функция извлечения квадратного корня числа типа double:
static double __inline__ __UNUSED_F MATH_INLINE_SQRT_FAST( double x )
{
#if( CPU == I80386 )
   __asm __volatile ( "fsqrt" : "=t" (x) : "0" (x) );
#elif( defined RM7000  )
   __asm __volatile ( "sqrt.d %0, %1" : "=f" (x) : "f" (x) );
#else // CPU == R4000
   return MATH_INLINE_SQRT_FAST_NOFPU( x );
#endif // CPU == R4000
   return( x );
}

#endif //__MATH_INLINE_SQRT_FAST_H__
