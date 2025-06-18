#ifndef __spiDrv_h_
#define __spiDrv_h_
/*
 * SPI definitions
 */
#define SPI_MODE	0	/* Operating Mode Register */
#define SPI_TIME	0x4	/* Timing Register */
#define SPI_CTL		0x8	/* Control Register */
#define SPI_STAT	0xc	/* Status Register */
#define SPI_RXF		0x10	/* Receive FIFO Register */
#define SPI_TXF		0x14	/* Transmit FIFO Register */
#define SPI_CFG		0x18	/* Configuration Register */
#define SPI_FSTAT	0x1c	/* FIFO Status Register */
#define SPI_IEN		0x20	/* Interrupt Enable Register */
#define SPI_IS		0x24	/* Interrupt Status Register */

#define SPI_MODE_DA_n			8 /*gmu: DevAddr*/
#define SPI_MODE_SOV			(1 << 6)
#define SPI_MODE_SOE			(1 << 5)
#define SPI_MODE_MFE			(1 << 4)
#define SPI_MODE_LSBF			(1 << 3)
#define SPI_MODE_CPH			(1 << 2)
#define SPI_MODE_CPO			(1 << 1)
#define SPI_MODE_MST			(1 << 0)
#define SPI_CTRL_TXDM			(1 << 14)
#define SPI_CTRL_CTXF			(1 << 13)
#define SPI_CTRL_CRXF			(1 << 12)
#define SPI_CTRL_ETX			(1 << 11)
#define SPI_CTRL_STX			(1 << 10)
#define SPI_CTRL_ERX			(1 << 9)
#define SPI_CTRL_SRX			(1 << 8)
#define SPI_CTRL_TRF_COUNT_MASK		(0xff)
#define SPI_STAT_LB			(1 << 17)
#define SPI_STAT_RXA			(1 << 16)
#define SPI_IS_TXD			(1 << 9)
#define SPI_IS_RXD			(1 << 8)
#define SPI_IS_TXE			(1 << 7)
#define SPI_IS_RXFF			(1 << 6)
#define SPI_IS_TXFOW			(1 << 5)
#define SPI_IS_RXFOW			(1 << 4)
#define SPI_IS_MSTROF			(1 << 3)
#define SPI_IS_SLVROF			(1 << 2)
#define SPI_IS_TXOBD			(1 << 1)
#define SPI_IS_MF			(1 << 0)
#define SPI_IEN_TXD			(1 << 9)
#define SPI_IEN_RXD			(1 << 8)
#define SPI_IEN_TXE			(1 << 7)
#define SPI_IEN_RXFF			(1 << 6)
#define SPI_IEN_TXFOW			(1 << 5)
#define SPI_IEN_RXFOW			(1 << 4)
#define SPI_IEN_MSTROF			(1 << 3)
#define SPI_IEN_SLVROF			(1 << 2)
#define SPI_IEN_TXOBD			(1 << 1)
#define SPI_IEN_MF			(1 << 0)
#define SPI_FSTAT_RXF_NUM_ENTRIES_MASK	(0x3f)
#define SPI_TIME_CLOCK_DIV_MASK		(0xff)


void spi_flash_read(unsigned adr_from,unsigned adr_to,unsigned size);

#endif  //__spiDrv_h_



