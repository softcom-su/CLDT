/* am53c974.h - am53c974akc SCSI controller driver header */

#ifndef	__INCam53c974h
#define	__INCam53c974h

#include <version.h>

/* function declarations */

#if OS_VERSION >= 210
SCSI_HOST* am53c974HostCreate(int ix, unsigned id, int ioPriority, int bqPriority);
#else
SCSI_HOST* am53c974HostCreate(int ix, unsigned id, int ioPriority);
#endif
int am53c974Version(int silence);

#endif	/* __INCam53c974h */
