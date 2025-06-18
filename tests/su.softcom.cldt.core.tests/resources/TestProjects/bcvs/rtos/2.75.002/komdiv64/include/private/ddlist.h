/*  ddlist.h  */
/*  обслуживание двусвязных списков  */

#ifndef _DDLIST_H
#define _DDLIST_H

typedef struct st_ddl_node {
	struct st_ddl_node *next;
	struct st_ddl_node *prev;
} ddlNODE;

typedef struct {
	ddlNODE *first;
	ddlNODE *c_null;
	ddlNODE *last;
} ddlHEAD;

#define ddlHead( head_lst )      ( (ddlNODE*)( head_lst) )
#define ddlTail( head_lst )      ( (ddlNODE*) &((head_lst)->c_null) )
#define ddlFirst( head_lst )     ( (ddlIs_empty(head_lst)) ? NULL : (head_lst)->first )
#define ddlLast( head_lst )      ( (ddlIs_empty(head_lst)) ? NULL : (head_lst)->last )
#define ddlNext( node )          ( (ddlIs_last((node)->next)) ? NULL : (node)->next )
#define ddlPrev( node )          ( (ddlIs_first((node)->prev)) ? NULL : (node)->prev )
#define ddlIs_empty( head_lst )  ( (head_lst)->first == ddlTail(head_lst) )
#define ddlIs_first( node )      ( (node)->prev == NULL )
#define ddlIs_last( node )       ( (node)->next == NULL )
#define ddlIs_one_node(head_lst) ( (head_lst)->first == (head_lst)->last )
#define ddlIs_null( head_lst )   ( (head_lst) == NULL )
#define ddlIs_node_null(node) 	 ( (node) == NULL )
#define ddlIs_node_head( head_lst, node ) ( (node) == ddlHead(head_lst) )
#define ddlIs_node_tail( head_lst, node ) ( (node) == ddlTail(head_lst) )
#define ddlIs_nodes_equal( node1, node2 ) ( (node1) == (node2) )

#define ddlInsertFirst( head_lst, node ) ddlInsert( ddlHead(head_lst), node )
#define ddlFlush( head_lst )     ddlInitEmpty( head_lst )


#ifdef __cplusplus
extern "C" {
#endif

void      ddlInit ( ddlHEAD *head_lst, void *addr, int n_nodes, int node_size );
void      ddlInitEmpty ( ddlHEAD *head_lst );
void      ddlExtract ( ddlNODE *node );
ddlNODE  *ddlGetFirst ( ddlHEAD *head_lst );
void      ddlInsert ( ddlNODE *after, ddlNODE *node );
void      ddlAppend ( ddlHEAD *head_lst, ddlNODE *node );
ddlNODE  *ddlEach (	ddlHEAD *head_lst, funcptr_t func, void *arg );
void ddlMove ( ddlHEAD *head_lstdes, ddlHEAD *head_lstsrc );

void      ddlExtractProtect ( ddlNODE *node );
ddlNODE * ddlGetFirstProtect ( ddlHEAD *head_lst );
void      ddlInsertProtect ( ddlNODE *after, ddlNODE *node );
void      ddlAppendProtect ( ddlHEAD *head_lst, ddlNODE *node );

#ifdef __cplusplus
}
#endif

#endif  /* _DDLIST_H */

