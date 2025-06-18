/*
 * Copyright
 */

#ifndef _ARCH_CPU_H
#define _ARCH_CPU_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <arch/cputypes.h>

#ifndef CPU
#error CPU type is not defined !!!
#endif

#if  CPU == I80386 
#include <arch/i386/cpu.h>
#endif

#if  CPU == R3000 || CPU == R4000
#include <arch/mips/cpu.h>
#endif

 
#ifndef	HEAP_ALLOC_ALIGNMENT
#define	HEAP_ALLOC_ALIGNMENT	4
#endif	/*HEAP_ALLOC_ALIGNMENT*/


#ifndef	STACK_ALIGNMENT
#ifdef  CPU_STACK_ALIGNMENT
#define	STACK_ALIGNMENT	CPU_STACK_ALIGNMENT
#else
#define	STACK_ALIGNMENT	4
#endif

#endif	/*STACK_ALIGNMENT*/

#ifndef __ASSEMBLER__
void __exit(int code);
void cpuReboot(void);
void cpuSRSet(unsigned int);
unsigned int cpuSRGet(void);
int cpuFpaSetRounding(int how);
int cpuFpaInit(vfuncptr_t fun);
int cpuFpaSetMask(int ieeef) ;
int cpuFpaAck(void) ;
int cpuFpaSRGet();
int cpuFpaSRSet(int);
void *cpuV2R(pid_t pid, void *vaddr);
#endif /* __ASSEMBLER__ */

#endif /* !_ARCH_CPU_H*/
