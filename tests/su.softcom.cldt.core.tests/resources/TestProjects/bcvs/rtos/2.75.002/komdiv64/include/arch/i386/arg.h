/*
 в настоящее время не используется - stdarg.h, varargs.h от GCC
*/
#ifndef _ARCH_I386_ARG_H
#define _ARCH_I386_ARG_H

#if 0

#ifdef	_VARARGS_H

#define __va_ellipsis ...

#define va_alist  __builtin_va_alist

#define va_dcl    int __builtin_va_alist; __va_ellipsis

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef char * __gnuc_va_list;
#endif

#define va_start(AP)  AP=(char *) &__builtin_va_alist

#define va_end(AP)

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_arg(list, TYPE)						\
 (list += __va_rounded_size (TYPE),					\
  *((TYPE *) (list - __va_rounded_size (TYPE))))

#endif /*_VARARGS_H*/

#endif

#endif /* _ARCH_I386_ARG_H */