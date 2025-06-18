/* semaphre.h
 */
#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H
#include   <version.h>
#include   <fcntl.h>
#include   <sys/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define  SEM_FAILED  (sem_t*)-1

struct timespec;


extern int    sem_close ( sem_t * );
extern int    sem_destroy ( sem_t * );
extern int    sem_getvalue ( sem_t *, int * );
extern int    sem_init ( sem_t *, int, unsigned int );
extern sem_t *sem_open ( const char *, int, ... );
extern int    sem_post ( sem_t * );
extern int    sem_trywait ( sem_t * );
extern int    sem_unlink ( const char * );
extern int    sem_wait ( sem_t * );
extern int    sem_timedwait ( sem_t *sem, const struct timespec *timeout );

#if !defined ( POSIX_SOURCE ) && OS_MAJOR >= 3
/* Средства просмотра семафоров */
extern void semPostDelayed( long semId, long unused );
extern void semShow( pid_t pid, unsigned int id, char *options );
extern int  semInit ( sem_t *, int, unsigned int );
#endif /*!POSIX_SOURCE*/


#ifdef __cplusplus
}
#endif

#endif /* _SEMAPHORE_H */
