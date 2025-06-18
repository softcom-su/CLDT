#define _ASMLANGUAGE
#define __ASSEMBLER__

#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <board.h>
#include <configbrd.h>

#define	RAM_ENTRY	flashInit

/*******************************************************************************
* flashInit - entry point for OC2000
*
*/
        .globl  romCacheReset
        .globl  regaSaved
        .globl	flashInit
	.ent	flashInit

flashInit:
	.set	noreorder
//	li	a0, BOOT_COLD
        nop
	bal	romReboot		/* implicite save entry+12 in ra */
	nop

romReboot:				/* sw reboot inherits a0 startType */

        .set	noreorder
	addiu	a1,ra,-(romReboot-flashInit) /* save rom entry */

	/* disable all interrupts, select prom exception handlers */
	mfc0    t0, C0_SR
	li	t1, ~SR_DE
	and	t0, t0, t1 		/* Preserve parity bit */
	or      t0, SR_CU1 | SR_BEV
	mtc0    t0, C0_SR
	ctc1	zero,C1_SR		/* mask all FPU exceptions */

	/* clear software interrupts */
	mtc0	zero, C0_CAUSE
#ifdef  BT206
	mtc0    zero, C0_WATCHLO
	mtc0    zero, C0_WATCHHI
#endif
	/* give us as long as possible before a clock interrupt */
	li	t0, 1
	mtc0	t0, C0_COUNT
	mtc0	zero, C0_COMPARE

	move	s7,a0
	move	s6,a1
//	bal     romCacheReset

        move      a0,zero
	lar	t0,romCacheReset
	or	t0,K1BASE
	jalr	t0
	nop

	move	a1,s6
	move	a0,s7

	.set noreorder

	move	t0,a1			/* rom entry address */
	la	t1,RAM_ENTRY		/* compiler RAM_ENTRY address */
	or	t1,K1BASE
	la	t2,copy_end		/* compiler copy_end address */
	or	t2,K1BASE
	subu	t3,t2,t1		/* copy code length */

	addu	t5,t0,t3		/* copy_end in rom */
	move	t6,t2			/* copy_end in ram */

	/* copy small init code to ram */
0:
	lw	t4,0(t0)	
	addiu	t0,t0,4
	sw	t4,0(t1)
	addiu	t3,t3,-4
	bgtz	t3,0b
	addiu	t1,t1,4

	/* prepare to jump to ram */

	la      k1,copy_ram		/* compiler copy_ram address */
    	j	k1
	nop

	/* prepare to copy text and data do ram */

copy_ram:
	la	t0,edata		/* data end address */
	or	t0,K1BASE
	subu	t0,t0,t6		/* text+data-copy_end length */

        li      t0, 0x500000                    /* debug: calc speed copy 5Мб */
	/* copy text and data to ram (128 bytes aligment) */

0:
/*gmu: for bcvs optimization algorithm*/

	ld	t1,0x00(t5)
	ld	t2,0x08(t5)
	sd	t1,0x00(t6)
	sd	t2,0x08(t6)
	ld	t1,0x10(t5)
	ld	t2,0x18(t5)
	sd	t1,0x10(t6)
	sd	t2,0x18(t6)

	addiu	t5,t5,32
	addiu	t0,t0,-32
	bgtz	t0,0b
	addiu	t6,t6,32

	.set reorder

	la      gp, _gp
        la      sp, flashInit-64 /* set stack, leave room for four param */

#ifdef	INCLUDE_CRC
        move	s0,a0		/* save StartType */
        move	s1,a1		/* save StartAddr */
	la	a0,_ftext	/* binary image start */
	la	a1,edata	/* binary image end */
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
	move	a0,s0		/* restore StartType */
	move	a1,s1		/* restore StartAddr */
#endif	/* INCLUDE_CRC */

#ifdef  BT206
        sw      a0,regaSaved

        la      t0,boardStartType
        sw      a0,0(t0)        /* save StartType */

        la      t0,boardStartAddr
        sw      a1,0(t0)        /* save StartAddress */
#endif
        jal     osBoardInit	/* never returns - starts up kernel */
        li      ra,R_VEC	/* load prom reset address */
        j       ra		/* just in case */

	.align	3
copy_end:

regaSaved:
	.word 	0
	.end	flashInit
//#include <cacheReset.s>
