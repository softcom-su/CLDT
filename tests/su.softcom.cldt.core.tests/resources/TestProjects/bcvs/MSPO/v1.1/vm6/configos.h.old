/*! Last Saved by OS VERSION 2.75.2 BSP VERSION 1.19.1 */
#define OC2000_CONFIG 1                 /*!    Конфигурирование ОС                */
#define OC2000_CONFIG_KERNEL 1          /*!       Ядро                            */
#undef INCLUDE_PRIO256_RDY_QUE          /*!          Очередь работоспособных потоков типа PRIO256 */
#undef INCLUDE_SPAN256_CLK_QUE          /*!          Очередь к таймеру типа SPAN256 */
#define ISR_STACK_SIZE 8196             /*!          Стек обработки прерываний    */
#define DEFAULT_STACK_SIZE 16384        /*!          Стек потоков управления      */
                                        /*!          Наследование атрибутов планирования */
#define DEFAULT_INHERITSCHED PTHREAD_EXPLICIT_SCHED
#define DEFAULT_SCHEDPOLICY SCHED_FIFO  /*!          Cтратегия планирования       */
#define DEFAULT_SCHED_PRIORITY 100      /*!          Приоритет по умолчанию       */
                                        /*!          Использование плавающей арифметики */
#define DEFAULT_FPUSTATE PTHREAD_FPU_ENABLE
#define DEFAULT_THREAD_QUANT 4          /*!          Квант планирования           */
#define OC2000_CONFIG_SCALING 1         /*!       Масштабирование                 */
#define INCLUDE_SIGNALS 1               /*!          Обработка сигналов           */
#define INCLUDE_TIMERS 1                /*!          Таймеры                      */
#define TIMER_MAX_CFG 10                /*!             Число таймеров            */
#define INCLUDE_POSIX_SEMAPHORES 1      /*!          Семафоры                     */
#define SEM_NSEMS_MAX_CFG 140           /*!             Число семафоров           */
#define INCLUDE_POSIX_CND 1             /*!          Условные переменные          */
#define INCLUDE_POSIX_MQ 1              /*!          Очереди сообщений            */
#undef INCLUDE_PRIO32_MQ_QUE            /*!             Очереди сообщений типа PRIO32 */
#define MQ_OPEN_MAX_CFG 10              /*!             Число очередей сообщений  */
#define INCLUDE_PIPES 1                 /*!          Программные каналы           */
#define INCLUDE_CONSOLE 1               /*!          Консоли                      */
#define CONSOLE_DEVICE "/dev/console"   /*!             Имя консоли               */
#define INCLUDE_FILESYS 1               /*!          INCLUDE_FILESYS              */
#undef INCLUDE_VFAT                     /*!             Файловая система VFAT     */
#undef INCLUDE_CD9660                   /*!             Файловая система ISO9660  */
#undef INCLUDE_FTP_FS                   /*!             Файловая система FTP      */
#undef INCLUDE_TAR_FS                   /*!             Файловая система TAR      */
#undef INCLUDE_NFS                      /*!             Файловая система NFS      */
#define INCLUDE_ENVIRONMENT 1           /*!             Переменные окружения      */
#undef INCLUDE_IEEE754                  /*!             Поддержка IEEE754         */
#define STREAM_MAX_CFG 100              /*!          Число потоков данных         */
#define OPEN_MAX_CFG 200                /*!          Число открытых файлов        */
#define VFS_CFG 1                       /*!          Файловая система             */
#define VFS_VNODES_MAX 100              /*!             Количество структур vnode */
#define VFS_BUFFERS_MAX 100             /*!             Количество буферов        */
#define VFS_MALLOC_MAX 819200           /*!             Объем буферной памяти     */
#undef MEMALLOC_CLEAR                   /*!          Очистка памяти               */
#define OC2000_CONFIG_A 1               /*!       Библиотеки стандарта С          */
#undef INCLUDE_CTYPE                    /*!          ctype.h                      */
#define INCLUDE_MATH 1                  /*!          math.h                       */
#undef INCLUDE_STDIO                    /*!          stdio.h                      */
#undef INCLUDE_STDLIB                   /*!          stdlib.h                     */
#undef INCLUDE_STRING                   /*!          string.h                     */
#undef INCLUDE_TIME                     /*!          time.h                       */
#define INCLUDE_NETWORK 1               /*!       Взаимодействие с сетью          */
#define NET_THREAD_PRIORITY 200         /*!          Приоритет inetd              */
#define INCLUDE_LOCAL_DOMAIN 1          /*!          Интерфейс петля              */
#define INCLUDE_PING 1                  /*!          Функция ping()               */
#undef BOOTER_INHERITANCE               /*!          Наследовать параметры загрузчика */
#undef INCLUDE_TELNET                   /*!          Подключение telnet-клиента   */
#undef INCLUDE_TELNETD                  /*!          Подключение telnet-сервера   */
#define TELNETD_PRIORITY 100            /*!             Приоритет сервера         */
#define INCLUDE_RPC 1                   /*!          Протокол RPC                 */
#define PORTMAPD_PRIORITY 127           /*!             Приоритет portmapd        */
#undef INCLUDE_NETTIME                  /*!          Получить значение времени по сети */
#define NETTIME_HOST_ADDRESS "host"     /*!             Адрес компьютера          */
#define NETTIME_ATTEMPT_COUNT 3         /*!             Число попыток             */
#define NETTIME_TIMEOUT 3               /*!             Тайм-аут в секундах       */
#define CONFIG_NETADDRESSES 1           /*!          Сетевые адреса               */
#undef INCLUDE_DEFAULT_GATEWAY          /*!             Используемый по умолчанию шлюз */
                                        /*!                Сетевой адрес шлюза    */
#define DEFAULT_GATEWAY_ADDRESS "192.168.0.1"
#define INCLUDE_NETADDRESS 1            /*!             Сетевые интерфейсы        */
#define INCLUDE_NETADDRESS0 1           /*!             Сетевые интерфейсы(0)     */
#define _IFNAME0 "de9"                  /*!                Имя интерфейса(0)      */
#define _ADDRESS0 "22.0.0.61"           /*!                IP адрес(0)            */
#define _NETMASK0 "255.255.255.0"       /*!                Сетевая маска(0)       */
#define _PTP0 "0.0.0.0"                 /*!                Адрес партнера(0)      */
#undef INCLUDE_NETADDRESS1              /*!             Сетевые интерфейсы(1)     */
#define _IFNAME1 "tu0"                  /*!                Имя интерфейса(1)      */
#define _ADDRESS1 "192.168.0.7"         /*!                IP адрес(1)            */
#define _NETMASK1 "255.255.255.0"       /*!                Сетевая маска(1)       */
#define _PTP1 "0.0.0.0"                 /*!                Адрес партнера(1)      */
#undef INCLUDE_NETADDRESS2              /*!             Сетевые интерфейсы(2)     */
#define _IFNAME2 "ln0"                  /*!                Имя интерфейса(2)      */
#define _ADDRESS2 "192.168.0.7"         /*!                IP адрес(2)            */
#define _NETMASK2 "255.255.255.0"       /*!                Сетевая маска(2)       */
#define _PTP2 "0.0.0.0"                 /*!                Адрес партнера(2)      */
#undef INCLUDE_NETADDRESS3              /*!             Сетевые интерфейсы(3)     */
#define _IFNAME3 "bn0"                  /*!                Имя интерфейса(3)      */
#define _ADDRESS3 "192.168.202.1"       /*!                IP адрес(3)            */
#define _NETMASK3 "255.255.255.0"       /*!                Сетевая маска(3)       */
#define _PTP3 "0.0.0.0"                 /*!                Адрес партнера(3)      */
#undef INCLUDE_NETADDRESS4              /*!             Сетевые интерфейсы(4)     */
#define _IFNAME4 "sl0"                  /*!                Имя интерфейса(4)      */
#define _ADDRESS4 "192.168.1.7"         /*!                IP адрес(4)            */
#define _NETMASK4 "255.255.255.0"       /*!                Сетевая маска(4)       */
#define _PTP4 "192.168.1.1"             /*!                Адрес партнера(4)      */
#undef INCLUDE_NETADDRESS5              /*!             Сетевые интерфейсы(5)     */
#define _IFNAME5 "ppp0"                 /*!                Имя интерфейса(5)      */
#define _ADDRESS5 "192.168.1.7"         /*!                IP адрес(5)            */
#define _NETMASK5 "255.255.255.0"       /*!                Сетевая маска(5)       */
#define _PTP5 "192.168.1.1"             /*!                Адрес партнера(5)      */
#define INCLUDE_HOST_TABLE_INIT 1       /*!          Таблица компьютеров          */
#define HOST_TABLE_NODES_COUNT 40       /*!             Число элементов           */
#define HOST_TABLE_ALIAS_COUNT 10       /*!             Число псевдонимов         */
#define HOST_TABLE_ADDRESS_COUNT 1      /*!             Число IP адресов          */
#define HOST_TABLE_NAME_LENGTH 40       /*!             Длина имени               */
#define HOST_TABLE_ADDRESS_LENGTH 4     /*!             Длина адреса              */
#define INCLUDE_ADDHOSTS 1              /*!             Список компьютеров        */
#define INCLUDE_ADDHOSTS0 1             /*!             Список компьютеров(0)     */
#define ADDHOSTS_NAME0 "host"           /*!                Имя компьютера(0)      */
#define ADDHOSTS_ADDRESS0 "192.168.0.1" /*!                Адрес компьютера(0)    */
#undef INCLUDE_ADDHOSTS1                /*!             Список компьютеров(1)     */
#define ADDHOSTS_NAME1 "host"           /*!                Имя компьютера(1)      */
#define ADDHOSTS_ADDRESS1 "192.168.0.1" /*!                Адрес компьютера(1)    */
#undef INCLUDE_ADDHOSTS2                /*!             Список компьютеров(2)     */
#define ADDHOSTS_NAME2 "host"           /*!                Имя компьютера(2)      */
#define ADDHOSTS_ADDRESS2 "192.168.0.1" /*!                Адрес компьютера(2)    */
#undef INCLUDE_ADDHOSTS3                /*!             Список компьютеров(3)     */
#define ADDHOSTS_NAME3 "host"           /*!                Имя компьютера(3)      */
#define ADDHOSTS_ADDRESS3 "192.168.0.1" /*!                Адрес компьютера(3)    */
#undef INCLUDE_ADDHOSTS4                /*!             Список компьютеров(4)     */
#define ADDHOSTS_NAME4 "host"           /*!                Имя компьютера(4)      */
#define ADDHOSTS_ADDRESS4 "192.168.0.1" /*!                Адрес компьютера(4)    */
#undef INCLUDE_NETGW                    /*!          Таблица маршрутизации        */
#define INCLUDE_NETGW0 1                /*!          Таблица маршрутизации(0)     */
#define GATEWAY_DST0 "192.168.0.1"      /*!             Цель(0)                   */
#define GATEWAY_GW0 "192.168.0.1"       /*!             Маршрут(0)                */
#define GATEWAY_MASK0 "255.255.255.0"   /*!             Маска(0)                  */
#undef INCLUDE_NETGW1                   /*!          Таблица маршрутизации(1)     */
#define GATEWAY_DST1 "192.168.0.1"      /*!             Цель(1)                   */
#define GATEWAY_GW1 "192.168.0.1"       /*!             Маршрут(1)                */
#define GATEWAY_MASK1 "255.255.255.0"   /*!             Маска(1)                  */
#undef INCLUDE_NETGW2                   /*!          Таблица маршрутизации(2)     */
#define GATEWAY_DST2 "192.168.0.1"      /*!             Цель(2)                   */
#define GATEWAY_GW2 "192.168.0.1"       /*!             Маршрут(2)                */
#define GATEWAY_MASK2 "255.255.255.0"   /*!             Маска(2)                  */
#undef INCLUDE_NETGW3                   /*!          Таблица маршрутизации(3)     */
#define GATEWAY_DST3 "192.168.0.1"      /*!             Цель(3)                   */
#define GATEWAY_GW3 "192.168.0.1"       /*!             Маршрут(3)                */
#define GATEWAY_MASK3 "255.255.255.0"   /*!             Маска(3)                  */
#undef INCLUDE_NETGW4                   /*!          Таблица маршрутизации(4)     */
#define GATEWAY_DST4 "192.168.0.1"      /*!             Цель(4)                   */
#define GATEWAY_GW4 "192.168.0.1"       /*!             Маршрут(4)                */
#define GATEWAY_MASK4 "255.255.255.0"   /*!             Маска(4)                  */
#undef INCLUDE_NETGW5                   /*!          Таблица маршрутизации(5)     */
#define GATEWAY_DST5 "192.168.0.1"      /*!             Цель(5)                   */
#define GATEWAY_GW5 "192.168.0.1"       /*!             Маршрут(5)                */
#define GATEWAY_MASK5 "255.255.255.0"   /*!             Маска(5)                  */
#undef INCLUDE_SNETGW                   /*!          Таблица MAC-адресов          */
#define INCLUDE_SNETGW0 1               /*!          Таблица MAC-адресов(0)       */
#define GATEWAY_SDST0 "192.168.0.1"     /*!             IP-адрес(0)               */
                                        /*!             MAC-адрес(0)              */
#define GATEWAY_SGW0 "00:00:00:00:00:00"
#undef INCLUDE_SNETGW1                  /*!          Таблица MAC-адресов(1)       */
#define GATEWAY_SDST1 "192.168.0.1"     /*!             IP-адрес(1)               */
                                        /*!             MAC-адрес(1)              */
#define GATEWAY_SGW1 "00:00:00:00:00:00"
#undef INCLUDE_SNETGW2                  /*!          Таблица MAC-адресов(2)       */
#define GATEWAY_SDST2 "192.168.0.1"     /*!             IP-адрес(2)               */
                                        /*!             MAC-адрес(2)              */
#define GATEWAY_SGW2 "00:00:00:00:00:00"
#undef INCLUDE_SNETGW3                  /*!          Таблица MAC-адресов(3)       */
#define GATEWAY_SDST3 "192.168.0.1"     /*!             IP-адрес(3)               */
                                        /*!             MAC-адрес(3)              */
#define GATEWAY_SGW3 "00:00:00:00:00:00"
#undef INCLUDE_SNETGW4                  /*!          Таблица MAC-адресов(4)       */
#define GATEWAY_SDST4 "192.168.0.1"     /*!             IP-адрес(4)               */
                                        /*!             MAC-адрес(4)              */
#define GATEWAY_SGW4 "00:00:00:00:00:00"
#undef INCLUDE_SNETGW5                  /*!          Таблица MAC-адресов(5)       */
#define GATEWAY_SDST5 "192.168.0.1"     /*!             IP-адрес(5)               */
                                        /*!             MAC-адрес(5)              */
#define GATEWAY_SGW5 "00:00:00:00:00:00"
#undef INCLUDE_SLIP                     /*!          Протокол SLIP                */
#define INCLUDE_SLIP0 1                 /*!          Протокол SLIP(0)             */
#define SLIP_DEVNAME0 "/dev/aux0"       /*!             Имя последовательного устройства(0) */
#define SLIP_MTU0 296                   /*!             MTU(0)                    */
#undef INCLUDE_SLIP1                    /*!          Протокол SLIP(1)             */
#define SLIP_DEVNAME1 "/dev/aux1"       /*!             Имя последовательного устройства(1) */
#define SLIP_MTU1 296                   /*!             MTU(1)                    */
#undef INCLUDE_SLIP2                    /*!          Протокол SLIP(2)             */
#define SLIP_DEVNAME2 "/dev/aux2"       /*!             Имя последовательного устройства(2) */
#define SLIP_MTU2 296                   /*!             MTU(2)                    */
#undef INCLUDE_SLIP3                    /*!          Протокол SLIP(3)             */
#define SLIP_DEVNAME3 "/dev/aux3"       /*!             Имя последовательного устройства(3) */
#define SLIP_MTU3 296                   /*!             MTU(3)                    */
#undef INCLUDE_PPP                      /*!          Протокол PPP                 */
#define INCLUDE_PPP0 1                  /*!          Протокол PPP(0)              */
#define PPP_DEVNAME0 "/dev/aux0"        /*!             Имя последовательного устройства(0) */
#undef INCLUDE_PPP1                     /*!          Протокол PPP(1)              */
#define PPP_DEVNAME1 "/dev/aux1"        /*!             Имя последовательного устройства(1) */
#undef INCLUDE_PPP2                     /*!          Протокол PPP(2)              */
#define PPP_DEVNAME2 "/dev/aux2"        /*!             Имя последовательного устройства(2) */
#undef INCLUDE_PPP3                     /*!          Протокол PPP(3)              */
#define PPP_DEVNAME3 "/dev/aux3"        /*!             Имя последовательного устройства(3) */
#define MBUF_CONFIG 1                   /*!          Сетевые буфера               */
#define NMB_INIT 16                     /*!             Начальное число m-буферов */
#define NCL_INIT 1                      /*!             Начальное число кластеров */
#define NMB_ADD 16                      /*!             Добавляемое число m-буферов */
#define NCL_ADD 1                       /*!             Добавляемое число кластеров */
#define NMB_MAX 8192                    /*!             Максимальное число m-буферов */
#define NCL_MAX 1024                    /*!             Максимальное число кластеров */
#define INCLUDE_EXTRA_FS 1              /*!       Файловые системы                */
#undef MOUNT_VFAT                       /*!          Файловая система VFAT        */
#define VFAT_CHECK 1                    /*!             Параметры проверки диска  */
#define BFSC_OUT BFSC_OUT_NORMAL        /*!                Режим вывода           */
#define BFSC_MODE BFSC_ALW_YES          /*!                Режим исправления      */
#undef BFSC_SAVE_BAD                    /*!                Сохранять некорректные цепочки */
#undef BFSC_DO_CHECK_LEN                /*!                Проверять длины файлов */
#define BFSC_DO_SYNC_FATS 1             /*!                Согласовать таблицы FAT */
#define VFAT_MNT_VOL 1                  /*!             Параметры установки VFAT  */
#define VFAT_MNT_VOL0 1                 /*!             Параметры установки VFAT(0) */
#define VFAT_MOUNTPOINT0 "/hda1"        /*!                Точка установки(0)     */
#define VFAT_BDNAME0 "/dev/hda1"        /*!                Имя блочного устройства(0) */
#define VFAT_RDONLY0 0                  /*!                Только чтение(0)       */
#define VFAT_SYNCHRONOUS0 0             /*!                Синхронизировать(0)    */
#define VFAT_NOCHECK0 0                 /*!                Не проверять(0)        */
#define VFAT_UID0 203                   /*!                UID(0)                 */
#define VFAT_GID0 3                     /*!                GID(0)                 */
#define VFAT_ACCESS0 0777               /*!                Права доступа(0)       */
#undef VFAT_MNT_VOL1                    /*!             Параметры установки VFAT(1) */
#define VFAT_MOUNTPOINT1 "/hda2"        /*!                Точка установки(1)     */
#define VFAT_BDNAME1 "/dev/hda2"        /*!                Имя блочного устройства(1) */
#define VFAT_RDONLY1 0                  /*!                Только чтение(1)       */
#define VFAT_SYNCHRONOUS1 0             /*!                Синхронизировать(1)    */
#define VFAT_NOCHECK1 0                 /*!                Не проверять(1)        */
#define VFAT_UID1 203                   /*!                UID(1)                 */
#define VFAT_GID1 3                     /*!                GID(1)                 */
#define VFAT_ACCESS1 0777               /*!                Права доступа(1)       */
#undef VFAT_MNT_VOL2                    /*!             Параметры установки VFAT(2) */
#define VFAT_MOUNTPOINT2 "/hdb1"        /*!                Точка установки(2)     */
#define VFAT_BDNAME2 "/dev/hdb1"        /*!                Имя блочного устройства(2) */
#define VFAT_RDONLY2 0                  /*!                Только чтение(2)       */
#define VFAT_SYNCHRONOUS2 0             /*!                Синхронизировать(2)    */
#define VFAT_NOCHECK2 0                 /*!                Не проверять(2)        */
#define VFAT_UID2 203                   /*!                UID(2)                 */
#define VFAT_GID2 3                     /*!                GID(2)                 */
#define VFAT_ACCESS2 0777               /*!                Права доступа(2)       */
#undef VFAT_MNT_VOL3                    /*!             Параметры установки VFAT(3) */
#define VFAT_MOUNTPOINT3 "/hdb2"        /*!                Точка установки(3)     */
#define VFAT_BDNAME3 "/dev/hdb2"        /*!                Имя блочного устройства(3) */
#define VFAT_RDONLY3 0                  /*!                Только чтение(3)       */
#define VFAT_SYNCHRONOUS3 0             /*!                Синхронизировать(3)    */
#define VFAT_NOCHECK3 0                 /*!                Не проверять(3)        */
#define VFAT_UID3 203                   /*!                UID(3)                 */
#define VFAT_GID3 3                     /*!                GID(3)                 */
#define VFAT_ACCESS3 0777               /*!                Права доступа(3)       */
#undef MOUNT_CD9660                     /*!          Файловая система CD-ISO9660  */
#define MOUNT_CD96600 1                 /*!          Файловая система CD-ISO9660(0) */
#define CD9660_MOUNTPOINT0 "/cd1"       /*!             Точка установки(0)        */
#define CD9660_BDNAME0 "/dev/hda1"      /*!             Имя блочного устройства(0) */
#undef MOUNT_CD96601                    /*!          Файловая система CD-ISO9660(1) */
#define CD9660_MOUNTPOINT1 "/cd2"       /*!             Точка установки(1)        */
#define CD9660_BDNAME1 "/dev/hda2"      /*!             Имя блочного устройства(1) */
#undef MOUNT_TAR                        /*!          Файловая система TAR         */
#define MOUNT_TAR0 1                    /*!          Файловая система TAR(0)      */
#define TAR_MOUNTPOINT0 "/tar"          /*!             Точка установки(0)        */
#define TAR_MEMADDRESS0 0x0             /*!             Адрес памяти(0)           */
#define TAR_MEMSIZE0 0x0                /*!             Размер(0)                 */
#define TAR_CHECK0 0x0                  /*!             Проверять на корректность перед установкой(0) */
#undef MOUNT_TAR1                       /*!          Файловая система TAR(1)      */
#define TAR_MOUNTPOINT1 "/tar"          /*!             Точка установки(1)        */
#define TAR_MEMADDRESS1 0x0             /*!             Адрес памяти(1)           */
#define TAR_MEMSIZE1 0x0                /*!             Размер(1)                 */
#define TAR_CHECK1 0x0                  /*!             Проверять на корректность перед установкой(1) */
#undef MOUNT_TAR2                       /*!          Файловая система TAR(2)      */
#define TAR_MOUNTPOINT2 "/tar"          /*!             Точка установки(2)        */
#define TAR_MEMADDRESS2 0x0             /*!             Адрес памяти(2)           */
#define TAR_MEMSIZE2 0x0                /*!             Размер(2)                 */
#define TAR_CHECK2 0x0                  /*!             Проверять на корректность перед установкой(2) */
#undef MOUNT_TAR3                       /*!          Файловая система TAR(3)      */
#define TAR_MOUNTPOINT3 "/tar"          /*!             Точка установки(3)        */
#define TAR_MEMADDRESS3 0x0             /*!             Адрес памяти(3)           */
#define TAR_MEMSIZE3 0x0                /*!             Размер(3)                 */
#define TAR_CHECK3 0x0                  /*!             Проверять на корректность перед установкой(3) */
#define TAR_PNAME0 "/tarfile"           /*!             Точка установки(0)        */
#define TAR_FCHECK0 0x0                 /*!             Проверять на корректность перед установкой(0) */
#define TAR_PNAME1 "/tarfile"           /*!             Точка установки(1)        */
#define TAR_FCHECK1 0x0                 /*!             Проверять на корректность перед установкой(1) */
#define TAR_PNAME2 "/tarfile"           /*!             Точка установки(2)        */
#define TAR_FCHECK2 0x0                 /*!             Проверять на корректность перед установкой(2) */
#define TAR_PNAME3 "/tarfile"           /*!             Точка установки(3)        */
#define TAR_FCHECK3 0x0                 /*!             Проверять на корректность перед установкой(3) */
#define MOUNT_NFS_AUTO 1                /*!          Файловая система NFS         */
#define UID 203                         /*!             Идентификатор пользователя */
#define GID 3                           /*!             Идентификатор группы      */
#define MOUNT_NFS 1                     /*!             MOUNT_NFS                 */
#define MOUNT_NFS0 1                    /*!             MOUNT_NFS0(0)             */
#define NFS_HOST_ADDRESS0 "host"        /*!                Адрес сервера(0)       */
#define NFS_EXPORT0 "/tmp"              /*!                Имя каталога на сервере(0) */
#define NFS_MOUNTPOINT0 "/nfs"          /*!                Точка установки(0)     */
#define NFS_RSIZE0 0                    /*!                NFS_RSIZE(0)           */
#define NFS_WSIZE0 0                    /*!                NFS_WSIZE(0)           */
#define NFS_FORCE0 0                    /*!                NFS_FORCE(0)           */
#undef MOUNT_NFS1                       /*!             MOUNT_NFS1(1)             */
#define NFS_HOST_ADDRESS1 "host"        /*!                Адрес сервера(1)       */
#define NFS_EXPORT1 "/tmp"              /*!                Имя каталога на сервере(1) */
#define NFS_MOUNTPOINT1 "/nfs"          /*!                Точка установки(1)     */
#define NFS_RSIZE1 0                    /*!                NFS_RSIZE(1)           */
#define NFS_WSIZE1 0                    /*!                NFS_WSIZE(1)           */
#define NFS_FORCE1 0                    /*!                NFS_FORCE(1)           */
#undef MOUNT_NFS2                       /*!             MOUNT_NFS2(2)             */
#define NFS_HOST_ADDRESS2 "host"        /*!                Адрес сервера(2)       */
#define NFS_EXPORT2 "/tmp"              /*!                Имя каталога на сервере(2) */
#define NFS_MOUNTPOINT2 "/nfs"          /*!                Точка установки(2)     */
#define NFS_RSIZE2 0                    /*!                NFS_RSIZE(2)           */
#define NFS_WSIZE2 0                    /*!                NFS_WSIZE(2)           */
#define NFS_FORCE2 0                    /*!                NFS_FORCE(2)           */
#undef MOUNT_NFS3                       /*!             MOUNT_NFS3(3)             */
#define NFS_HOST_ADDRESS3 "host"        /*!                Адрес сервера(3)       */
#define NFS_EXPORT3 "/tmp"              /*!                Имя каталога на сервере(3) */
#define NFS_MOUNTPOINT3 "/nfs"          /*!                Точка установки(3)     */
#define NFS_RSIZE3 0                    /*!                NFS_RSIZE(3)           */
#define NFS_WSIZE3 0                    /*!                NFS_WSIZE(3)           */
#define NFS_FORCE3 0                    /*!                NFS_FORCE(3)           */
#undef MOUNT_FTP                        /*!          Файловая система FTP         */
#define MOUNT_FTP0 1                    /*!          Файловая система FTP(0)      */
#define FTP_MOUNTPOINT0 "/ftp"          /*!             Точка установки(0)        */
                                        /*!             Параметры(0)              */
#define FTP_PARSTRING0 "user:password@hostname"
#undef MOUNT_FTP1                       /*!          Файловая система FTP(1)      */
#define FTP_MOUNTPOINT1 "/ftp"          /*!             Точка установки(1)        */
                                        /*!             Параметры(1)              */
#define FTP_PARSTRING1 "user:password@hostname"
#undef MOUNT_FTP2                       /*!          Файловая система FTP(2)      */
#define FTP_MOUNTPOINT2 "/ftp"          /*!             Точка установки(2)        */
                                        /*!             Параметры(2)              */
#define FTP_PARSTRING2 "user:password@hostname"
#define MASTER_CPU 0                    /*!             Номер ведущего процессора */
#define CPU_QUANTITY 2                  /*!             Число процессоров         */
#define MP_TCB_MAX 12                   /*!             Число потоков управления  */
#define MP_SEM_MAX 20                   /*!             Число семафоров           */
#define MP_MQP_MAX 10                   /*!             Число очередей сообщений  */
#define MP_DIC_MAX 40                   /*!             Число элементов в словаре */
#undef INCLUDE_MPMEMPOOL_SELF           /*!             Отдельный пул             */
#define MP_LOCKTRIES_MAX 200            /*!             Число попыток             */
#undef INCLUDE_MPSHM                    /*!             Использование общей памяти X протоколом */
#undef MP_NETWORK                       /*!          Эмуляция Ethernet на VME     */
#define MP_NETWORK_TYPE MP_NETWORK_VITA /*!             Реализация                */
#define VITA_CONFIGURATION 1            /*!             VITA                      */
#define BN_MASTER_ADDR 0                /*!                Номер мастера          */
#define BN_PACKET_SIZE 2048             /*!                Размер пакетов         */
#define BN_P_PROBE_STARTDELAY 1000000   /*!                Стартовая задержка(мкс) */
#define BN_P_PROBE_MININTERVAL 1000000  /*!                Задержка между попытками(мкс) */
#define BN_P_PROBE_MAXCOUNT 50          /*!                Число попыток          */
#define BN_P_PROBE_BACKOFFCOUNT 3       /*!                Число неудачных попыток */
#define BN_P_PROBE_MAXINTERVAL 20000000 /*!                Максимальное время попыток(мкс) */
#undef INCLUDE_CHANNELS                 /*!          Каналы передачи данных       */
#define MPCHAN_THREAD_PRIORITY 101      /*!             Приоритет потока          */
#define MPCHAN_MAX_NUMBER 5             /*!             Число каналов             */
#define INCLUDE_MPCHAN_VME 1            /*!             VME                       */
#undef INCLUDE_MPCHAN_RIO               /*!             RIO                       */
#undef INCLUDE_MPCHAN_NET               /*!             NET                       */
#undef MPCHAN_ETIMEDOUT_IGNORE          /*!             Игнорировать ошибку etimedout при отправлении сообщений по межмодульным каналам */
#undef MPCHATHREAD_PERIODIC             /*!             Периодический mpchanThread */
#define MPCHATHREAD_PERIOD 0            /*!                Период (нс)            */
#undef INCLUDE_ARINC_QPORT              /*!             Порты с очередью сообщений */
#define QPORTLIST_MAX_CFG 5             /*!                Число списков портов с очередью сообщений */
#define QPORT_OPEN_MAX_CFG 32           /*!                Число портов           */
#define KERNEL_QPORT_CONFIG 1           /*!                Порты с очередью сообщений */
#define KERNEL_QPORT_CONFIG0 1          /*!                Порты с очередью сообщений(0) */
#define KERNEL_QPORT_NAME0 ""           /*!                   Имя(0)              */
#define KERNEL_QPORT_DIRECT0 SOURCE     /*!                   Направление передачи данных(0) */
#define KERNEL_QPORT_BUFNUMBER0 0       /*!                   Число буферов(0)    */
#define KERNEL_QPORT_BUFSIZE0 0         /*!                   Размер буфера(0)    */
#define KERNEL_QPORT_CONFIG1 0          /*!                Порты с очередью сообщений(1) */
#define KERNEL_QPORT_NAME1 ""           /*!                   Имя(1)              */
#define KERNEL_QPORT_DIRECT1 SOURCE     /*!                   Направление передачи данных(1) */
#define KERNEL_QPORT_BUFNUMBER1 0       /*!                   Число буферов(1)    */
#define KERNEL_QPORT_BUFSIZE1 0         /*!                   Размер буфера(1)    */
#define KERNEL_QPORT_CONFIG2 0          /*!                Порты с очередью сообщений(2) */
#define KERNEL_QPORT_NAME2 ""           /*!                   Имя(2)              */
#define KERNEL_QPORT_DIRECT2 SOURCE     /*!                   Направление передачи данных(2) */
#define KERNEL_QPORT_BUFNUMBER2 0       /*!                   Число буферов(2)    */
#define KERNEL_QPORT_BUFSIZE2 0         /*!                   Размер буфера(2)    */
#define KERNEL_QPORT_CONFIG3 0          /*!                Порты с очередью сообщений(3) */
#define KERNEL_QPORT_NAME3 ""           /*!                   Имя(3)              */
#define KERNEL_QPORT_DIRECT3 SOURCE     /*!                   Направление передачи данных(3) */
#define KERNEL_QPORT_BUFNUMBER3 0       /*!                   Число буферов(3)    */
#define KERNEL_QPORT_BUFSIZE3 0         /*!                   Размер буфера(3)    */
#define KERNEL_QPORT_CONFIG4 0          /*!                Порты с очередью сообщений(4) */
#define KERNEL_QPORT_NAME4 ""           /*!                   Имя(4)              */
#define KERNEL_QPORT_DIRECT4 SOURCE     /*!                   Направление передачи данных(4) */
#define KERNEL_QPORT_BUFNUMBER4 0       /*!                   Число буферов(4)    */
#define KERNEL_QPORT_BUFSIZE4 0         /*!                   Размер буфера(4)    */
#define KERNEL_QPORT_CONFIG5 0          /*!                Порты с очередью сообщений(5) */
#define KERNEL_QPORT_NAME5 ""           /*!                   Имя(5)              */
#define KERNEL_QPORT_DIRECT5 SOURCE     /*!                   Направление передачи данных(5) */
#define KERNEL_QPORT_BUFNUMBER5 0       /*!                   Число буферов(5)    */
#define KERNEL_QPORT_BUFSIZE5 0         /*!                   Размер буфера(5)    */
#define KERNEL_QPORT_CONFIG6 0          /*!                Порты с очередью сообщений(6) */
#define KERNEL_QPORT_NAME6 ""           /*!                   Имя(6)              */
#define KERNEL_QPORT_DIRECT6 SOURCE     /*!                   Направление передачи данных(6) */
#define KERNEL_QPORT_BUFNUMBER6 0       /*!                   Число буферов(6)    */
#define KERNEL_QPORT_BUFSIZE6 0         /*!                   Размер буфера(6)    */
#define KERNEL_QPORT_CONFIG7 0          /*!                Порты с очередью сообщений(7) */
#define KERNEL_QPORT_NAME7 ""           /*!                   Имя(7)              */
#define KERNEL_QPORT_DIRECT7 SOURCE     /*!                   Направление передачи данных(7) */
#define KERNEL_QPORT_BUFNUMBER7 0       /*!                   Число буферов(7)    */
#define KERNEL_QPORT_BUFSIZE7 0         /*!                   Размер буфера(7)    */
#define KERNEL_QPORT_CONFIG8 0          /*!                Порты с очередью сообщений(8) */
#define KERNEL_QPORT_NAME8 ""           /*!                   Имя(8)              */
#define KERNEL_QPORT_DIRECT8 SOURCE     /*!                   Направление передачи данных(8) */
#define KERNEL_QPORT_BUFNUMBER8 0       /*!                   Число буферов(8)    */
#define KERNEL_QPORT_BUFSIZE8 0         /*!                   Размер буфера(8)    */
#define KERNEL_QPORT_CONFIG9 0          /*!                Порты с очередью сообщений(9) */
#define KERNEL_QPORT_NAME9 ""           /*!                   Имя(9)              */
#define KERNEL_QPORT_DIRECT9 SOURCE     /*!                   Направление передачи данных(9) */
#define KERNEL_QPORT_BUFNUMBER9 0       /*!                   Число буферов(9)    */
#define KERNEL_QPORT_BUFSIZE9 0         /*!                   Размер буфера(9)    */
#define KERNEL_QPORT_CONFIG10 0         /*!                Порты с очередью сообщений(10) */
#define KERNEL_QPORT_NAME10 ""          /*!                   Имя(10)             */
#define KERNEL_QPORT_DIRECT10 SOURCE    /*!                   Направление передачи данных(10) */
#define KERNEL_QPORT_BUFNUMBER10 0      /*!                   Число буферов(10)   */
#define KERNEL_QPORT_BUFSIZE10 0        /*!                   Размер буфера(10)   */
#define KERNEL_QPORT_CONFIG11 0         /*!                Порты с очередью сообщений(11) */
#define KERNEL_QPORT_NAME11 ""          /*!                   Имя(11)             */
#define KERNEL_QPORT_DIRECT11 SOURCE    /*!                   Направление передачи данных(11) */
#define KERNEL_QPORT_BUFNUMBER11 0      /*!                   Число буферов(11)   */
#define KERNEL_QPORT_BUFSIZE11 0        /*!                   Размер буфера(11)   */
#define KERNEL_QPORT_CONFIG12 0         /*!                Порты с очередью сообщений(12) */
#define KERNEL_QPORT_NAME12 ""          /*!                   Имя(12)             */
#define KERNEL_QPORT_DIRECT12 SOURCE    /*!                   Направление передачи данных(12) */
#define KERNEL_QPORT_BUFNUMBER12 0      /*!                   Число буферов(12)   */
#define KERNEL_QPORT_BUFSIZE12 0        /*!                   Размер буфера(12)   */
#define KERNEL_QPORT_CONFIG13 0         /*!                Порты с очередью сообщений(13) */
#define KERNEL_QPORT_NAME13 ""          /*!                   Имя(13)             */
#define KERNEL_QPORT_DIRECT13 SOURCE    /*!                   Направление передачи данных(13) */
#define KERNEL_QPORT_BUFNUMBER13 0      /*!                   Число буферов(13)   */
#define KERNEL_QPORT_BUFSIZE13 0        /*!                   Размер буфера(13)   */
#define KERNEL_QPORT_CONFIG14 0         /*!                Порты с очередью сообщений(14) */
#define KERNEL_QPORT_NAME14 ""          /*!                   Имя(14)             */
#define KERNEL_QPORT_DIRECT14 SOURCE    /*!                   Направление передачи данных(14) */
#define KERNEL_QPORT_BUFNUMBER14 0      /*!                   Число буферов(14)   */
#define KERNEL_QPORT_BUFSIZE14 0        /*!                   Размер буфера(14)   */
#define KERNEL_QPORT_CONFIG15 0         /*!                Порты с очередью сообщений(15) */
#define KERNEL_QPORT_NAME15 ""          /*!                   Имя(15)             */
#define KERNEL_QPORT_DIRECT15 SOURCE    /*!                   Направление передачи данных(15) */
#define KERNEL_QPORT_BUFNUMBER15 0      /*!                   Число буферов(15)   */
#define KERNEL_QPORT_BUFSIZE15 0        /*!                   Размер буфера(15)   */
#define KERNEL_QPORT_CONFIG16 0         /*!                Порты с очередью сообщений(16) */
#define KERNEL_QPORT_NAME16 ""          /*!                   Имя(16)             */
#define KERNEL_QPORT_DIRECT16 SOURCE    /*!                   Направление передачи данных(16) */
#define KERNEL_QPORT_BUFNUMBER16 0      /*!                   Число буферов(16)   */
#define KERNEL_QPORT_BUFSIZE16 0        /*!                   Размер буфера(16)   */
#define KERNEL_QPORT_CONFIG17 0         /*!                Порты с очередью сообщений(17) */
#define KERNEL_QPORT_NAME17 ""          /*!                   Имя(17)             */
#define KERNEL_QPORT_DIRECT17 SOURCE    /*!                   Направление передачи данных(17) */
#define KERNEL_QPORT_BUFNUMBER17 0      /*!                   Число буферов(17)   */
#define KERNEL_QPORT_BUFSIZE17 0        /*!                   Размер буфера(17)   */
#define KERNEL_QPORT_CONFIG18 0         /*!                Порты с очередью сообщений(18) */
#define KERNEL_QPORT_NAME18 ""          /*!                   Имя(18)             */
#define KERNEL_QPORT_DIRECT18 SOURCE    /*!                   Направление передачи данных(18) */
#define KERNEL_QPORT_BUFNUMBER18 0      /*!                   Число буферов(18)   */
#define KERNEL_QPORT_BUFSIZE18 0        /*!                   Размер буфера(18)   */
#define KERNEL_QPORT_CONFIG19 0         /*!                Порты с очередью сообщений(19) */
#define KERNEL_QPORT_NAME19 ""          /*!                   Имя(19)             */
#define KERNEL_QPORT_DIRECT19 SOURCE    /*!                   Направление передачи данных(19) */
#define KERNEL_QPORT_BUFNUMBER19 0      /*!                   Число буферов(19)   */
#define KERNEL_QPORT_BUFSIZE19 0        /*!                   Размер буфера(19)   */
#define KERNEL_QPORT_CONFIG20 0         /*!                Порты с очередью сообщений(20) */
#define KERNEL_QPORT_NAME20 ""          /*!                   Имя(20)             */
#define KERNEL_QPORT_DIRECT20 SOURCE    /*!                   Направление передачи данных(20) */
#define KERNEL_QPORT_BUFNUMBER20 0      /*!                   Число буферов(20)   */
#define KERNEL_QPORT_BUFSIZE20 0        /*!                   Размер буфера(20)   */
#define KERNEL_QPORT_CONFIG21 0         /*!                Порты с очередью сообщений(21) */
#define KERNEL_QPORT_NAME21 ""          /*!                   Имя(21)             */
#define KERNEL_QPORT_DIRECT21 SOURCE    /*!                   Направление передачи данных(21) */
#define KERNEL_QPORT_BUFNUMBER21 0      /*!                   Число буферов(21)   */
#define KERNEL_QPORT_BUFSIZE21 0        /*!                   Размер буфера(21)   */
#define KERNEL_QPORT_CONFIG22 0         /*!                Порты с очередью сообщений(22) */
#define KERNEL_QPORT_NAME22 ""          /*!                   Имя(22)             */
#define KERNEL_QPORT_DIRECT22 SOURCE    /*!                   Направление передачи данных(22) */
#define KERNEL_QPORT_BUFNUMBER22 0      /*!                   Число буферов(22)   */
#define KERNEL_QPORT_BUFSIZE22 0        /*!                   Размер буфера(22)   */
#define KERNEL_QPORT_CONFIG23 0         /*!                Порты с очередью сообщений(23) */
#define KERNEL_QPORT_NAME23 ""          /*!                   Имя(23)             */
#define KERNEL_QPORT_DIRECT23 SOURCE    /*!                   Направление передачи данных(23) */
#define KERNEL_QPORT_BUFNUMBER23 0      /*!                   Число буферов(23)   */
#define KERNEL_QPORT_BUFSIZE23 0        /*!                   Размер буфера(23)   */
#define KERNEL_QPORT_CONFIG24 0         /*!                Порты с очередью сообщений(24) */
#define KERNEL_QPORT_NAME24 ""          /*!                   Имя(24)             */
#define KERNEL_QPORT_DIRECT24 SOURCE    /*!                   Направление передачи данных(24) */
#define KERNEL_QPORT_BUFNUMBER24 0      /*!                   Число буферов(24)   */
#define KERNEL_QPORT_BUFSIZE24 0        /*!                   Размер буфера(24)   */
#define KERNEL_QPORT_CONFIG25 0         /*!                Порты с очередью сообщений(25) */
#define KERNEL_QPORT_NAME25 ""          /*!                   Имя(25)             */
#define KERNEL_QPORT_DIRECT25 SOURCE    /*!                   Направление передачи данных(25) */
#define KERNEL_QPORT_BUFNUMBER25 0      /*!                   Число буферов(25)   */
#define KERNEL_QPORT_BUFSIZE25 0        /*!                   Размер буфера(25)   */
#define KERNEL_QPORT_CONFIG26 0         /*!                Порты с очередью сообщений(26) */
#define KERNEL_QPORT_NAME26 ""          /*!                   Имя(26)             */
#define KERNEL_QPORT_DIRECT26 SOURCE    /*!                   Направление передачи данных(26) */
#define KERNEL_QPORT_BUFNUMBER26 0      /*!                   Число буферов(26)   */
#define KERNEL_QPORT_BUFSIZE26 0        /*!                   Размер буфера(26)   */
#define KERNEL_QPORT_CONFIG27 0         /*!                Порты с очередью сообщений(27) */
#define KERNEL_QPORT_NAME27 ""          /*!                   Имя(27)             */
#define KERNEL_QPORT_DIRECT27 SOURCE    /*!                   Направление передачи данных(27) */
#define KERNEL_QPORT_BUFNUMBER27 0      /*!                   Число буферов(27)   */
#define KERNEL_QPORT_BUFSIZE27 0        /*!                   Размер буфера(27)   */
#define KERNEL_QPORT_CONFIG28 0         /*!                Порты с очередью сообщений(28) */
#define KERNEL_QPORT_NAME28 ""          /*!                   Имя(28)             */
#define KERNEL_QPORT_DIRECT28 SOURCE    /*!                   Направление передачи данных(28) */
#define KERNEL_QPORT_BUFNUMBER28 0      /*!                   Число буферов(28)   */
#define KERNEL_QPORT_BUFSIZE28 0        /*!                   Размер буфера(28)   */
#define KERNEL_QPORT_CONFIG29 0         /*!                Порты с очередью сообщений(29) */
#define KERNEL_QPORT_NAME29 ""          /*!                   Имя(29)             */
#define KERNEL_QPORT_DIRECT29 SOURCE    /*!                   Направление передачи данных(29) */
#define KERNEL_QPORT_BUFNUMBER29 0      /*!                   Число буферов(29)   */
#define KERNEL_QPORT_BUFSIZE29 0        /*!                   Размер буфера(29)   */
#define KERNEL_QPORT_CONFIG30 0         /*!                Порты с очередью сообщений(30) */
#define KERNEL_QPORT_NAME30 ""          /*!                   Имя(30)             */
#define KERNEL_QPORT_DIRECT30 SOURCE    /*!                   Направление передачи данных(30) */
#define KERNEL_QPORT_BUFNUMBER30 0      /*!                   Число буферов(30)   */
#define KERNEL_QPORT_BUFSIZE30 0        /*!                   Размер буфера(30)   */
#define KERNEL_QPORT_CONFIG31 0         /*!                Порты с очередью сообщений(31) */
#define KERNEL_QPORT_NAME31 ""          /*!                   Имя(31)             */
#define KERNEL_QPORT_DIRECT31 SOURCE    /*!                   Направление передачи данных(31) */
#define KERNEL_QPORT_BUFNUMBER31 0      /*!                   Число буферов(31)   */
#define KERNEL_QPORT_BUFSIZE31 0        /*!                   Размер буфера(31)   */
#define INCLUDE_ARINC_SPORT 1           /*!             Порты без очереди сообщений */
#define SPORT_OPEN_MAX_CFG 32           /*!                Число портов           */
#define KERNEL_SPORT_CONFIG 1           /*!                Порты без очереди сообщений */
#define KERNEL_SPORT_CONFIG0 1          /*!                Порты без очереди сообщений(0) */
#define KERNEL_SPORT_NAME0 ""           /*!                   Имя(0)              */
#define KERNEL_SPORT_DIRECT0 SOURCE     /*!                   Направление передачи данных(0) */
#define KERNEL_SPORT_BUFSIZE0 0         /*!                   Размер буфера(0)    */
#define KERNEL_SPORT_REFRESH0 0         /*!                   Период обновления(0) */
#define KERNEL_SPORT_CONFIG1 0          /*!                Порты без очереди сообщений(1) */
#define KERNEL_SPORT_NAME1 ""           /*!                   Имя(1)              */
#define KERNEL_SPORT_DIRECT1 SOURCE     /*!                   Направление передачи данных(1) */
#define KERNEL_SPORT_BUFSIZE1 0         /*!                   Размер буфера(1)    */
#define KERNEL_SPORT_REFRESH1 0         /*!                   Период обновления(1) */
#define KERNEL_SPORT_CONFIG2 0          /*!                Порты без очереди сообщений(2) */
#define KERNEL_SPORT_NAME2 ""           /*!                   Имя(2)              */
#define KERNEL_SPORT_DIRECT2 SOURCE     /*!                   Направление передачи данных(2) */
#define KERNEL_SPORT_BUFSIZE2 0         /*!                   Размер буфера(2)    */
#define KERNEL_SPORT_REFRESH2 0         /*!                   Период обновления(2) */
#define KERNEL_SPORT_CONFIG3 0          /*!                Порты без очереди сообщений(3) */
#define KERNEL_SPORT_NAME3 ""           /*!                   Имя(3)              */
#define KERNEL_SPORT_DIRECT3 SOURCE     /*!                   Направление передачи данных(3) */
#define KERNEL_SPORT_BUFSIZE3 0         /*!                   Размер буфера(3)    */
#define KERNEL_SPORT_REFRESH3 0         /*!                   Период обновления(3) */
#define KERNEL_SPORT_CONFIG4 0          /*!                Порты без очереди сообщений(4) */
#define KERNEL_SPORT_NAME4 ""           /*!                   Имя(4)              */
#define KERNEL_SPORT_DIRECT4 SOURCE     /*!                   Направление передачи данных(4) */
#define KERNEL_SPORT_BUFSIZE4 0         /*!                   Размер буфера(4)    */
#define KERNEL_SPORT_REFRESH4 0         /*!                   Период обновления(4) */
#define KERNEL_SPORT_CONFIG5 0          /*!                Порты без очереди сообщений(5) */
#define KERNEL_SPORT_NAME5 ""           /*!                   Имя(5)              */
#define KERNEL_SPORT_DIRECT5 SOURCE     /*!                   Направление передачи данных(5) */
#define KERNEL_SPORT_BUFSIZE5 0         /*!                   Размер буфера(5)    */
#define KERNEL_SPORT_REFRESH5 0         /*!                   Период обновления(5) */
#define KERNEL_SPORT_CONFIG6 0          /*!                Порты без очереди сообщений(6) */
#define KERNEL_SPORT_NAME6 ""           /*!                   Имя(6)              */
#define KERNEL_SPORT_DIRECT6 SOURCE     /*!                   Направление передачи данных(6) */
#define KERNEL_SPORT_BUFSIZE6 0         /*!                   Размер буфера(6)    */
#define KERNEL_SPORT_REFRESH6 0         /*!                   Период обновления(6) */
#define KERNEL_SPORT_CONFIG7 0          /*!                Порты без очереди сообщений(7) */
#define KERNEL_SPORT_NAME7 ""           /*!                   Имя(7)              */
#define KERNEL_SPORT_DIRECT7 SOURCE     /*!                   Направление передачи данных(7) */
#define KERNEL_SPORT_BUFSIZE7 0         /*!                   Размер буфера(7)    */
#define KERNEL_SPORT_REFRESH7 0         /*!                   Период обновления(7) */
#define KERNEL_SPORT_CONFIG8 0          /*!                Порты без очереди сообщений(8) */
#define KERNEL_SPORT_NAME8 ""           /*!                   Имя(8)              */
#define KERNEL_SPORT_DIRECT8 SOURCE     /*!                   Направление передачи данных(8) */
#define KERNEL_SPORT_BUFSIZE8 0         /*!                   Размер буфера(8)    */
#define KERNEL_SPORT_REFRESH8 0         /*!                   Период обновления(8) */
#define KERNEL_SPORT_CONFIG9 0          /*!                Порты без очереди сообщений(9) */
#define KERNEL_SPORT_NAME9 ""           /*!                   Имя(9)              */
#define KERNEL_SPORT_DIRECT9 SOURCE     /*!                   Направление передачи данных(9) */
#define KERNEL_SPORT_BUFSIZE9 0         /*!                   Размер буфера(9)    */
#define KERNEL_SPORT_REFRESH9 0         /*!                   Период обновления(9) */
#define KERNEL_SPORT_CONFIG10 0         /*!                Порты без очереди сообщений(10) */
#define KERNEL_SPORT_NAME10 ""          /*!                   Имя(10)             */
#define KERNEL_SPORT_DIRECT10 SOURCE    /*!                   Направление передачи данных(10) */
#define KERNEL_SPORT_BUFSIZE10 0        /*!                   Размер буфера(10)   */
#define KERNEL_SPORT_REFRESH10 0        /*!                   Период обновления(10) */
#define KERNEL_SPORT_CONFIG11 0         /*!                Порты без очереди сообщений(11) */
#define KERNEL_SPORT_NAME11 ""          /*!                   Имя(11)             */
#define KERNEL_SPORT_DIRECT11 SOURCE    /*!                   Направление передачи данных(11) */
#define KERNEL_SPORT_BUFSIZE11 0        /*!                   Размер буфера(11)   */
#define KERNEL_SPORT_REFRESH11 0        /*!                   Период обновления(11) */
#define KERNEL_SPORT_CONFIG12 0         /*!                Порты без очереди сообщений(12) */
#define KERNEL_SPORT_NAME12 ""          /*!                   Имя(12)             */
#define KERNEL_SPORT_DIRECT12 SOURCE    /*!                   Направление передачи данных(12) */
#define KERNEL_SPORT_BUFSIZE12 0        /*!                   Размер буфера(12)   */
#define KERNEL_SPORT_REFRESH12 0        /*!                   Период обновления(12) */
#define KERNEL_SPORT_CONFIG13 0         /*!                Порты без очереди сообщений(13) */
#define KERNEL_SPORT_NAME13 ""          /*!                   Имя(13)             */
#define KERNEL_SPORT_DIRECT13 SOURCE    /*!                   Направление передачи данных(13) */
#define KERNEL_SPORT_BUFSIZE13 0        /*!                   Размер буфера(13)   */
#define KERNEL_SPORT_REFRESH13 0        /*!                   Период обновления(13) */
#define KERNEL_SPORT_CONFIG14 0         /*!                Порты без очереди сообщений(14) */
#define KERNEL_SPORT_NAME14 ""          /*!                   Имя(14)             */
#define KERNEL_SPORT_DIRECT14 SOURCE    /*!                   Направление передачи данных(14) */
#define KERNEL_SPORT_BUFSIZE14 0        /*!                   Размер буфера(14)   */
#define KERNEL_SPORT_REFRESH14 0        /*!                   Период обновления(14) */
#define KERNEL_SPORT_CONFIG15 0         /*!                Порты без очереди сообщений(15) */
#define KERNEL_SPORT_NAME15 ""          /*!                   Имя(15)             */
#define KERNEL_SPORT_DIRECT15 SOURCE    /*!                   Направление передачи данных(15) */
#define KERNEL_SPORT_BUFSIZE15 0        /*!                   Размер буфера(15)   */
#define KERNEL_SPORT_REFRESH15 0        /*!                   Период обновления(15) */
#define KERNEL_SPORT_CONFIG16 0         /*!                Порты без очереди сообщений(16) */
#define KERNEL_SPORT_NAME16 ""          /*!                   Имя(16)             */
#define KERNEL_SPORT_DIRECT16 SOURCE    /*!                   Направление передачи данных(16) */
#define KERNEL_SPORT_BUFSIZE16 0        /*!                   Размер буфера(16)   */
#define KERNEL_SPORT_REFRESH16 0        /*!                   Период обновления(16) */
#define KERNEL_SPORT_CONFIG17 0         /*!                Порты без очереди сообщений(17) */
#define KERNEL_SPORT_NAME17 ""          /*!                   Имя(17)             */
#define KERNEL_SPORT_DIRECT17 SOURCE    /*!                   Направление передачи данных(17) */
#define KERNEL_SPORT_BUFSIZE17 0        /*!                   Размер буфера(17)   */
#define KERNEL_SPORT_REFRESH17 0        /*!                   Период обновления(17) */
#define KERNEL_SPORT_CONFIG18 0         /*!                Порты без очереди сообщений(18) */
#define KERNEL_SPORT_NAME18 ""          /*!                   Имя(18)             */
#define KERNEL_SPORT_DIRECT18 SOURCE    /*!                   Направление передачи данных(18) */
#define KERNEL_SPORT_BUFSIZE18 0        /*!                   Размер буфера(18)   */
#define KERNEL_SPORT_REFRESH18 0        /*!                   Период обновления(18) */
#define KERNEL_SPORT_CONFIG19 0         /*!                Порты без очереди сообщений(19) */
#define KERNEL_SPORT_NAME19 ""          /*!                   Имя(19)             */
#define KERNEL_SPORT_DIRECT19 SOURCE    /*!                   Направление передачи данных(19) */
#define KERNEL_SPORT_BUFSIZE19 0        /*!                   Размер буфера(19)   */
#define KERNEL_SPORT_REFRESH19 0        /*!                   Период обновления(19) */
#define KERNEL_SPORT_CONFIG20 0         /*!                Порты без очереди сообщений(20) */
#define KERNEL_SPORT_NAME20 ""          /*!                   Имя(20)             */
#define KERNEL_SPORT_DIRECT20 SOURCE    /*!                   Направление передачи данных(20) */
#define KERNEL_SPORT_BUFSIZE20 0        /*!                   Размер буфера(20)   */
#define KERNEL_SPORT_REFRESH20 0        /*!                   Период обновления(20) */
#define KERNEL_SPORT_CONFIG21 0         /*!                Порты без очереди сообщений(21) */
#define KERNEL_SPORT_NAME21 ""          /*!                   Имя(21)             */
#define KERNEL_SPORT_DIRECT21 SOURCE    /*!                   Направление передачи данных(21) */
#define KERNEL_SPORT_BUFSIZE21 0        /*!                   Размер буфера(21)   */
#define KERNEL_SPORT_REFRESH21 0        /*!                   Период обновления(21) */
#define KERNEL_SPORT_CONFIG22 0         /*!                Порты без очереди сообщений(22) */
#define KERNEL_SPORT_NAME22 ""          /*!                   Имя(22)             */
#define KERNEL_SPORT_DIRECT22 SOURCE    /*!                   Направление передачи данных(22) */
#define KERNEL_SPORT_BUFSIZE22 0        /*!                   Размер буфера(22)   */
#define KERNEL_SPORT_REFRESH22 0        /*!                   Период обновления(22) */
#define KERNEL_SPORT_CONFIG23 0         /*!                Порты без очереди сообщений(23) */
#define KERNEL_SPORT_NAME23 ""          /*!                   Имя(23)             */
#define KERNEL_SPORT_DIRECT23 SOURCE    /*!                   Направление передачи данных(23) */
#define KERNEL_SPORT_BUFSIZE23 0        /*!                   Размер буфера(23)   */
#define KERNEL_SPORT_REFRESH23 0        /*!                   Период обновления(23) */
#define KERNEL_SPORT_CONFIG24 0         /*!                Порты без очереди сообщений(24) */
#define KERNEL_SPORT_NAME24 ""          /*!                   Имя(24)             */
#define KERNEL_SPORT_DIRECT24 SOURCE    /*!                   Направление передачи данных(24) */
#define KERNEL_SPORT_BUFSIZE24 0        /*!                   Размер буфера(24)   */
#define KERNEL_SPORT_REFRESH24 0        /*!                   Период обновления(24) */
#define KERNEL_SPORT_CONFIG25 0         /*!                Порты без очереди сообщений(25) */
#define KERNEL_SPORT_NAME25 ""          /*!                   Имя(25)             */
#define KERNEL_SPORT_DIRECT25 SOURCE    /*!                   Направление передачи данных(25) */
#define KERNEL_SPORT_BUFSIZE25 0        /*!                   Размер буфера(25)   */
#define KERNEL_SPORT_REFRESH25 0        /*!                   Период обновления(25) */
#define KERNEL_SPORT_CONFIG26 0         /*!                Порты без очереди сообщений(26) */
#define KERNEL_SPORT_NAME26 ""          /*!                   Имя(26)             */
#define KERNEL_SPORT_DIRECT26 SOURCE    /*!                   Направление передачи данных(26) */
#define KERNEL_SPORT_BUFSIZE26 0        /*!                   Размер буфера(26)   */
#define KERNEL_SPORT_REFRESH26 0        /*!                   Период обновления(26) */
#define KERNEL_SPORT_CONFIG27 0         /*!                Порты без очереди сообщений(27) */
#define KERNEL_SPORT_NAME27 ""          /*!                   Имя(27)             */
#define KERNEL_SPORT_DIRECT27 SOURCE    /*!                   Направление передачи данных(27) */
#define KERNEL_SPORT_BUFSIZE27 0        /*!                   Размер буфера(27)   */
#define KERNEL_SPORT_REFRESH27 0        /*!                   Период обновления(27) */
#define KERNEL_SPORT_CONFIG28 0         /*!                Порты без очереди сообщений(28) */
#define KERNEL_SPORT_NAME28 ""          /*!                   Имя(28)             */
#define KERNEL_SPORT_DIRECT28 SOURCE    /*!                   Направление передачи данных(28) */
#define KERNEL_SPORT_BUFSIZE28 0        /*!                   Размер буфера(28)   */
#define KERNEL_SPORT_REFRESH28 0        /*!                   Период обновления(28) */
#define KERNEL_SPORT_CONFIG29 0         /*!                Порты без очереди сообщений(29) */
#define KERNEL_SPORT_NAME29 ""          /*!                   Имя(29)             */
#define KERNEL_SPORT_DIRECT29 SOURCE    /*!                   Направление передачи данных(29) */
#define KERNEL_SPORT_BUFSIZE29 0        /*!                   Размер буфера(29)   */
#define KERNEL_SPORT_REFRESH29 0        /*!                   Период обновления(29) */
#define KERNEL_SPORT_CONFIG30 0         /*!                Порты без очереди сообщений(30) */
#define KERNEL_SPORT_NAME30 ""          /*!                   Имя(30)             */
#define KERNEL_SPORT_DIRECT30 SOURCE    /*!                   Направление передачи данных(30) */
#define KERNEL_SPORT_BUFSIZE30 0        /*!                   Размер буфера(30)   */
#define KERNEL_SPORT_REFRESH30 0        /*!                   Период обновления(30) */
#define KERNEL_SPORT_CONFIG31 0         /*!                Порты без очереди сообщений(31) */
#define KERNEL_SPORT_NAME31 ""          /*!                   Имя(31)             */
#define KERNEL_SPORT_DIRECT31 SOURCE    /*!                   Направление передачи данных(31) */
#define KERNEL_SPORT_BUFSIZE31 0        /*!                   Размер буфера(31)   */
#define KERNEL_SPORT_REFRESH31 0        /*!                   Период обновления(31) */
#define CONNECTION_CONFIG 1             /*!             Таблица связей            */
#define CONNECTION_CONFIG0 1            /*!             Таблица связей(0)         */
#define CONNECTION_CHANNEL_ID0 1        /*!                Идентификатор канала(0) */
                                        /*!                Имя(0)                 */
#define CONNECTION_CHANNEL_NAME0 "channel1"
#define CONNECTION_PORT_NAME0 "port1"   /*!                Имя порта(0)           */
#define CONNECTION_DIRECT0 SOURCE       /*!                Направление передачи данных(0) */
#define CONNECTION_CPU0 0               /*!                Номер модуля-партнёра(0) */
#define CONNECTION_NLINK0 1             /*!                Номер логической связи(0) */
#define CONNECTION_CONFIG1 0            /*!             Таблица связей(1)         */
#define CONNECTION_CHANNEL_ID1 1        /*!                Идентификатор канала(1) */
                                        /*!                Имя(1)                 */
#define CONNECTION_CHANNEL_NAME1 "channel1"
#define CONNECTION_PORT_NAME1 "port1"   /*!                Имя порта(1)           */
#define CONNECTION_DIRECT1 SOURCE       /*!                Направление передачи данных(1) */
#define CONNECTION_CPU1 0               /*!                Номер модуля-партнёра(1) */
#define CONNECTION_NLINK1 1             /*!                Номер логической связи(1) */
#define CONNECTION_CONFIG2 0            /*!             Таблица связей(2)         */
#define CONNECTION_CHANNEL_ID2 1        /*!                Идентификатор канала(2) */
                                        /*!                Имя(2)                 */
#define CONNECTION_CHANNEL_NAME2 "channel1"
#define CONNECTION_PORT_NAME2 "port1"   /*!                Имя порта(2)           */
#define CONNECTION_DIRECT2 SOURCE       /*!                Направление передачи данных(2) */
#define CONNECTION_CPU2 0               /*!                Номер модуля-партнёра(2) */
#define CONNECTION_NLINK2 1             /*!                Номер логической связи(2) */
#define CONNECTION_CONFIG3 0            /*!             Таблица связей(3)         */
#define CONNECTION_CHANNEL_ID3 1        /*!                Идентификатор канала(3) */
                                        /*!                Имя(3)                 */
#define CONNECTION_CHANNEL_NAME3 "channel1"
#define CONNECTION_PORT_NAME3 "port1"   /*!                Имя порта(3)           */
#define CONNECTION_DIRECT3 SOURCE       /*!                Направление передачи данных(3) */
#define CONNECTION_CPU3 0               /*!                Номер модуля-партнёра(3) */
#define CONNECTION_NLINK3 1             /*!                Номер логической связи(3) */
#define CONNECTION_CONFIG4 0            /*!             Таблица связей(4)         */
#define CONNECTION_CHANNEL_ID4 1        /*!                Идентификатор канала(4) */
                                        /*!                Имя(4)                 */
#define CONNECTION_CHANNEL_NAME4 "channel1"
#define CONNECTION_PORT_NAME4 "port1"   /*!                Имя порта(4)           */
#define CONNECTION_DIRECT4 SOURCE       /*!                Направление передачи данных(4) */
#define CONNECTION_CPU4 0               /*!                Номер модуля-партнёра(4) */
#define CONNECTION_NLINK4 1             /*!                Номер логической связи(4) */
#define CONNECTION_CONFIG5 0            /*!             Таблица связей(5)         */
#define CONNECTION_CHANNEL_ID5 1        /*!                Идентификатор канала(5) */
                                        /*!                Имя(5)                 */
#define CONNECTION_CHANNEL_NAME5 "channel1"
#define CONNECTION_PORT_NAME5 "port1"   /*!                Имя порта(5)           */
#define CONNECTION_DIRECT5 SOURCE       /*!                Направление передачи данных(5) */
#define CONNECTION_CPU5 0               /*!                Номер модуля-партнёра(5) */
#define CONNECTION_NLINK5 1             /*!                Номер логической связи(5) */
#define CONNECTION_CONFIG6 0            /*!             Таблица связей(6)         */
#define CONNECTION_CHANNEL_ID6 1        /*!                Идентификатор канала(6) */
                                        /*!                Имя(6)                 */
#define CONNECTION_CHANNEL_NAME6 "channel1"
#define CONNECTION_PORT_NAME6 "port1"   /*!                Имя порта(6)           */
#define CONNECTION_DIRECT6 SOURCE       /*!                Направление передачи данных(6) */
#define CONNECTION_CPU6 0               /*!                Номер модуля-партнёра(6) */
#define CONNECTION_NLINK6 1             /*!                Номер логической связи(6) */
#define CONNECTION_CONFIG7 0            /*!             Таблица связей(7)         */
#define CONNECTION_CHANNEL_ID7 1        /*!                Идентификатор канала(7) */
                                        /*!                Имя(7)                 */
#define CONNECTION_CHANNEL_NAME7 "channel1"
#define CONNECTION_PORT_NAME7 "port1"   /*!                Имя порта(7)           */
#define CONNECTION_DIRECT7 SOURCE       /*!                Направление передачи данных(7) */
#define CONNECTION_CPU7 0               /*!                Номер модуля-партнёра(7) */
#define CONNECTION_NLINK7 1             /*!                Номер логической связи(7) */
#define CONNECTION_CONFIG8 0            /*!             Таблица связей(8)         */
#define CONNECTION_CHANNEL_ID8 1        /*!                Идентификатор канала(8) */
                                        /*!                Имя(8)                 */
#define CONNECTION_CHANNEL_NAME8 "channel1"
#define CONNECTION_PORT_NAME8 "port1"   /*!                Имя порта(8)           */
#define CONNECTION_DIRECT8 SOURCE       /*!                Направление передачи данных(8) */
#define CONNECTION_CPU8 0               /*!                Номер модуля-партнёра(8) */
#define CONNECTION_NLINK8 1             /*!                Номер логической связи(8) */
#define CONNECTION_CONFIG9 0            /*!             Таблица связей(9)         */
#define CONNECTION_CHANNEL_ID9 1        /*!                Идентификатор канала(9) */
                                        /*!                Имя(9)                 */
#define CONNECTION_CHANNEL_NAME9 "channel1"
#define CONNECTION_PORT_NAME9 "port1"   /*!                Имя порта(9)           */
#define CONNECTION_DIRECT9 SOURCE       /*!                Направление передачи данных(9) */
#define CONNECTION_CPU9 0               /*!                Номер модуля-партнёра(9) */
#define CONNECTION_NLINK9 1             /*!                Номер логической связи(9) */
#define CONNECTION_CONFIG10 0           /*!             Таблица связей(10)        */
#define CONNECTION_CHANNEL_ID10 1       /*!                Идентификатор канала(10) */
                                        /*!                Имя(10)                */
#define CONNECTION_CHANNEL_NAME10 "channel1"
#define CONNECTION_PORT_NAME10 "port1"  /*!                Имя порта(10)          */
#define CONNECTION_DIRECT10 SOURCE      /*!                Направление передачи данных(10) */
#define CONNECTION_CPU10 0              /*!                Номер модуля-партнёра(10) */
#define CONNECTION_NLINK10 1            /*!                Номер логической связи(10) */
#define CONNECTION_CONFIG11 0           /*!             Таблица связей(11)        */
#define CONNECTION_CHANNEL_ID11 1       /*!                Идентификатор канала(11) */
                                        /*!                Имя(11)                */
#define CONNECTION_CHANNEL_NAME11 "channel1"
#define CONNECTION_PORT_NAME11 "port1"  /*!                Имя порта(11)          */
#define CONNECTION_DIRECT11 SOURCE      /*!                Направление передачи данных(11) */
#define CONNECTION_CPU11 0              /*!                Номер модуля-партнёра(11) */
#define CONNECTION_NLINK11 1            /*!                Номер логической связи(11) */
#define CONNECTION_CONFIG12 0           /*!             Таблица связей(12)        */
#define CONNECTION_CHANNEL_ID12 1       /*!                Идентификатор канала(12) */
                                        /*!                Имя(12)                */
#define CONNECTION_CHANNEL_NAME12 "channel1"
#define CONNECTION_PORT_NAME12 "port1"  /*!                Имя порта(12)          */
#define CONNECTION_DIRECT12 SOURCE      /*!                Направление передачи данных(12) */
#define CONNECTION_CPU12 0              /*!                Номер модуля-партнёра(12) */
#define CONNECTION_NLINK12 1            /*!                Номер логической связи(12) */
#define CONNECTION_CONFIG13 0           /*!             Таблица связей(13)        */
#define CONNECTION_CHANNEL_ID13 1       /*!                Идентификатор канала(13) */
                                        /*!                Имя(13)                */
#define CONNECTION_CHANNEL_NAME13 "channel1"
#define CONNECTION_PORT_NAME13 "port1"  /*!                Имя порта(13)          */
#define CONNECTION_DIRECT13 SOURCE      /*!                Направление передачи данных(13) */
#define CONNECTION_CPU13 0              /*!                Номер модуля-партнёра(13) */
#define CONNECTION_NLINK13 1            /*!                Номер логической связи(13) */
#define CONNECTION_CONFIG14 0           /*!             Таблица связей(14)        */
#define CONNECTION_CHANNEL_ID14 1       /*!                Идентификатор канала(14) */
                                        /*!                Имя(14)                */
#define CONNECTION_CHANNEL_NAME14 "channel1"
#define CONNECTION_PORT_NAME14 "port1"  /*!                Имя порта(14)          */
#define CONNECTION_DIRECT14 SOURCE      /*!                Направление передачи данных(14) */
#define CONNECTION_CPU14 0              /*!                Номер модуля-партнёра(14) */
#define CONNECTION_NLINK14 1            /*!                Номер логической связи(14) */
#define CONNECTION_CONFIG15 0           /*!             Таблица связей(15)        */
#define CONNECTION_CHANNEL_ID15 1       /*!                Идентификатор канала(15) */
                                        /*!                Имя(15)                */
#define CONNECTION_CHANNEL_NAME15 "channel1"
#define CONNECTION_PORT_NAME15 "port1"  /*!                Имя порта(15)          */
#define CONNECTION_DIRECT15 SOURCE      /*!                Направление передачи данных(15) */
#define CONNECTION_CPU15 0              /*!                Номер модуля-партнёра(15) */
#define CONNECTION_NLINK15 1            /*!                Номер логической связи(15) */
#define CONNECTION_CONFIG16 0           /*!             Таблица связей(16)        */
#define CONNECTION_CHANNEL_ID16 1       /*!                Идентификатор канала(16) */
                                        /*!                Имя(16)                */
#define CONNECTION_CHANNEL_NAME16 "channel1"
#define CONNECTION_PORT_NAME16 "port1"  /*!                Имя порта(16)          */
#define CONNECTION_DIRECT16 SOURCE      /*!                Направление передачи данных(16) */
#define CONNECTION_CPU16 0              /*!                Номер модуля-партнёра(16) */
#define CONNECTION_NLINK16 1            /*!                Номер логической связи(16) */
#define CONNECTION_CONFIG17 0           /*!             Таблица связей(17)        */
#define CONNECTION_CHANNEL_ID17 1       /*!                Идентификатор канала(17) */
                                        /*!                Имя(17)                */
#define CONNECTION_CHANNEL_NAME17 "channel1"
#define CONNECTION_PORT_NAME17 "port1"  /*!                Имя порта(17)          */
#define CONNECTION_DIRECT17 SOURCE      /*!                Направление передачи данных(17) */
#define CONNECTION_CPU17 0              /*!                Номер модуля-партнёра(17) */
#define CONNECTION_NLINK17 1            /*!                Номер логической связи(17) */
#define CONNECTION_CONFIG18 0           /*!             Таблица связей(18)        */
#define CONNECTION_CHANNEL_ID18 1       /*!                Идентификатор канала(18) */
                                        /*!                Имя(18)                */
#define CONNECTION_CHANNEL_NAME18 "channel1"
#define CONNECTION_PORT_NAME18 "port1"  /*!                Имя порта(18)          */
#define CONNECTION_DIRECT18 SOURCE      /*!                Направление передачи данных(18) */
#define CONNECTION_CPU18 0              /*!                Номер модуля-партнёра(18) */
#define CONNECTION_NLINK18 1            /*!                Номер логической связи(18) */
#define CONNECTION_CONFIG19 0           /*!             Таблица связей(19)        */
#define CONNECTION_CHANNEL_ID19 1       /*!                Идентификатор канала(19) */
                                        /*!                Имя(19)                */
#define CONNECTION_CHANNEL_NAME19 "channel1"
#define CONNECTION_PORT_NAME19 "port1"  /*!                Имя порта(19)          */
#define CONNECTION_DIRECT19 SOURCE      /*!                Направление передачи данных(19) */
#define CONNECTION_CPU19 0              /*!                Номер модуля-партнёра(19) */
#define CONNECTION_NLINK19 1            /*!                Номер логической связи(19) */
#define CONNECTION_CONFIG20 0           /*!             Таблица связей(20)        */
#define CONNECTION_CHANNEL_ID20 1       /*!                Идентификатор канала(20) */
                                        /*!                Имя(20)                */
#define CONNECTION_CHANNEL_NAME20 "channel1"
#define CONNECTION_PORT_NAME20 "port1"  /*!                Имя порта(20)          */
#define CONNECTION_DIRECT20 SOURCE      /*!                Направление передачи данных(20) */
#define CONNECTION_CPU20 0              /*!                Номер модуля-партнёра(20) */
#define CONNECTION_NLINK20 1            /*!                Номер логической связи(20) */
#define CONNECTION_CONFIG21 0           /*!             Таблица связей(21)        */
#define CONNECTION_CHANNEL_ID21 1       /*!                Идентификатор канала(21) */
                                        /*!                Имя(21)                */
#define CONNECTION_CHANNEL_NAME21 "channel1"
#define CONNECTION_PORT_NAME21 "port1"  /*!                Имя порта(21)          */
#define CONNECTION_DIRECT21 SOURCE      /*!                Направление передачи данных(21) */
#define CONNECTION_CPU21 0              /*!                Номер модуля-партнёра(21) */
#define CONNECTION_NLINK21 1            /*!                Номер логической связи(21) */
#define CONNECTION_CONFIG22 0           /*!             Таблица связей(22)        */
#define CONNECTION_CHANNEL_ID22 1       /*!                Идентификатор канала(22) */
                                        /*!                Имя(22)                */
#define CONNECTION_CHANNEL_NAME22 "channel1"
#define CONNECTION_PORT_NAME22 "port1"  /*!                Имя порта(22)          */
#define CONNECTION_DIRECT22 SOURCE      /*!                Направление передачи данных(22) */
#define CONNECTION_CPU22 0              /*!                Номер модуля-партнёра(22) */
#define CONNECTION_NLINK22 1            /*!                Номер логической связи(22) */
#define CONNECTION_CONFIG23 0           /*!             Таблица связей(23)        */
#define CONNECTION_CHANNEL_ID23 1       /*!                Идентификатор канала(23) */
                                        /*!                Имя(23)                */
#define CONNECTION_CHANNEL_NAME23 "channel1"
#define CONNECTION_PORT_NAME23 "port1"  /*!                Имя порта(23)          */
#define CONNECTION_DIRECT23 SOURCE      /*!                Направление передачи данных(23) */
#define CONNECTION_CPU23 0              /*!                Номер модуля-партнёра(23) */
#define CONNECTION_NLINK23 1            /*!                Номер логической связи(23) */
#define CONNECTION_CONFIG24 0           /*!             Таблица связей(24)        */
#define CONNECTION_CHANNEL_ID24 1       /*!                Идентификатор канала(24) */
                                        /*!                Имя(24)                */
#define CONNECTION_CHANNEL_NAME24 "channel1"
#define CONNECTION_PORT_NAME24 "port1"  /*!                Имя порта(24)          */
#define CONNECTION_DIRECT24 SOURCE      /*!                Направление передачи данных(24) */
#define CONNECTION_CPU24 0              /*!                Номер модуля-партнёра(24) */
#define CONNECTION_NLINK24 1            /*!                Номер логической связи(24) */
#define CONNECTION_CONFIG25 0           /*!             Таблица связей(25)        */
#define CONNECTION_CHANNEL_ID25 1       /*!                Идентификатор канала(25) */
                                        /*!                Имя(25)                */
#define CONNECTION_CHANNEL_NAME25 "channel1"
#define CONNECTION_PORT_NAME25 "port1"  /*!                Имя порта(25)          */
#define CONNECTION_DIRECT25 SOURCE      /*!                Направление передачи данных(25) */
#define CONNECTION_CPU25 0              /*!                Номер модуля-партнёра(25) */
#define CONNECTION_NLINK25 1            /*!                Номер логической связи(25) */
#define CONNECTION_CONFIG26 0           /*!             Таблица связей(26)        */
#define CONNECTION_CHANNEL_ID26 1       /*!                Идентификатор канала(26) */
                                        /*!                Имя(26)                */
#define CONNECTION_CHANNEL_NAME26 "channel1"
#define CONNECTION_PORT_NAME26 "port1"  /*!                Имя порта(26)          */
#define CONNECTION_DIRECT26 SOURCE      /*!                Направление передачи данных(26) */
#define CONNECTION_CPU26 0              /*!                Номер модуля-партнёра(26) */
#define CONNECTION_NLINK26 1            /*!                Номер логической связи(26) */
#define CONNECTION_CONFIG27 0           /*!             Таблица связей(27)        */
#define CONNECTION_CHANNEL_ID27 1       /*!                Идентификатор канала(27) */
                                        /*!                Имя(27)                */
#define CONNECTION_CHANNEL_NAME27 "channel1"
#define CONNECTION_PORT_NAME27 "port1"  /*!                Имя порта(27)          */
#define CONNECTION_DIRECT27 SOURCE      /*!                Направление передачи данных(27) */
#define CONNECTION_CPU27 0              /*!                Номер модуля-партнёра(27) */
#define CONNECTION_NLINK27 1            /*!                Номер логической связи(27) */
#define CONNECTION_CONFIG28 0           /*!             Таблица связей(28)        */
#define CONNECTION_CHANNEL_ID28 1       /*!                Идентификатор канала(28) */
                                        /*!                Имя(28)                */
#define CONNECTION_CHANNEL_NAME28 "channel1"
#define CONNECTION_PORT_NAME28 "port1"  /*!                Имя порта(28)          */
#define CONNECTION_DIRECT28 SOURCE      /*!                Направление передачи данных(28) */
#define CONNECTION_CPU28 0              /*!                Номер модуля-партнёра(28) */
#define CONNECTION_NLINK28 1            /*!                Номер логической связи(28) */
#define CONNECTION_CONFIG29 0           /*!             Таблица связей(29)        */
#define CONNECTION_CHANNEL_ID29 1       /*!                Идентификатор канала(29) */
                                        /*!                Имя(29)                */
#define CONNECTION_CHANNEL_NAME29 "channel1"
#define CONNECTION_PORT_NAME29 "port1"  /*!                Имя порта(29)          */
#define CONNECTION_DIRECT29 SOURCE      /*!                Направление передачи данных(29) */
#define CONNECTION_CPU29 0              /*!                Номер модуля-партнёра(29) */
#define CONNECTION_NLINK29 1            /*!                Номер логической связи(29) */
#define CONNECTION_CONFIG30 0           /*!             Таблица связей(30)        */
#define CONNECTION_CHANNEL_ID30 1       /*!                Идентификатор канала(30) */
                                        /*!                Имя(30)                */
#define CONNECTION_CHANNEL_NAME30 "channel1"
#define CONNECTION_PORT_NAME30 "port1"  /*!                Имя порта(30)          */
#define CONNECTION_DIRECT30 SOURCE      /*!                Направление передачи данных(30) */
#define CONNECTION_CPU30 0              /*!                Номер модуля-партнёра(30) */
#define CONNECTION_NLINK30 1            /*!                Номер логической связи(30) */
#define CONNECTION_CONFIG31 0           /*!             Таблица связей(31)        */
#define CONNECTION_CHANNEL_ID31 1       /*!                Идентификатор канала(31) */
                                        /*!                Имя(31)                */
#define CONNECTION_CHANNEL_NAME31 "channel1"
#define CONNECTION_PORT_NAME31 "port1"  /*!                Имя порта(31)          */
#define CONNECTION_DIRECT31 SOURCE      /*!                Направление передачи данных(31) */
#define CONNECTION_CPU31 0              /*!                Номер модуля-партнёра(31) */
#define CONNECTION_NLINK31 1            /*!                Номер логической связи(31) */
#define OC2000_MISC 1                   /*!       Дополнительные возможности      */
                                        /*!          Переменные окружения         */
#define ENVIRONMENT "TZ=MSK-3MSS-4,M3.5.0,M10.5.0"
#define OC2000_APPLICATION 1            /*!       Подключение прикладных программ */
#undef INCLUDE_LOGO                     /*!          Вывод заставки               */
#undef INCLUDE_DEMO_THREAD              /*!          Демонстрационная программа   */
#define INCLUDE_USER_THREAD 1           /*!          Прикладная программа         */
#define USER_THREAD_ENTRY osThreadMSPO  /*!             Точка входа               */
#define USER_THREAD_PRIORITY 100        /*!             Приоритет корневого потока */
#define OC2000_CONFIG_D 1               /*!       Средства отладки                */
                                        /*!          Обработка исключений         */
#define DEFAULT_INTMODE INT_MODE_SUSPEND
#define INCLUDE_SYMTBL 1                /*!          Таблица символов             */
#define INCLUDE_LOADER 1                /*!          Динамический загрузчик       */
#undef INCLUDE_SHELL                    /*!          Командный интерпретатор      */
#define SHELL_PRIORITY 200              /*!             Приоритет                 */
#define SHELL_STACK_SIZE 32768          /*!             Размер стека              */
#define SHELL_STARTUP_SCRIPT ""         /*!             Командный файл            */
#undef INCLUDE_DEBUG                    /*!          Локальный отладчик           */
#define DEBUG_PRIORITY 236              /*!             Приоритет debug-демона    */
#undef INCLUDE_RDB                      /*!          Удаленный отладчик           */
#define RDBTHREAD_PRIORITY 235          /*!             Приоритет                 */
#define RDBFUNCTION_PRIORITY 50         /*!             Приоритет запускаемых потоков управления */
#define RDBFUNCTION_STACKSIZE 32768     /*!             Размер стека запускаемых потоков управления */
#undef INCLUDE_SHOW_OBJECTS             /*!          Команды вывода информации об объектах ОС */
#define INCLUDE_SHOW_MEMORY 1           /*!          Команды вывода содержимого памяти */
#undef INCLUDE_SHOW_NETWORK             /*!          Команды вывода информации о сети */
#undef MEMORY_CHECK                     /*!          Проверять корректность пула памяти */
#define MEMORY_ERROR_PROCESSIG 1        /*!             Реакция на ошибку         */
#define MEMORY_ERROR_SUSPEND 1          /*!                Приостановка потока    */
#undef MEMORY_ERROR_LOG                 /*!                Регистрация ошибки     */
#undef INCLUDE_SHOW_INIT                /*!          Сообщения об инициализации   */
#define INCLUDE_SYSLOG 1                /*!          Системный журнал             */
#define SYSLOG_PRIORITY 240             /*!             Приоритет syslog-демона   */
#define SYSLOG_BUF_SIZE_CFG 128         /*!             Размер буфера             */
#define SYSLOG_BUF_COUNT_CFG 100        /*!             Число буферов системного журнала */
#undef NOMESSAGE_HEADER                 /*!             Без заголовка             */
#define OUTPUT_MSG_TO console           /*!             Выводить сообщения в      */
#undef OUTPUT_MSG_CONSOLE               /*!             Дублировать вывод сообщений на консоль */
#undef MEM_FULL_POLICY                  /*!             При переполнении писать в начало */
#define NAME_OUTSTREAM ""               /*!             Имя файла                 */
#undef ERASE_MEMORY                     /*!             Очистить флэш память      */
#define NAME_FMEMORY "area_1"           /*!             Имя области флэш памяти   */
#define MOVE_MEMORY ""                  /*!             Переместить сообщения журнала в */
#define MOVE_TYPE begin                 /*!             Как выгружать сообщения в файл */
#define MOVE_MEMORY_SIZE 0x1000000      /*!             Размер файла              */
#define NAME_PMEMORY "SYSLOGMEM"        /*!             Имя области сохраняемой памяти */
#define SYSLOG_OPEN_FUNCTION none       /*!             Пользовательская функция открытия */
#define SYSLOG_OUTPUT_FUNCTION none     /*!             Пользовательская функция вывода */
#define SYSLOG_CLOSE_FUNCTION none      /*!             Пользовательская функция закрытия */
#define REPORT_ERRORS 1                 /*!          Что выводить при особой ситуации */
#define INCLUDE_REPORT_DISASS 1         /*!             Дизассемблирование окрестности точки ошибки */
#define INCLUDE_REPORT_BACKTR 1         /*!             Стек вызовов функций      */
#undef INCLUDE_REPORT_THREADS           /*!             Список потоков            */
#undef INCLUDE_REPORT_FLREGS            /*!             Плавающие регистры        */
#define HM_CHECK_OBJECTS 0              /*!             Проверить объекты ОС      */
#define HM_REPORT_USER 0                /*!             Пользовательская функция  */
#define HM_FUNTION_NAME none            /*!             Имя функции пользователя  */
#define SIZE_FPAREA 0                   /*!             Размер фрейма             */
#undef INCLUDE_PRSMEM                   /*!          Сохраняемая память           */
#define PRSMEM_ALLOC PRSMEM_ALLOC_AUTO  /*!             Метод выделения памяти    */
#define PRSMEM_ADDRESS 0                /*!             Адрес сохраняемой памяти  */
#define PRSMEM_PART_CONFIG 1            /*!             Таблица областей сохраняемой памяти */
#define PRSMEM_PART_CONFIG0 1           /*!             Таблица областей сохраняемой памяти(0) */
#define PRSMEM_PART_NAME0 "TRACEBUF"    /*!                Имя(0)                 */
#define PRSMEM_PART_SIZE0 409600        /*!                Размер(0)              */
#define PRSMEM_PART_CONFIG1 0           /*!             Таблица областей сохраняемой памяти(1) */
#define PRSMEM_PART_NAME1 "TRACELEGACY" /*!                Имя(1)                 */
#define PRSMEM_PART_SIZE1 1024          /*!                Размер(1)              */
#define PRSMEM_PART_CONFIG2 0           /*!             Таблица областей сохраняемой памяти(2) */
#define PRSMEM_PART_NAME2 "SYSLOGMEM"   /*!                Имя(2)                 */
#define PRSMEM_PART_SIZE2 262144        /*!                Размер(2)              */
#define PRSMEM_PART_CONFIG3 0           /*!             Таблица областей сохраняемой памяти(3) */
#define PRSMEM_PART_NAME3 ""            /*!                Имя(3)                 */
#define PRSMEM_PART_SIZE3 0             /*!                Размер(3)              */
#define PRSMEM_PART_CONFIG4 0           /*!             Таблица областей сохраняемой памяти(4) */
#define PRSMEM_PART_NAME4 ""            /*!                Имя(4)                 */
#define PRSMEM_PART_SIZE4 0             /*!                Размер(4)              */
#define PRSMEM_PART_CONFIG5 0           /*!             Таблица областей сохраняемой памяти(5) */
#define PRSMEM_PART_NAME5 ""            /*!                Имя(5)                 */
#define PRSMEM_PART_SIZE5 0             /*!                Размер(5)              */
#define PRSMEM_PART_CONFIG6 0           /*!             Таблица областей сохраняемой памяти(6) */
#define PRSMEM_PART_NAME6 ""            /*!                Имя(6)                 */
#define PRSMEM_PART_SIZE6 0             /*!                Размер(6)              */
#define PRSMEM_PART_CONFIG7 0           /*!             Таблица областей сохраняемой памяти(7) */
#define PRSMEM_PART_NAME7 ""            /*!                Имя(7)                 */
#define PRSMEM_PART_SIZE7 0             /*!                Размер(7)              */
#undef INCLUDE_FLASHMEM                 /*!          Флэш память                  */
#define FLASHMEM_PART_CONFIG 1          /*!             Области флэш памяти       */
#define FLASHMEM_PART_CONFIG0 1         /*!             Области флэш памяти(0)    */
#define FLASHMEM_PART_NAME0 "area_1"    /*!                Имя(0)                 */
#define FLASHMEM_PART_OFFSET0 0x0       /*!                Смещение(0)            */
#define FLASHMEM_PART_SIZE0 0           /*!                Число секторов(0)      */
#define FLASHMEM_PART_CONFIG1 0         /*!             Области флэш памяти(1)    */
#define FLASHMEM_PART_NAME1 "area_2"    /*!                Имя(1)                 */
#define FLASHMEM_PART_OFFSET1 0x0       /*!                Смещение(1)            */
#define FLASHMEM_PART_SIZE1 0           /*!                Число секторов(1)      */
#define FLASHMEM_PART_CONFIG2 0         /*!             Области флэш памяти(2)    */
#define FLASHMEM_PART_NAME2 "area_3"    /*!                Имя(2)                 */
#define FLASHMEM_PART_OFFSET2 0x0       /*!                Смещение(2)            */
#define FLASHMEM_PART_SIZE2 0           /*!                Число секторов(2)      */
#define FLASHMEM_PART_CONFIG3 0         /*!             Области флэш памяти(3)    */
#define FLASHMEM_PART_NAME3 "area_4"    /*!                Имя(3)                 */
#define FLASHMEM_PART_OFFSET3 0x0       /*!                Смещение(3)            */
#define FLASHMEM_PART_SIZE3 0           /*!                Число секторов(3)      */
#define FLASHMEM_PART_CONFIG4 0         /*!             Области флэш памяти(4)    */
#define FLASHMEM_PART_NAME4 "area_5"    /*!                Имя(4)                 */
#define FLASHMEM_PART_OFFSET4 0x0       /*!                Смещение(4)            */
#define FLASHMEM_PART_SIZE4 0           /*!                Число секторов(4)      */
#define FLASHMEM_PART_CONFIG5 0         /*!             Области флэш памяти(5)    */
#define FLASHMEM_PART_NAME5 "area_6"    /*!                Имя(5)                 */
#define FLASHMEM_PART_OFFSET5 0x0       /*!                Смещение(5)            */
#define FLASHMEM_PART_SIZE5 0           /*!                Число секторов(5)      */
#define FLASHMEM_PART_CONFIG6 0         /*!             Области флэш памяти(6)    */
#define FLASHMEM_PART_NAME6 "area_7"    /*!                Имя(6)                 */
#define FLASHMEM_PART_OFFSET6 0x0       /*!                Смещение(6)            */
#define FLASHMEM_PART_SIZE6 0           /*!                Число секторов(6)      */
#define FLASHMEM_PART_CONFIG7 0         /*!             Области флэш памяти(7)    */
#define FLASHMEM_PART_NAME7 "area_8"    /*!                Имя(7)                 */
#define FLASHMEM_PART_OFFSET7 0x0       /*!                Смещение(7)            */
#define FLASHMEM_PART_SIZE7 0           /*!                Число секторов(7)      */
#define INCLUDE_SYSSHELL 1              /*!          Выполнение команд оболочки   */
#define SYSSHELL_PRIORITY 202           /*!             Приоритет sysShell-демона */
#define SYSTEM_BUF_SIZE 128             /*!             Размер буфера             */
#define SYSTEM_BUF_COUNT 10             /*!             Количество буферов        */
#define SYSTEM_TO_SYSLOG 0              /*!             Перенаправлять вывод в системный журнал */
#define SYSTEM_PRINT 0                  /*!             Выводить команду          */
#define TRACE_NAME "trace"              /*!          Имя протокола                */
#undef TRACE_CREATE                     /*!          Создание протокола           */
#define TRACE_AUTOSTART 0               /*!          Автоматический старт         */
#define INCLUDE_TRACE_STREAM 1          /*!          INCLUDE_TRACE_STREAM         */
                                        /*!             Действия при переполнении буфера */
#define TRACE_STREAMFULLPOLICY POSIX_TRACE_LOOP
#define TRACE_BLOCKS_SERVICE 3          /*!          Число служебных блоков       */
#undef INCLUDE_TRACE_LOG                /*!          Создание файла протокола     */
#define TRACE_LOGSIZE 409600            /*!             Размер файла              */
                                        /*!             Действия при переполнении файла */
#define TRACE_LOGFULLPOLICY POSIX_TRACE_APPEND
#define TRACE_LOGPRIORITY 1             /*!             Приоритет потока обслуживания трассы  */
#define TRACE_LOGHIGHPRIORITY 188       /*!             Приоритет сервисного потока при аварии */
#define TRACE_LOG "/nfs/trace"          /*!          Имя файла                    */
#define TRACE_LOGROTATE 0               /*!          Максимальный номер файла     */
#define TRACE_MAXDATASIZE 100           /*!          Размер пользовательских данных */
#define CONFIG_TRACE_EXCEPTION 1        /*!          Протоколирование исключений  */
#define TRACE_EXCEPTION_FP 0            /*!             Floating point register   */
#define TRACE_EXCEPTION_CALLSTACK 0     /*!             Вывод стека вызовов       */
                                        /*!             Реакция на исключения     */
#define TRACE_EXCEPTIONPOLICY TRACE_EXCEPTION_NONE
#undef TRACE_CHANNEL_CONFIG             /*!          Трассировка каналов          */
#define TRACE_CHANNEL_CONFIG0 1         /*!          Трассировка каналов(0)       */
#define TRACE_CHANNEL_ID0 0             /*!             Идентификатор(0)          */
#define TRACE_CHANMSG_SIZE0 0           /*!             Размер(0)                 */
#define TRACE_CHANMSG_HDR0 0            /*!             Заголовок(0)              */
#define TRACE_CHANNEL_CONFIG1 0         /*!          Трассировка каналов(1)       */
#define TRACE_CHANNEL_ID1 0             /*!             Идентификатор(1)          */
#define TRACE_CHANMSG_SIZE1 0           /*!             Размер(1)                 */
#define TRACE_CHANMSG_HDR1 0            /*!             Заголовок(1)              */
#define TRACE_CHANNEL_CONFIG2 0         /*!          Трассировка каналов(2)       */
#define TRACE_CHANNEL_ID2 0             /*!             Идентификатор(2)          */
#define TRACE_CHANMSG_SIZE2 0           /*!             Размер(2)                 */
#define TRACE_CHANMSG_HDR2 0            /*!             Заголовок(2)              */
#define TRACE_CHANNEL_CONFIG3 0         /*!          Трассировка каналов(3)       */
#define TRACE_CHANNEL_ID3 0             /*!             Идентификатор(3)          */
#define TRACE_CHANMSG_SIZE3 0           /*!             Размер(3)                 */
#define TRACE_CHANMSG_HDR3 0            /*!             Заголовок(3)              */
#define TRACE_CHANNEL_CONFIG4 0         /*!          Трассировка каналов(4)       */
#define TRACE_CHANNEL_ID4 0             /*!             Идентификатор(4)          */
#define TRACE_CHANMSG_SIZE4 0           /*!             Размер(4)                 */
#define TRACE_CHANMSG_HDR4 0            /*!             Заголовок(4)              */
#define TRACE_CHANNEL_CONFIG5 0         /*!          Трассировка каналов(5)       */
#define TRACE_CHANNEL_ID5 0             /*!             Идентификатор(5)          */
#define TRACE_CHANMSG_SIZE5 0           /*!             Размер(5)                 */
#define TRACE_CHANMSG_HDR5 0            /*!             Заголовок(5)              */
#define TRACE_CHANNEL_CONFIG6 0         /*!          Трассировка каналов(6)       */
#define TRACE_CHANNEL_ID6 0             /*!             Идентификатор(6)          */
#define TRACE_CHANMSG_SIZE6 0           /*!             Размер(6)                 */
#define TRACE_CHANMSG_HDR6 0            /*!             Заголовок(6)              */
#define TRACE_CHANNEL_CONFIG7 0         /*!          Трассировка каналов(7)       */
#define TRACE_CHANNEL_ID7 0             /*!             Идентификатор(7)          */
#define TRACE_CHANMSG_SIZE7 0           /*!             Размер(7)                 */
#define TRACE_CHANMSG_HDR7 0            /*!             Заголовок(7)              */
#define TRACE_APPEND1 1                 /*!          TRACE_APPEND1                */
#define TRACE_BUFFER_SAVE 0             /*!             Сохранить предыдущую трассу */
#undef TRACE_LOAD_CONFIG                /*!          Загрузка конфигурации        */
                                        /*!             Имя конфигурационного файла */
#define TRACE_CONFIG_FILE "/nfs/configos.h"
                                        /*!             Имя файла конфигурации маски протоколирования событий */
#define TRACE_CONFIG_MASK_REGISTRATION "/nfs/trace.mask"
#undef TRACE_EVENT                      /*!          Фильтр событий               */
#undef TRACE_GLB_INTERRUPT              /*!             Прерывания                */
#define TRACE_INTERRUPT_ENTRY 1         /*!                Вход в прерывание      */
#define TRACE_INTERRUPT_EXIT 1          /*!                Выход из прерывания    */
#undef TRACE_GLB_EXCEPTION              /*!             Исключительные ситуации   */
#undef TRACE_GLB_THREADSWITCH           /*!             Переключение потоков      */
#define TRACE_THREADSWITCH 1            /*!                Переключение потоков   */
#define TRACE_THREADIDLE 1              /*!                Ожидание (простой)     */
#undef TRACE_GLB_THREADSTATUS           /*!             Изменение состояния потоков */
#define TRACE_THREADCREATE 1            /*!                Создание потока        */
#define TRACE_THREADEXIT 1              /*!                Завершение потока      */
#define TRACE_THREADACTIVATE 1          /*!                Переход в работоспособное состояние */
#define TRACE_THREADBLOCK 1             /*!                Переход в неработоспособное состояние */
#define TRACE_THREADSCHEDYIELD 1        /*!                sched_yield()          */
#define TRACE_THREADTIMEQUANTUM 1       /*!                Исчерпание кванта времени */
#define TRACE_THREADSUSPEND 1           /*!                Приостановка потока    */
#define TRACE_THREADRESUME 1            /*!                Возобновление работы потока */
#undef TRACE_GLB_SIGNAL                 /*!             Порождение и доставка сигналов */
#define TRACE_SIGGENERATION 1           /*!                Порождение сигнала     */
#define TRACE_SIGDELIVERY 1             /*!                Доставка сигнала       */
#define TRACE_SIGCATCHFUNC 1            /*!                Вызов функции обработки сигнала */
#define TRACE_SIGCATCHFUNCRET 1         /*!                Выход из функции обработки сигнала */
#define TRACE_SIGTHREADCANCEL 1         /*!                Получение запроса на удаление потока */
#undef TRACE_GLB_SIGWAIT                /*!             Ожидание сигналов         */
#define TRACE_SIGPAUSE 1                /*!                pause()                */
#define TRACE_SIGSUSPEND 1              /*!                sigsuspend()           */
#define TRACE_SIGTIMEDWAIT 1            /*!                sigtimedwait()         */
#define TRACE_SIGWAIT 1                 /*!                sigwait()              */
#define TRACE_SIGWAITINFO 1             /*!                sigwaitinfo()          */
#undef TRACE_GLB_MEMORY                 /*!             Распределение памяти      */
#define TRACE_MEMPOOLCREATE 1           /*!                memPoolCreate()        */
#define TRACE_MEMPOOLADD 1              /*!                memPoolAdd()           */
#define TRACE_MEMPOOLDELETE 1           /*!                memPoolDelete()        */
#define TRACE_MEMPOOLALLOC 1            /*!                Выделение памяти       */
#define TRACE_MEMPOOLFREE 1             /*!                Освобождение памяти    */
#undef TRACE_GLB_THREAD                 /*!             Функции управления потоками */
#define TRACE_THREADCANCEL 1            /*!                pthread_cancel()       */
#define TRACE_THREADJOIN 1              /*!                pthread_join()         */
#define TRACE_THREADSIGMASK 1           /*!                pthread_sigmask()      */
#define TRACE_THREADSETSCHEDPARAM 1     /*!                pthread_setschedparam() */
#define TRACE_THREADSETCANCELSTATE 1    /*!                pthread_setcancelstate() */
#define TRACE_THREADSETCANCELTYPE 1     /*!                pthread_setcanceltype() */
#define TRACE_THREADDETACH 1            /*!                pthread_detach()       */
#undef TRACE_GLB_RETURN                 /*!             Возврат управления        */
#undef TRACE_GLB_SEMAPHORE              /*!             Функции управления семафорами */
#define TRACE_SEMGETVALUE 1             /*!                sem_getvalue()         */
#define TRACE_SEMOPEN 1                 /*!                sem_open()             */
#define TRACE_SEMCLOSE 1                /*!                sem_close()            */
#define TRACE_SEMUNLINK 1               /*!                sem_unlink()           */
#define TRACE_SEMINIT 1                 /*!                sem_init()             */
#define TRACE_SEMDESTROY 1              /*!                sem_destroy()          */
#define TRACE_SEMPOST 1                 /*!                sem_post()             */
#define TRACE_DELAYEDSEMPOST 1          /*!                Отложенный sem_post    */
#define TRACE_SEMWAIT 1                 /*!                sem_wait()             */
#define TRACE_SEMTIMEDWAIT 1            /*!                sem_timedwait()        */
#define TRACE_SEMTRYWAIT 1              /*!                sem_trywait()          */
#undef TRACE_GLB_MUTEX                  /*!             Функции управления мьютексами */
#define TRACE_MUTEXDESTROY 1            /*!                pthread_mutex_destroy() */
#define TRACE_MUTEXINIT 1               /*!                pthread_mutex_init()   */
#define TRACE_MUTEXDSETPRIOCEILING 1    /*!                pthread_mutex_setprioceiling() */
#define TRACE_MUTEXGETPRIOCEILING 1     /*!                pthread_mutex_getprioceiling() */
#define TRACE_MUTEXLOCK 1               /*!                pthread_mutex_lock()   */
#define TRACE_MUTEXTRYLOCK 1            /*!                pthread_mutex_trylock() */
#define TRACE_MUTEXTIMEDLOCK 1          /*!                pthread_mutex_timedlock() */
#define TRACE_MUTEXUNLOCK 1             /*!                pthread_mutex_unlock   */
#undef TRACE_GLB_COND                   /*!             Функции управления условными переменными */
#define TRACE_CONDBROADCAST 1           /*!                pthread_cond_broadcast() */
#define TRACE_CONDDESTROY 1             /*!                pthread_cond_destroy() */
#define TRACE_CONDINIT 1                /*!                pthread_cond_init()    */
#define TRACE_CONDSIGNAL 1              /*!                pthread_cond_signal()  */
#define TRACE_CONDTIMEDWAIT 1           /*!                pthread_cond_timedwait() */
#define TRACE_CONDWAIT 1                /*!                pthread_cond_wait()    */
#undef TRACE_GLB_MQP                    /*!             Функции управления очередями сообщений */
#define TRACE_MQCLOSE 1                 /*!                mq_close()             */
#define TRACE_MQNOTIFY 1                /*!                mq_notify()            */
#define TRACE_MQOPEN 1                  /*!                mq_open()              */
#define TRACE_MQRECEIVE 1               /*!                mq_receive()           */
#define TRACE_MQTIMEDRECEIVE 1          /*!                mq_timedreceive()      */
#define TRACE_MQSEND 1                  /*!                mq_send()              */
#define TRACE_MQTIMEDSEND 1             /*!                mq_timedsend()         */
#define TRACE_MQSETATTR 1               /*!                mq_setattr()           */
#define TRACE_MQGETATTR 1               /*!                mq_getattr()           */
#define TRACE_MQUNLINK 1                /*!                mq_unlink()            */
#undef TRACE_GLB_TMR                    /*!             Часы и таймеры            */
#define TRACE_TMRSLEEP 1                /*!                sleep()                */
#define TRACE_TMRNANOSLEEP 1            /*!                nanosleep()            */
#define TRACE_TMREXPIRE 1               /*!                timerExpire()          */
#define TRACE_TMRCREATE 1               /*!                timer_create()         */
#define TRACE_TMRDELETE 1               /*!                timer_delete()         */
#define TRACE_TMRGETOVERRUN 1           /*!                timer_getoverrun()     */
#define TRACE_TMRSETTIME 1              /*!                timer_settime()        */
#define TRACE_TMRCLCSETTIME 1           /*!                clock_settime()        */
#undef TRACE_GLB_IO                     /*!             Операции ввода/вывода     */
#define TRACE_IOMKFIFO 1                /*!                mkfifo()               */
#define TRACE_IOLSEEK 1                 /*!                lseek()                */
#define TRACE_IOOPEN 1                  /*!                open(),creat()         */
#define TRACE_IOCLOSE 1                 /*!                close()                */
#define TRACE_IODUP 1                   /*!                dup()                  */
#define TRACE_IODUP2 1                  /*!                dup2()                 */
#define TRACE_IOPIPE 1                  /*!                pipe()                 */
#define TRACE_IOREAD 1                  /*!                read()                 */
#define TRACE_IOWRITE 1                 /*!                write()                */
#define TRACE_IOFDATASYNC 1             /*!                fdatasync()            */
#define TRACE_AIOREAD 1                 /*!                aio_read()             */
#define TRACE_AIOWRITE 1                /*!                aio_write()            */
#define TRACE_AIOFSYNC 1                /*!                aio_fsync()            */
#define TRACE_AIOCANCEL 1               /*!                aio_cancel()           */
#define TRACE_AIOERROR 1                /*!                aio_error()            */
#define TRACE_AIORETURN 1               /*!                aio_return()           */
#define TRACE_AIOSUSPEND 1              /*!                aio_suspend()          */
#undef TRACE_MPMEM                      /*!             Распределение общей памяти */
#define TRACE_MPMEMPOOLCREATE 1         /*!                memPoolCreate()        */
#define TRACE_MPMEMPOOLADD 1            /*!                memPoolAdd()           */
#define TRACE_MPMEMPOOLALLOC 1          /*!                Выделение памяти       */
#define TRACE_MPMEMPOOLFREE 1           /*!                Освобождение памяти    */
#undef TRACE_MPSEM                      /*!             Функции управления семафорами на общей памяти */
#define TRACE_MPSEMTRYWAIT 1            /*!                sem_trywait() MP       */
#define TRACE_MPSEMTIMEDWAIT 1          /*!                sem_timedwait MP       */
#define TRACE_MPSEMWAIT 1               /*!                sem_wait() MP          */
#define TRACE_MPSEMPOST 1               /*!                sem_post() MP          */
#define TRACE_MPSEMINIT 1               /*!                sem_init() MP          */
#define TRACE_MPSEMOPEN 1               /*!                sem_open() MP          */
#define TRACE_MPSEMGETVALUE 1           /*!                sem_getvalue() MP      */
#undef TRACE_MPMQ                       /*!             Функции управления очередями сообщений на общей памяти */
#define TRACE_MPMQOPEN 1                /*!                mq_open() MP           */
#define TRACE_MPMQRECEIVE 1             /*!                mq_receive() MP        */
#define TRACE_MPMQTIMEDRECEIVE 1        /*!                mq_timedreceive() MP   */
#define TRACE_MPMQSEND 1                /*!                mq_send() MP           */
#define TRACE_MPMQTIMEDSEND 1           /*!                mq_timedsend() MP      */
#define TRACE_MPMQSETATTR 1             /*!                mq_setattr() MP        */
#define TRACE_MPMQGETATTR 1             /*!                mq_getattr() MP        */
#define TRACE_GLB_USEREVENT 1           /*!             TRACE_GLB_USEREVENT       */
#undef TRACE_GLB_USER                   /*!                События прикладной программы */
#define TEXTDOMAINDIR "/tar/locale"     /*!       Каталог региональных настроек   */
#define XLOCALEDIR "/tar/xlocale"       /*!       Каталог XLocale                 */
#undef ENVDISPLAY_SET                   /*!          Определить переменную DISPLAY */
                                        /*!          Значение переменной DISPLAY  */
#define ENVDISPLAY_VALUE "127.0.0.1:0.0"
#undef XSERVER_START                    /*!          Запустить X сервер           */
#define XSERVER_PRIORITY 100            /*!          Приоритет Х-сервера          */
#undef XSERVER_HOSTSCNT                 /*!          Управление доступом          */
#define XSERVER_PARMCONFIG 1            /*!          Заместить значения в файле конфигурации */
#define XSERVER_PREFIX "/tar"           /*!          Префикс                      */
#define XSERVER_RGB "/X/rgb"            /*!          Имя RGB файла                */
                                        /*!          Пути к шрифтам               */
#define XSERVER_FONTS "/X/fonts/besta,/X/fonts/cyrillic,/X/fonts/misc"
#define XSERVER_DEPTH 8                 /*!          Глубина цвета                */
                                        /*!          Режимы                       */
#define XSERVER_MODES "1024x768,800x600,640x480"
                                        /*!          Файл XF86Config              */
#define XSERVER_CFGFILE "/mips/XF86Config"
#define XSERVER_OUT ""                  /*!          Выходной поток X сервера     */
#undef XSERVER_LOG                      /*!          Протокол X сервера (log)     */
#define XSERVER_LOGFILE "/nfs/syslog"   /*!          Имя файла протокола          */
