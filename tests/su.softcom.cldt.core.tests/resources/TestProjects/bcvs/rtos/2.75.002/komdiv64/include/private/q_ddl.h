/*  q_ddl.h  */

#ifndef _Q_DDL_H
#define _Q_DDL_H

typedef struct {
    objHEADER		m_objHdr;   /* указатель на управляющий структуру объекта */
    objHEADER		*m_owner;    /* ссылка на объект которому принадлежит очередь */
	volatile int	m_lock;     /* Атрибут блокирования очереди */
    volatile int    m_dpcCnt;   /* Счетчик отложенных вызовов процедуры m_dpcRtn */
    funcptr_t		m_dpcRtn;   /* Процедура обработки отложенных запросов */
    queNODE			*m_first; 
	ddlHEAD			m_dll;
} queDDL_HEAD;

extern queCONTROL_ID queFifoCtrlId;
extern queCONTROL_ID queLifoCtrlId;
extern queCONTROL_ID quePrioCtrlId;
extern queCONTROL_ID queSpanCtrlId;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _Q_DDL_H */
