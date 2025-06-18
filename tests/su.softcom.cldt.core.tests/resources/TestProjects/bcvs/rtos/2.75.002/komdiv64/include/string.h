/*
Copyright
*/
#ifndef  _STRING_H
#define  _STRING_H

#include <sys/cdefs.h>
#include <sys/types.h>



#ifndef NULL
#define NULL        0
#endif

void *memchr  ( const void *s, int c, size_t n ) ;
int   memcmp  ( const void *s1, const void *s2, size_t n ) ;
void *memcpy  ( void *s1, const void *s2, size_t n ) ;
void *memmove ( void *s1, const void *s2, size_t n ) ;
void *memset  ( void *s, int c, size_t n ) ;

char  *strcat ( char *dest, const char *src ) ;
char  *strchr ( const char *s, int c ) ;
int    strcmp ( const char *s1, const char *s2 ) ;
int    strcoll ( const char *s1, const char *s2 );
char  *strcpy  ( char *dest, const char *src ) ;
size_t strcspn ( const char *s1, const char *s2 ) ;
char  *strerror( int errcode ) ;
size_t strlen  ( const char *s ) ;
char  *strncat ( char *dest, const char *src, size_t maxlen ) ;
int    strncmp ( const char *s1, const char *s2, size_t  maxlen ) ;
char  *strncpy ( char *dest, const char *src, size_t maxlen ) ;
char  *strpbrk ( const char *s1, const char *s2 ) ;
char  *strrchr ( const char *s, int c ) ;
size_t strspn  ( const char *s1, const char *s2 ) ;
char  *strstr  ( const char *s1, const char *s2 ) ;
char  *strtok  ( char *s1, const char *s2 ) ;
char  *strtok_r ( char *s, const char *sep, char **lasts ) ;
size_t strxfrm ( char *s1, const char *s2, size_t n );

/*не ANSI функции */
void * memscan(void * addr, int c, size_t size) ;
size_t strnlen(const char * s, size_t count) ;


char  *strdup ( const char *src ) ;
char  *strndup(const char *s, size_t size);

#define __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_BCOPY
#define __HAVE_ARCH_BZERO


#endif   /* _STRING_H */
