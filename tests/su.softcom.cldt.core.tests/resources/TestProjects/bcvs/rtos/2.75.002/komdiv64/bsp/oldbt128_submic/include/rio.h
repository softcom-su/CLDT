#ifndef __ASSEMBLER__
#ifndef	__INCrioh__
#define	__INCrioh__

#include <system.h>

/* rio host type */

#define RIO_HOST_INVALID        (-1)
#define RIO_HOST_UNKNOWN        0
#define RIO_HOST_K128           1
#define RIO_HOST_K64            2

/* device vendor ID */

#define RIO_VENDOR_ID_NIISI     0x0074

/* device ID */

#define RIO_DEVICE_ID_K128      0x0128
#define RIO_DEVICE_ID_K64RIO    0x8717

/* globals */

extern int rioMaintTout;				// maintainance cycle timeout us
extern int rioDebug;					// debug level

/* function declarations */

int rioMaintIn(unsigned hopCount, unsigned addr, unsigned dstID, unsigned* pData);
int rioMaintOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data);
int rioDbIntConnect(unsigned irq, VOIDFUNCPTR rtn, int arg);
int rioDbOut(unsigned dstID, unsigned i, unsigned data);
int _rioDbOut(unsigned dstID, unsigned i, unsigned data);
void rioHopShow(unsigned hop, unsigned id, int errClear, unsigned maxId);
int rioHostTypeGet(unsigned id);
void rioShutdown(void);

#endif	/* __INCrioh__ */
#endif
