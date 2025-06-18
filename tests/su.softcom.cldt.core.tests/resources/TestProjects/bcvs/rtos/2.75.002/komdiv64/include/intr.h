#ifndef _INTR_H
#define _INTR_H

typedef int intr_t;

#define POSIX_INTR_PRI_LOWEST		1
#define POSIX_INTR_PRI_LOW			2
#define POSIX_INTR_PRI_MED_LOW		3
#define POSIX_INTR_PRI_MEDIUM		4
#define POSIX_INTR_PRI_MED_HIGH		5
#define POSIX_INTR_PRI_HIGH			6
#define POSIX_INTR_PRI_HIGHEST		7

int posix_intr_assosiate(intr_t intr,int (*intr_handler)(void *area),volatile void *area,
						 size_t areasize);
int posix_intr_disassosiate(intr_t intr,int (*intr_handler)(void *area));
int posix_intr_lock(intr_t intr);
int posix_intr_unlock(intr_t intr);

#define POSIX_INTR_HANDLED_NOTIFY			0
#define POSIX_INTR_HANDLED_DO_NOT_NOTIFY	1
#define POSIX_INTR_NOT_HANDLED				2

#ifndef _POSIX_SOURCE
#include <arch/interrupt.h>
#endif

#endif /*!_INTR_H*/