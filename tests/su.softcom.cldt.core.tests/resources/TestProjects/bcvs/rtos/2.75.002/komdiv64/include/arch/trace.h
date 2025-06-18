/*
 * Copyright
 */

#ifndef _ARCH_TRACE_H
#define _ARCH_TRACE_H

#if  CPU == I80386
#  include <arch/i386/trace.h>
#elif  CPU == R3000 || CPU == R4000
#  include <arch/mips/trace.h>
#else
#  error CPU type is not defined !!!
#endif

#endif /* !_ARCH_TRACE_H*/

