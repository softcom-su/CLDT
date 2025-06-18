/*
Copyright
*/
#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef NULL
#define NULL      0
#endif

#define	offsetof(t,m)	(long)((&((t *)0L)->m))

#ifndef _DEF_ptrdiff_t
#define _DEF_ptrdiff_t typedef int ptrdiff_t
_DEF_ptrdiff_t;
#endif
/*
#ifndef _DEF_size_t
#define _DEF_size_t  typedef unsigned int size_t
_DEF_size_t;
#endif
*/
/* Fragment stddef.h 2.7.2.1 */
#ifndef _GCC_SIZE_T
#define _GCC_SIZE_T
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ long unsigned int
#endif
typedef __SIZE_TYPE__ size_t;
#endif  /* _GCC_SIZE_T */

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED  typedef unsigned short wchar_t
_WCHAR_T_DEFINED;
#endif
#endif  /*!_STDDEF_H*/

