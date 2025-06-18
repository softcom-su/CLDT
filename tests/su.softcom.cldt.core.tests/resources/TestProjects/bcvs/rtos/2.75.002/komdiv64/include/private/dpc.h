/*  dpc.h  */

#ifndef _DPC_H
#define _DPC_H

#define dpcENTRIES  256

extern u_int dpcReadIndx;
extern u_int dpcWriteIndx;

typedef struct 
{
    funcptr_t func;
    void *arg1;
    void *arg2;
} dpcENTRY;

#ifdef __cplusplus
extern "C" {
#endif


void dpcPut( funcptr_t, void *param1, void *param2 );
void dpcExec( void );

#ifdef __cplusplus
}
#endif

#endif /* _DPC_H */
