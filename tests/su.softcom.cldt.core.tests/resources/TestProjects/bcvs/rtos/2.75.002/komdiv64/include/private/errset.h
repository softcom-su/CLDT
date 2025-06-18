#ifndef _ERRSET_H
#define _ERRSET_H

#include  <assert.h>
#include  <errno.h>
extern int errno;
#define errGET( ) errno
#define errSET( err ) { assert(err >= 0); errno =  err; }
#define errSET_AND_RETURN( err ) { errSET(err); return -1; }
#define errSET_AND_RETNULL( err ) { errSET(err); return NULL; }
/*
enum {
	M_OBJCORE = 1,
	M_MEMPOOL = 2
};
*/
/*
enum {
	E_OBJCORE_NO_METHOD = 1
};
*/
/*
enum {
	E_MEMPOOL_NOT_ENOUGH_MEMORY = 1,
	E_MEMPOOL_WRONG_BLOCK = 2
};
*/


/*размер таблицы указателей на сообщения*/
#define MSG_TABLE_SIZE               ELAST+1  
#define MSG_TABLE_LANGUAGE_ENGLISH   1
#define MSG_TABLE_SHORT              2 /*короткие сообщения*/
#define MSG_TABLE_LONG               4 /*длинные*/

#define E2BIG_MSG_SHORT     "E2BIG"
#define E2BIG_MSG           "Arg list too long"
#define E2BIG_MSG_RU        "Список аргументов слишком длинен"
#define EACCES_MSG_SHORT    "EACCES"
#define EACCES_MSG          "Permission denied"
#define EACCES_MSG_RU       "Не хватает прав доступа"


#define	EADDRINUSE_MSG_SHORT    "EADDRINUSE" 
#define	EADDRINUSE_MSG		      "Address in use"
#define	EADDRINUSE_MSG_RU	      "Адрес используется"
		
#define	EADDRNOTAVAIL_MSG_SHORT	"EADDRNOTAVAIL"
#define	EADDRNOTAVAIL_MSG	      "Address not available"
#define	EADDRNOTAVAIL_MSG_RU    "Адрес недоступен"

#define	EAFNOSUPPORT_MSG_SHORT	"EAFNOSUPPORT"
#define	EAFNOSUPPORT_MSG	      "Address family not supported"
#define	EAFNOSUPPORT_MSG_RU	    "Семейство адресов не поддерживается"


#define EAGAIN_MSG_SHORT    "EAGAIN"
#define EAGAIN_MSG          "Resource temporarily unavailable"
#define EAGAIN_MSG_RU       "Ресурс временно недоступен"

#define EALREADY_MSG        "Connection already in progress"
#define EALREADY_MSG_RU     "Соединение уже установлено"
#define EALREADY_MSG_SHORT  "EALREADY"

#define EBADF_MSG_SHORT     "EBADF"
#define EBADF_MSG           "Bad file descriptor"
#define EBADF_MSG_RU        "Некорректный дескриптор файла"
#define EBADMSG_MSG_SHORT   "EBADMSG"
#define EBADMSG_MSG         "Bad message"
#define EBADMSG_MSG_RU      "Сообщение искажено"
#define EBUSY_MSG_SHORT     "EBUSY"
#define EBUSY_MSG           "Resource busy"
#define EBUSY_MSG_RU        "Ресурс занят"
#define ECANCELED_MSG_SHORT "ECANCELED"
#define ECANCELED_MSG       "Operation canceled"
#define ECANCELED_MSG_RU    "Операция аннулирована"
#define ECHILD_MSG_SHORT    "ECHILD"
#define ECHILD_MSG          "No child processes"
#define ECHILD_MSG_RU       "Дочерний процесс отсутствует"
#define ECONNABORTED_MSG    "Connection aborted"
#define ECONNABORTED_MSG_RU	"Соединение разорвано"
#define ECONNABORTED_MSG_SHORT "ECONNABORTED"
#define ECONNREFUSED_MSG    "Connection refused"
#define ECONNREFUSED_MSG_RU "Соединение отвергнуто"
#define ECONNREFUSED_MSG_SHORT "ECONNREFUSED"
#define ECONNRESET_MSG      "Connection reset"
#define ECONNRESET_MSG_RU	  "Соединение сброшено"
#define ECONNRESET_MSG_SHORT "ECONNRESET"
#define EDEADLK_MSG_SHORT   "EDEADLK"
#define EDEADLK_MSG         "Resource deadlock avoided"
#define EDEADLK_MSG_RU      "Уход от клинча"
#define EDESTADDRREQ_MSG    "Destination address required"
#define EDESTADDRREQ_MSG_RU	"Необходим адрес"
#define EDESTADDRREQ_MSG_SHORT "EDESTADDRREQ"
#define EDOM_MSG_SHORT      "EDOM"
#define EDOM_MSG            "Domain error"
#define EDOM_MSG_RU         "Ошибка области определения"
#define EDQUOT_MSG			    "Reserverd"
#define EDQUOT_MSG_RU		    "Зарезервировано"
#define EDQUOT_MSG_SHORT	  "EDQUOT"
#define EEXIST_MSG_SHORT    "EEXIST"
#define EEXIST_MSG          "File exists"
#define EEXIST_MSG_RU       "Файл уже существует"
#define EFAULT_MSG_SHORT    "EFAULT"
#define EFAULT_MSG          "Bad address"
#define EFAULT_MSG_RU       "Некорректный адрес"
#define EFBIG_MSG_SHORT     "EFBIG"
#define EFBIG_MSG           "File too large"
#define EFBIG_MSG_RU        "Файл слишком велик"
#define EHOSTUNREACH_MSG	  "Host is unreachable"
#define EHOSTUNREACH_MSG_RU	"Хост недостижим"
#define EHOSTUNREACH_MSG_SHORT	"EHOSTUNREACH"
#define EIDRM_MSG			      "Identifier removed"
#define EIDRM_MSG_RU		    "Идентификатор удален"
#define EIDRM_MSG_SHORT		  "EIDRM"
#define EILSEQ_MSG			    "Illegal byte sequence"
#define EILSEQ_MSG_RU		    "Неверный порядок байт"
#define EILSEQ_MSG_SHORT	  "EILSEQ"
#define EINPROGRESS_MSG_SHORT  "EINPROGRESS"
#define EINPROGRESS_MSG     "Operation now in progress"
#define EINPROGRESS_MSG_RU  "Операция в действии"
#define EINTR_MSG_SHORT     "EINTR"
#define EINTR_MSG           "Interrupted function call"
#define EINTR_MSG_RU        "Исполнение вызванной функции прервано"
#define EINVAL_MSG_SHORT    "EINVAL"
#define EINVAL_MSG          "Invalid argument"
#define EINVAL_MSG_RU       "Неверный аргумент"
#define EIO_MSG_SHORT       "EIO"
#define EIO_MSG             "Input/output error"
#define EIO_MSG_RU          "Ошибка ввода/вывода"
#define EISCONN_MSG			    "Socket is connected"
#define EISCONN_MSG_RU		  "Сокет уже соединен"
#define EISCONN_MSG_SHORT	  "EISCONN"
#define EISDIR_MSG_SHORT    "EISDIR"
#define EISDIR_MSG          "Is a directory"
#define EISDIR_MSG_RU       "Это каталог"
#define ELOOP_MSG			      "Too many levels of symbolic links"
#define ELOOP_MSG_RU		    "Слишком много уровней символьной ссылки"
#define ELOOP_MSG_SHORT     "ELOOP"
#define EMFILE_MSG_SHORT    "EMFILE"
#define EMFILE_MSG          "Too many open files"
#define EMFILE_MSG_RU       "Слишком много открытых файлов"
#define EMLINK_MSG_SHORT    "EMLINK"
#define EMLINK_MSG          "Too many links"
#define EMLINK_MSG_RU       "Слишком много ссылок"
#define EMSGSIZE_MSG_SHORT  "EMSGSIZE"
#define EMSGSIZE_MSG        "Inappropriate message buffer length"
#define EMSGSIZE_MSG_RU     "Неподходящая длина буфера сообщений"
#define EMULTIHOP_MSG		    "Reserverd"
#define EMULTIHOP_MSG_RU	  "Зарезервировано"
#define EMULTIHOP_MSG_SHORT "EMULTIHOP"
#define ENAMETOOLONG_MSG_SHORT  "ENAMETOOLONG"
#define ENAMETOOLONG_MSG        "Filename too long"
#define ENAMETOOLONG_MSG_RU     "Слишком длинное имя файла"
#define ENETDOWN_MSG		    "Network is down"
#define ENETDOWN_MSG_RU		  "Сеть выключена"
#define ENETDOWN_MSG_SHORT  "ENETDOWN"
#define ENETUNREACH_MSG		  "Network unreachable"
#define ENETUNREACH_MSG_RU	"Сеть недоступна"
#define ENETUNREACH_MSG_SHORT "ENETUNREACH"
#define ENFILE_MSG_SHORT    "ENFILE"
#define ENFILE_MSG          "Too many open files in system"
#define ENFILE_MSG_RU       "В системе слишком много открытых файлов"
#define ENOBUFS_MSG			    "No buffer space available"
#define ENOBUFS_MSG_RU		  "Нет памяти для буфера"
#define ENOBUFS_MSG_SHORT   "ENOBUFS"
#define ENODATA_MSG			    "No message is available on the STREAM head read queue"
#define ENODATA_MSG_RU		  "Нет сообщения"
#define ENODATA_MSG_SHORT   "ENODATA"
#define ENODEV_MSG_SHORT    "ENODEV"
#define ENODEV_MSG          "No such device"
#define ENODEV_MSG_RU       "Несуществующее устройство"
#define ENOENT_MSG_SHORT    "ENOENT"
#define ENOENT_MSG          "No such file or directory"
#define ENOENT_MSG_RU       "Файл или каталог отсутствует"
#define ENOEXEC_MSG_SHORT   "ENOEXEC"
#define ENOEXEC_MSG         "Exec format error"
#define ENOEXEC_MSG_RU      "Ошибочный формат файла, указанного в качестве исполнимого"
#define ENOLCK_MSG_SHORT    "ENOLCK"
#define ENOLCK_MSG          "No lock available"
#define ENOLCK_MSG_RU       "Нет свободных фиксаторов (locks)"
#define ENOLINK_MSG			    "Reserverd"
#define ENOLINK_MSG_RU		  "Зарезервировано"
#define ENOLINK_MSG_SHORT   "ENOLINK"
#define ENOMEM_MSG_SHORT    "ENOMEM"
#define ENOMEM_MSG          "Not enough space"
#define ENOMEM_MSG_RU       "Нехватка памяти"
#define ENOMSG_MSG			    "No message of the desired type"
#define ENOMSG_MSG_RU	  	  "Нет сообщения нужного типа"
#define ENOMSG_MSG_SHORT	  "ENOMSG"
#define ENOPROTOOPT_MSG		  "Protocol not available"
#define ENOPROTOOPT_MSG_RU	"Несуществующий протокол"
#define ENOPROTOOPT_MSG_SHORT	"ENOPROTOOPT"
#define ENOSPC_MSG_SHORT    "ENOSPC"
#define ENOSPC_MSG          "No space left on device"
#define ENOSPC_MSG_RU       "На устройстве нет места"
#define ENOSR_MSG	          "No STREAM resources"
#define ENOSR_MSG_RU	      "Не хватает ресурсов"
#define ENOSR_MSG_SHORT	    "ENOSR"
#define ENOSTR_MSG          "Not a STREAM"
#define ENOSTR_MSG_RU       "Не поток"
#define ENOSTR_MSG_SHORT    "ENOSTR"
#define ENOSYS_MSG_SHORT    "ENOSYS"
#define ENOSYS_MSG          "Function not implemented"
#define ENOSYS_MSG_RU       "Функция не реализована"
#define ENOTCONN_MSG	      "The socket is not connected"
#define ENOTCONN_MSG_RU	    "Сокет не соединен"
#define ENOTCONN_MSG_SHORT	"ENOTCONN"
#define ENOTDIR_MSG_SHORT   "ENOTDIR"
#define ENOTDIR_MSG         "Not a directory"
#define ENOTDIR_MSG_RU      "Это не каталог"
#define ENOTEMPTY_MSG_SHORT "ENOTEMPTY"
#define ENOTEMPTY_MSG       "Directory not empty"
#define ENOTEMPTY_MSG_RU    "Каталог не пуст"
#define ENOTSUP_MSG_SHORT   "ENOTSUP"
#define ENOTSOCK_MSG	      "Not a socket"
#define ENOTSOCK_MSG_RU	    "Не сокет"
#define ENOTSOCK_MSG_SHORT	"ENOTSOCK"
#define ENOTSUP_MSG         "Not supported"
#define ENOTSUP_MSG_RU      "Не поддерживается"
#define ENOTTY_MSG_SHORT    "ENOTTY"
#define ENOTTY_MSG          "Inappropriate I/O control operation"
#define ENOTTY_MSG_RU       "Неподходящая операция управления вводом/выводом"
#define ENXIO_MSG_SHORT     "ENXIO"
#define ENXIO_MSG           "No such device or address"
#define ENXIO_MSG_RU        "Устройство или адрес отсутствует"
#define EOPNOTSUPP_MSG      "Operation not supported on socket"
#define EOPNOTSUPP_MSG_RU	  "Операция не поддерживается на сокете"
#define EOPNOTSUPP_MSG_SHORT	"EOPNOTSUPP"
#define EOVERFLOW_MSG		    "Value too large to be stored in data type"
#define EOVERFLOW_MSG_RU	  "Значение слишком велико для типа данных"
#define EOVERFLOW_MSG_SHORT "EOVERFLOW"
#define EPERM_MSG_SHORT     "EPERM"
#define EPERM_MSG           "Operation not permitted"
#define EPERM_MSG_RU        "Операция запрещена"
#define EPIPE_MSG_SHORT     "EPIPE"
#define EPIPE_MSG           "Broken pipe"
#define EPIPE_MSG_RU        "Программный канал разрушен"
#define EPROTO_MSG			    "Protocol error"
#define EPROTO_MSG_RU		    "Ошибка протокола"
#define EPROTO_MSG_SHORT	  "EPROTO"
#define EPROTONOSUPPORT_MSG       "Protocol not supported"
#define EPROTONOSUPPORT_MSG_RU    "Протокол не поддерживается"
#define EPROTONOSUPPORT_MSG_SHORT "EPROTONOSUPPORT"
#define EPROTOTYPE_MSG		    "Socket type not supported"
#define EPROTOTYPE_MSG_RU	    "Тип сокета не поддерживается"
#define EPROTOTYPE_MSG_SHORT  "EPROTOTYPE"
#define ERANGE_MSG_SHORT    "ERANGE"
#define ERANGE_MSG          "Result too large"
#define ERANGE_MSG_RU       "Результат слишком велик"
#define EROFS_MSG_SHORT     "EROFS"
#define EROFS_MSG           "Read only file system"
#define EROFS_MSG_RU        "Файловая система допускает только чтение"
#define ESPIPE_MSG_SHORT    "ESPIPE"
#define ESPIPE_MSG          "Invalid seek"
#define ESPIPE_MSG_RU       "Некорректный запрос на поиск"
#define ESRCH_MSG_SHORT     "ESRCH"
#define ESRCH_MSG           "No such process"
#define ESRCH_MSG_RU        "Процесс отсутствует"
#define ESTALE_MSG			    "Reserverd"
#define ESTALE_MSG_RU		    "Зарезервировано"
#define ESTALE_MSG_SHORT	  "ESTALE"
#define ETIME_MSG			      "Stream ioctl() timeout"
#define ETIME_MSG_RU		    "Истекло время, отведенное для операции потокового ioctl()"
#define ETIME_MSG_SHORT		  "ETIME"
#define ETIMEDOUT_MSG_SHORT "ETIMEDOUT"
#define ETIMEDOUT_MSG       "Operation timed out"
#define ETIMEDOUT_MSG_RU    "Истекло время, отведенное для операции"
#define ETXTBSY_MSG			    "Text file busy"
#define ETXTBSY_MSG_RU		  "Текстовый файл занят"
#define ETXTBSY_MSG_SHORT	  "ETXTBSY"
#define EXDEV_MSG_SHORT     "EXDEV"
#define EXDEV_MSG           "Improper link"
#define EXDEV_MSG_RU        "Некорректная ссылка"

#define EHOSTDOWN_MSG		    "Host is down"
#define EHOSTDOWN_MSG_RU	  "Хост выключен"
#define EHOSTDOWN_MSG_SHORT "EHOSTDOWN"

#define ENETRESET_MSG		    "Network reset"
#define ENETRESET_MSG_RU	  "Сеть сброшена"
#define ENETRESET_MSG_SHORT "ENETRESET"

#define EPFNOSUPPORT_MSG		    "Protocol family not supported"
#define EPFNOSUPPORT_MSG_RU		  "Семейство протоколов не поддерживается"
#define EPFNOSUPPORT_MSG_SHORT	"EPFNOSUPPORT"

#define EBADRPC_MSG		          "RPC struct is bad"
#define EBADRPC_MSG_RU	        "Структура RPC испорчена"
#define EBADRPC_MSG_SHORT       "EBADRPC"

#define ENEEDAUTH_MSG		        "Need authenticator"
#define ENEEDAUTH_MSG_RU	      "Требуется аутентификация"
#define ENEEDAUTH_MSG_SHORT     "ENEEDAUTH"

#define EAUTH_MSG		            "Authentication error"
#define EAUTH_MSG_RU	          "Ошибка аутентификации"
#define EAUTH_MSG_SHORT         "EAUTH"

#define ERPCMISMATCH_MSG		    "RPC version wrong"
#define ERPCMISMATCH_MSG_RU	    "Неправильная версия RPC"
#define ERPCMISMATCH_MSG_SHORT  "ERPCMISMATCH"

#define EPROGUNAVAIL_MSG		    "RPC prog. not avail"
#define EPROGUNAVAIL_MSG_RU	    "RPC не доступно"
#define EPROGUNAVAIL_MSG_SHORT  "EPROGUNAVAIL"

#define EPROCUNAVAIL_MSG		    "Bad procedure for program"
#define EPROCUNAVAIL_MSG_RU	    "Плохая процедура"
#define EPROCUNAVAIL_MSG_SHORT  "EPROCUNAVAIL"

#define EPROGMISMATCH_MSG		    "Program version wrong"
#define EPROGMISMATCH_MSG_RU	  "Плохая версия программы"
#define EPROGMISMATCH_MSG_SHORT "EPROGMISMATCH"

#define EMEMBLOCK_MSG		        "Block memory wrong"
#define EMEMBLOCK_MSG_RU	      "Плохой блок памяти"
#define EMEMBLOCK_MSG_SHORT     "EMEMBLOCK"

#define EOBJEXIST_MSG		        "Object exists"
#define EOBJEXIST_MSG_RU	      "Объект уже существует"
#define EOBJEXIST_MSG_SHORT     "EOBJEXIST"

/*инициализация подсистемы выдачи системой текстовых сообщений*/
extern  int msgTableInit( int par, int charset );

extern int errGet(void);
extern char * (*errGetTextRtn)( int );
/*
void   errPrint(STATUS err);
*/


#endif /* _ERRSET_H */
