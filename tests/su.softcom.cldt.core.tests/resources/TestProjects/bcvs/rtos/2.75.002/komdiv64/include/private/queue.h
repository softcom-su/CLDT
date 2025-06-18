/*  queue.h  */

/*
14.02.2003 избавляемся от queFlushDpc, делаем inline queUNLOCK_DPC
*/

#ifndef _QUEUE_H
#define _QUEUE_H

#include "private/objcore.h"
#include "private/ddlist.h"
#include "arch/cpu.h"

#ifdef __cplusplus
typedef struct que_node *(*qnodefuncptr_t) (...);
#else
typedef struct que_node *(*qnodefuncptr_t) ();
#endif

/* Типы очередей */
#define	queTYPE_FIFO    queFifoCtrlId
#define	queTYPE_LIFO    queLifoCtrlId
#define	queTYPE_PRIO    quePrioCtrlId
#define	queTYPE_SPAN    queSpanCtrlId
#define	queTYPE_PRIO32  quePrio32CtrlId
#define	queTYPE_PRIO256 quePrio256CtrlId
#define	queTYPE_SPAN256 queSpan256CtrlId

/* Управляющая структура для очередей */
typedef struct que_control{
    objCONTROL      m_objClass;
    funcptr_t         m_moduleRtn;
    funcptr_t         m_initRtn;
    qnodefuncptr_t    m_lastRtn;
    qnodefuncptr_t    m_nextRtn;
    qnodefuncptr_t    m_prevRtn;
    funcptr_t         m_insertRtn;
    qnodefuncptr_t    m_extractRtn;
    funcptr_t         m_parmRtn;
    funcptr_t         m_advanceRtn;
    funcptr_t         m_updateRtn;
} queCONTROL, *queCONTROL_ID;

extern queCONTROL_ID queFifoCtrlId;
extern queCONTROL_ID queLifoCtrlId;
extern queCONTROL_ID quePrioCtrlId;
extern queCONTROL_ID queSpanCtrlId;
extern queCONTROL_ID quePrio32CtrlId;
extern queCONTROL_ID quePrio256CtrlId;
extern queCONTROL_ID queSpan256CtrlId;

/* Заголовок очереди */
typedef struct que_head {
    objHEADER        m_objHdr;   /* указатель на управл.структуру объекта */
    objHEADER       *m_owner;    /* ссылка на объект, которому
                                    принадлежит очередь */
    volatile int     m_lock;     /* Атрибут блокирования очереди */
    volatile int     m_dpcCnt;   /* Счетчик отложенных вызовов 
                                    процедуры m_dpcRtn */
    funcptr_t          m_dpcRtn;   /* Процедура обработки отложенных запросов */
    struct que_node *m_first;    /* Указатель на первый элемент очереди 
                                    или NULL если пусто*/
    u_long           m_common[3];/* Общий массив для всех очередей */
} queHEAD;

/* Описание базового элемента */
typedef struct que_node {
	ddlNODE	   m_node;   /* Узел для включения в очередь */
	objHEADER *m_owner;  /* Ссылка на объект в который входит данный элемент */
} queNODE;

/* Описание параметризированного элемента */
typedef struct {
	queNODE	m_node;
	u_long 	m_parm;    /* Параметр элемента очереди */
} quePARM_NODE;

/* Описание элемента параметризированной очереди с хуком*/
typedef struct {
	quePARM_NODE m_node;
    funcptr_t      m_hookRtn; /* Процедура обработки */
} queHOOK_NODE;


#ifdef __cplusplus
extern "C" {
#endif

/* Динамическое создание очереди */
queHEAD *queCreate ( queCONTROL_ID queType, objHEADER *owner );
/* Удаоение очереди */
int  queRemove ( queHEAD *que_head );
/* Макросы для обращения к методам очереди */
#define queInitModule(queCtrl) (*(queCtrl)->m_moduleRtn)(queCtrl)
#define queInit(queCtrl, queHead, owner) (*(queCtrl)->m_initRtn)(queCtrl, queHead, owner)
#define queFirst(que_head) (((queHEAD *)que_head)->m_first)
#define queLast(que_head) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_lastRtn)(que_head)
#define queNext(que_head, que_node) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_nextRtn)(que_head, que_node)
#define quePrev(que_head, que_node) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_prevRtn)(que_head, que_node)
#define queInsert(que_head, que_node, parm) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_insertRtn)(que_head, que_node, parm)
#define queExtract(que_head, que_node) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_extractRtn)(que_head, que_node)
#define queGetParm(que_head, que_node, parm) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_parmRtn)(que_head, que_node, parm)
#define queAdvance(que_head) (*((queCONTROL_ID)objCONTROL_GET(que_head))->m_advanceRtn)(que_head)
/* Дополнительные макросы */
#define queIsEmpty(que_head) (queFirst(que_head) == NULL)
#define queUpdateParm(que_head,que_node,newParm) \
	queExtract(que_head, (queNODE *)que_node); \
	queInsert(que_head, (queNODE *)que_node, newParm);
/* Макросы для установки/получения владельца очереди */
#define  queSetOwner(que_head, owner)  ((queHEAD *)(que_head))->m_owner  = (objHEADER *)owner
#define  queGetOwner(que_head)  ( ((queHEAD *)(que_head))->m_owner )
/* Макросы для установки/получения параметров элемента очереди */
#define  queSetNodeOwner(que_node, owner) ((queNODE *)(que_node))->m_owner = (objHEADER *)owner
#define  queGetNodeOwner(que_node)  ( ((queNODE *)(que_node))->m_owner )
#define  queSetHookNode(que_node, owner, hook) \
    queSetNodeOwner(que_node, owner); \
    (que_node)->m_hookRtn = (funcptr_t)hook;

/* Макросы и функции используемые для обеспечения одновременного
   использования очереди несколькими потоками */

#define queSetDpcRtn(que_head, dpcRtn) (que_head)->m_dpcRtn = (funcptr_t) dpcRtn;

#define queLOCK(que) (que)->m_lock = TRUE
#define queIS_LOCKED(que) ((que)->m_lock )
#define queUNLOCK(que) (que)->m_lock = FALSE

/*#define queUNLOCK_DPC(que) { queUNLOCK(que); if ((que)->m_dpcCnt) queFlushDpc(que); }*/
#define queUNLOCK_DPC(que)  queUnlockDpc(que);

#define queLock(que) { \
   kernDispatchDisable(); \
   queLOCK(que); }


#define queUnlock(que) { \
   queUNLOCK_DPC(que); \
   kernDispatchEnable(); }

#define queINCREASE_DPC_CNT(que) \
    { \
        int level; \
        level = intLock(); \
        (que)->m_dpcCnt++; \
        intUnlock(level);  \
    }

void queUnlockDpc( queHEAD *que );

void queReleaseDpc(queHEAD *que);
/*void queFlushDpc(queHEAD *que);*/

#define queGETLIST_MAXSIZE   20
int queGetList(	queHEAD *que, queNODE **nodes, int maxnodes );

#ifdef __cplusplus
}
#endif

#endif  /* _QUEUE_H */

