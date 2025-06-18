/* memtest.h - memtest header */

#ifndef __INCmemtesth
#define __INCmemtesth

/* globals */

extern FUNCPTR memtestError;	/* memtest error handler */

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

int mt(void* addr, unsigned length, unsigned testmask);
void mtInit(void);

#else

int mt();
void mtInit();

#endif  /* __STDC__ */

#endif /* __INCmemtesth */
