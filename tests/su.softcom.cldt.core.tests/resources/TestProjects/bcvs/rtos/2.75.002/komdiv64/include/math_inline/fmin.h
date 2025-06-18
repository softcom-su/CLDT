// fmin() - функция для выбора меньшего из двух чисел с плавающей запятой типа double.
//
// Синтаксис:
//   double fmin( double x, double y );
//
// Возвращаемое значение.
//   Функция возвращает  меньшее из двух чисел с плавающей запятой.
//
// Особые значения.
//   Если оба аргумента равны NAN, функция возвращает NaN,
//   Если только один аргумент равен NAN, функция возвращает другой аргумент.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FMIN_H__ ))
#define __MATH_INLINE_FMIN_H__

#ifdef BT_MATH_INLINE 
#define fmin MATH_INLINE_FMIN 
#endif

static double __inline__ __UNUSED_F MATH_INLINE_FMIN( double x, double y )
{
#if( !defined( BT_MATH_NOCHECK ))
#if( !defined( BT_MATH_NORMAL ))
   if( isnan( x ))
   {
      return( y ); //  x = [ NAN ]
   }
   if( isnan( y ))
   {
      return( x ); // y = [ NAN ]
   }
#endif // BT_MATH_NORMAL
#endif // BT_MATH_NOCHECK
   return(( x < y ) ? x : y );
}

#endif // __MATH_INLINE_FMIN_H__
