#ifndef  _STRINGS_H
#define  _STRINGS_H

#include <string.h>
#include <arch/cpu.h>

#ifndef bcmp
int    bcmp(const void *, const void *, size_t);
#endif
#ifndef bcopy
void   bcopy(const void *, void *, size_t);
#endif
#ifndef bzero
void   bzero(void *, size_t);
#endif
#ifndef _ARCH_FFS
#define ffs flsbs
#else
int ffs(int);
#endif
#define __HAVE_ARCH_FFS

#ifndef index
#define index(a,b) strchr((a),(b))
#endif
#ifndef rindex
#define rindex(a,b) strrchr((a),(b))
#endif

int    strcasecmp(const char *, const char *);
int    strncasecmp(const char *, const char *, size_t);

extern unsigned const char const  fmsbTbl [];
extern unsigned const char const flsbTbl [];
/****************************************************************
 *
 * fmsbs1 - поиск старшего бита в 32 разрядном слове установленного в 1
 * (биты нумеруются с 1)
 */
static __inline__ int fmsbs1(u_long arg)
{
   u_short hiw, low;
   unsigned char ch;

   hiw = (u_short) (arg >> 16);
   if (hiw)
   {
      ch = (unsigned char) (hiw >> 8);
      if (ch)
         return (fmsbTbl[ch] + 24);
      return (fmsbTbl[(unsigned char) hiw] + 16);
   }

   low = (u_short) (arg & 0xffff);
   ch = (unsigned char) (low >> 8);
   if (ch)
      return (fmsbTbl[ch] + 8);
   return (fmsbTbl[(unsigned char) low]);
}
/****************************************************************
 *
 * fmsbs - поиск старшего бита в 32 разрядном слове установленного в 1
 * (биты нумеруются с 0)
 */
static __inline__ int fmsbs(u_long arg)
{
   if (arg == 0)
      return (0);
   return fmsbs1(arg)-1;
}
/********************************************************************
 *
 * flsbs - поиск младшего бита в 32 разрядном слове установленного в 1
 * (биты нумеруются с 1)
 */
static __inline__ int flsbs(u_long arg)
{
   u_short hiw, low;
   unsigned char ch;
   if (arg == 0)
      return (0);

   low = (u_short) (arg & 0xffff);
   if (low)
   {
      ch = (unsigned char) (low & 0xff);
      if (ch)
         return (flsbTbl[ch]);
      return (flsbTbl[(unsigned char) (low >> 8)] + 8);
   }

   hiw = (u_short) (arg >> 16);
   ch = (unsigned char) (hiw & 0xff);
   if (ch)
      return (flsbTbl[ch] + 16);
   return (flsbTbl[(unsigned char) (hiw >> 8)] + 24);
}
/////////////////////////// 64-bit ///////////////////////////////

/****************************************************************
 *
 * fmsbs64() - поиск старшего бита в 64-разрядном слове установленного в 1
 * (биты нумеруются с 0)
 */
static __inline__ int fmsbs64(u_int64_t arg)
{
   u_int32_t hi32, lo32;
   hi32 = (u_int32_t) (arg >> 32);
   if (hi32)
      return fmsbs(hi32)+32;
   else
   {
      lo32 = (u_int32_t) (arg & 0xffffffff);
      return fmsbs(lo32);
   }
}

/****************************************************************
 *
 * fmsbs64_1() - поиск старшего бита в 64-разрядном слове установленного в 1
 * (биты нумеруются с 1)
 */
static __inline__ int fmsbs64_1(u_int64_t arg)
{
   u_int32_t hi32, lo32;
   hi32 = (u_int32_t) (arg >> 32);
   if (hi32)
      return fmsbs1(hi32)+32;
   else
   {
      lo32 = (u_int32_t) (arg & 0xffffffff);
      return fmsbs1(lo32);
   }
}

/*****************************************************************
 *
 * flsbs64() - поиск младшего бита в 64-разрядном слове установленного в 1
 * (биты нумеруются с 1)
 */
static __inline__ int flsbs64(u_int64_t arg)
{
   u_int32_t hi32, lo32;
   lo32 = (u_int32_t) (arg & 0xffffffff);
   if (lo32)
      return flsbs(lo32);
   else
   {
      hi32 = (u_int32_t) (arg >> 32);
      return flsbs(hi32)+32;
   }
}
#endif /* !_STRINGS_H */

