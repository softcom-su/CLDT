#ifndef _PRIVATE_ECB_H
#define _PRIVATE_ECB_H

typedef struct {
#define ecbhWAITED		 1			/* есть ожидающий  поток			*/
#define ecbhWAITCOUNT    2			/* нужно ждать счетчик				*/
    volatile int        ecbh_flags;
    volatile int        ecbh_count;
    void				*ecbh_thread;
} ecbHEAD;

typedef struct {
#define ecbnPOSTED		1			
#define ecbnHASHEAD		2			
    int                 ecbn_flags;
	int					ecbn_seltype;
	ecbHEAD				*ecbn_head;
} ecbNODE;


#define ecbhINIT(ecbh)    (ecbh)->ecbh_flags=0;
#define ecbnINIT(ecbn)    (ecbn)->ecbn_flags=0;


int     ecbPost(ecbNODE *ecbn);

#define ecbWAIT_ALL	1
#define ecbWAIT_ANY	2

struct timespec;

int     ecbWait(int count,ecbNODE **ecbnl,int type,const struct timespec *ts);

#endif                                  /* !_PRIVATE_ECB_H */
