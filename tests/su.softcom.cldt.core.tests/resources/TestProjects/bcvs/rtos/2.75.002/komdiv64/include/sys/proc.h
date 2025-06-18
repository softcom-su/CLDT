#ifndef _SYS_PROC_H
#define _SYS_PROC_H
#include <sys/ucred.h>
/*
 * MOVE TO ucred.h?
 *
 * Shareable process credentials (always resident).  This includes a reference
 * to the current user credentials as well as real and saved ids that may be
 * used to change ids.
 */
struct   pcred {
   struct   ucred *pc_ucred;  /* Current credentials. */
   uid_t p_ruid;        /* Real user id. */
   uid_t p_svuid;    /* Saved effective user id. */
   gid_t p_rgid;        /* Real group id. */
   gid_t p_svgid;    /* Saved effective group id. */
   int   p_refcnt;      /* Number of references. */
};
struct filedesc
{
   struct vnode *fd_rdir;
   struct vnode *fd_cdir;
   mode_t fd_cmask;
};
struct   proc 
{
   /* substructures: */
   struct   pcred *p_cred;    /* Process owner's identity. */
   struct   filedesc *p_fd;      /* Ptr to open files structure. */
   pid_t p_pid;         /* Process identifier. */
#define  p_ucred     p_cred->pc_ucred
   int   p_flags;
#define procFLAGS_FEINIT 0x1
};
extern struct proc *curproc;
extern struct proc proc0;
#endif
