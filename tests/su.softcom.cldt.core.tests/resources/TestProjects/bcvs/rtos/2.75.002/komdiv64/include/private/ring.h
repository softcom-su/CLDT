/*
Copyright

*/
#ifndef _PRIVATE_RING_H
#define _PRIVATE_RING_H

#include <sys/cdefs.h>

#ifndef _POSIX_SOURCE
#ifndef min
#define min(a,b) ((a)<(b)? (a):(b))
#endif

struct  rngbuf {
    int size;
    char *to,*from,*toa;
    char buf;
};

struct  rngbuf *ringInit(int size);
void    ringRemove(struct rngbuf *q);

static __inline int __UNUSED_F ringBytes(struct rngbuf *q)
{
    register int n = q->to - q->from;
    if (n < 0)  n += q->size;
    return (n);
}

static __inline int __UNUSED_F ringBytesAhead(struct rngbuf *q)
{
    register int n = q->toa - q->to;
    if (n < 0)  n += q->size;
    return (n);
}
static __inline int __UNUSED_F ringFree(struct rngbuf *q)
{
    register int n = q->toa - q->from;
    if (n < 0)  n += q->size;
    return (q->size-n-1);
}

#define ringIsEmpty(q)  ((q)->from==(q)->to)
#define ringIsEmptyA(q) ((q)->toa==(q)->to)
#define ringIsFull(q)   (ringFree(q)==0)
#define ringFlush(q)    {int l = intLock(); ((q)->from=(q)->to=(q)->toa=&(q)->buf); intUnlock(l);}
#define ringFlushA(q)   ((q)->toa=(q)->from)

/*
static int  ringPut(struct rngbuf *q,char *buf,int nbyte);
static int  ringPutc(struct rngbuf *q,char buf);
static int  ringPutA(struct rngbuf *q,char *buf,int nbyte);
static int  ringGet(struct rngbuf *q,char *buf,int nbyte);
*/

static __inline int __UNUSED_F ringPutAhead(struct rngbuf *q,char *buf,int nbyte)
{
    register int n,n1;
    register char *from=q->from,*to=q->toa;
    if(ringIsFull(q)) return 0;
    if(from > to ) 
    {
        n=from-to-1;
        if(n)
        {
            n=min(n,nbyte);
            memcpy(to,buf,n);
            to+=n;
        }
    } else  
    {
        if(from==&q->buf)
        {
            n=q->size-(to-&q->buf)-1;
            if(n)
            {
                n=min(n,nbyte);
                memcpy(to,buf,n);
                to+=n;
            }
        } else
        {
            n=q->size-(to-&q->buf);
            n=min(n,nbyte);
            memcpy(to,buf,n);
            nbyte -=n;
            if(nbyte >0) 
            {
                to=&q->buf;
                n1=from-to-1;
                n1=min(n1,nbyte);
                memcpy(to,buf+n,n1);
                to=to+n1;
                n+=n1;
            } else 
            {
                to+=n;
            }
        }
    }
    if(to==q->size+&q->buf)  q->toa=&q->buf;
    else                     q->toa=to;
    return n;
}

static __inline int __UNUSED_F ringPut(struct rngbuf *q,char *buf,int nbyte)
{
    register int n=ringPutAhead(q,buf,nbyte);
    q->to=q->toa;
    return n;
}

static __inline int __UNUSED_F ringGetAhead(struct rngbuf *q)
{
    register char *from=q->to,*to=q->toa;
    if(from == to) return 0;
    if(to == &q->buf) to=&q->buf+q->size-1;
    else    to--;
    q->toa=to;
    return *to;
}

static __inline int __UNUSED_F ringGet(struct rngbuf *q,char *buf,int nbyte)
{
    register int n,n1;
    register char *from=q->from,*to=q->to;
    if(to==from) return 0;
    if(to > from ) {
        n=to-from;
        n=min(n,nbyte);
        memcpy(buf,from,n);
        from+=n;
    } else {
        n=q->size-(from-&q->buf);
        n=min(n,nbyte);
        memcpy(buf,from,n);
        nbyte-=n;
        if(nbyte>0) {
            from=&q->buf;
            n1=to-from;
            n1=min(n1,nbyte);
            memcpy(buf+n,from,n1);
            from+=n1;
            n+=n1;
        } else {
            from+=n;
        }
    }
    if(from==q->size+&q->buf) q->from=&q->buf;
    else                      q->from=from;
    return n;
}
static __inline int __UNUSED_F ringGetChar(struct rngbuf *q,char *buf)
{
    register char *from=q->from,*to=q->to;
    if(to==from) return 0;

    *buf=*from++;
    if(from==q->size+&q->buf) q->from=&q->buf;
    else                      q->from=from;
    return 1;
}
/*
 * Переместить ahead в основной буфер и вернуть число байт в буфере
 */
static __inline int __UNUSED_F ringPutEmpty(struct rngbuf *q)
{
    q->toa=q->to;
    return ringBytes(q);
}
/* 
 *  Положить в кольцевой буфер байт если места нет -- не класть 
 */
static __inline int __UNUSED_F ringPutChar(struct rngbuf *q,char buf)
{
    register char *to=q->toa;
    if(ringFree(q)==0) return 0;
    
    *to++=buf;
    if(to==q->size+&q->buf)  q->toa=q->to=&q->buf;
    else                     q->toa=q->to=to;
    return 1;
}
/* 
 *  Положить в кольцевой буфер байт с перехлестом
 */
static __inline int __UNUSED_F ringPutCharOverwrite(struct rngbuf *q,char buf)
{
    char ch;
    register char *to=q->toa;
    if(ringFree(q)==0) ringGetChar(q,&ch);
    
    *to++=buf;
    if(to==q->size+&q->buf)  q->toa=q->to=&q->buf;
    else                     q->toa=q->to=to;
    return 1;
}
static __inline int __UNUSED_F ringPutCharAhead(struct rngbuf *q,char buf)
{
    register char *to=q->toa;
    if(ringFree(q)==0) return 0;
    
    *to++=buf;
    if(to==q->size+&q->buf)  q->toa=&q->buf;
    else                     q->toa=to;
    return 1;
}

#endif  /* _POSIX_SOURCE */
#endif /* _PRIVATE_RING_H */


