#include <arch/cputypes.h>
#include <arch/mips/iregdef.h>
#include <arch/mips/idtcpu.h>
#include <board.h>

	.globl  fill128			/* fast memory fill */

	.text

/*******************************************************************************
*
* fill128 - fill (bcount + 127) / 8 dwords from addr, use 16 dwords block
*
* RETURNS: N/A
*
* void fill128 ( unsigned* addr, unsigned bcount, unsigned value )
*
*/
	.ent	fill128
	.set    noreorder

fill128:
	sd	a2,0(a0)
	sd	a2,0x08(a0)
	sd	a2,0x10(a0)
	sd	a2,0x18(a0)
	sd	a2,0x20(a0)
	sd	a2,0x28(a0)
	sd	a2,0x30(a0)
	sd	a2,0x38(a0)
	sd	a2,0x40(a0)
	sd	a2,0x48(a0)
	sd	a2,0x50(a0)
	sd	a2,0x58(a0)
	sd	a2,0x60(a0)
	sd	a2,0x68(a0)
	sd	a2,0x70(a0)
	sd	a2,0x78(a0)
	addiu	a1,a1,-128
	bgtz	a1,fill128
	addiu	a0,a0,128

	j 	ra
	nop

	.set    reorder
	.end	fill128

/*******************************************************************************
*
* boardTLBInit - initialize TLB. Layout:
*         pairs     vaddress    paddress      size     attribute
*         [0:15]   0xc0000000  0x40000000  0x20000000 NONCOHERENT
*         [15:31]  0xe0000000  0x40000000  0x20000000  UNCACHED
*         [32:63]  0x80000000  0x00000000  ----------  INVALID
*
* set WIRED register to 32
*
* RETURNS: N/A
*
* void boardTLBInit(void)
*
*/
	.globl	boardTLBInit
	.ent	boardTLBInit
	.set    noreorder

boardTLBInit:
	li	t0,KSBASE		# virtual base
	li	t1,TLBLO_G|TLBLO_V|TLBLO_D|TLBLO_NONCOHERENT|(0x40000000>>6)
					# physical base + attribute
	li	t2,0x2000000		# 32 MB step
	li	t3,TLBPGMASK_MASK	# 16 MB page mask
	li	t4,0			# current index
	li	t5,16			# index upper bound
	li	t6,(0x1000000 >> 6)	# 16 MB step

0:	
	mtc0	t3,C0_PAGEMASK
	dmtc0	t0,C0_TLBHI
	dmtc0	t1,C0_TLBLO0
	daddu	t1,t6
	dmtc0	t1,C0_TLBLO1
	dmtc0	t4,C0_INX
	nop; nop;			# hazard delay
	c0	C0_WRITEI		# write entry
	addu	t1,t6
	addu	t4,1
	bne	t4,t5,0b
	addu	t0,t2

	li	t1,TLBLO_G|TLBLO_V|TLBLO_D|TLBLO_UNCACHED|(0x40000000>>6)
					# physical base + attribute
	li	t5,32			# index upper bound

0:	
	mtc0	t3,C0_PAGEMASK		
	dmtc0	t0,C0_TLBHI
	dmtc0	t1,C0_TLBLO0
	daddu	t1,t6
	dmtc0	t1,C0_TLBLO1
	dmtc0	t4,C0_INX
	nop; nop;			# hazard delay
	c0	C0_WRITEI		# write entry
	addu	t1,t6
	addu	t4,1
	bne	t4,t5,0b
	addu	t0,t2

	mtc0	t5,C0_WIRED

	li	t0,K0BASE		# virtual base
	li	t1,0			# invalid attribute
	li	t2,0x2000		# 8 KB step
	li	t3,0			# 4 KB page mask
	li	t5,64			# index upper bound

0:
	mtc0	t3,C0_PAGEMASK		
	mtc0	t0,C0_TLBHI
	mtc0	t1,C0_TLBLO0
	mtc0	t1,C0_TLBLO1
	mtc0	t4,C0_INX
	nop; nop;			# hazard delay
	tlbwi				# write entry
	addu	t4,1
	bne	t4,t5,0b
	addu	t0,t2

	jr	ra
	nop

	.set    reorder
	.end	boardTLBInit

/*******************************************************************************
*
* boardTLBInitPCI - initialize TLB. Layout:
*         pairs     vaddress    paddress      size     attribute
*         [0:31]   0xc0000000  0xd20000000  0x40000000 UNCACHED
*         [32:63]  0x80000000  0x000000000  ----------  INVALID
*
* set WIRED register to 32
*
* RETURNS: N/A
*
* void boardTLBInit(void)
*
*/
	.globl	boardTLBInitPCI
	.ent	boardTLBInitPCI
	.set    noreorder

boardTLBInitPCI:
	li	t0,KSBASE		# virtual base
	li	t1,TLBLO_G|TLBLO_V|TLBLO_D|TLBLO_UNCACHED|(0xd20000000>>6)
					# physical base + attribute
	li	t2,0x2000000		# 32 MB step
	li	t3,TLBPGMASK_MASK	# 16 MB page mask
	li	t4,0			# current index
	li	t5,32			# index upper bound
	li	t6,(0x1000000 >> 6)	# 16 MB step

0:	
	mtc0	t3,C0_PAGEMASK
	dmtc0	t0,C0_TLBHI
	dmtc0	t1,C0_TLBLO0
	daddu	t1,t6
	dmtc0	t1,C0_TLBLO1
	dmtc0	t4,C0_INX
	nop; nop;			# hazard delay
	c0	C0_WRITEI		# write entry
	addu	t1,t6
	addu	t4,1
	bne	t4,t5,0b
	addu	t0,t2

	mtc0	t5,C0_WIRED

	li	t0,K0BASE		# virtual base
	li	t1,0			# invalid attribute
	li	t2,0x2000		# 8 KB step
	li	t3,0			# 4 KB page mask
	li	t5,64			# index upper bound

0:
	mtc0	t3,C0_PAGEMASK		
	mtc0	t0,C0_TLBHI
	mtc0	t1,C0_TLBLO0
	mtc0	t1,C0_TLBLO1
	mtc0	t4,C0_INX
	nop; nop;			# hazard delay
	tlbwi				# write entry
	addu	t4,1
	bne	t4,t5,0b
	addu	t0,t2

	jr	ra
	nop

	.set    reorder
	.end	boardTLBInitPCI

	.globl	tlbl
	.ent	tlbl
	.set    reorder

tlbl:
	dmtc0	a0,C0_INX
	c0	C0_READI		# write entry
	nop
	nop
	nop
	nop
	mfc0	t0,C0_PAGEMASK
	nop
	sw	t0,0(a1)
	dmfc0	t0,C0_TLBHI
	nop
	sw	t0,4(a1)
	dsrl32	t0,0
	sw	t0,8(a1)
	dmfc0	t0,C0_TLBLO0
	nop
	sw	t0,12(a1)
	dsrl32	t0,0
	sw	t0,16(a1)
	dmfc0	t0,C0_TLBLO1
	nop
	sw	t0,20(a1)
	dsrl32	t0,0
	sw	t0,24(a1)
	j	ra

	.end	tlbl

	.globl  dbgBreak
	.ent    dbgBreak
	.set    reorder
	
dbgBreak:	
        break
	
	.end    dbgBreak
