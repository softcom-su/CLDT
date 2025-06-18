/*
 * objcore.h
 * 20.11.02 выделены типы классов в sys/objclasses.h
 */
#ifndef _OBJCORE_H
#define _OBJCORE_H

#ifdef KERNEL

#include <sys/types.h>
#include <sys/objclasses.h>
#include <private/sdlist.h>

#define ID_NODE

#ifdef __cplusplus
extern "C" {
#endif


/* флаги определяющие работу контроля объектов */
#define ctrlINITIALIZED    0x01 /* Контроль проинициализирован */
#define ctrlCLEARNEW       0x02 /* При создании объекта расписать его нулями */
#define ctrlACCOUNTING     0x04 /* Созданные объекты учитываются в едином списке */
/* флаги задающие способ выделения памяти при создании нового объекта */
#define ctrlSTATIC         0x10 /* Память под объект не выделяется */
#define ctrlDYNCREATE      0x20 /* Память при создании объекта брать динамически */
#define ctrlLISTPOOL       0x40 /* Объекты выдаются из заранее выделенного пула - список m_freeList */
#define ctrlEXPAND         0x80
#define ctrlMP            0x100 /* Объекты принадлежат общей памяти */
#define ctrlNAME          0x200 /* Объекты подлежат именованию по умолчанию */

#define ctrlLISTPOOLEX     ctrlLISTPOOL|ctrlEXPAND /* Если список m_freeList, выделять новый пул*/

/* Флаги объекта */
#define objFLAGS_TRACING     0x04      /* объект в списке трассируемых, события с
                                          объектом регистрируются независимо от маски  */
/*#define objFLAGS_TRACETHREAD 0x08*/      /* поток трассируется (входит в категорию
                                          трассируемых потоков)                        */

typedef struct obj_header     /* objHEADER */
{
   struct  obj_control *m_pClass;   /* указатель на управляющую структуру объекта */
   void   *m_pCreator;
//   #define traceOBJECT_YES     1    /* события с объектом регистрируются    */
//   #define traceOBJECT_NO      0    /* события с объектом не регистрируются */
   u_long  m_Flags;                 /*  флаги объекта                       */
   void   *m_idObj;                 /* указатель на идентификатор объекта   */
   u_long  m_idName;                /* идентификатор имени объекта          */
   sdlNODE m_listNode;              /* узел для включения в список объектов */
   u_long  m_idCounter;             /* порядковый номер объекта в классе    */
}  objHEADER;

/* Получение заголовка объекта по объекту                */
#define objTO_HEADER(obj) \
     ((objHEADER*)(obj))


#ifdef MAX_PERF
# define OBJ_NAME_MTX(pObj,name)
#else
# define OBJ_NAME_MTX(pObj,name)  objSetName(CLASS_MTXCONTROLPX,(objHEADER*)pObj,name)
#endif

#ifndef ITEM_OFFSET
#define ITEM_OFFSET(structure, member) ((int) &(((structure *) 0) -> member))
#endif

#define objNODE_TO_HDR(pNode) ( (objHEADER *) ((unsigned char  *)pNode - ITEM_OFFSET(objHEADER, m_listNode)) )

/* Блок управления объектами */

typedef struct obj_control /* objCONTROL */
{
    objHEADER     m_objHdr;           /* Заголовок управляющего блока           */
    u_short       m_type;             /* Тип объектов, управляемых контролем    */
    u_short       m_flags;            /* Флаги управления                       */
    u_long        m_objSize;          /* Размер объекта                         */
    u_long        m_total;            /* число объектов                         */
    sdlHEAD       m_objList;          /* Список активных объектов               */
    sdlHEAD       m_freeList;         /* Список свободных объектов              */
    struct mem_pool *m_memPoolId;     /* Пул памяти для создания объектов       */
    u_long        m_objInitCnt;       /* Общее число инициализированных объектов*/
    u_long        m_objDestroyCnt;    /* Общее число разрушенных объектов       */
    funcptr_t     m_allocRtn;         /* Функция создания объекта               */
    funcptr_t     m_initRtn;          /* Функция создания объекта               */
    funcptr_t     m_freeRtn;          /* Функция освобождения памяти взятой под объект */
    funcptr_t     m_showRtn;          /* Вывод информации об объекте            */
    funcptr_t     m_usrValidate;      /* Дополнительный обработчик проверки объекта */
    funcptr_t     m_usrFree;          /* Дополнительный обработчик освобождения памяти */
    char*         m_prefName;         /* Префикс имени объектов по умолчанию    */
    const char   *m_className;            /* указатель на имя объекта класса         */
} objCONTROL, *objCONTROL_ID;

int     objInitModule ( void );
int     objInitControl ( objCONTROL_ID pClass, u_short type, u_short flags, struct mem_pool *memPoolId,
          u_long objSize, u_long total, funcptr_t showRtn, funcptr_t usrValidate, funcptr_t usrFree );
void   *objCreate ( objCONTROL_ID pClass, u_long addSize);
void    objInitHeader ( objCONTROL_ID pClass, objHEADER *pObj );
void    objDestroy ( objCONTROL_ID pClass, objHEADER *pObj );
#define objFree(pClass,pObj) (*((objCONTROL_ID)(pClass))->m_freeRtn)((pClass),(pObj))
int     objRemove ( objCONTROL_ID pClass, objHEADER *pObj );
int     objDisplay ( objHEADER *pObj, int showType );
int     objDisplayList ( objCONTROL_ID pClass, int showType );
void    objDisplayControl(objCONTROL_ID pClass,int (*f)());
void    objHashNamesInit( void ); /* таблица имен объектов */
void    objHashInitMutex( void ); /* инициализация ее мьютекса */
void    objInitNaming ( objCONTROL_ID pClass );
int     objClassName ( int class_type, const char** name );
void    objGetName ( objHEADER  *pObj, char **name );
int     objSetName ( int type, objHEADER  *pObj, char *name );
int     objRemoveName ( objHEADER  *pObj );
int     objHashNamesLock( void );
int     objHashNamesUnlock( void );
void   *objHashNamesFind( char *name, int type );
void    objHashShow ( void );
#define objINIT (struct  obj_control *)0x12345678
#define objCONTROL_GET(objId) ( ((objHEADER *)objId)->m_pClass )
#define objCONTROL_SET(classId, objId) ( objCONTROL_GET(objId) = classId )
#define objCOUNTER_SET(classId, objId) ( ((objHEADER*)(objId))->m_idCounter = classId->m_objInitCnt )
#define objCOUNTER_GET(objId) ( ((objHEADER *)objId)->m_idCounter )

extern objCONTROL *objClasses[];
#define objTYPE_TO_CLASS(type)    objClasses[type]

#ifndef MAX_PERF
#  define objCHECK(classId, objId ) ( ((objId != NULL) && (objCONTROL_GET(objId) == (objCONTROL *)classId)) ? 0 : -1 )
#else
#  define objCHECK(classId, objId ) ( (objCONTROL_GET(objId) == (objCONTROL *)classId) ? 0 : -1 )
#endif

#define objIDNAME_SET(h,idName) ((objHEADER*)h)->m_idName = (u_long)(idName)
#define objIDOBJ_SET(h,idObj) ((objHEADER*)h)->m_idObj = (void*)(idObj)
#ifdef ID_NODE
#  define objIDNAME_GET(h) (hshNODE*)(((objHEADER*)h)->m_idName)
#else
#  define objIDNAME_GET(h) ((objHEADER*)h)->m_idName
#endif
#define objIDOBJ_GET(h) ((objHEADER*)h)->m_idObj

/* Проверка номера класса       */
#define objCHECK_CLASSTYPE(type) \
    (type < CLASS_TOTAL)


/* Установить/снять флаг трассировки системного объекта */
#define objSET_TRACESTATE(obj) \
  ((objHEADER *)(obj))->m_Flags |= objFLAGS_TRACING
#define objUNSET_TRACESTATE(obj) \
  ((objHEADER *)(obj))->m_Flags &= ~objFLAGS_TRACING
#define objIS_TRACING(obj) \
  (((objHEADER *)(obj))->m_Flags & objFLAGS_TRACING)

#define objTO_CONTROL(objId,cntl) cntl=(void*)(*((int*)objId))

#ifdef __cplusplus
}
#endif /* __cplusplus   */
#endif /* KERNEL        */
#endif /* _OBJCORE_H    */
