/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
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
 *	@(#)nfsdiskless.h	8.2 (Berkeley) 3/30/95
 * $FreeBSD: src/sys/nfs/nfsdiskless.h,v 1.10.2.1 1999/08/29 16:30:33 peter Exp $
 */


#ifndef _NFS_NFSDISKLESS_H_
#define _NFS_NFSDISKLESS_H_

/*
 * Structure that must be initialized for a diskless nfs client.
 * This structure is used by nfs_mountroot() to set up the root vnode,
 * and to do a partial ifconfig(8) and route(8) so that the critical net
 * interface can communicate with the server.
 * The primary bootstrap is expected to fill in the appropriate fields before
 * starting the kernel. Whether or not the swap area is nfs mounted is
 * determined by the value in swdevt[0]. (equal to NODEV --> swap over nfs)
 * Currently only works for AF_INET protocols.
 * NB: All fields are stored in net byte order to avoid hassles with
 * client/server byte ordering differences.
 */

/*
 * I have defined a new structure that can handle an NFS Version 3 file handle
 * but the kernel still expects the old Version 2 one to be provided. The
 * changes required in nfs_vfsops.c for using the new are documented there in
 * comments. (I felt that breaking network booting code by changing this
 * structure would not be prudent at this time, since almost all servers are
 * still Version 2 anyhow.)
 */
struct nfsv3_diskless {
	struct ifaliasreq myif;			/* Default interface */
	struct sockaddr_in mygateway;		/* Default gateway */
	struct nfs_args	swap_args;		/* Mount args for swap file */
	int		swap_fhsize;		/* Size of file handle */
	u_char		swap_fh[NFSX_V3FHMAX];	/* Swap file's file handle */
	struct sockaddr_in swap_saddr;		/* Address of swap server */
	char		swap_hostnam[MNAMELEN];	/* Host name for mount pt */
	int		swap_nblks;		/* Size of server swap file */
	struct ucred	swap_ucred;		/* Swap credentials */
	struct nfs_args	root_args;		/* Mount args for root fs */
	int		root_fhsize;		/* Size of root file handle */
	u_char		root_fh[NFSX_V3FHMAX];	/* File handle of root dir */
	struct sockaddr_in root_saddr;		/* Address of root server */
	char		root_hostnam[MNAMELEN];	/* Host name for mount pt */
	long		root_time;		/* Timestamp of root fs */
#define MAXHOSTNAMELEN 30
	char		my_hostnam[MAXHOSTNAMELEN]; /* Client host name */
};

/*
 * Old arguments to mount NFS
 */
struct onfs_args {
	struct sockaddr	*addr;		/* file server address */
	int		addrlen;	/* length of address */
	int		sotype;		/* Socket type */
	int		proto;		/* and Protocol */
	u_char		*fh;		/* File handle to be mounted */
	int		fhsize;		/* Size, in bytes, of fh */
	int		flags;		/* flags */
	int		wsize;		/* write size in bytes */
	int		rsize;		/* read size in bytes */
	int		readdirsize;	/* readdir size in bytes */
	int		timeo;		/* initial timeout in .1 secs */
	int		retrans;	/* times to retry send */
	int		maxgrouplist;	/* Max. size of group list */
	int		readahead;	/* # of blocks to readahead */
	int		leaseterm;	/* Term (sec) of lease */
	int		deadthresh;	/* Retrans threshold */
	char		*hostname;	/* server's name */
};

struct nfs_diskless {
	struct ifaliasreq myif;			/* Default interface */
	struct sockaddr_in mygateway;		/* Default gateway */
	struct onfs_args swap_args;		/* Mount args for swap file */
	u_char		swap_fh[NFSX_V2FH];	/* Swap file's file handle */
	struct sockaddr_in swap_saddr;		/* Address of swap server */
	char		swap_hostnam[MNAMELEN];	/* Host name for mount pt */
	int		swap_nblks;		/* Size of server swap file */
	struct ucred	swap_ucred;		/* Swap credentials */
	struct onfs_args root_args;		/* Mount args for root fs */
	u_char		root_fh[NFSX_V2FH];	/* File handle of root dir */
	struct sockaddr_in root_saddr;		/* Address of root server */
	char		root_hostnam[MNAMELEN];	/* Host name for mount pt */
	long		root_time;		/* Timestamp of root fs */
	char		my_hostnam[MAXHOSTNAMELEN]; /* Client host name */
};

#endif
