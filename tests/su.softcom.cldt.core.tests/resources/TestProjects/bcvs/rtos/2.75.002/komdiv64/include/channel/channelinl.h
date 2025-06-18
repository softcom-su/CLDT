/*  channel/channelinl.h  */

#if !defined _CHANNEL_CHANNEL_INL_H && defined KERNEL
#define _CHANNEL_CHANNEL_INL_H

#include <public/difference.h>
#include <assert.h>
#include <arch/board.h> /* boardProcNumGet() */
#include <private/modmem.h>
#include <private/longcopy.h>
#include <trace.h>

/*--- chanNotifyUp - установка требование известить порт о появлении буфера/сообщения ---
 * Аргументы:
 *   pChanPort - указатель на описатель порта в канале.
 * Требование выставляется другому порту на том же модуле.
 */
static __inline void  chanNotifyUp ( chanPORT* pChanPort )
{
   pChanPort->cp_reqNotify = 1;
//   TRACE_SYS_EVENT( traceFunction56, "C_UP", 4 );
}
/*---- chanNotifyDown - сброс требования известить порт о появлении буфера/сообщения ----
 * Аргументы:
 *   pChanPort - указатель на описатель порта в канале.
 * Требование выставляется другому порту на том же модуле.
 */
static __inline void  chanNotifyDown ( chanPORT* pChanPort )
{
   pChanPort->cp_reqNotify = 0;
//   TRACE_SYS_EVENT( traceFunction56, "C_DW", 4 );
}

/*---- chanIsNotify - проверка, нужно ли известить порт о появлении буфера/сообщения ----
 * Аргументы:
 *   pChanPort - указатель на описатель порта в канале.
 *   Порт извещается, если установлено требование и извещение еще не было сделано.
 */
static __inline int  chanIsNotify ( chanPORT* pChanPort )
{
   return (chanIsReqNotifyUp(pChanPort) && !pChanPort->cp_flnotify);
}

/*--------------------- chanBufNodeInit - инициализация узла буфера ---------------------
 * Аргументы:
 *   pBufNode     - указатель на узел буфера сообщения;
 *   pDescChannel - адрес описателя канала;
 *   pHeadBuf     - адрес заголовка буфера.
 */
static __inline void  chanBufNodeInit
      ( chanBUFNODE* pBufNode, chanDESCRIPTION*  pDescChannel, chanHEADBUF* pHeadBuf )
{
   pBufNode->cb_pChannel = pDescChannel;  /* указатель на описатель канала                */
   pBufNode->cb_pHead    = pHeadBuf;      /* указатель на заголовок буфера                */
   pBufNode->cb_flNBO    = 0;             /* порядок байт в заголовке не NBO              */
   pBufNode->cb_cntRead  = 0;             /* число потоков, читающих этот буфер (SAMPLING)*/
#if __mips__
   MODCALL(pBufNode->cb_pPHead = cpuV2P(curpid,pHeadBuf)); /* физический адрес данных     */
#endif
}
/*------------------- chanHdrBufInit - инициализация заголовка буфера -------------------
 * Функция chanHdrBufInit - инициализация заголовка буфера
 * Аргументы:
 *   pHeadBuf - адрес заголовка буфера;
 *   pChannel - адрес описателя канала.
 */
#if 0
static __inline void  chanHdrBufInit
      ( chanHEADBUF* pHeadBuf, chanDESCRIPTION*  pChannel )
{
   pHeadBuf->cpu = boardProcNumGet();            /* номер данного модуля в крейте      */
   pHeadBuf->connect = pChannel->dc_indPsd;      /* номер логической связи             */
}
#endif
/*--------------- chanHdrBufNBOInit - инициализация заголовка буфера (NBO) --------------
 * Аргументы:
 *   pHdrBuf  - адрес заголовка буфера;
 *   pChannel - адрес описателя канала.
 */
static __inline void  chanHdrBufNBOInit
      ( chanHEADBUF* pHdrBuf, chanDESCRIPTION*  pChannel )
{
   pHdrBuf->chb_cpu      = htonl( boardProcNumGet() );  /* номер данного модуля в крейте  */
   pHdrBuf->chb_connect  = htonl( pChannel->dc_indPsd );/* номер логической связи         */
   pHdrBuf->chb_nMsg     = 0;                           /* номер принятого сообщения      */
   pHdrBuf->chb_overflow = 0;                           /* флаг переполнения              */
}

/*------------------ chanMemAllocCached - получение кэшированное памяти -----------------
 * Аргументы:
 *   size    -  объем памяти;
 *   align   -  выравнивание;
 *   res     -  указатель на переменную, куда будет помещен указатель
 *              на выделенный участок памяти;
 *   verbose -  выводить трассу распределения памяти.
 * Возвращаемые значения.
 *   В случае успешного завершения возвращается 0, в противном случае - код ошибки:
 *   ENOMEM   - недостаточно памяти.
 *
 * Память выделяется из области общей системной памяти модуля.
 * Полученный указатель обеспечивает доступ, к полученному участку памяти, минуя кэш.
 */
static inline int chanMemAllocCached( int size, int align, anyptr_t res, int verbose )
{
   char*  p;
   p = (char*)modMPDSharedAlloc( shmemCACHED_SYSDATA, size, align ); /* не чистится */
   if( p == NULL ) {
      /* Нет нужного объема памяти             */
      printf( "modChanMemAlloc(not enough cached memory):%d error\n", size );
      return ENOMEM;
   }
   if( verbose ) {
     printf( "(modChanMemAlloc(request): size=%d align=%d)\n", size, align );
   }
   *res.c = p;
   return 0;
}
/*---- chanMemAllocUncached - выделение некешированной памяти в сегменте общих данных ---
 * Аргументы:
 *   size    - объем памяти;
 *   align   - выравнивание;
 *   res     - указатель на переменную, куда будет помещен указатель
 *             на выделенный участок памяти;
 *   verbose - уровень сообщений об ошибках (0 - не печатать сообщения об ошибке).
 * Возвращаемые значения.
 *   В случае успешного завершения возвращается 0, в противном случае - код ошибки:
 *   ENOMEM   - недостаточно памяти.
 */
static inline int chanMemAllocUncached( int size, int align, anyptr_t res, int verbose )
{
   char*  p;
   p = (char*)modMPDSharedAlloc( shmemUNCACHED_SYSDATA, size, align ); /* не чистится */
   if( p == NULL  ) {
       /* Нет нужного объема памяти             */
       if( verbose )
          printf( "modChanMemAlloc(not enough uncached memory):%d error\n", size );
       return ENOMEM;
   }
   if( verbose ) {
     printf( "(modChanMemAlloc(request): size=%d align=%d)\n", size, align );
   }
   *res.c = p;
   return 0;
}

/*------------- chanSetNotification - регистрация функции уведомления порта -------------
 * Аргументы:
 *   pChanPort - указатель на описатель порта (в канале),
 *               для которого определяется функция уведомления;
 *   f         - функция обслуживания;
 *   arg1      - 1-ый аргумент функции уведомления;
 *   arg2      - 2-ой аргумент функции уведомления.
 */
static __inline void chanSetNotification
    ( chanPORT* pChanPort, void (*f)(void*,void*), void* arg1, void* arg2 )
{
   pChanPort->cp_notify.fun  = f;
   pChanPort->cp_notify.arg1 = arg1;
   pChanPort->cp_notify.arg2 = arg2;
}

/*------------- chanInsertMessage - вернуть в канал сообщение -------------
 * Аргументы:
 *   pChannel - указатель на описатель канала;
 *   pBufNode - указатель на буферный узел;
 *   mode     - вставлять в начало/конец списка.
 *   Прерывания должны быть запрещены перед вызовом функции.
 */

static __inline void chanInsertMessage
         ( chanDESCRIPTION* pChannel, chanBUFNODE* pBufNode, int mode )
{
   union list_head*  list = chanMSGLIST( pChannel );
   sdlHEAD*          sdlHead = &list->shead;
   sdlNODE*          sdlNode = (sdlNODE*)chanBUFNODE_TO_NODE( pBufNode );

   assert( chanIS_MODE_QUEUING(pChannel) || chanIS_MODE_SAP(pChannel) );

   if( mode )
      sdlAppend( sdlHead, sdlNode );
   else
      sdlInsertFirst( sdlHead, sdlNode );
   ++pChannel->dc_curmsgs;          /* число сообщений в очереди канала             */
   assert(pChannel->dc_curmsgs>0 && pChannel->dc_curmsgs <= pChannel->dc_maxmsg);
   // sdlCheck(sdlHead,pChannel->dc_curmsgs);
}

/*---------- KernelConfig - получение указателя на описатель главного процесса ---------
 */
static __inline struct PartitionType*  KernelConfig( void )
{
#if OS_MAJOR == 2
   extern struct PartitionType chanKernelConfig;
   return &chanKernelConfig;
#else
   return  modLinkPage->mlp_KernelConfig;
#endif
}


#endif /* _CHANNEL_CHANNEL_INL_H */
