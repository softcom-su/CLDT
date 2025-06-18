/* prsmem.h - поддержка сохраняемой памяти */
#ifndef _PPSMEM_H
#define _PPSMEM_H

int   prsmemUserInit( size_t size );
int   prsmemUserPut( void* p, size_t size, int off );
int   prsmemUserGet( void* p, size_t size, int off );

int   prsmemProcInit( size_t size );
int   prsmemProcPut( void* p, size_t size, int off );
int   prsmemProcGet( void* p, size_t size, int off );

#endif /* _PPSMEM_H */
