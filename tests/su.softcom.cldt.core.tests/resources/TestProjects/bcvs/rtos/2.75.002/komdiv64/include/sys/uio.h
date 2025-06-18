#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#include <sys/types.h>

#define UIO_MAXIOV	1024		/* max 1K of iov's */
#define UIO_SMALLIOV	8		/* 8 on stack, else malloc */

struct iovec {
	char	*iov_base;	/* Base address. */
	size_t	 iov_len;	/* Length. */
};

enum	uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE,		/* from system space */
	UIO_USERISPACE,		/* from user I space */
	UIO_NOCOPY			/* don't copy, already in object */
};

struct uio {
	struct	iovec *uio_iov;
	int		uio_iovcnt;
	off_t	uio_offset;
	int		uio_resid;
	enum	uio_seg uio_segflg;
	enum	uio_rw uio_rw;
	struct	proc *uio_procp;
};

int uiomove(caddr_t ,int len,struct uio *);


int  readv(int  fd,  const  struct  iovec * vector, int count);
int writev(int fd, const struct  iovec  *  vector,  int count);

#endif
