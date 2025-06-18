/* pf.h - parallel IDE floppy driver header */

#ifndef	__INCpfh
#define	__INCpfh

#include <paride.h>

/* ioctl functions */

#define	PF_IOCTL_LOCK	10000		/* door lock */
#define	PF_IOCTL_UNLOCK	10001		/* door unlock */
#define	PF_IOCTL_EJECT	10002		/* eject disk */

/* typedefs */

typedef struct pf_unit PF_UNIT;

/* function declarations */

#if OS_VERSION >= 210
PF_UNIT* pfDevCreate(LPT_CTRL* lpt, int mode, int unit, PIP* protocol,
 int delay, int drive, int lun, int cluster, int nice, int _verbose,
 int priority);
#else	/* OS_VERSION >= 210 */
PF_UNIT* pfDevCreate(LPT_CTRL* lpt, int mode, int unit, PIP* protocol,
 int delay, int drive, int lun, int cluster, int nice, int _verbose);
#endif	/* OS_VERSION >= 210 */

BLK_DEV* pfBlkDevCreate(PF_UNIT* pf, unsigned size, unsigned offset);

#if OS_VERSION >= 210
BLK_IO_DEV* pfBlkDevInstall(PF_UNIT* pf, unsigned size, unsigned offset,
    const char* name);
#endif	/* OS_VERSION >= 210 */

#endif	/* __INCpfh */
