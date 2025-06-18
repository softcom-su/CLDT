// fmax() - функция для выбора большего из двух чисел с плавающей запятой типа double.
//
// Синтаксис:
//   double fmax( double x, double y );
//
// Возвращаемое значение:
//   функция возвращает большее из двух чисел с плавающей запятой.
//
// Особые значения:
//   если оба аргумента равны NAN, функция возвращает NaN,
//   если только один аргумент равен NAN, функция возвращает другой аргумент.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FMAX_H__ ))
#define __MATH_INLINE_FMAX_H__

#ifdef BT_MATH_INLINE 
#define fmax MATH_INLINE_FMAX 
#endif

static double __inline__ __UNUSED_F MATH_INLINE_FMAX( double x, double y )
{
#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( isnan( x ))
   {
      return( y );         //  x = [ NAN ]
   }
   if( isnan( y ))
   {
       return( x );         // y = [ NAN ]
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   return(( x > y ) ? x : y );
}

#endif // __MATH_INLINE_FMAX_H__

