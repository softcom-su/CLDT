#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H

#define __dead2 

#ifdef _MSC_VER
#define ASM	   __asm
#define __UNUSED_F
#define __USED_F 
#define UNUSED_ARG(a) (a)
#define __PRETTY_FUNCTION__ (const char *)0
#if _MSC_VER<1300
#define __FUNCTION__ (const char *)0
#endif
#define __inline__ __inline
#pragma warning( disable : 4018 4244 4305 ) 
#endif /* _MSC_VER */

#ifdef __GNUC__
#ifndef __STDC__
#error -traditional not supported
#endif
#define __UNUSED_F __attribute__ ((unused))
#if __GNUC__ >=3 
#define __USED_F __attribute__ ((used))
#else /* __GNUC__ >=3 */
#define __USED_F 
#endif /* __GNUC__ >=3 */
#define UNUSED_ARG(a) 
#endif /* __GNUC__ */

#define __P(a) a

#if defined(__cplusplus)
#define	__BEGIN_DECLS	extern "C" {
#define	__END_DECLS	};
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif /* __cplusplus */

#endif /* _SYS_CDEFS_H */

