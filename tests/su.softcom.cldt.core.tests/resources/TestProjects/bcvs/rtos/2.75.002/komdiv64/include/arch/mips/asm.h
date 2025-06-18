#ifndef _MIPS_ASM_H
#define _MIPS_ASM_H

#include <arch/endian.h>
#define LEAF(a)  .globl a;.ent a; a:;
#define XLEAF(a)  .globl a;a:;
#define END(a)    .end a
#define NGLEAF(a)    .ent   a;  .set reorder ; a:    
#define ELEAF(a)     a##_end:; .globl   a##_end; .end a;
#define EXPORT(a)
/*      $NetBSD: endian_machdep.h,v 1.1 2000/03/17 00:09:22 mycroft Exp $       */

#ifndef BYTE_ORDER
# error  Define MIPS target CPU endian-ness in port-specific header file.
#endif
#ifdef __ASSEMBLER__
/*
 *   Endian-independent assembly-code aliases for unaligned memory accesses.
 */
#if BYTE_ORDER == LITTLE_ENDIAN
# define LWHI lwr
# define LWLO lwl
# define SWHI swr
# define SWLO swl
#endif

#if BYTE_ORDER == BIG_ENDIAN
# define LWHI lwl
# define LWLO lwr
# define SWHI swl
# define SWLO swr
#endif
#ifdef K64SMP
#define DELAY nop;nop;nop;nop
#else
#define DELAY 
#endif
#endif /* __ASSEMBLER__ */
#if __LP64__
#define xaddu  daddu
#define xaddiu daddiu
#define xsubu  dsubu
#define xla    dla
#define lx     ld
#define sx     sd
#define xli    dli
#define xmtc0  dmtc0
#define xmfc0  dmfc0
#define xlong  .quad
#define swapx  swapd
#define SIZEOFLONG_BITS 3
#else
#define xaddu  addu
#define xaddiu addiu
#define xsubu  subu
#define xla    la
#define lx     lw
#define sx     sw
#define xli    li
#define xmtc0  mtc0
#define xmfc0  mfc0
#define xlong  .long
#define swapx  swapw
#define SIZEOFLONG_BITS 2
#endif

#endif /*!_MIPS_ASM_H*/
