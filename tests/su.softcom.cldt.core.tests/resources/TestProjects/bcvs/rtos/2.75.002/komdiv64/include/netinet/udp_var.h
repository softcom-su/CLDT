/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)udp_var.h	8.1 (Berkeley) 6/10/93
 * $FreeBSD: src/sys/netinet/udp_var.h,v 1.16.2.2 1999/08/29 16:29:59 peter Exp $
 */

#ifndef _NETINET_UDP_VAR_H_
#define _NETINET_UDP_VAR_H_

/*
 * UDP kernel structures and variables.
 */
struct	udpiphdr {
	struct 	ipovly ui_i;		/* overlaid ip structure */
	struct	udphdr ui_u;		/* udp header */
};
#define	ui_x1		ui_i.ih_x1
#define	ui_pr		ui_i.ih_pr
#define	ui_len		ui_i.ih_len
#define	ui_src		ui_i.ih_src
#define	ui_dst		ui_i.ih_dst
#define	ui_sport	ui_u.uh_sport
#define	ui_dport	ui_u.uh_dport
#define	ui_ulen		ui_u.uh_ulen
#define	ui_sum		ui_u.uh_sum

struct	udpcb {
	/* XXX - these should be by reference so we can do options quickly */
	struct	ip udb_ip;
	struct	udphdr udb_uh;
	struct	sockaddr_in udb_conn;
	struct	in_hostcache *udb_hc;
	struct	mbuf *udb_queue;
};
#define	inptoudpcb(inp)	((struct udpdb *)(inp)->inp_ppcb)

struct	udpstat {
				/* input statistics: */
	uint32_t	udps_ipackets;		/* total input packets */
	uint32_t	udps_hdrops;		/* packet shorter than header */
	uint32_t	udps_badsum;		/* checksum error */
	uint32_t	udps_badlen;		/* data length larger than packet */
	uint32_t	udps_noport;		/* no socket on port */
	uint32_t	udps_noportbcast;	/* of above, arrived as broadcast */
	uint32_t	udps_fullsock;		/* not delivered, input socket full */
	uint32_t	udpps_pcbcachemiss;	/* input packets missing pcb cache */
	uint32_t	udpps_pcbhashmiss;	/* input packets not for hashed pcb */
				/* output statistics: */
	uint32_t	udps_opackets;		/* total output packets */
	uint32_t	udps_fastout;		/* output packets on fast path */
};

/*
 * Names for UDP sysctl objects
 */
#define	UDPCTL_CHECKSUM		1	/* checksum UDP packets */
#define UDPCTL_STATS		2	/* statistics (read-only) */
#define	UDPCTL_MAXDGRAM		3	/* max datagram size */
#define	UDPCTL_RECVSPACE	4	/* default receive buffer space */
#define	UDPCTL_PCBLIST		5	/* list of PCBs for UDP sockets */
#define UDPCTL_MAXID		6

#define UDPCTL_NAMES { \
	{ 0, 0 }, \
	{ "checksum", CTLTYPE_INT }, \
	{ "stats", CTLTYPE_STRUCT }, \
	{ "maxdgram", CTLTYPE_INT }, \
	{ "recvspace", CTLTYPE_INT }, \
	{ "pcblist", CTLTYPE_STRUCT }, \
}

#ifdef KERNEL
extern struct	pr_usrreqs udp_usrreqs;
extern struct	inpcbinfo udbinfo;

void	 udp_ctlinput __P((int, struct sockaddr *, void *));
void	 udp_init __P((void));
void	 udp_input __P((struct mbuf *, int));
#endif

#endif
