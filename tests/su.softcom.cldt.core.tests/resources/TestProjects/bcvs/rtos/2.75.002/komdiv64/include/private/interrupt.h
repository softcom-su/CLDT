#ifndef _PRIVATE_INTERRUPT_H
#define _PRIVATE_INTERRUPT_H

struct pt_regs;

typedef struct isr_entry {
	void (*ISR)(int);
	int parameter;
} ISR_TABLE;

void exceptionHookAdd(funcptr_t excepHook) ;
int defaultMapSig(int vecnum);
int tryHook(void *threadExecuting, int vecnum, struct pt_regs *r);

#endif
