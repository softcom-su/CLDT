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

        li      t0, 0x510000                    /* debug: calc speed copy 5Mb */
	/* copy text and data to ram (128 bytes aligment) */

/*gmu: for bcvs optimization algorithm*/

/*initialization PCI for inxrement cod */
        /* trace */
        li     t7,0xafffc000
        li     t8,0xabcd0000
        sw     t8,0(t7)
	mfc0   t8, C0_COUNT
        sw     t8,4(t7)
        sw     t5,8(t7)
        sw     t6,12(t7)
        add    t7,t7,16
        /* -----*/

        bal   init_pci_pre_submic
        nop

/* ---------------- copy to ram with increment ----------------------------*/        
        li       t3,0x100000  /* size block 1 Mb*/ 
        srl      t4,t0,20     /* count block 1 Mb*/ 

        /* trace */        
        li     t8,0xabcd1000
        sw     t8,0(t7)
	mfc0   t8, C0_COUNT
        sw     t8,4(t7)
        sw     t3,8(t7)
        sw     t4,12(t7)
        sw     t1,16(t7)
        add    t7,t7,20
        /* -----*/
0:

	ld	t1,0x00(t5)
	ld	t2,0x08(t5)
	sd	t1,0x00(t6)
	sd	t2,0x08(t6)
	ld	t1,0x10(t5)
	ld	t2,0x18(t5)
	sd	t1,0x10(t6)
	sd	t2,0x18(t6)

	ld	t1,0x20(t5)
	ld	t2,0x28(t5)
	sd	t1,0x20(t6)
	sd	t2,0x28(t6)
	ld	t1,0x30(t5)
	ld	t2,0x38(t5)
	sd	t1,0x30(t6)
	sd	t2,0x38(t6)

	addiu	t5,t5,64
	addiu	t3,t3,-64
	bgtz	t3,0b
	addiu	t6,t6,64

         /* trace */
        li     t8,0xaaaaaaaa
        sw     t8,0(t7)
        mfc0   t8, C0_COUNT
        sw     t8,4(t7)  
        sw     t4,8(t7)
        sw     t3,12(t7)
        sw     t0,16(t7)
        add    t7,t7,20
        /* -----*/
       
        /* increment cod sVM6LIVEINC*/
        bal   inc_sVM6LIVEINC
        nop 
        /* increment cod sVM7LIVEINC*/
        bal    inc_sVM7LIVEINC
        nop 
        

        addi    t4,t4,-1       
        bgtz	t4,0b
        li      t3,0x100000       /* size 1 block 1Mb*/

        li      t3,0xFFFFF
        and     t3,t3,t0          /* rest */
        li      t4,0xFFF00000
        and     t0,t0,t4

        /* trace */
        li     t8,0xbbbbbbbb
        sw     t8,0(t7)
        mfc0   t8, C0_COUNT
        sw     t8,4(t7) 
        sw     t3,8(t7)
        sw     t4,12(t7)    
        sw     t0,16(t7)    
        add    t7,t7,20
        /* -----*/

        bnez    t3,0b             /* copy rest to RAM*/
        li      t4,1

        /* trace */
        li     t8,0xabcd2000
        sw     t8,0(t7)
        mfc0   t8, C0_COUNT
        sw     t8,4(t7)
        add    t7,t7,8
        /* -----*/
/* ---------------- copy to ram with increment  end --------------------------*/    
        /* trace */
        li     t8,0xabcd3000
        sw     t8,0(t7)
        mfc0   t8, C0_COUNT
        sw     t8,4(t7)
        add    t7,t7,8
        /* -----*/
/*debug -------------*/
end_debug: 
        li t1,0xbfc00180  /*mprime handler*/
        jr  t1
        nop
.asciz "END debug \n"
/* gmu: end -------------*/
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
        nop

#include "pscinit_submic.s"

	.align	3
copy_end:

regaSaved:
	.word 	0
	.end	flashInit
//#include <cacheReset.s>
