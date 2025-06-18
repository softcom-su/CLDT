/*
 * rio.h
 *
 *  Created on: 27.01.2011
 *      Author: nkigor
 */

#ifndef RIOLOCAL_H_
#define RIOLOCAL_H_
#include <stddef.h>

/* rio host type */

#define RIO_HOST_INVALID        (-1)
#define RIO_HOST_UNKNOWN        0
#define RIO_HOST_K128           1
#define RIO_HOST_K64            2
#define RIO_HOST_K64N           3

/* device vendor ID */

#define RIO_VENDOR_ID_NIISI     0x0074

/* device ID */

#define RIO_DEVICE_ID_K128      0x0128
#define RIO_DEVICE_ID_K64RIO    0x8717

#define DMA_INT_CP2             0x00000001
#define DMA_INT_RX              0x00000004
#define DMA_INT_VSK             0x00000008
#define DMA_INT_TX              0x00000010

#define DMA_RX_CTRL_START       0x00000001
#define DMA_RX_CTRL_STOP        0x00000002
#define DMA_RX_CTRL_STOP_INT    0x00000004
#define DMA_RX_CTRL_SEL9        0x00000008

#define DMA_RX_INT_CNT0         0x00000001
#define DMA_RX_INT_CNT1         0x00000002
#define DMA_RX_INT_CNT          0x00000004
#define DMA_RX_INT_LETTER       0x00000008
#define DMA_RX_INT_MSEG         0x00000010
#define DMA_RX_INT_LAST         0x00000020
#define DMA_RX_INT_MSG_DONE     0x00000040
#define DMA_RX_INT_SID9         0x00000080
#define DMA_RX_INT_CNT9_0       0x00000100
#define DMA_RX_INT_CNT9_1       0x00000200
#define DMA_RX_INT_CNT9         0x00000400
#define DMA_RX_INT_SEG_DONE     0x00000800
#define DMA_RX_INT_FIFO_OVF     0x00001000
#define DMA_RX_INT_FIFO_NE      0x00002000

#define DMA_TX_CTRL_START       0x00000001
#define DMA_TX_CTRL_STOP        0x00000002
#define DMA_TX_CTRL_FHALT       0x00000004
#define DMA_TX_CTRL_FINT        0x00000008

#define DMA_TX_STAT_HALT        0x00000001
#define DMA_TX_STAT_INT         0x00000002
#define DMA_TX_STAT_SEG_ERR     0x00000004
#define DMA_TX_STAT_OVERTIME    0x00000008
#define DMA_TX_STAT_FIFO_OVF    0x00000010
#define DMA_TX_STAT_FOVRTERR    0x00000020
#define DMA_TX_STAT_FUNDERR     0x00000040
#define DMA_TX_STAT_FSEGERR     0x00000080
#define DMA_TX_STAT_BUF_TOUT    0x00000100
#define DMA_TX_STAT_DESC_MASK   0x000f0000

#define DMA_TX_FLAG_HALT        0x01
#define DMA_TX_FLAG_INT         0x02
#define DMA_TX_FLAG_OVERTIME    0x04
#define DMA_TX_FLAG_SEG_ERR     0x08
#define DMA_TX_FLAG_FIFO_OVF    0x10
#define DMA_TX_FLAG_FIFO_WRITE  0x20
#define DMA_TX_FLAG_VALID       0x80

#define RIO_INT_CPU             0x00000001
#define RIO_INT_CSR             0x00000002
#define RIO_INT_DBI             0x00000004
#define RIO_INT_DBO             0x00000008
#define RIO_INT_ERR_PKT         0x00000010
#define RIO_INT_UNSUPP_PKT      0x00000020


/* bits reserved for demultiplexor in doorbell data */

#define DB_HANDLER_BITS         4
#define DB_HANDLER_COUNT        (1 << DB_HANDLER_BITS)
#define DB_HANDLER_MASK         (DB_HANDLER_COUNT-1)

#define SYNC_MAGIC              0x55aa1234



// static struct fifo_queue fifo;
int rioDbOut(unsigned dstID, unsigned i, unsigned data);
int _rioDbOut(unsigned dstID, unsigned i, unsigned data);
void rioIntr();
int rioDbIntConnect(unsigned irq, vfuncptr_t rtn, int arg);
int boardRioIsReady(int id);
int boardRioIdMax();

#endif /* RIOLOCAL_H_ */
