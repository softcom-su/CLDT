/* tracestream.h */
#ifndef _TRACE_TRACESTREAM_H
#define _TRACE_TRACESTREAM_H

#ifdef INCLUDE_TRACE

#define TRACE_EVENT_UNNAMED   "unnamed"

#if BYTE_ORDER == BIG_ENDIAN
#define TRC_SIGNATURE           (' '<<3*8)+('T'<<2*8)+('R'<<1*8)+'C' /* TRC  */
#define TRCATTR_SIGNATURE       ('A'<<3*8)+('T'<<2*8)+('R'<<1*8)+'C' /* ATRC */
#else
#define TRC_SIGNATURE           ('C'<<3*8)+('R'<<2*8)+('C'<<1*8)+' '
#define TRCATTR_SIGNATURE       ('C'<<3*8)+('R'<<2*8)+('T'<<1*8)+'A'
#endif
/* Проверяем инициализацию атрибутов трассы */
#define traceIS_ATTR_INIT(a)  ( (a)->signature == TRCATTR_SIGNATURE )

/* Структура, передающая пар-ры в SYSCALL */
struct trace_create {
   pid_t       pid;
   const trace_attr_t*
               trattr;
   int         fildes;
//   struct trace_descriptor*
//               descriptor;
   trace_id_t* trid;
   char        filename[PATH_MAX];
};

/* Структура, передающая пар-ры в SYSCALL */
struct trace_getnext {
   trace_id_t  trid;
   struct posix_trace_event_info*
               event;
   void*       data;
   size_t      num_bytes;
   size_t*     data_len;
   int*        unavailable;
   const struct timespec*
               abs_timeout;
};

struct trace_setfiler {
    pid_t                       pid;
    trace_id_t                  trid;
    const trace_event_set_t*    set;
    int                         how;
};

struct trace_attrinit {
    pid_t         pid;
    trace_attr_t* attr;
};

/* Описатель  группы записей */
struct trace_getnext_group {
   char* pBlockRec;  /* указатель на начало группы записей                 */
   int   blocksize;  /* размер группы                                      */
   struct posix_trace_event_info
         info;       /* описатель события группы                           */
   char* pRec;       /* указатель на последнюю прочитанную запись в группе */
   int   recsize;    /* ее размер                                          */
};

/* Описатель потока трассы процесса, используемый при доступе к буферу через
 * posix_trace_getnext_event()
 */
struct trace_descriptor {
  int                            signature;
  traceBLOCK_HEADER*             pBlock;     /* адрес взятого из буфера блока             */
  int                            nBlock;     /* номер взятого из буфера блока             */
  int                            sizeBlock;  /* размер прочитанного блока                 */
//  int                            offTo;
  char*                          pTo;        /* адрес свбодного места в буфере в момент
                                                                 чтения из него данных    */
  char*                          pRec;       /* адрес последней прочитанной записи        */
  int                            sizeRec;    /* размер последней прочитанной записи       */
  int                            status;     /* состояние взятого из потока блока         */
  struct trace_getnext_group     group;      /* описатель группы записей */
  char                           buffer[TRACE_BLOCKSIZE]; /* обрабатываемый блок событий  */
};


#ifdef KERNEL

/* Обслуживание функций непосредственного чтения из буфера */
#define traceAVAILABLE            0
#define traceUNAVAILABLE          1
#define traceIS_AVAILABLE(w)      ((w) == traceAVAILABLE)
#define traceSET_AVAILABLE(w)     w = traceAVAILABLE
#define traceUNSET_AVAILABLE(w)   w = traceUNAVAILABLE

extern int  traceCreateWithlog( struct trace_create* par );
extern int  traceGetnextEvent( struct trace_getnext* par );
extern void traceNextThreadRelease( traceHEADBUF pBufHeader, int pid );
extern void traceShutdownComplete( pid_t pid, traceHEADBUF pBufHeader );

int _traceSetFilter( struct trace_setfiler* param );
int _traceAttrInit( struct trace_attrinit* param );

#endif /*KERNEL*/
#endif /*INCLUDE_TRACE*/

#endif /*_TRACE_TRACESTREAM_H*/
