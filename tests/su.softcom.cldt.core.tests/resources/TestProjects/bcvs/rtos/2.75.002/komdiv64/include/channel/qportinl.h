/*  channel/qportinl.h  */

#if !defined _CHANNEL_QPORT_INL_H && defined KERNEL
#define _CHANNEL_QPORT_INL_H

#include  <channel/channel.h>

/*----------- qportSetOverflow --------------
 * Функция qportSetOverflow - установка флага переполнения в порте-приемнике.
 * Аргумент:
 *   pChanPort  - указатель на описатель порта в канале.
 */
static __inline void __UNUSED_F qportSetOverflow( chanPORT* pChanPort )
{
  struct desc_channel*  pDescChannel = chanPORT_TO_CHANNEL( pChanPort );
  chanSET_OVERFLOW( pDescChannel );
}

/*----------- qportReadMessage ------------
 * Функция qportReadMessage - копирование сообщения из буфера канала в буфер потока
 * Аргументы:
 *   pBufNode- указатель на узел буфера сообщения;
 *   buf_ptr - адрес буфера для приема сообщения;
 *   len     - указатель на переменную, куда будет помещена длина принятого сообщения.
 */
static __inline void __UNUSED_F qportReadMessage
                           ( chanBUFNODE* pBufNode, void* buf_ptr, MESSAGE_SIZE_TYPE* len )
{
   chanHEADBUF* pHeadBuf = chanBUFNODE_TO_HEADBUF( pBufNode );
   char *b = buf_ptr;
   *len =  pBufNode->cb_size;                                                       /* длина сообщения              */
   memmove( b, chanHEADBUF_TO_DATA(pHeadBuf), pBufNode->cb_parts.copysize );    /* копиров. сообщения из буфера */
   b+= pBufNode->cb_parts.copysize;
   while( (pBufNode = pBufNode->cb_parts.next) )
   {
      memmove( b, chanBUFNODE_TO_HEADBUF( pBufNode ), pBufNode->cb_parts.copysize ); /* копиров. сообщения из буфера */
      b+= pBufNode->cb_parts.copysize;
   }
}


#endif /* _CHANNEL_QPORT_INL_H */
