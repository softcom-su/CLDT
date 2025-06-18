/*
 * initos.c - configure OC2000 kernel
 */

extern char   _edata[];       /* automatically defined by loader */
extern char   _end[];         /* automatically defined by loader */
char  *osFreeRamAdr = (char*)FREE_RAM_ADRS;
/*
 * Описание функций
 */
void    osKernelInit(void);       /* Инициализация ядра */
void   *osRootThread(void *arg);  /* Стартовый поток ОС */
void   *osDemoThread (void *arg); /* Демонстрационный поток ОС */
void    cacheInit();              /* Инициализация кеша команд и данных () */

#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <mqueue.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <fenv.h>
#include <sys/ioctl.h>
#include <net/netutils.h>
#include <private/errset.h>
#include <private/charset.h>
#include <private/rusfont.h>
#include <private/symtbl.h>
#include <private/systime.h>
#include <private/mempool.h>
#include <bfsck.h>

pthread_t shellTCB;
static int reportFlags ;

#if (INCLUDE_ENVIRONMENT) && defined (ENVIRONMENT)
#  define SET_ENVIRONMENT
   static void SetEnvironment( const char *env );
#endif  /*ENVIRONMENT*/

#include  "flashmemconfig.c"

int getReportFlags(void)
{
  return reportFlags ;
}

#undef  INCLUDE_SHOW_BOOT_TIME

#ifdef INCLUDE_VALIDATION

#include <validation.h>


/**
 * @brief Инициализация средств валидации
 */
void validationClassesInit(void)
{
    validationInit();
    /*
     * Классы, добавляемые безусловно
     */
    validationSET_CLASS_FUNC(CLASS_SYSCONTROL, validateControlClass, dumpObjControl);
    validationSET_CLASS_FUNC(CLASS_THREADCONTROL, validateThreadControl, dumpThreadControl);
    validationSET_CLASS_FUNC(CLASS_MEMCONTROL, validateMemPool, dumpMemPool);
    validationSET_CLASS_FUNC(CLASS_MTXCONTROLPX, validateMutex, dumpMutex);
    validationSET_CLASS_FUNC(CLASS_CLCCONTROLPX, validateClock, dumpClock);

#ifdef INCLUDE_POSIX_SEMAPHORES
    validationSET_CLASS_FUNC(CLASS_SEMCONTROL, validateSem, dumpSem);
//    validationSET_CLASS_FUNC(CLASS_SEMCONTROLPX, validateOS2Sem, dumpOS2Sem);
#endif

#ifdef INCLUDE_POSIX_CND
    validationSET_CLASS_FUNC(CLASS_CNDCONTROLPX, validateCond, dumpCond);
#endif

#ifdef INCLUDE_TIMERS
	validationSET_CLASS_FUNC(CLASS_TMRCONTROLPX, validateTimer, dumpTimer);
#endif

#ifdef INCLUDE_POSIX_MQ
	validationSET_CLASS_FUNC(CLASS_MQPCONTROL, validateMq, dumpMq);
#endif
}
#endif

#define CODERET_NONE  0x50505050


void osBoardInit(void)
{
    memset (_edata, 0x0, _end - _edata);        /* обнулим bss сегмент */

    KPRINT("Init start\r\n");
    VKCHECK(cacheInit());
    /* Инициализация векторов прерываний */
    VKCHECK(intVecInit(DEFAULT_INTMODE));

    /*
     * Инициализация оборудования
     * прерывания закрыты, печать kprint
     */
#ifdef BOARD_HW_INIT1
        VKCHECK(BOARD_HW_INIT1);
#else
        VKCHECK(boardHWInit());
#endif
    /*
     * Инициализация ядра операционной системы
     * средства просмотра сведений
     */
#if INCLUDE_SHOW_MEMORY
    {
        void memShowInit();
        memShowInit();
    }
#endif
    /*
     * системные объекты
     */
#if INCLUDE_SHOW_OBJECTS
    /*
     * перед инициализацией класса мьютексов - mtxPxInit
     * включаем в образ функцию sysconf
     */
      mtxShowInit();
      sysconf( 0 );
#if INCLUDE_POSIX_SEMAPHORES
    semPxShowInit();
# endif
# ifdef INCLUDE_POSIX_MQ
    mqShowInit();
# endif
# ifdef INCLUDE_POSIX_CND
    cndShowInit();
# endif

    hshShowInit();

    sigShowInit();

#endif

/* Проверка и обработка ошибок системы разпределения памяти */
#if MEMORY_CHECK
    memDefFlags |= memALLOC_CHECK | memFREE_CHECK;
# if MEMORY_ERROR_SUSPEND
    memDefFlags |= memALLOC_ERROR_SUSPEND | memFREE_ERROR_SUSPEND;
# endif
# if MEMORY_ERROR_LOG
    memDefFlags |= memALLOC_ERROR_LOG | memFREE_ERROR_LOG;
# endif
#endif  /*MEMORY_CHECK*/
#if MEMALLOC_CLEAR
    memDefFlags |= memALLOC_CLEAR;
#endif  /*MEMALLOC_CLEAR*/

  {
    /*
     * создаем системный пул и прочее
     */
    KPRINT("Mem start %p, end %p\r\n",
             osFreeRamAdr, (char*)boardMemTop() );
    VKCHECK(kernInit( (char *)osFreeRamAdr, (char*)boardMemTop(), (u_int) ISR_STACK_SIZE ));
  }
#ifdef INCLUDE_PRSMEM
/* Размещение сохраняемой области */
   {
      prsmemShowInit();
      prsmemInit();
   }
#endif   /* INCLUDE_PRSMEM */



  /* Инициализация валидатора */
#ifdef INCLUDE_VALIDATION
  VKCHECK(validationClassesInit());
#endif

    /*
     * Инициализация модулей включенных в систему при конфигурации
     */
    VKCHECK(osKernelInit());
    /*
     * Запуск операционной системы
     */
   kernOsStart(osRootThread);
}

void osKernelInit(void)
{
#if  INCLUDE_PRIO256_RDY_QUE
    /* Смена типа очереди активных потоков */
    queInitModule( queTYPE_PRIO256 );
    VKCHECK(kernReadyThreadQue = *((queHEAD *)queCreate(queTYPE_PRIO256, NULL)));
#endif
#if  INCLUDE_SPAN256_CLK_QUE
    /* Смена типа очереди к системному таймеру */
    queInitModule( queTYPE_SPAN256 );
    VKCHECK(kernTimerQue = *((queHEAD *)queCreate(queTYPE_SPAN256, (objHEADER *)&kernClock)));
    queSetDpcRtn(&kernTimerQue, kernClockDpcRtn);
#endif
    /* Инициализация таймеров */
#if INCLUDE_TIMERS
    {
        int     tmrPxInit(int);
        VKCHECK(tmrPxInit(TIMER_MAX_CFG));
#if INCLUDE_SHOW_OBJECTS
        {
            void    tmrPxShowInit();
            VKCHECK(tmrPxShowInit());
        }
#endif
    }
#endif
    /* Инициализация сигналов */
#if INCLUDE_SIGNALS
    {
        int sigPxInit( int );
        int count =  SIGQUEUE_MAX;
        #if INCLUDE_TIMERS
        count += TIMER_MAX_CFG ;
        #endif
        VKCHECK(sigPxInit( count ));
    }
#endif

    /* Инициализация семафоров */
#if INCLUDE_POSIX_SEMAPHORES
    {
        int semPxInit(int);
        /* Инициализация семафоров POSIX */
        VKCHECK(semPxInit( SEM_NSEMS_MAX_CFG ));
    }
#endif
    /* Инициализация ввода-вывода */
    VKCHECK(ioInit( OPEN_MAX_CFG ));
    /*
     * Иницализация системы ввода-вывода (потоки)
     */
    {
      int     fileInit( int );
      VKCHECK(fileInit( STREAM_MAX_CFG ));
    }
    /* Инициализация очередей сообщений */
#if INCLUDE_POSIX_MQ
    VKCHECK(hshInit());
    /* тип очереди задержанных сообщений */
#if INCLUDE_PRIO32_MQ_QUE
    VKCHECK(mqpPxInit(MQ_OPEN_MAX_CFG,1));
#else
    VKCHECK(mqpPxInit(MQ_OPEN_MAX_CFG,0));
#endif
#endif

    /* Инициализация cond-переменных */
#if INCLUDE_POSIX_CND
    VKCHECK(cndPxInit());
#endif
}
/*
 * Инициализация виртуальной файловой системы
 */
int vfs_init()
{
    int vfsinit(int,int,int);
    int default_vnodeop_opv_descinit();
    int rootfs_vnodeop_opv_descinit();
    int spec_vnodeop_opv_descinit();
    int msdosfs_vnodeop_opv_descinit();
    int cd9660_vnodeop_opv_descinit();
    int ftpfs_vnodeop_opv_descinit();
    int tarfs_vnodeop_opv_descinit();
    int nfsv2_vnodeop_opv_descinit();
    int default_vnodeop_opv_descinit();
    int dead_vnodeop_opv_descinit();
    int procfs_vnodeop_opv_descinit();
    int root_fsinit();
    int dev_fsinit();
    int msdos_fsinit();
    int cd9660_fsinit();
    int ftp_fsinit();
    int tar_fsinit();
    int nfs_fsinit();
    int proc_fsinit();

    vfsinit(VFS_VNODES_MAX,VFS_BUFFERS_MAX,VFS_MALLOC_MAX);

    default_vnodeop_opv_descinit();
    dead_vnodeop_opv_descinit();
    rootfs_vnodeop_opv_descinit();
    spec_vnodeop_opv_descinit();
#if INCLUDE_PROC
    procfs_vnodeop_opv_descinit();
#endif
#if INCLUDE_VFAT
    msdosfs_vnodeop_opv_descinit();
#endif
#if INCLUDE_CD9660
    cd9660_vnodeop_opv_descinit();
#endif
#if INCLUDE_TAR_FS
    tarfs_vnodeop_opv_descinit();
#endif
#if INCLUDE_NETWORK
#if INCLUDE_FTP_FS
    ftpfs_vnodeop_opv_descinit();
#endif
#if INCLUDE_NFS
    nfsv2_vnodeop_opv_descinit();
#endif
#endif
    root_fsinit();
    dev_fsinit();
    __mount("dev","/dev",0,0);
#if INCLUDE_PROC
    proc_fsinit();
    __mount("proc","/proc",0,0);
#endif
#if INCLUDE_VFAT
    msdos_fsinit();
#endif
#if INCLUDE_CD9660
    cd9660_fsinit();
#endif
#if INCLUDE_FTP_FS
    ftp_fsinit();
#endif
#if INCLUDE_TAR_FS
    tar_fsinit();
#endif
#if INCLUDE_NETWORK
#if INCLUDE_NFS
    nfs_fsinit();
#endif
    {
        void vfs_export_init();
        vfs_export_init();
    }
#endif
    return 0;
}
/*----------- Описание функции setExceptionPF: ----------*/
void setExceptionPF(int num, funcptr_t excPF) ;
/*
 *  ------------------------------------
 *  Стартовый поток операционной системы
 */
void *osRootThread(void *arg)
{
    pthread_t   osInitThread_t;
    pthread_attr_t attr;
    sigset_t  msig;
    int coderet = CODERET_NONE;
    struct sched_param param;

    KPRINT("osRootThread start\r\n");

    /*корневой поток будет иметь нижеустановленную маску
      (сигналы блокированы), что позволяет создать все служебные
      потоки с такой же маской; исключение составляет
      порождаемый пользовательский поток, у которого все
      сигналы могут обрабатываться; возможно эту маску следует
      конфигурировать
    */
    sigfillset(&msig);
    pthread_sigmask (SIG_SETMASK, &msig, NULL);

    bzero(&attr,sizeof(attr));
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr,DEFAULT_STACK_SIZE);
    pthread_attr_setinheritsched(&attr, DEFAULT_INHERITSCHED);
    pthread_attr_setschedpolicy(&attr, DEFAULT_SCHEDPOLICY);        /* политика планирования работы нити */
    param.sched_priority= DEFAULT_SCHED_PRIORITY;                   /* параметры для планирования */
    pthread_attr_setschedparam(&attr,&param);
    pthread_attr_setfpustate(&attr,DEFAULT_FPUSTATE);       /* будет ли поток использовать сопроцессор */
#ifdef INCLUDE_TRACE
    /*установим значение трассировки потока по умолчанию*/
    if( traceIS_THREAD_USER )
      pthread_attr_settracestate( &attr, PTHREAD_TRACE_ENABLE );
    else
      pthread_attr_settracestate( &attr, PTHREAD_TRACE_DISABLE );
#endif

    pthreadSetDefaults( &attr );
    /* Установить значение кванта для режима планирования SCHED_RR*/
    kernSetThreadSlice( DEFAULT_THREAD_QUANT );


#if  INCLUDE_DEMO_THREAD ||  INCLUDE_USER_THREAD ||  INCLUDE_SHELL

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
    pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setfpustate(&attr,PTHREAD_FPU_ENABLE);
#endif


    {
        int procinit(int cron_prio);
        VKCHECK(procinit(200));
    }


  /*
     * Инициализация дополнительного оборудования
     * работает nanosleep malloc
     */
#ifdef BOARD_HW_INIT2
        VKCHECK(BOARD_HW_INIT2);
#else
        VKCHECK(bHWInit());
#endif

#if INCLUDE_CONSOLE
#ifndef CONSOLE_DEVICE
#define CONSOLE_DEVICE "/dev/null"
#endif
#else   /* INCLUDE_CONSOLE */
#undef CONSOLE_DEVICE
#define CONSOLE_DEVICE "/dev/null"
#endif  /* INCLUDE_CONSOLE */
    {
        int  fd;
        char *devname=CONSOLE_DEVICE;

        if((fd=open(devname,O_RDWR,0))==-1)
        {
            devname="/dev/null";
            fd=open(devname,O_RDWR,0);
        }
        KPRINT("open(%s)=%d\n\r",devname,fd);
        fd=open(devname,O_RDWR,0);
        fd=open(devname,O_RDWR,0);
    }

    localeInit();
    setlocale(LC_ALL,"C@short");

#if INCLUDE_DEBUG
# ifndef DEBUG_PRIORITY
#  define DEBUG_PRIORITY 240
# endif
    {
        int ldebugThreadInit () ;
        int ldbInclude() ;
        VKCHECK(ldebugThreadInit(DEBUG_PRIORITY));
        VKCHECK(ldbInclude());
    }
#endif /* INCLUDE_DEBUG */

/* Конфигурирование выдачи информации в случае exception (7 функций) */
/* function number 0 */
#if INCLUDE_REPORT_DISASS
    {
     int __ldbList() ;
     reportFlags |= 0x01 ;
     setExceptionPF(0, __ldbList) ;
    }
#endif /* INCLUDE_REPORT_DISASS */
/* function number 1 */
#if INCLUDE_REPORT_BACKTR
    {
     int traceStack() ;
     reportFlags |= 0x02 ;
     setExceptionPF(1, traceStack) ;
    }
#endif /* INCLUDE_REPORT_BACKTR */
/* function number 2 */
#if INCLUDE_REPORT_THREADS
    {
     int __threadList() ;
     reportFlags |= 0x04 ;
     setExceptionPF(2, __threadList) ;
    }
#endif /* INCLUDE_REPORT_THREADS */
/* function number 3 */
#if INCLUDE_REPORT_FLREGS
    {
     int fppRegsShow() ;
     reportFlags |= 0x08 ;
     setExceptionPF(3, fppRegsShow) ;
    }
#endif /* INCLUDE_REPORT_THREADS */
/* function number 4 */
#if (HM_CHECK_OBJECTS) && (INCLUDE_VALIDATION)
    {
      extern struct validation_info *validationInfo;
      reportFlags |= 0x10 ;
      setExceptionPF(4, validationInfo->hm_validation_func) ;
    }
#endif /* HM_CHECK_OBJECTS && INCLUDE_VALIDATION */
/* function number 5 */
#if HM_REPORT_USER
# ifdef  none
#  undef none
# endif
# define  none 1
# if (HM_FUNTION_NAME != none)
    {
      int HM_FUNTION_NAME() ;
      reportFlags |= 0x20 ;
      setExceptionPF(5, HM_FUNTION_NAME) ;
    }
# endif /* HM_FUNTION_NAME != none */
#endif /* HM_REPORT_USER */
/* function number 6 */
#if defined(SIZE_FPAREA) && (SIZE_FPAREA != 0)
    {
     int __memDi() ;
     extern int __sizeofFParea ;
     reportFlags |= 0x40 ;
     __sizeofFParea = SIZE_FPAREA ;
     setExceptionPF(6, __memDi) ;
    }
#endif /* INCLUDE_SIZE_FPAREA */

#ifndef INCLUDE_PRSMEM
#undef INCLUDE_TRACE
#endif

/* Инициализация областей во flash-памяти */
#ifdef INCLUDE_FLASHMEM
 {
  int error, total_error = 0 ;
  PCHECK(flashmemInit()) ;
 }
#endif



    /* Инициализация системного журнала        */
#ifdef INCLUDE_SYSLOG
#define  console             1
#define  file                2
#define  persistent_memory   3
#define  flash_memory        4
#define  user_functions      5
#define  begin               1
#define  end                 2
#define  loop                3
   {
      int syslogStart();
      int setFlashMemory() ;
      int _setFlashMemory() ;
# if  defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO==user_functions)
# define none  1
#  if (SYSLOG_OPEN_FUNCTION != none)
      int SYSLOG_OPEN_FUNCTION();
#  endif
#  if (SYSLOG_OUTPUT_FUNCTION != none)
      int SYSLOG_OUTPUT_FUNCTION();
#  endif
#  if (SYSLOG_CLOSE_FUNCTION != none)
      int SYSLOG_CLOSE_FUNCTION();
#  endif
# endif /* OUTPUT_MSG_TO==user_functions */
      struct syslog_start sls;         /* описание конфигурации системного журнала     */
      void *flashOpn() ;
      int flashWrt() ;
      void *prsmemOpn() ;
      int prsmemWrt() ;
      void *syslogOpn() ;
      int syslogWrt() ;
      int syslogCls() ;
      int error, total_error = 0 ;
      bzero(&sls,sizeof(sls));
      sls.sls_BufSize  = SYSLOG_BUF_SIZE_CFG + 3;/* размер буфера системного журнала   */
      sls.sls_BufSize &= 0xFFFFFFFC ;            /* должен быть кратен 4               */
      sls.sls_BufCount = SYSLOG_BUF_COUNT_CFG;   /* число буферов системного журнала   */
#ifdef NOMESSAGE_HEADER
      sls.sls_flags |= LOG_MSG_NOHEADER ;   /* сообщения журнала не содержат заголовок */
#endif
#ifdef INCLUDE_CONSOLE
      sls.sls_console = CONSOLE_DEVICE ;
      sls.sls_flags |= LOG_MSG_TO_CONSOLE ; /* сообщения журнала выводить на консоль */
#else
      sls.sls_flags |= LOG_MSG_TO_STDOUT ;
#endif
#if defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == file)
//     if (OUTPUT_MSG_TO == file)
     {
#ifdef NAME_OUTSTREAM
        if (strlen(NAME_OUTSTREAM) > 0)
        {
            sls.sls_flags &= ~LOG_MSG_TO_CONSOLE ;
            sls.sls_flags |= LOG_MSG_TO_FILE ;
            sls.sls_filename = NAME_OUTSTREAM ;
            sls.sls_flags |= LOG_MSG_TO_USERFUNC ;
            sls.sls_Open = syslogOpn ;
            sls.sls_Write = syslogWrt ;
            sls.sls_Close = syslogCls ;
        }
#endif
     }
#endif /* OUTPUT_MSG_TO == file */
#if defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == persistent_memory)
//     if (OUTPUT_MSG_TO == persistent_memory)
     {
         sls.sls_flags &= ~LOG_MSG_TO_CONSOLE ;
         sls.sls_flags = LOG_MSG_TO_MEMORY ;
         sls.sls_Open = prsmemOpn ;
         sls.sls_Write = prsmemWrt ;
     }
#endif /* OUTPUT_MSG_TO == persistent_memory */

#if (defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == persistent_memory))
     {
# ifdef MOVE_MEMORY
        sls.sls_memory_file = MOVE_MEMORY ;
        sls.sls_moveMemory2File = prsmemMove2File_ ;
# if defined(MOVE_TYPE)
#  if (MOVE_TYPE == begin)
        sls.sls_sizeF = MAX_COUNT_FILES ;
#  endif /* MOVE_TYPE == begin */
#  if (MOVE_TYPE == end)
        sls.sls_sizeF = 0 ;
#  endif /* MOVE_TYPE == end */
#  if (MOVE_TYPE == loop)
        sls.sls_sizeF = MOVE_MEMORY_SIZE ;
#  endif /* MOVE_TYPE == loop */
# endif /* MOVE_TYPE */
# endif /* MOVE_MEMORY */
# ifdef MEM_FULL_POLICY
        sls.sls_flags |= LOG_MEM_FULL_POLICY ;
# endif
# ifdef NAME_PMEMORY
        sls.sls_prsmem_name = NAME_PMEMORY ;
# endif
# ifdef INCLUDE_PRSMEM
        sls.sls_prsmem_Find = prsmemPartFind ;
# endif
     }
#endif /* persistent_memory */

#if defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == flash_memory)
//     if (OUTPUT_MSG_TO == flash_memory)
     {
         sls.sls_flags &= ~LOG_MSG_TO_CONSOLE ;
         sls.sls_flags |= LOG_MSG_TO_FLASH ;
         sls.sls_Open = flashOpn ;
         sls.sls_Write = flashWrt ;
     }
#endif /* OUTPUT_MSG_TO == flash_memory */

#if 0 && defined(OUTPUT_EMERG_TO) && (OUTPUT_EMERG_TO == flash_memory)
//     if (OUTPUT_EMERG_TO == flash_memory)
     {
         sls.sls_flags |= LOG_EMERG_TO_FLASH ;
         sls.sls_OpenEmerg = flashOpn ;
         sls.sls_WriteEmerg = flashWrt ;
     }
#endif /* OUTPUT_EMERG_TO == flash_memory */

#if (defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == flash_memory))
     {
# if defined(ADDRESS_MEMORY) && defined(SIZE_MEMORY)
      int *addr, size ;
# endif
# ifdef ERASE_MEMORY
        sls.sls_flags |= LOG_ERASE_FLASHMEMORY ;
# endif
# ifdef MOVE_MEMORY
        sls.sls_memory_file = MOVE_MEMORY ;
        sls.sls_moveMemory2File = flashMove2File_ ;
# if defined(MOVE_TYPE)
#  if (MOVE_TYPE == begin)
        sls.sls_sizeF = MAX_COUNT_FILES ;
#  endif /* MOVE_TYPE == begin */
#  if (MOVE_TYPE == end)
        sls.sls_sizeF = 0 ;
#  endif /* MOVE_TYPE == end */
#  if (MOVE_TYPE == loop)
        sls.sls_sizeF = MOVE_MEMORY_SIZE ;
#  endif /* MOVE_TYPE == loop */
# endif /* MOVE_TYPE */

# endif /* MOVE_FLASH */
# if defined(ADDRESS_MEMORY) && defined(SIZE_MEMORY)
        addr = (int *)ADDRESS_MEMORY ;
        size = SIZE_MEMORY ;
        PCHECK(setFlashMemory(&sls.sls_flash, addr, size));
# endif
# if defined(NAME_FMEMORY)
        PCHECK(_setFlashMemory(&sls.sls_flash, NAME_FMEMORY));
# endif
     }
#endif /* flash_memory */

#if defined(OUTPUT_MSG_TO) && (OUTPUT_MSG_TO == user_functions)
     {
         sls.sls_flags &= ~LOG_MSG_TO_CONSOLE ;
         sls.sls_flags |= LOG_MSG_TO_USERFUNC ;
#if (SYSLOG_OPEN_FUNCTION != none)
         sls.sls_Open = SYSLOG_OPEN_FUNCTION ;
#endif
#if (SYSLOG_OUTPUT_FUNCTION != none)
         sls.sls_Write = SYSLOG_OUTPUT_FUNCTION ;
#endif
#if (SYSLOG_CLOSE_FUNCTION != none)
         sls.sls_Close = SYSLOG_CLOSE_FUNCTION ;
#endif
     }
#endif /* OUTPUT_MSG_TO == user_functions */

#ifdef OUTPUT_MSG_CONSOLE
      if ((sls.sls_flags & LOG_MSG_TO_CONSOLE)==0)
         sls.sls_flags |= LOG_MSG_CONSOLE_AS ;
#endif /* OUTPUT_MSG_CONSOLE */



     PCHECK(syslogStart(&sls, SYSLOG_PRIORITY)); /* запуск системного журнала          */
     openlog(                                    /* открытие системного журнала, POSIX */
              "**",             /* префикс сообщения                                   */
              LOG_PID|LOG_CONS, /* выводить ид. проц. и при ошибке выводить на консоль */
              LOG_USER);        /* выводить сообщения посылаемые любым процессом       */
   }
#undef  console
#undef  file
#undef  persistent_memory
#undef  flash_memory
#undef  user_functions
#undef  begin
#undef  end
#undef  loop
#endif  /* INCLUDE_SYSLOG */


/* Инициализация sysShell-демона */
#ifdef  INCLUDE_SYSSHELL
   {
     int systemShellInit(int pri, int sizeBf, int nmbBf, int flg) ;
     int flags = 0, pri, size, cnt ;
     int error, total_error = 0 ;
     pri=SYSSHELL_PRIORITY ;
     size=SYSTEM_BUF_SIZE ;
     cnt=SYSTEM_BUF_COUNT ;
     if (SYSTEM_TO_SYSLOG != 0)
         flags |= STDOUT_TO_SYSLOG ;
     if (SYSTEM_PRINT != 0)
         flags |= SYSTEM_COM_PRINT ;
     PCHECK(systemShellInit(pri, size, cnt, flags)) ;
   }
#endif  /* INCLUDE_SYSSHELL */


#if INCLUDE_SYSLOG && 0
   {
      int syslogInit(int prio,void (*uwf)(int,const char *,size_t));
	  void (*uwf)(int,const char *,size_t) =0;
#if  INCLUDE_SYSLOG_FUNCTION
	  void SYSLOG_FUNCTION();
	  uwf = SYSLOG_FUNCTION;
#endif
      VKCHECK(syslogInit(SYSLOG_PRIORITY,uwf));
   }
#endif
  {
    void *osInitThread( void *p );

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );
    pthread_attr_setname( &attr, "osInitThread" );
    pthread_attr_setsystem( &attr, PTHREAD_CREATE_SYSTEM );
//    traceSET_ATTR_SYSTHREAD( &attr );
    PRINTF("osInitThread start\n");
    pthread_create( &osInitThread_t, &attr, osInitThread, (void*)NULL );
//    KCHECK(pthread_create( &osInitThread_t, &attr, osInitThread, (void*)NULL ));
  }
#if INCLUDE_SYMTBL
    {
        int symInit(),symLoad();
        VPCHECK(symInit(100));
        {
            int i=symLoad();
            PRINTF("symLoad()=%d\n",i);
        }

    }
#endif /* INCLUDE_SYMTBL */

#if (INCLUDE_LOADER)
    {
        void    moduleInit(void) ;
        VPCHECK(moduleInit());
    }
#endif /* INCLUDE_LOADER */

#if  INCLUDE_SHELL
    {
#if INCLUDE_CONSOLE
        void helpInit();
        struct  sched_param sp;
        extern  pthread_attr_t shellAttr;
        static  struct shellStartParam ssp;

        VPCHECK(helpInit());
#if SHELL_PRIORITY
        sp.sched_priority = SHELL_PRIORITY;
#else
        sp.sched_priority = 150;
#endif
        pthread_attr_init(&shellAttr);
        pthread_attr_setinheritsched(&shellAttr, PTHREAD_EXPLICIT_SCHED );
        pthread_attr_setfpustate(&shellAttr,PTHREAD_FPU_ENABLE);
#if (INCLUDE_MULTIPROCESSING) && (INCLUDE_MP_OBJS)
/*  shell не может запускаться до полной инициализации MP
    (ему требуется mpTCB), поэтому исключаем доступ
        pthread_attr_setmpstate( &attr, PTHREAD_MP_ENABLE );
*/
#endif
        pthread_attr_setschedparam( &shellAttr,&sp);
        pthread_attr_setdetachstate( &shellAttr, PTHREAD_CREATE_JOINABLE );
        pthread_attr_setstacksize(&shellAttr,SHELL_STACK_SIZE);
        pthread_attr_setbreakstate( &shellAttr, PTHREAD_BREAK_DISABLE );
        pthread_attr_setsystem( &shellAttr, PTHREAD_CREATE_SYSTEM );
//        traceSET_ATTR_SYSTHREAD( &shellAttr );
        ssp.ssp_exec=SHELL_STARTUP_SCRIPT;
        ssp.ssp_flags = SHELL_SSP_EXEC;
        pthread_create( &shellTCB, &shellAttr, osShell,  &ssp );
#if INCLUDE_DEBUG
        {
            int setThreadStatusBr( int,int);
            setThreadStatusBr( (int)shellTCB, 0) ;
        }
#endif /* INCLUDE_DEBUG */
#endif /* INCLUDE_CONSOLE  */
    }
#endif /* INCLUDE_SHELL */

#if INCLUDE_IEEE754
        VPCHECK(feInit());
#endif

#ifndef INCLUDE_SHOW_BOOT_TIME
#if  INCLUDE_LOGO
    /*  PRINTF("os2000Logo start\r\n");*/
    os2000Logo();   /* print out OS200 banner page */
    /*  PRINTF("os2000Logo OK\r\n");*/
    boardLogo();    /* print out board information */
    /*  PRINTF("boardLogo OK\r\n");*/
#endif
#endif  /* INCLUDE_SHOW_BOOT_TIME */

#if  INCLUDE_SHOW_BOOT_TIME
    {
        volatile unsigned short *tlo = ASIC_ReadRegLo2;
        volatile unsigned short *thi = ASIC_ReadRegHi2;
        unsigned timer_boot = ((*tlo & 0xffffU)+((*thi << 16U) & 0xffff0000U));
        unsigned timer_romcpy = *((unsigned*)TIMER_ROMCPY);
        unsigned cpy_romsize = *((unsigned*)CPY_ROMSIZE);
        unsigned start = 0xffffffffUL;
        unsigned ms_romcpy = ((start - timer_romcpy) * 1000) / TIMER_CLC_FREQ;
        unsigned ms_boot = ((start - timer_boot) * 1000) / TIMER_CLC_FREQ;
#ifndef COMPRESS
        printf("Rom copy time %u ms, rom size %u bytes\n\t"
            "100000 bytes - %u ms, %u bytes/ms\n",
            ms_romcpy, cpy_romsize, 100000 * ms_romcpy / cpy_romsize,
            cpy_romsize / ms_romcpy);
#else   /* COMPRESS */
        printf("Uncompress time %u ms, rom image size %u bytes\n\t"
            "100000 bytes - %u ms, %u bytes/ms\n",
            ms_romcpy, cpy_romsize, 100000 * ms_romcpy / cpy_romsize,
            cpy_romsize / ms_romcpy);
#endif  /* COMPRESS */
        printf("Init time %u ms\n", ms_boot - ms_romcpy);
        printf("Total boot time %u ms\n", ms_boot);
    }
#endif  /* INCLUDE_SHOW_BOOT_TIME */

#ifdef SET_ENVIRONMENT
    {
        VPCHECK(SetEnvironment( ENVIRONMENT ));
    }
#endif  /*SET_ENVIRONMENT*/

#if (INCLUDE_ENVIRONMENT) && (ENVDISPLAY_SET)
    {
        char *penv ;
        penv = (char *) malloc (sizeof(ENVDISPLAY_VALUE) + 9) ;
        if (penv != NULL)
        {
          strcpy(penv,"DISPLAY=") ;
          strcat(penv,ENVDISPLAY_VALUE) ;
          putenv(penv) ;
          PRINTF("Set Value DISPLAY=%s done\r\n", ENVDISPLAY_VALUE);
        }
    }
#endif  /*ENVIRONMENT*/

#if  INCLUDE_DEMO_THREAD
    {
        struct  sched_param sp;
        sp.sched_priority = 40;
        pthread_attr_setschedparam( &attr,&sp);
        pthread_create( NULL, &attr, osDemoThread,  NULL );
    }
#endif /* INCLUDE_DEMO_THREAD */

#if INCLUDE_PIPES
    {
        int pipeDevInit();
        VPCHECK(pipeDevInit());
    }
#endif
#if defined(INCLUDE_GTK)
    {
        int GtkInit();
        VPCHECK(GtkInit());
    }
#endif


  if( coderet == CODERET_NONE ) {
    /*
     * ждем завершения инициализации, выполняемой потоком
     * osInitThread
     */
        void *v = 0;
        if( pthread_join( osInitThread_t, &v ) == 0 )
            coderet = (int)v;
  }


#ifdef INCLUDE_TRACE
  if( !coderet ) {
      /* osInitThread закончился благополучно */
      struct trace_config*  traceConfig = traceGetConfig();
      traceSaveConfig( traceConfig ); /* сохраняем конфигурационные параметры трассировки */
#   ifdef TRACE_LOAD_CONFIG
   /* Требуется загрузить параметры конфигурации из файла */
      traceConfig->configload = 1;
      traceLoadConfiguration();
#   endif
      /* Выполнение действий, определенных конфигурацией трассы      */
      traceProcStub();
  }
#endif

#ifdef INCLUDE_SYSLOG
   {int *flags = logGetAddrSmFlags() ;
    int error, total_error = 0 ;
    if ((*flags & LOG_MSG_TO_MEMORY) || (*flags & LOG_MSG_TO_FLASH))
    {
       PCHECK( logMoveMemory2File()) ;
    }
    *flags |= LOG_GLOB_USERFUNC ; /* Можно выполнять пользовательские функции */
   }
#endif /* INCLUDE_SYSLOG */


#if  INCLUDE_USER_THREAD
#ifndef USER_THREAD_ENTRY
#define USER_THREAD_ENTRY osUserThread
#endif
#define USER_THREAD_NAME_QUOTE(a)   #a
#define USER_THREAD_NAME(a)         USER_THREAD_NAME_QUOTE(a)
 /*
  * если поток завершился с кодом отличным от 0,
  * не запускаем пользовательский поток
  * поток пользователя запускается, если благополучно завершается
  *  инициализация в потоке
  */
    if( coderet == 0 ) {
        void *USER_THREAD_ENTRY(void *);
        struct  sched_param sp;
        sigset_t  oldsig;

#if  USER_THREAD_PRIORITY
        sp.sched_priority = USER_THREAD_PRIORITY;
#else
        sp.sched_priority = DEFAULT_SCHED_PRIORITY;
#endif
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
        pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
#if (INCLUDE_MULTIPROCESSING) && (INCLUDE_MP_OBJS)
        pthread_attr_setmpstate( &attr, PTHREAD_MP_ENABLE );
#endif
        pthread_attr_setschedparam( &attr,&sp);
        pthread_attr_setbreakstate( &attr, PTHREAD_BREAK_ENABLE );
#ifdef INCLUDE_TRACE
        if( traceIS_THREAD_USER )
          pthread_attr_settracestate( &attr, PTHREAD_TRACE_ENABLE );
        else
          pthread_attr_settracestate( &attr, PTHREAD_TRACE_DISABLE );
#endif

/* gmu: for submic: Выключаем таймер к которому был подключен обработчик вычтавляющий годность*/
#ifdef FOR_SUBMICRON
#ifdef SUBMICRON_TIMER_LIVE
       #warning   !!! boardAuxNNClkDisable befor start osUserThread for 1890VM6  !!!
boardAuxNNClkDisable(SUBMICRON_TIMER_LIVE-1);
#endif //SUBMICRON_TIMER_LIVE
#endif //FOR_SUBMICRON
 
        pthread_attr_setname( &attr, USER_THREAD_NAME(USER_THREAD_ENTRY) );

    /*устанавливаем наследуемую маску пользовательского потока*/
        sigemptyset(&msig);
        pthread_sigmask (SIG_SETMASK, &msig, &oldsig);
        pthread_create( NULL, &attr, USER_THREAD_ENTRY, NULL );
        pthread_sigmask (SIG_SETMASK, &oldsig, NULL);
    }
#endif /* INCLUDE_USER_THREAD */

#if (INCLUDE_XSERVER) && (XSERVER_START)
  /*X-сервер запускается, если благополучно завершается
    инициализация в потоке
  */
    if( !coderet ) {
        void *_xserver(void *);
        struct  sched_param sp;
        sigset_t  oldsig;

#if  XSERVER_PRIORITY
        sp.sched_priority = XSERVER_PRIORITY;
#else
        sp.sched_priority = DEFAULT_SCHED_PRIORITY;
#endif
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
        pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
#if (INCLUDE_MULTIPROCESSING) && (INCLUDE_MP_OBJS)
        pthread_attr_setmpstate( &attr, PTHREAD_MP_ENABLE );
#endif
        pthread_attr_setschedparam( &attr,&sp);
        pthread_attr_setstacksize(&attr,32*1024);
        pthread_attr_setbreakstate( &attr, PTHREAD_BREAK_ENABLE );
        pthread_attr_setsystem( &attr, PTHREAD_CREATE_SYSTEM );
//        traceSET_ATTR_SYSTHREAD( &attr );

        /*устанавливаем наследуемую маску пользовательского потока*/
        sigemptyset(&msig);
        pthread_sigmask (SIG_SETMASK, &msig, &oldsig);
        pthread_create( NULL, &attr, _xserver, NULL );
        pthread_sigmask (SIG_SETMASK, &oldsig, NULL);
    }
#endif /* XSERVER_START */



if( coderet )  /*проблема инициализации*/
    printf( "System init error!\n" );

    return (void*)coderet;
}

#if  INCLUDE_DEMO_THREAD
void *osDemoThread (void *arg)
{
    int i=0;
    struct timespec ts={2,40000000};

    printf ("oc2000 - the best in the world !!!");

    while(TRUE)
    {
        printf ("\n\rDon't worry, be patient. This is a test #%i.",i++);
        nanosleep(&ts,NULL);
    }

    return NULL;
}

#endif /* INCLUDE_DEMO_THREAD */


#ifndef INCLUDE_CONSOLE
int kprint(const char *fmt,...)
{
    return 0;
}
int kvprint(const char *fmt,...)
{
    return 0;
}
#endif


#ifdef SET_ENVIRONMENT
static void SetEnvironment( const char *env )
{
    char *p = (char*)env, *ep;

    if( env == NULL )
        return;

    do {
        while( *p == ' ' || *p == ';' )  p++;
        if( *p == '\0' )
            break;
        ep = strpbrk( p, " ;" );
        if( ep == NULL ) {
            putenv( p );
            break;
        } else {
            char str[512];
            char *wp;
            int i;
            for( i=0,wp=str; i < sizeof(str) && p < ep; p++,wp++,i++ ) {
                *wp = *p;
            }
            if( i < sizeof(str) ) {
          char *pnt;

          *wp = '\0';
          pnt = (char*)malloc( strlen(str)+1 );
          if( pnt == NULL )
                  break;
          strcpy( pnt, str );
          if( putenv( pnt ) != 0 )
            break;
            }
            p = ep;
        }
    } while( ep != NULL );
}
#endif /*SET_ENVIRONMENT*/

#if INCLUDE_NETWORK
static struct mbufinit
{
    int nmb_init;
    int ncl_init;
    int nmb_add;
    int ncl_add;
    int nmb_max;
    int ncl_max;
} mbi={NMB_INIT,NCL_INIT,NMB_ADD,NCL_ADD,NMB_MAX,NCL_MAX};
extern struct mbufinit *mbuf_init;
#endif

/*функция инициализации, выполняемой в потоке*/
void *osInitThread( void *p )
{
  int error=0,total_error=0;
    /* инициализация средств обработки исключительных ситуаций
      при операциях с плавающими числами
    */

#if INCLUDE_NETWORK
    {
        void netInit(int inetd_priority);
        mbuf_init=&mbi;
        VPCHECK(netInit(NET_THREAD_PRIORITY));
#if INCLUDE_SHOW_NETWORK
        {
            nethelpInit();
        }
#endif
    }
#endif /* INCLUDE_NETWORK */

/*
 * Инициализация дополнительного оборудования
 * работает nanosleep malloc printf
 */
#ifdef BOARD_HW_INIT3
  VPCHECK1(BOARD_HW_INIT3);
#else
#endif

#if INCLUDE_VFAT
  {
		int bfscFlags = BFSC_OUT | BFSC_MODE;
#if BFSC_SAVE_BAD
		bfscFlags |= (BFSC_SAVE_CROSS_LINKS |
			BFSC_SAVE_LOST_CHAINS |
			BFSC_SAVE_BAD_DIRS);
#endif
#if BFSC_DO_CHECK_LEN
		bfscFlags |= BFSC_CHECK_LENGTH;
#endif
#if BFSC_DO_SYNC_FATS
		bfscFlags |= BFSC_SYNC_FATS;
#endif
		bfscInitFlags(bfscFlags);
  }
#endif
#if INCLUDE_VFAT && MOUNT_VFAT
  {
        char buf[100];
        const char *msdos_fmt = "-t msdos -o uid=%d,gid=%d,umask=0%o%s%s%s %s %s";
#define VFAT_MOUNT(a) \
        sprintf(buf,msdos_fmt,\
        VFAT_UID##a,VFAT_GID##a,VFAT_ACCESS##a,                 \
    VFAT_RDONLY##a?",ro":"",                                    \
        VFAT_SYNCHRONOUS##a?",sync":"",                         \
        VFAT_NOCHECK##a?",nocheck":"",                          \
            VFAT_BDNAME##a,VFAT_MOUNTPOINT##a);                 \
        error=_mount(buf);                                      \
        total_error +=error;                                    \
        PRINTF("mount %s %s \n",buf,error?"Fail":"OK");

#if VFAT_MNT_VOL0
        VFAT_MOUNT(0);
#endif
#if VFAT_MNT_VOL1
        VFAT_MOUNT(1);
#endif
#if VFAT_MNT_VOL2
        VFAT_MOUNT(2);
#endif
#if VFAT_MNT_VOL3
        VFAT_MOUNT(3);
#endif
  }
#endif /* INCLUDE_VFAT && MOUNT_VFAT */
#if  INCLUDE_CD9660 &&  MOUNT_CD9660
  {
     char buf[100];
     const char *cd_fmt = "-t cd9660 -r %s %s";
#define CD9660_MOUNT(a) \
    sprintf(buf,cd_fmt,CD9660_BDNAME##a,CD9660_MOUNTPOINT##a);  \
     error=_mount(buf);                                         \
     total_error +=error;                                       \
     PRINTF("mount %s %s \n",buf,error?"Fail":"OK");
#if MOUNT_CD96600
     CD9660_MOUNT(0);
#endif
#if MOUNT_CD96601
     CD9660_MOUNT(0);
#endif
  }
#endif /*INCLUDE_CD9660 &&  MOUNT_CD9660 */
#if  INCLUDE_TAR_FS &&  MOUNT_TAR
    {
        char buf[100];
        const char *tar_fmt = "-t tar -o b=0x%x,e=0x%x%s null %s";
#define TAR_MOUNT(a) \
        sprintf(buf,tar_fmt,\
    TAR_MEMADDRESS##a,TAR_MEMADDRESS##a+TAR_MEMSIZE##a,TAR_CHECK##a?",check":"",TAR_MOUNTPOINT##a); \
        error=_mount(buf);                                                                          \
        total_error +=error;                                                                        \
        PRINTF("mount %s %s \n",buf,error?"Fail":"OK");
#if MOUNT_TAR0
        TAR_MOUNT(0);
#endif
#if MOUNT_TAR1
        TAR_MOUNT(1);
#endif
#if MOUNT_TAR2
        TAR_MOUNT(2);
#endif
#if MOUNT_TAR3
        TAR_MOUNT(3);
#endif
    }
#endif /*  INCLUDE_TAR &&  MOUNT_TAR */

#if  INCLUDE_TAR_FS &&  defined(INCLUDE_TARFILES)
# if defined(INCLUDE_TARFILES0)
extern unsigned char start__tar_f_00[], end__tar_f_00[] ;
# endif
# if defined(INCLUDE_TARFILES1)
extern unsigned char start__tar_f_01[], end__tar_f_01[] ;
# endif
# if defined(INCLUDE_TARFILES2)
extern unsigned char start__tar_f_02[], end__tar_f_02[] ;
# endif
# if defined(INCLUDE_TARFILES3)
extern unsigned char start__tar_f_03[], end__tar_f_03[] ;
# endif
    {
        char buf[100];
        const char *tar_fmt = "-t tar -o b=0x%x,e=0x%x%s null %s";
#define TARFILE_MOUNT(a) \
        sprintf(buf,tar_fmt,\
    start__tar_f_0##a,end__tar_f_0##a,TAR_FCHECK##a?",check":"",TAR_PNAME##a); \
        error=_mount(buf);                                                                          \
        total_error +=error;                                                                        \
        PRINTF("mount %s %s \n",buf,error?"Fail":"OK");
# if defined(INCLUDE_TARFILES0)
        TARFILE_MOUNT(0);
# endif
# if defined(INCLUDE_TARFILES1)
        TARFILE_MOUNT(1);
# endif
# if defined(INCLUDE_TARFILES2)
        TARFILE_MOUNT(2);
# endif
# if defined(INCLUDE_TARFILES3)
        TARFILE_MOUNT(3);
# endif
    }
#endif /*  INCLUDE_TAR &&  defined(INCLUDE_TARFILES) */


#if INCLUDE_NETWORK
#ifdef FOR_SUBMICRON  //gmu: debug
#warning !!! SUBMICRON: netconfig() must be start in SPO !!!
#else
  VPCHECK1(error=netconfig());
  total_error +=error;
#endif /*FOR_SUBMICRON*/
#endif /* INCLUDE_NETWORK */

#if (INCLUDE_MULTIPROCESSING) && (INCLUDE_MP_OBJS)
  error = (int) mpInit();
  total_error +=error;
  if( error )
    KPRINT("mpInit error\r\n");
  else {
#if INCLUDE_MPSHM
    mpShmInit();
    KPRINT("mpInit done\r\n");
#endif
  }
#endif /* INCLUDE_MP_OBJS*/

#if (INCLUDE_MULTIPROCESSING)
#  if  INCLUDE_CHANNELS
  error = osChannelInit();
  PCHECK( error );
  if( error ) {
     printf( "osChannelInit() = %d\n", error );
     total_error += error;
  }
#  endif /* INCLUDE_CHANNELS */
#endif /* INCLUDE_MULTIPROCESSING */

  return (void*) total_error;
}
#if __GNUC__
/* установить адрес начала свободной памяти */
void osFreeRamAdrSet( void *p )
{
  osFreeRamAdr = p;
}

/* получить адрес начала свободной памяти */
void *osFreeRamAdrGet( void )
{
  return osFreeRamAdr;
}
#endif /* __GNUC__ */
