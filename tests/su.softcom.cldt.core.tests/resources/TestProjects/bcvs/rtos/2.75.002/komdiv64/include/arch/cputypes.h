/*
 * Copyright
 */
#ifndef _ARCH_CPUTYPES_H
#define _ARCH_CPUTYPES_H

#ifndef I80386
#define I80386    2
#endif

#ifndef R3000
#define R3000	  3
#endif

#ifndef R4000
#define R4000	  4
#endif

#ifndef __TARGET_CPU
#ifdef __R3000__
#define __TARGET_CPU R3000
#endif
#ifdef __R4000__
#define __TARGET_CPU R4000
#endif
#ifdef __i386__
#define __TARGET_CPU I80386
#endif
#endif /* __TARGET_CPU */


#ifndef CPU
#ifdef __TARGET_CPU
#define CPU __TARGET_CPU
#else
#error What CPU?
#endif /* __TARGET_CPU */
#endif /* CPU */

#if CPU != R3000
#undef R3000
#endif

#if CPU != R4000
#undef R4000
#endif

#if CPU != I80386
#undef I80386
#endif

#endif /* !_ARCH_CPUTYPES_H */
