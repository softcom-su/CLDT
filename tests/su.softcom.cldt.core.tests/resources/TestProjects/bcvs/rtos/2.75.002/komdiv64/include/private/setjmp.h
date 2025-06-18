#if  CPU == I80386
#define	EBX_     0*4

#define	ESI_     1*4
#define	EDI_     2*4
#define	PC_      3*4
#define	FP_      4*4
#define	SP_      5*4
#define	THREAD_  6*4
#define	SIGNAL_  7*4
#define  EFLAGS_  8*4
#define _JBLEN    9
#define	SAVE_    9*4
#define	SIGMASK_ 10*4
/* format int+pusha */
#define P_EFLAGS  10
#define P_CS      9
#define P_EIP     8
#define P_EAX     7
#define P_ECX     6
#define P_EDX     5
#define P_EBX     4
#define P_ESP     3
#define P_EBP     2
#define P_ESI     1
#define P_EDI     0
#endif

#if  CPU == R3000 || CPU == R4000
#include <arch/mips/iregdef.h>
#define	   PCREG    0*R_SZ
#define	   SPREG    1*R_SZ
#define	   SRREG    2*R_SZ
#define     V0REG    3*R_SZ
#define     LOREG    4*R_SZ
#define     HIREG    5*R_SZ
#define     S0REG    6*R_SZ
#define     S1REG    7*R_SZ
#define     S2REG    8*R_SZ
#define     S3REG    9*R_SZ
#define     S4REG    10*R_SZ
#define     S5REG    11*R_SZ
#define     S6REG    12*R_SZ
#define	   S7REG    13*R_SZ
#define	   S8REG    14*R_SZ

#define	   THREAD_  15*R_SZ
#define	   SIGNAL_  16*R_SZ
#define _JBLEN       17

#define	   SAVE_    17*R_SZ
#define	   SIGMASK_ 18*R_SZ
#endif

#ifndef __ASSEMBLER__
#if  CPU == R3000 
typedef unsigned long jbreg;
#endif
#if  CPU ==  I80386
typedef unsigned long jbreg;
#endif
#if  CPU == R4000
typedef unsigned long long jbreg;
#endif

typedef struct {
   jbreg _jb[_JBLEN];
} jmp_buf[1];

typedef struct { 
   jbreg _jb[_JBLEN+2];
} sigjmp_buf[1];


int   setjmp (jmp_buf envbuf);
void  longjmp (jmp_buf envbuf, int val);

int   sigsetjmp(sigjmp_buf envbuf, int savemask);
void  siglongjmp(sigjmp_buf envbuf, int val);
#endif /* ! __ASSEMBLER__ */
