//    Файл "math.h" содержит математические константы и описания математических
//    функций и макросов (код функций находится в библиотеке libm.а).
//
//    Рассматриваемые функции соответствуют стандарту POSIX 1003.1, 2004 г.
//
// Дополнительные возможности.
//
//    Часть функций реализованы в двух вариантах как обычные функции и как inline функции.
//    Обе реализации соответствуют стандарту POSIX.
//    Использование inline функции по сравнению с обычными функциями
//    сокращает время выполнения функций, но увеличивает размер кода.
//    По умолчанию используются обычные функции, для использования inline функций
//    нужно перед включением в программу файла "math.h"
//    определить define-переменную BT_MATH_INLINE, например:
//
//       #define BT_MATH_INLINE 1
//
//    2. При использовании inline функций можно дополнительно увеличить скорость
//    за счет отказа от проверок аргументов.
//    В этом случае пользователь должен гарантировать корректность аргументов,
//    что является отступлением от стандарта POSIX.
//    За этим исключением функции соответствуют стандарту POSIX и в этом случае.
//
//    Если определить define-переменную BT_MATH_NORMAL
//    перед включением в программу файла "math.h" , например:
//
//        #define BT_MATH_NORMAL 1
//
//    то математические функции будут работать исходя из того,
//    что аргументы являются конечными числами.
//    Поведение математических функции не определено,
//    если в качестве аргумента используется NaN (не число),
//    или -Inf (минус бесконечность), или +Inf (плюс бесконечность),
//    или ненормализованное число.
//    Отметим, что в этом случае проверки, принадлежит ли аргумент
//    области определения функции не будут полностью отменены.
//    Например, если вызвать функцию извлечения квадратного корня sqrt()
//    с аргументом равным -1, то в соответствии со стандартом POSIX функция вернет NaN
//    и присвоит значение [EDOM] переменной errno.
//
//    3. В случае inline функций можно полностью отменить
//    проверку корректности аргументов, если определить
//    define-переменную BT_MATH_NOCHECK перед включением в программу файла "math.h",
//    например:
//
//        #define BT_MATH_NOCHECK 1
//
//    При использовании некорректных значений аргументов (вне области определения функции)
//    поведение функции не определено.


#ifndef    __MATH_H__
#define    __MATH_H__
#undef log
#include  <float.h>
#include  <sys/cdefs.h>

// Флаг в math_errhandling, указыващий что математические функции выставляют перенную errno при возникновении ошибок
#define    MATH_ERRNO     1
// Флаг в math_errhandling, указыващий что математические функции вызывают исключительную ситуацию при возникновении ошибок
#define    MATH_ERREXCEPT 2
// Константа переменная указывающая способ обработки ошибок математических функций:
#define math_errhandling MATH_ERRNO

#define    HUGE_VALF   ARCH_HUGE_VALF // наибольшее представимое число типа float
#define    HUGE_VAL    ARCH_HUGE_VAL  // наибольшее представимое число типа double
//#define    HUGE_VALL ARCH_HUGE_VALL // наибольшее представимое число типа long double

#define    INFINITY    HUGE_VALF         // плюс бесконечность типа float
#define    NAN         (FLT_INT_NAN.flt) // "тихое" не число (quiet NaN) типа float
#define    MAXFLOAT    FLT_MAX           // максимальное не бесконечное число типа float

#if FLT_EVAL_METHOD == 0
  typedef float float_t;
  typedef double double_t;
#elif FLT_EVAL_METHOD == 1
  typedef double float_t;
  typedef double double_t;
#elif FLT_EVAL_METHOD == 2
  typedef long double float_t;
  typedef long double double_t;
#else
  typedef float float_t;
  typedef double double_t;
#endif

// Mатематические константы:
#define M_E         2.7182818284590452354        // число e
#define M_LOG2E     1.4426950408889634074        // число log2 e
#define M_LOG10E    0.43429448190325182765       // число log_10 e
#define M_LN2       0.69314718055994530942       // число ln 2
#define M_LN10      2.30258509299404568402       // число ln 10
#define M_PI        3.14159265358979323846       // число pi
#define M_PI_2      1.57079632679489661923       // число pi/2
#define M_PI_4      0.78539816339744830962       // число pi/4
#define M_1_PI      0.31830988618379067154       // число 1/pi
#define M_2_PI      0.63661977236758134308       // число 2/pi
#define M_2_SQRTPI  1.12837916709551257390       // число 2/sqrt(pi)
#define M_SQRT2     1.41421356237309504880       // число sqrt(2)
#define M_SQRT1_2   0.70710678118654752440       // число 1/sqrt(2)

#define LN2_HI      6.93147180369123816490e-01   /* 3fe62e42 fee00000 */
#define LN2_LO      1.90821492927058770002e-10   /* 3dea39ef 35793c76 */

// Значения, возвращаемые fpclassify():
#define  FP_ZERO        0    // число 0.0 (независимо от знака)
#define  FP_INFINITE    1    // плюс или минус бесконечность
#define  FP_NAN         2    // не число
#define  FP_SUBNORMAL   3    // ненормализованное число
#define  FP_NORMAL      4    // конечное нормализованным число не равное 0

#include <limits.h>          // определения двоичных констант и макросов
#define  FP_ILOGB0      (-INT_MAX)
#define  FP_ILOGBNAN    (INT_MIN)

#include <math_inline/math_bits.h>     // определения двоичных констант и макросов
#include <math_inline/isinf.h>
#include <math_inline/isnan.h>

// Функции для классификации чисел с плавающей запятой:
#include <math_inline/fpclassify.h>
#include <math_inline/isfinite.h>
#include <math_inline/isnormal.h>
#include <math_inline/signbit.h>

// Функции для сравнения чисел с плавающей запятой (включая NаN, -Inf, +Inf ):
#include <math_inline/isgreater.h>
#include <math_inline/isgreaterequal.h>
#include <math_inline/isless.h>
#include <math_inline/islessequal.h>
#include <math_inline/islessgreater.h>
#include <math_inline/isunordered.h>

#ifdef BT_MATH_INLINE

// Функции поиска экстремумов и манипуляции со знаком:

#include <math_inline/copysign.h>
#include <math_inline/copysignf.h>
#include <math_inline/fabs.h>
#include <math_inline/fabsf.h>
#include <math_inline/fdim.h>
#include <math_inline/fdimf.h>
#include <math_inline/fmax.h>
#include <math_inline/fmaxf.h>
#include <math_inline/fmin.h>
#include <math_inline/fminf.h>
#include <math_inline/frexp.h>
#include <math_inline/frexpf.h>
#include <math_inline/ldexp.h>
#include <math_inline/ldexpf.h>
#include <math_inline/modf.h>
#include <math_inline/modff.h>
#include <math_inline/scalb.h>
#include <math_inline/scalbf.h>
// scalbn();   - определена в файле <math_inline/ldexp.h>
// scalbnf();  - определена в файле <math_inline/ldexpf.h>
// scalbln();  - определена в файле <math_inline/ldexp.h>
// scalblnf(); - определена в файле <math_inline/ldexpf.h>

// Алгебраические функции:

#include <math_inline/ceil.h>
#include <math_inline/ceilf.h>
#include <math_inline/floor.h>
#include <math_inline/floorf.h>
#include <math_inline/ilogb.h>
#include <math_inline/ilogbf.h>
#include <math_inline/logb.h>
#include <math_inline/logbf.h>
#include <math_inline/nextafter.h>
#include <math_inline/nextafterf.h>
#include <math_inline/nearbyint.h>
#include <math_inline/nearbyintf.h>
#include <math_inline/rint.h>
#include <math_inline/rintf.h>
#include <math_inline/round.h>
#include <math_inline/roundf.h>
#include <math_inline/sqrt.h>
#include <math_inline/sqrtf.h>
#include <math_inline/trunc.h>
#include <math_inline/truncf.h>

// Тригонометрические функции:

#include <math_inline/cos.h>
#include <math_inline/cosf.h>
#include <math_inline/sin.h>
#include <math_inline/sinf.h>

#else // BT_MATH_INLINE

// Функции поиска экстремумов и манипуляции со знаком:

double copysign( double x, double y );
float  copysignf( float x, float y );
double fabs( double x );
float  fabsf( float x );
double fdim( double x, double y );
float  fdimf( float x, float y );
double fmax( double x, double y );
float  fmaxf( float x, float y );
double fmin( double x, double y );
float  fminf( float x, float y );
double frexp( double x, int* pexp );
float  frexpf( float x, int* pexp );
double ldexp( double x, int exp );
float  ldexpf( float x, int exp );
double modf( double x, double *iptr );
float  modff( float x, float *iptr );
double scalb( double x, double n );
float  scalbf( float x, float n );
double scalbn(double x, int n );
float  scalbnf(float x, int n );
double scalbln(double x, long n );
float  scalblnf(float x, long n );

// Алгебраические функции:

double ceil( double x );
float  ceilf( float x );
double floor( double x );
float  floorf( float x );
int    ilogb( double x );
int    ilogbf( float x );
double logb( double x );
float  logbf( float x );
double nextafter( double x, double y );
float  nextafterf( float x, float y );
double nearbyint( double x );
float  nearbyintf( float x );
double rint( double x );
float  rintf( float x );
double round( double x );
float  roundf( float x );
double sqrt( double x );
float  sqrtf( float x );
double trunc( double x );
float  truncf( float x );

// Тригонометрические функции:

double cos( double x );
float  cosf( float x );
double sin( double x );
float  sinf( float x );

#endif // BT_MATH_INLINE

// Алгебраические функции:

double cbrt( double x );
double exp( double x );
double exp2( double x );
double expm1( double x );
double fma( double x, double y, double z );
double log( double x );
float  logf( float x );
double log10( double x );
double log1p( double x );
double log2( double x );
double fmod( double x, double y );
double hypot( double x, double y );
float  hypotf( float x, float y );
double pow( double x, double y );
double remquo( double x, double y, int* quo );
float  remquof( float x, float y, int* quo );
double remainder( double x, double y );
float  remainderf( float x, float y );

// Тригонометрические функции:

double asin( double x );
double asinh( double x );
double atan( double x );
double atanh( double x );
// Функция atan2() возвращает значение арктангенса отношения аргументов y/x
double atan2( double y, double x );
float  atan2f( float y, float x );
double acos( double x );
double acosh( double x );
double cosh( double x );
int    rem_pio2( double x, double *y );
double sinh( double x );
double tan( double x );
double tanh( double x );

// Специальные функции:

double erf( double x );
double erfc( double x );
double j0(double x);
double j1(double x);
double jn( int n, double x );
double lgamma(double x);
double tgamma(double x);
double y0(double x);
double y1(double x);
double yn(int n, double x);

// Прочие функции:

// Функция nan() возвращает "тихое не число" (quiet NaN) без посылки сигнала
double nan( const char *tagp );
float  nanf( const char *tagp );

// Устаревшие функции:

#define drem remainder

#endif // __MATH_H__
