/* get_myaddr.h - header file for get_myaddr.c */

/* Copyright 1998-1999 NIISI RAN. */

#ifndef __INCget_myaddrh
#define __INCget_myaddrh

#include <sys/cdefs.h>
#include <netinet/in.h>

/* function declarations */
__BEGIN_DECLS

extern	  int   get_myaddress __P((struct sockaddr_in *addr));

__END_DECLS

#endif /* __INCget_myaddrh */
