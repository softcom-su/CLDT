#ifndef __INCrregh__
#define __INCrregh__

/* size of message queue used for synchronization doorbell's */

#ifndef RREG_MQ_SIZE
#define RREG_MQ_SIZE            512
#endif

/* rreg doorbell */

#ifndef RREG_DB_NET
#define RREG_DB_NET             0
#endif

#ifndef RREG_DB_CHAN
#define RREG_DB_CHAN            1
#endif

#define RREG_DB_NET_IX          0
#define RREG_DB_CHAN_IX         1
#define	RREG_DB_MAX		2

/* first synchronization doorbell */

#define RREG_DB_ALIVE           0xffe

/* globals */

extern int rregDebug;                      /* debug level */
extern int rregIdMax;                      /* number of id's'*/

/* function declarations */
int rregInit(unsigned nId, unsigned pri, void (*)(int,unsigned,const char *), int hostType);
void rregResync(unsigned id, unsigned db);
void rregShutdown(unsigned id, unsigned db);
int rregIsReady(unsigned id, unsigned db);
void rregResyncAll(unsigned db, int forced);
void rregShutdownAll(unsigned db, int forced);
int rregResyncPost(unsigned id, unsigned db);
int rregHostTypeGet(unsigned id);
int rregIdMapped(unsigned id);
int rregWait(unsigned db, unsigned tout);
int rregEnable(unsigned db);
int rregDisable(unsigned db);
int rregTagRegistration(unsigned isMain, unsigned tout);
int rregTagMasterSync(unsigned id, unsigned tout);
int rregTagSlaveSync(unsigned tout);

#endif  /* __INCrregh__ */
