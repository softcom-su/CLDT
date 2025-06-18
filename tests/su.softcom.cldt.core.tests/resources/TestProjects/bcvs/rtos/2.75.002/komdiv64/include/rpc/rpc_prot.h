/* rpc_prot.h - header file for rpc_prot.c */

/* Copyright 1998-1999 NIISI RAN. */

#ifndef __INCrpc_proth
#define __INCrpc_proth

#include <sys/cdefs.h>

/* function declarations */
__BEGIN_DECLS

extern	  bool_t       xdr_opaque_auth __P((XDR *xdrs, struct opaque_auth *ap));

__END_DECLS

#endif /* __INCrpc_proth */
