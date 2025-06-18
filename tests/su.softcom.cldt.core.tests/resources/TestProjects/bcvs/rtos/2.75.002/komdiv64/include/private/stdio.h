#ifndef _PRIVATE_STDIO_H
#define _PRIVATE_STDIO_H
#define MULTITHREAD_STDIO

#ifdef MULTITHREAD_STDIO
#include <pthread.h>
#endif /* MULTITHREAD_STDIO */

#ifndef KERNEL
#define KERNEL
#endif

#include <private/objcore.h>

#define   __SLBF   0x0001      /* line buffered */
#define   __SNBF   0x0002      /* unbuffered */
#define   __SRD   0x0004      /* OK to read */
#define   __SWR   0x0008      /* OK to write */
   /* RD and WR are never simultaneously asserted */
#define   __SRW   0x0010      /* open for reading & writing */
#define   __SEOF   0x0020      /* found EOF */
#define   __SERR   0x0040      /* found error */
#define   __SMBF   0x0080      /* _buf is from malloc */
#define   __SAPP   0x0100      /* fdopen()ed in append mode */
#define   __SSTR   0x0200      /* this is an sprintf/snprintf string */
#define   __SOPT   0x0400      /* do fseek() optimisation */
#define   __SNPT   0x0800      /* do not do fseek() optimisation */
#define   __SOFF   0x1000      /* set iff _offset is in fact correct */
#define   __SMOD   0x2000      /* true => fgetln modified _p text */
#define  __SSTE   0x4000      /* true => valid strerror ptr */

/* stdio buffers */
struct __sbuf {
   unsigned char *_base;
   int   _size;
};
struct __sFILE {
   objHEADER     m_objCore;
   unsigned char *_p;   /* current position in (some) buffer */
   int   _r;      /* read space left for getc() */
   int   _w;      /* write space left for putc() */
   short   _flags;      /* flags, below; this FILE is free if 0 */
   short   _file;      /* fileno, if Unix descriptor, else -1 */
   struct   __sbuf _bf;   /* the buffer (at least 1 byte, if !NULL) */
   int   _lbfsize;   /* 0 or -_bf._size, for inline putc */

   /* operations */
   void   *_cookie;   /* cookie passed to io functions */
   int   (*_close) __P((void *));
   int   (*_read)  __P((void *, char *, int));
   fpos_t   (*_seek)  __P((void *, fpos_t, int));
   int   (*_write) __P((void *, const char *, int));
   char  *(*_strerror)( int errcode ) ;

   /* separate buffer for long sequences of ungetc() */
   struct   __sbuf _ub;   /* ungetc buffer */
   unsigned char *_up;   /* saved _p when _p is doing ungetc data */
   int   _ur;      /* saved _r when _r is counting ungetc data */

   /* tricks to meet minimum requirements even when malloc() fails */
   unsigned char _ubuf[3];   /* guarantee an ungetc() buffer */
   unsigned char _nbuf[1];   /* guarantee a getc() buffer */

   /* separate buffer for fgetln() when line crosses buffer boundary */
   struct   __sbuf _lb;   /* buffer for fgetln() */

   /* Unix stdio files get aligned to block boundaries on fseek() */
   int   _blksize;   /* stat.st_blksize (may be != _bf._size) */
   fpos_t   _offset;   /* current lseek offset (see WARNING) */

#ifdef MULTITHREAD_STDIO
   pthread_mutex_t _mutex;
#endif

};


extern objCONTROL_ID      fileObjControlId;
/*поток может быть удален с захваченным мьютексом*/
/*возможно следовало бы иметь 2 набора:
  для случая возможного канселирования
  для его отсутствия
*/
#ifdef MULTITHREAD_STDIO
  void stream_mtx_unlock( void *p );
  #define STREAM_LOCK(a) \
     if(objCHECK(fileObjControlId,a)!=0) return 0; \
      pthread_cleanup_push( stream_mtx_unlock, (void*)a); \
      pthread_mutex_lock(&a->_mutex);

  #define STREAM_UNLOCK(a) pthread_cleanup_pop(1);
/*  #define STREAM_UNLOCK(a) pthread_mutex_unlock(&a->_mutex); \
      pthread_cleanup_pop(0);
*/
  #define STREAM_FREE(a) if(a->_mutex) pthread_mutex_destroy(&a->_mutex);
  #define STREAM_CREATE(a) pthread_mutex_init(&a->_mutex,NULL);OBJ_NAME_MTX(a->_mutex,"stream_mutex")
#else /* MULTITHREAD_STDIO */
  #define STREAM_LOCK(a) if(objCHECK(fileObjControlId,a)!=0) return 0;
  #define STREAM_UNLOCK(a)
  #define STREAM_FREE(a)
  #define STREAM_CREATE(a)
#endif /* MULTITHREAD_STDIO */

#define   __sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))

#define   __sfeof(p)   (((p)->_flags & __SEOF) != 0)
#define   __sferror(p)   (((p)->_flags & __SERR) != 0)
#define   __sclearerr(p)   ((void)((p)->_flags &= ~(__SERR|__SEOF)))
#define   __sfileno(p)   ((p)->_file)

int   __sflush __P((FILE *));
FILE  *__sfp __P((void));
int   __srefill __P((FILE *));
int   __sread __P((void *, char *, int));
int   __swrite __P((void *, char const *, int));
fpos_t   __sseek __P((void *, fpos_t, int));
int   __sclose __P((void *));
void  __sinit __P((void));
void  _cleanup __P((void));
void  __smakebuf __P((FILE *));
int   __swhatbuf __P((FILE *, size_t *, int *));
int   _fwalk __P((int (*)(FILE *)));
int   __swsetup __P((FILE *));
int   __sflags __P((const char *, int *));



/*
 * Return true iff the given FILE cannot be written now.
 */
#define  cantwrite(fp) \
  ((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
   __swsetup(fp))

/*
 * Test whether the given stdio file has an active ungetc buffer;
 * release such a buffer, without restoring ordinary unread data.
 */
#define  HASUB(fp) ((fp)->_ub._base != NULL)
#define  FREEUB(fp) { \
  if ((fp)->_ub._base != (fp)->_ubuf) \
     free((char *)(fp)->_ub._base); \
  (fp)->_ub._base = NULL; \
}

/*
 * test for an fgetln() buffer.
 */
#define  HASLB(fp) ((fp)->_lb._base != NULL)
#define  FREELB(fp) { \
  free((char *)(fp)->_lb._base); \
  (fp)->_lb._base = NULL; \
}



#define   feof(p)      __sfeof(p)
#define   ferror(p)   __sferror(p)

#define    clearerr(p)   ((void)((p)->_flags &= ~(__SERR|__SEOF)))

static __inline int __UNUSED_F __sputc(int _c, FILE *_p)
{
      if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
          return (*_p->_p++ = (unsigned char)_c);
      else
          return (__swbuf(_c, _p));
}


#endif
