/* logLib.h - message logging library header */

#ifndef __INClogLibh
#define __INClogLibh

/* globals */

extern volatile int logProcNum;		/* if TRUE log processor number */

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

int logInit(int maxMsgs);
int logMsg(const char* fmt, long arg1, long arg2, long arg3, long arg4,
    long arg5, long arg6);
void logFdSet(int fd);
int logFdAdd(int fd);
int logFdDelete(int fd);

#else	/* __STDC__ */

int logInit();
int logMsg();
void logFdSet();
int logFdAdd();
int logFdDelete();

#endif	/* __STDC__ */

#endif /* __INClogLibh */
