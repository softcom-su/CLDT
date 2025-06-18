#ifndef KERNEL
  #define KERNEL
#endif


#include <public/difference.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <hash.h>
#include <time.h>
#include <termios.h>
#include <mem.h>
#include <math.h>
#include <errno.h>
#include <semaphore.h>
#include <ossys.h>
#include <sys/socket.h>
#include <sys/systm.h>
#include <sys/ioctl.h>
#include <sys/shm.h>
#include <sys/mount.h>
#include <private/ios.h>
#include <private/oslib.h>
#include <private/kernel.h>
#include <private/mutex.h>
#include <private/sysdef.h>
#include <private/cond.h>
#include <private/charset.h>
#include <private/sempriv.h>
#include <prsmem/prsmem.h>
#include <arch/interrupt.h>
#include <arch/testmem.h>
#include <locale.h>
#include <ctype.h>
#include <busnet.h>
#include <try.h>
#include <trace.h>

#define __CONFIGURE__

#define MP_NETWORK_BUSNET   1
#define MP_NETWORK_VITA     2

#include "board.h"
#include "configos.h"
#include "configbrd.h"

#define   FMT "** %d ** start %-40s  ..."
#if defined(INCLUDE_SHOW_INIT) || defined(INCLUDE_SHOW_BOOT_TIME)
  #define	KPRINT	kprint
  #define	PRINTF	printf
  #define KCHECK(a) { int error; kprint("start %s ...",#a); error=a; kprint("%s\r\n",error==0?"Ok":"Fail");}
  #define VKCHECK(a) { kprint("start %s ...",#a); a; kprint("done\r\n");}
  #define VKCHECK1(a) { kprint("start %s ...\n",#a); a; kprint("%s done\r\n",#a);}
  #define VPCHECK(a) { fprintf(stderr,"start %s ...",#a); a; fprintf(stderr,"done\n");}
  #define VPCHECK1(a) { fprintf(stderr,"start %s ...\n",#a); a; fprintf(stderr,"%s done\n",#a);}
  #define PCHECK(a) {   char buf[100],*c=buf;\
    error=a;c+=sprintf(buf,FMT,(int)0,#a);\
    if(error) { sprintf(c,"Fail(%d)\r\n",error); total_error++;}\
    else sprintf(c,"Ok\r\n");printf("%s",buf);}
#else	/* not (defined(INCLUDE_SHOW_INIT) || defined(INCLUDE_SHOW_BOOT_TIME)) */
  #define	PRINTF	KPRINT
  static int KPRINT(char* fmt,...)
  {
    return 0;
  }
  #define VKCHECK(a) a
  #define VPCHECK(a) a
  #define VPCHECK1(a) a
  #define PCHECK(a) error = a; if(error) { printf("\n%s Error(%d)\n",#a,error); total_error++;}
#endif	/* INCLUDE_SHOW_BOOT_TIME */


#include "board.c"
#include "configbrd.c"

#if defined(INCLUDE_MULTIPROCESSING) && defined(INCLUDE_MP_OBJS)
#define MP_OFFSET   MP_MEM_OBJECTS
#else
#define MP_OFFSET   0
#endif

/* включение функций указанных библиотек C */
#include "clibconfig.c"


#ifdef INCLUDE_NETWORK
#include "netconfig.c"
#endif /* INCLUDE_NETWORK */


#if defined(INCLUDE_MULTIPROCESSING) && defined(INCLUDE_MP_OBJS)
  #include "mpconfig.c"
#endif /* INCLUDE_MP_OBJS*/

#if (INCLUDE_MULTIPROCESSING) && !(INCLUDE_MP_OBJS)
#  if INCLUDE_CHANNELS && CONNECTION_CONFIG
#     include "portconfig.c"
#     include "arinc653.c"
#     ifdef INCLUDE_TRACE
#        include  <traceChanConfig.c>
#     endif
#  endif /* CONNECTION_CONFIG */
#endif /* INCLUDE_MP_OBJS */

#include "prsmemconfig.c"

#ifdef INCLUDE_TRACE
#  include  <traceSetMask.c>
#  include  <traceSetOptions.c>
#  include  "traceconfig.c"
#endif


void __exit(int code)
{
	cpuReboot();
}

#include "initos.c"
