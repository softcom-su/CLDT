
#ifndef inc_mram_h
#define inc_mram_h


int      mramInit(unsigned cpu);
int      mramWrite(unsigned cpu,void * from,unsigned to,unsigned size);
int      mramRead(unsigned cpu,unsigned from,void * to,unsigned size);

unsigned mramReadCache(unsigned cpu,unsigned wrd);
void     mramWriteCache(unsigned cpu,unsigned wrd,unsigned val);

int  mramWaitReady(unsigned int reg);
void mramStop(unsigned int reg);
void mramStart(unsigned reg,unsigned wrd,unsigned len,unsigned cyc);


extern volatile unsigned mram_flag;

#include <pthread.h>
extern pthread_mutex_t mram_mutex_rd;
extern pthread_mutex_t mram_mutex_wr;

#endif
