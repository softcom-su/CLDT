/*
 * This file is produced automatically.
 * Do not modify anything in here by hand.
 *
 * Created from @(#)vnode_if.sh	8.1 (Berkeley) 6/10/93
 */

extern struct vnodeop_desc vop_default_desc;
struct vop_lookup_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_lookup_desc;
static int VOP_LOOKUP __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp));
static __inline int VOP_LOOKUP(dvp, vpp, cnp)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
{
	struct vop_lookup_args a;

	a.a_desc = VDESC(vop_lookup);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	return (VCALL(dvp, VOFFSET(vop_lookup), &a));
}
struct vop_cachedlookup_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_cachedlookup_desc;
static int VOP_CACHEDLOOKUP __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp));
static __inline int VOP_CACHEDLOOKUP(dvp, vpp, cnp)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
{
	struct vop_cachedlookup_args a;

	a.a_desc = VDESC(vop_cachedlookup);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	return (VCALL(dvp, VOFFSET(vop_cachedlookup), &a));
}
struct vop_create_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern struct vnodeop_desc vop_create_desc;
static int VOP_CREATE __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap));
static __inline int VOP_CREATE(dvp, vpp, cnp, vap)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
	struct vattr *vap;
{
	struct vop_create_args a;

	a.a_desc = VDESC(vop_create);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VCALL(dvp, VOFFSET(vop_create), &a));
}
struct vop_whiteout_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
	int a_flags;
};
extern struct vnodeop_desc vop_whiteout_desc;
static int VOP_WHITEOUT __P((
	struct vnode *dvp,
	struct componentname *cnp,
	int flags));
static __inline int VOP_WHITEOUT(dvp, cnp, flags)
	struct vnode *dvp;
	struct componentname *cnp;
	int flags;
{
	struct vop_whiteout_args a;

	a.a_desc = VDESC(vop_whiteout);
	a.a_dvp = dvp;
	a.a_cnp = cnp;
	a.a_flags = flags;
	return (VCALL(dvp, VOFFSET(vop_whiteout), &a));
}
struct vop_mknod_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern struct vnodeop_desc vop_mknod_desc;
static int VOP_MKNOD __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap));
static __inline int VOP_MKNOD(dvp, vpp, cnp, vap)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
	struct vattr *vap;
{
	struct vop_mknod_args a;

	a.a_desc = VDESC(vop_mknod);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VCALL(dvp, VOFFSET(vop_mknod), &a));
}
struct vop_open_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_mode;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_open_desc;
static int VOP_OPEN __P((
	struct vnode *vp,
	int mode,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_OPEN(vp, mode, cred, p)
	struct vnode *vp;
	int mode;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_open_args a;

	a.a_desc = VDESC(vop_open);
	a.a_vp = vp;
	a.a_mode = mode;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_open), &a));
}
struct vop_close_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_fflag;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_close_desc;
static int VOP_CLOSE __P((
	struct vnode *vp,
	int fflag,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_CLOSE(vp, fflag, cred, p)
	struct vnode *vp;
	int fflag;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_close_args a;

	a.a_desc = VDESC(vop_close);
	a.a_vp = vp;
	a.a_fflag = fflag;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_close), &a));
}
struct vop_access_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_mode;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_access_desc;
static int VOP_ACCESS __P((
	struct vnode *vp,
	int mode,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_ACCESS(vp, mode, cred, p)
	struct vnode *vp;
	int mode;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_access_args a;

	a.a_desc = VDESC(vop_access);
	a.a_vp = vp;
	a.a_mode = mode;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_access), &a));
}
struct vop_getattr_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct vattr *a_vap;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_getattr_desc;
static int VOP_GETATTR __P((
	struct vnode *vp,
	struct vattr *vap,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_GETATTR(vp, vap, cred, p)
	struct vnode *vp;
	struct vattr *vap;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_getattr_args a;

	a.a_desc = VDESC(vop_getattr);
	a.a_vp = vp;
	a.a_vap = vap;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_getattr), &a));
}
struct vop_setattr_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct vattr *a_vap;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_setattr_desc;
static int VOP_SETATTR __P((
	struct vnode *vp,
	struct vattr *vap,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_SETATTR(vp, vap, cred, p)
	struct vnode *vp;
	struct vattr *vap;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_setattr_args a;

	a.a_desc = VDESC(vop_setattr);
	a.a_vp = vp;
	a.a_vap = vap;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_setattr), &a));
}
struct vop_read_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	struct ucred *a_cred;
};
extern struct vnodeop_desc vop_read_desc;
static int VOP_READ __P((
	struct vnode *vp,
	struct uio *uio,
	int ioflag,
	struct ucred *cred));
static __inline int VOP_READ(vp, uio, ioflag, cred)
	struct vnode *vp;
	struct uio *uio;
	int ioflag;
	struct ucred *cred;
{
	struct vop_read_args a;

	a.a_desc = VDESC(vop_read);
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VCALL(vp, VOFFSET(vop_read), &a));
}
struct vop_write_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	struct ucred *a_cred;
};
extern struct vnodeop_desc vop_write_desc;
static int VOP_WRITE __P((
	struct vnode *vp,
	struct uio *uio,
	int ioflag,
	struct ucred *cred));
static __inline int VOP_WRITE(vp, uio, ioflag, cred)
	struct vnode *vp;
	struct uio *uio;
	int ioflag;
	struct ucred *cred;
{
	struct vop_write_args a;

	a.a_desc = VDESC(vop_write);
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VCALL(vp, VOFFSET(vop_write), &a));
}
struct vop_lease_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct proc *a_p;
	struct ucred *a_cred;
	int a_flag;
};
extern struct vnodeop_desc vop_lease_desc;
static int VOP_LEASE __P((
	struct vnode *vp,
	struct proc *p,
	struct ucred *cred,
	int flag));
static __inline int VOP_LEASE(vp, p, cred, flag)
	struct vnode *vp;
	struct proc *p;
	struct ucred *cred;
	int flag;
{
	struct vop_lease_args a;

	a.a_desc = VDESC(vop_lease);
	a.a_vp = vp;
	a.a_p = p;
	a.a_cred = cred;
	a.a_flag = flag;
	return (VCALL(vp, VOFFSET(vop_lease), &a));
}
struct vop_ioctl_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	u_long a_command;
	caddr_t a_data;
	int a_fflag;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_ioctl_desc;
static int VOP_IOCTL __P((
	struct vnode *vp,
	u_long command,
	caddr_t data,
	int fflag,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_IOCTL(vp, command, data, fflag, cred, p)
	struct vnode *vp;
	u_long command;
	caddr_t data;
	int fflag;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_ioctl_args a;

	a.a_desc = VDESC(vop_ioctl);
	a.a_vp = vp;
	a.a_command = command;
	a.a_data = data;
	a.a_fflag = fflag;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_ioctl), &a));
}
struct vop_poll_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_events;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_poll_desc;
static int VOP_POLL __P((
	struct vnode *vp,
	int events,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_POLL(vp, events, cred, p)
	struct vnode *vp;
	int events;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_poll_args a;

	a.a_desc = VDESC(vop_poll);
	a.a_vp = vp;
	a.a_events = events;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_poll), &a));
}
struct vop_revoke_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_flags;
};
extern struct vnodeop_desc vop_revoke_desc;
static int VOP_REVOKE __P((
	struct vnode *vp,
	int flags));
static __inline int VOP_REVOKE(vp, flags)
	struct vnode *vp;
	int flags;
{
	struct vop_revoke_args a;

	a.a_desc = VDESC(vop_revoke);
	a.a_vp = vp;
	a.a_flags = flags;
	return (VCALL(vp, VOFFSET(vop_revoke), &a));
}
struct vop_mmap_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_fflags;
	struct ucred *a_cred;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_mmap_desc;
static int VOP_MMAP __P((
	struct vnode *vp,
	int fflags,
	struct ucred *cred,
	struct proc *p));
static __inline int VOP_MMAP(vp, fflags, cred, p)
	struct vnode *vp;
	int fflags;
	struct ucred *cred;
	struct proc *p;
{
	struct vop_mmap_args a;

	a.a_desc = VDESC(vop_mmap);
	a.a_vp = vp;
	a.a_fflags = fflags;
	a.a_cred = cred;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_mmap), &a));
}
struct vop_fsync_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct ucred *a_cred;
	int a_waitfor;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_fsync_desc;
static int VOP_FSYNC __P((
	struct vnode *vp,
	struct ucred *cred,
	int waitfor,
	struct proc *p));
static __inline int VOP_FSYNC(vp, cred, waitfor, p)
	struct vnode *vp;
	struct ucred *cred;
	int waitfor;
	struct proc *p;
{
	struct vop_fsync_args a;

	a.a_desc = VDESC(vop_fsync);
	a.a_vp = vp;
	a.a_cred = cred;
	a.a_waitfor = waitfor;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_fsync), &a));
}
struct vop_remove_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_remove_desc;
static int VOP_REMOVE __P((
	struct vnode *dvp,
	struct vnode *vp,
	struct componentname *cnp));
static __inline int VOP_REMOVE(dvp, vp, cnp)
	struct vnode *dvp;
	struct vnode *vp;
	struct componentname *cnp;
{
	struct vop_remove_args a;

	a.a_desc = VDESC(vop_remove);
	a.a_dvp = dvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VCALL(dvp, VOFFSET(vop_remove), &a));
}
struct vop_link_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_tdvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_link_desc;
static int VOP_LINK __P((
	struct vnode *tdvp,
	struct vnode *vp,
	struct componentname *cnp));
static __inline int VOP_LINK(tdvp, vp, cnp)
	struct vnode *tdvp;
	struct vnode *vp;
	struct componentname *cnp;
{
	struct vop_link_args a;

	a.a_desc = VDESC(vop_link);
	a.a_tdvp = tdvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VCALL(tdvp, VOFFSET(vop_link), &a));
}
struct vop_rename_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_fdvp;
	struct vnode *a_fvp;
	struct componentname *a_fcnp;
	struct vnode *a_tdvp;
	struct vnode *a_tvp;
	struct componentname *a_tcnp;
};
extern struct vnodeop_desc vop_rename_desc;
static int VOP_RENAME __P((
	struct vnode *fdvp,
	struct vnode *fvp,
	struct componentname *fcnp,
	struct vnode *tdvp,
	struct vnode *tvp,
	struct componentname *tcnp));
static __inline int VOP_RENAME(fdvp, fvp, fcnp, tdvp, tvp, tcnp)
	struct vnode *fdvp;
	struct vnode *fvp;
	struct componentname *fcnp;
	struct vnode *tdvp;
	struct vnode *tvp;
	struct componentname *tcnp;
{
	struct vop_rename_args a;

	a.a_desc = VDESC(vop_rename);
	a.a_fdvp = fdvp;
	a.a_fvp = fvp;
	a.a_fcnp = fcnp;
	a.a_tdvp = tdvp;
	a.a_tvp = tvp;
	a.a_tcnp = tcnp;
	return (VCALL(fdvp, VOFFSET(vop_rename), &a));
}
struct vop_mkdir_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern struct vnodeop_desc vop_mkdir_desc;
static int VOP_MKDIR __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap));
static __inline int VOP_MKDIR(dvp, vpp, cnp, vap)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
	struct vattr *vap;
{
	struct vop_mkdir_args a;

	a.a_desc = VDESC(vop_mkdir);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VCALL(dvp, VOFFSET(vop_mkdir), &a));
}
struct vop_rmdir_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_rmdir_desc;
static int VOP_RMDIR __P((
	struct vnode *dvp,
	struct vnode *vp,
	struct componentname *cnp));
static __inline int VOP_RMDIR(dvp, vp, cnp)
	struct vnode *dvp;
	struct vnode *vp;
	struct componentname *cnp;
{
	struct vop_rmdir_args a;

	a.a_desc = VDESC(vop_rmdir);
	a.a_dvp = dvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VCALL(dvp, VOFFSET(vop_rmdir), &a));
}
struct vop_symlink_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
	char *a_target;
};
extern struct vnodeop_desc vop_symlink_desc;
static int VOP_SYMLINK __P((
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap,
	char *target));
static __inline int VOP_SYMLINK(dvp, vpp, cnp, vap, target)
	struct vnode *dvp;
	struct vnode **vpp;
	struct componentname *cnp;
	struct vattr *vap;
	char *target;
{
	struct vop_symlink_args a;

	a.a_desc = VDESC(vop_symlink);
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	a.a_target = target;
	return (VCALL(dvp, VOFFSET(vop_symlink), &a));
}
struct vop_readdir_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	struct ucred *a_cred;
	int *a_eofflag;
	int *a_ncookies;
	u_long **a_cookies;
};
extern struct vnodeop_desc vop_readdir_desc;
static int VOP_READDIR __P((
	struct vnode *vp,
	struct uio *uio,
	struct ucred *cred,
	int *eofflag,
	int *ncookies,
	u_long **cookies));
static __inline int VOP_READDIR(vp, uio, cred, eofflag, ncookies, cookies)
	struct vnode *vp;
	struct uio *uio;
	struct ucred *cred;
	int *eofflag;
	int *ncookies;
	u_long **cookies;
{
	struct vop_readdir_args a;

	a.a_desc = VDESC(vop_readdir);
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_cred = cred;
	a.a_eofflag = eofflag;
	a.a_ncookies = ncookies;
	a.a_cookies = cookies;
	return (VCALL(vp, VOFFSET(vop_readdir), &a));
}
struct vop_readlink_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	struct ucred *a_cred;
};
extern struct vnodeop_desc vop_readlink_desc;
static int VOP_READLINK __P((
	struct vnode *vp,
	struct uio *uio,
	struct ucred *cred));
static __inline int VOP_READLINK(vp, uio, cred)
	struct vnode *vp;
	struct uio *uio;
	struct ucred *cred;
{
	struct vop_readlink_args a;

	a.a_desc = VDESC(vop_readlink);
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_cred = cred;
	return (VCALL(vp, VOFFSET(vop_readlink), &a));
}
struct vop_abortop_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
};
extern struct vnodeop_desc vop_abortop_desc;
static int VOP_ABORTOP __P((
	struct vnode *dvp,
	struct componentname *cnp));
static __inline int VOP_ABORTOP(dvp, cnp)
	struct vnode *dvp;
	struct componentname *cnp;
{
	struct vop_abortop_args a;

	a.a_desc = VDESC(vop_abortop);
	a.a_dvp = dvp;
	a.a_cnp = cnp;
	return (VCALL(dvp, VOFFSET(vop_abortop), &a));
}
struct vop_inactive_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_inactive_desc;
static int VOP_INACTIVE __P((
	struct vnode *vp,
	struct proc *p));
static __inline int VOP_INACTIVE(vp, p)
	struct vnode *vp;
	struct proc *p;
{
	struct vop_inactive_args a;

	a.a_desc = VDESC(vop_inactive);
	a.a_vp = vp;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_inactive), &a));
}
struct vop_reclaim_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_reclaim_desc;
static int VOP_RECLAIM __P((
	struct vnode *vp,
	struct proc *p));
static __inline int VOP_RECLAIM(vp, p)
	struct vnode *vp;
	struct proc *p;
{
	struct vop_reclaim_args a;

	a.a_desc = VDESC(vop_reclaim);
	a.a_vp = vp;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_reclaim), &a));
}
struct vop_lock_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_flags;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_lock_desc;
static int VOP_LOCK __P((
	struct vnode *vp,
	int flags,
	struct proc *p));
static __inline int VOP_LOCK(vp, flags, p)
	struct vnode *vp;
	int flags;
	struct proc *p;
{
	struct vop_lock_args a;

	a.a_desc = VDESC(vop_lock);
	a.a_vp = vp;
	a.a_flags = flags;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_lock), &a));
}
struct vop_unlock_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_flags;
	struct proc *a_p;
};
extern struct vnodeop_desc vop_unlock_desc;
static int VOP_UNLOCK __P((
	struct vnode *vp,
	int flags,
	struct proc *p));
static __inline int VOP_UNLOCK(vp, flags, p)
	struct vnode *vp;
	int flags;
	struct proc *p;
{
	struct vop_unlock_args a;

	a.a_desc = VDESC(vop_unlock);
	a.a_vp = vp;
	a.a_flags = flags;
	a.a_p = p;
	return (VCALL(vp, VOFFSET(vop_unlock), &a));
}
struct vop_bmap_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	daddr_t a_bn;
	struct vnode **a_vpp;
	daddr_t *a_bnp;
	int *a_runp;
	int *a_runb;
};
extern struct vnodeop_desc vop_bmap_desc;
static int VOP_BMAP __P((
	struct vnode *vp,
	daddr_t bn,
	struct vnode **vpp,
	daddr_t *bnp,
	int *runp,
	int *runb));
static __inline int VOP_BMAP(vp, bn, vpp, bnp, runp, runb)
	struct vnode *vp;
	daddr_t bn;
	struct vnode **vpp;
	daddr_t *bnp;
	int *runp;
	int *runb;
{
	struct vop_bmap_args a;

	a.a_desc = VDESC(vop_bmap);
	a.a_vp = vp;
	a.a_bn = bn;
	a.a_vpp = vpp;
	a.a_bnp = bnp;
	a.a_runp = runp;
	a.a_runb = runb;
	return (VCALL(vp, VOFFSET(vop_bmap), &a));
}
struct vop_strategy_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct buf *a_bp;
};
extern struct vnodeop_desc vop_strategy_desc;
static int VOP_STRATEGY __P((
	struct vnode *vp,
	struct buf *bp));
static __inline int VOP_STRATEGY(vp, bp)
	struct vnode *vp;
	struct buf *bp;
{
	struct vop_strategy_args a;

	a.a_desc = VDESC(vop_strategy);
	a.a_vp = vp;
	a.a_bp = bp;
	return (VCALL(vp, VOFFSET(vop_strategy), &a));
}
struct vop_print_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern struct vnodeop_desc vop_print_desc;
static int VOP_PRINT __P((
	struct vnode *vp));
static __inline int VOP_PRINT(vp)
	struct vnode *vp;
{
	struct vop_print_args a;

	a.a_desc = VDESC(vop_print);
	a.a_vp = vp;
	return (VCALL(vp, VOFFSET(vop_print), &a));
}
struct vop_islocked_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern struct vnodeop_desc vop_islocked_desc;
static int VOP_ISLOCKED __P((
	struct vnode *vp));
static __inline int VOP_ISLOCKED(vp)
	struct vnode *vp;
{
	struct vop_islocked_args a;

	a.a_desc = VDESC(vop_islocked);
	a.a_vp = vp;
	return (VCALL(vp, VOFFSET(vop_islocked), &a));
}
struct vop_pathconf_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_name;
	register_t *a_retval;
};
extern struct vnodeop_desc vop_pathconf_desc;
static int VOP_PATHCONF __P((
	struct vnode *vp,
	int name,
	register_t *retval));
static __inline int VOP_PATHCONF(vp, name, retval)
	struct vnode *vp;
	int name;
	register_t *retval;
{
	struct vop_pathconf_args a;

	a.a_desc = VDESC(vop_pathconf);
	a.a_vp = vp;
	a.a_name = name;
	a.a_retval = retval;
	return (VCALL(vp, VOFFSET(vop_pathconf), &a));
}
struct vop_advlock_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	caddr_t a_id;
	int a_op;
	struct flock *a_fl;
	int a_flags;
};
extern struct vnodeop_desc vop_advlock_desc;
static int VOP_ADVLOCK __P((
	struct vnode *vp,
	caddr_t id,
	int op,
	struct flock *fl,
	int flags));
static __inline int VOP_ADVLOCK(vp, id, op, fl, flags)
	struct vnode *vp;
	caddr_t id;
	int op;
	struct flock *fl;
	int flags;
{
	struct vop_advlock_args a;

	a.a_desc = VDESC(vop_advlock);
	a.a_vp = vp;
	a.a_id = id;
	a.a_op = op;
	a.a_fl = fl;
	a.a_flags = flags;
	return (VCALL(vp, VOFFSET(vop_advlock), &a));
}
struct vop_balloc_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	off_t a_startoffset;
	int a_size;
	struct ucred *a_cred;
	int a_flags;
	struct buf **a_bpp;
};
extern struct vnodeop_desc vop_balloc_desc;
static int VOP_BALLOC __P((
	struct vnode *vp,
	off_t startoffset,
	int size,
	struct ucred *cred,
	int flags,
	struct buf **bpp));
static __inline int VOP_BALLOC(vp, startoffset, size, cred, flags, bpp)
	struct vnode *vp;
	off_t startoffset;
	int size;
	struct ucred *cred;
	int flags;
	struct buf **bpp;
{
	struct vop_balloc_args a;

	a.a_desc = VDESC(vop_balloc);
	a.a_vp = vp;
	a.a_startoffset = startoffset;
	a.a_size = size;
	a.a_cred = cred;
	a.a_flags = flags;
	a.a_bpp = bpp;
	return (VCALL(vp, VOFFSET(vop_balloc), &a));
}
struct vop_reallocblks_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct cluster_save *a_buflist;
};
extern struct vnodeop_desc vop_reallocblks_desc;
static int VOP_REALLOCBLKS __P((
	struct vnode *vp,
	struct cluster_save *buflist));
static __inline int VOP_REALLOCBLKS(vp, buflist)
	struct vnode *vp;
	struct cluster_save *buflist;
{
	struct vop_reallocblks_args a;

	a.a_desc = VDESC(vop_reallocblks);
	a.a_vp = vp;
	a.a_buflist = buflist;
	return (VCALL(vp, VOFFSET(vop_reallocblks), &a));
}
struct vop_getpages_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	vm_page_t *a_m;
	int a_count;
	int a_reqpage;
	vm_ooffset_t a_offset;
};
extern struct vnodeop_desc vop_getpages_desc;
static int VOP_GETPAGES __P((
	struct vnode *vp,
	vm_page_t *m,
	int count,
	int reqpage,
	vm_ooffset_t offset));
static __inline int VOP_GETPAGES(vp, m, count, reqpage, offset)
	struct vnode *vp;
	vm_page_t *m;
	int count;
	int reqpage;
	vm_ooffset_t offset;
{
	struct vop_getpages_args a;

	a.a_desc = VDESC(vop_getpages);
	a.a_vp = vp;
	a.a_m = m;
	a.a_count = count;
	a.a_reqpage = reqpage;
	a.a_offset = offset;
	return (VCALL(vp, VOFFSET(vop_getpages), &a));
}
struct vop_putpages_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	vm_page_t *a_m;
	int a_count;
	int a_sync;
	int *a_rtvals;
	vm_ooffset_t a_offset;
};
extern struct vnodeop_desc vop_putpages_desc;
static int VOP_PUTPAGES __P((
	struct vnode *vp,
	vm_page_t *m,
	int count,
	int sync,
	int *rtvals,
	vm_ooffset_t offset));
static __inline int VOP_PUTPAGES(vp, m, count, sync, rtvals, offset)
	struct vnode *vp;
	vm_page_t *m;
	int count;
	int sync;
	int *rtvals;
	vm_ooffset_t offset;
{
	struct vop_putpages_args a;

	a.a_desc = VDESC(vop_putpages);
	a.a_vp = vp;
	a.a_m = m;
	a.a_count = count;
	a.a_sync = sync;
	a.a_rtvals = rtvals;
	a.a_offset = offset;
	return (VCALL(vp, VOFFSET(vop_putpages), &a));
}
struct vop_freeblks_args {
	struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	daddr_t a_addr;
	daddr_t a_length;
};
extern struct vnodeop_desc vop_freeblks_desc;
static int VOP_FREEBLKS __P((
	struct vnode *vp,
	daddr_t addr,
	daddr_t length));
static __inline int VOP_FREEBLKS(vp, addr, length)
	struct vnode *vp;
	daddr_t addr;
	daddr_t length;
{
	struct vop_freeblks_args a;

	a.a_desc = VDESC(vop_freeblks);
	a.a_vp = vp;
	a.a_addr = addr;
	a.a_length = length;
	return (VCALL(vp, VOFFSET(vop_freeblks), &a));
}
#include <sys/buf.h>

struct vop_bwrite_args {
	struct vnodeop_desc *a_desc;
	struct buf *a_bp;
};
extern struct vnodeop_desc vop_bwrite_desc;
static int VOP_BWRITE __P((
	struct buf *bp));
static __inline int VOP_BWRITE(bp)
	struct buf *bp;
{
	struct vop_bwrite_args a;

	a.a_desc = VDESC(vop_bwrite);
	a.a_bp = bp;
	return (VCALL((bp)->b_vp, VOFFSET(vop_bwrite), &a));
}

extern int vfs_opv_numops;
