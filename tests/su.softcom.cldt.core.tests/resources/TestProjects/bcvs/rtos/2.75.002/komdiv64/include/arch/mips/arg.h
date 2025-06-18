/*
 в настоящее время не используется - stdarg.h, varargs.h от GCC
*/
#ifndef _ARCH_MIPS_ARG_H
#define _ARCH_MIPS_ARG_H

#if 0

#ifdef	_VARARGS_H

#ifndef __MIPSEB__
#define __MIPSEB__
#endif
#include <va-mips.h>

#else /*_STDARG_H*/

#define va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_alignoff(TYPE)  \
  (((__alignof(TYPE) <= __alignof(int))?__alignof(int):__alignof(TYPE)) - 1)

/* arguments smaller than int are in higher memory addresses */

#define va_arg(list, TYPE) 						      \
 (list = 									      \
   (va_list) (((unsigned)list + va_alignoff(TYPE)) & ~va_alignoff(TYPE))  \
      + va_rounded_size(TYPE), 					      \
   *((TYPE *) (void *) (list - (sizeof(TYPE) <= sizeof(int) ? sizeof(TYPE) :    \
				        va_rounded_size(TYPE)))))
#endif /*_VARARGS_H*/

#endif 

#endif /* _ARCH_MIPS_ARG_H */