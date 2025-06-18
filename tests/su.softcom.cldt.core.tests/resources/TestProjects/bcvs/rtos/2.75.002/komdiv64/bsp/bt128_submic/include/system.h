
#ifndef _BSP_SYSTEM_H
#define _BSP_SYSTEM_H

#include <unistd.h>
#include <sys/types.h>
#include <private/errset.h>
#include <version.h>

#if OS_VERSION < 270 || (OS_VERSION == 270 && OS_RELEASE < 6)
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#ifdef	KERNEL
#include <sys/time.h>
#include <sys/systm.h>
#endif

#if OS_VERSION < 300 && ! defined MODCALL
#define	MODCALL(arg)	arg
#endif

/* Define NULL */
#ifndef NULL
#define NULL      0          /* NULL value */
#endif

/* Boolean constants */
#if !defined( TRUE ) || (TRUE != 1)
#undef TRUE
#define TRUE     (1)
#endif

#if !defined( FALSE ) || (FALSE != 0)
#undef FALSE
#define FALSE     (0)
#endif

#define STAT_OK              0
#define STAT_ER             -1

#define WAIT_FOREVER  -1
#define NOWAIT         0

#define EOS        '\0'

#define STD_IN      0
#define STD_OUT     1
#define STD_ERR     2

#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#define ITEM_OFFSET(structure, member) ((int) &(((structure *) 0) -> member))

#define ITEM_SIZE(structure, member) (sizeof (((structure *) 0) -> member))

#define ARRAY_SIZE(array) (sizeof (array) / sizeof ((array) [0]))

#define FOREVER	for (;;)

#ifndef ADDR_ALIGN_UP
#define ADDR_ALIGN_UP(x, align)		(((uintptr_t) (x) + (align - 1)) & ~(align - 1))
#define ADDR_ALIGN_DOWN(x, align)	((uintptr_t)(x) & ~(align - 1))
#define ADDR_ALIGNED(x, align)		(((uintptr_t)(x) & (align - 1)) == 0)
#endif

#define STATUS int
typedef long 		(*FUNCPTR) ();
typedef void        (*VOIDFUNCPTR) ();
typedef double 		(*DBLFUNCPTR) ();
typedef float 		(*FLTFUNCPTR) ();

#if OS_VERSION >= 305

/* shared memory BSP private */

typedef struct {
    void*	flash;		/* flash descriptor */
    unsigned	cpuClockRate;	/* CPU clock rate in MHz */
} MLP_BSP;

#endif

#endif /* _SYSTEM_H */


