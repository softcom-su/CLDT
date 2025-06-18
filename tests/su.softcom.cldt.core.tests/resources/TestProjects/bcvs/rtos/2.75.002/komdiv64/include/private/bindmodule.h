#ifndef _PRIVATE_BINDMODULE_H
#include <stdarg.h>

struct prsmem_head;

extern void (*kernResolveSigRtn)(void);
/* Обработка сигналов, доставленные процессу или потоку  */
extern void (*kernExecSigRtn)(void);
extern void (*kernExecSigProcRtn)(void);  /* обработка сигналов, доставленные процессу */
extern char *  (*kernResolveSymRtn)(int,void *);
struct sigINFO_NODE;
struct thrdControl;
struct prsmem_main;

  extern int (*kernSigKillRtn) (struct thrdControl *thread,
                              int sig, int si_code, int si_value,
                              struct sigINFO_NODE *parm
                             );


extern void (*termIntrHook)(void);
extern void (*termQuitHook)(void);

extern int	(*pipe_stat)(void *pn,void *ub);
extern void (*closestreams)();

extern int  (*sharemem)( int cpu, int type, volatile void** ptr, size_t* size );
extern int  (*prsmeminfo)( struct prsmem_main* info );
extern void (*trValidationMessage)( const char *format, va_list args );

extern struct _desc_ostest* pOstestDescription;
#define MOD_COND_CALL(a) a()  //@fixme
#endif /* !_PRIVATE_BINDMODULE_H */
