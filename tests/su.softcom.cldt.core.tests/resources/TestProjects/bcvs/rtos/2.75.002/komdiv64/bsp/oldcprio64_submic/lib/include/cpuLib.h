/* cpuLib.h - mips CPU specific routines */

#ifndef	__INCcpuLibh__
#define	__INCcpuLibh__

#ifdef __ASSEMBLER__ 

#------------------------------------------------------------------------------
## Load Address and Relocate. This macro is used in code that may be
## linked to execute out of RAM but is actually executed from ROM. The
## code that initializes the memory controller and copies the ROM
## contents to RAM must work in this way, for example. This macro is used
## in place of an "la" macro instruction when loading code and data
## addresses.

	.macro	lar	reg,addr
	.set	noat
	move	$at,ra			# save ra
	la	\reg,\addr		# get address into register
	la	ra,x\@			# get linked address of label
	subu	\reg,\reg,ra		# subtract it from value
	bal	x\@			# branch and link to label
	nop				#  to get current actual address
x\@:
	addu	\reg,\reg,ra		# add actual address
	move	ra,$at			# restore ra
	.set	at
	.endm

#ifndef xla
#define	xla	la
#endif

#endif	/* __ASSEMBLER__ */

/* CPU definitions */

#define	IS_KSEG0(x)	((unsigned)(x) >= K0BASE && (unsigned)(x) < K1BASE)
#define	IS_KSEG1(x)	((unsigned)(x) >= K1BASE && (unsigned)(x) < K2BASE)
#define	IS_KUSEG(x)	((unsigned)(x) < K0BASE)

/* number of TLB entries on RM7000 */

#define TLB_ENTRIES	64

/* RM7000 onchip L2 cache size */

#define L2_CACHE_SIZE	0x40000

/* R7000 CP0 set 1 Interrupt Control register */

#define	C0_IC		$20

/* IC bits */

#define	IC_VS		0x001f		/* vector spacing */
#define	IC_TE		0x0080		/* enable alternative timer */
#define	IC_IM_INT6	0x0100		/* Int*6 mask */
#define	IC_IM_INT7	0x0200		/* Int*7 mask */
#define	IC_IM_INT8	0x0400		/* Int*8 mask */
#define	IC_IM_INT9	0x0800		/* Int*9 mask */
#define	IC_IM_TIMER	0x1000		/* alternative timer mask */
#define	IC_IM_PERF	0x2000		/* perfomance counter mask */
#define ICR_IMASK0	0x00003F00	/* interrump mask */

/* start types */

#ifndef	BOOT_NORMAL
#define	BOOT_NORMAL		0	/* start after booter */
#endif

#ifndef	BOOT_COLD
#define	BOOT_COLD		2	/* start without booter */
#endif

#ifndef __ASSEMBLER__ 
void boardCauseSet(unsigned val);
unsigned boardCauseGet(void);
void     boardWiredSet(unsigned val);
unsigned boardWiredGet(void);
void     boardStatusSet(unsigned val);
unsigned boardStatusGet(void);
void boardConfigSet(unsigned val);
unsigned boardConfigGet(void);
unsigned boardConfig1Get(void);
unsigned boardConfig2Get(void);
unsigned boardConfig3Get(void);
void boardCountSet(unsigned val);
unsigned boardCountGet(void);
void boardCompareSet(unsigned val);
unsigned boardCompareGet(void);
unsigned boardPrIDGet(void);
void boardIntControlSet(unsigned val);
unsigned boardIntControlGet(void);
void boardInfoSet(unsigned val);
unsigned boardInfoGet(void);
unsigned boardRandomGet(void);
void cp0CauseSet(unsigned val);
unsigned cp0CauseGet(void);
void cp0WiredSet(unsigned val);
unsigned cp0WiredGet(void);
void cp0StatusSet(unsigned val);
unsigned cp0StatusGet(void);
void cp0ConfigSet(unsigned val);
unsigned cp0ConfigGet(void);
unsigned cp0Config1Get(void);
unsigned cp0Config2Get(void);
unsigned cp0Config3Get(void);
void     cp0CountSet(unsigned val);
unsigned cp0CountGet(void);
void     cp0CompareSet(unsigned val);
unsigned cp0CompareGet(void);
unsigned cp0PrIDGet(void);
void     cp0IntControlSet(unsigned val);
unsigned cp0IntControlGet(void);
void     cp0InfoSet(unsigned val);
unsigned cp0InfoGet(void);
unsigned cp0RandomGet(void);
void     boardWbFlush(void);
void     boardNsDelay(unsigned nCycles);
void     cpuCycleDelay(unsigned nCycles);
unsigned cp1FCSRGet (void);
void     cp1FCSRSet (unsigned FCSR);
#endif /*__ASSEMBLER__*/

#endif	/* __INCcpuLibh__ */
