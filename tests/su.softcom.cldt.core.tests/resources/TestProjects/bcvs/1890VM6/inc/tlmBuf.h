
#ifndef inc_tlmBuf_h
#define inc_tlmBuf_h

#include "psc.h"

int tlmBufInit(int n);
void tlmBufWrite(int n,void * adr, size_t size);
int tlmBufIsReady(int n);
void tlmBufWaitReady(int n,unsigned delay);
int tlmBufStart(int n,size_t size);
void tlmBufSend(int n,void * adr,size_t size);
void tlmBufWritePiece(int n, unsigned * adr, size_t size, unsigned offset);
void tlmSetCount(int n, size_t count);

#endif /*inc_tlmBuf_h*/
