
#ifndef inc_mkio_h
#define inc_mkio_h
#include <pthread.h>
#include <usr401a.h>


//В БЦВС: два модуля МКИО; на каждом модуле - два канала МКИО;
//в каждом канале МКИО - две линии МКИО
//В документации НИИСИРАН: "интерфейсное устройство (ИУ)" - это "канал МКИО"



/*
 * Дескриптор ИУ
 */
typedef struct {
  char ini;          //признак инициализации ИУ
  char open;	     //признак открытия файла для ИУ
  char nMod;         //номер модуля МКИО
  char nIU;          //номер ИУ на модуле
  char nMKIO;        //порядковый номер МКИО
  char rgm;          //режим работы (MANCH_BC,MANCH_MT,MANCH_RT...)
  char name[16];     //имя устройства ("/dev/bc0"...)
  int rdBufSize;     //размер внутреннего кольцевого буфера чтения в режиме
                     //оконечного устройства в 16-ти битных словах (одного
                     //для всех подустройств)
  int wrtSubBufSize; //размер внутренних кольцевых буферов записи для каждого
                     //из подустройств в режиме оконечного устройства в 16-ти
                     //битных словах  
  int fd;            //дескриптор файла ИУ
              //функции обратного вызова:
  PNOTIFYFUNC   bc_func;  //для КК
  PCALLBACKFUNC rt_func;  //для ОУ
} mkioIUDesc_t;

/*
 * Дескриптор одного модуля МКИО
 */
typedef struct {
  char ini;     //признак инициализации модуля
  char nMod;    //=0,1 - номер модуля МКИО
  char dev;     //=1,2 - номер PMC-слота, на который установлен модуль???
  char interrupt;//=0-3 - номер линии прерывания шины PCI (INTA-INTC)
  int module;  //номер модуля от bt401DevInit()
  int offset;  //=0 - для модуля БТМ23-401???
  int mod;     //=1 - для модуля БТМ23-401???тип модуля???
  mkioIUDesc_t IUDesc[2];//дескрипторы ИУ
} mkioModuleDesc_t;


/*
 * Дескриптор драйвера модулей МКИО
 */
typedef struct {
 char ini; //признак инициализации
 int  err; //код возврата из bt401Drv()
 mkioModuleDesc_t Module[2]; //дескрипторы модулей МКИО
} mkioDrvDesc_t;


/*
 * Дескриптор пакета МКИО
 */
typedef struct {
 u_int sgn;           //=0x000BA7C1 - сигнатура
 pthread_mutex_t mtx; //для монопольного захвата пакета
 u_short nMKIO;       //номер МКИО
 u_char  started;     //признак запуска пакета
 u_char  del;         //запрос на удаление пакета
 bt401chain chain;    //цепочка команд МКИО
} mkioBatchDesc_t;
//максимальное количество сообщений в пакете:
#define MANCH_BATCH_MAX  MAX_CK_LEN

extern mkioBatchDesc_t mkioBatchDesc[1024]; //gmu: зарезервируем место для создания пакетов
extern unsigned mkioBatchDesc_busy[32];     //gmu: для определения занятости места под пакет в mkioBatchDesc


extern mkioDrvDesc_t mkioDrvDesc;
extern pthread_mutex_t manch_mutex;
extern pthread_mutex_t mkio_mutex[4];
extern mqd_t mkio_mq;             //дескриптор очереди


#define MKIO_SERVERS 8 //количество потоков сервера МКИО

void mkioBatchServer(void);
void mkioBatchServer_start(void);

void mkioRTServer(int *arg);
void mkioRTServer_start(int i);
void mkioRTServer0_start(void);
void mkioRTServer1_start(void);
void mkioRTServer2_start(void);
void mkioRTServer3_start(void);

int mkioBatchInit(int batch,PMANCH_COMMAND buf,u_short count);
int mkioBatchSend(mkioBatchDesc_t * p);
void mkioBatchNotify(PNOTIFYFUNC func,mkioBatchDesc_t * p);
int mkioBatchCK(mkioIUDesc_t *pi,mkioBatchDesc_t *p);


#define mkio_info(__s) do{ \
/* fprintf(stdout,"#MKIO-I# %s:%d - %s\r\n",__PRETTY_FUNCTION__,__LINE__,__s); */\
 }while(0)


#define mkio_error(__s) do{ \
 fprintf(stderr,"#MKIO-E# %s:%d - %s\r\n",__PRETTY_FUNCTION__,__LINE__,__s); \
 }while(0)


#define mkio_assert_r(__exp,__retval) \
 do { if(!(__exp)) {    \
    mkio_error(#__exp); \
    return __retval;  \
 } }while(0)

#define mkio_assert_g(__exp,__retval,__label) \
 do { if(!(__exp)) {    \
    mkio_error(#__exp); \
    ret= __retval;/*???*/\
    goto __label;       \
 } }while(0)



#endif  /*inc_mkio_h*/

