#ifndef _SPAN_FLASH_H_
#define _SPAN_FLASH_H_
#include <sys/types.h>
#include <stddef.h>
#include <sys/types.h>
#include <system.h>
#include <ftlDrv.h>

#define FLASH_REGS	K1_TO_TLB(0xbff00000)
#define SPAN_MODE_SOV		(1 << 6)
#define SPAN_MODE_SOE		(1 << 5)
#define SPAN_MODE_MFE		(1 << 4)
#define SPAN_MODE_LSBF		(1 << 3)
#define SPAN_MODE_CPH		(1 << 2)
#define SPAN_MODE_CPO		(1 << 1)
#define SPAN_MODE_MST		(1 << 0)
#define SPAN_CTRL_TXDM		(1 << 14)
#define SPAN_CTRL_CTXF		(1 << 13)
#define SPAN_CTRL_CRXF		(1 << 12)
#define SPAN_CTRL_ETX		(1 << 11)
#define SPAN_CTRL_STX		(1 << 10)
#define SPAN_CTRL_ERX		(1 << 9)
#define SPAN_CTRL_SRX		(1 << 8)
#define SPAN_CTRL_TrfCountMask	(0xff)
#define SPAN_STAT_LB		(1 << 17)
#define SPAN_STAT_RXA		(1 << 16)
#define SPAN_IS_TXD		(1 << 9)
#define SPAN_IS_RXD		(1 << 8)
#define SPAN_IS_TXE		(1 << 7)
#define SPAN_IS_RXFF		(1 << 6)
#define SPAN_IS_TXFOW		(1 << 5)
#define SPAN_IS_RXFOW		(1 << 4)
#define SPAN_IS_MSTROF		(1 << 3)
#define SPAN_IS_SLVROF		(1 << 2)
#define SPAN_IS_TXOBD		(1 << 1)
#define SPAN_IS_MF		(1 << 0)
#define SPAN_IEN_TXD		(1 << 9)
#define SPAN_IEN_RXD		(1 << 8)
#define SPAN_IEN_TXE		(1 << 7)
#define SPAN_IEN_RXFF		(1 << 6)
#define SPAN_IEN_TXFOW		(1 << 5)
#define SPAN_IEN_RXFOW		(1 << 4)
#define SPAN_IEN_MSTROF		(1 << 3)
#define SPAN_IEN_SLVROF		(1 << 2)
#define SPAN_IEN_TXOBD		(1 << 1)
#define SPAN_IEN_MF		(1 << 0)
#define SPAN_FSTAT_RxFNumEntriesMask	(0x1f)

static ssize_t span_read(void *buf, size_t nbyte);
static ssize_t span_write(const void *buf, size_t nbyte);

typedef struct {
	uint32_t	mode;	/* Operating Mode Register */
	uint32_t	time;	/* Timing Register */
	uint32_t	ctrl;	/* Control Register */
	uint32_t	stat;	/* Status Register */
	uint32_t	rxf;	/* Receive FIFO Register */
	uint32_t	txf;	/* Transmit FIFO Register */
	uint32_t	conf;	/* Configuration Register */
	uint32_t	fstat;	/* FIFO Status Register */
	uint32_t	ienbl;	/* Interrupt Enable Register */
	uint32_t	istat;	/* Interrupt Status Register */
} SPAN_CTRL_REGS;

static ssize_t	span_flash_read(FLASH_DESC *f, uint32_t addr, void *buf, size_t size);
static ssize_t	span_flash_write(FLASH_DESC *f, uint32_t addr, void *buf, size_t size);

static int span_flash_block_is_used(uint32_t addr);

static int span_flash_block_erase(FLASH_DESC* f, ERASE_REQUEST* request);
static int span_flash_bulk_erase();


#endif /* _SPAN_FLASH_H_ */
