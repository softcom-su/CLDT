#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#include <arch/param.h>

#define	NGROUPS 1
#define MAXPHYS		(64 * 1024)	/* max raw I/O transfer size */

#include <sys/types.h>
#include <sys/time.h>
#include <arch/lock.h>
#include <limits.h>
#include <arch/endian.h>

#include <errno.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0l
#endif

/*FIXME  */
#define SIGXFSZ 1
/*
 * File system parameters and macros.
 *
 * The file system is made out of blocks of at most MAXBSIZE units, with
 * smaller units (fragments) only in the last direct block.  MAXBSIZE
 * primarily determines the size of buffers in the buffer pool.  It may be
 * made larger without any effect on existing file systems; however making
 * it smaller make make some file systems unmountable.  Also, MAXBSIZE
 * must be less than MAXPHYS!!!  DFLTBSIZE is the average amount of
 * memory allocated by vfs_bio per nbuf.  BKVASIZE is the average amount
 * of kernel virtual space allocated per nbuf.  BKVASIZE should be >=
 * DFLTBSIZE.  If it is significantly bigger than DFLTBSIZE, then
 * kva fragmentation causes fewer performance problems.
 */
#define	MAXBSIZE	65536
#define BKVASIZE	8192
#define DFLTBSIZE	4096
#define MAXFRAG 	8
#define MAXPATHLEN 255
/* #define NAME_MAX 255 */
#define MAXSYMLINKS 8
#define NULLDEV

#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#define powerof2(x)	((((x)-1)&(x))==0)

#endif
