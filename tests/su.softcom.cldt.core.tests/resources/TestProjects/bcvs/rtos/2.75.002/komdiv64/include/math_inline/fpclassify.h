// fpclassify() - функция для классификации плавающих чисел.
//
// Синтаксис:
//   #include <math.h>
//   int fpclassify( float x );
//   int fpclassify( double x );
//
// Возвращаемое значение:
//   Возвращается одно из следующих (целых) значений:
//     FP_INFINITE  - аргумент x равен плюс или минус бесконечности;
//     FP_NAN       - аргумент x не является числом;
//     FP_SUBNORMAL - аргумент x является ненормализованным числом (очень маленьким);
//     FP_ZERO      - аргумент x является 0 (независимо от знака);
//     FP_NORMAL    - аргумент x является конечным нормализованным числом не равным 0.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FPCLASSIFY_H__ ))
#define __MATH_INLINE_FPCLASSIFY_H__

static __inline__ int __UNUSED_F FPCLASSIFY_FLOAT_INLINE( float x )
{
   FLT_INT_UNION X = { x };
   if( FLT_INT_IS_NAN_OR_INF( X ))
   {
      if( FLT_INT_IS_MANT_NE_ZERO( X ))
      {
         return( FP_NAN );
      }
      return( FP_INFINITE );
   }
   if( FLT_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      if( FLT_INT_IS_ZERO( X ))
      {
         return( FP_ZERO );
      }
      return( FP_SUBNORMAL );
   }
   return( FP_NORMAL );
}

static __inline__ int __UNUSED_F FPCLASSIFY_DOUBLE_INLINE( double x )
{
   DBL_INT_UNION X = { x };
   if( DBL_INT_IS_NAN_OR_INF( X ))
   {
      if( DBL_INT_IS_MANT_NE_ZERO( X ))
      {
         return( FP_NAN );
      }
      return( FP_INFINITE );
   }
   if( DBL_INT_IS_ZERO_OR_SUBNORMAL( X ))
   {
      if( DBL_INT_IS_ZERO( X ))
      {
         return( FP_ZERO );
      }
      return( FP_SUBNORMAL );
   }
   return( FP_NORMAL );
}

#define fpclassify( X ) (( sizeof( X ) == sizeof( float )) ? FPCLASSIFY_FLOAT_INLINE( X ) : FPCLASSIFY_DOUBLE_INLINE( X ))

#endif // __MATH_INLINE_FPCLASSIFY_H__
