/*
 * Copyright (c) 1982, 1986, 1993, 1994, 1995
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
 *	@(#)tcp_var.h	8.4 (Berkeley) 5/24/95
 * $FreeBSD: src/sys/netinet/tcp_var.h,v 1.49.2.1 1999/08/29 16:29:57 peter Exp $
 */

#ifndef _NETINET_TCP_VAR_H_
#define _NETINET_TCP_VAR_H_
/*
 * Kernel variables for tcp.
 */

/*
 * Tcp control block, one per tcp; fields:
 * Organized for 16 byte cacheline efficiency.
 */
struct tcpcb {
	struct	mbuf *t_segq;
	int	t_dupacks;		/* consecutive dup acks recd */
	struct	tcpiphdr *t_template;	/* skeletal packet for transmit */

	int	t_timer[TCPT_NTIMERS];	/* tcp timers */

	struct	inpcb *t_inpcb;		/* back pointer to internet pcb */
	int	t_state;		/* state of this connection */
	u_int	t_flags;
#define	TF_ACKNOW	0x00001		/* ack peer immediately */
#define	TF_DELACK	0x00002		/* ack, but try to delay it */
#define	TF_NODELAY	0x00004		/* don't delay packets to coalesce */
#define	TF_NOOPT	0x00008		/* don't use tcp options */
#define	TF_SENTFIN	0x00010		/* have sent FIN */
#define	TF_REQ_SCALE	0x00020		/* have/will request window scaling */
#define	TF_RCVD_SCALE	0x00040		/* other side has requested scaling */
#define	TF_REQ_TSTMP	0x00080		/* have/will request timestamps */
#define	TF_RCVD_TSTMP	0x00100		/* a timestamp was received in SYN */
#define	TF_SACK_PERMIT	0x00200		/* other side said I could SACK */
#define	TF_NEEDSYN	0x00400		/* send SYN (implicit state) */
#define	TF_NEEDFIN	0x00800		/* send FIN (implicit state) */
#define	TF_NOPUSH	0x01000		/* don't push */
#define	TF_REQ_CC	0x02000		/* have/will request CC */
#define	TF_RCVD_CC	0x04000		/* a CC was received in SYN */
#define	TF_SENDCCNEW	0x08000		/* send CCnew instead of CC in SYN */
#define	TF_MORETOCOME	0x10000		/* More data to be appended to sock */
	int	t_force;		/* 1 if forcing out a byte */

	tcp_seq	snd_una;		/* send unacknowledged */
	tcp_seq	snd_max;		/* highest sequence number sent;
					 * used to recognize retransmits
					 */
	tcp_seq	snd_nxt;		/* send next */
	tcp_seq	snd_up;			/* send urgent pointer */

	tcp_seq	snd_wl1;		/* window update seg seq number */
	tcp_seq	snd_wl2;		/* window update seg ack number */
	tcp_seq	iss;			/* initial send sequence number */
	tcp_seq	irs;			/* initial receive sequence number */

	tcp_seq	rcv_nxt;		/* receive next */
	tcp_seq	rcv_adv;		/* advertised window */
	uint32_t	rcv_wnd;		/* receive window */
	tcp_seq	rcv_up;			/* receive urgent pointer */

	uint32_t	snd_wnd;		/* send window */
	uint32_t	snd_cwnd;		/* congestion-controlled window */
	uint32_t	snd_ssthresh;		/* snd_cwnd size threshold for
					 * for slow start exponential to
					 * linear switch
					 */
	u_int	t_maxopd;		/* mss plus options */

	u_int	t_idle;			/* inactivity time */
	uint32_t	t_duration;		/* connection duration */
	int	t_rtt;			/* round trip time */
	tcp_seq	t_rtseq;		/* sequence number being timed */

	int	t_rxtcur;		/* current retransmit value */
	u_int	t_maxseg;		/* maximum segment size */
	int	t_srtt;			/* smoothed round-trip time */
	int	t_rttvar;		/* variance in round-trip time */

	int	t_rxtshift;		/* log(2) of rexmt exp. backoff */
	u_int	t_rttmin;		/* minimum rtt allowed */
	uint32_t	t_rttupdated;		/* number of times rtt sampled */
	uint32_t	max_sndwnd;		/* largest window peer has offered */

	int	t_softerror;		/* possible error not yet reported */
/* out-of-band data */
	char	t_oobflags;		/* have some */
	char	t_iobc;			/* input character */
#define	TCPOOB_HAVEDATA	0x01
#define	TCPOOB_HADDATA	0x02
/* RFC 1323 variables */
	u_char	snd_scale;		/* window scaling for send window */
	u_char	rcv_scale;		/* window scaling for recv window */
	u_char	request_r_scale;	/* pending window scaling */
	u_char	requested_s_scale;
	uint32_t	ts_recent;		/* timestamp echo data */

	uint32_t	ts_recent_age;		/* when last updated */
	tcp_seq	last_ack_sent;
/* RFC 1644 variables */
	tcp_cc	cc_send;		/* send connection count */
	tcp_cc	cc_recv;		/* receive connection count */
};

/*
 * Structure to hold TCP options that are only used during segment
 * processing (in tcp_input), but not held in the tcpcb.
 * It's basically used to reduce the number of parameters
 * to tcp_dooptions.
 */
struct tcpopt {
	uint32_t	to_flag;		/* which options are present */
#define TOF_TS		0x0001		/* timestamp */
#define TOF_CC		0x0002		/* CC and CCnew are exclusive */
#define TOF_CCNEW	0x0004
#define	TOF_CCECHO	0x0008
	uint32_t	to_tsval;
	uint32_t	to_tsecr;
	tcp_cc	to_cc;		/* holds CC or CCnew */
	tcp_cc	to_ccecho;
};

/*
 * The TAO cache entry which is stored in the protocol family specific
 * portion of the route metrics.
 */
struct rmxp_tao {
	tcp_cc	tao_cc;			/* latest CC in valid SYN */
	tcp_cc	tao_ccsent;		/* latest CC sent to peer */
	u_short	tao_mssopt;		/* peer's cached MSS */
#ifdef notyet
	u_short	tao_flags;		/* cache status flags */
#define	TAOF_DONT	0x0001		/* peer doesn't understand rfc1644 */
#define	TAOF_OK		0x0002		/* peer does understand rfc1644 */
#define	TAOF_UNDEF	0		/* we don't know yet */
#endif /* notyet */
};
#define rmx_taop(r)	((struct rmxp_tao *)(r).rmx_filler)

#define	intotcpcb(ip)	((struct tcpcb *)(ip)->inp_ppcb)
#define	sototcpcb(so)	(intotcpcb(sotoinpcb(so)))

/*
 * The smoothed round-trip time and estimated variance
 * are stored as fixed point numbers scaled by the values below.
 * For convenience, these scales are also used in smoothing the average
 * (smoothed = (1/scale)sample + ((scale-1)/scale)smoothed).
 * With these scales, srtt has 3 bits to the right of the binary point,
 * and thus an "ALPHA" of 0.875.  rttvar has 2 bits to the right of the
 * binary point, and is smoothed with an ALPHA of 0.75.
 */
#define	TCP_RTT_SCALE		32	/* multiplier for srtt; 3 bits frac. */
#define	TCP_RTT_SHIFT		5	/* shift for srtt; 3 bits frac. */
#define	TCP_RTTVAR_SCALE	16	/* multiplier for rttvar; 2 bits */
#define	TCP_RTTVAR_SHIFT	4	/* shift for rttvar; 2 bits */
#define	TCP_DELTA_SHIFT		2	/* see tcp_input.c */

/*
 * The initial retransmission should happen at rtt + 4 * rttvar.
 * Because of the way we do the smoothing, srtt and rttvar
 * will each average +1/2 tick of bias.  When we compute
 * the retransmit timer, we want 1/2 tick of rounding and
 * 1 extra tick because of +-1/2 tick uncertainty in the
 * firing of the timer.  The bias will give us exactly the
 * 1.5 tick we need.  But, because the bias is
 * statistical, we have to test that we don't drop below
 * the minimum feasible timer (which is 2 ticks).
 * This version of the macro adapted from a paper by Lawrence
 * Brakmo and Larry Peterson which outlines a problem caused
 * by insufficient precision in the original implementation,
 * which results in inappropriately large RTO values for very
 * fast networks.
 */
#define	TCP_REXMTVAL(tp) \
	max((tp)->t_rttmin, (((tp)->t_srtt >> (TCP_RTT_SHIFT - TCP_DELTA_SHIFT))  \
	  + (tp)->t_rttvar) >> TCP_DELTA_SHIFT)

/*
 * TCP statistics.
 * Many of these should be kept per connection,
 * but that's inconvenient at the moment.
 */
struct	tcpstat {
	uint32_t	tcps_connattempt;	/* connections initiated */
	uint32_t	tcps_accepts;		/* connections accepted */
	uint32_t	tcps_connects;		/* connections established */
	uint32_t	tcps_drops;		/* connections dropped */
	uint32_t	tcps_conndrops;		/* embryonic connections dropped */
	uint32_t	tcps_closed;		/* conn. closed (includes drops) */
	uint32_t	tcps_segstimed;		/* segs where we tried to get rtt */
	uint32_t	tcps_rttupdated;	/* times we succeeded */
	uint32_t	tcps_delack;		/* delayed acks sent */
	uint32_t	tcps_timeoutdrop;	/* conn. dropped in rxmt timeout */
	uint32_t	tcps_rexmttimeo;	/* retransmit timeouts */
	uint32_t	tcps_persisttimeo;	/* persist timeouts */
	uint32_t	tcps_keeptimeo;		/* keepalive timeouts */
	uint32_t	tcps_keepprobe;		/* keepalive probes sent */
	uint32_t	tcps_keepdrops;		/* connections dropped in keepalive */

	uint32_t	tcps_sndtotal;		/* total packets sent */
	uint32_t	tcps_sndpack;		/* data packets sent */
	uint32_t	tcps_sndbyte;		/* data bytes sent */
	uint32_t	tcps_sndrexmitpack;	/* data packets retransmitted */
	uint32_t	tcps_sndrexmitbyte;	/* data bytes retransmitted */
	uint32_t	tcps_sndacks;		/* ack-only packets sent */
	uint32_t	tcps_sndprobe;		/* window probes sent */
	uint32_t	tcps_sndurg;		/* packets sent with URG only */
	uint32_t	tcps_sndwinup;		/* window update-only packets sent */
	uint32_t	tcps_sndctrl;		/* control (SYN|FIN|RST) packets sent */

	uint32_t	tcps_rcvtotal;		/* total packets received */
	uint32_t	tcps_rcvpack;		/* packets received in sequence */
	uint32_t	tcps_rcvbyte;		/* bytes received in sequence */
	uint32_t	tcps_rcvbadsum;		/* packets received with ccksum errs */
	uint32_t	tcps_rcvbadoff;		/* packets received with bad offset */
	uint32_t	tcps_rcvshort;		/* packets received too short */
	uint32_t	tcps_rcvduppack;	/* duplicate-only packets received */
	uint32_t	tcps_rcvdupbyte;	/* duplicate-only bytes received */
	uint32_t	tcps_rcvpartduppack;	/* packets with some duplicate data */
	uint32_t	tcps_rcvpartdupbyte;	/* dup. bytes in part-dup. packets */
	uint32_t	tcps_rcvoopack;		/* out-of-order packets received */
	uint32_t	tcps_rcvoobyte;		/* out-of-order bytes received */
	uint32_t	tcps_rcvpackafterwin;	/* packets with data after window */
	uint32_t	tcps_rcvbyteafterwin;	/* bytes rcvd after window */
	uint32_t	tcps_rcvafterclose;	/* packets rcvd after "close" */
	uint32_t	tcps_rcvwinprobe;	/* rcvd window probe packets */
	uint32_t	tcps_rcvdupack;		/* rcvd duplicate acks */
	uint32_t	tcps_rcvacktoomuch;	/* rcvd acks for unsent data */
	uint32_t	tcps_rcvackpack;	/* rcvd ack packets */
	uint32_t	tcps_rcvackbyte;	/* bytes acked by rcvd acks */
	uint32_t	tcps_rcvwinupd;		/* rcvd window update packets */
	uint32_t	tcps_pawsdrop;		/* segments dropped due to PAWS */
	uint32_t	tcps_predack;		/* times hdr predict ok for acks */
	uint32_t	tcps_preddat;		/* times hdr predict ok for data pkts */
	uint32_t	tcps_pcbcachemiss;
	uint32_t	tcps_cachedrtt;		/* times cached RTT in route updated */
	uint32_t	tcps_cachedrttvar;	/* times cached rttvar updated */
	uint32_t	tcps_cachedssthresh;	/* times cached ssthresh updated */
	uint32_t	tcps_usedrtt;		/* times RTT initialized from route */
	uint32_t	tcps_usedrttvar;	/* times RTTVAR initialized from rt */
	uint32_t	tcps_usedssthresh;	/* times ssthresh initialized from rt*/
	uint32_t	tcps_persistdrop;	/* timeout in persist state */
	uint32_t	tcps_badsyn;		/* bogus SYN, e.g. premature ACK */
	uint32_t	tcps_mturesent;		/* resends due to MTU discovery */
	uint32_t	tcps_listendrop;	/* listen queue overflows */
};

/*
 * TCB structure exported to user-land via sysctl(3).
 * Evil hack: declare only if in_pcb.h and sys/socketvar.h have been
 * included.  Not all of our clients do.
 */
#if defined(_NETINET_IN_PCB_H_) && defined(_SYS_SOCKETVAR_H_)
struct	xtcpcb {
	size_t	xt_len;
	struct	inpcb	xt_inp;
	struct	tcpcb	xt_tp;
	struct	xsocket	xt_socket;
	u_quad_t	xt_alignment_hack;
};
#endif

/*
 * Names for TCP sysctl objects
 */
#define	TCPCTL_DO_RFC1323	1	/* use RFC-1323 extensions */
#define	TCPCTL_DO_RFC1644	2	/* use RFC-1644 extensions */
#define	TCPCTL_MSSDFLT		3	/* MSS default */
#define TCPCTL_STATS		4	/* statistics (read-only) */
#define	TCPCTL_RTTDFLT		5	/* default RTT estimate */
#define	TCPCTL_KEEPIDLE		6	/* keepalive idle timer */
#define	TCPCTL_KEEPINTVL	7	/* interval to send keepalives */
#define	TCPCTL_SENDSPACE	8	/* send buffer space */
#define	TCPCTL_RECVSPACE	9	/* receive buffer space */
#define	TCPCTL_KEEPINIT		10	/* receive buffer space */
#define	TCPCTL_PCBLIST		11	/* list of all outstanding PCBs */
#define TCPCTL_MAXID		12

#define TCPCTL_NAMES { \
	{ 0, 0 }, \
	{ "rfc1323", CTLTYPE_INT }, \
	{ "rfc1644", CTLTYPE_INT }, \
	{ "mssdflt", CTLTYPE_INT }, \
	{ "stats", CTLTYPE_STRUCT }, \
	{ "rttdflt", CTLTYPE_INT }, \
	{ "keepidle", CTLTYPE_INT }, \
	{ "keepintvl", CTLTYPE_INT }, \
	{ "sendspace", CTLTYPE_INT }, \
	{ "recvspace", CTLTYPE_INT }, \
	{ "keepinit", CTLTYPE_INT }, \
	{ "pcblist", CTLTYPE_STRUCT }, \
}

#ifdef KERNEL
extern	struct inpcbhead tcb;		/* head of queue of active tcpcb's */
extern	struct inpcbinfo tcbinfo;
extern	struct tcpstat tcpstat;	/* tcp statistics */
extern	int tcp_mssdflt;	/* XXX */
extern	uint32_t tcp_now;		/* for RFC 1323 timestamps */
extern	int tcp_delack_enabled;

void	 tcp_canceltimers __P((struct tcpcb *));
struct tcpcb *
	 tcp_close __P((struct tcpcb *));
void	 tcp_ctlinput __P((int, struct sockaddr *, void *));
int	 tcp_ctloutput __P((struct socket *, struct sockopt *));
struct tcpcb *
	 tcp_drop __P((struct tcpcb *, int));
void	 tcp_drain __P((void));
void	 tcp_fasttimo __P((void));
struct rmxp_tao *
	 tcp_gettaocache __P((struct inpcb *));
void	 tcp_init __P((void));
void	 tcp_input __P((struct mbuf *, int));
void	 tcp_mss __P((struct tcpcb *, int));
int	 tcp_mssopt __P((struct tcpcb *));
void	 tcp_mtudisc __P((struct inpcb *, int));
struct tcpcb *
	 tcp_newtcpcb __P((struct inpcb *));
int	 tcp_output __P((struct tcpcb *));
void	 tcp_quench __P((struct inpcb *, int));
void	 tcp_respond __P((struct tcpcb *,
	    struct tcpiphdr *, struct mbuf *, tcp_seq, tcp_seq, int));
struct rtentry *
	 tcp_rtlookup __P((struct inpcb *));
void	 tcp_setpersist __P((struct tcpcb *));
void	 tcp_slowtimo __P((void));
struct tcpiphdr *
	 tcp_template __P((struct tcpcb *));
struct tcpcb *
	 tcp_timers __P((struct tcpcb *, int));
void	 tcp_trace __P((int, int, struct tcpcb *, struct tcpiphdr *, int));

extern	struct pr_usrreqs tcp_usrreqs;
extern	uint32_t tcp_sendspace;
extern	uint32_t tcp_recvspace;

#endif /* KERNEL */

#endif /* _NETINET_TCP_VAR_H_ */
