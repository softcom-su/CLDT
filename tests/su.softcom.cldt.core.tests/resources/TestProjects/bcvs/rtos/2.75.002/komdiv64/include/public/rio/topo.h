/*
 * topo.h
 *
 *  Created on: Feb 14, 2012
 *      Author: nkalex
 */

#ifndef RIO_TOPO_H_
#define RIO_TOPO_H_

#include <sys/queue.h>

#define RIT_MODELNAME_SIZE  16
#define RIT_MAX_IDS     256

typedef enum
{
    L_UNKNOWN,
    L_SERIAL,
    L_PARALLEL
} rioLinkType;

typedef struct ritNode_tag
{
    UINT32 id;
    UINT32 geoAddr;
    UINT32 caps;
    UINT32 identity;
    char   model[RIT_MODELNAME_SIZE];
    UINT8  nPorts;
    const rioDarDriver *driver;
    int mark;
    UINT8 rtable[RIT_MAX_IDS];
    SLIST_ENTRY(ritNode_tag) entry;
    SLIST_ENTRY(ritNode_tag) frontier;
} ritNode;

typedef struct ritLink_tag
{
    ritNode *node1;
    UINT8    port1;
    ritNode *node2;
    UINT8    port2;
    rioLinkType type;
    SLIST_ENTRY(ritLink_tag) entry;
    SLIST_ENTRY(ritLink_tag) incident;
} ritLink;

typedef SLIST_HEAD(ritNodeList_tag, ritNode_tag) ritNodeList;
typedef SLIST_HEAD(ritLinkList_tag, ritLink_tag) ritLinkList;

struct ritTopo_tag;
typedef struct ritTopo_tag ritTopo;

struct ritTopo_tag
{
    ritNodeList nodes;
    ritLinkList links;
    UINT32 nextSwitchId;
    UINT32 nextEpId;
    UINT32 maxNodeId;
};

ritTopo* ritInit();
void ritDestroy(ritTopo *topo);
ritLink* ritAddLink(ritTopo *topo, ritNode *node1, UINT8 port1,
                    ritNode *node2, UINT8 port2, rioLinkType type);
ritNode* ritAddNode(ritTopo *topo, const rioDarDriver *drv, UINT32 geoAddr);
void ritForEachNode(ritTopo *topo,
                    void (*handler)(ritTopo*, ritNode*, void*), void *arg);
void ritForEachLink(ritTopo *topo,
                    void (*handler)(ritTopo*, ritLink*, void*), void *arg);
ritNode* ritGetNodeById(ritTopo *topo, UINT32 id);
ritNodeList ritExpandFrontier(ritTopo *topo, ritNodeList frontier, ritNode *targetNode);
UINT8 ritGetLinkSrcPort(ritNode *node, ritLink *link);
UINT8 ritGetLinkTrgPort(ritNode *node, ritLink *link);
void ritMakeRoutes(ritTopo *topo);
ritNode* ritGetPeerNode(ritNode *node, ritLink *link);
int ritNodesEqual(const ritNode *n1, const ritNode *n2);
void ritClearNodeMarks(ritTopo *topo);
void ritForEachIncidentLink(ritTopo *topo, ritNode *node,
                            void(*handler)(ritTopo*,ritNode*,ritLink*,void*), void *arg);
void ritRemoveNodes(ritTopo *topo, int (*predicate)(ritNode*));

#endif /* TOPO_H_ */
