/* mpdic.h*/

#ifndef _MPDIC_H
#define _MPDIC_H


#include <mp/mpmem.h>

enum { /*типы элементов, помещенных в словарь*/
  DIC_TYPE_SEM = 1,
  DIC_TYPE_MQ,
  DIC_TYPE_POOL,
  DIC_TYPE_MEM,
  DIC_TYPE_INT
};

enum {
	DIC_SHOW_HEAD =         1,
	DIC_SHOW_NAME =         2,
	DIC_SHOW_WAIT =         4
};

int   mpDicAdd( void *p, char *name, int type );
int   mpDicDelete( char *name, int type );
int   mpDicFind( char *name, int type, void **p );
int   mpDicFirst( void **p, char **name, int *type, void **pos );
int   mpDicNext( void **p, char **name, int *type, void **pos );
int   mpDicInit( mpMEMPOOL_ID pool, int num );
void  mpdic( char *filter );

#ifdef KERNEL

#include <limits.h>
#include <mp/mpsdlist.h>
#include <mp/mpobjcore.h>

#define DIC_INIT         1 /*проинициализирован*/
#define DIC_SEM_INIT     2 /*семафор проинициализирован*/
#define DIC_SIGNATURE    0xdcba9800 


#define DIC_CHECK()   \
      ((ntohl(mpObjDic->flags) & (DIC_INIT | DIC_SIGNATURE)) \
      == (DIC_INIT | DIC_SIGNATURE))

#define DIC_IS_SEM_INIT()   \
      (ntohl(mpObjDic->flags) & DIC_SEM_INIT)

#define DICTNAME_LENGTH   NAME_MAX   /*макс.длина имени в словаре*/
/* Заголовок очереди прерываний*/
typedef struct mpdict_head {
  u_long             flags;
  sem_t              sem;
  mpsdlHEAD          list;    /*список использованных элементов */
  mpsdlHEAD          free;    /*список свободных элементов*/
  u_long             count;   /*текущее число элементов*/
} mpDIC_HEAD;

typedef struct mpdict_node {
    mpsdlNODE  node;
    char       name[DICTNAME_LENGTH + 1];   /*имя объекта*/
    u_long     type;                        /*тип объекта*/
    mpoffset_t offset;                      /*смещение в об.памяти объекта*/
} mpDIC_NODE;

#define  mpDIC_OBJECT_NAME(pnode)  (pnode)->name  
#define  mpDIC_OBJECT_TYPE(pnode)  (pnode)->type  
#define  mpDIC_OBJECT(pnode)       mpAdrLocal((pnode)->offset)  

extern mpDIC_HEAD *mpObjDict; /*локальный адрес словаря объектов общей памяти*/

void mpDicInitSem( void ); /*инициализация словарного семафора*/
int mpDicInsert( void *p, char *name, int type, mpDIC_NODE **node_dic );

int mpDicShowInit( void );

#endif /* KERNEL */

#endif /*_MPDIC_H*/
