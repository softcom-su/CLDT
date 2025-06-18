/*
 * Copyright (c) 1989, 1993
 * The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Karels at Berkeley Software Design, Inc.
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
 * This product includes software developed by the University of
 * California, Berkeley and its contributors.
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
 * @(#)sysctl.h   8.1 (Berkeley) 6/2/93
 * $FreeBSD: src/sys/sys/sysctl.h,v 1.70.2.1 1999/08/29 16:32:46 peter Exp $
 */

#ifndef _SYS_SYSCTL_H_
#define  _SYS_SYSCTL_H_

#include <sys/_posix.h>

/*
 * Definitions for sysctl call.  The sysctl call uses a hierarchical name
 * for objects that can be examined or modified.  The name is expressed as
 * a sequence of integers.  Like a file path name, the meaning of each
 * component depends on its place in the hierarchy.  The top-level and kern
 * identifiers are defined here, and other identifiers are defined in the
 * respective subsystem header files.
 */

#define CTL_MAXNAME  12 /* largest number of components supported */

/*
 * Each subsystem defined by sysctl defines a list of variables
 * for that subsystem. Each name is either a node with further
 * levels defined below it, or it is a leaf of some particular
 * type given below. Each sysctl level defines a set of name/type
 * pairs to be used by sysctl(1) in manipulating the subsystem.
 */
struct ctlname {
   char  *ctl_name;  /* subsystem name */
   int   ctl_type;   /* type of name */
};

#define CTLTYPE      0xf   /* Mask for the type */
#define  CTLTYPE_NODE   1  /* name is a node */
#define  CTLTYPE_INT 2  /* name describes an integer */
#define  CTLTYPE_STRING 3  /* name describes a string */
#define  CTLTYPE_QUAD   4  /* name describes a 64-bit number */
#define  CTLTYPE_OPAQUE 5  /* name describes a structure */
#define  CTLTYPE_STRUCT CTLTYPE_OPAQUE /* name describes a structure */

#define CTLFLAG_RD   0x80000000  /* Allow reads of variable */
#define CTLFLAG_WR   0x40000000  /* Allow writes to the variable */
#define CTLFLAG_RW   (CTLFLAG_RD|CTLFLAG_WR)
#define CTLFLAG_NOLOCK  0x20000000  /* XXX Don't Lock */
#define CTLFLAG_ANYBODY 0x10000000  /* All users can set this var */
#define CTLFLAG_SECURE  0x08000000  /* Permit set only if securelevel<=0 */


/*
 * USE THIS instead of a hardwired number from the categories below
 * to get dynamically assigned sysctl entries using the linker-set
 * technology. This is the way nearly all new sysctl variables should
 * be implemented.
 * e.g. SYSCTL_INT(_parent, OID_AUTO, name, CTLFLAG_RW, &variable, 0, "");
 */ 
#define OID_AUTO  (-1)

#ifdef KERNEL

#define SYSCTL_HANDLER_ARGS (struct sysctl_oid *oidp, void *arg1, int arg2, \
   struct sysctl_req *req)

/*
 * This describes the access space for a sysctl request.  This is needed
 * so that we can use the interface from the kernel or from user-space.
 */
struct sysctl_req {
   struct proc *p;
   int      lock;
   void     *oldptr;
   size_t      oldlen;
   size_t      oldidx;
   int      (*oldfunc)(struct sysctl_req *, const void *, size_t);
   void     *newptr;
   size_t      newlen;
   size_t      newidx;
   int      (*newfunc)(struct sysctl_req *, void *, size_t);
};

/*
 * This describes one "oid" in the MIB tree.  Potentially more nodes can
 * be hidden behind it, expanded by the handler.
 */
struct sysctl_oid {
   int      oid_number;
   int      oid_kind;
   void     *oid_arg1;
   int      oid_arg2;
   const char  *oid_name;
   int      (*oid_handler) SYSCTL_HANDLER_ARGS;
   const char  *oid_fmt;
};


int sysctl_handle_opaque SYSCTL_HANDLER_ARGS;

#define  KIPC_MBSTAT    8  /* struct: mbuf usage statistics */


#define SYSCTL_IN(r, p, l) (r->newfunc)(r, p, l)
#define SYSCTL_OUT(r, p, l) (r->oldfunc)(r, p, l)

#define SYSCTL_DECL(a)


/* This constructs a "raw" MIB oid. */
#define SYSCTL_OID(parent, nbr, name, kind, a1, a2, handler, fmt, descr) \
   static struct sysctl_oid sysctl__##parent##_##name = {       \
      nbr, kind, a1, a2, #parent "_" #name, handler, fmt };     \
   DATA_SET(sysctl_set, sysctl__##parent##_##name)

/* Oid for a procedure.  Specified by a pointer and an arg. */
#define SYSCTL_PROC(parent, nbr, name, access, ptr, arg, handler, fmt, descr) \
   SYSCTL_OID(parent, nbr, name, access, \
      ptr, arg, handler, fmt, descr)

/* Oid for a struct.  Specified by a pointer and a type. */
#define SYSCTL_STRUCT(parent, nbr, name, access, ptr, type, descr) \
   SYSCTL_OID(parent, nbr, name, CTLTYPE_OPAQUE|access, \
      ptr, sizeof(struct type), sysctl_handle_opaque, \
      "S," #type, descr)

/* This constructs a node from which other oids can hang. */
#define SYSCTL_NODE(parent, nbr, name, access, handler, descr)        \
   SYSCTL_OID(parent, nbr, name, CTLTYPE_NODE|access,        \
         (void*)0, 0, handler, \
         "N", descr);

#ifdef __GNUC__
#if __GNUC__ >= 3
#define DATA_SET(set, var) static void *__##var  __attribute__ ((section ("."#set),unused,used)) = &var
#else
#define DATA_SET(set, var) static void *__##var  __attribute__ ((section ("."#set),unused)) = &var
#endif
#else   /* __GNUC __ */
#pragma data_seg("sysctl_set")
#pragma data_seg("domain_set")
#pragma data_seg("netisr_set")
#pragma data_seg()
#define DATA_SET(set, var) __declspec(allocate(#set)) static void *__##var = &var
#endif



/*
 * Top-level identifiers
 */
#define  CTL_UNSPEC  0     /* unused */
#define  CTL_KERN 1        /* "high kernel": proc, limits */
#define  CTL_VM      2     /* virtual memory */
#define  CTL_VFS     3     /* file system, mount type is next */
#define  CTL_NET     4     /* network, see socket.h */
#define  CTL_DEBUG   5     /* debugging parameters */
#define  CTL_HW      6     /* generic cpu/io */
#define  CTL_MACHDEP 7     /* machine dependent */
#define  CTL_USER 8        /* user-level */
#define  CTL_P1003_1B   9  /* POSIX 1003.1B */
#define  CTL_MAXID   10    /* number of valid top-level ids */

#define CTL_NAMES { \
   { 0, 0 }, \
   { "kern", CTLTYPE_NODE }, \
   { "vm", CTLTYPE_NODE }, \
   { "vfs", CTLTYPE_NODE }, \
   { "net", CTLTYPE_NODE }, \
   { "debug", CTLTYPE_NODE }, \
   { "hw", CTLTYPE_NODE }, \
   { "machdep", CTLTYPE_NODE }, \
   { "user", CTLTYPE_NODE }, \
   { "p1003_1b", CTLTYPE_NODE }, \
}
#define SYSCTL_INT(parent, nbr, name, access, ptr, val, descr) 

#else /* !KERNEL */
#include <sys/cdefs.h>

__BEGIN_DECLS
int   sysctl __P((int *, u_int, void *, size_t *, void *, size_t));
int   sysctlbyname __P((const char *, void *, size_t *, void *, size_t));
__END_DECLS
#endif   /* KERNEL */
#endif   /* !_SYS_SYSCTL_H_ */
