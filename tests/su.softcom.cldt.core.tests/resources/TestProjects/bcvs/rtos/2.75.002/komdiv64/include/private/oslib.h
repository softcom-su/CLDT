#ifndef	_OSLIB_H
#define	_OSLIB_H
#include <sys/types.h>

void os2000Logo (void) ;

/* getPSN - функция возвращает номер процессора:*/
int getPSN(void) ;
/* getHostAddr - функция возвращает адрес инструментальной машины:*/
char *getHostAddr(void) ; 
/* getBootFile - функция возвращает имя загрузочного файла:*/
char *getBootFile(void) ;
/* getUserName - функция возвращает имя пользователя целевой машины:*/
char *getUserName(void) ;
/* getUserPassWd - функция возвращает пароль пользователя целевой машины:*/
char *getUserPassWd(void) ;
/* getHostName - функция возвращает имя инструментальной машины:*/
char *getHostName(void) ; 
/* getLANAddress - функция IP адрес интерфейсалокальной сети:      */
char *getLANAddress(void);
/* getDefaultGateway() - функция GW по умолчанию                 */
char *getDefaultGateway(void);
/* getSMAddress - функция IP адрес интерфейса сети на общей памяти:*/
char *getSMAddress(void);

int osVersion (void);
int osVersionMajor (void);
int osVersionMinor (void);
int osVersionRelease (void);

#endif /* _OSLIB_H */
