/* file.h */

#ifndef _FILEPRIV_H
#define _FILEPRIV_H

#include   <stdio.h>
#include   <private/objcore.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {                     /* стандартные файлы */
	FILE_STDIN =       0,
	FILE_STDOUT =      1,
	FILE_STDERR =      2
};

typedef struct __sFILE fileCONTROL;

typedef struct __sFILE *fileCONTROL_ID;

int fileInit( int num );
FILE  *fileCreate( void );
int fileDestroy( FILE *pf );
int fileStdInit(int fd);
void   fileSetFile( FILE *fp, int fd, int flags, int mtx );

#ifdef __cplusplus
}
#endif

#endif /* _FILEPRIV_H */
