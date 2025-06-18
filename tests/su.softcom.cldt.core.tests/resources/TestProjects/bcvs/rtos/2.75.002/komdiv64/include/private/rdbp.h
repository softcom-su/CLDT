/* rdbp.h - заголовочный файл сервера удаленного отладчика */

#ifndef __INCrdbph
#define __INCrdbph	

#include <semaphore.h> 
#include <xdr_ptrace.h>
#include <xdr_rdb.h>
#include <private/ldebug.h>

#define  NEWSTART 

typedef struct 
{
 sdlNODE	ptraceNotifNode;	/* узел списка уведомлений потока */
 RDB_NOTIFICATION	rdbNotif;	/* описатeль уведомления */
} RDB_NOTIF_NODE;

#define  SIZE_FREE  20     /* размер списка свободных элементов */
#define  THREAD_ISSUSPEND  0x01
typedef struct 
{
 sdlNODE    notifNode;     /* узел общего списка уведомлений  */
 int        thread;        /* идентификатор отлаживаемого потока */
 sem_t      semId;         /* семафор синхронизации доступа к списку */
 COMMAND *  stepStartAddr; /* начальный адрес диапазона пошагового режима */
 COMMAND *  stepEndAddr;   /* конечный адрес диапазона пошагового режима */
 sdlHEAD    notifList;     /* список уведомлений потока */
 sdlHEAD    freeList;      /* список свободных элементов */
 int        cntFree;       /* счетчик свободных элементов */
 int        flags;
} THREAD_NOTIF;

typedef struct 
{
 sdlNODE    sonNode;        /* узел общего списка сыновьих потоков  */
 int        thFather;       /* идентификатор отцовского потока */
 int        thSon;          /* идентификатор сыновьего потока */
} THREAD_SON ;


extern int	rdbInit (void);
extern void	rdbThread (void);
extern void	rdbInterrupt (int thread);
extern COMMAND *rdbPcGet(int thread) ;


int rdbThreadRegsGet(C_bytes *outBuf,	int	thread) ;
int rdbThreadFpRegsGet(C_bytes *outBuf,	int	thread) ;
int rdbThreadRegsSet (C_bytes *addr, int pid) ;
int rdbThreadFpRegsSet (C_bytes *addr, int pid) ;
void   rdbExcHookAdd (vfuncptr_t rdbExcHook) ;

#endif /* __INCrdbph */
