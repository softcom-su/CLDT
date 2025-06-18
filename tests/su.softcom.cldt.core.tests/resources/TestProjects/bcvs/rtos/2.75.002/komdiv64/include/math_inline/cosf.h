// Файл "cosf.h" содержит inline версиию функцию математической библиотеки cosf().
//   (inline версия для аргументов меньших pi/4)

/*
 * ============================
 * Copyright (C) 2008 NIISI RAN
 * ============================
 */


// cosf() - функция для вычисления значение косинуса аргумента, заданного в радианах.

// Синтаксис:
//    #include <math.h>
//    float cosf( float x );
//
// Возвращаемое значение:
//    Функция возвращает значение косинуса аргумента, заданного в радианах.
//
// Особые значения:
//    Если аргумент равен NaN, то функция возвращает NaN.
//    Если аргумент равен -0.0 или +0.0, то возвращается значение 1.0.
//    Если аргумент равен +Inf или -Inf, то функция возвращает значение NaN,
//        и переменной errno присваивается код ошибки [EDOM].
//
// Ошибки:    [EDOM].
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_COSF_H__ ))
#define __MATH_INLINE_COSF_H__

#ifdef BT_MATH_INLINE
#define cosf MATH_INLINE_COSF
#endif

#define M_PI_1_2_1  1.57079632673412561417e+00 /* 0x3FF921FB54400000 */ // pio2_1:   first  33 bit of pi/2
#define M_PI_1_2_1T 6.07710050650619224932e-11 /* 0x3DD0B4611A626331 */ // pio2_1t:  pi/2 - pio2_1
#define M_PI_1_2_2  6.07710050630396597660e-11 /* 0x3DD0B4611A600000 */ // pio2_2:   second 33 bit of pi/2
#define M_PI_1_2_2T 2.02226624879595063154e-21 /* 0x3BA3198A2E037073 */ // pio2_2t:  pi/2 - (pio2_1+pio2_2)

// Таблица коэффициентов для вычисления функции sin():
#define SIN_K1 (-1.66666666666666324348e-01) /* 0xBFC5555555555549 */
#define SIN_K2  (8.33333333332248946124e-03) /* 0x3F8111111110F8A6 */
#define SIN_K3 (-1.98412698298579493134e-04) /* 0xBF2A01A019C161D5 */
#define SIN_K4  (2.75573137070700676789e-06) /* 0x3EC71DE357B1FE7D */
#define SIN_K5 (-2.50507602534068634195e-08) /* 0xBE5AE5E68A2B9CEB */
#define SIN_K6  (1.58969099521155010221e-10) /* 0x3DE5D93A5ACFD57C */

// Таблица коэффициентов для вычисления функции cos():
#define COS_K1  (4.16666666666666019037e-02) /* 0x3FA555555555554C */
#define COS_K2 (-1.38888888888741095749e-03) /* 0xBF56C16C16C15177 */
#define COS_K3  (2.48015872894767294178e-05) /* 0x3EFA01A019CB1590 */
#define COS_K4 (-2.75573143513906633035e-07) /* 0xBE927E4F809C52AD */
#define COS_K5  (2.08757232129817482790e-09) /* 0x3E21EE9EBDB4B1C4 */
#define COS_K6 (-1.13596475577881948265e-11) /* 0xBDA8FAE9BE8838D4 */

// Разложение функции в ряд:
#define TAYLOR_RAW_K5_2(F,X) ((X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*F##_K6)))))
#define SIN_RAW_K5(X) (TAYLOR_RAW_K5_2(SIN,X))
#define SIN_RAW_K6(X) (TAYLOR_RAW_K6(SIN,X))
#define COS_RAW_K6(X) (TAYLOR_RAW_K6(COS,X))
// Разложение функции sin() в ряд в диапазоне -pi/4 < x < pi/4:
#define SIN_PI_1_4(X) (X+X*SIN_RAW_K6(X*X))
#define SIN_PI_1_4_2(X,Y) (X-(-X*X*X*SIN_K1+(X*X*(0.5*Y-X*SIN_RAW_K5(X*X))-Y)))
// Разложение функции cos() в ряд в диапазоне -0.3<x<0.3:
#define COS_PI_1_4(X) (1.0-(0.5*X*X-(X*X*COS_RAW_K6(X*X))))
#define COS_PI_1_4_2(X,Y) (1.0-(0.5*X*X-(X*X*COS_RAW_K6(X*X)-X*Y)))
// Разложение функции cos() в ряд в диапазоне -pi/4<x<pi/4 (Z=X/4):
#define COS_PI_1_4_3(X,Y,Z) (1.0-Z-(0.5*X*X-Z-(X*X*COS_RAW_K6(X*X)-X*Y)))

double __cos_for_large_arg(double x);

#define M_PI_4_HEX   0x3fe921fb54442d18LL // pi/4   = 0.785398163397448
#define M_PI_3_4_HEX 0x4002d97c7f3321d2LL // pi*3/4 = 2.356194490192345

#include <math_inline/fabs.h>

static float __inline__ __UNUSED_F MATH_INLINE_COSF( float fx )
{
   double x = (double)fx;
   DBL_INT_UNION ABS_X = { MATH_INLINE_FABS( x ) }; // двоичное представление модуля аргумента x без знака
   if( ABS_X.u64 <= M_PI_4_HEX )
   {
       // |x| < pi/4
       if( ABS_X.dbl <= 0x1.0p-27 )
       {
          // |x| < 2^(-27)
          return( (float)1.0 );            // cos(x) ~ 1.0
       }
       // |x| <= +pi/4
       return((float)(COS_PI_1_4( x )));    // вычисление cos(x) по формуле Тейлора
   }
   return((float)__cos_for_large_arg( x ));
}

#endif //__MATH_INLINE_COSF_H__
