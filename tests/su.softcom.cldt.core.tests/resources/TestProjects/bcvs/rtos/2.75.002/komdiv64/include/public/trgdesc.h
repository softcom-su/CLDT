#ifndef _TRGDESC_H_
#define _TRGDESC_H_

/**
 * @brief Интерфейс доступа к целевому описанию системы (ЦОС).
 * 
 * ЦОС - это ELF-файл. Перед работой с ЦОС нужно его открыть с помощью одной
 * из функций:
 *   - tdscOpenTDescrFromMem() - если файл ЦОС лежит где-то в памяти
 *   - tdscOpenTDescrFromFile() - если файл ЦОС расположен в файловой системе.
 * 
 * После открытия ЦОС нужно выбрать режим старта с помощью функции
 * tdscSetStartupMode(). Если этого не сделать, будет использоваться "общий"
 * режим (general).
 * 
 * Большинство данных в ЦОС представляю собой массивы структур-описателей
 * (процессоров, монтируемых каталогов, интерфейсов ethernet и т. п.). Получить
 * эти данные можно двумя способами:
 *   1) Инициализировать итератор одной из функций tdscXXXBegin(), после чего
 *      он указывает на начало соответствующего массива. Элемент массива, на
 *      который указывает итератор, можно получить с помощью соответствующей
 *      функции tdscGetXXX(). Передвижение итератора на следующий элемент
 *      выполняется функцией tdscIterNext().
 *   2) Вызвать одну из функций tdscForeachXXX(), передав ей указатель на
 *      функцию-обработчик. Эта функция будет вызвана для каждого элемента
 *      массива, и при каждом вызове этот элемент будет ей передан в качестве
 *      аргумента.
 * 
 * Есть также несколько функций, не укладывающихся в приведенную выше схему.
 * К ним относятся, например, функции tdscGetCpuById() и
 * tdscGetCurrentStartupMode().
 */

#include <limits.h> /* для PATH_MAX */
#include <sys/types.h>
#include <stdio.h> /* для sprintf */

/**
 * @brief Идентификатор устройства RapidIO.
 *
 * Оконечные устройства могут иметь реальные идентификаторы в диапазоне
 * от 0 до 65535. Коммутаторы имеют виртуальные идентификаторы, используемые 
 * только для идентификации коммутаторов в целевом описании.
 */
typedef unsigned int tdscRioId;

/**
 * @brief Географический адрес устройства RapidIO.
 */
typedef unsigned int tdscGeoId;

/* ========================================================================== */

/**
 * @brief Итератор массива элементов целевого описания.
 */
typedef struct {
    int   i_ArrayType; /**< Идентификатор типа элементов массива (пока не
                        *   используется. */
    char *i_ArrayPtr;  /**< Указатель на начало массива */
    int   i_ElemSize;  /**< Размер элемента. */
    int   i_ElemsCnt;  /**< Количество элементов в массиве. */
    int   i_CurElem;   /**< Номер текущего элемента (начиная с 0). */
    char *i_ParentPtr; /**< Указатель на родительский элемент массива. */
} tdscIterator;

/**
 * @brief Передвинуть итератор на следующий элемент массива.
 *
 * Если в массиве отсутствует следующий элемент, то итератор остается в неизменном
 * состоянии, и возвращается ENODATA.
 *
 * @param [in,out] pIter Указатель на передвигаемый итератор.
 *
 * @retval 0          Итератор успешно передвинут на следующий элемент.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Адрес итератора равен нулю.
 * @retval ENODATA    В массиве больше нет элементов, итератор не передвинут.
 */
int tdscIterNext(tdscIterator *pIter);

/* ===================== Открытие и закрытие описания ======================= */

/**
 * @brief Общая информация о целевом описании.
 */
typedef struct
{
    char tdi_Name[64];           /**< Имя описания. */
    char tdi_Version[16];        /**< Версия описания. */
    char tdi_CompilerVersion[16];/**< Версия компилятора, с помощью которого получено
                                  *   описание. */
} tdscTDescrInfo;

/**
 * @brief Открыть целевое описание, расположенное в памяти.
 *
 * @param [in] descAddr Адрес начала целевого описания в памяти.
 * @param [out] pTdi Адрес структуры с информацией о целевом описании. Если адрес
 * не равен NULL, то в случае успешного открытия в структуру записывается информация
 * об открытом описании.
 *
 * @retval 0        Целевое описание успешно открыто.
 * @retval EALREADY Целевое описание уже было открыто.
 * @retval EINVAL   Ошибка при разборе формата описания.
 * @retval ENOTSUP  Версия формата целевого описания не поддерживается текущей версией
 * библиотеки.
 */
int tdscOpenTDescrFromMem(void *descAddr, tdscTDescrInfo *pTdi);

/**
 * @brief Открыть целевое описание, расположенное в файле.
 *
 * @param [in] path Путь к файлу описания.
 * @param [in] pTdi Адрес структуры с информацией о целевом описании. Если адрес
 * не равен NULL, то в случае успешного открытия в структуру записывается информация
 * об открытом описании.
 *
 * @retval 0        Целевое описание успешно открыто.
 * @retval EALREADY Целевое описание уже было открыто.
 * @retval ENOENT   Невозможно открыть файл.
 * @retval ENOMEM   Ошибка выделения памяти под описание.
 * @retval EIO      Ошибка чтения файла.
 * @retval EINVAL   Ошибка при разборе формата описания.
 * @retval ENOTSUP  Версия формата целевого описания не поддерживается текущей версией
 * библиотеки.
 */
int tdscOpenTDescrFromFile(const char *path, tdscTDescrInfo *pDdi);

/**
 * @brief Закрыть целевое описание.
 *
 * Если целевое описание было открыто с помощью функции tdscOpenTDescrFromFile(), то
 * выделенная под описание память освобождается. В противном случае этого не происходит.
 *
 * @retval 0          Описание успешно закрыто.
 * @retval EAVAILABLE Описание не было открыто.
 */
int tdscCloseTDescr();

/* ===================== Режимы старта ====================================== */

/**
 * @brief Режим старта системы.
 */
typedef enum {
    SM_GENERAL, /**< Общий режим. */
    SM_LOAD,    /**< Режим загрузки (штатной работы). */
    SM_RECONF,  /**< Режим реконфигурирования. */
    SM_OS       /**< Режим штатной работы ОС. */
} tdscStartupMode;

/**
 * @brief Установить режим и подрежим старта.
 * 
 * В описании могут быть заданы различные атрибуты для различных режимов и
 * подрежимов старта. После вызова этой функции вызовы функций получения данных из
 * описания будут возвращать те значения атрибутов, которые определены для указанного 
 * режима и подрежима. При открытии описания с помощью этой функции устанавливается
 * режим SM_GENERAL.
 * 
 * Режимы SM_GENERAL и SM_OS не могут иметь подрежимов.
 * 
 * Если описание не поддерживает в точности заданный режим и/или подрежим, то будет
 * использоваться поддерживаемый более общий режим, ближайший к заданному. Например,
 * если заданая неподдерживаемая комбинация режима SM_LOAD и подрежима 5, то будет
 * использоваться режим SM_LOAD без подрежима (если таковой поддерживается описанием)
 * или режим SM_GENERAL (если SM_LOAD без подрежима тоже не поддерживается).
 * 
 * Проверить, какой именно режим установился в результате вызова, можно с
 * помощью функции tdscGetCurrentStartupMode().
 * 
 * После вызова функции все инициализированные ранее итераторы продолжают
 * указывать на данные, относящиеся к прежнему режиму и подрежиму.
 * 
 * @param [in] mode    Режим старта системы.
 * @param [in] submode Имя подрежима старта. Передача NULL означает отсутствие 
 * подрежима.
 * 
 * @retval 0          Режим старта успешно установлен.
 * @retval EAVAILABLE Целевое описание не открыто.
 */
int tdscSetStartupMode(tdscStartupMode mode, const char *submode);

/**
 * @brief Опрос текущего режима и подрежима старта.
 * 
 * @param [out] pMode     Адрес, по которому будет записан идентификатор текущего
 * режима.
 * @param [out] ppSubmode Адрес, по которрому будет записан адрес строки с именем
 * текущего подрежима. Если подрежим не установлен, будет записан NULL.
 * 
 * @retval 0          Режим и подрежим успешно получены.
 * @retval EINVAL     В pMode или ppSubmode передан нулевой указатель.
 * @retval EAVAILABLE Целевое описание не открыто. 
 */
int tdscGetCurrentStartupMode(tdscStartupMode *pMode, const char **ppSubmode);

/**
 * @brief Установить итератор на начало массива поддерживаемых режимов старта.
 * 
 * @param [out] pIter Указатель на инициализируемый итератор.
 * 
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIter. 
 */
int tdscStartupModesBegin(tdscIterator *pIter);

/**
 * @brief Получить режим старта, на который указывает итератор.
 * 
 * @param [in]  pIter Адрес итератора.
 * @param [out] pMode 
 * @param [out] ppSubmode Адрес, по которому будет записан адрес строки с именем
 * подрежима. Если подрежим не задан, будет записан NULL.
 * 
 * @retval 0          Информация о режиме успешно получена.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIter, pMode или ppSubmode.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetStartupMode(tdscIterator *pIter, tdscStartupMode *pMode,
        const char **ppSubmode);

/* ===================== Файлы ВОС ========================================== */

/**
 * @brief Файл ВОС, использованный при построении описания.
 */
typedef struct tdscHldFile_tag {
    char hf_Path[PATH_MAX+1]; /**< Путь к файлу ВОС. */
    char hf_Version[16];      /**< Версия файла ВОС. */
} tdscHldFile;

/**
 * @brief Установить итератор на начало массива описателей файлов ВОС.
 *
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое pIter.
 */
int tdscHldFilesBegin(tdscIterator *pIter);

/**
 * @brief Получить описатель файла ВОС, на который указывает итератор.
 *
 * @param [in]  pIter    Указатель на итератор.
 * @param [out] pHldFile Адрес, по которому записывается описатель файла.
 *
 * @retval 0          Описатель файла успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pHldFile.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetHldFile(const tdscIterator *pIter, tdscHldFile *pHldFile);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую файл ВОС.
 * 
 * @param [in] pHldFile Указатель на описатель файла.
 * @param [in] arg      Пользовательский аргумент.
 */
typedef void (*tdscHldFileCallback)(const tdscHldFile *pHldFile, void *arg);

/**
 * @brief Вызвать заданную функцию для описателя каждого файла ВОС.
 *
 * @param [in] pCallback Адрес функции обратного вызова.
 * @param [in] arg Пользовательский аргумент, передаваемый функции обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCallback.
 */
int tdscForeachHldFile(tdscHldFileCallback pCallback, void *arg);

/* ===================== Процессорные элементы ============================== */

/**
 * @brief Режим загрузки процессорного элемента.
 */
typedef enum {
    LM_SELF,                  /**< Автономная загрузка. */
    LM_PASSIVE,               /**< Принудительная (пассивная) загрузка. */
    LM_REQUEST,
} tdscLoadMode;

/**
 * @brief Параметры процессорного элемента, связанные с его загрузкой.
 */
typedef struct tdscLoad_tag {
    void         *origin;
    tdscLoadMode  l_Mode;             /**< Режим загрузки. */
    char          l_Path[PATH_MAX+1]; /**< Путь к образу. */
    unsigned long l_Address;          /**< Адрес образа. */
} tdscLoad;

/**
 * @brief Процессорный элемент с оконечным устройством RapidIO.
 */
typedef struct tdscCpu_tag {
    void     *origin;
    char      c_Name[128];          /**< Имя процессорного элемента. */
    char      c_AConfigPath[128];   /**< Путь к файлу ARINC-конфигурации. */
    tdscRioId c_RioId;              /**< Идентификатор RapidIO. */
    tdscRioId c_HostRioId;          /**< Идентификатор ведущего устройства. Если 
                                     * данное устройство является ведущим,
                                     * значения c_RioId и c_HostRioId совпадают. */
    tdscGeoId c_GeoId;              /**< Географический адрес. */
    tdscLoad  c_Load;               /**< Параметры загрузки. */
    char      c_DefaultGateway[16]; /**< IP-адрес шлюза по умолчанию. */
    unsigned  c_LoaderFlags;   /**< Флаги загрузчика. */
    unsigned  c_Reconf;             /**< Флаг разрешения реконфигурирования. */
} tdscCpu;

/**
 * @brief Найти процессорный элемент с заданным идентификатором RapidIO.
 *
 * @param [in]  rioId Идентификатор RapidIO.
 * @param [out] pCpu  Адрес, по которому записывается описатель найденного 
 * процессорного элемента.
 *
 * @retval 0          Процессорный элемент успешно найден.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Некорректный идентификатор или нулевое значение pCpu.
 * @retval ENOENT     Процессорный элемент с заданным идентификатором не найден.
 */
int tdscGetCpuById(tdscRioId rioId, tdscCpu *pCpu);

/**
 * @brief Найти процессорный элемент с заданным географическим адресом.
 *
 * Если несколько процессорных элементов в целевом описании имеют одинаковые
 * географические адреса, то выбирается один из них произвольным образом.
 *
 * @param [in]  geoId Географический адрес.
 * @param [out] pCpu  Адрес, по которому записывается описатель найденного 
 * процессорного элемента.
 *
 * @retval 0          Процессорный элемент успешно найден.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu.
 * @retval ENOENT     Процессорный элемент с заданным географическим адресом
 * не найден.
 */
int tdscGetCpuByGeoId(tdscGeoId geoId, tdscCpu *pCpu);

/**
 * @brief Найти процессорный элемент с заданным полным именем.
 *
 * @param [in]  name Полное имя процессорного элемента.
 * @param [out] pCpu Адрес, по которому записывается описатель найденного
 * процессорного элемента.
 *
 * @retval 0          Процессорный элемент успешно найден.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu.
 * @retval ENOENT     Процессорный элемент с заданным именем не найден.
 */
int tdscGetCpuByName(const char *name, tdscCpu *pCpu);

/**
 * @brief Установить итератор на начало массива процессорных элементов.
 *
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIter.
 */
int tdscCpusBegin(tdscIterator *pIter);

/**
 * @brief Получить описатель процессорного элемента, на который указывает итератор.
 *
 * @param [in] pIter Указатель на итератор.
 * @param [out] pCpu Адрес, по которому записывается описатель процессорного элемента.
 *
 * @retval 0          Процессорный элемент успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIter или pCpu.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetCpu(const tdscIterator *pIter, tdscCpu *pCpu);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую описатель
 * процессорного элемента.
 * @param [in] pCpu Указатель на описатель процессорного элемента.
 * @param [in] arg  Пользовательский аргумент.
 */
typedef void (*tdscCpuCallback)(const tdscCpu *pCpu, void *arg);

/**
 * @brief Вызвать заданную функцию для каждого процессорого элемента.
 *
 * @param [in]  fpCallback Указатель на функцию обратного вызова.
 * @param [out] arg        Пользовательский аргумент, передаваемый функции
 * обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевой адрес функции обратного вызова.
 */
int tdscForeachCpu(tdscCpuCallback fpCallback, void *arg);

/* ===================== Интерфейсы Ethernet ================================ */

/**
 * @brief Интерфейс Ethernet.
 */
typedef struct tdscEthInterface_tag {
    void *origin;
    char  ei_Name[8];  /**< Имя устройства. */
    char  ei_Ip[16];   /**< IP-адрес. */
    char  ei_Mask[16]; /**< Маска подсети. */
} tdscEthInterface;

/**
 * @brief Установить итератор на начало массива описателей интерфейсов Ethernet
 * заданного процессорного элемента.
 *
 * @param [in]  pCpu  Указатель на описатель процессорного элемента.
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или pIter.
 */
int tdscEthInterfacesBegin(const tdscCpu *pCpu, tdscIterator *pIter);

/**
 * @brief Получить описатель интерфейса Ethernet, на который указывает итератор.
 *
 * @param [in] pIter Указатель на итератор.
 * @param [out] pEth Адрес, по которому записывается описатель интерфейса Ethernet.
 *
 * @retval 0          Описатель интерфейса Ethernet успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pEth или pCpu.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetEthInterface(const tdscIterator *pIter, tdscEthInterface *pEth);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую интерфейс Ethernet.
 * @param [in] pEth Указатель на описатель интерфейса Ethernet.
 * @param [in] arg Пользовательский аргумент.
 */
typedef void (*tdscEthInterfaceCallback)(const tdscEthInterface *pEth, void *arg);

/**
 * @brief Вызвать заданную функцию для каждого интерфейса Ethernet заданного 
 * роцессорного элемента.
 *
 * @param [in] pCpu       Указатель на описатель процессорного элемента.
 * @param [in] fpCallback Указатель на функцию обратного вызова.
 * @param [in] arg        Пользовательский аргумент, передаваемый функции 
 * обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или fpCallback.
 */
int tdscForeachEthInterface(const tdscCpu *pCpu,
                            tdscEthInterfaceCallback fpCallback, void *arg);

/* ===================== Реконфигурируемые файлы ============================ */

/**
 * @brief Реконфигурируемый файл.
 */
typedef struct tdscReplFile_tag {
    void         *origin;
    char          rf_Name[64];             /**< Имя файла. */
    char          rf_SrcPath[PATH_MAX+1];  /**< Путь к оригиналу файла. */
    unsigned long rf_SrcAddress;           /**< Адрес оригинала файла. */
    size_t        rf_Size;                 /**< Размер файла. */
    char          rf_DestPath[PATH_MAX+1]; /**< Путь записи файла. */
    unsigned long rf_DestAddress;          /**< Адрес записи файла. */
} tdscReplFile;

/**
 * @brief Установить итератор на начало массива описателей реконфигурируемых
 * файлов процессорного элемента.
 *
 * @param [in]  pCpu  Указатель на описатель процессорного элемента.
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или pIter.
 */
int tdscReplFilesBegin(const tdscCpu *pCpu, tdscIterator *pIter);

/**
 * @brief Получить описатель реконфигурируемого файла, на который указывает итератор.
 *
 * @param [in]  pIter  Указатель на итератор.
 * @param [out] pRFile Адрес, по которому записывается описатель файла.
 *
 * @retval 0          Описатель файла успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pRFile.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetReplFile(const tdscIterator *pIter, tdscReplFile *pRFile);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую реконфигурируемый файл.
 * 
 * @param [in] pRFile Указатель на описатель файла.
 * @param [in] arg    Пользовательский аргумент.
 */
typedef void (*tdscReplFileCallback)(const tdscReplFile *pRFile, void *arg);

/**
 * @brief Вызвать заданную функцию для каждого реконфигурируемого файла заданного
 * процессорного элемента.
 *
 * @param [in] pCpu       Адрес описателя процессорного элемента.
 * @param [in] fpCallback Адрес функции обратного вызова.
 * @param [in] arg        Пользовательский аргумент, передаваемый функции
 * обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или fpCallback.
 */
int tdscForeachReplFile(const tdscCpu *pCpu,
                        tdscReplFileCallback fpCallback, void *arg);

/* ===================== Монтируемые каталоги =============================== */

/**
 * @brief Протокол или тип файловой системы монтируемлшл каталога.
 */
typedef enum tdscMountProtocol_tag {
    MDP_NONE, /**< Неизвестный протокол. */
    MDP_FTP,  /**< Протокол FTP. */
    MDP_NFS,  /**< Протокол NFS. */
    MDP_TFTP, /**< Протокол TFTP. */
    MDP_VFAT, /**< Файловая система VFAT. */
    MDP_TAR   /**< Файловая система TAR. */
} tdscMountProtocol;

/**
 * @brief Монтируемый каталог.
 */
typedef struct tdscMountDir_tag {
    void             *origin;
    char              md_ServerIp[16];           /**< IP-адрес сервера. */
    tdscMountProtocol md_Proto;                  /**< Протокол монтирования. */
    char              md_Path[PATH_MAX+1];       /**< Путь монтирования. */
    unsigned long     md_Address;                /**< Адрес монтируемого образа. */
    size_t            md_Size;                   /**< Размер монтируемого образа. */
    char              md_MountPoint[PATH_MAX+1]; /**< Путь к точке монтирования. */
    unsigned          md_Uid;                    /**< Идентификатор пользователя. */
    unsigned          md_Gid;                    /**< Идентификатор группы. */
    char              md_User[32];               /**< Имя пользователя. */
    char              md_Password[32];           /**< Пароль. */
} tdscMountDir;

/**
 * @brief Установить итератор на начало массива описателей монтируемых каталогов
 * заданного процессорного элемента.
 *
 * @param [in]  pCpu  Указатель на описатель процессорного элемента.
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или pIter.
 */
int tdscMountDirsBegin(const tdscCpu *pCpu, tdscIterator *pIter);

/**
 * @brief Получить описатель монтируемого каталога, на который указывает итератор.
 *
 * @param [in]  pIter     Указатель на итератор.
 * @param [out] pMountDir Адрес, по которому записывается описатель монтируемого
 * каталога.
 *
 * @retval 0          Описатель монтируемого каталога успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pMountDir или pIter.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetMountDir(const tdscIterator *pIter, tdscMountDir *pMountDir);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую монтируемый каталог.
 * @param [in] pMountDir Указатель на описатель монтируемого каталога.
 * @param [in] arg       Пользовательский аргумент.
 */
typedef void (*tdscMountDirCallback)(const tdscMountDir *pMountDir, void *arg);

/**
 * @brief Вызвать заданную функцию для каждого монтируемого каталога заданного
 * процессорного элемента.
 *
 * @param [in] pCpu       Указатель на описатель процессорного элемента.
 * @param [in] fpCallback Указатель на функцию обратного вызова.
 * @param [in] arg        Пользовательский аргумент, передаваемый функции 
 * обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или fpCallback.
 */
int tdscForeachMountDir(const tdscCpu *pCpu,
                        tdscMountDirCallback fpCallback, void *arg);

/* ===================== IP-маршруты ======================================== */

/**
 * @brief Маршрут в IP-сети.
 */
typedef struct tdscIpRoute_tag {
    void *origin;
    char  ir_Dest[16];    /**< Адрес назначения (сеть или хост). */
    char  ir_Mask[16];    /**< Маска подсети. */
    char  ir_Gateway[16]; /**< IP-адрес шлюза. */
} tdscIpRoute;

/**
 * @brief Установить итератор на начало массива описателей IP-маршрутов заданного
 * процессорного элемента.
 *
 * @param [in] pCpu  Указатель на описатель процессорного элемента.
 * @param [in] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или pIter.
 */
int tdscIpRoutesBegin(const tdscCpu *pCpu, tdscIterator *pIter);

/**
 * @brief Получить описатель IP-маршрута, на который указывает итератор.
 *
 * @param [in]  pIter    Указатель на итератор.
 * @param [out] pIpRoute Адрес, по которому записывается описатель IP-маршрута.
 *
 * @retval 0          Описатель IP-маршрута успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIpRoute или pIter.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetIpRoute(const tdscIterator *pIter, tdscIpRoute *pRoute);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую IP-маршрут.
 * @param [in] pRoute Указатель на описатель IP-маршрута/
 * @param [in] arg    Пользовательский аргумент.
 */
typedef void (*tdscIpRouteCallback)(const tdscIpRoute *pRoute, void *arg);

/**
 * @brief Вызвать заданную функцию для каждого IP-маршрута заданного процессорного
 * элемента.
 *
 * @param [in] pCpu       Указатель на описатель процессорного элемента.
 * @param [in] fpCallback Указатель на функцию обратного вызова.
 * @param [in] arg        Пользовательский аргумент, передаваемый функции 
 * обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pCpu или pCallback.
 */
int tdscForeachIpRoute(const tdscCpu *pCpu,
                       tdscIpRouteCallback fpCallback, void *arg);

/* ===================== Коммутаторы и таблицы маршрутизации ================ */

/**
 * @brief Коммутатор RapidIO.
 */
typedef struct tdscRioSwitch_tag {
    void *origin;
    char  rs_Name[128];
    int   rs_HostId; /**< Идентификатор ведущего процессора, ответственного за
                          инициализацию коммутатора. */
    int   rs_Id;     /**< Виртуальный идентификатор коммутатора (превышает 65535). */
} tdscRioSwitch;

/**
 * @brief Таблица маршрутизации коммутатора RapidIO.
 */
typedef struct tdscRioRoutingTable_tag {
    void *origin;
    int   rrt_InPort; /**< Номер входного порта, которому соответствует таблица (255,
                           если используется единая таблица. */
} tdscRioRoutingTable;

/**
 * @brief Запись в таблице маршрутизации коммутатора RapidIO.
 */
typedef struct tdscRioRoutingEntry_tag {
    void     *origin;
    int       rre_InPort;  /**< Номер входного порта соответствующей таблицы
                            * (особые значения: 255 - используется единая таблица,
                            * 254 - таблица для маршрутизации отправляемых
                            * пакетов). */
    tdscRioId rre_DstId;   /**< Идентификатор пункта назначения. */
    int       rre_OutPort; /**< Выходной порт для заданного пункта назначения. */
    int       rre_SpanLength;
} tdscRioRoutingEntry;

/**
 * @brief Получить описатель коммутатора RapidIO с заданным идентификатором.
 *
 * @param [in] id  Идентификатор коммутатора (превышает 65535).
 * @param [in] pSw Адрес, по которому записывается описатель коммутатора.
 *
 * @retval 0          Коммутатор успешно найден.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pSw.
 * @retval ENOENT     Коммутатор с заданным идентификатором не найден.
 */
int tdscGetRioSwitchById(tdscRioId id, tdscRioSwitch *pSw);

/**
 * @brief Установить итератор на начало массива описателей таблиц маршрутизации
 * заданного коммутатора RapidIO.
 *
 * @param [in]  pSw   Указатель на описатель коммутатора.
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pSw или pIter.
 */
int tdscRioRoutingTablesBegin(const tdscRioSwitch *pSw, tdscIterator *pIter);

/**
 * @brief Получить описатель таблицы маршрутизации RapidIo, на который указывает 
 * итератор.
 *
 * @param [in]  pIter    Указатель на итератор.
 * @param [out] pRioRTbl Адрес, по которому записывается описатель таблицы 
 * маршрутизации.
 *
 * @retval 0          Описатель таблицы маршрутизации успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pRioRTbl или pIter.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetRioRoutingTable(const tdscIterator *pIter, tdscRioRoutingTable *pRioRTbl);

/**
 * @brief Установить итератор на начало массива описателей записей заданной таблицы
 * маршрутизации RapidIO.
 *
 * @param [in]  pRt   Указатель на описатель таблицы маршрутизации.
 * @param [out] pIter Указатель на инициализируемый итератор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pRt или pIter.
 */
int tdscRioRoutingEntriesBegin(const tdscRioRoutingTable *pRt, tdscIterator *pIter);

/**
 * @brief Получить описатель записи таблицы маршрутизации RapidIO, на который указывает
 * итератор.
 *
 * @param [in]  pIter      Указатель на итератор.
 * @param [out] pRioREntry Адрес, по которому записывается описатель записи таблицы
 * маршрутизации.
 *
 * @retval 0          Описатель записи таблицы маршрутизации успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pRioREntry или pIter.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetRioRoutingEntry(const tdscIterator *pIter, tdscRioRoutingEntry *pRioREntry);

/**
 * @brief Указатель на функцию обратного вызова, обрабатывающую запись таблицы
 * маршрутизации.
 * @param [in] pEntry Указатель на описатель записи таблицы маршрутизации.
 * @param [in] arg    Пользовательский аргумент.
 */
typedef void (*tdscRioRoutingEntryCallback)(const tdscRioRoutingEntry *pEntry, void *arg);

/**
 * @brief Вызвать заданную функцию для каждой записи каждой таблицы маршрутизации
 * заданного коммутатора RapidIO.
 *
 * @param [in] pSw        Указатель на описатель коммутатора RapidIO.
 * @param [in] fpCallback Указатель на функцию обратного вызова.
 * @param [in] arg        Пользовательский аргумент, передаваемый функции обратного вызова.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pSw или pCallback.
 */
int tdscForeachRioRoutingEntry(const tdscRioSwitch *pSw,
                               tdscRioRoutingEntryCallback fpCallback, void *arg);

/* ===================== Топология системы ================================== */

/**
 * @brief Физический канал между двумя устройствами RapidIO.
 */
typedef struct tdscRioLink_tag {
    void      *origin;
    tdscRioId rl_FromId;   /**< Идентификатор устройства-источника. */
    int       rl_FromPort; /**< Порт на устройстве-источнике. */
    tdscRioId rl_ToId;     /**< Идентификатор устройства-приемника. */
    int       rl_ToPort;   /**< Порт на устройстве-приемнике. */
} tdscRioLink;

/**
 * @brief Установить итератор на начало массива физических каналов RapidIO,
 * подключенных к указанному устройству.
 *
 * @param [in]  id    Идентификатор устройства RapidIO.
 * @param [out] pIter Указатель на инициализируемый коммутатор.
 *
 * @retval 0          Функция выполнена успешно.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pIter.
 * @retval ENOENT     Устройство с заданным идентификатором не найдено.
 */
int tdscIncidentRioLinksBegin(tdscRioId id, tdscIterator *pIter);

/**
 * @brief Получить описатель физического канала RapidIO, на который указывает итератор.
 *
 * @param [in]  pIter Указатель на итератор.
 * @param [out] pLink Адрес, по которому записывается описатель физического канала.
 *
 * @retval 0          Описатель физического канала успешно получен.
 * @retval EAVAILABLE Целевое описание не открыто.
 * @retval EINVAL     Нулевое значение pLink или pIter.
 * @retval ENODATA    Итератор не указывает на элемент массива.
 */
int tdscGetRioLink(const tdscIterator *pIter, tdscRioLink *pLink);

#endif /* _TRGDESC_H_ */
