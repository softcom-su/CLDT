// Файл "math_const.h" содержит описание математических констант
//    используемых в математической библиотеке.

#if( !defined( __MATH_CONST_H__ ))
#define __MATH_CONST_H__

#include <math_inline/math_bits.h>

#define M_PI_2_HIGH 1.57079632679489655800e+00 // 0x3FF921FB, 0x54442D18
#define M_PI_2_LOW  6.12323399573676603587e-17 // 0x3C91A626, 0x33145C07
#define M_PI_4_HIGH 7.85398163397448278999e-01 // 0x3FE921FB, 0x54442D18

#define FLT_INT_INF_HEX       0x7f800000L
#define FLT_INT_MINUS_INF_HEX 0xff800000L
#define FLT_INT_PLUS_INF_HEX  0x7f800000L

#define DBL_INT_INF_HEX       0x7ff0000000000000LL
#define DBL_INT_MINUS_INF_HEX 0xfff0000000000000LL
#define DBL_INT_PLUS_INF_HEX  0x7ff0000000000000LL

#define FLT_INT_SUB_MAX_HEX   (FLT_INT_MANT_MASK)   // представление максимально возможного ненормализованное числа типа float
#define FLT_INT_SUB_MIN_HEX   0x1L                  // представление минимально  возможного ненормализованное числа типа float
#define FLT_INT_SUB_MIN_EXP   (-160)                // степень минимально возможного ненормализованного числа типа float
#define FLT_INT_SUB_HEX       (FLT_INT_SUB_MAX_HEX)

#define DBL_INT_SUB_MAX_HEX   (DBL_INT_MANT_MASK_64) // представление максимально возможного ненормализованное числа типа double
#define DBL_INT_SUB_MIN_HEX   0x1LL                  // представление минимально  возможного ненормализованное числа типа double
#define DBL_INT_SUB_MIN_EXP   (-1074)                // степень минимально возможного ненормализованного числа типа double
#define DBL_INT_SUB_HEX       (DBL_INT_SUB_MAX_HEX)

union fp_type
{
   unsigned int mant[2];
   double val;
   int64_t i64;  // представление виде целого 64-х разрядного числа со знаком
   uint64_t u64; // представление виде целого 64-х разрядного числа без знака
};
union fp_typef
{
   float val;
   uint32_t u32;
};

extern const union fp_typef floatNAN;
extern const union fp_typef floatINF;


#define  SNAN_VALF        (floatNAN.val)

extern const FLT_INT_UNION FLT_INT_NAN;       // quiet NaN (float)
extern const FLT_INT_UNION FLT_INT_SNAN;      // signaling NaN (float)

extern const DBL_INT_UNION DBL_INT_NAN;       // quiet NaN (double)
extern const DBL_INT_UNION DBL_INT_SNAN;      // signaling NaN (double)

extern const FLT_INT_UNION FLT_INT_INF;       // -Inf (float)
extern const FLT_INT_UNION FLT_INT_MINUS_INF; // -Inf (float)
extern const FLT_INT_UNION FLT_INT_PLUS_INF;  // +Inf (float)

extern const DBL_INT_UNION DBL_INT_INF;       // -Inf (double)
extern const DBL_INT_UNION DBL_INT_MINUS_INF; // -Inf (double)
extern const DBL_INT_UNION DBL_INT_PLUS_INF;  // +Inf (double)

extern const FLT_INT_UNION FLT_INT_PLUS_ZERO; // +0.0
extern const FLT_INT_UNION FLT_INT_MINUS_ZERO;// -0.0

extern const DBL_INT_UNION DBL_INT_PLUS_ZERO; // +0.0
extern const DBL_INT_UNION DBL_INT_MINUS_ZERO;// -0.0

extern const FLT_INT_UNION FLT_INT_HALF;  //  2.0
extern const FLT_INT_UNION FLT_INT_ONE;   //  1.0

extern const DBL_INT_UNION DBL_INT_HALF;  //  0.5
extern const DBL_INT_UNION DBL_INT_ONE;   //  1.0
extern const DBL_INT_UNION DBL_INT_TWO;   //  2.0
extern const DBL_INT_UNION DBL_INT_M1;    // -1.0
extern const DBL_INT_UNION DBL_INT_M2;    // -2.0

extern const DBL_INT_UNION DBL_INT_1022;  // 1022.0
extern const DBL_INT_UNION DBL_INT_1023;  // 1023.0
extern const DBL_INT_UNION DBL_INT_1024;  // 1024.0
extern const DBL_INT_UNION DBL_INT_M1022; // -1022.0
extern const DBL_INT_UNION DBL_INT_M1023; // -1023.0
extern const DBL_INT_UNION DBL_INT_M1024; // -1024.0

extern const DBL_INT_UNION DBL_INT_0_975; //  0.975
extern const DBL_INT_UNION DBL_INT_7_16;  //  0.4375
extern const DBL_INT_UNION DBL_INT_11_16; //  0.6875
extern const DBL_INT_UNION DBL_INT_19_16; //  1.1875
extern const DBL_INT_UNION DBL_INT_39_16; //  2.4375

extern const DBL_INT_UNION DBL_INT_LN_DBL_MAX; // log(DBL_MAX) = 709.7827 - порог переполнения для exp()
extern const DBL_INT_UNION DBL_INT_LN_2_54;    // log(2)*54 = 37.429947750237048

extern const DBL_INT_UNION DBL_INT_DBL_MAX_2; // DBL_MAX/2
extern const DBL_INT_UNION DBL_INT_DBL_MAX_4; // DBL_MAX/4
extern const DBL_INT_UNION DBL_INT_DBL_MAX_8; // DBL_MAX/8 - порог для remquo()

// pi/4:
#define PIo4_DBL_INT_32       0x3fe921fbL
#define PIo4_DBL_INT_64       0x3fe921fb54442d18LL
extern const DBL_INT_UNION    DBL_INT_PIo4;
// pi/2:
#define PIo2_DBL_INT_32       0x3ff921fbL
#define PIo2_DBL_INT_64       0x3ff921fb54442d18LL
// pi*3/4:
#define PI3o4_DBL_INT_32      0x4002d97cL
#define PI3o4_DBL_INT_64      0x4002d97c7f3321d2LL
// pi:
#define PI_DBL_INT_32         0x400921fbL
#define PI_DBL_INT_64         0x400921fb54442d18LL
// pi*5/4:
#define PI5o4_DBL_INT_32      0x400f6a7aL
#define PI5o4_DBL_INT_64      0x400f6a7a2955385eLL
// pi*3/2:
#define PI3o2_DBL_INT_32      0x4012d97cL
#define PI3o2_DBL_INT_64      0x4012d97c7f3321d2LL
// pi*7/4:
#define PI7o4_DBL_INT_32      0x4015fdbbL
#define PI7o4_DBL_INT_64      0x4015fdbbe9bba775LL
// pi*2:
#define PI2_DBL_INT_32        0x401921fbL
#define PI2_DBL_INT_64        0x401921fb54442d18LL
// pi*9/4:
#define PI9o4_DBL_INT_32      0x401c463aL
#define PI9o4_DBL_INT_64      0x401c463abeccb2bbLL
// pi/2*2^19:
#define PIo2_TWO19_DBL_INT_32 0x412921fbL
#define PIo2_TWO19_DBL_INT_64 0x412921fb54442d18LL

// степени 2 для двоичного сравнения степени числа типа float:

extern const FLT_INT_UNION FLT_INT_TWO_23;  //pow(2,+23)={0x1.0p+23}

// степени 2 для двоичного сравнения степени числа типа double:

extern const DBL_INT_UNION DBL_INT_TWO_28;  //pow(2,+28)={0x1.0p+28}={2.6843545600000000e+008}={.u64=0x41b0000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_31;  //pow(2,+31)={0x1.0p+31}={2.1474836480000000e+009}={.u64=0x41e0000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_52;  //pow(2,+52)={0x1.0p+52}={4.5035996273704960e+015}={.u64=0x4330000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_53;  //pow(2,+53)={0x1.0p+53}={9.0071992547409920e+015}={.u64=0x4340000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_64;  //pow(2,+64)={0x1.0p+64}={1.8446744073709552e+019}={.u64=0x43f0000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_66;  //pow(2,+66)={0x1.0p+66}={7.3786976294838206e+019}={.u64=0x4410000000000000}

extern const DBL_INT_UNION DBL_INT_TWO_M27; //pow(2,-27)={0x1.0p-27}={7.4505805969238281e-009}={.u64=0x3e40000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_M28; //pow(2,-28)={0x1.0p-28}={3.7252902984619141e-009}={.u64=0x3e30000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_M29; //pow(2,-29)={0x1.0p-29}={1.8626451492309570e-009}={.u64=0x3e20000000000000}
extern const DBL_INT_UNION DBL_INT_TWO_M54; //pow(2,-54)={0x1.0p-54}={5.5511151231257827e-017}={.u64=0x3c90000000000000}

#endif // #if( !defined( __MATH_CONST_H__ ))
