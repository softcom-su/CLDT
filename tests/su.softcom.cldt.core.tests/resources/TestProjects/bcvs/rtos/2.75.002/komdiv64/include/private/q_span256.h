/*  q_span256.h  */

#ifndef _Q_SPAN256_H
#define _Q_SPAN256_H

typedef struct {
    objHEADER	m_objHdr;   /* указатель на управляющий структуру объекта */
    objHEADER  *m_owner;    /* ссылка на объект которому принадлежит очередь */
	int			m_lock;     /* Атрибут блокирования очереди */
    int         m_dpcCnt;   /* Счетчик отложенных вызовов процедуры m_dpcRtn */
    funcptr_t     m_dpcRtn;   /* Процедура обработки отложенных запросов */
    queNODE    *m_first; 
	u_long       m_mapIndex;
    ddlHEAD    *m_map;
	u_long       m_reserve;
    ddlHEAD     m_dll[256];
} queSPAN256_HEAD;

extern queCONTROL_ID queSpan256CtrlId;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _Q_SPAN256_H */

