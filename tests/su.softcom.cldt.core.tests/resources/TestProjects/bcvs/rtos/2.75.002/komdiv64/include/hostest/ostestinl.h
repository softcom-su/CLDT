#ifndef _PRIVATE_OSTESTINL_H
#define _PRIVATE_OSTESTINL_H

#include <errno.h>
#include <hostest/ostest.h>

/* -------------- __oststLock ----------
 *  Функция __oststLock - запретить выполнение тестов.
 * Аргументы:
 *   pOstest   - указатель на описатель тестов процесса.
 */
static inline int __oststLock( struct ss_ostest* pOstest )
{
   if( !pOstest->init )
      return EPERM;
   if( pOstest->lock )
      return EACTION;
   pOstest->lock = 1;
   return 0;
}

/* -------------- __oststUnlock ----------
 *  Функция __oststUnlock - разрешить выполнение тестов.
 * Аргументы:
 *   pOstest   - указатель на описатель тестов процесса.
 */
static inline int __oststUnlock( struct ss_ostest* pOstest )
{
   if( !pOstest->init )
      return EPERM;
   if( !pOstest->lock )
      return EACTION;
   pOstest->lock = 0;
   return 0;
}

/* -------------- __oststStop ----------
 *  Функция __oststStop - выполнение тестов при переключении потоков прекращается.
 * Аргументы:
 *   pOstest   - указатель на описатель тестов процесса.
 */
static inline int __oststStop( struct ss_ostest* pOstest )
{
   struct s_ostest*  p;
   int               i;

   if( !pOstest->init )
      return EPERM;
   if( !pOstest->active )
      return EEXIST; /* не было старта */
   p = pOstest->tests;
   __oststLock( pOstest );
   pOstest->active = 0;
   for( i=1; i < oststLAST; i++ ) {
      if( !p[i].oststFunctionStore )
         continue;   /* тест не имеет функции тестирования */
      p[i].oststFunction = NULL;
      p[i].nActive = 0;
   }
   __oststUnlock( pOstest );
   return 0;
}


#ifndef MAX_PERF
#ifdef KERNEL

#include <try.h>
#include <private/queue.h>
#include <private/q_ddl.h>
#if OS_MAJOR == 3
#  include <sys/proc.h>
#  include <private/copymem.h>
#  include <private/thread.h>
#  include <private/pproc.h>
#else
#  include <mqueue.h>
#endif
#include <private/mqueue.h>

/* -------------- oststObjectActive ----------
 *  Функция oststObjectActive - проверка завершенности объекта
 * Аргументы:
 *   pHdr   - указатель на заголовок объекта.
 */
static inline int oststObjectActive( objHEADER* pHdr )
{
   unsigned flags = pHdr->m_Flags;
#if OS_MAJOR == 2
   flags &= ~( objFLAGS_TRACING);
#else
   flags &= ~(objFLAGS_READY | objFLAGS_TRACING | objFLAGS_SHARED | objFLAGS_CLEAR);
#endif
   if( flags )
      return EINVAL; /* непредусмотренные биты  */

   return 0;
}

/* -------------- oststDdlNode ----------
 *  Функция oststDdlNode - проверка узла объекта, исрользуемого в списке DDL
 * Аргументы:
 *   buf  - указатель на буфер, в который кладутся результаты проверки;
 *   parm - указатель на структуру с опциями тестирования.
 */
static inline int oststDdlNode( void* buf, struct par_ostest* parm )
{
#if OS_MAJOR == 2
   u_int*   pBuf = (u_int*)buf;
   queHEAD* pHead = (queHEAD*)parm->obj1; /* указатель на заголовок очереди */
   ddlNODE* pDdl = (ddlNODE*)parm->obj2;  /* указатель на узел объекта */
   ddlNODE* next;
   ddlNODE* prev;
   int      lvl;
   int      rt = 0;

   lvl = intLock();
//#if OS_MAJOR == 2
   if( pHead && queIS_LOCKED(pHead) ) {
      intUnlock( lvl );
      return oststE_ACCESS; /* невозможно проверить */
   }
//#endif
   if( !pDdl->next ) {
      if( !pDdl->prev ) {
         intUnlock( lvl );
         return 0;   /* узел не стоит в очереди */
      }
      intUnlock( lvl );
      rt = oststE_NODE;
      goto er_node; /* узел испорчен           */
   }
   if( !pDdl->prev ) {
      intUnlock( lvl );
      rt = oststE_NODE;
      goto er_node; /* узел испорчен           */
   }
   /* Стоит в очереди - есть 2 указателя */
   tryBegin();
      next = pDdl->next;
      prev = pDdl->prev;
      if( (next->prev != pDdl) || (prev->next != pDdl) ) {
         intUnlock( lvl );
         rt = oststE_NODE;
         goto er_node; /* узел испорчен           */
      }
   tryCatch();
      rt = oststE_ADDRESS;
   tryEnd();

   intUnlock( lvl );

   if( rt )
      goto er_node;

   return 0;

er_node:
   parm->test = oststNODE;
   if( pBuf ) {
      pBuf[0] = (u_int)pHead;
      pBuf[1] = (u_int)pDdl;
      pBuf[2] = (u_int)pDdl->next;
      pBuf[3] = (u_int)pDdl->prev;
      parm->bufsize += oststSIZE_TCBNODE;
   }
   return rt;
#else
   return 0;
#endif
}

/* Проверка принадлежности адреса адресному пространству текущего процесса */
static inline int oststIsProcData( void* adr )
{
   int ret;
#if OS_MAJOR == 3
   ret = (memcheck_s(PPE()->ppe_pid,adr,sizeof(adr),msflagsREADABLE) == 0)? 1 : 0;
#else
   tryBegin();
      ret = *(int*)adr;
      ret = 1;
   tryCatch();
      ret = 0;
   tryEnd();
#endif
   return ret;
}

/* Проверка заголовка очереди */
static inline int oststQueHead( void* buf, struct par_ostest* parm )
{
   queHEAD*   head = (queHEAD*)parm->obj1;
   void*      owner = parm->obj2;
   ddlHEAD*   ddlHead;
   void*      pInfo;    /* адрес информации, пересылаемой в буфер */
   int        rt;

#if OS_MAJOR == 3
   pInfo = head;
   if( (head->m_signature & 0xFFFFFF00) != QUE_SIGNATURE ) {
      rt = oststE_SIGNATURE;
      goto er_head;
   }
   ddlHead = &head->m_dll;
#else
   ddlHead = &((queDDL_HEAD*)head)->m_dll;
   pInfo = (char*)head + sizeof(head->m_objHdr);
#endif
   if( owner && (head->m_owner != owner) ) {
      rt = oststE_OWNER;
      goto er_head;
   }
   if( ddlHead->c_null != 0 ) {
      rt = oststE_CNULL;
      goto er_head;
   }
   return 0;
er_head:
   parm->test = oststQUE_HEAD;
   if( buf ) {
      memmove( buf, pInfo, oststSIZE_QUE_HEAD<<2 );
      parm->bufsize += oststSIZE_QUE_HEAD;
   }
   return rt;
}

/* Проверка связности очереди DDL */
static inline int oststQueList( void* buf, struct par_ostest* parm )
{
   queHEAD*    que_head = (queHEAD*)parm->obj1;
   u_int*      pBuf = (u_int*)buf;
   ddlNODE*    nNode = 0;
   ddlNODE*    pNode = 0;
   ddlHEAD*    ddlHead;
   int         lvl;
   int         i;
   int         maxi;
   int         rt;

   maxi = (parm->restriction)? *(int*)parm->restriction : oststMAX_CYCLE;
#if OS_MAJOR == 3
   ddlHead = &que_head->m_dll;
   lvl = intLock();
#else
   ddlHead = &((queDDL_HEAD*)que_head)->m_dll;
   lvl = intLock();
   if( queIS_LOCKED(que_head) ) {
      rt = oststE_ACCESS;
      goto finish;
   }
#endif
   tryBegin();
      pNode = (ddlNODE*)ddlHead;
      nNode = ddlHead->first;
      for( i=0,rt=0; !rt && (i < maxi); i++ ) {
         if( nNode->prev != pNode ) {
            rt = oststE_LIST;
            break;
         }
         pNode = nNode;
         nNode = nNode->next;
         if( nNode == NULL )
            break;
      }
   tryCatch();
      rt = oststE_ADDRESS;
   tryEnd();

#if OS_MAJOR == 2
finish:
#endif
   intUnlock( lvl );
   if( !rt )
      return 0;

   if( buf ) {
      /* Всего 9 слов */
      pBuf[0] = (u_int)que_head;
      pBuf[1] = (u_int)pNode;
      if( pNode == NULL ) {
         memset( &pBuf[2], 0, sizeof(queNODE) );
      }
      else
         memmove( &pBuf[2], pNode, sizeof(queNODE) ); /* 3 */
      pBuf[5] = (u_int)nNode;
      if( rt != oststE_ADDRESS )
         memmove( &pBuf[6], nNode, sizeof(queNODE) ); /* 3 */
      else
         memset(&pBuf[6], 0, sizeof(queNODE) );
      parm->bufsize += oststSIZE_QUE_LIST;
   }
   return rt;
}

/* Проверка связности очереди SDL */
static inline int oststSdlList( void* buf, struct par_ostest* parm )
{
   sdlHEAD*       sdlHead = (sdlHEAD*)parm->obj1;
   int            n = (int)parm->obj2;
   u_int*         pBuf = (u_int*)buf;
   sdlNODE*       nNode = NULL;
   sdlNODE*       pNode = NULL;
   sdlNODE*       pNodeLast = NULL;
   int            lvl;
   int            i;
   int            rt;

   lvl = intLock();
   tryBegin();
      pNodeLast = (sdlNODE*)sdlHead->last;
      for( i=0,rt=0; i < n + 1; i++ ) {
         if( pNode ) {
            nNode = pNode->next;
         } else {
            nNode = (sdlNODE*)sdlHead->first;
         }
         if( nNode == NULL )
            break;
         pNode = nNode;
      }
   tryCatch();
      rt = oststE_ADDRESS;
   tryEnd();
   intUnlock( lvl );
   if( rt )
      goto er_list;
   if( pNode != pNodeLast ) {
      rt = oststE_LIST;
      goto er_list;
   }
   return 0;

er_list:
   parm->test = oststSDL_LIST;
   if( buf ) {
      /* Всего 8 слов */
      pBuf[0] = (u_int)sdlHead;
      pBuf[1] = (u_int)sdlHead->first;
      pBuf[2] = (u_int)sdlHead->last;
      pBuf[3] = (u_int)pNode;
      pBuf[4] = (u_int)nNode;
      parm->bufsize += oststSIZE_SDL_LIST;
   }
   return rt;
}

/* -------------- oststGetPpe ---------------
 * Функция oststGetPpe - получить адрес описателя системных тестов
 */
static inline struct ss_ostest* oststGetPpe( void )
{
#if OS_MAJOR == 3
   struct modlink_page* mlp = modLinkPage;
   struct ss_ostest*    pPpeOstest = (struct ss_ostest*)mlp->mlp_Ostest.t[epid];
#else
   struct ss_ostest*    pPpeOstest = (struct ss_ostest*)pOstestDescription->t[0];
#endif

   return pPpeOstest;
}

/* -------------- oststGetPpeId ----------
 *  Функция oststGetPpeId - получить адрес описателя указанного теста.
 * Аргументы:
 *   id  - идентификатор теста.
 */
static inline struct s_ostest*  oststGetPpeId( enum id_ostst id )
{
   struct ss_ostest* p = oststGetPpe();
   if( !p ) return NULL;   /* подсистема не инициализировалась */
   if( (id < 1) || (id >= oststLAST) )
      return NULL;
   return p->tests + id;
}

/* -------------- oststIsProcess ----------
 *  Функция oststIsProcess - проверка инициализации процессом подсистемы.
 * Аргументы:
 *   id  - идентификатор теста.
 */
static inline int  oststIsProcess( void )
{
   struct ss_ostest* p = oststGetPpe();
   return (p)? p->init : 0;
}

/* -------------- oststCallForUser ---------------
 * Функция oststCallForUser - выполнение системного теста, способ тестирования
 *                            которого задан функцией пользователя oststUserFunction
 * Аргументы:
 *   p      - адрес описателя теста,
 */
static __inline__ void oststCallForUser ( struct s_ostest* p )
{
   oststFunction( p, (void*)NULL, (void*)NULL );
}

/* -------------- oststCallForObjects ---------------
 * Функция oststCallForObjects - выполнение системного теста, объекты тестирования
 *                               которого заданы списком
 * Аргументы:
 *   p      - адрес описателя теста,
 */
static __inline__ void oststCallForObjects ( struct s_ostest* p )
{
   int   i;
   for( i=0; i < p->objCount; i++ )
      if( p->objects[i].object )
         oststFunction( p, (void*)p->objects[i].objId, (void*)p->objects[i].object );
}

/* -------------- oststCallForClass ---------------
 * Функция oststCallForClass - выполнение системного теста, для всех объектов
 *                             заданного класса тестировая
 * Аргументы:
 *   p      - адрес описателя теста,
 */
static __inline__ void oststCallForClass ( struct s_ostest* p )
{
   objCONTROL* pClass = p->pClass;  /* адрес описателя класса */
   void*       pObj;
   int         rt;
#if OS_MAJOR == 3
   objINDEX*   pIndex = pClass->m_index;  /* адрес массива индексов класса */
   int         i;

   for( i=0; i < pClass->m_total; i++,pIndex++ ) {
     if( objID_ACTIVE( pIndex->m_Id ) ) {
       /* Объект активен */
       pObj = (void*)objIDX_TO_OBJECT( pIndex );
       if( pClass->m_Type == CLASS_MQCONTROL ) {
          if( oststObjectActive(objTO_HEADER(pObj)) )
             continue;  /* объект не завершен или флаги его испорчены */
          pObj = (void*)mqMQD_TO_MQ( (mqdCONTROL*)pObj );
       }
       rt = oststFunction( p, (void*)pIndex->m_Id, pObj );
     }
   }
#else
   if ( pClass->m_flags & ctrlACCOUNTING ) {
      sdlNODE* pNode;
//       kernDISPATCH_DISABLE();
       for (pNode = sdlFirst(&pClass->m_objList); pNode; pNode = sdlNext(pNode))
       {
          pObj = (void*)objNODE_TO_HDR( pNode );
          if( pClass == mqpObjControlId )
             rt = oststFunction( p, 0, pObj );   /* очередь сообщений */
          else
             rt = oststFunction( p, pObj, pObj );
       }
//       kernDISPATCH_ENABLE();
   }
#endif
}

#endif /* KERNEL */
#endif /* MAX_PERF */
#endif   /* _PRIVATE_OSTESTINL_H */

