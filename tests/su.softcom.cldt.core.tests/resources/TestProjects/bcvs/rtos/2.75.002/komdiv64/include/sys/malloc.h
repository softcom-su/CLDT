#if !defined( _SYS_MALLOC_H ) && defined(KERNEL)
#define _SYS_MALLOC_H
#include <sys/types.h>
#include <sys/systm.h>
#include <mem.h>
#define M_PCB			1
#define M_IFADDR		15
#define M_SONAME		16
#define M_RTABLE		17
#define M_IPMADDR		18
#define M_IPMOPTS		19
#define M_IFMADDR		20
#define M_SUBPROC		21
#define M_IGMP			22
#define M_CRED			23
#define M_IPFLOW		24
#define M_IOV			25
#define M_ZONE			25
#define M_MSDOSFSMNT	26
#define M_MSDOSFSNODE	27
#define M_MSDOSFSFAT	28
#define M_VNODE			29
#define M_NFSREQ		30
#define M_NFSBIGFH		31
#define M_NFSHASH		32
#define M_NFSDIROFF		33
#define M_NQMHOST		34
#define M_NQLEASE		35
#define M_NFSUID		36
#define M_NFSRVDESC		37
#define M_BIOBUF		38
#define M_CLUSTER		39
#define M_DEVDATA		40
#define M_LOCKF			41
#define M_NFSSVC		42
#define M_NFSD			43
#define M_DEVBUF		44
#define	M_TEMP			45	/* misc temporary data buffers */
#define	M_CACHE			46
#define M_MOUNT			47
#define M_NETADDR		48
#define M_IPFW		        49
#define M_ISOFSMNT              50
#define M_ISOFSNODE             51

/* #define	M_MAGIC		877983977	*//* time when first defined :-) */ 
#if 0
struct malloc_type {
	struct malloc_type *ks_next;	/* next in list */
	long 	ks_memuse;	/* total memory held in bytes */
	long	ks_limit;	/* most that are allowed to exist */
	long	ks_size;	/* sizes of this thing that are allocated */
	long	ks_inuse;	/* # of packets of this type currently in use */
	long	ks_calls;	/* total packets of this type ever allocated */
	long	ks_maxused;	/* maximum number ever used */
	u_long	ks_magic;	/* if it's not magic, don't touch it */
	const char *ks_shortdesc;	/* short description */
	u_short	ks_limblocks;	/* number of times blocked for hitting limit */
	u_short	ks_mapblocks;	/* number of times blocked for kernel map */
};


#define	MALLOC_DEFINE(type, shortdesc, longdesc) \
	struct malloc_type type[1] = { \
		{ NULL, 0, 0, 0, 0, 0, 0, M_MAGIC, shortdesc, 0, 0 } \
	}; \
	SYSINIT(type##_init, SI_SUB_KMEM, SI_ORDER_ANY, malloc_init, type); \
	SYSUNINIT(type##_uninit, SI_SUB_KMEM, SI_ORDER_ANY, malloc_uninit, type)

#define	MALLOC_DECLARE(type) \
	extern struct malloc_type type[1]
#else

#define	MALLOC_DEFINE(type, shortdesc, longdesc) int a___##type __attribute__ ((used))
#define	MALLOC_DECLARE(type)

#endif
#define M_NOWAIT 		1
#define M_WAITOK        2

static __inline void *syspool_malloc(u_long size,int type, int flags)
{
	void *ret = 0;
    if(size)
    {
       ret=memAlloc(memSysPoolId,size,0);
	   if(!ret && flags == M_WAITOK)
	   {
		  panic("nomem size=%x type=%d\n");
	   }
    }
	return ret;
}
static __inline void *asyspool_malloc(u_long size,int type, int flags,int align)
{
   void *ret = 0;
    if(size)
    {
       ret=memAlloc(memSysPoolId,size,align);
      if(!ret && flags == M_WAITOK)
      {
        panic("nomem size=%x type=%d\n");
      }
    }
   return ret;
}

void *bufpool_malloc(u_long size,int type, int flags);
void bufpool_free(caddr_t addr,int type);
int  bufpoolInit(int maxbufspace);

static __inline void syspool_free(caddr_t addr,int type)
{
	memFree(memSysPoolId,addr);
}
static __inline void *userpool_malloc(u_long size)
{
	caddr_t ret=memAlloc(0,size,0);
	return ret;
}
static __inline void userpool_free(caddr_t addr)
{
	memFree(0,addr);
}

#define	MALLOC(space, cast, size, type, flags) \
	(space) = (cast)syspool_malloc((u_long)(size), type, flags)
#define FREE(addr, type) syspool_free((caddr_t)(addr), type)

#define malloc(a,b,c)	syspool_malloc(a,(int)b,c)
#define free(a,b)		syspool_free((caddr_t)a,b)

#endif
