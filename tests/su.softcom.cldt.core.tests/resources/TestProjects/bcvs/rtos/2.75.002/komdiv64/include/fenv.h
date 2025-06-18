#ifndef _FENV_H
#define _FENV_H

#include <sys/types.h>
/* Режимы округления */
#define FE_DOWNWARD     2       /* округление в сторону минус бесконечности;            */
#define FE_TONEAREST    0       /* округление к ближайшему представимому числу;         */
#define FE_TOWARDZERO   3       /* округление в сторону 0;                              */
#define FE_UPWARD       1       /* округление в сторону плюс бесконечности.             */

/* Флаги исключительных ситуаций     */
#define FE_DIVBYZERO            (1<<0)  /* деление на 0 (D);                  */
#define FE_INEXACT              (1<<1)  /* потеря точности (I);               */
#define FE_INVALID              (1<<2)  /* недопустимая операция (V);         */
#define FE_OVERFLOW             (1<<3)  /* переполнение (O);                  */
#define FE_UNDERFLOW            (1<<4)  /* слишком малый результат (U);       */

#define FE_ALL_EXCEPT           ( FE_DIVBYZERO|FE_INEXACT|FE_INVALID|FE_OVERFLOW|FE_UNDERFLOW )

/* Код операции может принимать следующие значения       */

#define FPOP_ADD     1       /* сложение                                               */
#define FPOP_SUB     2       /* вычитание                                              */
#define FPOP_MUL     3       /* умножение                                              */
#define FPOP_DIV     4       /* деление                                                */
#define FPOP_SQRT    5       /* извлечение квадратного корня                           */
#define FPOP_CVT     6       /* преобразование форматов                                */
#define FPOP_FMA     7       /* операция "умножение со сложением" и ее разновидности   */
#define FPOP_CMP     8       /* cравнение                                              */
#define FPOP_MISC    9       /* прочие (абсолютная величина, смена знака и др.);       */

#define FPOP_RAISE   99
#ifndef __ASSEMBLER__
typedef enum {
   E_DIVBYZERO,
   E_INEXACT,
   E_INVALID,
   E_OVERFLOW,
   E_UNDERFLOW
} exceptions;


typedef enum {
   FPFMT_S  =   16,          /* double op_d                                            */
   FPFMT_D  =   17,          /* float  op_ss                                           */
   FPFMT_W  =   20,          /* unsigned op_w[fp_lo]                                   */
   FPFMT_L  =   21,          /* unsigned long long op_l                                */
   FPFMT_PS =   22,
   FPFMT_dsp =  23,
   FPFMT_0   =  99
} fpfmt_t;


/* Типы */


typedef int fexcept_t;       /* флаги исключительных ситуаций;                         */
typedef union
{
    double             op_d;
    float              op_s;
    unsigned           op_w[2];
    unsigned long long op_l;
} feOP;
#include <private/fenv.h>



/* функция обработки исключительных ситуаций.   */

typedef int (*feACTION)(feDATA *);

/* среда выполнения операций с плавающей точкой; */
typedef struct
{
        int fe_flags;
        feACTION        fe_actions[5];
        int fe_mask;
        int fe_rounding;
} fenv_t;

/* Указатель на используемую по умолчанию среду выполнения операций с плавающей точкой
   (см. функции fesetenv() и feupdateenv()):
*/
extern fenv_t fe_dfl_env;
#define FE_DFL_ENV (&fe_dfl_env)



/* Сброс флагов исключительных ситуаций при операциях с плавающими числами             */
int feclearexcept(int excepts);
/* Проверка флагов исключительных ситуаций при операциях с плавающими числами          */
int fetestexcept(int except);

/* Опрос функции обработки исключительных ситуаций при операциях с плавающими числами  */
feACTION feGetAction(int except);
/* Установка функции обработки исключит. ситуаций при операциях с плавающими числами   */
feACTION feSetAction(int except, feACTION action);

/* Опрос маски исключительных ситуаций при операциях с плавающими числами              */
int feGetMask(void);
/* Установка маски исключительных ситуаций при операциях с плавающими числами          */
int feSetMask(int mask);

/* Опрос режима округления           */
int fegetround(void);
/* Установка режима округления       */
int fesetround(int round);


/* Сохранение среды выполнения операций с плавающими числами       */
int fegetenv(fenv_t *envp);
/* Восстановление среды выполнения операций с плавающими числами   */
int fesetenv(const fenv_t *envp);
/* Сохранение среды выполнения операций с плавающими числами       */
int feholdexcept(fenv_t *envp);
/* Восстановление среды выполнения операций с плавающими числами   */
int feupdateenv(const fenv_t *envp);


/* Сохранение флагов исключительных ситуаций при операциях с плавающими числами        */
int fegetexceptflag(fexcept_t *flagp, int excepts);
/* Восстановление флагов исключительных ситуаций при операциях с плавающими числами    */
int fesetexceptflag(const fexcept_t *flagp, int excepts);

/* Генерация исключительных ситуаций */
int feraiseexcept(int excepts);
/* Генерация исключительных ситуаций */
typedef enum {
   feeNOTHING_HAPPEND,
   feeHANDLER_CALLED,
   feeREPEAT_COMMAND
} feraiseexcept_t;
feraiseexcept_t feRaiseExcept(int except,feDATA *data);
/* Инициализация средств обработки исключительных ситуаций при опер. с плав. числами   */
int feInit(void);
int osFeInit(void);

int fpgetsticky(void);
int fpsetsticky(int sticky);

struct pt_regs;
void feGetContext(struct pt_regs *r);
void feSetContext(struct pt_regs *r);

int cpuFpaSetRounding(int how) ;
int cpuFpaGetRounding() ;
int cpuFpaSetMask(int mask) ;
int cpuFpaInit(vfuncptr_t fptr);
int cpuFpaGetSticky();
int cpuFpaClearSticky();

void fpsetflag();
struct thread_regs_fp;
#else
#include <private/fenv.h>
#endif /* !__ASSEMBLER__ */

#endif /* !_FENV_H */
