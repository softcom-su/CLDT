/* hosts.h - описание структур и функций для списка хостов */

#ifndef TAB_HOSTS_H
#define TAB_HOSTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <private/sdlist.h>
#include <netinet/in.h>
#include <netdb.h>

/*  Умолчательные значения параметров для функции hostTableInit:  */
#ifndef HOSTABLE_SIZE
#define HOSTABLE_SIZE  40 /* максимальное число узлов в таблице   */
#endif/*HOSTABLE_SIZE*/

#ifndef HOSTABLE_ALIAS
#define HOSTABLE_ALIAS 10 /* максимальное число алиасов у одного узла */
#endif/*HOSTABLE_ALIAS*/

#ifndef HOSTABLE_ADDR
#define HOSTABLE_ADDR   1 /* максимальное число адресов у одного узла */
#endif/*HOSTABLE_ADDR*/


#ifndef HOSTABLE_ADDR_SIZE
#define HOSTABLE_ADDR_SIZE  4 /* максимальная длина адреса */
#endif/*HOSTABLE_ADDR_SIZE */

/* Функции работы с таблицей хостов: */
int hostTableInit(int nhost, int nalias, int naddr, int namelen, int addrlen) ;

int 	hostAdd (char *name, char *addr) ; /* добавить к таблице хостов */
int 	hostRemove (char *name) ;          /* удалить из таблицы хостов */
void hostShow (void) ;                  /* вывести список хостов */
/* см. также gethostbyname & gethostbyaddr - вфайле net/netdb.h */

int 	sethostname (char *machname);
unsigned long hostGetByName (char *name) ;

#ifdef __cplusplus
}
#endif

#endif /* TAB_HOSTS_H */
