/*-
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)stdio.h 8.4 (Berkeley) 1/4/94
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <limits.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <stdarg.h>

typedef off_t fpos_t;
typedef struct __sFILE FILE;

#ifndef EOF
#define EOF -1
#endif

#ifndef NULL
#define NULL        0
#endif

int __srget( FILE *stream );
int __svfscanf( FILE *stream, const char *p, va_list );
int __swbuf ( int, FILE *stream );

FILE *threadGetStdF(int n);

#define stdin  threadGetStdF(0)
#define stdout threadGetStdF(1)
#define stderr threadGetStdF(2)
#define BUFSIZ  1024        /* size of buffer used by setbuf */

#ifdef _MSC_VER
#define fflush  ffflush
int     ffflush( FILE *stream );
#else
int     fflush( FILE *stream );
#endif

/* static __inline int __sputc(int _c, FILE *_p) __UNUSED_F; */

void   clearerr( FILE *stream );
int    feof( FILE *stream );
int    ferror( FILE *stream );
int    printf(  const char *format ,...);



/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which brilliantly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although numbered as their counterparts above, the implementation
 * does not rely on this.
 */

#define _IOFBF  0       /* setvbuf should set fully buffered */
#define _IOLBF  1       /* setvbuf should set line buffered */
#define _IONBF  2       /* setvbuf should set unbuffered */



#define FOPEN_MAX         STREAM_MAX    /* must be <= OPEN_MAX <sys/syslimits.h> */
#define FILENAME_MAX    1024    /* must be <= PATH_MAX <sys/syslimits.h> */

#define L_tmpnam        1024    /* XXX must be == PATH_MAX */
#define TMP_MAX         308915776

#ifndef SEEK_SET
#define SEEK_SET        0   /* set file offset to offset */
#endif

#ifndef SEEK_CUR
#define SEEK_CUR        1   /* set file offset to current plus offset */
#endif

#ifndef SEEK_END
#define SEEK_END        2   /* set file offset to EOF plus offset */
#endif


int     sscanf( const char *buffer, const char *format , ... );
int    fclose( FILE *stream );


int    fgetc( FILE *fp );
int    fgetpos( FILE *stream, fpos_t *pos );
char  *fgets( char *string, int n, FILE *stream );
FILE  *fopen( const char *file, const char *mode );
int    fprintf( FILE *stream, const char *format ,...);
int    fputc( int, FILE *stream );
int    fputs( const char *sptr, FILE *fp );
size_t fread( void *buffer, size_t size, size_t count, FILE *stream );
FILE    *freopen ( const char *, const char *, FILE *stream );
int    fscanf( FILE *fp, char const *fmt, ... );
int    fseek( FILE *fp, long offset, int mode );
int    fsetpos (FILE *stream, const fpos_t *pos );
long   ftell( const FILE *stream );
size_t fwrite( void const *buffer, size_t size, size_t count, FILE *stream );
int    getc( FILE *stream );
char  *gets( char *string );
void   perror( const char *sptr );


int    getc_unlocked(FILE *stream);
int    getchar_unlocked(void);
int    putc_unlocked(int c, FILE *stream);
int    putchar_unlocked(int c);
void   flockfile(FILE *file);
int    ftrylockfile(FILE *file);
void   funlockfile(FILE *file);

int    putc( int ch, FILE *stream );
int    puts( const char *str );
int    rename ( const char * oldn, const char * newn );
int    remove ( const char * path );
void   rewind( FILE *fp );
int    scanf( char const *fmt, ...);
void     setbuf ( FILE *stream, char *buf );
int    setvbuf ( FILE *stream, char *buf, int mode, size_t size );
int    sprintf( char *buf, const char *fmt, ... );
FILE    *tmpfile ( void );
char    *tmpnam ( char *sptr );
int    ungetc( int ch, FILE *stream );
int    vfprintf( FILE *stream, const char *format, va_list argptr );
int    vprintf ( const char *fmt, va_list ap );
int    vsnprintf(char *str, size_t n, const char *fmt, va_list args );
int    vsprintf( char *buf, const char *fmt, va_list args );

FILE  *fdopen( int handle, const char *mode );
int    fileno( FILE *stream );



#define  getchar()  getc(stdin)
#define  putchar(c) putc(c,stdout)

int isatty( int handle );
/*
 * Routines that are purely local.
 */
int  vscanf __P((const char *, va_list));
int  vsscanf __P((const char *, const char *, va_list));
int  snprintf __P((char *str, size_t n, char const *fmt, ...));
int vfscanf( FILE *fp, const char *fmt, va_list ap);

#if !defined (_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
int _vfprintf(FILE *fp,const char *fmt0,va_list ap);
int _fprintf(FILE *fp,const char *fmt0,...);
#endif

#endif      /* !_STDIO_H */
