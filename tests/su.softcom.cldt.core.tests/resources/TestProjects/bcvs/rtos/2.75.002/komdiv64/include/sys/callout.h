#ifndef _SYS_CALLOUT_H
#define _SYS_CALLOUT_H

typedef void timeout_t __P((void *));	/* timeout function type */

#ifdef KERNEL
#include <private/clock.h>
#include <sys/queue.h>
struct callout
{
	clockWAIT_NODE m_waitNode;
	timeout_t      *m_ftn;
	void		   *m_arg;
	SLIST_ENTRY(callout) m_list;						
	int				c_flags;
};
#else
struct callout;
#endif /* KERNEL */
#define	CALLOUT_LOCAL_ALLOC	0x0001 /* was allocated from callfree */
#define	CALLOUT_ACTIVE		0x0002 /* callout is currently active */
#define	CALLOUT_PENDING		0x0004 /* callout is waiting for timeout */
#define	CALLOUT_TIMENODE	0x0008

#define	callout_active(c)	((c)->c_flags & CALLOUT_ACTIVE)
#define	callout_deactivate(c)	((c)->c_flags &= ~CALLOUT_ACTIVE)
#define	callout_pending(c)	((c)->c_flags & CALLOUT_PENDING)
/* Timeouts */
#define CALLOUT_HANDLE_INITIALIZER(handle)	\
	{ NULL }
struct callout_handle {
	struct callout *callout;
};

void	callout_handle_init __P((struct callout_handle *));
struct	callout_handle timeout __P((timeout_t *, void *, int));
void	untimeout __P((timeout_t *, void *, struct callout_handle));

void	callout_init(struct callout *);
void	callout_reset(struct callout *, int, void (*)(void *), void *);
void	callout_stop(struct callout *);
#endif
