#ifndef _TERMIOS_H
#define _TERMIOS_H

#include <sys/fcntl.h>

typedef unsigned int tcflag_t;
typedef unsigned int cc_t;
typedef unsigned int speed_t;

#define NCCS     13

struct termios
{
   tcflag_t c_iflag; /* режимы ввода                                 */
   tcflag_t c_oflag; /* режимы вывода                                */
   tcflag_t c_cflag; /* режимы управления                            */
   tcflag_t c_lflag; /* локальные режимы                             */
   cc_t c_cc[NCCS]; /* управляющие символы                          */
   speed_t c_ispeed; /* скорости приема данных                       */
   speed_t c_ospeed; /* скорости передачи данных                     */
};

/*
 * 7.1.2.2. Режимы ввода c_iflag
 */

#define BRKINT    0x01                   /* порождать прерывание по break                   */
#define ICRNL    0x02                   /* при вводе преобразовывать CR в NL               */
#define IGNBRK    0x04                   /* игнорировать условие break                   */
#define IGNCR    0x08                   /* игнорировать CR                               */
#define IGNPAR    0x10                   /* игнорировать символы с ошибкой четности       */
#define INLCR    0x20                   /* при вводе преобразовывать NL в CR               */
#define INPCK    0x40                   /* включить проверку четности при вводе           */
#define ISTRIP    0x80                   /* отсекать восьмой бит                           */
#define IXOFF    0x100                  /* включить старт/стопное управление вводом       */
#define IXON    0x200                  /* включить старт/стопное управление выводом    */
#define PARMRK    0x400                  /* отмечать ошибки четности                       */

/*
 * 7.1.2.3. Режимы вывода c_oflag
 */

#define ONLCR     0x01                 /* Map NL to CR-NL on output.       */
#define OCRNL     0x02                 /* Map CR to NL on output.          */
#define ONOCR     0x04                 /* No CR output at column 0.        */
#define ONLRET    0x08                 /* NL performs CR function.         */
#define OFILL     0x10                 /* Use fill characters for delay.   */
#define OFDEL     0x20                 /* Fill is DEL, else NUL.           */       
#define NLDLY     0xc0                 /* Select newline delays:           */
#define NL0       0x40                 /* Newline character type 0.        */
#define NL1       0x80                 /* Newline character type 1.        */
#define CRDLY     0xf00                   /* Select carriage-return delays:    */
#define CR0      0x100                   /* Carriage-return delay type 0.    */
#define CR1      0x200                   /* Carriage-return delay type 1.    */
#define CR2      0x400                   /* Carriage-return delay type 2.    */
#define CR3      0x800                   /* Carriage-return delay type 3.    */
#define TABDLY    0xf000                   /* Select horizontal-tab delays:    */
#define TAB0    0x1000                   /* Horizontal-tab delay type 0.     */
#define TAB1    0x2000                   /* Horizontal-tab delay type 1.     */
#define TAB2    0x4000                 /* Horizontal-tab delay type 2.     */
#define TAB3    0x8000                 /* Expand tabs to spaces.           */
#define BSDLY  0x30000                 /* Select backspace delays:         */
#define BS0    0x10000                 /* Backspace-delay type 0.          */
#define BS1    0x20000                 /* Backspace-delay type 1.          */
#define VTDLY  0xc0000                 /* Select vertical-tab delays:      */
#define VT0    0x40000                 /* Vertical-tab delay type 0.       */
#define VT1    0x80000                 /* Vertical-tab delay type 1.       */
#define FFDLY 0x300000                 /* Select form-feed delays:         */
#define FF0   0x100000                 /* Form-feed delay type 0.          */
#define FF1   0x200000                 /* Form-feed delay type 1.          */
#define OPOST 0x400000                   /* Выполнить обработку вывода       */

/*
 * 7.1.2.4. Режимы управления c_cflag
 */

#define CLOCAL    0x1                    /* Игнорировать линии состояния модема           */
#define CREAD     0x2                    /* Включить приемник                               */
#define CSIZE     0x3c                   /* Количество битов в байте:                       */
#define  CS5      0x4                    /*    5 битов                                       */
#define  CS6      0x8                    /*    6 битов                                       */
#define  CS7      0x10                   /*    7 битов                                       */
#define  CS8      0x20                   /*    8 битов                                       */
#define CSTOPB    0x40                   /* Передать 2 стоп-бита, иначе - 1               */
#define  HUPCL    0x80                   /* Отсоединиться при последнем закрытии           */
#define PARENB    0x100                  /* Включить четность                               */
#define PARODD    0x200                  /* Контроль на нечетность, иначе - на четность  */

#ifndef _POSIX_SOURCE
#define CRTSCTS ( 0x400 | 0x800 )      /*   Не POSIX flow control                       */
#endif

/*
 * 7.1.2.5. Локальные режимы c_lflag
 */

#define ECHO      0x1 /* Включить эхо-отображение */
#define ECHOE     0x2 /* Включить эхо-отображение ERASE в виде возврата на одну позицию при исправлении ошибки */
#define ECHOK     0x4 /* Эхо-отображение KILL */
#define ECHONL    0x8 /* Эхо-отображение '\n' */
#define ICANON    0x10 /* Канонический ввод (обработка erase и kill) */
#define IEXTEN    0x20 /* Включить дополнительные функции (определяемые реализацией) */
#define ISIG      0x40 /* Включить сигналов */
#define NOFLSH    0x80 /* Запретить сброс после прерывания, завершения (quit) или приостановки */
#define TOSTOP    0x100 /* Послать SIGTTOU для фонового вывода */
#ifndef _POSIX_SOURCE
#define ECHOCTL 0x200
#endif
/*
 *    7.1.2.6. Особые управляющие символы
 */

#define VEOF        0 /* Символ EOF        */
#define VEOL        1 /* Символ EOL        */
#define VERASE        2 /* Символ ERASE        */
#define VINTR        3 /* Символ INTR        */
#define VKILL        4 /* Символ KILL        */
#define VMIN        5 /* Значение MIN        */
#define VQUIT        6 /* Символ QUIT        */
#define VSUSP        7 /* Символ SUSP        */
#define VTIME        8 /* Значение TIME    */
#define VSTART        9 /* Символ START        */
#define VSTOP        10 /* Символ STOP        */
#ifndef _POSIX_SOURCE
#define VHIWATER    11 /* Значение HIWATER    */
#define VLOWATER    12 /* Значение LOWATER    */
#endif

#define T_EOF        'D'-0x40    /* ctrl+D   */
#define T_EOL        0
#define T_ERASE      'H'-0x40    /* ctrl+H   */
#define T_INTR       'C'-0x40    /* ctrl+c    */
#define T_KILL       'U'-0x40    /* ctrl+U   */
#define T_MIN        0x1         /* один символ и конец ввода */
#define T_QUIT       'X'-0x40   /* ctrl+X   */
#define T_SUSP       'Z'-0x40    /* ctrl+Z   */
#define T_TIME        0x0         /* время не установлено */
#define T_START       17            /* XON  */
#define T_STOP        19            /* XOFF */

/* tcflow() and TCXONC use these */
#define    TCOOFF      0
#define    TCOON       1
#define    TCIOFF      2
#define    TCION       3

/* tcflush() and TCFLSH use these */
#define    TCIFLUSH    FREAD              /* сброс (очистка) буфера ввода                 */
#define    TCOFLUSH    FWRITE             /* сброс (очистка) буфера вывода                */
#define    TCIOFLUSH   (FREAD | FWRITE)   /* сброс (очистка) буфера ввода и буфера вывода */

/* tcsetattr uses these */
#define    TCSANOW      0
#define    TCSADRAIN    1
#define    TCSAFLUSH    2

#define B0            0
#define B50          50
#define B75          75
#define B110        110
#define B134        134
#define B150        150
#define B200        200
#define B300        300
#define B600        600
#define B1200      1200
#define B1800      1800
#define B2400      2400
#define B4800      4800
#define B9600      9600
#define B19200    19200
#define B38400    38400
#define B115200  115200

int tcdrain(int fildes);
int tcflow(int fildes, int action);
int tcflush(int fildes, int queue_selector);
int tcgetattr(int fildes, struct termios *termios_p);
int tcsendbreak(int fildes, int duration);
int tcsetattr(int fildes, int opt_actions, struct termios *termios_p);

speed_t cfgetospeed(const struct termios *termios_p);
speed_t cfgetispeed(const struct termios *termios_p);
int cfsetospeed(struct termios *termios_p, speed_t speed);
int cfsetispeed(struct termios *termios_p, speed_t speed);

#endif /* !_TERMOIS_H */

