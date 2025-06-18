/*

*/
#ifndef _ARCH_INTERRUPT_H
#define _ARCH_INTERRUPT_H

#define INT_MODE_POSIX    0
#define	INT_MODE_REBOOT   1
#define	INT_MODE_SUSPEND  2
#define	INT_MODE_TRY      3 /*исключение обрабатывается try-блоком*/

#include <arch/cpu.h>

extern   u_int       intNestLevel;
#define  intNest()   (intNestLevel !=0)

void	   intVecInit(int mode);

int 	intConnect (int vector, void (*ISR)(int), int parameter);
int 	intVecGet (int vector, void (**ISR)(int), int *parameter);
void	exceptionHookAdd(funcptr_t excepHook);
#endif                                  /* !_ARCH_INTERRUPT_H */

