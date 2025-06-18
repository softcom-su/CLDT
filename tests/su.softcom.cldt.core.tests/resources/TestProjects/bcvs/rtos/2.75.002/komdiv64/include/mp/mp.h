/* mp.h*/
#ifndef _MP_H
#define _MP_H

#include <sys/types.h>
#include <mp/mpbase.h>

#define mpIS_MASTER_CPU()  (mpSELF_CPU() == mpMASTER_CPU()) /*текущий CPU - мастер?*/
#define mpSELF_CPU()     mpPSN       /*возвращаем номер CPU*/
#define mpMASTER_CPU()   mpCPUMaster /*возвращаем номер ведущего CPU*/
/*минимальнодопустимый размер памяти, предоставляемой под объекты*/
#define MP_MEM_OBJECTS_MIN  0x2000 


/*преобразование смещения относительно начала общей памяти
в локальный адрес и обратно*/
void *mpAdrLocal( mpoffset_t offset );
mpoffset_t mpAdrOffset( void *addr );

#endif /*_MP_H*/
