/* uncompInit.s -  uncompress program initialization module */

#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <board.h>
#include <configbrd.h>

#define	RAM_ENTRY	uncompInit

/*******************************************************************************
*
* uncompInit - entry point for uncompress routine in ROM
*
*/

        .globl	uncompress		/* image uncompress routine */
	.globl	uncompInit		/* start of system code */

	.text
        .set	noreorder
	.ent	uncompInit

uncompInit:				/* force power-on startType */
	.set	noreorder
	li	a0, BOOT_COLD
	bal	romReboot		/* implicite save entry+12 in ra */
	nop

romReboot:				/* sw reboot inherits a0 startType */

        .set	noreorder
	addiu	a1,ra,-(romReboot-uncompInit)  /* true rom entry adr for uncompress */

	mfc0	t0,C0_SR
	li	t1,~1
	and	t0,t1
	mtc0	t0,C0_SR		/* disable all interrupts */
	nop; nop; nop; nop;

        mtc0    zero,C0_CAUSE		/* clear sw interrupts */
	mtc0    zero,$25,0              /* disable perfomance counter 0 */
        mtc0    zero,$25,2              /* disable perfomance counter 1 */
        li      t2,SR_FR|SR_CU0|SR_CU1|SR_BEV|0x10000	/* CP0, CP1, ROM exception vector */
        mtc0    t2,C0_SR		/* disable all interrupts */
	nop; nop; nop; nop;
	ctc1	zero,C1_SR		/* mask all FPU exceptions */
	nop

	move	s7,a0
	move	s6,a1

	li	a0,(RAM_COMP_ADRS & 0x1ffffff8)
	lar	t0,romCacheReset
	or	t0,K1BASE
	jalr	t0
	nop

	move	a1,s6
	move	a0,s7

	.set noreorder

	move	t0,a1		/* rom entry address */
	#addiu	t0,ra,-12		/* rom entry address */
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

	/* set stack to grow down from beginning of data and call init */

	la	gp, _gp			/* set global ptr from compiler */
	la	sp,uncompInit		/* set temp stack ptr */
	addiu	sp,sp,-64
#ifdef	INCLUDE_CRC
	move	s0,a0
	move	s1,a1
	la	a0,uncompInit	/* binary image start */
	la	a1,edata	/* binary image end */
	jal	crcVerify	/* result of CRC compare: 1 - OK, 0 - ERROR */
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
#endif	/* INCLUDE_CRC */
	la      t0,uncompress		/* jump to uncompress */
	jal    	t0			/* never returns - starts up kernel */
	nop
       	li	ra, R_VEC	/* load prom reset address */
	j	ra		/* just in case */

	.align	3
copy_end:

	.end uncompInit

