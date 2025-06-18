/* mpsem.h*/

#ifndef _MPSEM_H
#define _MPSEM_H

#ifdef KERNEL

#include <mp/mpddlist.h>
#include <mp/mpmem.h>
#include <mp/mpque.h>

/* Описатель семафора в общей памяти */
typedef struct mpSemControl
{
  objHEADER     m_objCore;
  int           m_stype;
  volatile
  int           m_count;
  sem_t         m_desc;
  mpQUEHEAD     m_queHead;
} mpSEMCONTROL;

typedef  mpSEMCONTROL  *mpSEMCONTROL_ID;

/* от sem_t к структуре управления */
#define mpobjTO_SEMCONTROL(objId) \
   (struct semPxControl*)mpAdrLocal((mpoffset_t)(objId)->ma_sem)
/* семафор именованный? */
#define mpSEM_IS_NAMED(sem)      \
   ((u_int)(sem)->m_named == htonl((u_long)SEM_TY_POSIX_NAMED))

extern int (*mpSemTableRtn[])();          /* таблица функций semmp            */

extern mpOBJCONTROL *mpSemControlId;      /* описатель класса                 */
extern mpOBJCONTROL *mpSemPxControlId;    /* описатель класса неименованных   */

extern mpoffset_t    mpSemControlOff;     /* смещение описателя               */
extern mpoffset_t    mpSemPxControlOff;

extern mpOBJCONTROLLOC_ID	  mpSemControlLocId;
extern mpOBJCONTROLLOC_ID	  mpSemPxControlLocId;

extern sem_t *mpSemAlloc( void );

extern int (*mpSemPxShowRtn)();
extern int (*mpSemShowRtn)();
extern int (*mpSemGetIdRtn)( char *s );

#endif /*KERNEL*/

extern int  mpSemPxInit( mpMEMPOOL_ID pool, int num );
extern void mpsem( void *sem, int list );
extern int  mpSemShowInit( void );

#endif /*_MPSEM_H*/
