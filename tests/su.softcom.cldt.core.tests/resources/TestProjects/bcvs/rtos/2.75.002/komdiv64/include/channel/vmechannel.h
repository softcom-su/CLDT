/* channel/vmechannel.h
 */

#ifndef _CHANNEL_VMECHANNEL_H
#define _CHANNEL_VMECHANNEL_H

#ifdef KERNEL

#define vmechanBUFFER_ALIGN      256    /* выравнивание буфера                          */
#define vmechanBUFFERSIZE_ALIGN  256    /* выравнивание размера буфера                  */

/* Обслуживание буферов общей памяти */
/* Состояния буферов */
typedef enum {
   mpchanBUF_DISC,      /* буфер не используется                 */
   mpchanBUF_IDLE,      /* буфер передан партнеру                */
   mpchanBUF_READY,     /* буфер готов для использования         */
   mpchanBUF_WRITE,     /* отправитель заносит сообщ. в буфер    */
   mpchanBUF_NAK        /* буфер не должен передаваться партнеру */
} mpchanSTATE_BUF;

#define mpchanVmeInterruptModule(pMod)    \
  {                                       \
    ++(pMod)->mpm_IntrSend;               \
    boardMPIrqIntr((pMod)->mpm_cpu);      \
  }

#define SHMEM_MPCHANNEL   SHMEM_OBJECTS          /* тип запрашиваемой в MP памяти      */

#define mpchanVmeInterruptConnect(rtn)   boardMPIrqConnect(rtn)
#define mpchanVmeInterruptEnable()       boardMPIrqEnable()

int mpchanVmeTableInit ( struct chan_options* pChanOptions );

#endif /*KERNEL*/

#endif /* _CHANNEL_VMECHANNEL_H */
