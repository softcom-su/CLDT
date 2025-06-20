/*-
 * Copyright (c) 1997 Peter Wemm <peter@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/sys/poll.h,v 1.4.2.1 1999/08/29 16:32:36 peter Exp $
 */

#ifndef _SYS_POLL_H_
#define	_SYS_POLL_H_

/*
 * This file is intended to be compatable with the traditional poll.h.
 */

/*
 * This structure is passed as an array to poll(2).
 */
struct pollfd {
	int	fd;		/* which file descriptor to poll */
	short	events;		/* events we are interested in */
	short	revents;	/* events found on return */
};

/*
 * Requestable events.  If poll(2) finds any of these set, they are
 * copied to revents on return.
 * XXX Note that FreeBSD doesn't make much distinction between POLLPRI
 * and POLLRDBAND since none of the file types have distinct priority
 * bands - and only some have an urgent "mode".
 * XXX Note POLLIN isn't really supported in true SVSV terms.  Under SYSV
 * POLLIN includes all of normal, band and urgent data.  Most poll handlers
 * on FreeBSD only treat it as "normal" data.
 */
#define	POLLIN		0x0001		/* any readable data available */
#define	POLLPRI		0x0002		/* OOB/Urgent readable data */
#define	POLLOUT		0x0004		/* file descriptor is writeable */
#define	POLLRDNORM	0x0040		/* non-OOB/URG data available */
#define	POLLWRNORM	POLLOUT		/* no write type differentiation */
#define	POLLRDBAND	0x0080		/* OOB/Urgent readable data */
#define	POLLWRBAND	0x0100		/* OOB/Urgent data can be written */

/*
 * FreeBSD extensions: polling on a regular file might return one
 * of these events (currently only supported on UFS).
 */
#define	POLLEXTEND	0x0200		/* file may have been extended */
#define	POLLATTRIB	0x0400		/* file attributes may have changed */
#define	POLLNLINK	0x0800		/* (un)link/rename may have happened */
#define	POLLWRITE	0x1000		/* file's contents may have changed */

/*
 * These events are set if they occur regardless of whether they were
 * requested.
 */
#define	POLLERR		0x0008		/* some poll error occurred */
#define	POLLHUP		0x0010		/* file descriptor was "hung up" */
#define	POLLNVAL	0x0020		/* requested events "invalid" */

#define	POLLSTANDARD	(POLLIN|POLLPRI|POLLOUT|POLLRDNORM|POLLRDBAND|\
			 POLLWRBAND|POLLERR|POLLHUP|POLLNVAL)

/*
 * Request that poll() wait forever.
 * XXX in SYSV, this is defined in stropts.h, which is not included
 * by poll.h.
 */
#define	INFTIM		(-1)

#ifndef KERNEL

#include <sys/cdefs.h>

__BEGIN_DECLS
/*
 * XXX logically, poll() should be declared in <poll.h>, but SVR4 at
 * least has it here in <sys/poll.h>.
 * XXX poll() has "unsigned long" nfds on SVR4, not unsigned as on the
 * other BSDs.
 */
int	poll __P((struct pollfd *_pfd, unsigned _nfds, int _timeout));
__END_DECLS

#endif /* !KERNEL */

#endif /* !_SYS_POLL_H_ */
