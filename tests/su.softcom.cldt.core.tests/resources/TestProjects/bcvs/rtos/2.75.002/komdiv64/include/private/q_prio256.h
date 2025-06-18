
/*  q_prio256.h  */

#ifndef _Q_PRIO256_H
#define _Q_PRIO256_H

typedef struct {
	unsigned char         m_minor[32];
	ddlHEAD     m_dll[256];
} quePRIO256_MAP;

typedef struct {
    objHEADER	m_objHdr;   /* указатель на управляющий структуру объекта */
    objHEADER  *m_owner;    /* ссылка на объект которому принадлежит очередь */
	int			m_lock;     /* Атрибут блокирования очереди */
    int         m_dpcCnt;   /* Счетчик отложенных вызовов процедуры m_dpcRtn */
    funcptr_t     m_dpcRtn;   /* Процедура обработки отложенных запросов */
    queNODE    *m_first; 
	u_long       m_major;
    quePRIO256_MAP *m_map;
	u_long       m_reserve;
    quePRIO256_MAP m_qplist;
} quePRIO256_HEAD;

extern queCONTROL_ID quePrio256CtrlId;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif  /* _Q_PRIO256_H */

