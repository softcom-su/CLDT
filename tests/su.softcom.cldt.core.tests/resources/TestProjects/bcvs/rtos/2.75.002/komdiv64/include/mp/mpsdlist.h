/*  mpsdlist.h  */
/* обслуживание односвязных списков общей памяти */

#ifndef _MPSDLIST_H
#define _MPSDLIST_H

typedef struct st_mpsdl_node {
	struct st_mpsdl_node *next;
} mpsdlNODE;

typedef struct {
	mpsdlNODE *first;
	mpsdlNODE *last;
} mpsdlHEAD;

#define mpsdlNODE_LOCAL(node)  ((mpsdlNODE*)mpAdrLocal((u_long)(node)))
#define mpsdlNODE_GLOBAL(node) ((mpsdlNODE*)mpAdrOffset(/*(u_long)*/(node)))

#define mpsdlHead( head_lst )      ( mpsdlNODE*)( head_lst)
#define mpsdlTail( head_lst )      ((mpsdlNODE*) &( (head_lst)->last))

#define mpsdlFirst( head_lst )     ( mpsdlIs_empty( head_lst ))? NULL : \
              mpsdlNODE_LOCAL((head_lst)->first)
#define mpsdlLast( head_lst )      ( mpsdlIs_empty( head_lst ))? NULL : \
              mpsdlNODE_LOCAL((head_lst)->last)
#define mpsdlNext( node )          (mpsdlIs_last( node ))? NULL :  \
              mpsdlNODE_LOCAL((node)->next)
#define mpsdlIs_empty( head_lst )  ( (head_lst)->first == NULL )
#define mpsdlIs_last( node )       ( (node)->next == NULL )
#define mpsdlInsertFirst( head_lst, node ) \
				mpsdlInsert( head_lst, mpsdlHead(head_lst), node )

#ifdef __cplusplus
extern "C" {
#endif

void      mpsdlInit ( mpsdlHEAD *head_lst, void *addr, int n_nodes, int node_size );
void      mpsdlInitEmpty ( mpsdlHEAD *head_lst );
int       mpsdlExtract ( mpsdlHEAD *head_lst, mpsdlNODE *node );
mpsdlNODE  *mpsdlGetFirst ( mpsdlHEAD *head_lst );
void      mpsdlAppend ( mpsdlHEAD *head_lst, mpsdlNODE *node );
void mpsdlInsert ( mpsdlHEAD *head_lst, mpsdlNODE *after, mpsdlNODE *node );
mpsdlNODE *mpsdlEach (	mpsdlHEAD *head_lst, funcptr_t func, void *arg );


#ifdef __cplusplus
}
#endif

#endif  /* _MPSDLIST_H */

