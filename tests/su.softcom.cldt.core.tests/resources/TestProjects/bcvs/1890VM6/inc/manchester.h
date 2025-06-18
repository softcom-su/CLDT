/**/
/*CP1251*/
#ifndef inc_manchester_h
#define inc_manchester_h

/**/
#include <pthread.h>
#include <usr401a.h>


#undef __far
#define __far

/**/
#define RAW_LENGHT(cmd) ((cmd)&0x1F)
#define LENGTH(cmd) ???
#define SUBADDRESS(cmd) ((((cmd)>>5)&0x1F))
#define ADDRESS(cmd) ((((cmd)>>11)&0x1F))

#define MANCH_READ 1
#define MANCH_WRITE 0
#define MAKECOMMAND(a,f,s,l) (\
   (((a)&0x1f)<<11) |\
   (((f)&0x01)<<10) |\
   (((s)&0x1f)<< 5) |\
   (((l)&0x1f)<< 0) )

#define MANCH_READ_HW (1<<10)

#define CTRL_SUBADDR(s)  ((SUBADDRESS(s)==0) || (SUBADDRESS(s)==0x1F))


#define ME_NODRIVER          -101
#define ME_INVALID_EXCHANGE  -102
#define ME_INVALID_PARAMETER -103
#define ME_NOMEMORY          -104
#define ME_CTRL_COMMAND      -105
#define DE_DRIVER_SPECIFIC   -201
#define DE_ALREADY_USED      -202
#define DE_NOT_SUPPORTED     -203
#define DE_INVALID_PARAMETER -204
#define DE_DRIVER_BUSY       -205
#define DE_HARDWARE          -206
#define DE_INVALID_REQUEST   -207

#define HE_SPECIFIC_0  -300
#define HE_SPECIFIC_1  -301
#define HE_SPECIFIC_2  -302
#define HE_SPECIFIC_3  -303
#define HE_SPECIFIC_4  -304
#define HE_SPECIFIC_5  -305
#define HE_SPECIFIC_6  -306
#define HE_SPECIFIC_7  -307


typedef union {
 u_short    cmd;
 struct {
    u_short address  :5;
    u_short flag     :1;
    u_short subaddres:5;
    u_short length   :5;
 } fld;                    //ВНИМАНИЕ! Компилятор непонятно ругается, если структуру оставить без имени!!!
} COMMAND;
typedef COMMAND * PCOMMAND;

typedef struct {
 COMMAND command;
 u_short * buf;
} MANCH_COMMAND;
typedef MANCH_COMMAND * PMANCH_COMMAND;


//typedef unsigned (NOTIFYFUNC)(u_long id,u_long cmd,u_short *buf,u_long length);
typedef unsigned (NOTIFYFUNC)(
  u_long batch,  //идентификатор пакета
  u_long cmd,    //<15- 0> - выдававшееся командное слово
                 //<31-16> - результат: 0 - норма, иначе - ошибка
  u_short *buf,  //указатель на принятые слова данных (0 для команд выдачи)
  u_long length  //< 7- 0> - количество принятых слов данных
                 //<15- 8> - номер команды в пакете
                 //<31-16> - полученное ответное слово
                 //          (=0xFFFFF - нет ответного слова)
);
typedef NOTIFYFUNC *PNOTIFYFUNC;

typedef unsigned (CALLBACKFUNC)(u_short cmd,u_short *buf,u_long length);
typedef CALLBACKFUNC *PCALLBACKFUNC;



/**/
int manch_open(int device, int chanel, u_char number);
int manch_close(int device);
int manch_trans(int device, u_short address,u_short cmd, u_short *buf, u_long length,u_char np);
int manch_send(int device, u_short address,u_short cmd, u_short *buf, u_long length);
int manch_receive(int device, u_short address,u_short cmd, u_short *buf, u_long length);
int manch_create_batch(int device);
int manch_add_cmd(int batch, u_short flag, u_short address, u_short cmd, u_short *buf, u_long length);
int manch_start(int batch);
int manch_send_batch(int device, PMANCH_COMMAND buf, u_short count);
int manch_delete_batch(int batch);

int manch_enable_buf(int device, u_char subaddress, void *buf);
int manch_disable_buf(int device, u_char subaddress);

int manch_hint_attach(int device, PCALLBACKFUNC func,unsigned ds);
int manch_hint_detach(int device, int cookie);

int manch_notify_attach(int device, PNOTIFYFUNC func,unsigned ds);
int manch_notify_detach(int device, int cookie);


int manch_read_buf(int device, u_char subaddress, u_long off, u_short 
                   *buf, u_long length); /*Не реализована*/

int manch_write_buf(int device, u_char subaddress, u_long off, u_short *buf,
                    u_long length);

//Добавлена
int manch_send_com(int device, u_short address, u_short cod, u_short *buf);



/*Режимы работы МКИО*/
#define MANCH_RT(aou) (aou) /*ОУ с адресом aou=0-30*/
#define  MANCH_RT00   MANCH_RT(0)
#define  MANCH_RT01   MANCH_RT(1)
#define  MANCH_RT02   MANCH_RT(2)
#define  MANCH_RT03   MANCH_RT(3)
#define  MANCH_RT04   MANCH_RT(4)
#define  MANCH_RT05   MANCH_RT(5)
#define  MANCH_RT06   MANCH_RT(6)
#define  MANCH_RT07   MANCH_RT(7)
#define  MANCH_RT08   MANCH_RT(8)
#define  MANCH_RT09   MANCH_RT(9)
#define  MANCH_RT10   MANCH_RT(10)
#define  MANCH_RT11   MANCH_RT(11)
#define  MANCH_RT12   MANCH_RT(12)
#define  MANCH_RT13   MANCH_RT(13)
#define  MANCH_RT14   MANCH_RT(14)
#define  MANCH_RT15   MANCH_RT(15)
#define  MANCH_RT16   MANCH_RT(16)
#define  MANCH_RT17   MANCH_RT(17)
#define  MANCH_RT18   MANCH_RT(18)
#define  MANCH_RT19   MANCH_RT(19)
#define  MANCH_RT20   MANCH_RT(20)
#define  MANCH_RT21   MANCH_RT(21)
#define  MANCH_RT22   MANCH_RT(22)
#define  MANCH_RT23   MANCH_RT(23)
#define  MANCH_RT24   MANCH_RT(24)
#define  MANCH_RT25   MANCH_RT(25)
#define  MANCH_RT26   MANCH_RT(26)
#define  MANCH_RT27   MANCH_RT(27)
#define  MANCH_RT28   MANCH_RT(28)
#define  MANCH_RT29   MANCH_RT(29)
#define  MANCH_RT30   MANCH_RT(30)

#define MANCH_BC      MANCH_RT(31) /*контроллер*/
#define MANCH_MT      MANCH_RT(32) /*монитор*/
#define MANCH_NONE    MANCH_RT(33) /*не задан*/
#define MANCH_RGM_MAX MANCH_NONE

#include "mkio.h"

#endif /*inc_manchester_h*/

