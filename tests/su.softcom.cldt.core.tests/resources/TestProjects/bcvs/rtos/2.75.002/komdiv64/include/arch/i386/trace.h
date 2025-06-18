#ifndef _ARCH_I386_TRACE_H
#define _ARCH_I386_TRACE_H


#ifdef INCLUDE_TRACE
/*
 * С-функции обработки прерываний передается три аргумента:
 *    - аргумент из таблицы векторов прерываний;
 *    - номер вектора прерывания;
 *    - адрес области сохранения;
 *    - адрес функции обработки прерывания.
 */
#  define TRACE_INTERRUPT                                               \
        mov       cpuIsrTable(,%eax,8),%edx                           ; \
        pushl     %edx                                                ; \
        pushl     %ebp                                                ; \
        pushl     %eax                                                ; \
        mov       cpuIsrTable+4(,%eax,8),%edx                         ; \
        pushl     %edx                                                ; \
        call      traceIntFunc                                        ; \
        addl      $16,%esp

#else /* INCLUDE_TRACE */

/*
        pushl     %ebp
        pushl     %eax                       # push vector number
        mov       cpuIsrTable+4(,%eax,8),%edx
        pushl     %edx
        mov       cpuIsrTable(,%eax,8),%eax
        call      %eax                       # invoke user ISR
        addl      $12,%esp
*/
#  define TRACE_INTERRUPT                                               \
        pushl     %ebp                                                ; \
        pushl     %eax                                                ; \
        mov       cpuIsrTable+4(,%eax,8),%edx                         ; \
        pushl     %edx                                                ; \
        mov       cpuIsrTable(,%eax,8),%eax                           ; \
        call      %eax                                                ; \
        addl      $12,%esp

#endif  /* INCLUDE_TRACE */

/*к основным регистрам добавляем EFLAGS,CS,EIP (4*8 + 4*3)*/
#define TRACE_SIZEREGS    sizeof(struct pt_regs)
#define TRACE_SIZEFPREGS  CPU_FPU_CONTEXT_SIZE

#endif /* _ARCH_I386_TRACE_H */
