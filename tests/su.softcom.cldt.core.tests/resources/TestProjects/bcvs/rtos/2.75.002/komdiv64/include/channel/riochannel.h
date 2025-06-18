/* channel/riochannel.h
 */

#ifndef _CHANNEL_RIOCHANNEL_H
#define _CHANNEL_RIOCHANNEL_H

#ifdef KERNEL

#define  rioMAX_MESSAGESIZE      4096            /* максимальный размер сообщения      */
/* Для передачи посредством RIO необходимо выравнивание на 256
 * границы и размера буфера
 */
#define riochanBUFFER_ALIGN      256             /* выравнивание буфера                */
#define riochanBUFFERSIZE_ALIGN  256             /* выравнивание размера буфера        */

#define riochanINIT              0               /* драйвер ППМ инициализирован        */

/* Формат извещения (служебного сообщения) о числе свободных буферов                   */
typedef
struct {
   int             rcmb_nbuf;                         /* число буферов                      */
} riochanMSG_BUFFERS;

#define RIOMEM_MPCHANNEL SHMEM_OBJECTS           /* тип запрашиваемой в MP памяти      */

/*------ mpchanRioBufnodeToPseudo - опрос описателя (межмодульной) логической связи -----
 *                                  по описателю буфера и номеру процессора
 * Аргументы:
 *   cpu      - номер модуля приемник сообщения;
 *   pBufNode - адрес описателя узла буфера.
 */
static __inline__ mpchanPSEUDO* mpchanRioBufnodeToPseudo( int cpu, chanBUFNODE* pBufNode )
{
   chanDESCRIPTION* pChannel  = pBufNode->cb_pChannel;        /* описатель канала         */
   chanPORT*        pChanPort = chanTO_PORTSRC( pChannel );/*описатель порта-оправителя*/
   mpchanPSEUDO *   pPseudo;                               /*описатель логической связи*/
   int              i;

   if( chanIS_PORT_STD(pChanPort) ) {
       /* Порт-отправитель принадлежит стандартной партиции - модуль-отправитель       */

       /* Цикл по всем порт-получателям канала */
       for( i=0; i < pChannel->dc_cntDest; i++ )
       {
         pChanPort = chanTO_PORTDSTJ( pChannel, i );       /* порт-получатель номер i  */
         pPseudo = (mpchanPSEUDO *)chanPORT_MPPORT( pChanPort );/* описат. логич. связи*/
         if( pPseudo && (pPseudo->mpps_cpuPartner == cpu) )
            return  pPseudo;
       }
       assert( 0 );                    /* ошибка порт-получатель с номер i не найден   */
       return NULL;
   } else {
       /* Порт-отправитель принадлежит псевдопартиции - модуль-получатель              */
       pPseudo = (mpchanPSEUDO *)chanPORT_MPPORT( pChanPort ); /* описат. логич. связи */
       assert( pPseudo );              /* ошибка, если описателя нет                   */
       return pPseudo;
   }
}

extern int mpchanRioTableInit ( struct chan_options* pChanOptions );
/*
extern int  boardRioInit( int nBuf, int size, void(*)(int,chanBUFNODE*,int), void(*)(void*) );
extern int  boardRioSendMsg( int cpu, chanBUFNODE* pBufNode, int size );
extern void boardRioFreeBuf( void* pBuf );
*/
#endif /*KERNEL*/

#endif /* _CHANNEL_RIOCHANNEL_H */
