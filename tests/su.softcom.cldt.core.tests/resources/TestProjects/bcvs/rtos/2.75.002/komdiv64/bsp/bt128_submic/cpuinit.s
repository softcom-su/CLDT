#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <board.h>
#include <version.h>
#include <bspversion_bcvs.h>  /*gmu: version ppm for bcvs*/

#if OS_VERSION < 300
#include <configbrd.h>
#endif

	.globl	osBoardInit

	.text

/*******************************************************************************
*
* cpuInit - entry point for OC2000
*
*/
        .globl	cpuInit
	.ent	cpuInit
        .set	reorder

cpuInit:
        j cpuInit_start
        nop
        .org 0x10,0   /*gmu: offset*/
        .asciz VERSION_PPM_BCVS /*gmu: version ppm for bcvs*/
        nop
        .org 0x80,0 /*gmu: offset*/
cpuInit_start:
	mfc0	t0,C0_SR
	li	t1,~1
	and	t0,t1
	mtc0	t0,C0_SR		/* disable all interrupts */
	nop; nop; nop; nop;

        mtc0    zero,C0_CAUSE		/* clear sw interrupts */
        mtc0    zero,$25,0              /* disable perfomance counter 0 */
        mtc0    zero,$25,2              /* disable perfomance counter 1 */


#if OS_VERSION >=300
        li      t2,SR_FR|SR_CU0|SR_CU1|SR_BEV|SR_PX|SR_UX|SR_SX|SR_KX	/* CP0, CP1, ROM exception vector */
#else
        li      t2,SR_FR|SR_CU0|SR_CU1|SR_BEV|0x10000	/* CP0, CP1, ROM exception vector */
#endif
        mtc0    t2,C0_SR		/* disable all interrupts */
	nop; nop; nop; nop;
	ctc1	zero,C1_SR		/* mask all FPU exceptions */
	nop; nop; nop; nop;

        la	sp,cpuInit		/* set initial stack pointer */
#if OS_VERSION >=300
	subu	k1,sp,0x10000
#endif
        la      gp,_gp			/* set global ptr from compiler */

#ifdef	INCLUDE_CRC
        move	s0,a0		/* save StartType */
	la	a0,_ftext	/* binary image start */
	la	a1,_edata	/* binary image end */
	jal	crcVerify	/* result of CRC compare: 1 - OK, 0 - ERROR */
	la	t0,CRC_NVRAM_ADDRESS	/* non-volatile address for result */
	lhu	t1,0(t0)
	ori	t2,v0,0x00a0
	andi	t1,t1,0xff00
	or	t1,t1,t2
	sh	t1,0(t0)
	bne	v0,zero,1f
#if CRC_ERROR_BEHAVIOR == CRC_REBOOT
	la	t0,CRC_REBOOT_ENTRY
	jr	t0
#endif
#if CRC_ERROR_BEHAVIOR == CRC_STOP
0:	b	0b
#endif
1:
#endif	/* INCLUDE_CRC */

#if OS_VERSION >=300
	.set	reorder
        bal 	0f
0:      move    a0, ra            /* a0 - adr of cpuInit */
#endif

//        li	t2,0xa0f00000
//        li	t1,0xa5a5a5a5
//        sw      t1,(t2)

	la	t0,osBoardInit
99:
        jalr	t0			/* never returns - starts up kernel */
        li      ra,R_VEC		/* load prom reset address */
        j       ra			/* just in case */

	.end    cpuInit

