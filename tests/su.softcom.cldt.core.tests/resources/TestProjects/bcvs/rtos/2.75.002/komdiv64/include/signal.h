#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/signal.h>
#include <sys/time.h>


#if defined(_ANSI_SOURCE) || defined(__cplusplus) || defined(_MSC_VER)
#define SIG_DFL     (void (*)(int))0
#define SIG_IGN     (void (*)(int))1
#define SIG_ERR     (void (*)(int))-1
#else
#define SIG_DFL     (void (*)())0
#define SIG_IGN     (void (*)())1
#define SIG_ERR     (void (*)())-1
#endif


/* flags for sa_flags of struct sigaction */
#define SA_NOCLDSTOP 0x0001  /* Do not generate SIGCHLD when children stop */
#define SA_SIGINFO   0x0002  /* Pass additional siginfo structure */

/* values of sigev_notify */
#define SIGEV_NONE      0    /* no notification */
#define SIGEV_SIGNAL    1    /* queued signal notification */
#define SIGEV_THREAD    2    /* call back from another thread */
#define SIGEV_FUNCTION  3    /* call back from interrupt */
#define SIGEV_SIGTHR    4    /* queued signal to defined thread */

/* Flags for sigprocmask */
#define SIG_BLOCK       1    /* block specified signal set */
#define SIG_UNBLOCK     2    /* unblock specified signal set */
#define SIG_SETMASK     3    /* set specified signal set */
                             
/* The si_code returned in siginfo */
#define SI_USER         0    /* signal from kill() function -
                                сигнал был послан функцией kill(), raise() или abort() */
#define SI_QUEUE        1    /* signal from sigqueue() function - 
                                                 сигнал был послан функцией sigqueue() */
#define SI_TIMER        2    /* signal from expiration of a timer -
                                сигнал был сгенерирован в результате истечения
                                интервала времени, заданного таймером и
                                установленного функцией timer_settime()                */
#define SI_ASYNCIO      3    /* signal from completion of an async I/O */
#define SI_MESGQ        4    /* signal from arrival of a message - 
                                сигнал был сгенерирован в результате поступления
                                сообщения в пустую очередь сообщений                   */
#define ILL_ILLOPC      5    /* Illegal opcode -             недопустимый код операции */
#define ILL_ILLOPN      6    /* Illegal operand                                        */
#define ILL_ILLADR      7    /* Illegal addressing mode                                */
#define ILL_ILLTRP      8    /* Illegal trap                                           */
#define ILL_PRVOPC      9    /* Privileged opcode                                      */
#define ILL_PRVREG      10   /* Privileged register                                    */
#define ILL_COPROC      11   /* Coprocessor error - сопроцессор недоступен             */
#define ILL_BADSTK      12   /* Internal stack error                                   */
/* SIGFPE	*/
#define FPE_INTDIV      13   /* Integer divide by zero -    целочисленное деление на 0 */ 
#define FPE_INTOVF      14   /* Integer overflow -          целочисленное переполнение */
#define FPE_FLTDIV      15   /* Floating-point divide by zero - 
                                                         деление на 0 плавающего числа */ 
#define FPE_FLTOVF      16   /* Floating-point overflow - переполнение при выполнении
                                                          операций с плавающими числами*/
#define FPE_FLTUND      17   /* Floating-point underflow -     слишком маленькое число */
#define FPE_FLTRES      18   /* Floating-point inexact result                          */
#define FPE_FLTINV      19   /* Invalid floating-point operation - 
                                          недопустимая операция плавающей арифметики   */
#define FPE_FLTSUB      20   /* Subscript out of range                                 */
/* SIGSEGV */
#define SEGV_MAPERR     21   /* Address not mapped to object                           */
#define SEGV_ACCERR     22   /* Invalid permissions for mapped object                  */ 
/* SIGBUS	*/
#define BUS_ADRALN      23   /* Invalid address alignment                              */
#define BUS_ADRERR      24   /* Nonexistent physical address - неверный физ. адрес     */
#define BUS_OBJERR      25   /* Object-specific hardware error                         */ 
/* SIGTRAP */                
#define TRAP_BRKPT      26   /* Process breakpoint                                     */ 
#define TRAP_TRACE      27   /* Process trace trap                                     */ 
/* SIGCHLD */
#define CLD_EXITED      28   /* Child has exited                                       */ 
#define CLD_KILLED      29   /* Child has terminated abnormally and did not create a core file*/ 
#define CLD_DUMPED      30   /* Child has terminated abnormally and created a core file*/ 
#define CLD_TRAPPED     31   /* Traced child has trapped                               */ 
#define CLD_STOPPED     32   /* Child has stopped                                      */
#define CLD_CONTINUED   33   /* Stopped child has continued                            */ 
/* SIGPOLL */
#define POLL_IN         34   /* Data input available                                   */ 
#define POLL_OUT        35   /* Output buffers available                               */ 
#define POLL_MSG        36   /* Input message available                                */ 
#define POLL_ERR        37   /* I/O error                                              */ 
#define POLL_PRI        38   /* High priority input available                          */ 
#define POLL_HUP        39   /* Device disconnected                                    */ 


#if defined(_ANSI_SOURCE) || defined(__cplusplus) || defined(_MSC_VER)
void (*signal(int __sig, void (*__handler)(int)))(int);
#else
void (*signal(int __sig, void (*__handler)()))();
#endif
int raise(int);
int kill (pid_t, int);
int sigaction (int, const struct sigaction *, struct sigaction *);
int sigaddset (sigset_t *, int);
int sigdelset (sigset_t *, int);
int sigemptyset (sigset_t *);
int sigfillset (sigset_t *);
int sigismember (const sigset_t *, int);
int sigpending (sigset_t *);
int sigprocmask (int, const sigset_t *, sigset_t *);
int sigsuspend (const sigset_t *);

/* 3.3.10 Послать сигнал нити */
int pthread_kill( pthread_t thread,int sig );
/*  3.3.5 Установить и/или получить маску блокирования сигналов */
int pthread_sigmask( int how, const sigset_t *set, sigset_t *oset);

int sigqueue    (pid_t,int,const union sigval);
int sigwait (const sigset_t *, int *sig);
int sigwaitinfo (const sigset_t *,siginfo_t *);
int sigtimedwait(const sigset_t *,siginfo_t *, const struct timespec *);


#endif  /* _SIGNAL_H */
