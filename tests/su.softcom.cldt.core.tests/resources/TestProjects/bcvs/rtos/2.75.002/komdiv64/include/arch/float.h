/*
 * Copyright
 */

#ifndef _ARCH_FLOAT_H
#define _ARCH_FLOAT_H
#include <arch/cputypes.h>
#include <sys/cdefs.h>

#ifndef CPU
#error CPU type is not defined !!!
#endif

#if  CPU == I80386 
#include <arch/i386/float.h>
#endif

#if  CPU == R3000 || CPU == R4000
#include <arch/mips/float.h>
#endif


#endif /* !_ARCH_CPU_H*/

