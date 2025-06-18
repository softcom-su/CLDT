#ifndef _PRIV_SYSDEF_H
#define _PRIV_SYSDEF_H

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

#define ADDR_ALIGN_UP(x, align)	(((int) (x) + (align - 1)) & ~(align - 1))
#define ADDR_ALIGN_DOWN(x, align)	((int)(x) & ~(align - 1))
#define ADDR_ALIGNED(x, align)	(((int)(x) & (align - 1)) == 0)

#define UADDR_ALIGN_UP(x, align)  (((unsigned int) (x) + (align - 1)) & ~(align - 1))
#define UADDR_ALIGN_DOWN(x, align)   ((unsigned int)(x) & ~(align - 1))
#define UADDR_ALIGNED(x, align)      (((unsigned int)(x) & (align - 1)) == 0)

#endif /* _PRIV_SYSDEF_H */


