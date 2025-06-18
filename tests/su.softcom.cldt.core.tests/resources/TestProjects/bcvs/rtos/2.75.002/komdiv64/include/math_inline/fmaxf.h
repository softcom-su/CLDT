// fmaxf() - функция для выбора большего из двух чисел с плавающей запятой типа float.
//
// Синтаксис:
//   float fmaxf( float x, float y );
//
// Возвращаемое значение:
//   функция возвращает большее из двух чисел с плавающей запятой.
//
// Особые значения:
//   если оба аргумента равны NAN, функция возвращает NaN,
//   если только один аргумент равен NAN, функция возвращает другой аргумент.
//
// Cтандарт: функция соответствует стандарту POSIX 1003.1, 2004 г.

#if( !defined( __MATH_INLINE_FMAXF_H__ ))
#define __MATH_INLINE_FMAXF_H__

#ifdef BT_MATH_INLINE 
#define fmaxf MATH_INLINE_FMAXF
#endif

static float __inline__ __UNUSED_F MATH_INLINE_FMAXF( float x, float y )
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

#endif // __MATH_INLINE_FMAXF_H__

