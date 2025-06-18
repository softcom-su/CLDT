/* ldebugp.h - внутренний файл для локального отладчика */

/* Copyright 1998-1999 НИИСИ РАН. */

/*
modification history
--------------------
17.45, 03 nov 98 created.
*/


#ifndef _LDEBUGP_H
#define _LDEBUGP_H
#ifdef __cplusplus
extern "C" {
#endif



/* externals */

extern pthread_t shellTCB;
extern int    	trcDefaultArgs;		/* число аргументов для печати */

extern funcptr_t 	a_dblDsmCommandRtn ; /* функция дисассмблера */
extern funcptr_t 	a_dblAdrChkRtn ; 	 /* контроль адреса точки останова */
extern int  ldbBrkAdd (COMMAND *addr, u_long thread, int count, int type);


/* архитектурно-зависимые процедуры */
/*-----------------------------------------------------------------*/
/* traceStack - распечатка стека вызовов "сверху вниз".            */
/*-----------------------------------------------------------------*/
void traceStack(threadTCB *tcbTh, int (printLvl)(FRAME_PARMS *params)) ;
/*-----------------------------------------------------------------*/
/* traceStackB - распечатка стека вызовов "снизу вверх".           */
/*-----------------------------------------------------------------*/
void traceStackB(threadTCB *tcbTh, int (printLvl)(FRAME_PARMS *params)) ;

int ldbBreakpoint (void *pEsf)  ;
void ldbTrace (void *pEsf) ;

#ifdef __cplusplus
}
#endif

#endif /* _LDEBUGP_H */
