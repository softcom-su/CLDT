// Файл "fma_fast.h" содержит вспомогательную функцию математической библиотеки.
//
//	MATH_INLINE_FMA_FAST() - функция для вычисления x*y+z чисел типа double
//		                      без предварительных проверок значения аргументов.
//
// Примечание:
//  Функция не соответствует стандарту POSIX 1003.1, 2004 г., потому что не делает
//  ни каких проверок аргументов, но зато вычисляется с максимальной скоростью.
//  Проверка, что аргументы - конечные нормализованные числа
//  в области определения функции, должна быть сделана перед вызовом функции.
#if( !defined( __MATH_INLINE_FMA_FAST_H__ ))
#define __MATH_INLINE_FMA_FAST_H__

static double __inline__ __UNUSED_F MATH_INLINE_FMA_FAST( double x, double y, double z )
{
#if( CPU == R4000 )
   __asm __volatile ( "madd.d %0, %3, %1, %2" : "=f" (x) : "f" (x), "f" (y), "f" (z) );
#else // CPU == R4000
	return( x * y + z );
#endif // CPU == R4000
	return( x );
}

#endif //__MATH_INLINE_FMA_FAST_H__
