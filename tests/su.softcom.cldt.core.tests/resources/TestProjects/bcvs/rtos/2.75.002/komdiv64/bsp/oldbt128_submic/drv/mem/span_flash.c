    #include "span_flash.h"
#include <stdlib.h>


#define FLASH_MAX_PAGE_SIZE	0x100

typedef struct {
	const char		*const devname;
	const ssize_t		size;
	const ssize_t		block_size;
	uint8_t			block_erase_cmd;
} SPAN_FLASH_DEV;

SPAN_FLASH_DEV span_flash_dev = 
{
	"span0:1",
	0x1000000,
	0x10000,
	0x20,
};

typedef struct {
	const char		*const devname;
	volatile SPAN_CTRL_REGS	*regs;
	short int number;
} SPAN_CTRL_DEV;


SPAN_CTRL_DEV span_ctrl_dev =
{
	"span0",
	(SPAN_CTRL_REGS*) FLASH_REGS,
	1,
};

static int span_flash_locked(FLASH_DESC* f, unsigned offset)
{
	return FALSE;
}

static int span_flash_show(FLASH_DESC *f)
{
	printf("sector: %dK, size: %dM\n",pSysFlashDesc->sectorSize/0x400,pSysFlashDesc->size/0x100000);
	return 0;
}

static void span_prepare()
{

			/* end transmitting and receiving, clear FIFOs */
			span_ctrl_dev.regs->ctrl = SPAN_CTRL_CTXF | SPAN_CTRL_CRXF |
					SPAN_CTRL_ETX | SPAN_CTRL_ERX;

			/* set up mode, and active chip select signal */
			span_ctrl_dev.regs->mode = (span_ctrl_dev.number << 8) |
					SPAN_MODE_SOE | SPAN_MODE_CPH |
					SPAN_MODE_CPO | SPAN_MODE_MST;
			span_ctrl_dev.regs->mode |= SPAN_MODE_SOV;

			/* set up timing */
			span_ctrl_dev.regs->time = 1; /* range from 1 to 6 */

			/* disable interrupt */
			span_ctrl_dev.regs->ienbl = 0;

			/* clear interrupt status */
			span_ctrl_dev.regs->istat = -1;
}

static void span_end()
{
		/* end transmitting and receiving */
		span_ctrl_dev.regs->ctrl = SPAN_CTRL_ETX | SPAN_CTRL_ERX;

		/* set up mode and inactive chip select signal */
		span_ctrl_dev.regs->mode &= ~SPAN_MODE_SOV;

}


static int span_free(FLASH_DESC *f, unsigned offset, unsigned size)
{
	unsigned int n, i, portion=1024, portion1;
	void *buf = (void *)calloc(portion,1);
	unsigned char* buf1= (unsigned char *)calloc(portion,1);
	portion1 = size % portion;

	for (i=0; i<portion; i++)
	{
		buf1[i]=0xff;
	}

	n=size/portion;
	for (i=0; i<n; i++)
	{
		span_flash_read(f, offset, buf, portion);
		if (memcmp(buf,buf1,portion) != 0)
		{
			free(buf);
			free(buf1);
			return 0;
		}
	}	

	span_flash_read(f, offset, buf, portion1);

	if(memcmp(buf,buf1,portion1) != 0)
	{
		free(buf);
		free(buf1);
		return 0;
	}
	
	free(buf);
	free(buf1);
	return 1;
	
}

static ssize_t span_read(void *buf, size_t nbyte)
{
	ssize_t			cnt = 0;
	ssize_t			txf_cnt;
	uint32_t		tmp;
	volatile uint8_t	*rxf_buf = buf;

		/* end transmitting and receiving, clear FIFOs */
		span_ctrl_dev.regs->ctrl = SPAN_CTRL_CTXF | SPAN_CTRL_CRXF |
				SPAN_CTRL_ETX | SPAN_CTRL_ERX;

		while (cnt < nbyte) {;
			txf_cnt = nbyte - cnt;

			if (txf_cnt > sizeof(uint32_t))
				txf_cnt = sizeof(uint32_t);
			span_ctrl_dev.regs->ctrl = SPAN_CTRL_TXDM | SPAN_CTRL_STX |
					SPAN_CTRL_SRX | (txf_cnt & SPAN_CTRL_TrfCountMask);
			/* wait for received data */
			while (!(span_ctrl_dev.regs->fstat & SPAN_FSTAT_RxFNumEntriesMask));
			tmp = span_ctrl_dev.regs->rxf;

			do {
				*rxf_buf++ = tmp;
				tmp >>= 8;
				cnt++;
			} while (cnt < nbyte && cnt % sizeof(uint32_t));
		}

	return cnt;
}

static ssize_t span_write(const void *buf, size_t nbyte)
{
	ssize_t			cnt = 0;
	ssize_t			txf_cnt;
	ssize_t			byte_cnt;
	uint32_t		tmp = 0;
	const uint8_t		*txf_buf = buf;

		/* end transmitting and receiving, clear FIFOs */
		span_ctrl_dev.regs->ctrl = SPAN_CTRL_CTXF | SPAN_CTRL_CRXF |
				SPAN_CTRL_ETX | SPAN_CTRL_ERX;

		while (cnt < nbyte) {

			txf_cnt = nbyte - cnt;

			if (txf_cnt > sizeof(uint32_t))
				txf_cnt = sizeof(uint32_t);

			byte_cnt = sizeof(uint32_t);

			do {
				tmp >>= 8;
				tmp |= *txf_buf++ << 24;
				cnt++;
				byte_cnt--;
			} while (cnt < nbyte && byte_cnt);

			tmp >>= 8 * byte_cnt;

			span_ctrl_dev.regs->istat = SPAN_IS_TXD;
			span_ctrl_dev.regs->txf = tmp;
			span_ctrl_dev.regs->ctrl = SPAN_CTRL_STX | txf_cnt;

			/* wait for a transfer */
			while (!(span_ctrl_dev.regs->istat & SPAN_IS_TXD));
		}

	return cnt;
}

inline static void span_flash_enable_writting()
{
	uint8_t cmd = 0x06;

		span_prepare();

		span_write(&cmd, sizeof(cmd));

		span_end();
}

inline static uint8_t span_flash_get_status()
{
	uint8_t cmd = 0x05;
	uint8_t val = 0;

		span_prepare();

		span_write(&cmd, sizeof(cmd));
		span_read(&val, sizeof(val));

		span_end();

	return val;
}

static ssize_t	span_flash_read(FLASH_DESC *f, uint32_t addr, void *buf, size_t size)
{

	uint32_t cmd = (0x03 << 24) | (addr & ((1 << 24) - 1));

	if (!buf || addr + size > span_flash_dev.size)
		return -1;
	span_prepare();
	span_write(&cmd, sizeof(cmd));
	size = span_read(buf, size);
	span_end();

	return 0;
}


static ssize_t	span_flash_write(FLASH_DESC *f, uint32_t addr, void *buf, size_t size)
{
	size_t cnt = 0;
	size_t page_size;
	uint32_t cmd;

	if (!buf || addr + size > span_flash_dev.size)
		return -1;



		page_size = size - cnt;

		if (page_size > FLASH_MAX_PAGE_SIZE)
			page_size = FLASH_MAX_PAGE_SIZE;

		

	while (cnt < size) {
		span_flash_enable_writting();
		span_prepare();
		cmd = (0x02 << 24) | ((addr + cnt) & ((1 << 24) - 1));

		span_write(&cmd, sizeof(cmd));
		cnt += span_write((void*) ((size_t) buf + cnt), page_size);

		span_end();
		while (span_flash_get_status() & (1 << 0));
	}
	
	return 0;
}

static int span_flash_block_is_used(uint32_t addr)
{
	int64_t buf[512 / sizeof(int64_t)];
	size_t size;
	size_t cnt;
	uint32_t block_base;

	if (addr > span_flash_dev.size)
		return -1;

	block_base = addr & ~(span_flash_dev.block_size - 1);

	for (size = 0; size < span_flash_dev.block_size; size += sizeof(buf)) {
	span_flash_read(0, block_base + size, buf, sizeof(buf));
			
		for (cnt = 0; cnt < sizeof(buf) / sizeof(int64_t); cnt++) {
			if (buf[cnt] != (int64_t) -1)
				return 1;
		}
	}

	return 0;
}

static int span_flash_block_erase(FLASH_DESC* f, ERASE_REQUEST* request)
{
	uint32_t	cmd;

	if (request->offset > span_flash_dev.size)
		return 0;

	cmd = (span_flash_dev.block_erase_cmd << 24) | (request->offset & ((1 << 24) - 1));

	span_flash_enable_writting();

	span_prepare();

	span_write(&cmd, sizeof(cmd));

	span_end();

	while (span_flash_get_status() & (1 << 0));

	request->state = ERASE_SUCCESS;
	request->callback(request);

	return 0;
}

static int span_flash_bulk_erase()
{
	uint8_t cmd = 0xc7;

	span_flash_enable_writting();

	span_prepare();

	span_write(&cmd, sizeof(cmd));

	span_end();

	while (span_flash_get_status() & (1 << 0));

	return 1;
}

FLASH_DESC* spanFlashInit()
{
	FLASH_DESC* f = calloc(1,sizeof(FLASH_DESC));
	f->size  = 0x1000000;
	f->nSectors = 256;
	f->sectorSize = 0x10000;
 	f->sectorBits = 16;
	f->read    = (void*)span_flash_read;
	f->write   = (void*)span_flash_write;
	f->erase   = span_flash_block_erase;
	f->lock    = NULL;
	f->unlock  = NULL;
	f->locked  = span_flash_locked;
	f->show    = span_flash_show;
	f->free    = span_free;
	f->suspend = NULL;
	f->resume  = NULL;
	return f;
}

