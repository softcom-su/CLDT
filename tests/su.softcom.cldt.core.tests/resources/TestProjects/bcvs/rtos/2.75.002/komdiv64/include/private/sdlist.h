/*  sdlist.h  */
/* обслуживание односвязных списков  */

#ifndef _SDLIST_H
#define _SDLIST_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_sdl_node {
	struct st_sdl_node *next;
} sdlNODE;

typedef struct {
	sdlNODE *first;
	sdlNODE *last;
} sdlHEAD;

#define sdlHead( head_lst )      ( sdlNODE*)( head_lst)
#define sdlTail( head_lst )      ((sdlNODE*) &( (head_lst)->last))
#define sdlFirst( head_lst )     (head_lst)->first
#define sdlLast( head_lst )      (head_lst)->last
#define sdlNext( node )          (node)->next
#define sdlFlush( head_lst )       sdlInitEmpty( head_lst )
#define sdlIs_empty( head_lst )  ( (head_lst)->first == 0 )
#define sdlIs_Not_empty( head_lst )  ( (head_lst)->first != 0 )
#define sdlIs_last( node )       ( (node)->next == 0 )
#define sdlIs_one_node(head_lst) ( (head_lst)->first == (head_lst)->last )
#define sdlIs_null( head_lst )   ( head_lst == 0 )
#define sdlIs_node_null(node) 	 ( node == 0 )
#define sdlIs_node_head( head_lst, node ) ( node == sdlHead( head_lst ) )
#define sdlIs_node_tail( head_lst, node ) ( node == sdlTail( head_lst ) )
#define sdlIs_nodes_equal( node1, node2 ) ( node1 == node2 )
#define sdlInsertFirst( head_lst, node ) \
				sdlInsert( head_lst, sdlHead(head_lst), node )

void      sdlInit ( sdlHEAD *head_lst, void *addr, int n_nodes, int node_size );
void      sdlInitEmpty ( sdlHEAD *head_lst );
/*
int       sdlExtract ( sdlHEAD *head_lst, sdlNODE *node );
void      sdlExtractNext( sdlHEAD *sdl_lst, sdlNODE *node );
*/
sdlNODE  *sdlGetFirst ( sdlHEAD *head_lst );
void      sdlInsert ( sdlHEAD *head_lst, sdlNODE *after, sdlNODE *node );
/*
void      sdlAppend ( sdlHEAD *head_lst, sdlNODE *node );
*/
void      sdlFree ( sdlHEAD *head_lst );
sdlNODE  *sdlEach (	sdlHEAD *head_lst, funcptr_t func, void *arg );

 int       sdlExtractProtect( sdlHEAD *head_lst, sdlNODE *node );
 sdlNODE * sdlGetFirstProtect( sdlHEAD *head_lst );
 void      sdlInsertProtect( sdlHEAD *head_lst, sdlNODE *after, sdlNODE *node );
 void      sdlAppendProtect( sdlHEAD *head_lst, sdlNODE *node );

/*----------- sdlExtractNext ---------*/
/*извлечь из списка элемент следующий за указанным */
static __inline void __UNUSED_F sdlExtractNext( sdlHEAD *sdl_lst, sdlNODE *w )
{
  sdlNODE *node = w->next; /*удаляемый*/

  w->next = node->next;
  if( node == sdl_lst->last )
    sdl_lst->last = ( sdlIs_empty( sdl_lst ) )? 0 : w;
}
/*----------- sdlExtract ---------*/
/*извлечь из списка указанный элемент */
static __inline int __UNUSED_F sdlExtract( sdlHEAD *sdl_lst, sdlNODE *node )
{
	sdlNODE *w = sdlHead( sdl_lst );
	while( w )
		if( w->next == node ) {
      sdlExtractNext( sdl_lst, w );
			return 0;
		}
		else
			w = w->next;
	return -1;
}
/*----------- sdlAppend ---------*/
/*добавить в конец списка*/
static __inline void __UNUSED_F sdlAppend( sdlHEAD *head_lst, sdlNODE *node )
{
	if( sdlIs_empty( head_lst ) ) {
		head_lst->first = head_lst->last = node;
	} else {
		head_lst->last->next = node;
		head_lst->last = node;
	}
	node->next = 0;
}


#ifdef __cplusplus
}
#endif

#endif  /* _SDLIST_H */

