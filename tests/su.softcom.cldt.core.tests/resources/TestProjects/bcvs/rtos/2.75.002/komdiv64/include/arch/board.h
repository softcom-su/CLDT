#ifndef ARCH_BOARD_H
#define ARCH_BOARD_H

/* Назначение общей памяти */
#define SHMEM_FIRMWARE  101
#define SHMEM_RESERVED  102
#define SHMEM_MBOX_ETH  103
#define SHMEM_MBOX_OBJ  104
#define SHMEM_OBJECTS   105
#define SHMEM_ETHERNET  106

extern int boardTimerHZ;

void  boardIntInit ();
void  boardIntEOI (void);
int   boardIntEnable(int irq);
int   boardIntDisable(int irq);
int   boardClockInit(int tps);
int   boardClockConnect ( int (routine)(), int arg	);
void  boardClockEnable(void);
char *getAddrBootString(void);
/* Получить загружаемое в таймер значение */
unsigned boardGetTimerFreq( void );
int   boardClkRateGet( void );
/* Получение изменения счетчика часов */
unsigned int traceClockCounterPass( void );


int   boardMailboxEnable(char *address);
void  boardMailboxIntr(int cputo);
int   boardMailboxConnect(void (IrqHandler)(int),int parm);
/*int   boardMailboxConnectI( int i, void (IrqHandler)(int), int parm );*/
int   boardBusToLocalAdrs(int    adrsSpace,char  *busAdrs,char **pLocalAdrs);
int   boardLocalToBusAdrs(int    adrsSpace,char  *localAdrs,char **pBusAdrs);
int   boardShMem( int boardnum, int type, volatile void **ptr, size_t *size );
int   boardProcNumGet();
int   boardBnConfig();
/* Подключение обработчика прерываний для объектов общей памяти */
int   boardMPIrqConnect( void (IrqHandler)(int) );
int   boardMPIrqEnable( void );
void  boardMPIrqIntr( int cpu );
char *boardModel(void);

struct chan_bufnode;
#define NET_MBOX   5
#define CHAN_MBOX  4

int   boardRioInit( int nBuf, int bufsize, void(*txRtn)(int,struct chan_bufnode*,int), void(*rxRtn)(void*,int) );

int   boardRioSendMsg( int cpu, struct chan_bufnode* pBufNode, int size );
int   boardRioRecvMsg( int cpu, struct chan_bufnode* pBufNode );
void  boardRioFreeBuf( void* pBuf );
typedef struct {
   int mtu;
   int sizeAlign;
   int addrAlign;
} BOARD_RIO_PARAM;
void  boardRioParamGet(BOARD_RIO_PARAM* p);

#if __mips__
int   cacheDataInvalidate( void* ptr, int nBytes );
void flashInfo() ;
int flashFree() ;
int flashEraseSector() ;
int flashWriteBlock() ;
#else
void  cacheDataInvalidate(char *ptr, int size);
#endif

/* Функции ППМ для ANSI-VITA:
>>
>>1. Подключение обработчика почтовых прерываний:
>>
>>    int boardBnMboxConnect(VOIDFUNCPTR rtn, int arg);
>>
>>    Прежняя функция boardMboxConnect() не устраивает т.к. она же
>>    используется для объектов, а почтовых ящиков может быть несколько.
>>
>>
>>
*/
int boardBnMboxConnect(vfuncptr_t rtn, int arg);

/*
>>2. Послать почтовое прерывание:
>>
>>    void boardBnMboxSend(int method, int va, int am, int value, int
>>
>>
>unused);
>
>
>>    Параметры:
>>
>>        method (BN_P_ND_METHOD) - тип доступа (байт, слово и пр.).
>>        va (BN_P_ND_VALUE1) - адрес почтового ящика на VME.
>>        am (BN_P_ND_VALUE2) - адресный модификатор почтового ящика на VME.
>>        value (BN_P_ND_VALUE3) - записываемое значение.
>>        unused (BN_P_ND_VALUE4).
>>
>>
*/

void boardBnMboxSend(int method, int va, int space, int value, int unused);


int boardBnVmeLock(int va, int space, int method, void **la);
int boardBnVmeUnlock(int va, int space, int method);


/* receive data from BusNet ANSI-VITA */

int boardBnDataReceive(int va, int space, int method, void *dst, int nBytes);
int boardBnDataSend(int va, int space, int method, void *src, int nBytes);



/* Тип компьютера: RM7000, и т.д. */
enum cpu_type_id {
 cptype_R3000   =   1,
 cptype_R3081   =   2,
 cptype_1B578   =   3,
 cptype_1B812   =   4,
 cptype_1890BM1 =   5,
 cptype_1890BM2 =   6,
 cptype_RM7000  = 100,
 cptype_1890BM3 = 103,
 cptype_1890BM5 = 105,
 cptype_1890BM6 = 106,
 cptype_1890BM7 = 107,
 cptype_i386    = 300,
} ;
/* Функция ППМ, которая возвращает тип компьютера: */
int boardGetCPUType(void) ; 



struct netCONFIG {
    struct {
        char ifname[16];    /* interface name */
        char ip[16];        /* interface IP */
        char netmask[16];   /* interface IP netmask */
        char ptp[16];       /* point-to-point for SLIP */
    } ifc[4];
    char default_gw[16];
    char host_name[16];
    char host_IP[16];
    char user[16];
    char password[16];
    char nfs_export[16];
    int uid;
    int gid;
    int ftp_nfs;
} ;
int boardBootParamsGet (struct netCONFIG *nc);

// Функции ППМ, для синхронизации времени:

// Функция коррекции системных часов:
void          boardClockAdjust(system_time_t D, int diff);
// Функция опроса точного времени системных часов:
system_time_t boardClockGetTime(void);
// Функция опроса числа тиков процессора за 1 секунду:
//    Если параметр nom=0, то функция возвращает текущее значение.
//    Если параметр nom=1, то функция возвращает номинальное (начальное) значение.
unsigned long long boardClockCountPerSecGet(int nom);


#endif /* !ARCH_BOARD_H */
