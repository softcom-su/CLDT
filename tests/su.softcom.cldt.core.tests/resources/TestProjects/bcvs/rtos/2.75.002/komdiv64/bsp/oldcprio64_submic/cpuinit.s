#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <version.h>
#include <board.h>
#include <bspversion_bcvs.h>  /*gmu: version ppm for bcvs*/

#if OS_VERSION < 300
  #include "configbrd.h"
  #include "bsp.h"
#endif

#ifdef CPRIO64
  #define PMON
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
        mtc0    zero,C0_CAUSE		/* clear sw interrupts */
	mtc0    zero,C0_WATCHLO
	mtc0    zero,C0_WATCHHI
#if OS_VERSION >= 300
        li      t2,SR_FR|SR_CU0|SR_CU1|SR_CU3|SR_BEV|SR_PX|SR_UX|SR_SX|SR_KX	/* CP0, CP1, ROM exception vector */
#else
        li      t2,SR_FR|SR_CU0|SR_CU1|SR_BEV|0x10000	/* CP0, CP1, ROM exception vector */
#endif
        mtc0    t2,C0_SR		/* disable all interrupts */
	nop; nop; nop; nop;
	ctc1	zero,C1_SR		/* mask all FPU exceptions */
	nop; nop; nop; nop;
        la	sp,cpuInit		/* set initial stack pointer */
#if OS_VERSION >= 300
    	subu	k1,sp,0x10000
#endif
	la      gp,_gp			/* set global ptr from compiler */

#ifdef	BOOTER_CONSOLE_BAUD
// init UART-B in 8N1 mode
        li      t0,0xbff70008
        sb      zero,1(t0)
        li      t1,0x3
        sb      t1,3(t0)
        sb      zero,4(t0)
        sb      zero,2(t0)
        li      t1,0x80
        sb      t1,3(t0)
        li      t1,(115200/BOOTER_CONSOLE_BAUD) & 0xff
        sb      t1,0(t0)
        li      t1,(115200/BOOTER_CONSOLE_BAUD) >> 8
        sb      t1,1(t0)
        li      t1,0x3
        sb      t1,3(t0)
        lb      zero,6(t0)
        lb      zero,5(t0)
        lb      zero,2(t0)
#endif

#ifdef	INCLUDE_CRC
        move	s0,a0		/* save StartType */
	move	s1,a1
        move    s2,a2
	la	a0,_ftext	/* binary image start */
	la	a1,_edata	/* binary image end */
	la	t1,crcVerify
	jalr	t1		/* result of CRC compare: 1 - OK, 0 - ERROR */
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
	move	a0,s0		/* restore StartType */
        move	a1,s1
        move 	a2,s2
#endif	/* INCLUDE_CRC */

#if OS_VERSION >=300
	.set	reorder
        bal 	0f
0:      move    a0, ra            /* a0 - adr of cpuInit */
#endif

        la      t0,boardStartAddr
        sw	a1,0(t0)        /* save StartAddress */
#if 0  /* defined(PMON) && OS_VERSION < 300 */
        lui     t0,0xa000
        sw      a2,0x614(t0)
        nop
        la      t0,pmonStructAddr
        sw	a2,0(t0)        /* save PMON's structure address */
#endif
        
	la	t0,osBoardInit
	jalr	t0
        li      ra,R_VEC		/* load prom reset address */
        j       ra			/* just in case */

	nop
        break
        nop


	.end    cpuInit

