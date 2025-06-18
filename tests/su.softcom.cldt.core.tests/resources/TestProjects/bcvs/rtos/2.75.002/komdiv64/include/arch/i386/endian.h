#ifndef _ARCH_I386_ENDIAN_H
#define _ARCH_I386_ENDIAN_H

#ifdef __GNUC__
#define __word_swap_long(x) \
({ register unsigned long __X = (x); \
   __asm ("rorl $16, %1" \
    : "=r" (__X) \
    : "0" (__X)); \
   __X; })
#if __GNUC__ >= 2
#define __byte_swap_long(x) \
({ register unsigned long __X = (x); \
   __asm ("xchgb %h1, %b1\n\trorl $16, %1\n\txchgb %h1, %b1" \
    : "=q" (__X) \
    : "0" (__X)); \
   __X; })
#define __byte_swap_word(x) \
({ register unsigned short __X = (x); \
   __asm ("xchgb %h1, %b1" \
    : "=q" (__X) \
    : "0" (__X)); \
   __X; })
#else /* __GNUC__ >= 2 */
#define __byte_swap_long(x) \
({ register unsigned long __X = (x); \
   __asm ("rorw $8, %w1\n\trorl $16, %1\n\trorw $8, %w1" \
    : "=r" (__X) \
    : "0" (__X)); \
   __X; })
#define __byte_swap_word(x) \
({ register unsigned short __X = (x); \
   __asm ("rorw $8, %w1" \
    : "=r" (__X) \
    : "0" (__X)); \
   __X; })
#endif /* __GNUC__ >= 2 */
#else
#ifdef _MSC_VER
#pragma warning(disable: 4035 4514)
#endif

static __inline__ unsigned long __UNUSED_F __word_swap_long(unsigned long x) 
{ 
   ASM    mov eax,x
   ASM    ror eax,16
}
static __inline__ unsigned long __UNUSED_F __byte_swap_long(unsigned long x) 
{ 
   ASM    mov eax,x
   ASM    ror ax,8
   ASM    ror eax,16
   ASM    ror ax,8
}

static __inline__ unsigned short __UNUSED_F __byte_swap_word(unsigned short x) 
{ 
   ASM    mov  ax,x
   ASM    ror  ax,8
}
#endif

#ifndef BYTE_ORDER
#define    BYTE_ORDER    LITTLE_ENDIAN
#endif

#ifndef i386
#define i386 1
#endif

typedef struct {
    unsigned long lo;
    long    hi;
} es_longlong; /* endian safe long-long */
/*
 * Define the order of 32-bit words in 64-bit words.
 */
#define    _QUAD_HIGHWORD 1
#define    _QUAD_LOWWORD 0

#endif /* _ARCH_I386_ENDIAN_H */
