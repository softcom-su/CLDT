/* portmap.h - header file for portmap.c */

/* Copyright 1998-1999 NIISI RAN. */

#ifndef __INCrpc_portmaph
#define __INCrpc_portmaph

#include  <rpc/rpc.h>

/* function declarations */
__BEGIN_DECLS

extern	 void   portmapd (void);
extern	 void	  reg_service (struct svc_req *rqstp, SVCXPRT *xprt);

__END_DECLS

#endif /* __INCrpc_portmaph */
