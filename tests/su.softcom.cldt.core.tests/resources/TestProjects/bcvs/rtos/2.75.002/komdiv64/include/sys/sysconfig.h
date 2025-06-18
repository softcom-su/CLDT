#ifndef _SYS_SYSCONFIG_H
#define _SYS_SYSCONFIG_H
/*
 * Constants related to network buffer management.
 * MCLBYTES must be no larger than CLBYTES (the software page size), and,
 * on machines that exchange pages of input or output buffers with mbuf
 * clusters (MAPPED_MBUFS), MCLBYTES must also be an integral multiple
 * of the hardware page size.
 */
#define PGSHIFT		12		/* LOG2(NBPG) */
#define PAGE_SHIFT	12
#define NBPG		(1 << PAGE_SHIFT)	/* bytes/page */

#define	CLBYTES		(1*NBPG)

#ifndef __CONFIGURE__

#define MAX_MBUFS		4096	/* 1024	max mbuf allocation */
#define MAX_CLUSTERS	132		/* max cluster allocation */
#define NCL_INIT		8		/* Четыре кластера по CLBYTES */
#define MBUF_INIT		256     /* 256 m буферoв по MSIZE */
#define NCL_ADD			4
#define MBUF_ADD		16

#endif

struct mbconfig {
	int  max_mbufs;
	int  max_clusters;
	int  ncl_init;
	int  mbuf_init;
	int  ncl_add;
	int  mbuf_add;
};

extern void mbinit (void *);
extern void ifinit(void);
extern void domaininit(void);
extern void netInit(int);
#endif
