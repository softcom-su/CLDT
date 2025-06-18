/* mpobjcore.h*/
#ifndef _MPOBJCORE_H
#define _MPOBJCORE_H

#ifdef KERNEL
#include <private/objcore.h>
#include <mp/mpsdlist.h>
#include <mp/mpspin.h>

/*расположение описателей классов в общей памяти*/
#define NM_CLASS_MEMCONTROLMP        0
#define NM_CLASS_SEMCONTROLMP        1
#define NM_CLASS_SEMPXCONTROLMP      2
#define NM_CLASS_MQPCONTROLMP        3
#define NM_CLASS_MQPPXCONTROLMP      4
#define NM_CLASS_THREADCONTROLMP     5
#define MP_NUMBERS_CLASS             6  /*всего классов в общей памяти*/ 

/* Блок управления объектами */

typedef struct mpobj_control
{
	  objHEADER   objHdr;	          /* Заголовок управляющего блока */
    u_short     type;             /* Тип объектов, управляемых контролем */
    u_short     flags;            /* Флаги управления */
    u_long      objSize;          /* Размер объекта */
    u_long      total;            /* Размер объекта */
    mpsdlHEAD   objList;          /* Список активных объектов */
    mpsdlHEAD   freeList;         /* Список свободных объектов */
    mpoffset_t  memPoolId;
    u_long      objInitCnt;	      /* Общее число инициализированных объектов */
    u_long      objDestroyCnt;	  /* Общее число разрушенных объектов */
    spin_t      spin;             /* spin */
} mpOBJCONTROL, *mpOBJCONTROL_ID;

typedef struct mpobj_cnt_loc /*локальный описатель класса*/
{
    mpOBJCONTROL_ID pClass;       /* лок.адрес описателя в общей памяти */
    u_short        flags;            /* Флаги управления */
    u_long       objSize;          /* Размер объекта */
    struct mpmem_pool *memPoolId; /* Пул памяти для создания объектов */
    funcptr_t     allocRtn;         /* Функция создания объекта */
    funcptr_t     initRtn;          /* Функция создания объекта */
    funcptr_t	    freeRtn;	        /* Функция освобождения памяти взятой под объект */
    funcptr_t     showRtn;	        /* Вывод информации об объекте */
    funcptr_t	    usrValidate;	    /* Дополнительный обработчик проверки объекта */
    funcptr_t	    usrFree;	        /* Дополнительный обработчик освобождения памяти */
} mpOBJCONTROLLOC, *mpOBJCONTROLLOC_ID;

/*обслуживание элементов заголовка объекта*/
#define mpOBJCONTROL_SET(classId, objId) \
    ((objHEADER *)objId)->m_pClass = (objCONTROL*)mpAdrOffset(classId)
#define mpOBJCONTROL_GET(objId) ( (mpOBJCONTROL*)mpAdrLocal(mpOBJCONTROLOFF_GET(objId)) )
#define mpOBJCONTROLOFF_GET(objId) ( (mpoffset_t)((objHEADER *)objId)->m_pClass )
/*идентификатор объекта*/
#define mpOBJIDOBJ_SET(h,idObj) \
    ((objHEADER*)h)->m_idObj = (void*)mpAdrOffset((void*)idObj)
#define mpOBJIDOBJ_GET(h) \
    mpAdrLocal((u_long)((objHEADER*)h)->m_idObj)
/*порядковый номер объекта*/
#define mpOBJCOUNTER_SET(objId,cnt) ((objHEADER*)(objId))->m_idCounter = htonl(cnt)
#define mpOBJCOUNTER_GET(objId)     ntohl( ((objHEADER *)objId)->m_idCounter )
/*именная ссылка*/
#define mpOBJIDNAME_SET(h,idName) ((objHEADER*)h)->m_idName = mpAdrOffset((void*)(idName))
#define mpOBJIDNAME_GET(h)        mpAdrLocal(((objHEADER*)h)->m_idName)

#define mpOBJCHECK(classId_Off, objId ) ( (mpOBJCONTROLOFF_GET(objId) == classId_Off) ? 0 : -1 )

int mpObjInitControl ( 
         mpOBJCONTROL_ID pClass, 
         u_short type, 
         u_short flags, 
         struct mpmem_pool *mpPoolId, 
         u_long objSize, 
         u_long total );
int mpObjInitControlLoc( 
         mpOBJCONTROL_ID pClass, 
         mpOBJCONTROLLOC_ID pClassLoc,
         funcptr_t showRtn, 
         funcptr_t usrValidate, 
         funcptr_t usrFree );

void *mpObjCreate ( mpOBJCONTROLLOC_ID pClass, u_long addSize );
int   mpObjInitHeader ( mpOBJCONTROL_ID pClass, objHEADER  *pObj );
int   mpObjDestroy ( mpOBJCONTROLLOC_ID pClass, objHEADER *pObj );
int   mpObjRemove ( mpOBJCONTROLLOC_ID pClass, objHEADER *pObj );

int   mpObjDisplay ( mpOBJCONTROLLOC_ID pClass, objHEADER *pObj, int showType );
int   mpObjDisplayList ( mpOBJCONTROLLOC_ID pClass, int showType );

#endif /*KERNEL*/

#endif /*_MPOBJCORE_H*/
