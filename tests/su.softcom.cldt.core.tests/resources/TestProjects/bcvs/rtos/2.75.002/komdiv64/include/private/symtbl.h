#ifndef _PRIVATE_SYMTBL_H
#define _PRIVATE_SYMTBL_H

// #ifdef KERNEL

#include <sys/queue.h>
typedef enum   {
    tVoid,tUnknown,tChar,tWord,tInt,tFloat,tDouble,tStr,tId,tInt64
} Type;

typedef enum {
    pLeft,pRight,pFunction
} Pos;

#define YYSTYPE VVV


typedef struct sym_entry {
    STAILQ_ENTRY(sym_entry)   m_Next;
    char     *m_Name;
    char     *m_Type;
    void     *m_Ptr;
    int         m_module;
    double      m_Value;
} symENTRY;

typedef struct {
    Type type;
    Pos  pos;
    union {
        int      *p;
        char  c;
        short w;
        int      i;
        char  *s;
        float   f;
        double   d;
        long long int i64;
    }u;
    symENTRY *se;
} VVV;

struct symtbl {
    char *name;
    void *addr;
    char type[20];
};
extern struct symtbl sym_tbl[];

YYSTYPE getvalue(YYSTYPE *);
YYSTYPE setvalue(YYSTYPE *,YYSTYPE *);
YYSTYPE lkupcmd(int,YYSTYPE *);

int symInit();
int symLoad();
int getnumth(int thread) ;
int getid(char *);
void shellHelp(const char *prm);

symENTRY *symFind(const char *name);
symENTRY *symGet(void);
symENTRY *symCFind(const char *name);
void      symResolve(int addr,void *,char *);
int       symAddDin(char *name,int *v,char *type, uint module) ;
int       symAdd (char *name,int *v,char *type) ;
int       symModRemove(int modId) ;
void     *symFindName( char *name, char *type );

// #endif /* KERNEL */

struct help_node {
    char *_n;      /* Имя функции  */
    char *_sh;     /* Краткий хэлп */
    char *_fh;     /* Полный хэлп  */
    void *_a;      /* Адрес ф-ции  */
};


struct categ_node {
    char _c[10];
    char *_s;
    struct help_node *_hn;
    int help_node_list_size;
};
void helpAddCateg(struct categ_node *);

struct shellStartParam {
    int ssp_flags;
#define SHELL_SSP_FDIN      0x1
#define SHELL_SSP_FDOUT     0x2
#define SHELL_SSP_FDERR     0x4
#define SHELL_SSP_FDALL     SHELL_SSP_FDIN | SHELL_SSP_FDOUT  | SHELL_SSP_FDERR
#define SHELL_SSP_PROMPT    0x8
#define SHELL_SSP_EXEC      0x10
#define SHELL_SSP_SSPMALLOC 0x20
#define SHELL_SSP_NOTRY     0x40
#define SHELL_SSP_SETINPUT  0x80
    int ssp_fdin;
    int ssp_fdout;
    int ssp_fderr;
    char *ssp_prompt;
    char *ssp_exec;
    int (*p_input)(char *buf,int max_size, void *v);
};
void  *osShell(void *arg);

#endif
