/*
*/
#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

#ifndef NULL
#define NULL        0
#endif
#define EXIT_FAILURE    -1
#define EXIT_SUCCESS    0
#define RAND_MAX        32767  /*для rand_r более недопустимо*/

#define	MB_CUR_MAX	3

typedef struct {
        int quot;
        int rem;
} div_t;

typedef struct {
        long quot;
        long rem;
} ldiv_t;

#if !defined(_SYS_MALLOC_H)
void    free( void *mem_block );
void   *malloc( size_t mem_size );
void   *calloc ( size_t nelem, size_t size );
void   *realloc(void *bufptr,size_t size);
#endif

extern char **environ; /*переменные окружения*/

void   abort ( void );
int    abs ( int i );
int    atexit( void (*func)(void) );
double atof( const char *sptr );
int    atoi( const char *string );
long   atol( const char *string );
void  *bsearch ( const void *key,
            const void *base,
            size_t nelem,
            size_t size,
            int (*cmp)( const void *ck, const void *ce )
        );
div_t   div ( int numer, int denom );
void    exit( int status );
void    _exit( int status );
long    labs ( long i );
ldiv_t  ldiv ( long numer, long denom );
void    qsort (
        void *base,
        size_t nelem,
        size_t size,
        int (*cmp)(const void *el1, const void *el2)
        );
int		  rand(void);
int     rand_r ( unsigned int *seed );
void    srand( unsigned seed );

long    random();
void    srandom(unsigned long x);

double  strtod ( const char *sptr, char **endptr );
long    strtol(const char *, char **, int);
unsigned long	strtoul(const char *, char **, int);
unsigned long long  strtoull(const char *, char **, int);
long long  strtoll(const char *, char **, int);
int     system ( const char *sptr );  /*возвращает всегда Ок, ничего не выполняя*/


#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
void   *reallocf(void *, size_t);
int     kprint(const char *format,...);
#endif

int   putenv(const char *sptr);
char *getenv(const char *sptr);

int  setenv(const char *name, const char *value, int override);
void unsetenv(const char *sptr);

/*
 * multibyte support:
 */
int    mbtowc(wchar_t *pwc, const char *sptr, size_t n);
int    wctomb(char *sptr, wchar_t wchar);
size_t wcstombs ( char *sptr, const wchar_t *wcs, size_t n );
int    mblen ( const char *sptr, size_t n );
size_t mbstowcs ( wchar_t *wcs, const char *sptr, size_t n );

#endif  /*_STDLIB_H*/

