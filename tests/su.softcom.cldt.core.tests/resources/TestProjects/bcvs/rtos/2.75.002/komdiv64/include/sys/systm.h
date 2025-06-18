#ifndef _SYS_SYSTEM_H
#define _SYS_SYSTEM_H
#ifdef KERNEL
#include <sys/cdefs.h>
#include <sys/callout.h>
#include <vm/vm_extern.h>
/* Define NULL */
#ifndef NULL
#define NULL      0          /* NULL value */
#endif
extern int bootverbose;
/*
 * Description of a process.
 *
 * This structure contains the information needed to manage a thread of
 * control, known in UN*X as a process; it has references to substructures
 * containing descriptions of things that the process uses, but may share
 * with related processes.  The process structure and the substructures
 * are always addressable except for those marked "(PROC ONLY)" below,
 * which might be addressable only on a processor on which the process
 * is running.
 */
struct itimerval
{
    int a;
};
struct   rtprio
{
        int a;
};
struct   sigiolst
{
    int a;
};
struct   mdproc
{
    int a;
};
struct   pasleep
{
    int a;
};
#define  MAXCOMLEN   16    /* max command name remembered */
/*
 * Priorities.  Note that with 32 run queues, differences less than 4 are
 * insignificant.
 */
#define  PSWP  0
#define  PVM   4
#define  PINOD 8
#define  PRIBIO   16
#define  PVFS  20
#define  PZERO 22    /* No longer magic, shouldn't be here.  XXX */
#define  PSOCK 24
#define  PWAIT 32
#define  PLOCK 36
#define  PPAUSE   40
#define  PUSER 50
#define  MAXPRI   127      /* Priorities range from 0 through MAXPRI. */
#define  PCATCH   0x100    /* OR'd with pri for tsleep to check signals */


/*
 * User specifiable flags.
 */
#define  MNT_RDONLY  0x00000001  /* read only filesystem */
#define  MNT_SYNCHRONOUS   0x00000002  /* file system written synchronously */
#define  MNT_NOEXEC  0x00000004  /* can't exec from filesystem */
#define  MNT_NOSUID  0x00000008  /* don't honor setuid bits on fs */
#define  MNT_NODEV   0x00000010  /* don't interpret special files */
#define  MNT_UNION   0x00000020  /* union with underlying filesystem */
#define  MNT_ASYNC   0x00000040  /* file system written asynchronously */
#define  MNT_SUIDDIR 0x00100000  /* special handling of SUID on dirs */
#define  MNT_SOFTDEP 0x00200000  /* soft updates being done */
#define  MNT_NOSYMFOLLOW   0x00400000  /* do not follow symlinks */
#define  MNT_NOATIME 0x10000000  /* disable update of file access time */
#define  MNT_NOCLUSTERR 0x40000000  /* disable cluster read */
#define  MNT_NOCLUSTERW 0x80000000  /* disable cluster write */



#define ROOTNAME  "root_device"

#ifdef KERNEL
/* pseudo-errors returned inside kernel to modify return to process */
#define  ERESTART (-1)     /* restart syscall */
#define  EJUSTRETURN (-2)     /* don't modify regs, just return */
#define  ENOIOCTL (-3)     /* ioctl not handled by this layer */
#define  ENOTBLK     1115     /* Block device required */
#endif

#define __FreeBSD__

#define  NODEV (dev_t)(-1) /* non-existent device */

#define vm_page_alloc(kernel_object,s,VM_ALLOC_NORMAL) 0


#define MAP_NOFAULT 0

#define  KASSERT(exp,msg)  do { if (!(exp)) panic msg; } while (0)

#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef _POSIX_SOURCE
/*
 * minor() gives a cookie instead of an index since we don't want to
 * change the meanings of bits 0-15 or waste time and space shifting
 * bits 16-31 for devices that don't use them.
 */
#define  major(x) ((int)(((u_int)(x) >> 8)&0xff))  /* major number */
#define  minor(x) ((int)((x)&0xffff00ff))    /* minor number */
#define  makedev(x,y)   ((dev_t)(((x)<<8) | (y)))  /* create dev_t */
#endif


#include <arch/cpu.h>


#define time_second getsec()
extern int boardTimerHZ;
#define hz boardTimerHZ
extern int tick;
#define ETOOMANYREFS -3 /*FIXME  */

#define imin(a,b) (((a) < (b)) ? (a) : (b))  /* interger min */
#define imax(a,b) (((a) > (b)) ? (a) : (b))  /* interger min */
#define lmin(a,b) (((a) < (b)) ? (a) : (b))  /* interger min */
#define ulmin(a,b)   (((a) < (b)) ? (a) : (b))  /* interger min */

#define ovbcopy bcopy

struct linker_set {
   int ls_length;
   void *ls_items[100];  /* really ls_length of them, trailing NULL */
};
#define LINKER_SET_FOREACH( field, ls ) \
      int __i=0; \
      for (__i=0, field = ls.ls_items[0]; field; field = ls.ls_items[++__i])

struct proc;

int splimp();
int splvm();
int splbio();
int splhigh();
int splnet();
int splsoftclock();
int spl0();
void splx (int x);

void panic (char *msg,...);
int psignal(struct proc *p,int signum);
void wakeup(void *ident);
void wakeup_one(void *ident);
int tsleep(void *ident, int priority,const char *wmesg, int timo);
int printf(const char *fmt,...);
int sprintf(char *buf,const char *fmt,...);
int snprintf(char *str, size_t n, char const *fmt, ...);

int   copystr __P((const void *kfaddr, void *kdaddr, size_t len,
      size_t *lencopied));
int   copyinstr __P((const void *udaddr, void *kaddr, size_t len,
      size_t *lencopied));
int   copyout __P((const void *kaddr, void *udaddr, size_t len));
int   copyin __P((const void *udaddr, void *kaddr, size_t len));

caddr_t kmem_malloc(int mb_map,int size,int how);

#ifndef _STRINGS_H
void bcopy( const void *src, void *dest, size_t len );
void bzero( void *s, size_t n );
int  bcmp ( const void *str1, const void *str2, size_t n );
#endif

#ifndef _STRING_H
char    *strncpy(char *,const char *,size_t);
char    *strcpy(char *,const char *);
size_t  strlen(const char *);
void    *memcpy(void *,const void *,size_t);
int     strncmp ( const char *s1, const char *s2, size_t  maxlen ) ;
#endif

struct timeval;
struct timespec;
void getmicrotime(struct timeval *tvp);
void getmicrouptime(struct timeval *tvp);
void getnanotime(struct timespec *tsp);
int    tvtohz(struct timeval *tv);
void microtime(struct timeval *tv);
u_long getsec();
u_long getusec();

void setsoftnet();

struct sigio;
pid_t fgetown __P((struct sigio *));
int      fsetown __P((pid_t, struct sigio **));
void  funsetown __P((struct sigio *));

long  random();

void vntblinit(int n);
void nchinit();
void *hashinit(int elements,int /*struct malloc_type * */ type,uint32_t *hashmask);

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int  getenv_int(char *name, int *data);

extern struct timeval boottime;


#endif /* KERNEL */

int     hmPrintf(const char *format,...);

#endif /* _SYS_SYSTEM_H */
