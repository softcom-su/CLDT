/*
 * riodriver.h
 *
 *  Created on: 19.07.2013
 *      Author: nkigor
 */

#ifndef RIODRIVER_H_
#define RIODRIVER_H_

struct chan_bufnode;
#define CHAN_MBOX  4
#define NET_MBOX   5
#define DATA_MBOX  6


typedef void (*riohook_t)(int cpu, struct chan_bufnode *b, int error);
void boardRioSetRxHook(int mbox, riohook_t hook);
void boardRioSetTxHook(int mbox, riohook_t hook);
int  boardRioInit( int nBuf, int bufsize );

typedef struct {
   int mtu;
   int sizeAlign;
   int addrAlign;
} BOARD_RIO_PARAM;
void  boardRioParamGet(BOARD_RIO_PARAM* p);
int   boardRioSendMsg( int cpu, int mbox, struct chan_bufnode* pBufNode, int size );
int   boardRioRecvMsg( int cpu, int mbox, struct chan_bufnode* pBufNode );
void  boardRioFreeBuf( int mbox, void* pBuf );
void  boardRioSetAlias(int rio_id,int alias);
int   boardRioIdMax();


#endif /* RIODRIVER_H_ */
