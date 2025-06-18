/* private/prsmemprv.h - поддержка сохраняемой памяти */

#ifndef _PRIVATE_PPSMEM_H
#define _PRIVATE_PPSMEM_H

/* Типы областей в сохраняемой памяти */
#define PRS_MEM_PERSIST   0   /* область сохранения                  */
#define PRS_MEM_TRACE     1   /* область под трассировку             */
#define PRS_MEM_USER      2   /* область предоставляемая клиенту     */
#define PRS_MEM_TESTAMENT 3   /* область под завещание               */

#define PRSMEM_SIGNATURE            "persist"
#define PRSMEM_TRACE_SIGNATURE      "trace  "
#define PRSMEM_USER_SIGNATURE       "client "
#define PRSMEM_TESTAMENT_SIGNATURE  "testam "
#define PRSMEM_ALIGN        1024*8

#define prsmemTRACE_SIGNATURE_SIZE     \
         UADDR_ALIGN_UP( sizeof(PRSMEM_TRACE_SIGNATURE), 4 )
#define prsmemUSER_SIGNATURE_SIZE      \
         UADDR_ALIGN_UP( sizeof(PRSMEM_USER_SIGNATURE), 4 )
#define prsmemTESTAMENT_SIGNATURE_SIZE \
         UADDR_ALIGN_UP( sizeof(PRSMEM_TESTAMENT_SIGNATURE), 4 )

#define PRSMEM_ALLOC_AUTO  1        /* областиь сохранения размещена автоматически  */
#define PRSMEM_ALLOC_MP    2        /* областиь сохранения размещена в общей памяти */
#define PRSMEM_ALLOC_ADR   3        /* областиь сохранения указана в конфигураторе  */

/* Флаги инициализации */
#define prsmemSTATE_INIT             0x0001 /* не получена память под буфер и ...   */
#define prsmemSTATE_EMEMORY          0x0002 /* не получена память под буфер и ...   */
#define prsmemSTATE_SIGNATURE        0x0004 /* сигнатуры установлены                */
#define prsmemSTATE_COMPLETE         0x0010 /* буфер инициализирован                */
#define prsmemSTATE_ESIGN_START      0x0100 /* сигнатура не найдена в начале области
                                              сохранения                            */
#define prsmemSTATE_ESIGN_END        0x0200 /* сигнатура не найдена в конце области
                                             сохранения                             */
#define prsmemSTATE_ESIGN_TRACE      0x0400 /* сигнатура не найдена в начале буфера
                                              трассы                                */
#define prsmemSTATE_ESIGN_TESTAMENT  0x0800 /* сигнатура не найдена в начале области
                                              завещания                             */
#define prsmemSTATE_ESIGN_USER       0x1000 /* сигнатура не найдена в начале области
                                              пользователя                          */
/* Идентификаторы вызываемых через SYSCALL функций */
#define prsmemSYSCALL_PROC_INIT     0
#define prsmemSYSCALL_PROC_PUT      1     /* положить данные в сохраняемую область  */
#define prsmemSYSCALL_PROC_GET      2     /* получить данные из сохраняемой области */
#define prsmemSYSCALL_USER_INIT     3
#define prsmemSYSCALL_USER_PUT      4     /* положить данные в сохраняемую область  */
#define prsmemSYSCALL_USER_GET      5     /* получить данные из сохраняемой области */

/* Заголовок области сохранения */
struct prsmem_head {
  u_int  prsm_state;          /* информация о состоянии                       */
  u_int  prsm_allocation;     /* способ размещения                            */
  void*  prsm_start;          /* указатель на начало области сохранения       */
  size_t prsm_size;           /* общий размер сохраняемой области             */
  void*  prsm_trace_ptr;      /* указатель на память, выделенную под трассу   */
  size_t prsm_trace_size;     /* размер под трассу                            */
  size_t prsm_trace_alloc;    /* уже используется                             */
  void*  prsm_testament_ptr;  /* указатель на память под завещание ОС         */
  size_t prsm_testament_size; /* размер под завещание ОС                      */
  size_t prsm_testament_alloc;/* уже используется                             */
  void*  prsm_user_ptr;       /* указатель на память пользователя             */
  size_t prsm_user_size;      /* размер пользователя                          */
  size_t prsm_user_alloc;     /* уже используется                             */
  void*  prsm_functions[6];   /* указатели функций обслуживания               */
  /* Содержимое полей сигнатур перед инициализацией */
  char   prsm_sign_start[sizeof(PRSMEM_SIGNATURE)];
  char   prsm_sign_end[sizeof(PRSMEM_SIGNATURE)];
  char   prsm_sign_trace[sizeof(PRSMEM_TRACE_SIGNATURE)];
  char   prsm_sign_testament[sizeof(PRSMEM_TESTAMENT_SIGNATURE)];
  char   prsm_sign_user[sizeof(PRSMEM_USER_SIGNATURE)];
};

/* Входные параметры конфигурирования области сохранения */
struct prsmem_config {
   int      allocation;
   void*    addres;
   size_t   trace_size;
   size_t   testament_size;
   size_t   user_size;
   /* Функция запроса общей памяти, указатель инициализируется, если
    * ППМ поддерживает общую память
    */
   int      (*shmem)(int cpu, int type, volatile void** ptr, size_t* size);
};

/* Описатель сохраняемой области процесса */
struct ppe_prsmem {
   int      procInit;
   void*    procPtr;
   size_t   procSize;
   int      userInit;
   void*    userPtr;
   size_t   userSize;
};

struct move_prsmem {
   struct ppe_prsmem*
            pPrsmem;
   void*    adr;        /* адрес процесса             */
   size_t   size;       /* размер данных              */
   int      offset;     /* смещение от начала области */
};

int   prsmemAllocation( struct prsmem_config* p );
int   prsmemSetConfig( struct prsmem_config* p );
int   prsmemGetInfo( struct prsmem_head* p );
int   prsmemSysCall( int fId, void* p );
//void* prsmemGet( int type, size_t *size );
//int   prsmemCheck( void );
void  prsmemShowInit( void );

#endif   /* _PRIVATE_PPSMEM_H */
