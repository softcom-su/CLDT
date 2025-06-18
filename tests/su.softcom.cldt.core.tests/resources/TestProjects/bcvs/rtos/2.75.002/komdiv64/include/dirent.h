/* dirent.h - структуры и функции POSIX для работы с каталогами */

#ifndef _DIRENT_H
#define _DIRENT_H

#include <limits.h>
#include <sys/types.h>
#include <sys/dirent.h>

typedef void *    DIR;


#ifdef __cplusplus
extern "C" {
#endif

/* Описания функций работы с каталогами: */

/* Открывает каталог, заданный аргументом dirname. Возвращает */
/* указатель на объект типа DIR. В случае неудачи возвращает  */
/* NULL и errno присваивает код ошибки.                       */
DIR * opendir (const char * dirname) ;

/* Возвращает указатель на текущий элемент каталога */
struct dirent * readdir (DIR * dirp) ;

int readdir_r (DIR * dirp, struct dirent * entry, struct dirent ** result) ;

/* Позиционирует каталог, заданный элементом , на начало. */
void rewinddir (DIR * dirp) ;

/* Закрывает каталог, заданный элементом. В случае успешного */
/* выполнения возвращает 0. В случае ошибки возвращает -1 и  */
/* присваивает код ошибки.                                   */
int closedir (DIR * dirp) ;

/* Если указатель dirp не ссылается на открытый в данный момент */
/* каталог, то результат выполнения ниже представленных функций */
/* не определен.                                                */


long telldir(DIR *dirp);
void seekdir(DIR *dirp, long loc);

#ifdef __cplusplus
}
#endif

#endif /* _DIRENT_H */
