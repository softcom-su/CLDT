/* rpcVar.h - заголовочный файл для переменных RPC */

#ifndef _RPC_VAR_H
#define _RPC_VAR_H

#include <sys/select.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/svc.h>
#include <private/thread.h>
#include <private/kernel.h>

#define MAX_MARSHEL_SIZE 20
#define RPC_ERRBUF_SIZE	 256

/******************************************************************/
/* статические переменные из модулей rpc перенесены               */
/* в структуру RPC_SVR, память для которой выделяется динамически.*/
/*  Указатель на эту памяить хранится в TCB.                      */
/******************************************************************/
typedef struct rpcTchVar {

 /* из модуля auth_none.c: */
 struct authnone_private {
	 AUTH	no_client;
	 char	marshalled_client[MAX_MARSHEL_SIZE];
	 u_int	mcnt;
 } authnone_private;

 /* из модуля clnt_simple.c: */
 struct callrpc_private {
	 CLIENT	*client;
	 int	socket;
	 int	oldprognum, oldversnum, valid;
	 char	oldhost[50]; /*	char *oldhost */
 } callrpc_private;

  /* из модуля svc.c: */
 struct svc {
 	SVCXPRT *xports[FD_SETSIZE];
	 fd_set svc_fdset;
  /*
   * The services list
   * Each entry represents a set of procedures (an rpc program).
   * The dispatch routine takes request structs and runs the
   * apropriate procedure.
   */
  struct svc_callout {
	  struct svc_callout *sc_next;
	  uint32_t		    sc_prog;
	  uint32_t		    sc_vers;
	  void		    (*sc_dispatch)();
  } *svc_head;
 } svc ;

 /* из модуля svc_simple.c: */
 struct svc_simple {
  struct proglst {
	  char *(*p_progname)();
	  int  p_prognum;
	  int  p_procnum;
	  xdrproc_t p_inproc, p_outproc;
	  struct proglst *p_nxt;
  } *proglst;
  SVCXPRT *transp;
  struct proglst *pl;
 } svc_simple ;

 /* статические переменные для удаленной файловой системы (NFS).
 */
 void (*nfs_threadRemove)(void *nfsPool) ;
 void *nfsPool ;

 /* так как модули clnt_raw и svc_raw используются достаточно редко,
  * то статические переменные этих модулей заводятся динамически при
  * первом обращении. Кроме того для модулей используется общий буфер строк.
 */
 struct clntraw_private *clnt_raw ;
 struct svcraw_private *svc_raw;
 void (*clnt_rawExit)();
 void (*svc_rawExit)();
 char *_raw_buf;			/* буфер для строк clnt/svc */
 char *errBuf;			  /* буфер для сообщений об ошибках */

 /* A handle on why an rpc creation routine failed (returned NULL.) */
 struct rpc_createerr thread_rpc_createerr;

} RPC_SVR;

/* macro for the pointer to the current task's rpc statics */

#define threadRpcSVR	((RPC_SVR *)(threadExecuting)->m_pRPC)

/* macro for the current task's rpc_createerr structure */

#define rpc_createerr	(threadRpcSVR->thread_rpc_createerr)

__BEGIN_DECLS

extern	void	clnt_genericInclude (void) ;
extern	void	clnt_rawInclude (void) ;
extern	void	clnt_simpleInclude (void) ;
extern	void	pmap_getmapsInclude (void) ;
extern	void	svc_rawInclude (void) ;
extern	void	svc_simpleInclude (void) ;
extern	void	xdr_floatInclude (void) ;
extern	void	xdr_int64Include (void) ;

__END_DECLS


#endif /* _RPC_VAR_H */
