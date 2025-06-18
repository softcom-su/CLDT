#ifndef _ARCH_MIPS_TRACE_H
#define _ARCH_MIPS_TRACE_H

#include <arch/mips/cpu.h>

#ifdef INCLUDE_TRACE
/*
 * С-функции обработки прерываний передается три аргумента:
 *   a0 - аргумент из таблицы векторов прерываний;
 *   a1 - номер вектора прерывания;
 *   a2 - адрес области сохранения;
 */
#  define TRACE_INTERRUPT                                               \
    move a3,v0;               /* адрес функции-обработчика */           \
    jal   traceIntFunc        /* регистрация прерывания с вызовом обработчика */
#else /* INCLUDE_TRACE */

#  define TRACE_INTERRUPT                                               \
    jalr    ra, v0            /* вызов С-обработчика прерывания               */
#endif  /* INCLUDE_TRACE */

#define TRACE_SIZEREGS    sizeof(struct pt_regs)
#define TRACE_SIZEFPREGS  CPU_FPU_CONTEXT_SIZE

#endif /* _ARCH_MIPS_TRACE_H */
