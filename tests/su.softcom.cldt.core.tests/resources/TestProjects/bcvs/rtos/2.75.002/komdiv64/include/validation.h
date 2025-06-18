/**
 * @file   validation.h
 * @author Ivan Korotkov <nkik@niisi.ras.ru>
 * @date   Wed May  5 17:03:43 2010
 *
 * @brief  Средства самоконтроля (валидации) системы
 *
 */

#ifndef _VALIDATION_H_
#define _VALIDATION_H_

#include <sys/types.h>
#include <sys/objclasses.h>
/**
 * Коды ошибок
 *
 * @sa validation_context
 */
enum validation_error {
    VE_OK        = 0x0,
    VE_PTR       = 0x1,       ///< найден неверный указатель
    VE_VALUE     = 0x2,       ///< недопустимое значение поля
    VE_OBJMEM    = 0x4,       ///< недоступна область памяти объекта
    VE_LINK      = 0x8,       ///< нарушена целостность ссылок в списке
    VE_OBJID     = 0x10,      ///< неверный или неактивный id объекта 
    VE_OBJTYPE   = 0x20,      ///< в id неверный тип объекта
    VE_OBJLINK   = 0x40,      ///< неверные ссылки между объектами
    VE_LOST      = 0x80,      ///< объект потерян (не содержится ни в одном списке)
    VE_DUP       = 0x100,     ///< объект входит в несколько несовместных списков
    VE_ALIGN     = 0x200,     ///< объект (регион пула) не выровнен
    VE_2FREE     = 0x400,     ///< два свободных блока пула подряд
    VE_POOLOVF   = 0x800,     ///< блок выходит за границы пула
    VE_STATE     = 0x1000,    ///< неверное состояние объекта (блока памяти и т.п.)
    VE_STACK     = 0x2000,    ///< разрушение стека
    VE_RA        = 0x4000,    ///< неверный адрес возврата
    VE_DEAD      = 0x8000,    ///< обнаружен мертвый объект (с нулевым счетчиком ссылок)
    VE_BOUNDS    = 0x10000,   ///< буфер выходит за границы доступного пула
    VE_ORDER     = 0x20000,   ///< нарушен порядок объектов в очереди
    VE_EXCEPTION = 0x40000,   ///< исключение в ходе проверки
};
/**
 * Флаги типов классов (для использования в битовой маске)
 */
#define    CLASSMASK_SYSCONTROL      (1LL << CLASS_SYSCONTROL)
#define    CLASSMASK_MEMCONTROL      (1LL << CLASS_MEMCONTROL)
#define    CLASSMASK_SMEMCONTROL     (1LL << CLASS_SMEMCONTROL)
#define    CLASSMASK_SEMCONTROL      (1LL << CLASS_SEMCONTROL)
#define    CLASSMASK_SSEMCONTROL     (1LL << CLASS_SSEMCONTROL)
#define    CLASSMASK_EVNCONTROL      (1LL << CLASS_EVNCONTROL)
#define    CLASSMASK_MQCONTROL       (1LL << CLASS_MQCONTROL)
#define    CLASSMASK_IODEVCONTROL    (1LL << CLASS_IODEVCONTROL)
#define    CLASSMASK_HSHCONTROL      (1LL << CLASS_HSHCONTROL)
#define    CLASSMASK_SHSHCONTROL     (1LL << CLASS_SHSHCONTROL)
#define    CLASSMASK_MTXCONTROL      (1LL << CLASS_MTXCONTROL)
#define    CLASSMASK_SMTXCONTROL     (1LL << CLASS_SMTXCONTROL)
#define    CLASSMASK_CNDCONTROL      (1LL << CLASS_CNDCONTROL)
#define    CLASSMASK_SCNDCONTROL     (1LL << CLASS_SCNDCONTROL)
#define    CLASSMASK_ASEMCONTROL     (1LL << CLASS_ASEMCONTROL)
#define    CLASSMASK_BUFCONTROL      (1LL << CLASS_BUFCONTROL)
#define    CLASSMASK_BBRDCONTROL     (1LL << CLASS_BBRDCONTROL)
#define    CLASSMASK_SPORTCONTROL    (1LL << CLASS_SPORTCONTROL)
#define    CLASSMASK_QPORTCONTROL    (1LL << CLASS_QPORTCONTROL)
#define    CLASSMASK_THREADCONTROL   (1LL << CLASS_THREADCONTROL)
#define    CLASSMASK_TMRCONTROL      (1LL << CLASS_TMRCONTROL)
#define    CLASSMASK_CLCCONTROL      (1LL << CLASS_CLCCONTROL)
#define    CLASSMASK_SYMCONTROL      (1LL << CLASS_SYMCONTROL)
#define    CLASSMASK_STDIOCONTROL    (1LL << CLASS_STDIOCONTROL)
#define    CLASSMASK_ECBCONTROL      (1LL << CLASS_ECBCONTROL)
#define    CLASSMASK_SELCONTROL      (1LL << CLASS_SELCONTROL)
#define    CLASSMASK_MODULECONTROL   (1LL << CLASS_MODULECONTROL)
#define    CLASSMASK_IOFILECONTROL   (1LL << CLASS_IOFILECONTROL)
#define    CLASSMASK_ANY             -1LL

typedef int (*vi_func_t)();
struct validation_info {
    /**
     * Указатели на функции, осуществляющие валидацию указанных классов
     */
    vi_func_t vi_chk_funcs[CLASS_TOTAL];
    /**
     * Указатели на функции дампа полей объекта
     */
    vi_func_t vi_dump_funcs[CLASS_TOTAL];
    /**
     * Указатель на функцию (без параметров), вызываемую из HM
     * для проверки объектов при сбое системы
     */
    vi_func_t hm_validation_func;
};

/**
 * Метод уведомления об обнаруженных проблемах
 *
 * @sa validateSystem
 * @sa validationInitContext
 */
enum validation_report_facility {
    VR_SILENT  = 0x0,           ///< не выводить сообщения
    VR_KPRINT  = 0x1,           ///< выводить сообщения через kprint,
    VR_PRINTF  = 0x2,           ///< выводить сообщения через printf
    VR_SYSLOG  = 0x3,           ///< выводить сообщения в системный журнал 
    VR_METHOD_MASK = 0x3,       ///< маска битов, определяющих функцию вывода
    VR_OBJDUMP = 0x100,         ///< выводить дамп объектов
    VR_DEEPCHECK = 0x200        ///< для совместимости с ос2
};

/** 
 * @brief Выполняет проверку системных объектов и списков
 *
 * @param class_mask Маска типов классов, которые необходимо проверить.
 *                   Если указано -1, проверяются все поддерживаемые классы объектов.
 * @param objectId Проверить данный объект (если 0, проверяются все).
 * @param facility Используемый метод журналирования, возможные значения из enum validation_report_facility
 * @param status Если не NULL, по указанному адресу сохраняется категория обнаруженной ошибки
 *               (комбинация флагов из enum validation_error).
 *
 * @return 0, если не обнаружено проблем, в случае ошибки:
 *   - [EINVAL]   -- неверное значение одного из параметров
 *   - [ECORRUPT] -- обнаружены проблемы
 *   - [ENOSYS]   -- средства проверки объектов не были включены при конфигурировании ОС
 */
int validateObjects(uint64_t class_mask, objid_t objectId,
                    enum validation_report_facility facility, int *status);
int validateSystem(int facility);
int validationInit();

void validationSetClassFunc(int cls, void *chk_func, void *dump_func);

#define validationSET_CLASS_FUNC(cls, chk_func, dump_func)              \
    ({                                                                  \
        int chk_func();                                                 \
        int dump_func();                                                \
        validationSetClassFunc(cls,chk_func, dump_func);                \
    })



#endif /* _VALIDATION_H_ */
