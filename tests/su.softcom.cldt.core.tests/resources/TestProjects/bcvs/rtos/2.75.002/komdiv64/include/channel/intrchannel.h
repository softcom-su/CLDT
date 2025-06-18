/* channel/intrchannel.h
 */

#ifndef _CHANNEL_INTRCHANNEL_H
#define _CHANNEL_INTRCHANNEL_H

#ifdef KERNEL

/* Помещение SAMPLING сообщения в канал */
int intrchanSportWrite( chanPORT* pChanPort, u_char *msg_ptr, long msg_len );
/* Помещение QUEUING сообщения в канал */
int intrchanQportSend ( chanPORT* pChanPort, u_char *msg_ptr, long msg_len );

/* Получить адрес описателя порта по имени псевдопартиции */
chanPORT* intrchanGetPort ( char* name );

#ifdef chanDEBUG_INTERRUPT
extern chanPORT* pChanPortIntrSrc;
extern chanPORT* pChanSPortIntrSrc;

extern int intrchanDebugPutMsg( chanPORT* pChanPortIntrSrc );
#endif

#endif /*KERNEL*/

#endif /* _CHANNEL_INTRCHANNEL_H */
