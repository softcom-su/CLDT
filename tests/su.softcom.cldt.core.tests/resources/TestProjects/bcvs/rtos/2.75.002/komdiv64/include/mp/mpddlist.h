/*  mpddlist.h  */
/*  обслуживание двусвязных списков в общей памяти */

#ifndef _MPDDLIST_H
#define _MPDDLIST_H
#include <sys/types.h>

typedef struct st_mpddl_node {
	struct st_mpddl_node *next;
	struct st_mpddl_node *prev;
} mpddlNODE;

typedef struct {
	mpddlNODE *first;
	mpddlNODE *last;
} mpddlHEAD;

/*размер массива для чтения списка узлов*/
#define mpddlGETLIST_MAXSIZE   20
#define mpddlNODE_LOCAL(node)  ((mpddlNODE*)mpAdrLocal((u_long)(node)))
#define mpddlNODE_GLOBAL(node) ((mpddlNODE*)mpAdrOffset(node))

#define mpddlFirst( head_lst )     (mpddlIs_empty(head_lst))? NULL : \
              mpddlNODE_LOCAL((head_lst)->first)
#define mpddlLast( head_lst )      (mpddlIs_empty(head_lst)) ? NULL : \
              mpddlNODE_LOCAL((head_lst)->last)
#define mpddlNext( node )          (mpddlIs_last(node)) ? NULL : \
              mpddlNODE_LOCAL((node)->next)
#define mpddlPrev( node )          (mpddlIs_first(node)) ? NULL : \
              mpddlNODE_LOCAL((node)->prev)

#define mpddlIs_empty( head_lst )  ( (head_lst)->first == NULL )
#define mpddlIs_first( node )      ( (node)->prev == NULL )
#define mpddlIs_last( node )       ( (node)->next == NULL )
#define mpddlIs_node_null(node) 	 ( (node) == NULL )

#ifdef __cplusplus
extern "C" {
#endif

void      mpddlInit ( mpddlHEAD *head_lst, void *addr, int n_nodes, int node_size );
void      mpddlInitEmpty ( mpddlHEAD *head_lst );
void      mpddlExtract ( mpddlHEAD *head_lst, mpddlNODE *node );
mpddlNODE  *mpddlGetFirst ( mpddlHEAD *head_lst );
void      mpddlAppend ( mpddlHEAD *head_lst, mpddlNODE *node );
mpddlNODE  *mpddlEach (	mpddlHEAD *head_lst, funcptr_t func, void *arg );
int mpddlGetList(	mpddlHEAD *head_lst, mpddlNODE **nodes, int maxnodes );
/*void mpddlMove ( mpddlHEAD *head_lstdes, mpddlHEAD *head_lstsrc );*/

#ifdef __cplusplus
}
#endif

#endif  /* _MPDDLIST_H */

