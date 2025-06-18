#include <version.h>
#include <arch/cputypes.h>
#if OS_VERSION >= 300
#include <arch/cpu.h>
#include <arch/trace.h>
#endif
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>


/*******************************************************************************
*
* traceClockCounterPass - get number of timer ticks from last interrupt
*
* Note: can use only at,v0,t0, k0 or t1
*
*/

#if OS_VERSION < 300
	.extern  boardCompareVal      /* ticks between two interrupts */
#endif

	.globl	traceClockCounterPass
	.ent	traceClockCounterPass
	.set	noreorder
	.set	at

traceClockCounterPass:

#if OS_VERSION >= 300
	mfc0	v0,C0_COMPARE		# v0 = next or last interrupt tick
	mfc0	t1,C0_COUNT		# t1 = current tick
	li	t0,cpuVA_MODDATA
	lw	t0,traceOFFSET_COMPAREADD(t0)
	subu	v0,v0,t1		# v0 = delta: COMPARE - COUNT
#else
	mfc0	v0,C0_COMPARE		# v0 = next or last interrupt tick
	mfc0	k0,C0_COUNT		# k0 = current tick
	lw	t0,boardCompareVal	# t0 = ticks between two interrupts
	subu	v0,v0,k0		# v0 = delta: COMPARE - COUNT
#endif

	bgeu	v0,t0,0f		# if(delta >= tick interval) goto 0;
	nop
	j	ra			# COMPARE set at next tick
	subu	v0,t0,v0		# result = INTERVAL - (COMPARE - COUNT)
0:
	j	ra			# COMPARE set at last tick
	negu	v0			# result = COUNT - COMPARE

	.end  traceClockCounterPass



