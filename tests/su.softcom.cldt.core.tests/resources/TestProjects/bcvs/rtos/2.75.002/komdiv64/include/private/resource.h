/*  private/resource.h  */

#ifndef _PRIVATE_RESOURCE_H
#define _PRIVATE_RESOURCE_H

/* Ресурсы используются для разделения доступа к одному 
 * и тому же объекту из разных потоков и из прерываний 
 * Все действия, которые надо провести с объектом в режиме разделения 
 * содержатся в процедуре обработки ресурса - элемент m_handler 
 * Используемая техника позволяет гарантировать, что если процедура обработки 
 * началась исполняться, то до её завершения она не будет запущена повторно 
 * (вложенно) из другого потока или прерывания 
 */
struct obj_header;
/* Структура описания ресурса */
typedef struct res_descr {
	struct obj_header  *m_owner;	/* ссылка на объект которому принадлежит ресурс */
	volatile    int	     m_lock;    /* Атрибут блокирования ресупса */
    volatile    u_long  m_counter;  /* Счетчик неудачных попыток вызова обработчика ресурса */
    funcptr_t   m_handler;			/* Процедура обработки ресурса */
    u_long      m_param;			/* Параметр процедуры обработки ресурса */
} resDESCR;


#ifdef __cplusplus
extern "C" {
#endif

/* Инициализация структуры ресурса */
#define resInit(res, owner, handler, param) \
    (res)->m_owner = owner; \
    (res)->m_lock = FALSE; \
    (res)->m_counter = 0; \
    (res)->m_handler = handler; \
    (res)->m_param = param;

/* Захват ресурса */
#define resLock(res) ((res)->m_lock = TRUE)

/* Проверка ресурса */
#define resTest(res) ( (res)->m_lock  )

/* Освобождение ресурса */
#define resUnlock(res) { (res)->m_lock = FALSE; if ( (res)->m_counter ) resDeferredHandling(res); }

/* Попытка захвата ресурса */
#define resTryLock(res) ( (res)->m_lock ? resIncCounter(res) : 1 /*((res)->m_lock = TRUE)*/ )

/* Обработка отложенных запросов на вызов процедуры обработки ресурса */
void resDeferredHandling(resDESCR *res);

/* Безопасное увеличение счетчика неудачных попыток захвата ресурса */
int resIncCounter(resDESCR *res);


#ifdef __cplusplus
}
#endif

#endif  /* _PRIVATE_RESOURCE_H */

