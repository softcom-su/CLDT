/* mpbase.h */
#ifndef _MPBASE_H
#define _MPBASE_H

extern int      mpPSN;	        /* номер платы (процессора), processor serial number */
extern int      mpCPUMaster;    /* ведущий процессор */

#ifdef KERNEL

#include <mp/mpirq.h>
#include <mp/mpdic.h>

#ifndef STAT_OK
  #define STAT_OK  0
  #define STAT_ER -1
#endif

/*размер заголовка BusNet Memory Region*/
#define mpBUSNETHEADER_SIZE	    20  

typedef struct st_mphead { /*заголовок общей памяти*/
#define MP_SIGNATURE       "mpObj"
  char        Signature[8];  /*mpMain*/
/*слово состояния общей памяти*/
/* #define mpSTATUS_PROCESS   1 в процессе инициализации*/
/* #define mpSTATUS_READY     2 инициализация закончена*/
  int         Status;
  mpoffset_t  OffsetMain;   /*смещение к mpMain*/
} mpHEADER;

/*главный описатель общей памяти*/
typedef struct st_mpmain {

#define MPMAIN_SIGNATURE       "mpMain"
#define MPMAIN_INIT            1
#define MPMAIN_REQUEST         2 /*выставлено требование*/
  int         Signature[2];  /*mpMain*/
  int         Flags;
  int         CPUQuantity;   /*количество процессоров*/
  mpoffset_t 	CpuTable;      /*смещение массива указателей на описатели CPU*/
  mpoffset_t  ClassDesc;     /*смещение массива описателей классов объектов
                               общей памяти*/
  mpoffset_t 	PoolDefault;   /*смещение пула общей памяти, используемого по умолчанию*/
  mpoffset_t 	ObjDic;        /*смещение словаря объектов в общей памяти*/
  mpoffset_t 	spinHead;      /*смещение заголовка списка спинов*/
} mpMAIN_DESC;

#define mpBOARD_IS_READY(h) \
    ( (h)->Status & htonl((u_long)mpSTATUS_READY) )

#define mpMAIN_IS_INIT() \
    ( mpMainAdr->Flags & htonl((u_long)MPMAIN_INIT) )

#define mpCPU_IS_ACTIVE(adrcpu) \
    ( ntohl((u_long)adrcpu->Flags) & mpCPU_ACTIVE) 

/*начать инициализацию общей памяти ЦПУ*/
#define   mpSTART_INIT(us) \
          bzero( (void*)mpHeaderAdr, (size_t)sizeof(mpHEADER) ); \
          mpMemUsed = us;

/*слово состояния общей памяти*/
#define mpSTATUS_PROCESS   1 /*в процессе инициализации*/
#define mpSTATUS_READY     2 /*инициализация закончена*/

/* определение поля Flags описателя ЦПУ*/
#define mpCPU_READY	      1
#define mpCPU_PKTREADY    2
#define mpCPU_ACTIVE      4
/*требование подтверждения готовности: выставляется slave 
перед посылкой прерывания мастеру, которое мастер должен снять
*/
#define mpCPU_REQUEST     8  
/*выставить требование на подтверждение активности мастера*/
#define mpSET_REQUEST(mpCpuAdr)  (mpCpuAdr->Flags |= htonl(mpCPU_REQUEST))

struct mail_inter {
  u_long value;
#define mpINT_MAILBOX_WR1 1
#define mpINT_MAILBOX_WR4 2
  int IntSendType;
  int IntSendId;
};

typedef struct st_mpcpu {
  int         Flags;
  mpIRQ_HEAD  IRQHead;       /*заголовок очереди запросов на прерывание*/
  struct mail_inter IntMail;
} mpCPU_DESC;

/*инициализация собственного ЦПУ закончена?*/
#define mpCPUSELF_IS_READY() \
    ( ((ntohl)mpCpuAdr->Flags & mpCPU_READY)      \
      && ((ntohl)mpCpuAdr->Flags & mpCPU_ACTIVE) )

#define mpSetBoardReady()  \
  mpInitialized = TRUE; \
  (mpHeaderAdr)->Status &= ~htonl((u_long)mpSTATUS_PROCESS); \
  (mpHeaderAdr)->Status |= htonl((u_long)mpSTATUS_READY); \
  mpCpuAdr->Flags |= htonl( (u_long)mpCPU_ACTIVE ); 

#define mpSetMasterReady() \
  mpMainAdr->Flags |= htonl((u_long)MPMAIN_INIT); \
  mpSetBoardReady()

extern u_long   mpMemUsed;      /* размер используемой общей памяти ЦПУ */
extern mpMAIN_DESC *mpMainAdr;  /* локальный адрес начала общей памяти */
extern u_long   mpMemStart;     /* начало общей памяти */
extern u_long   mpMemSize;      /* размер общей памяти */
#if 0
extern int      mpDicName;     /* MAX число имен в словаре */
extern int      mpSemMax;      /* MAX число семафоров */
extern int      mpMqpMax;      /* MAX число очередей */
extern int      mpTcbMax;      /* MAX число потоков */
#endif
extern int      mpMaxTriesConfig; /*макс.число попыток захвата спина*/
/*локальные сведения из mpMain*/
extern int           mpCPUQuantity;/* количество процессоров */
extern mpCPU_DESC   *mpCpuAdr;	   /* адрес описателя собственного CPU */
extern objCONTROL   *mpClassDesc;  /* дрес массива описателей классов объектов */
extern mpMEMPOOL_ID  mpPoolDefault; 
extern mpDIC_HEAD	  *mpObjDic;     /* адрес словаря объектов в общей памяти */

/*инициализация mpMain*/
int mpMainInit( mpoffset_t *mpCpuDesc );
/*инициализация описателей классов*/
int mpClassInit( void );
/*инициализация описателя ЦПУ*/
/*int mpCPUInit(int SendType,int SendId);*/
int mpCPUInit( struct mail_inter *s );
/*создание системного пула общей памяти*/
int mpMemSysInit( size_t objsize );
/*прочитать сведения mpMain*/
void mpGetInfoMain( void );
/*получить адрес описателя ЦПУ*/
mpCPU_DESC *mpGetAdrCPUDesc( int cpu );
/*получить адрес смещения описателя ЦПУ*/
mpoffset_t *mpGetAdrCPUDescOffset( int cpu );
/*ждать завершения инициализации ведущего ЦПУ*/
int mpWaitForMasterReady( void );

#endif /*KERNEL*/

#endif /*_MPBASE_H*/
