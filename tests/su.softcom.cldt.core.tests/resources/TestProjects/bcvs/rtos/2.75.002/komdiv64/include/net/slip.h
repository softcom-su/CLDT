/*-
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)slip.h	8.1 (Berkeley) 2/12/94
 * $FreeBSD: src/sys/net/slip.h,v 1.8.4.1 1999/08/29 16:28:35 peter Exp $
 */

#ifndef _NET_SLIP_H_
#define	_NET_SLIP_H_

/* Ioctls operating on SLIP ttys. */
#define	SLIOCGUNIT	_IOR('t', 88, int)	/* get slip unit number */
#define	SLIOCSKEEPAL	_IOW('t', 84, int)	/* set keepalive */
#define	SLIOCSOUTFILL	_IOW('t', 83, int)	/* set out fill time */
#define	SLIOCGKEEPAL	_IOR('t', 82, int)	/* get keepalive time */
#define	SLIOCGOUTFILL	_IOR('t', 81, int)	/* get out fill time */
#define	SLIOCSUNIT	_IOW('t', 80, int)	/* set slip unit number */

/*
 * Definitions of the pseudo-link-level header attached to slip
 * packets grabbed by the packet filter (bpf) traffic monitor.
 */
#define	SLIP_HDRLEN	16		/* BPF SLIP header length */

/* Offsets into BPF SLIP header. */
#define	SLX_DIR		0		/* direction; see below */
#define	SLX_CHDR	1		/* compressed header data */
#define	CHDR_LEN	15		/* length of compressed header data */

#define	SLIPDIR_IN	0		/* incoming */
#define	SLIPDIR_OUT	1		/* outgoing */

#endif /* !_NET_SLIP_H */
