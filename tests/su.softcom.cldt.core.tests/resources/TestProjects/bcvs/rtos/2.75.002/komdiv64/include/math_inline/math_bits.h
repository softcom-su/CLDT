//  Файл "math_bits.h" содержит определения типов данных, констант
//    и макросов, которые используются математической библиотекой,
//    для двоичного представления чисел с плавающей запятой.

#if( !defined( __MATH_BITS_H__ ))
#define __MATH_BITS_H__

#include <inttypes.h>     // целочисленные типы данных
#include <errno.h>        // коды ошибок
#if( defined( __baget__ ))
   #include <arch/endian.h>
#else // __baget__
   #include "../arch/endian.h"
#endif // __baget__

// Single Format:
//  1    8               23           ... widths
// +-+-------+-----------------------+
// |s|   e   |           f        ulp|
// +-+-------+-----------------------+
//    msb lsb msb                 lsb ... order

#define FLT_INT_SIGN_MASK ( 0x80000000L )
#define FLT_INT_EXP_MASK  ( 0x7F800000L )
#define FLT_INT_MANT_MASK ( 0x007FFFFFL )

#define FLT_INT_SIGN_SHIFT ( 31 )
#define FLT_INT_EXP_SHIFT  ( 23 )
#define FLT_INT_MANT_SHIFT (  0 )

#define FLT_INT_SIGN_LENGTH (  1 )
#define FLT_INT_EXP_LENGTH  (  8 )
#define FLT_INT_MANT_LENGTH ( 23 )

#define FLT_INT_EXP_BIAS ( 127 )
#define FLT_INT_EXP_MIN  (-126 )
#define FLT_INT_EXP_MAX  ( 127 )

#define FLT_INT_MANT_MSB ( 0x00000001L )
#define FLT_INT_MANT_LSB ( 0x00400000L )

// Double Format:
//  1    11              52           ... widths
// +-+-------+-----------------------+
// |s|   e   |           f        ulp|
// +-+-------+-----------------------+
//    msb lsb msb                 lsb ... order

#define DBL_INT_SIGN_MASK_64 ( 0x8000000000000000LL )
#define DBL_INT_EXP_MASK_64  ( 0x7FF0000000000000LL )
#define DBL_INT_MANT_MASK_64 ( 0x000FFFFFFFFFFFFFLL )

#define DBL_INT_SIGN_SHIFT_64 ( 63 )
#define DBL_INT_EXP_SHIFT_64  ( 52 )
#define DBL_INT_MANT_SHIFT_64 (  0 )

#define DBL_INT_SIGN_LENGTH (  1 )
#define DBL_INT_EXP_LENGTH  ( 11 )
#define DBL_INT_MANT_LENGTH ( 52 )

#define DBL_INT_EXP_BIAS ( 1023 )
#define DBL_INT_EXP_MIN  (-1022 )
#define DBL_INT_EXP_MAX  ( 1023 )

#define DBL_INT_MANT_MSB_64 ( 0x0008000000000000LL )
#define DBL_INT_MANT_LSB_64 ( 0x0000000000000001LL )

// Union Format (1 Double or 2 Int32):
//  1    11     20              32         ... widths
// +-+-------+-------+ +------------------+
// |s|   e   |f[HIGH]| |      f[LOW]   ulp|
// +-+-------+-------+ +------------------+
//    msb lsb msb  lsb   msb            lsb ... order

#define DBL_INT_SIGN_MASK_32        ( 0x80000000L )
#define DBL_INT_EXP_MASK_32         ( 0x7FF00000L )
#define DBL_INT_MANT_MASK_HIGH_32   ( 0x000FFFFFL )
#define DBL_INT_MANT_MASK_LOW_32    ( 0x000FFFFFL )

#define DBL_INT_SIGN_SHIFT_32       ( 31 )
#define DBL_INT_EXP_SHIFT_32        ( 20 )
#define DBL_INT_MANT_SHIFT_HIGH_32  (  0 )
#define DBL_INT_MANT_SHIFT_LOW_32   (  0 )

#define DBL_INT_SIGN_LENGTH_32      (  1 )
#define DBL_INT_EXP_LENGTH_32       ( 11 )
#define DBL_INT_MANT_LENGTH_HIGH_32 ( 20 )
#define DBL_INT_MANT_LENGTH_LOW_32  ( 32 )

// Макрос для определения длины целого числа для двоичного представления чисел типа double:
#if( defined( __mips64 ) )
   #define BT_MATH_ARCH 64
#else  // __mips64
   #define BT_MATH_ARCH 32
#endif  // __mips64

// Тип данных для целочисленного представления чисел с плавающей запятой одинарной точности (float):
typedef union
{
   float flt; // представление виде числа с плавающей запятой одинарной точности
   int32_t i32; // представление виде целого 32-х разрядного числа со знаком
   uint32_t u32; // представление виде целого 32-х разрядного числа без знака
} FLT_INT_UNION;

// Тип данных для целочисленного представления чисел с плавающей запятой двойной точности (double):
typedef union
{
   double dbl; // представление виде числа с плавающей запятой двойной точности
   int64_t i64; // представление виде целого 64-х разрядного числа со знаком
   uint64_t u64; // представление виде целого 64-х разрядного числа без знака
   struct
   {
#if( defined( BYTE_ORDER ))
#if( BYTE_ORDER == LITTLE_ENDIAN ) // Архитектура Intel
      int32_t l; // младшая часть мантиссы
      int32_t h; // знак, экспонента, старшая часть мантиссы
#endif // LITTLE_ENDIAN
#if( BYTE_ORDER == BIG_ENDIAN ) // Архитектура MIPS
   int32_t h; // знак, экспонента, старшая часть мантиссы
   int32_t l; // младшая часть мантиссы
#endif // BIG_ENDIAN
#endif // BYTE_ORDER
   } i32; // представление виде двух целых 32-х разрядных числ со знаком
   struct
   {
#if( defined( BYTE_ORDER ))
#if( BYTE_ORDER == LITTLE_ENDIAN ) // Архитектура Intel
      uint32_t l;// младшая часть мантиссы
      uint32_t h;// знак, экспонента, старшая часть мантиссы
#endif // LITTLE_ENDIAN
#if( BYTE_ORDER == BIG_ENDIAN ) // Архитектура MIPS
   uint32_t h;// знак, экспонента, старшая часть мантиссы
   uint32_t l;// младшая часть мантиссы
#endif // BIG_ENDIAN
#endif
   } u32; // представление виде двух целых 32-х разрядных числ без знака
} DBL_INT_UNION;

#include "math_const.h" // математические константы в целочисленном представлении

// Макрос для выделения целочисленного представления знака числа с плавающей запятой X (типа DBL_INT_UNION):
#define FLT_INT_SIGN( X )    ( (X).i32   & (int32_t)FLT_INT_SIGN_MASK    )
#define DBL_INT_SIGN_32( X ) ( (X).i32.h & (int32_t)DBL_INT_SIGN_MASK_32 )
#define DBL_INT_SIGN_64( X ) ( (X).i64   & (int64_t)DBL_INT_SIGN_MASK_64 )

// Макрос для выделения целочисленной представления экспоненциальной части числа с плавающей запятой X (типа DBL_INT_UNION):
#define FLT_INT_EXP( X )    ( (X).u32   & (uint32_t)FLT_INT_EXP_MASK    )
#define DBL_INT_EXP_32( X ) ( (X).u32.h & (uint32_t)DBL_INT_EXP_MASK_32 )
#define DBL_INT_EXP_64( X ) ( (X).u64   & (uint64_t)DBL_INT_EXP_MASK_64 )

// Макрос для выделения целочисленного представления мантисы числа с плавающей запятой X (типа DBL_INT_UNION):
#define FLT_INT_MANT( X ) ( (X).u32 & (uint32_t)FLT_INT_MANT_MASK )
#define DBL_INT_MANT( X ) ( (X).u64 & (uint64_t) DBL_INT_MANT_MASK_64 )

// Макросы для проверки, что мантисса числа с плавающей запятой равна или не равна нулю:
#define FLT_INT_IS_MANT_EQ_ZERO( X ) ( !( (X).u32 & (uint32_t)FLT_INT_MANT_MASK ))
#define FLT_INT_IS_MANT_NE_ZERO( X ) ( (X).u32 & (uint32_t)FLT_INT_MANT_MASK )
#define DBL_INT_IS_MANT_EQ_ZERO( X ) ( !( (X).u64 & (uint64_t)DBL_INT_MANT_MASK_64 ))
#define DBL_INT_IS_MANT_NE_ZERO( X ) ( (X).u64 & (uint64_t)DBL_INT_MANT_MASK_64 )

// Макросы для двоичной проверки числа X (типа DBL_INT_UNION) на +0.0, -0.0
//или ненормализованное число (порядок числа равен 0):
#define FLT_INT_IS_ZERO_OR_SUBNORMAL( X ) ( !( (X).u32 & (uint32_t)FLT_INT_EXP_MASK ))
#define FLT_INT_IS_ZERO( X )      ( !( (X).u32 & (uint32_t)( FLT_INT_EXP_MASK | FLT_INT_MANT_MASK )))
#define FLT_INT_IS_SUBNORMAL( X ) (( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))&&( FLT_INT_MANT( X )))
#if( defined( BT_MATH_ARCH )&&( BT_MATH_ARCH == 64 ))
   #define DBL_INT_IS_ZERO_OR_SUBNORMAL( X ) ( !( (X).u64 & (uint64_t)DBL_INT_EXP_MASK_64 ))
   #define DBL_INT_IS_ZERO( X )      ( !( (X).u64 & (uint64_t)( DBL_INT_EXP_MASK_64 | DBL_INT_MANT_MASK_64 )))
#else
   #define DBL_INT_IS_ZERO_OR_SUBNORMAL( X ) ( !( (X).u32.h & (uint32_t)DBL_INT_EXP_MASK_32 ))
   #define DBL_INT_IS_ZERO( X )      ( !(((X).u32.h & (uint32_t)( DBL_INT_EXP_MASK_32 | DBL_INT_MANT_MASK_HIGH_32 ))|( (X).u32.l )))
#endif
#define DBL_INT_IS_SUBNORMAL( X ) (( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))&&( DBL_INT_MANT( X )))

// Макросы для двоичной проверки числа с плавающей запятой X (типа DBL_INT_UNION) на NaN, -Inf и +Inf (порядок числа максимален):
#define FLT_INT_IS_NAN_OR_INF( X ) ((( (X).u32 & (uint32_t)FLT_INT_EXP_MASK )) == (uint32_t)FLT_INT_EXP_MASK )
#define FLT_INT_IS_NAN( X )        (( FLT_INT_IS_NAN_OR_INF( X ))&&( FLT_INT_MANT( X )))
#define FLT_INT_IS_INF( X )        (( FLT_INT_IS_NAN_OR_INF( X ))&&( !( FLT_INT_MANT( X ))))
#if( defined( BT_MATH_ARCH )&&( BT_MATH_ARCH == 64 ))
   #define DBL_INT_IS_NAN_OR_INF( X ) ((( (X).u64 & (uint64_t)DBL_INT_EXP_MASK_64 )) == (uint64_t)DBL_INT_EXP_MASK_64 )
#else
   #define DBL_INT_IS_NAN_OR_INF( X ) ((( (X).u32.h & (uint32_t)DBL_INT_EXP_MASK_32 )) == (uint32_t)DBL_INT_EXP_MASK_32 )
#endif
   #define DBL_INT_IS_NAN( X ) (( DBL_INT_IS_NAN_OR_INF( X ))&&( DBL_INT_MANT( X )))
   #define DBL_INT_IS_INF( X ) (( DBL_INT_IS_NAN_OR_INF( X ))&&( !( DBL_INT_MANT( X ))))

// Макросы для двоичной проверки знака числа X (типа DBL_INT_UNION):
#define FLT_INT_IS_NEG( X )    ( FLT_INT_SIGN( X ))
#define DBL_INT_IS_NEG_32( X ) ( DBL_INT_SIGN_32( X ))
#define DBL_INT_IS_NEG_64( X ) ( DBL_INT_SIGN_64( X ))
#define FLT_INT_IS_POS( X )    ( !( FLT_INT_SIGN( X )))
#define DBL_INT_IS_POS_32( X ) ( !( DBL_INT_SIGN_32( X )))
#define DBL_INT_IS_POS_64( X ) ( !( DBL_INT_SIGN_64( X )))

// Макросы для двоичного сравнения положительного числа X (типа FLT_INT_UNION)
// c положительным числом Y (типа FLT_INT_UNION):
#define FLT_INT_LT( X, Y ) ( (X).u32 <  (Y).u32 )
#define FLT_INT_LE( X, Y ) ( (X).u32 <= (Y).u32 )
#define FLT_INT_EQ( X, Y ) ( (X).u32 == (Y).u32 ) // можно сравнивать числа со знаком
#define FLT_INT_NE( X, Y ) ( (X).u32 != (Y).u32 ) // можно сравнивать числа со знаком
#define FLT_INT_GE( X, Y ) ( (X).u32 >= (Y).u32 )
#define FLT_INT_GT( X, Y ) ( (X).u32 >  (Y).u32 )

// Макросы для двоичного сравнения положительного числа X (типа DBL_INT_UNION)
// c положительным числом Y (типа DBL_INT_UNION):
#define DBL_INT_LT( X, Y ) ( (X).u64 <  (Y).u64 )
#define DBL_INT_LE( X, Y ) ( (X).u64 <= (Y).u64 )
#define DBL_INT_EQ( X, Y ) ( (X).u64 == (Y).u64 ) // можно сравнивать числа со знаком
#define DBL_INT_NE( X, Y ) ( (X).u64 != (Y).u64 ) // можно сравнивать числа со знаком
#define DBL_INT_GE( X, Y ) ( (X).u64 >= (Y).u64 )
#define DBL_INT_GT( X, Y ) ( (X).u64 >  (Y).u64 )

// Макрос для проверки знаковых битов сразу двух чисел (оба числа отрицательны):
//    TRUE  - если оба числа отрицательные и
//    FALSE - если оба числа положительные или разных знаков
#define DBL_INT_IS_X_AND_Y_NEG(X,Y) (( X.i32.h & Y.i32.h )&DBL_INT_SIGN_MASK_32)

// Макрос для проверки знаковых битов сразу двух чисел (хотя бы одно число отрицательно):
//    TRUE  - если оба числа отрицательные или разных знаков и
//    FALSE - если оба числа положительные и
#define DBL_INT_IS_X_OR_Y_NEG(X,Y) (( X.i32.h | Y.i32.h )&DBL_INT_SIGN_MASK_32)

// Макрос для проверки на равенство нулю сразу двух чисел (оба числа равны нулю):
//    TRUE  - если оба числа равны нулю и
//    FALSE - если хотя бы одно из чисел не равно нулю
#define DBL_INT_IS_X_AND_Y_ZERO(X,Y) (!(( X.i64 | Y.i64 )&(~DBL_INT_SIGN_MASK_64)))

// Макросы для очистки или установки знакового разряда числа с плавающей запятой X
// ( требует предварительной декларации DBL_INT_UNION X ):
#define FLT_INT_CLR_SIGN( X ) ( (X).i32 &= ~( (int32_t)FLT_INT_SIGN_MASK ))
#define FLT_INT_SET_SIGN( X ) ( (X).i32 |= (int32_t)FLT_INT_SIGN_MASK )
#if( defined( BT_MATH_ARCH )&&( BT_MATH_ARCH == 64 ))
   #define DBL_INT_CLR_SIGN( X ) ( (X).i64 &= ~( (int64_t)DBL_INT_SIGN_MASK_64 ))
   #define DBL_INT_SET_SIGN( X ) ( (X).i64 |= (int64_t)DBL_INT_SIGN_MASK_64 )
#else
   #define DBL_INT_CLR_SIGN( X ) ( (X).i32.h &= ~( (int32_t)DBL_INT_SIGN_MASK_32 ))
   #define DBL_INT_SET_SIGN( X ) ( (X).i32.h |= (int32_t)DBL_INT_SIGN_MASK_32 )
#endif

// Макросы для получения и установки целочисленной экспоненциальной части числа с плавающей запятой X
// ( требуется предварительная декларация двочного передставления числа DBL_INT_UNION или FLT_INT_UNION ):

#define FLT_INT_GET_EXP( X ) ((( (X).i32 & (int32_t)FLT_INT_EXP_MASK ) >> (int32_t)FLT_INT_EXP_SHIFT ) - (int32_t)FLT_INT_EXP_BIAS )
#define FLT_INT_SET_EXP( X, EXP ) {( (X).i32 &= ~((int32_t)FLT_INT_EXP_MASK ));( (X).i32 |= ((( (int32_t)EXP + (int32_t)FLT_INT_EXP_BIAS ) << (int32_t)FLT_INT_EXP_SHIFT ) & (int32_t)FLT_INT_EXP_MASK ));}
#define FLT_INT_GET_EXP_WITH_BIAS( X ) (( (X).i32 & (int32_t)FLT_INT_EXP_MASK ) >> (int32_t)FLT_INT_EXP_SHIFT )
#define FLT_INT_SET_EXP_WITH_BIAS( X, EXP ) {( (X).i32 &= ~((int32_t)FLT_INT_EXP_MASK ));( (X).i32 |= ((( (int32_t)EXP ) << (int32_t)FLT_INT_EXP_SHIFT ) & (int32_t)FLT_INT_EXP_MASK ));}
#if( defined( BT_MATH_ARCH )&&( BT_MATH_ARCH == 64 ))
   #define DBL_INT_GET_EXP( X ) ((( (X).i64 & (int64_t)DBL_INT_EXP_MASK_64 ) >> (int64_t)DBL_INT_EXP_SHIFT_64 ) - (int64_t)DBL_INT_EXP_BIAS )
   #define DBL_INT_SET_EXP( X, EXP ) {( (X).i64 &= ~((int64_t)DBL_INT_EXP_MASK_64 ));( (X).i64 |= ((( (int64_t)EXP + (int64_t)DBL_INT_EXP_BIAS ) << (int64_t)DBL_INT_EXP_SHIFT_64 ) & (int64_t)DBL_INT_EXP_MASK_64 ));}
   #define DBL_INT_GET_EXP_WITH_BIAS( X ) (( (X).i64 & (int64_t)DBL_INT_EXP_MASK_64 ) >> (int64_t)DBL_INT_EXP_SHIFT_64 )
   #define DBL_INT_SET_EXP_WITH_BIAS( X, EXP ) {( (X).i64 &= ~((int64_t)DBL_INT_EXP_MASK_64 ));( (X).i64 |= ((( (int64_t)EXP ) << (int64_t)DBL_INT_EXP_SHIFT_64 ) & (int64_t)DBL_INT_EXP_MASK_64 ));}
#else
   #define DBL_INT_GET_EXP( X ) ((( (X).i32.h & (int32_t)DBL_INT_EXP_MASK_32 ) >> (int32_t)DBL_INT_EXP_SHIFT_32 ) - (int32_t)DBL_INT_EXP_BIAS )
   #define DBL_INT_SET_EXP( X, EXP ) {( (X).i32.h &= ~((int32_t)DBL_INT_EXP_MASK_32 ));( (X).i32.h |= ((( (int32_t)EXP + (int32_t)DBL_INT_EXP_BIAS ) << (int32_t)DBL_INT_EXP_SHIFT_32 ) & (int32_t)DBL_INT_EXP_MASK_32 ));}
   #define DBL_INT_GET_EXP_WITH_BIAS( X ) (( (X).i32.h & (int32_t)DBL_INT_EXP_MASK_32 ) >> (int32_t)DBL_INT_EXP_SHIFT_32 )
   #define DBL_INT_SET_EXP_WITH_BIAS( X, EXP ) {( (X).i32.h &= ~((int32_t)DBL_INT_EXP_MASK_32 ));( (X).i32.h |= ((( (int32_t)EXP ) << (int32_t)DBL_INT_EXP_SHIFT_32 ) & (int32_t)DBL_INT_EXP_MASK_32 ));}
#endif

#if( defined( BT_MATH_ARCH )&&( BT_MATH_ARCH == 64 ))
   #define DBL_INT_IS_NEG( X ) DBL_INT_IS_NEG_64( X )
   #define DBL_INT_IS_POS( X ) DBL_INT_IS_POS_64( X )
   #define DBL_INT_SIGN( X ) DBL_INT_SIGN_64( X )
   #define DBL_INT_EXP( X ) DBL_INT_EXP_64( X )
#else
   #define DBL_INT_IS_NEG( X ) DBL_INT_IS_NEG_32( X )
   #define DBL_INT_IS_POS( X ) DBL_INT_IS_POS_32( X )
   #define DBL_INT_SIGN( X )   DBL_INT_SIGN_32( X )
   #define DBL_INT_EXP( X )    DBL_INT_EXP_32( X )
#endif

// Разложение функции в ряд Тейлора:
#define TAYLOR_RAW_K2(F,X)  ((X)*(F##_K1+(X)*(F##_K2)))
#define TAYLOR_RAW_K3(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3))))
#define TAYLOR_RAW_K4(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4)))))
#define TAYLOR_RAW_K5(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5))))))
#define TAYLOR_RAW_K6(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6)))))))
#define TAYLOR_RAW_K7(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6+(X)*(F##_K7))))))))
#define TAYLOR_RAW_K8(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6+(X)*(F##_K7+(X)*(F##_K8)))))))))
#define TAYLOR_RAW_K9(F,X)  ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6+(X)*(F##_K7+(X)*(F##_K8+(X)*(F##_K9))))))))))
#define TAYLOR_RAW_K10(F,X) ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6+(X)*(F##_K7+(X)*(F##_K8+(X)*(F##_K9+(X)*(F##_K10)))))))))))
#define TAYLOR_RAW_K11(F,X) ((X)*(F##_K1+(X)*(F##_K2+(X)*(F##_K3+(X)*(F##_K4+(X)*(F##_K5+(X)*(F##_K6+(X)*(F##_K7+(X)*(F##_K8+(X)*(F##_K9+(X)*(F##_K10+(X)*(F##_K11)))))))))))

#define GET_HIGH_WORD(i,d) \
do {                       \
   DBL_INT_UNION X;        \
   X.dbl = (d);            \
   (i) = X.u32.h;          \
} while (0)

#define GET_LOW_WORD(i,d) \
do {                      \
   DBL_INT_UNION X;       \
   X.dbl = (d);           \
   (i) = X.u32.l;         \
} while (0)

#define EXTRACT_WORDS(ix0,ix1,d) \
do {                      \
   DBL_INT_UNION X;       \
   X.dbl = (d);           \
  (ix0) = X.u32.h;        \
  (ix1) = X.u32.l;        \
} while (0)

#endif // __MATH_BITS_H__
