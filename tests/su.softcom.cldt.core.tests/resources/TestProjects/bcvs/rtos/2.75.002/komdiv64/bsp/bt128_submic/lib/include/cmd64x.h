#ifndef	__INCcmd64x
#define	__INCcmd64x

#include <ideDrv.h>

#define	CMD_MAX_XFER		256	/* maximum transfer sectors */
#define	CMD_PIO_DEF		(-1)	/* use reset pio timings */

/* function declarations */

IDE_CTRL* cmd64xCtrlInit(int unit, int reset, int priority, unsigned pio);

#endif	/* __INCcmd64x */
