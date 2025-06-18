#ifndef __INCtaskLibh
#define __INCtaskLibh

#include <system.h>
#include <version.h>
#include <semaphore.h>

/* task options */

#define	VX_FP_TASK	0x0008	/* 1 = f-point coprocessor support */
#define OC_SCHED_RR	0x1000	/* round robin task */
#define	OC_JOIN_TASK	0x2000	/* joined task */

/* function declarations */

int taskPriorityGet(int tid, int* pPriority);
long taskSpawn(char* name, int pri, int options, int stackSize, FUNCPTR entry,
    long, long, long, long, long, long, long, long, long, long);
STATUS taskLock();
STATUS taskUnlock();
STATUS taskDelay(int ticks);
unsigned long tickGet();
int taskDelete(int tid);
int taskSuspend(int tid);
int taskResume(int tid)	;
int taskIdSelf(void);
int taskSafe();
int taskUnsafe();
int taskPrioritySet(int tid, int newPriority);
int _sem_timedwait(sem_t* s, unsigned sec, unsigned ns);
int _sem_timedwait_us(sem_t* s, unsigned us);
#if OS_VERSION >= 300
int semTimedwait(sem_t* s, unsigned nTicks);
#endif

#endif /* __INCtaskLibh */

