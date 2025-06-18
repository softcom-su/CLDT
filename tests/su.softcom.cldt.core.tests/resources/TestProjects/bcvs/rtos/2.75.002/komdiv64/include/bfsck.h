#ifndef __FSCKDISP_H__
#define __FSCKDISP_H__
#include <string.h>

#include <sys/vnode.h>

#define BFSC_INTERACTIVE		0x01   /* запрашивать пользов. перед исправл. ошибки       */
#define BFSC_ALW_YES			0x02   /* всегда исправлять обнаруженные ошибки            */
#define BFSC_ALW_NO				0x04   /* не исправлять обнаруженные ошибки                */
#define BFSC_AM_MASK			0x07

#define BFSC_OUT_SILENT			0x08   /* вывод отсутствует                                */
#define BFSC_OUT_NORMAL			0x10   /* выводится краткая информация в конце работы      */
#define BFSC_OUT_VERBOSE		0x20   /* выводится подробная информация о работе прог.    */
#define BFSC_OUT_MASK			0x38 

#define BFSC_SAVE_LOST_CHAINS	0x40   /* потерянные цепочки сохраняются в каталоге BAD    */
#define BFSC_SAVE_CROSS_LINKS	0x80   /* цепочки, содержащие кросс-линки, сохраняются 
                                                                         в каталоге BAD    */
#define BFSC_SAVE_BAD_DIRS		0x100  /* некорректные каталоги сохраняется в каталоге BAD */
#define BFSC_SYNC_FATS  		0x200  /* копировать FAT0 в FAT1                        */
#define BFSC_CHECK_LENGTH		0x400  /* проверять длину файла в элементе каталога        */

int bfscStart(struct vnode *dev);
int bfsck(struct vnode *dev, int flags);
int bfscInitFlags(int flags);
int bfscGetFlags();

#endif

