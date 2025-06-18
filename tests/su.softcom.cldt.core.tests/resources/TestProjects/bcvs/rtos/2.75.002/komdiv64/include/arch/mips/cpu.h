#ifndef _ARCH_MIPS_CPU_H
#define _ARCH_MIPS_CPU_H

#include <arch/mips/idtcpu.h>
#include <arch/mips/iregdef.h>

#ifndef  __ASSEMBLER__
#if defined KERNEL
#include <sys/types.h>

#define CPU_THREAD_STACK_MIN    8192
#define CPU_STACK_ALIGNMENT     16

#if R_SZ==4
typedef u_long rreg;
#endif
#if R_SZ==8
typedef unsigned long long rreg;
#endif

typedef struct {
    rreg m_s0;
    rreg m_s1;
    rreg m_s2;
    rreg m_s3;
    rreg m_s4;
    rreg m_s5;
    rreg m_s6;
    rreg m_s7;
    rreg m_sp;
    rreg m_fp;
    rreg m_ra;
    rreg m_c0_sr;
} threadREGS;

/* Значение stack pointer потока               */
#define SP(t) t->m_context.m_regs.m_sp
/* Значение instruction pointer потока         */
#define THREAD_IP(t) t->m_context.m_regs.m_ra

struct pt_regs {
        rreg dummy[4];  /* аргументы функции обработки прерываний                 */
        rreg zero;      /* #define    R_ZERO        R_R0 */
        rreg AT;        /* #define    R_AT        R_R1 */
        rreg v0;        /* #define    R_V0        R_R2 */
        rreg v1;        /* #define    R_V1        R_R3 */
        rreg a0;        /* #define    R_A0        R_R4 */
        rreg a1;        /* #define    R_A1        R_R5 */
        rreg a2;        /* #define    R_A2        R_R6 */
        rreg a3;        /* #define    R_A3        R_R7 */
        rreg t0;        /* #define    R_T0        R_R8 */
        rreg t1;        /* #define    R_T1        R_R9 */
        rreg t2;        /* #define    R_T2        R_R10*/
        rreg t3;        /* #define    R_T3        R_R11*/
        rreg t4;        /* #define    R_T4        R_R12*/
        rreg t5;        /* #define    R_T5        R_R13*/
        rreg t6;        /* #define    R_T6        R_R14*/
        rreg t7;        /* #define    R_T7        R_R15*/
        rreg s0;        /* #define    R_S0        R_R16*/
        rreg s1;        /* #define    R_S1        R_R17*/
        rreg s2;        /* #define    R_S2        R_R18*/
        rreg s3;        /* #define    R_S3        R_R19*/
        rreg s4;        /* #define    R_S4        R_R20*/
        rreg s5;        /* #define    R_S5        R_R21*/
        rreg s6;        /* #define    R_S6        R_R22*/
        rreg s7;        /* #define    R_S7        R_R23*/
        rreg t8;        /* #define    R_T8        R_R24*/
        rreg t9;        /* #define    R_T9        R_R25*/
        rreg k0;        /* #define    R_K0        R_R26*/
        rreg k1;        /* #define    R_K1        R_R27*/
        rreg gp;        /* #define    R_GP        R_R28*/
        rreg sp;        /* #define    R_SP        R_R29*/
        rreg fp;        /* #define    R_FP        R_R30*/
        rreg ra;        /* #define    R_RA        R_R31*/
        rreg cp0_epc;
        rreg hi;
        rreg lo;
        rreg cp0_sr;
        rreg cp0_cause;
        rreg cp0_badvaddr;
        rreg ic;
        rreg exctype;
        rreg errno;
};

#define SP(t) t->m_context.m_regs.m_sp

typedef struct {
    rreg m_fpregs[32];
    int  m_fpcsr[2];
} threadREGS_FP;

#define CPU_FPU_CONTEXT_SIZE sizeof(threadREGS_FP)

#define CPU_INIT_THREAD_CONTEXT cpuContextInit

void cpuContextInit(threadREGS *regs, void *stackAddr, u_long stackSize, int fpu);
void cpuContextSwitch( threadREGS *runRegs, threadREGS *heirRegs );

void fpuContextSave( void *regs );
void fpuContextRestore( void *regs );
void fpuContextInit( void *regs );

#endif /* KERNEL */


void __throw(void);
// int ffs(int mask);

int intLock( void );
void intUnlock( int level );


#if CPU == R3000 || CPU == R4000

static __inline__ int intIsLock()
{
    register int a0;
    __asm __volatile ( "mfc0    %0, $12; nop;nop;nop;":"=r" (a0));
    return !(a0 & SR_IEC);
}

#define cpuOR(field,value)          \
    __asm __volatile (              \
            "mfc0    $8, $12\n"     \
            "li      $9, ~0x1\n"    \
            "and     $9, $9,$8\n"   \
            "mtc0    $9, $12\n"     \
            "lw     $9, %0\n"       \
            "ori     $9, %1\n"      \
            "sw     $9, %0\n"       \
            "mtc0    $8, $12\n"     \
            : "=m" (field)          \
            : "i" (value) )

#define cpuINC(field)               \
    __asm __volatile (              \
            "mfc0    $8, $12\n"     \
            "li      $9, ~0x1\n"    \
            "and     $9, $9, $8\n"  \
            "mtc0    $9, $12\n"     \
            "lw         $9, %0\n"   \
            "addiu     $9,1  \n"    \
            "sw         $9, %0\n"   \
            "mtc0   $8, $12\n"      \
            : "=m" (field)          \
            :  )

#if R_SZ == 8
# define cpuSP(field)   \
  __asm __volatile (    \
    "sd $29,%0\n"       \
    : "=m" (field)      \
    : )
#else
# define cpuSP(field)   \
  __asm __volatile (    \
    "sw $29,%0\n"       \
    : "=m" (field)      \
    : )
#endif

/*получить адрес вызова функции throwException
  по адресу возврата, со смещением
*/
static __inline int __UNUSED_F throwExcAddress( void )
{
  int    result;
  __asm __volatile("move %0,$ra; subu %0,8;"
             : "=r" (result) : );
  return (result);
}
/* Информация для загрузки элемента TLB (длина 8 + 4*4 = 24 байта) */
struct cpu_tlb {
   unsigned long long ct_hi; /* регистр EntryHi                        ctHI_OFFSET     */
   unsigned int ct_lo;       /* регистр EntryLo0                       ctLO_OFFSET     */
   unsigned int ct_lo1;      /* регистр EntryLo1                       ctLO1_OFFSET    */
   unsigned int ct_inx;      /* номер элемента TLB                     ctINDEX_OFFSET  */
   unsigned int ct_pgmask;   /* регистр PageMask                       ctPGMASK_OFFSET */
};
int  cpuTlbGet(int ,struct cpu_tlb *);


#endif   /* KERNEL */

#define IEEE_MC68k
#define HEAP_ALLOC_ALIGNMENT R_SZ

#endif /* __ASSEMBLER__ */
#endif /* _ARCH_MIPS_CPU_H */
