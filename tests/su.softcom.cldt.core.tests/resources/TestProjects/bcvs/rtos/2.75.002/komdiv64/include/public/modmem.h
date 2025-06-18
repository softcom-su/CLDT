#ifndef _PUBLIC_MODMEM_H
#define _PUBLIC_MODMEM_H
#include <sys/types.h>
#include <arch/cpu.h>
#if OS_MAJOR == 2
#define curpid 0
#define __usleep usleep
typedef     unsigned long long paddr_t;
#define cpuV2P(pid,vaddr) K0_TO_PHYS(vaddr)
#endif
typedef enum {
   shmemSYSDATA = 0,                             /* системная общая память                   */
   shmemCACHED_SYSDATA = 0,                      /* системная общая память                   */
   shmemUNCACHED_SYSDATA,                        /* системная некешированная общая память    */
   shmemUSERDATA,                                /* пользовательская общая память            */
   shmemPROTECTED_SYSDATA,                       /* защищенные данные модуля                 */
   shmemUNCACHED_USERDATA,
   shmemARINC,
} shmemTYPE;
void   *modMPDSharedAlloc(shmemTYPE type, size_t size, int align);
#if OS_MAJOR >= 3
void    *modMemAlloc(shmemTYPE type, size_t size, int align);
void    modMemFree(shmemTYPE type, void *ptr);
#endif
#endif /* ! _PUBLIC_MODMEM_H */
