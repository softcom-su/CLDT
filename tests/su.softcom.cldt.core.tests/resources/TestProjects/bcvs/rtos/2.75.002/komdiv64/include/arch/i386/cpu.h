#ifndef _ARCH_I386_CPU_H
#define _ARCH_I386_CPU_H

#include <sys/types.h>
#include <sys/cdefs.h>

#define CPU_THREAD_STACK_MIN    16384
#define CPU_STACK_ALIGNMENT     4

#ifdef KERNEL

typedef u_long rreg;

typedef struct {
  u_long      m_eflags;
  void       *m_esp;
  void       *m_ebp;
  u_long   m_ebx;
  u_long   m_esi;
  u_long      m_edi;
} threadREGS;
/*
 * This struct defines the way the registers are stored on the stack during a
 * system call/exception.
 */
struct pt_regs
{
   unsigned p_esp0;
   unsigned p_edi;
   unsigned p_esi;
   unsigned p_ebp;
   unsigned p_esp;
   unsigned p_ebx;
   unsigned p_edx;
   unsigned p_ecx;
   unsigned p_eax;
   unsigned p_eip;
   unsigned p_cs;
   unsigned p_eflags;
};

#define IV_DIVBYZERO_VEC   0  //                Ошибка деления
#define IV_TRAP_VEC        1  //                Отладочное исключение
#define IV_NMI_VEC         2  //                Немаскируемое (NMI) прерывание
#define IV_BP_VEC          3  //                Точка останова (контрольная точка)
#define IV_OVERFLOW_VEC    4  //                Переполнение, обнаруженное командой INTO
#define IV_BOUND_VEC       5  //                Превышение диапазона команды BOUND
#define IV_RESVDINST_VEC   6  //                Неверный код операции
#define IV_IO_VEC          7  //                Устройство не доступно
#define IV_DBLFAULTT_VEC   8  //                Двойной сбой
#define IV_RESVD_VEC       9  //                (Резервировано Intel. Не используйте.
                              //                ЦПУ i486 не используется)
#define IV_SEGILL_VEC      10 //                 Неверный сегмент состояния задачи
#define IV_SEGNOT_VEC      11 //                 Сегмент не присутствует
#define IV_STACKILL_VEC    12 //                 Сбой в стеке
#define IV_PROTECT_VEC     13 //                 Общая защита
#define IV_PAGE_VEC        14 //                 Сбой в странице
#define IV_RESVD2_VEC      15 //                 (Резервировано Intel. Не используйте)
#define IV_FPE_VEC         16 //                 Ошибка операции с плавающей точкой
#define IV_ALIGN_VEC       17 //                 Проверка выравнивания
//  18-31                (Резервировано Intel. Не используйте)
// 32-255                Маскируемые прерывания


/* Значение stack pointer потока               */
#define SP(t) t->m_context.m_regs.m_esp
/* Значение instruction pointer потока         */
#define THREAD_IP(t) ((u_long *)t->m_context.m_regs.m_esp)[0]

/*
 *  FP context save area for the i387 numeric coprocessors.
 */

typedef struct {
  unsigned cw;
  unsigned sw;
  unsigned tw;
  unsigned ip;
  unsigned cs;
  unsigned op_off;
  unsigned op_sel;
  struct
  {
   u_char d[10];
  } regs[8];
} threadREGS_FP;


typedef struct {
  unsigned short offset_0_15;
  unsigned short segment_selector;
  unsigned char  reserved;
  unsigned char  p_dpl;
  unsigned short offset_16_31;
}   cpuIdtItem;

#define R_SZ   4

#define CPU_FPU_CONTEXT_SIZE sizeof(threadREGS_FP)

#define CPU_EFLAGS_DEFAULT  0x00003202

#define CPU_INIT_THREAD_CONTEXT( pContext, stackAdd, stackSize, fpu ) \
  do { \
    u_long pStack; \
    \
    (pContext)->m_eflags = CPU_EFLAGS_DEFAULT; \
    pStack = ((u_long)(stackAdd)) + (stackSize) - 4; \
    *((void **)pStack) = (kernThreadStub); \
    (pContext)->m_esp     =  (void *) pStack; \
    (pContext)->m_ebp     =  (void *) pStack; \
  } while (0)

void cpuContextSwitch( threadREGS *runRegs, threadREGS *heirRegs );
void fpuContextInit( void *regs );

#ifdef __GNUC__
#define _ARCH_FFS
static __inline int __UNUSED_F ffs(int mask)
{
   int   result;
   __asm __volatile("testl %0,%0; je 1f; bsfl %0,%0; incl %0; 1:"
      : "=r" (result) : "0" (mask));
   return (result);
}
#define cpuOR(field,value)\
   __asm __volatile ("or %0,%1": : "i" (value),"m" (field) )

#define cpuINC(field)\
   __asm __volatile ("inc %0": : "m" (field) )

#define cpuSP(field)\
   __asm __volatile ("movl %%esp,%0": : "m" (field) )

#define CHECKSTACK {\
   uint _sp,sa=(int)threadExecuting->m_stackAddr;\
   __asm __volatile ( "push %%esp ; pop %0"\
   : "=r" ((_sp)) : "0" ((_sp)) );\
   if((_sp<=sa) ||( _sp >=(sa+threadExecuting->m_stackSize))) {\
   kprint("Stack error ====================\n");\
   _exit(0);\
   }\
}
#endif /* __GNUC__ */
#endif /* KERNEL */
#define K0_TO_PHYS(a) (u_int)a

void fpuContextSave( void *regs );
void fpuContextRestore( void *regs );

void __throw(void);
#define EFLAGS_IF 0x200         /* прерывания разрешены */

static __inline__ int intIsLock()
{
    int level = 0;
    __asm __volatile ( "pushf ; popl %0"
                    : "=r" ((level)) : "0" ((level)) );
    return (level & EFLAGS_IF) == 0 ;
}

static __inline__ int __UNUSED_F intLock( void )
{
    int level = 0;
    __asm __volatile ( "pushf ; cli ; pop %0"
                    : "=r" ((level)) : "0" ((level)) );
    return level;
}
static __inline__ void __UNUSED_F intUnlock( int level )
{
    __asm __volatile ( "push %0 ; popf"
                    : "=r" ((level)) : "0" ((level)) );
}
/* chain IN operation */
static __inline__ void __UNUSED_F cpuInsb(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; insb"
          : "=D" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt)
          );
}
static __inline__ void  __UNUSED_F cpuInsw(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; insw"
          : "=D" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt)
          );
}
static __inline__ void  __UNUSED_F cpuInsl(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; insl"
          : "=D" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt)
          );
}

/* chain OUT operation */
static __inline__ void __UNUSED_F cpuOutsb(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; outsb"
          : "=S" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt) );
}
static __inline__ void __UNUSED_F cpuOutsw(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; outsw"
          : "=S" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt) );
}
static __inline__ void __UNUSED_F cpuOutsl(u_int port, void *addr, size_t cnt)
{
   __asm __volatile("cld; rep; outsl"
          : "=S" (addr), "=c" (cnt) : "d" (port), "0" (addr), "1" (cnt) );
}


static __inline__ u_char __UNUSED_F cpuInByte(u_int port)
{
   u_char   data;
   __asm __volatile("inb %%dx,%0" : "=a" (data) : "d" ((u_short)(port)));
   return (data);
}

static __inline__ void __UNUSED_F cpuOutByte(u_int port, u_char data)
{
   __asm __volatile("outb %0,%%dx" : : "a" (data), "d" (port));
}
static __inline__ u_short __UNUSED_F cpuInWord(u_int port)
{
   u_short  data;

   __asm __volatile("inw %%dx,%0" : "=a" (data) : "d" (port));
   return (data);
}

static __inline__ void __UNUSED_F cpuOutWord(u_int port, u_short data)
{
   __asm __volatile("outw %0,%%dx" : : "a" (data), "d" (port));
}

static __inline__ u_long __UNUSED_F cpuInLong(u_int port)
{
   u_long   data;

   __asm __volatile("inl %%dx,%0" : "=a" (data) : "d" (port));
   return (data);
}

static __inline__ void __UNUSED_F cpuOutLong(u_int port, u_long data)
{
   __asm __volatile("outl %0,%%dx" : : "a" (data), "d" (port));
}

/*получить адрес вызова функции throwException
  по адресу возврата, сосмещением
*/
static __inline int __UNUSED_F throwExcAddress( void )
{
  int result;
  __asm __volatile("movl 4(%%ebp),%0; subl $5,%0;"
          : "=r" (result) : );
  return (result);
}

u_char  inb(u_int port);
u_short inw(u_int port);
void    insw(u_int port, void *addr, size_t cnt);
void    insb(u_int port, void *addr, size_t cnt);
void    outb(u_int port,unsigned char v);
void    outsb(u_int port, void *addr, size_t cnt);
void    outsw(u_int port, void *addr, size_t cnt);
void    outw(u_int port, u_short data);

void    outl(u_int port, u_long data);
u_long  inl(u_int port);

/*определяем внутреннее представление чисел с плавающей точкой */
#define IEEE_8087
#define national

/*функции, используемые только в архитектурнозависимом исполнении*/
#define  __HAVE_ARCH_FFS

#define __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_BCOPY
#define __HAVE_ARCH_BZERO


#endif /* _ARCH_I386_CPU_H */
