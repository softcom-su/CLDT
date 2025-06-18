/* ldebug.h - Файл заголовков для архитектурно независимой части */
/* локального отладчика ldebug.c */

/* Copyright 1998-1999 НИИСИ РАН. */

/*
modification history
--------------------
17.45, 29 oct 98 created.
*/

#ifndef _LDEBUG_H
#define _LDEBUG_H

#include "private/sdlist.h"
#include <arch/cputypes.h>
#include <private/thread.h>

#define LDB_CRET   TRUE  /* команда cret поддерживается локальным отладчиком */

/* Для R3000 & R4000 : */
#if 	CPU == R3000 || CPU == R4000
#define LDB_COMMAND_ALIGN     4
typedef unsigned long COMMAND ;    /* команды mips имеют фиксированную длину */
#define LDB_TRACE  FALSE  /* пошаговый режим выполнения на R3000 отсутствует */
#define	BREAK_COMMAND	0x0000000d                      /* код команды break */
# if CPU == R3000
#  define BRK_HW_BP   0x00        /* аппаратный останов на R3000 отсутствует */ 
#  define HW_REGS_NUM    0  /* количество регистров для аппаратных остановов */
# else
#  define BRK_HW_BP   0x80     /* аппаратный останов на R4000 поддерживается */ 
#  define HW_REGS_NUM	 2  /* количество регистров для аппаратных остановов */
# endif  /* CPU == R4000 */
#endif /* CPU == R3000 || CPU == R4000 */

/* Для Intel: */
#if 	CPU == I80386 
#define LDB_COMMAND_ALIGN	  1
typedef	unsigned char COMMAND ;    /* команды на Intel имеют различную длину */
#define LDB_TRACE  TRUE    /* на Intel поддерживается пошаговый режим (int 1)*/
#define BREAK_COMMAND     0xcc           /* код команды break вызывает int 3 */
#define TRACE_FLAG		  0x0100      /* флаг пошагового режима TF in EFLAGS */
#define BRK_HW_BP         0x80 /* аппаратный останов на Intel поддерживается */ 
#define HW_REGS_NUM	    4	/* количество регистров для аппаратных остановов */
#endif 	 /* CPU == I80386 */

#define ALL_THREAD	  0	/* останов для всех потоков */

/* список способов (типов) продолжения потока после останова */

#define TR_CONTINUE	  0	/* продолжить выполнение после останова */
#define TR_ONE_STEP	  1	/* выполнить один шаг */
#define TR_ONE_FR_BR  2	/* выполнить один шаг после останова */

/* флаги состояния потока */

#define LDB_THREAD_CONT		1	/* продолжить после останова */
#define LDB_THREAD_O_STEP	2	/* выполнить один шаг */
#define LDB_THREAD_QUIET	4	/* выполнить один шаг без печати на консоль */
#define LDB_TRACE_MODE		8	/* установить режим трассировки */

#define LDB_EXCEPTION  0x10 /* продолжить после exception */

#define LDB_HWBREAK   0x100 /* аппаратный останов по записи */

/* типы возможных остановов */

#define BRK_USER         0x1	/* пользовательский останов */
#define BRK_USER_QUIET	 0x2	/* пользовательский останов без вывода на консоль */
#define BRK_STEP_OVER	 0x4	/* служебный останов укрупненных шагов */
#define BRK_SINGLE_STEP	 0x8	/* служебный останов пошагового режима */
#define BRK_TEMP	    0x10    /* временный служебный останов */

#define BRK_SPECIAL     0x20    /* */

#if (BRK_HW_BP != 0)
typedef struct brkHw
{
	sdlNODE	  nodeHw;	   /* узел элемента  */
    int		  typeHw;	   /* тип останова   */
    int 	   regHw;	   /* номер регистра */
} HWBP;
#endif /* (BRK_HW_BP != 0) */

typedef struct brkElement  /* BRKELEM: элемент списка остановов */
{
	sdlNODE 	  node;	   /* узел элемента */
    u_long         thread;  /* поток для которого (0-для всех) */
    u_long         count;   /* сколько раз пропустить */
    u_long         type;	   /* тип точки останова */
    COMMAND       *addr;   /* адрес */
    COMMAND 	  command; /* оригинальный код команды */
#if	(BRK_HW_BP != 0)
    HWBP          *pHw ;   /* ссылка на аппаратную точку останова */
#endif /* (BRK_HW_BP != 0) */
} BRKELEM;

#ifdef __cplusplus
extern "C" {
#endif

#define LEVEL_BREAK    0x10   /* Прекратить раскрутку стека */

#define PRINT_FRAME    0x0100 /* Напечатать строку стека вызовов */
#define PRINT_ERROR    0x0200 /* Напечатать сообщение об ошибке */
#define PRINT_COM      0x0400 /* Напечатать команду */
#define PRINT_EXP      0x0800 /* Вместо имени функции: -->() */
/*  Старшие 16 байтов переменной флаг содержат код ошибки */
#define NOT_FOUND_ADDR_CF 1  /* Не найдена команда вызова функции */
#define NOT_FOUND_ADDR_BF 2  /* Не найдено начало функции */
#define BAD_ADDR_PC       3  /* Адрес текущей команды не корректен */
#define EXCEPTION         4  /* При обработке кадра стека произошёл exception */
#define BAD_ESP           5  /* Указатель на стек не корректен */
#define BAD_EBP           6  /* Указатель на кадр стека не корректен */
#define LDB_OVERFLOW      7  /* Стек потока будет переполнен в следующем кадре */
#define GETERROR_CODE(a) ((a)>>16)
#define SETERROR_CODE(code) ((code)<<16)
typedef struct
{
 int flags ;         /* Что печатать */
 COMMAND *callAdrs ; /* Адрес команды откуда вызывают функцию */
 COMMAND *funcAdrs ; /* Адрес функции которую вызываем */
 u_long *pArgs ;     /* Адрес начала аргументов */
 int nargs ;         /* Количество аргументов */
 void *userData ;    /* Указатель на структуру Elf32_func (private/elf_repl.h) */
 int level ;         /* Номер уровня (нумерация начинается с 0) */
} FRAME_PARMS ;
/* Стандартная функция печати кадра стека с помощью функции printf: */
extern int ldbPrintCall(FRAME_PARMS *params);
/* Стандартная функция печати кадра стека с помощью функции kprint  */
/* и без таблицы символов несистемных процессов: */
extern int ldbKPrintCall(FRAME_PARMS *params); 
/* Стандартная функция печати кадра стека с помощью функции hmPrintf  */
extern int ldbHPrintCall(FRAME_PARMS *params); 
 /* Печать ошибки, полученной при раскрутке стека функции */
extern void ldbPrintErrorCall(FRAME_PARMS *params,int (printf)(const char *fmt,...));
/*-----------------------------------------------------------------*/
void backTrace( int (printLvl)(FRAME_PARMS *params), void *userData, int forw) ;
#define  TOPDOWN     0x01  /* вывод раскрутки стека сверху вниз, иначе снизу вверх */
/* Параметр forw определяет направление печати раскрутки (сверху вниз/снизу вверх )*/
/*-----------------------------------------------------------------*/

/* Внешние функции локального отладчика */

extern int 	ldbC (int thread, COMMAND *addr);
extern int 	ldbS (int thread, COMMAND *addr);

/*extern void dH (void);*/
extern int ldbInclude (void);
extern int ldbB(COMMAND *addr, int thread, int count, int quiet);
#if (CPU == R4000)
extern int ldbBh(COMMAND *addr, int thread, int count, int quiet);
#else
extern int ldbBh(COMMAND *addr, int type, int thread, int count, int quiet);
#endif 
extern int ldbBd (COMMAND *addr, int thread);
extern int ldbBdall (int thread);
extern int ldbCret (int thread);
extern int ldbSo (int thread);

extern void ldbL (COMMAND *addr, int count);
extern void ldbLb (void) ;

extern void ldbTt (int thread);
extern void ldbStt(void);
void ldbTRegs (int thread, char *regName) ; /* tRegs - печать регистров потока */
void ldbTfRegs (int thread, char *regName, int typeP) ; /* tfRegs - печать плавающих регистров */
void fppRegsShow(threadTCB *tcbTh, char *regs, int tf, int (print)(const char *fmt,...)) ;

void ldbDsf (int rgFl, int fppRg, int maxBrk) ;
void ldbTb (int thread) ;
void ldbTu (int thread) ;
void ldbDsb(int reg) ;

extern int	ldbBrkExists (COMMAND *addr, int thread);
extern funcptr_t	ldbBreakNotifyInstall (funcptr_t breakpointRtn);
extern int	ldbStepQuiet (int thread);
extern int	bdThread (int thread);
extern unsigned char *ldbList(unsigned char *addr,	int count, int flgTop) ;
unsigned char *__ldbList(unsigned char *addr, int count, int flgTop,
                         int(printf)(const char *fmt,...)) ;
extern int setThreadStatusBr(int thread, int breakable) ;
extern void ldbCa (void) ;

extern int getSizeCom(unsigned char *addr) ;

#define MAX_NAME_SIZE 80

#define PRIO_NORM  1

extern  char  *ldbMqName ;
typedef struct ldbOneMsg {
 funcptr_t func ;
 void *arg ;
 int numMsg ;
} LDB_MSG ;

typedef struct {
 char *regName ;
 int  regNum ;
} REG_NAME;


typedef struct {
    funcptr_t  WhCom ;     /* сколько команд печатать (COMMAND *addr) */
    funcptr_t  SizeGet ;   /*(COMMAND *brkInst);*/
    funcptr_t  StepAdd  ;  /* (int thread, int brkType, int *pEsf); */
    funcptr_t  FuncCallCheck ; /* (COMMAND *addr); */
    vfuncptr_t  ThreadPCSet ; /* (int thread, COMMAND *pc) ; */
    funcptr_t  AdrChkRtn ;   /* можно ли ставить break */
    vfuncptr_t  OStepSet ; /* (int *pEsf) ; */
    vfuncptr_t  VecInit ; /* (void) ; */
    vfuncptr_t  RegsAdjust ; /*  (int thread, int *pEsf) ; */
    vfuncptr_t  IntrSave ; /*   (int *pEsf) ; */
    vfuncptr_t  IntrRestore ; /* (int *pEsf) ; */
    vfuncptr_t  RegsShow ; /* (int thread, char *regName) ; */
    vfuncptr_t  fppRegsShow ; /* (int thread, char *regName, int typeP) ; */
    COMMAND * (*ThreadPCGet)() ; /* (int thread) ; */
    COMMAND * (*RetAdrsGet)() ; /* (threadTCB *tcbTh); */
    COMMAND * (*PCGet)() ; /* (int *pEsf) ; */
    funcptr_t prn ; /* печать сообщений отладчика */
    funcptr_t ldP ; /* отладочная печать */
    vfuncptr_t  TraceSet ;       /* (int thread) ; */
    vfuncptr_t  TraceDisable ;   /* (int *pEsf) ; */
#if (BRK_HW_BP!=0)
    vfuncptr_t  dbRegsShow ;  /* (char *regName) ; */
    vfuncptr_t  HwSet ;       /* (COMMAND *addr, HWBP *hw) ; */
    vfuncptr_t  HwClear ;     /* (HWBP *hw) ; */
    funcptr_t  HwCheck ;         /* (threadTCB *thread) ; */
#endif /* (BRK_HW_BP!=0) */
} ArchFunct ;

extern int  ArchDebInit (ArchFunct *_db) ;
void sendPrMsg(char *msg) ;
	
#ifdef __cplusplus
}
#endif

#endif /* _LDEBUG_H */
