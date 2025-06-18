enum {
    E2BIG = 1,    /* Список аргументов слишком длинен*/
    EACCES,       /* Не хватает прав доступа*/
    EAGAIN,         /* Ресурс временно недоступен*/
    EBADF,          /* Некорректный дескриптор файла*/
    EBADMSG,        /* 5 Сообщение искажено*/
    EBUSY,          /* Ресурс занят*/
    ECANCELED,    /* Операция аннулирована*/
    ECHILD,         /* Дочерний процесс отсутствует*/
    EDEADLK,        /* Уход от клинча*/
    EDOM,           /* 10 Ошибка области определения*/
    EEXIST,         /* Файл уже существует*/
    EFAULT,         /* некорректный адрес*/
    EFBIG,          /* Файл слишком велик*/
    EINPROGRESS,  /* Операция в действии*/
    EINTR,          /* 15 Исполнение вызванной функции прервано*/
    EINVAL,         /* Неверный аргумент*/
    EIO,            /* Ошибка ввода/вывода*/
    EISDIR,         /* Это каталог*/
    EMFILE,         /* Слишком много открытых файлов*/
    EMLINK,         /* 20 Слишком много ссылок*/
    EMSGSIZE,     /* Неподходящая длина буфера сообщений*/
    ENAMETOOLONG,   /* Слишком длинное имя файла*/
    ENFILE,         /* В системе слишком много открытых файлов*/
    ENODEV,         /* Несуществующее устройство*/
    ENOENT,         /* 25 Файл или каталог отсутствует*/
    ENOEXEC,        /* Ошибочный формат файла, указанного в качестве исполнимого*/
    ENOLCK,       /* Нет свободных фиксаторов (locks)*/
    ENOMEM,       /* Нехватка памяти*/
    ENOSPC,       /* На устройстве нет места*/
    ENOSYS,       /* 30 Функция не реализована*/
    ENOTDIR,        /* Это не каталог*/
    ENOTEMPTY,    /* Каталог не пуст*/
    ENOTSUP,      /* Не поддерживается*/
    ENOTTY,       /* Неподходящая операция управления вводом/выводом*/
    ENXIO,          /* 35 Устройство или адрес отсутствует*/
    EPERM,          /* Операция запрещена*/
    EPIPE,          /* Программный канал разрушен*/
    ERANGE,         /* Результат слишком велик*/
    EROFS,          /* Файловая система допускает только чтение*/
    ESPIPE,         /* 40 Некорректный запрос на поиск*/
    ESRCH,          /* Процесс отсутствует*/
    ETIMEDOUT,    /* Истекло время, отведенное для операции*/
    EXDEV,        /* Некорректная ссылка*/
    EADDRINUSE,       /* Address in use*/
    EADDRNOTAVAIL,  /* 45 Address not available*/
    EAFNOSUPPORT,     /* Address family not supported*/
    EALREADY,           /* Connection already in progress*/
    ECONNABORTED,     /* Connection aborted*/
    ECONNREFUSED,     /* Connection refused*/
    ECONNRESET,       /* 50 Connection reset*/
    EDESTADDRREQ,     /* Destination address required*/
    EDQUOT,         /* Reserved*/
    EHOSTUNREACH,     /* Host is unreachable*/
    EIDRM,          /* Identifier removed*/
    EILSEQ,         /* 55 Illegal byte sequence*/
    EISCONN,        /* Socket is connected*/
    ELOOP,          /* Too many levels of symbolic links*/
    EMULTIHOP,      /* Reserved*/
    ENETDOWN,       /* Network is down*/
    ENETUNREACH,    /* 60 Network unreachable*/
    ENOBUFS,        /* No buffer space available*/
    ENODATA,        /* No message is available on the STREAM head read queue*/
    ENOLINK,        /* Reserved*/
    ENOMSG,         /* No message of the desired type*/
    ENOPROTOOPT,    /* 65 Protocol not available*/
    ENOSR,          /* No STREAM resources*/
    ENOSTR,         /* Not a STREAM*/
    ENOTCONN,       /* The socket is not connected*/
    ENOTSOCK,       /* Not a socket*/
    EOPNOTSUPP,     /* 70 Operation not supported on socket*/
    EOVERFLOW,      /* Value too large to be stored in data type*/
    EPROTO,         /* Protocol error*/
    EPROTONOSUPPORT,/* Protocol not supported*/
    EPROTOTYPE,     /* Socket type not supported*/
    ESTALE,         /* 75 Reserved*/
    ETIME,          /* Stream ioctl() timeout*/
    ETXTBSY,        /* Text file busy*/

    EHOSTDOWN,      /* Host is down */
    ENETRESET,      /* Network dropped connection because of reset */
    EPFNOSUPPORT,   /* 80 Protocol family not supported*/
    EBADRPC,        /* RPC struct is bad */
    ENEEDAUTH,      /* Need authenticator */
    EAUTH,          /* Authentication error */
    ERPCMISMATCH,   /* RPC version wrong */
    EPROGUNAVAIL,   /* 85 RPC prog. not avail */
    EPROCUNAVAIL,   /* Bad procedure for program */
    EPROGMISMATCH,  /* Program version wrong */
    EMEMBLOCK,      /* Block memory wrong */
    EOBJEXIST,      /* Object exists */

    ECONFIG,         /* 90 ARINC INVALID_CONFIG */
    EMODE,           /* ARINC INVALID_MODE */
    EACTION,         /* ARINC NO_ACTION */
    EAVAILABLE,      /* ARINC NOT_AVAILABLE */

    ECORRUPT,        /* Валидация объекта выявила ошибку */

    ELAST
};
/* EWOULDBLOCK,  Operation would block (may be the same value as [EAGAIN])*/
#define EWOULDBLOCK EAGAIN

extern int errno;


