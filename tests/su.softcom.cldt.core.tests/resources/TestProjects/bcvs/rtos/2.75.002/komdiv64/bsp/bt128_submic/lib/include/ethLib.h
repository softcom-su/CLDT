/* ethLib.h - ethernet raw I/O routines and hooks */

#ifndef	__INCethLibh
#define	__INCethLibh

#include <system.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/if_ether.h>

/* frame transmit status */

#define	XMIT_NONE		0	/* initial state */
#define	XMIT_QUEUED		1	/* frame queued to device tx list */
#define	XMIT_OK			2	/* frame successefully transmitted */
#define	XMIT_ERROR		3	/* frame transmit error */

/* node for list of xmit routines */

typedef struct eth_xmit_node {
    struct eth_xmit_node*	next;	/* ptr to next node */
    struct ifnet*		ifp;	/* entry key: interface descriptor */
    FUNCPTR			rtn;	/* direct transmit routine */
} ETH_XMIT_NODE;

/* frame descriptor for transmit routine */

typedef struct {
    struct ether_header*	eh;	/* ethernet header or NULL */
    void*			data;	/* frame data (with header if eh == NULL) */
    unsigned			nBytes;	/* size of frame data without header */
    volatile unsigned		status;	/* frame transmit status (see above) */
} ETH_FRAME;

/* globals */

extern FUNCPTR ethInputHook;
extern FUNCPTR ethOutputHook;

/* function declarations */

int ethResolve(struct ifnet* ifp, char* ip, char* mac, int nTries, int nTicks);
int ethOutput(struct ifnet* ifp, struct ether_header* eh, char* data, int nBytes);
int ethXmitRtnAdd(struct ifnet* ifp, FUNCPTR rtn);
FUNCPTR ethXmitRtnGet(struct ifnet* ifp);
int ethXmit(struct ifnet* ifp, ETH_FRAME* list, int nFrames,
    unsigned timeout_us, FUNCPTR rtn);
int ethXmitFrame(struct ifnet* ifp, unsigned timeout_us, FUNCPTR rtn,
    struct ether_header* eh, char* data, int nBytes);

#endif	/* __INCethLibh */
