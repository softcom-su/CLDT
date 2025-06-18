#ifndef	_SYS_SIGNAL_H_
#define	_SYS_SIGNAL_H_

#include <sys/types.h>

typedef unsigned long   sigset_t;

union sigval {
		int                 sival_int;
		void                *sival_ptr;
};

struct sigevent	{
		int						sigev_notify;
		int						sigev_signo;
		volatile union sigval	sigev_value;
		void					(*sigev_notify_function)(union sigval) ;
		pthread_attr_t *		sigev_notify_attributes;
		pthread_t               sigev_thread;
        int						m_count; /* Для lio_listio */
};

typedef struct siginfo {
		int            si_signo;
		int            si_code;
		union sigval   si_value;
} siginfo_t;

struct sigaction {
		union	{
#if defined(__STDC__) || defined(__cplusplus)
				void    (*__sa_handler)(int);
				void    (*__sa_sigaction)(int, siginfo_t *, void *);
#else
				void    (*__sa_handler)();
				void    (*__sa_sigaction)();
#endif /* __STDC__ */
		}  sa_u;
		sigset_t    sa_mask;
		int         sa_flags;
};
#define sa_handler      sa_u.__sa_handler
#define sa_sigaction    sa_u.__sa_sigaction

#define SIGHUP       1  /* hangup */
#define SIGINT       2  /* interrupt (rubout) */
#define SIGQUIT      3  /* quit (ASCII FS) */
#define SIGILL       4  /* illegal instruction (not reset when caught) */
#define SIGTRAP      5  /* trace trap (not reset when caught) */ 
#define SIGIOT       6  /* IOT instruction */
#define SIGABRT      6  /* used by abort, replace SIGIOT in the future */
#define SIGEMT       7  /* EMT instruction */
#define SIGFPE       8  /* floating point exception */
#define SIGKILL      9  /* kill (cannot be caught or ignored) */
#define SIGBUS      10  /* bus error */
#define SIGSEGV     11  /* segmentation violation */
#define SIGSYS      12  /* bad argument to system call */
#define SIGPIPE		13  /* write on a pipe with no one to read it */
#define SIGALRM     14  /* alarm clock */
#define SIGTERM     15  /* software termination signal from kill */
#define SIGUSR1     16  /* user defined signal 1 */
#define SIGUSR2     17  /* user defined signal 2 */


#define	SIGCLD      18  /* child status change */
#define	SIGCHLD     18  /* to parent on child stop or exit */
#define	SIGSTOP     19  /* sendable stop signal not from tty (cannot be caught or ignored) */
#define	SIGTSTP     20  /* stop signal from tty */
#define	SIGCONT     21  /* stopped process has been continued */
#define	SIGTTIN     22  /* background tty read attempted */
#define	SIGTTOU     23  /* background tty write attempted */



#define	SIGRTMIN	24
#define	SIGRTMAX	31

int sigPxInit( int  max_queued_signals );

#ifdef KERNEL

#include <private/sdlist.h>

struct sigINFO_NODE {
    sdlNODE     m_node;
    sdlHEAD    *m_owner;
    siginfo_t   m_info;
} ;



#define sigNUM_TO_BIT( sig ) (1 << ((sig) - 1))
#define sigIS_UNSUPPORTED( sig ) ( (sig) < 1 || (sig) > NSIG )
#define sigIS_QUEUED( sig ) ( sig >= SIGRTMIN && sig <= SIGRTMAX )
/*
#ifdef _POSIX_REALTIME_SIGNALS
#define sigIS_QUEUED( sig ) ( sig >= SIGRTMIN && sig <= SIGRTMAX )
#else
#define sigIS_QUEUED( sig ) (0)
#endif
*/
#define sigEMPTY_MASK  0x00000000
#define sigFILL_MASK   0xffffffff

#endif /* KERNEL */

#define _NSIG       32
#define NSIG		_NSIG


#endif	/* !_SYS_SIGNAL_H_ */
