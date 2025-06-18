#ifndef _PRIVATE_SI_H
#define _PRIVATE_SI_H
#ifdef KERNEL
struct vnode;
struct specinfo 
{
	struct	vnode **si_hashchain;
	struct	vnode *si_specnext;
	struct	mount *si_mountpoint;
	dev_t		si_rdev;
	unsigned long	si_blksize; /* smallest IO unit */
	void		*si_dev;
};
#define si_bsize_best si_blksize
#define si_bsize_phys si_blksize
/*
 * Exported shorthand
 */
/* #define v_rdev v_specinfo->si_rdev */
#define v_hashchain v_specinfo->si_hashchain
/* #define v_specnext v_specinfo->si_specnext */
#define v_specmountpoint v_specinfo->si_mountpoint
#define v_blksize v_specinfo->si_blksize

#endif /* KERNEL */

#endif
