/* uncompInit.s -  uncompress program initialization module for K64SMP and K64RIO */

#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <board.h>
#include <configbrd.h>

#define	RAM_ENTRY	uncompInit

#ifdef CPRIO64
  #define PMON
#endif

/*******************************************************************************
* entry point for uncompress routine in ROM: */
#ifndef PMON
/*
* uncompInit:      a2 <- 0
* uncompInit+0x10: a2 <- 1
* uncompInit+0x20: a2 <- 2
*         a2 - argument for romCacheReset:
*          0 - regular mode (for BT211 - inherit CONFIG0 and 0xbb000154 from ROM-program)
*                           ( for MPON - booter makes all initializing)
*          1 - for MPON - start without L2 cache
*              for BT211 - booter makes all initializing and no L2 cache
*          2 - for BT211 - booter makes all initializing and with L2 cache
*              for MPON - as 0
*
*/
#else
/* for PMON's boards a2= address of PMON's structure, which allows to get values of PMON's variables */
#endif
        .globl	uncompress		/* image uncompress routine */
	.globl	uncompInit		/* start of system code */

	.text
        .set	noreorder
	.ent	uncompInit
uncompInit:				/* force power-on startType */
	.set	noreorder

#if !defined(PMON) 
	lui	t1,0xbb00               /*  debugging (???) ROM program */
        lui     t2,0xa000
	lw	t0,0x154(t1)
        mfc0    t3,C0_CONFIG
        sw      t0,0x600(t2)
        sw      t3,0x604(t2)

	la	a2,0
	beqz	a2,0f
	nop
        nop

uncompInit_10:
	la	a2,1
        beqz    r0,0f
        nop
        nop

uncompInit_20:
	la	a2,2
0:
	li	a0,BOOT_COLD            /* rudiment - don't work now */
#else                                 /* PMON's boards */
        lui     t2,0xa000
        sw      a2,0x608(t2)
#endif

	bal	romReboot		/* bal - to get our address in ra */
	nop

romReboot:				/* sw reboot inherits a0 startType */
        .set	noreorder
	addiu	a1,ra,-(romReboot-uncompInit)	/* in a1 - ROM entry address for uncompress */

	/* disable all interrupts, select prom exception handlers */
	li      t0, SR_CU1 | SR_CU0 |SR_BEV
	mtc0    t0, C0_SR
	nop; nop; nop; nop;
#ifndef	DISABLE_FPU
	ctc1	zero,C1_SR		/* mask all FPU exceptions ??? */
#endif
	/* clear software interrupts, disable count */
	lui	t0,0x0800
	mtc0	t0, C0_CAUSE

	/* give us as long as possible before a clock interrupt */
	li	t0, 1
	mtc0	t0, C0_COUNT
	mtc0	zero, C0_COMPARE

#if !defined(CPU_RUN_UNCACHED) && !defined(NO_CACHE_RESET)
	move	s7,a0
	move	s6,a1
	move	s5,a2
#ifdef CPRIO64
        move	a2,zero
#endif
	li	a0,(RAM_COMP_ADRS & 0x1ffffff8)
	lar	t0,romCacheReset
	or	t0,K1BASE
	jalr	t0
	nop

	move	a1,s6
	move	a0,s7
	move	a2,s5

#if 0
#warning test code: set CPU frequency to 96MHz
//	li	t0,0x06013a04 // 120MHz
	li	t0,0x0602ee0d // 96MHz
	lui	t1,0xbb00
	sw	t0,0x184(t1)
	li	t0,7
	sw	t0,0x18c(t1)
#endif

#if 0  /* if for test code set memory frequency to 96MHz */
#warning test code set memory frequency to 96MHz
#if 1
	lar	t0,romCacheReset
	lui	t1,0x1000
	and	t0,t1
	beqz	t0,1f
	nop
#endif
	li	t0,0x0602ee0d // 96MHz
//	li	t0,0x06023a04 // 60MHz
//	li	t0,0x06412604 // 80MHz
	lui	t1,0xbb00
	sw	t0,0x180(t1)
	li	t0,1
	sw	t0,0x18c(t1)

0:
	lw	t0,0x18c(t1)
	andi	t0,1
	bnez	t0,0b
	nop
1:
#endif    /* fi for test code set memory frequency to 96MHz */
#endif    /* if !defined(CPU_RUN_UNCACHED) && !defined(NO_CACHE_RESET) */

#ifdef	ENABLE_HAMMING
#warning enable hamming
	move	s7,a0
	move	s6,a1
	move	s5,a2

	lar	t0,hammEnable
	jalr	t0
	nop

	move	a1,s6
	move	a0,s7
	move	a2,s5
#endif
	.set noreorder

	move	t0,a1		/* rom entry address */
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

	/* copy text and data to ram (128 bytes aligment) */

0:
	lw	t1,0x00(t5)
	lw	t2,0x04(t5)
	sw	t1,0x00(t6)
	sw	t2,0x04(t6)
	lw	t1,0x08(t5)
	lw	t2,0x0c(t5)
	sw	t1,0x08(t6)
	sw	t2,0x0c(t6)
	lw	t1,0x10(t5)
	lw	t2,0x14(t5)
	sw	t1,0x10(t6)
	sw	t2,0x14(t6)
	lw	t1,0x18(t5)
	lw	t2,0x1c(t5)
	sw	t1,0x18(t6)
	sw	t2,0x1c(t6)
	addiu	t5,t5,32
	addiu	t0,t0,-32
	bgtz	t0,0b
	addiu	t6,t6,32

	.set reorder

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! cache, TLB and something else disable
#ifdef	CPU_RUN_UNCACHED
        li      t0,0xbb000154
	li      t1,0xc5c0
	sw      t1,0(t0)
//	li      t0,0x80d1c082
//	mtc0    t0,C0_CONFIG
//	nop; nop; nop; nop;

#endif
	/* set stack to grow down from beginning of data and call init */

	la	gp, _gp			/* set global ptr from compiler */
	la	sp,uncompInit		/* set temp stack ptr */
	addiu	sp,sp,-64
#ifdef	INCLUDE_CRC
	move	s0,a0
	move	s1,a1
	move	s2,a2

	la	a0,uncompInit	/* binary image start */
	la	a1,edata	/* binary image end */
	la	t0,crcVerify
	jal	t0		/* result of CRC compare: 1 - OK, 0 - ERROR */
	la	t0,CRC_NVRAM_ADDRESS	/* non-volatile address for result */
	sll	t1,v0,8
	ori	t1,t1,0xa000
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
	move	a0,s0
	move	a1,s1
	move	a2,s2
#endif	/* INCLUDE_CRC */

//        lui     t0,0xa000            /* debuggung ! */
//        sw      a2,0x60c(t0)
        nop
	la      t0,uncompress		/* jump to uncompress */
	jal    	t0			/* never returns - starts up kernel */
	nop

        break

       	li	ra, R_VEC	/* load prom reset address */
	j	ra		/* just in case */

	.align	3
copy_end:

	.end uncompInit

#ifdef	ENABLE_HAMMING
        .set	noreorder
	.ent	hammEnable
hammEnable:
	lui	t0,0xbb00
	lw	t1,0x174(t0)
	ori	t1,1
	sw	t1,0x174(t0)
	lar	t0,hammStart
	lar	t1,hammEnd
	lui	t2,0xa000
0:
	ld	t3,0(t0)
	sd	t3,0(t2)
	addiu	t0,8
	bne	t0,t1,0b
	addiu	t2,8
	lui	t0,0x8000
	and	t2,~0x3f
	lui	t2,0xa100
	jr	t0
	lui	t1,0xb000
	.align	6
hammStart:
	sd	zero,0x00(t2)
	sd	zero,0x08(t2)
	sd	zero,0x10(t2)
	sd	zero,0x18(t2)
	sd	zero,0x20(t2)
	sd	zero,0x28(t2)
	sd	zero,0x30(t2)
	sd	zero,0x38(t2)
	addiu	t2,0x40
	bne	t2,t1,hammStart
	nop
	jr	ra
	nop
	.align	6
hammEnd:
	nop
	.end hammEnable
#endif
