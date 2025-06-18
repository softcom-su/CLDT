/*  q_prio32.h  */

#ifndef _Q_PRIO32_H
#define _Q_PRIO32_H

typedef struct {
    objHEADER	m_objHdr;   /* указатель на управляющий структуру объекта */
    objHEADER  *m_owner;    /* ссылка на объект которому принадлежит очередь */
	int	    m_lock;     /* Атрибут блокирования очереди */
    int         m_dpcCnt;   /* Счетчик отложенных вызовов процедуры m_dpcRtn */
    funcptr_t     m_dpcRtn;   /* Процедура обработки отложенных запросов */
    queNODE    *m_first; 
	u_long       m_major;
    ddlHEAD    *m_map;
	u_long       m_reserve;
	ddlHEAD     m_dll[32];
} quePRIO32_HEAD;


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _Q_PRIO32_H */

