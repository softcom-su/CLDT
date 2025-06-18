/* idle.h - cpu load measurements library header */

#ifndef __INCidleh
#define __INCidleh

/* function declarations */

void idleStart(void);
void* idleThread(void* unused);
int idleInit(int priority, int _delay);
inline unsigned idleCpuLoadGet(void);
void idleShow(void);

#endif /* __INCidleh */
