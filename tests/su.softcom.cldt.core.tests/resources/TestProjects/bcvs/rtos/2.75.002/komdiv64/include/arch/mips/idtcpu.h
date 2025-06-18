/*

Based upon IDT provided code with the following release:

This source code has been made available to you by IDT on an AS-IS
basis. Anyone receiving this source is licensed under IDT copyrights
to use it in any way he or she deems fit, including copying it,
modifying it, compiling it, and redistributing it either with or
without modifications.  No license under IDT patents or patent
applications is to be implied by the copyright license.

Any user of this software should understand that IDT cannot provide
technical support for this software and will not be responsible for
any consequences resulting from the use of this software.

Any person who transfers this source code or any derivative work must
include the IDT copyright notice, this paragraph, and the preceeding
two paragraphs in the transferred software.

COPYRIGHT IDT CORPORATION 1996
LICENSED MATERIAL - PROGRAM PROPERTY OF IDT

  idtcpu.h,v 1.2 1996/09/11 19:16:01 joel Exp
*/

/*
** idtcpu.h -- cpu related defines
*/

#ifndef _IDTCPU_H__
#define _IDTCPU_H__

/*
 * 950313: Ketan added Register definition for XContext reg.
 *               added define for WAIT instruction.
 * 950421: Ketan added Register definition for Config reg (R3081)
 */
#if __LP64__
#define K0BASE  0xffffffff80000000
#define K1BASE  0xffffffffa0000000
#define K2BASE  0xffffffffc0000000
#define K0_TO_PHYS(x)   ((unsigned long )(x)&0x1FFFFFFF)     /* kseg0 to physical        */
#define K1_TO_PHYS(x)   ((unsigned long )(x)&0x1FFFFFFF)     /* kseg1 to physical        */
#define PHYS_TO_K0(x)   ((unsigned long )(x)|K0BASE)         /* physical to kseg0        */
#define K0_TO_K1(x)     ((unsigned long )(x)|K1BASE)         /* kseg0 to kseg1           */
#else
/*
** memory configuration and mapping
*/

#define K0BASE  0x80000000
#define K0SIZE  0x20000000
#define K1BASE  0xa0000000
#define K1SIZE  0x20000000
#define K2BASE  0xc0000000
#define K2SIZE  0x20000000
#if CPU == R4000
#define KSBASE  0xc0000000
#define KSSIZE  0x20000000
#define K3BASE  0xe0000000
#define K3SIZE  0x20000000
#endif

#define KUBASE  0
#define KUSIZE  0x80000000
/*
 * Address conversion macros
 * kseg0 - kernel address space, unmapped, сached
 * kseg1 - kernel address space, unmapped, unсached
*/
#define K0_TO_K1(x)     ((unsigned long )(x)|0xA0000000)         /* kseg0 to kseg1           */
#define K1_TO_K0(x)     ((unsigned long )(x)&0x9FFFFFFF)         /* kseg1 to kseg0           */
#define K0_TO_PHYS(x)   ((unsigned long )(x)&0x1FFFFFFF)         /* kseg0 to physical        */
#define K1_TO_PHYS(x)   ((unsigned long )(x)&0x1FFFFFFF)         /* kseg1 to physical        */
#define PHYS_TO_K0(x)   ((unsigned long )(x)|0x80000000)         /* physical to kseg0        */
#define PHYS_TO_K1(x)   ((unsigned long )(x)|0xA0000000)         /* physical to kseg1        */
#endif
/*
** Exception Vectors
*/
#if CPU == R3000
#define UT_VEC  K0BASE                  /* utlbmiss vector */
#define E_VEC   (K0BASE+0x80)           /* exception vevtor */
#endif
#if CPU == R4000
#define T_VEC   (K0BASE+0x000)          /* tlbmiss vector               */
#define X_VEC   (K0BASE+0x080)          /* xtlbmiss vector              */
#define C_VEC   (K0BASE+0x100)          /* cache error vector           */
#define E_VEC   (K0BASE+0x180)          /* exception vector             */
#define UT_VEC  T_VEC                   /* utlbmiss vector              */
#endif
#define R_VEC   (K1BASE+0x1fc00000)     /* reset vector */

#if     (CPU==R3000)
/* vector numbers of specific MIPS R3000/33000/33020 exceptions */
#define IV_LOW_VEC            0  /* lowest vector initialized (Int - Interrupt)           */
#define IV_TLBMOD_VEC         1  /* Mod  - TLB modification exception - защита записи     */
#define IV_TLBL_VEC           2  /* TLBL - TLB exception (load or instruction fetch)      */
#define IV_TLBS_VEC           3  /* TLBS - TLB exception (store)                          */
#define IV_ADEL_VEC           4  /* AdEL - Address error exception (load or instr. fetch) */
#define IV_ADES_VEC           5  /* AdES - Address error exception (store)                */
#define IV_IBUS_VEC           6  /* IBE  - Bus error exception (instruction fetch)        */
#define IV_DBUS_VEC           7  /* DBE  - Bus error exception (data ref.: load or store) */
#define IV_SYSCALL_VEC        8  /* Sys  - Syscall exception                              */
#define IV_BP_VEC             9  /* Bp   - Breakpoint exception                           */
#define IV_RESVDINST_VEC     10  /* RI   - Reserved instruction exception                 */
#define IV_CPU_VEC           11  /* CpU  - Coprocessor Unusable exception                 */
#define IV_OVF_VEC           12  /* Ov   - Arithmetic Overflow exception                  */
#define IV_RESVD2_VEC        13  /* reserved entry                                        */
#define IV_RESVD3_VEC        14  /* reserved entry                                        */
#define IV_RESVD4_VEC        15  /* reserved entry                                        */
#define USER_VEC_START       16  /* start of user interrupt vectors                       */
#define LOW_VEC              16  /* lowest autovector initialized                         */
#define IV_FPA_UNIMP_VEC     38      /* unimplemented FPA oper       */
#define IV_FPA_INV_VEC       39      /* invalid FPA operation        */
#define IV_FPA_DIV0_VEC      40      /* FPA div by zero              */
#define IV_FPA_OVF_VEC       41      /* FPA overflow exception       */
#define IV_FPA_UFL_VEC       42      /* FPA underflow exception      */
#define IV_FPA_PREC_VEC      43      /* FPA inexact operation        */
#define IV_BUS_ERROR_VEC     44      /* bus error vector             */
#define USER_VEC_START       16      /* start of user interrupt vectors */
#define HIGH_VEC             255     /* highest  autovector initialized */
#define IV_FPA_BASE_VEC      IV_FPA_UNIMP_VEC
#endif

#if     (CPU==R4000)
/* vector numbers of specific MIPS R4000 exceptions */
#define IV_LOW_VEC         0  /* lowest vector initialized (Int - Interrupt)           */
#define IV_TLBMOD_VEC      1  /* Mod  - TLB modification exception - защита записи     */
#define IV_TLBL_VEC        2  /* TLBL - TLB exception (load or instruction fetch)      */
#define IV_TLBS_VEC        3  /* TLBS - TLB exception (store)                          */
#define IV_ADEL_VEC        4  /* AdEL - Address error exception (load or instr. fetch) */
#define IV_ADES_VEC        5  /* AdES - Address error exception (store)                */
#define IV_IBUS_VEC        6  /* IBE  - Bus error exception (instruction fetch)        */
#define IV_DBUS_VEC        7  /* DBE  - Bus error exception (data ref.: load or store) */
#define IV_SYSCALL_VEC     8  /* Sys  - Syscall exception                              */
#define IV_BP_VEC          9  /* Bp   - Breakpoint exception                           */
#define IV_RESVDINST_VEC  10  /* RI   - Reserved instruction exception                 */
#define IV_CPU_VEC        11  /* CpU  - Coprocessor Unusable exception                 */
#define IV_OVF_VEC        12  /* Ov   - Arithmetic Overflow exception                  */
#define IV_TRAP_VEC       13  /* Tr   - Trap exception                                 */
#define IV_VCEI_VEC       14  /* virtual coherency inst. vec                           */
#define IV_FPE_VEC        15  /* FPE  - Floating-Point exception                       */

#define IV_IWE_VEC        16  /* IWE -  Instruction Watch Exception                    */

#define IV_RESVD17_VEC    17  /* reserved entry                                        */
#define IV_RESVD18_VEC    18  /* reserved entry                                        */
#define IV_RESVD19_VEC    19  /* reserved entry                                        */
#define IV_RESVD20_VEC    20  /* reserved entry                                        */
#define IV_RESVD21_VEC    21  /* reserved entry                                        */
#define IV_RESVD22_VEC    22  /* reserved entry                                        */

#define IV_WATCH_VEC      23  /* DWE  - Data Watch Exception                           */
#define IV_RESVD24_VEC    24  /* reserved entry                                        */
#define IV_RESVD25_VEC    25  /* reserved entry                                        */
#define IV_RESVD26_VEC    26  /* reserved entry                                        */
#define IV_RESVD27_VEC    27  /* reserved entry                                        */
#define IV_RESVD28_VEC    28  /* reserved entry                                        */
#define IV_RESVD29_VEC    29  /* reserved entry                                        */
#define IV_CACHE_ERROR_VEC  30
#define IV_VCED_VEC       31  /* virtual coherency data vec                            */

#define USER_VEC_START    32  /* start of user interrupt vectors                       */
#define LOW_VEC           32  /* lowest autovector initialized                         */
#define HIGH_VEC         255  /* highest  autovector initialized                       */
#endif


/*
**      Cache size constants
*/
#define MINCACHE        0x200           /* 512       For 3041. */
#define MAXCACHE        0x40000         /* 256*1024   256k */

#if CPU == R4000
/* R4000 configuration register definitions */
#define CFG_CM          0x80000000      /* Master-Checker mode */
#define CFG_ECMASK      0x70000000      /* System Clock Ratio */
#define CFG_ECBY2       0x00000000      /* divide by 2 */
#define CFG_ECBY3       0x10000000      /* divide by 3 */
#define CFG_ECBY4       0x20000000      /* divide by 4 */
#define CFG_EPMASK      0x0f000000      /* Transmit data pattern */
#define CFG_EPD         0x00000000      /* D */
#define CFG_EPDDX       0x01000000      /* DDX */
#define CFG_EPDDXX      0x02000000      /* DDXX */
#define CFG_EPDXDX      0x03000000      /* DXDX */
#define CFG_EPDDXXX     0x04000000      /* DDXXX */
#define CFG_EPDDXXXX    0x05000000      /* DDXXXX */
#define CFG_EPDXXDXX    0x06000000      /* DXXDXX */
#define CFG_EPDDXXXXX   0x07000000      /* DDXXXXX */
#define CFG_EPDXXXDXXX  0x08000000      /* DXXXDXXX */
#define CFG_SBMASK      0x00c00000      /* Secondary cache block size */
#define CFG_SBSHIFT     22
#define CFG_SB4         0x00000000      /* 4 words */
#define CFG_SB8         0x00400000      /* 8 words */
#define CFG_SB16        0x00800000      /* 16 words */
#define CFG_SB32        0x00c00000      /* 32 words */
#define CFG_SS          0x00200000      /* Split secondary cache */
#define CFG_SW          0x00100000      /* Secondary cache port width */
#define CFG_EWMASK      0x000c0000      /* System port width */
#define CFG_EWSHIFT     18
#define CFG_EW64        0x00000000      /* 64 bit */
#define CFG_EW32        0x00010000      /* 32 bit */
#define CFG_SC          0x00020000      /* Secondary cache absent */
#define CFG_SM          0x00010000      /* Dirty Shared mode disabled */
#define CFG_BE          0x00008000      /* Big Endian */
#define CFG_EM          0x00004000      /* ECC mode enable */
#define CFG_EB          0x00002000      /* Block ordering */
#define CFG_ICMASK      0x00000e00      /* Instruction cache size */
#define CFG_ICSHIFT     9
#define CFG_DCMASK      0x000001c0      /* Data cache size */
#define CFG_DCSHIFT     6
#define CFG_IB          0x00000020      /* Instruction cache block size */
#define CFG_DB          0x00000010      /* Data cache block size */
#define CFG_CU          0x00000008      /* Update on Store Conditional */
#define CFG_K0MASK      0x00000007      /* KSEG0 coherency algorithm */

/*
 * R4000 primary cache mode
 */
#define CFG_C_UNCACHED          2
#define CFG_C_NONCOHERENT       3
#define CFG_C_COHERENTXCL       4
#define CFG_C_COHERENTXCLW      5
#define CFG_C_COHERENTUPD       6

/*
 * R4000 cache operations (should be in assembler...?)
 */
#define Index_Invalidate_I               0x0         /* 0       0 */
#define Index_Writeback_Inv_D            0x1         /* 0       1 */
#define Index_Invalidate_SI              0x2         /* 0       2 */
#define Index_Writeback_Inv_SD           0x3         /* 0       3 */
#define Index_Load_Tag_I                 0x4         /* 1       0 */
#define Index_Load_Tag_D                 0x5         /* 1       1 */
#define Index_Load_Tag_SI                0x6         /* 1       2 */
#define Index_Load_Tag_SD                0x7         /* 1       3 */
#define Index_Store_Tag_I                0x8         /* 2       0 */
#define Index_Store_Tag_D                0x9         /* 2       1 */
#define Index_Store_Tag_SI               0xA         /* 2       2 */
#define Index_Store_Tag_SD               0xB         /* 2       3 */
#define Create_Dirty_Exc_D               0xD         /* 3       1 */
#define Create_Dirty_Exc_SD              0xF         /* 3       3 */
#define Hit_Invalidate_I                 0x10        /* 4       0 */
#define Hit_Invalidate_D                 0x11        /* 4       1 */
#define Hit_Invalidate_SI                0x12        /* 4       2 */
#define Hit_Invalidate_SD                0x13        /* 4       3 */
#define Hit_Writeback_Inv_D              0x15        /* 5       1 */
#define Hit_Writeback_Inv_SD             0x17        /* 5       3 */
#define Fill_I                           0x14        /* 5       0 */
#define Hit_Writeback_D                  0x19        /* 6       1 */
#define Hit_Writeback_SD                 0x1B        /* 6       3 */
#define Hit_Writeback_I                  0x18        /* 6       0 */
#define Hit_Set_Virtual_SI               0x1E        /* 7       2 */
#define Hit_Set_Virtual_SD               0x1F        /* 7       3 */


#if !defined wait && defined __ASSEMBLER__
#define wait .word 0x42000020
#endif /* wait */

#endif

/*
** TLB resource defines
*/
#if CPU == R3000
#define N_TLB_ENTRIES        64
#define TLB_PGSIZE           0x1000
#define RANDBASE             8
#define TLBLO_PFNMASK        0xfffff000
#define TLBLO_PFNSHIFT       12               /* позиция PFN (the page frame number)   */
                                              /* в регистре EntryLo                    */
#define TLBLO_N              0x800            /* non-cacheable     */
#define TLBLO_D              0x400            /* D - Dirty  (writeable)                 */
#define TLBLO_V              0x200            /* V - Valid  (valid bit)                 */
#define TLBLO_G              0x100            /* G - Global (global access bit)         */

#define TLBHI_VPNMASK        0xfffff000
#define TLBHI_VPNSHIFT       12
#define TLBHI_PIDMASK        0xfc0
#define TLBHI_PIDSHIFT       6
#define TLBHI_NPID           64

#define TLBINX_PROBE         0x80000000
#define TLBINX_INXMASK       0x00003f00
#define TLBINX_INXSHIFT      8

#define TLBRAND_RANDMASK     0x00003f00
#define TLBRAND_RANDSHIFT    8

#define TLBCTXT_BASEMASK     0xffe00000
#define TLBCTXT_BASESHIFT    21

#define TLBCTXT_VPNMASK      0x001ffffc
#define TLBCTXT_VPNSHIFT     2
#endif
#if CPU == R4000
#define N_TLB_ENTRIES        48

#define TLBHI_VPN2MASK       0xffffe000
#define TLBHI_PIDMASK        0x000000ff
#define TLBHI_NPID           256

#define TLBLO_PFNMASK        0x3fffffc0LL
#define TLBLO_PFNSHIFT       6              /* позиция PFN (the page frame number)     */
                                            /* в регистре EntryLo                      */
#define TLBLO_D              0x00000004     /* D - Dirty  (writeable)                 */
#define TLBLO_V              0x00000002     /* V - Valid  (valid bit)                 */
#define TLBLO_G              0x00000001     /* G - Global (global access bit)          */
#define TLBLO_CMASK          0x00000038     /* cache algorithm mask */
#define TLBLO_CSHIFT         3

#define TLBLO_UNCACHED       (CFG_C_UNCACHED<<TLBLO_CSHIFT)
#define TLBLO_NONCOHERENT    (CFG_C_NONCOHERENT<<TLBLO_CSHIFT)
#define TLBLO_COHERENTXCL    (CFG_C_COHERENTXCL<<TLBLO_CSHIFT)
#define TLBLO_COHERENTXCLW   (CFG_C_COHERENTXCLW<<TLBLO_CSHIFT)
#define TLBLO_COHERENTUPD    (CFG_C_COHERENTUPD<<TLBLO_CSHIFT)

#define TLBINX_PROBE         0x80000000
#define TLBINX_INXMASK       0x0000003f

#define TLBRAND_RANDMASK     0x0000003f

#define TLBCTXT_BASEMASK     0xff800000
#define TLBCTXT_BASESHIFT    23

#define TLBCTXT_VPN2MASK     0x007ffff0
#define TLBCTXT_VPN2SHIFT    4

#define TLBPGMASK_MASK       0x01ffe000
#endif

#if CPU == R3000
#define SR_CUMASK       0xf0000000      /* coproc usable bits */
#define SR_CU3          0x80000000      /* Coprocessor 3 usable */
#define SR_CU2          0x40000000      /* Coprocessor 2 usable */
#define SR_CU1          0x20000000      /* Coprocessor 1 usable */
#define SR_CU0          0x10000000      /* Coprocessor 0 usable */

#define SR_BEV          0x00400000      /* use boot exception vectors */

/* Cache control bits */
#define SR_TS           0x00200000      /* TLB shutdown */
#define SR_PE           0x00100000      /* cache parity error */
#define SR_CM           0x00080000      /* cache miss */
#define SR_PZ           0x00040000      /* cache parity zero */
#define SR_SWC          0x00020000      /* swap cache */
#define SR_ISC          0x00010000      /* Isolate data cache */

/*
**      status register interrupt masks and bits
*/

#define SR_IMASK        0x0000ff00      /* Interrupt mask */
#define SR_IMASK8       0x00000000      /* mask level 8 */
#define SR_IMASK7       0x00008000      /* mask level 7 */
#define SR_IMASK6       0x0000c000      /* mask level 6 */
#define SR_IMASK5       0x0000e000      /* mask level 5 */
#define SR_IMASK4       0x0000f000      /* mask level 4 */
#define SR_IMASK3       0x0000f800      /* mask level 3 */
#define SR_IMASK2       0x0000fc00      /* mask level 2 */
#define SR_IMASK1       0x0000fe00      /* mask level 1 */
#define SR_IMASK0       0x0000ff00      /* mask level 0 */

#define SR_IMASKSHIFT   8

#define SR_IBIT8        0x00008000      /* bit level 8 */
#define SR_IBIT7        0x00004000      /* bit level 7 */
#define SR_IBIT6        0x00002000      /* bit level 6 */
#define SR_IBIT5        0x00001000      /* bit level 5 */
#define SR_IBIT4        0x00000800      /* bit level 4 */
#define SR_IBIT3        0x00000400      /* bit level 3 */
#define SR_IBIT2        0x00000200      /* bit level 2 */
#define SR_IBIT1        0x00000100      /* bit level 1 */

#define SR_KUO          0x00000020      /* old kernel/user, 0 => k, 1 => u    */
#define SR_IEO          0x00000010      /* old interrupt enable, 1 => enable  */
#define SR_KUP          0x00000008      /* prev kernel/user, 0 => k, 1 => u   */
#define SR_IEP          0x00000004      /* prev interrupt enable, 1 => enable */
#define SR_KUC          0x00000002      /* cur kernel/user, 0 => k, 1 => u    */
#define SR_IEC          0x00000001      /* cur interrupt enable, 1 => enable  */
#define SR_KUMSK    (SR_KUO|SR_IEO|SR_KUP|SR_IEP|SR_KUC|SR_IEC)
#define CAUSE_EXCMASK   0x0000003C      /* Cause code bits */
#endif  /* CPU == R3000 */

#if CPU == R4000
#define SR_CUMASK       0xf0000000      /* coproc usable bits */
#define SR_CU3          0x80000000      /* Coprocessor 3 usable */
#define SR_CU2          0x40000000      /* Coprocessor 2 usable */
#define SR_CU1          0x20000000      /* Coprocessor 1 usable */
#define SR_CU0          0x10000000      /* Coprocessor 0 usable */

#define SR_RP           0x08000000      /* Reduced power operation */
#define SR_FR           0x04000000      /* Additional floating point registers */
#define SR_RE           0x02000000      /* Reverse endian in user mode */

#define SR_MX           0x01000000      /* Enables access to MDMX      */
#ifdef RM7000
#define SR_PX           0x80000000      /* Enables access to 64-bit operations in User mode */
#else
#define SR_PX           0x00800000      /* Enables access to 64-bit operations in User mode */
#endif
#define SR_BEV          0x00400000      /* Use boot exception vectors */
#define SR_TS           0x00200000      /* TLB shutdown */
#define SR_SR           0x00100000      /* Soft reset */
#define SR_CH           0x00040000      /* Cache hit */
#define SR_CE           0x00020000      /* Use cache ECC  */
#define SR_DE           0x00010000      /* Disable cache exceptions */

/*
 *      status register interrupt masks and bits
 */

#define SR_IMASK        0x0000ff00     /* Interrupt mask */
#define SR_IMASK8       0x00000000     /* mask level 8 */
#define SR_IMASK7       0x00008000     /* mask level 7 */
#define SR_IMASK6       0x0000c000     /* mask level 6 */
#define SR_IMASK5       0x0000e000     /* mask level 5 */
#define SR_IMASK4       0x0000f000     /* mask level 4 */
#define SR_IMASK3       0x0000f800     /* mask level 3 */
#define SR_IMASK2       0x0000fc00     /* mask level 2 */
#define SR_IMASK1       0x0000fe00     /* mask level 1 */
#define SR_IMASK0       0x0000ff00     /* mask level 0 */

#define SR_IMASKSHIFT   8

#define SR_IBIT8        0x00008000     /* bit level 8 */
#define SR_IBIT7        0x00004000     /* bit level 7 */
#define SR_IBIT6        0x00002000     /* bit level 6 */
#define SR_IBIT5        0x00001000     /* bit level 5 */
#define SR_IBIT4        0x00000800     /* bit level 4 */
#define SR_IBIT3        0x00000400     /* bit level 3 */
#define SR_IBIT2        0x00000200     /* bit level 2 */
#define SR_IBIT1        0x00000100     /* bit level 1 */

#define SR_KX           0x00000080     /* 64-bit addressing in Kernel mode             */
#define SR_SX           0x00000040     /* 64-bit addressing in Supervisor mode         */
#define SR_UX           0x00000020     /* 64-bit addressing in User mode               */

#define SR_KSMASK       0x00000018     /* Kernel mode mask                             */
#define SR_KSUSER       0x00000010     /* User mode                                    */
#define SR_KSSUPER      0x00000008     /* Supervisor mode                              */
#define SR_KSKERNEL     0x00000000     /* Kernel mode                                  */
#define SR_ERL          0x00000004     /* Error level                                  */
#define SR_EXL          0x00000002     /* Exception level                              */
#define SR_IEC          0x00000001     /* cur interrupt enable, 1 => enable            */
#define CAUSE_EXCMASK   0x0000007C     /* Cause code bits                              */
#endif /* CPU == R4000 */

/*
 * fpa definitions
 */

#define FP_ROUND        0x3             /* r3010 round mode mask */
#define FP_STICKY       0x7c            /* r3010 sticky bits mask */
#define FP_ENABLE       0xf80           /* r3010 enable mode mask */
#define FP_EXC          0x3f000         /* r3010 exception mask */

/* Биты Control/Status Register сопроцессора плавающей арифметики  */

/* Биты 1-0 режимы округления        */
#define FP_ROUND_N      0x0            /* round to nearest                             */
#define FP_ROUND_Z      0x1            /* round to zero                                */
#define FP_ROUND_P      0x2            /* round to + infinity                          */
#define FP_ROUND_M      0x3            /* round to - infinity                          */
/*
 * Биты 6-2 флаги исключительных ситуаций (Flags)
 * Эти флаги аналог sticky bits, предусмотренных стандартом IEEE 754.
 * Флаги исключительных ситуаций не устанавливаются и не сбрасываются аппаратурой.
 * Они должны устанавливаться программным обеспечением при возникновении
 * исключительной ситуации. Сбрасываются по явному запросу записью
 * в Control/Status регистр командой ctc1.
 */
#define FP_STICKY_I     0x04           /* sticky inexact operation                     */
#define FP_STICKY_U     0x08           /* sticky underflow                             */
#define FP_STICKY_O     0x10           /* sticky overflow                              */
#define FP_STICKY_Z     0x20           /* sticky divide by zero                        */
#define FP_STICKY_V     0x40           /* sticky invalid operation                     */

/*
 * Биты 11-7 биты разрешения исключительных ситуаций (Enable bits).
 * Исключительная ситуации при выполнении операции с плавающей точкой
 * происходит всякий раз когда установлен бит причины (cause) и
 * соответствующий бит разрешения.
 *
 * Для нереализованных операций (E - Unimplemented Operation ) бит разрешения отсутствует
 * и исключительная ситуация возникает всякий раз,
 * когда установлен соответствующий бит причины.
 * При обработке исключительной ситуации необходмо снимать бит причины
 * (если установлен соответствующий бит разрешения),
 * иначе исключительная ситуация возникнет вновь.
 */
#define FP_ENABLE_I     0x0080         /* enable inexact operation                     */
#define FP_ENABLE_U     0x0100         /* enable underflow exc                         */
#define FP_ENABLE_O     0x0200         /* enable overflow exc                          */
#define FP_ENABLE_Z     0x0400         /* enable divide by zero exc                    */
#define FP_ENABLE_V     0x0800         /* enable invalid operation exc                 */
/*
 * Биты 11-7 биты причины исключительных ситуаций (Cause Bits).
 * Эти биты отражают результат выполнения последней операции с плавающей точкой и
 * и указывают возникшие исключительные ситуации.
 * Если произошла исключительная ситуация то результат операции никуда не записываетяс
 */
#define FP_EXC_I        0x001000       /* inexact operation                            */
#define FP_EXC_U        0x002000       /* underflow                                    */
#define FP_EXC_O        0x004000       /* overflow                                     */
#define FP_EXC_Z        0x008000       /* divide by zero                               */
#define FP_EXC_V        0x010000       /* invalid operation                            */
#define FP_EXC_E        0x020000       /* unimplemented operation                      */

#define FP_COND         0x800000       /* condition bit */
#define FP_FS           0x1000000      /* fs bit        */

#define FP_EXC_SHIFT    12
#define FP_ENABLE_SHIFT 7

/* Маска (всех) битов причины исключительных ситуаций (Cause Bits) */
#define FP_EXC_MASK     (FP_EXC_I|FP_EXC_U|FP_EXC_O|FP_EXC_Z|FP_EXC_V|FP_EXC_E)

/* Маска (всех) битов разрешения исключительных ситуаций (Enable bits) */
#define FP_ENABLE_MASK  (FP_ENABLE_I|FP_ENABLE_U|FP_ENABLE_O|FP_ENABLE_Z| \
                         FP_ENABLE_V)
#define FP_TASK_STATUS  0x0             /* up to user to enable exceptions
                                           for signals see fppALib.s */

/*
 * Cause Register
 */
#define CAUSE_BD        0x80000000      /* Branch delay slot */
#define CAUSE_CEMASK    0x30000000      /* coprocessor error */
#define CAUSE_CESHIFT   28

#define CAUSE_IP8       0x00008000      /* External level 8 pending */
#define CAUSE_IP7       0x00004000      /* External level 7 pending */
#define CAUSE_IP6       0x00002000      /* External level 6 pending */
#define CAUSE_IP5       0x00001000      /* External level 5 pending */
#define CAUSE_IP4       0x00000800      /* External level 4 pending */
#define CAUSE_IP3       0x00000400      /* External level 3 pending */
#define CAUSE_SW2       0x00000200      /* Software level 2 pending */
#define CAUSE_SW1       0x00000100      /* Software level 1 pending */

#define CAUSE_IPMASK    0x00FFFF00      /* Pending interrupt mask */
#define CAUSE_IPSHIFT   8

#define CAUSE_EXCSHIFT  2

#ifndef XDS
/*
**  Coprocessor 0 registers
*/
#define C0_INX          $0              /* tlb index */
#define C0_RAND         $1              /* tlb random */
#if CPU == R3000
#define C0_TLBLO        $2              /* tlb entry low */
#endif
#if CPU == R4000
#define C0_TLBLO0       $2              /* tlb entry low 0 */
#define C0_TLBLO1       $3              /* tlb entry low 1 */
#endif

#define C0_CTXT         $4              /* tlb context */

#if CPU == R4000
#define C0_PAGEMASK     $5              /* tlb page mask */
#define C0_WIRED        $6              /* number of wired tlb entries */
#endif

#define C0_BADVADDR     $8              /* bad virtual address */

#if CPU == R4000
#define C0_COUNT        $9              /* cycle count */
#endif

#define C0_TLBHI        $10             /* tlb entry hi */

#if CPU == R4000
#define C0_COMPARE      $11             /* cyccle count comparator  */
#endif

#define C0_SR           $12             /* status register */
#define C0_CAUSE        $13             /* exception cause */
#define C0_EPC          $14             /* exception pc */
#define C0_PRID         $15             /* revision identifier */

#if CPU == R3000
#define C0_CONFIG   $3     /* configuration register R3081*/
#endif

#if CPU == R4000
#define C0_CONFIG       $16             /* configuration register */
#define C0_LLADDR       $17             /* linked load address */
#define C0_WATCHLO      $18             /* watchpoint trap register */
#define C0_WATCHHI      $19             /* watchpoint trap register */
#define C0_XCTXT        $20             /* extended tlb context */
#define C0_PERFCTRL     $22
#define C0_DEBUG        $23
#define C0_DEPC         $24
#define C0_PERFCNTR     $25
#define C0_ECC          $26             /* secondary cache ECC control */
#define C0_CACHEERR     $27             /* cache error status */
#define C0_TAGLO        $28             /* cache tag lo */
#define C0_TAGHI        $29             /* cache tag hi */
#define C0_ERRPC        $30             /* cache error pc */
#define C0_DESAVE       $31
#endif

#endif /* XDS */

/*
* Coprocessor 0 operations
*/

#define C0_READI  0x1           /* read ITLB entry addressed by C0_INDEX */
#define C0_WRITEI 0x2           /* write ITLB entry addressed by C0_INDEX */
#define C0_WRITER 0x6           /* write ITLB entry addressed by C0_RAND */
#define C0_PROBE  0x8           /* probe for ITLB entry addressed by TLBHI */
#define C0_RFE    0x10          /* restore for exception */


#ifdef R4650
#define IWATCH $18
#define DWATCH $19
#define IBASE  $0
#define IBOUND $1
#define DBASE  $2
#define DBOUND $3
#define CALG    $17
#endif

#if defined( XLP )
#define C0_EIMR   $9,7
#define C0_EIRR   $9,6
#define C0_EBASE  $15,1
#endif

#endif /* _IDTCPU_H__ */

