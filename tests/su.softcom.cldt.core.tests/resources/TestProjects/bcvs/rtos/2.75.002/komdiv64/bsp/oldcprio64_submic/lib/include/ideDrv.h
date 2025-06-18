/* ideDrv.h - IDE driver header */

#ifndef	__INCideDrvh
#define	__INCideDrvh

#include <blkIo.h>
#if OS_VERSION == 206
  #include <inttypes.h>
#endif

/* default timeouts in seconds */

#define	TIMEOUT_IO	10	/* io operations irq timeout */
#define	TIMEOUT_CMD	1	/* nondata operations irq timeout */
#define	TIMEOUT_SRST	31	/* software reset busy timeout */
#define	TIMEOUT_SLEEP	2	/* enter sleep mode irq timeout */
#define	TIMEOUT_ACTIVE	30	/* io irq timeout after wakeup */
#define	TIMEOUT_STANDBY	30	/* enter standby mode irq timeout */

/* io modes */

#define	IDE_IO_MODE_PIO_DEF	0			/* default pio mode */
#define	IDE_IO_MODE_NO_IORDY	1			/* default pio mode, no iordy */
#define	IDE_IO_MODE_PIO		(1 << 3)		/* pio, flow control */
#define	IDE_IO_MODE_PIO0	(IDE_IO_MODE_PIO|0)	/* pio 0 */
#define	IDE_IO_MODE_PIO1	(IDE_IO_MODE_PIO|1)	/* pio 1 */
#define	IDE_IO_MODE_PIO2	(IDE_IO_MODE_PIO|2)	/* pio 2 */
#define	IDE_IO_MODE_PIO3	(IDE_IO_MODE_PIO|3)	/* pio 3 */
#define	IDE_IO_MODE_PIO4	(IDE_IO_MODE_PIO|4)	/* pio 4 */
#define	IDE_IO_MODE_SDMA	(2 << 3)		/* single word dma */
#define	IDE_IO_MODE_SDMA0	(IDE_IO_MODE_SDMA|0)	/* single word dma 0 */
#define	IDE_IO_MODE_SDMA1	(IDE_IO_MODE_SDMA|1)	/* single word dma 1 */
#define	IDE_IO_MODE_SDMA2	(IDE_IO_MODE_SDMA|2)	/* single word dma 2 */
#define	IDE_IO_MODE_MDMA	(4 << 3)		/* multiword dma */
#define	IDE_IO_MODE_MDMA0	(IDE_IO_MODE_MDMA|0)	/* multiword dma 0 */
#define	IDE_IO_MODE_MDMA1	(IDE_IO_MODE_MDMA|1)	/* multiword dma 1 */
#define	IDE_IO_MODE_MDMA2	(IDE_IO_MODE_MDMA|2)	/* multiword dma 2 */
#define	IDE_IO_MODE_UDMA	(8 << 3)		/* ultra dma */
#define	IDE_IO_MODE_UDMA0	(IDE_IO_MODE_UDMA|0)	/* ultra dma 0 */
#define	IDE_IO_MODE_UDMA1	(IDE_IO_MODE_UDMA|1)	/* ultra dma 1 */
#define	IDE_IO_MODE_UDMA2	(IDE_IO_MODE_UDMA|2)	/* ultra dma 2 */
#define	IDE_IO_MODE_UDMA3	(IDE_IO_MODE_UDMA|3)	/* ultra dma 3 */
#define	IDE_IO_MODE_UDMA4	(IDE_IO_MODE_UDMA|4)	/* ultra dma 4 */
#define	IDE_IO_MODE_UDMA5	(IDE_IO_MODE_UDMA|5)	/* ultra dma 5 */

/* ioctl commands */

#define	IDE_IOCTL			0x80000000		/* this is ideDrv ioctl */
#define	IDE_BLOCK			0x40000000		/* take mutex befor executing */
#define	IDE_IOCTL_IDLE			(1|IDE_BLOCK|IDE_IOCTL)	/* enter idle mode */
#define	IDE_IOCTL_STANDBY		(2|IDE_BLOCK|IDE_IOCTL)	/* enter standby mode */
#define	IDE_IOCTL_SLEEP			(3|IDE_BLOCK|IDE_IOCTL)	/* enter sleep mode */
#define	IDE_IOCTL_TIMEOUT_IO_SET	(4|IDE_IOCTL)		/* set io irq timeout */
#define	IDE_IOCTL_TIMEOUT_IO_GET	(5|IDE_IOCTL)		/* get io irq timeout */
#define	IDE_IOCTL_TIMEOUT_CMD_SET	(6|IDE_IOCTL)		/* set nondata irq timeout */
#define	IDE_IOCTL_TIMEOUT_CMD_GET	(7|IDE_IOCTL)		/* get nondata irq timeout */
#define	IDE_IOCTL_TIMEOUT_SRST_SET	(8|IDE_IOCTL)		/* set soft reset busy timeout */
#define	IDE_IOCTL_TIMEOUT_SRST_GET	(9|IDE_IOCTL)		/* get soft reset busy timeout */
#define	IDE_IOCTL_TIMEOUT_SLEEP_SET	(10|IDE_IOCTL)		/* set enter sleep mode irq timeout */
#define	IDE_IOCTL_TIMEOUT_SLEEP_GET	(11|IDE_IOCTL)		/* get enter sleep mode irq timeout */
#define	IDE_IOCTL_TIMEOUT_ACTIVE_SET	(12|IDE_IOCTL)		/* set wakeup irq timeout */
#define	IDE_IOCTL_TIMEOUT_ACTIVE_GET	(13|IDE_IOCTL)		/* get wakeup irq timeout */
#define	IDE_IOCTL_TIMEOUT_STANDBY_SET	(14|IDE_IOCTL)		/* set enter standby mode irq timeout */
#define	IDE_IOCTL_TIMEOUT_STANDBY_GET	(15|IDE_IOCTL)		/* get enter standby mode irq timeout */
#define	IDE_IOCTL_IO_MODE_SET		(16|IDE_BLOCK|IDE_IOCTL)/* set transfer mode */
#define	IDE_IOCTL_DMA_ENABLE		(17|IDE_IOCTL)		/* enable dma transfer mode */
#define	IDE_IOCTL_DMA_DISABLE		(18|IDE_IOCTL)		/* disable dma transfer mode */
#define	IDE_IOCTL_MULT_ENABLE		(19|IDE_IOCTL)		/* enable multiple sector pio */
#define	IDE_IOCTL_MULT_DISABLE		(20|IDE_IOCTL)		/* disable multiple sector pio */
#define	IDE_IOCTL_LBA_ENABLE		(21|IDE_IOCTL)		/* enable lba access mode */
#define	IDE_IOCTL_LBA_DISABLE		(22|IDE_IOCTL)		/* enable chs access mode */
#define	IDE_IOCTL_SETTINGS_CURRENT	(25|IDE_BLOCK|IDE_IOCTL)/* soft reset will not change the feature selections */
#define	IDE_IOCTL_SETTINGS_FACTORY	(26|IDE_BLOCK|IDE_IOCTL)/* soft reset will return settings to the factory defaults */
#define	IDE_IOCTL_WCACHE_ENABLE		(27|IDE_BLOCK|IDE_IOCTL)/* enable write cache */
#define	IDE_IOCTL_WCACHE_DISABLE	(28|IDE_BLOCK|IDE_IOCTL)/* disable write cache */
#define	IDE_IOCTL_PRIORITY_SET		(29|IDE_IOCTL)		/* set priority task doing ide I/O*/
#define	IDE_IOCTL_PRIORITY_GET		(30|IDE_IOCTL)		/* get priority task doing ide I/O*/
#define	IDE_IOCTL_POLL_ENABLE		(31|IDE_IOCTL)		/* enable PIO polling */
#define	IDE_IOCTL_POLL_DISABLE		(32|IDE_IOCTL)		/* disable PIO polling */

/* max number of ide controllers */

#define	IDE_MAX_CTRL	16

/* priority of queue thread or -1 for synchronous I/O */

#define	IDE_IO_PRI	(-1)

/* drive enumeration */

#define	IDE_MASTER	0
#define	IDE_SLAVE	1

/* typedefs */

typedef struct ide_ctrl IDE_CTRL;

/* globals */

extern IDE_CTRL* sysIdeCtrl[IDE_MAX_CTRL];
extern int ideDebug;
extern int ideSlowIO;

/* function declarations */

void ideDebugSet (int level);
void ideSlowIOSet (int flag);
IDE_CTRL* ideCtrlCreate(IDE_CTRL* pIdeCtrl, uintptr_t commBase,
    uintptr_t ctrlBase, int regOffset, FUNCPTR dma, int priority,
    int maxXfer);
BLK_DEV* ideBlkDevCreate(IDE_CTRL* pIdeCtrl, int drive, unsigned size,
	unsigned offset);
void ideBlkDevDelete(BLK_DEV* ide);
void ide_intr(IDE_CTRL* pIdeCtrl);
STATUS ide_init(IDE_CTRL* pIdeCtrl);
STATUS sysIdeInit(void);
int ideDrvVersion(int silence);
STATUS ideShow(unsigned ctrl, unsigned drive);
STATUS ideShowInit();

#if OS_VERSION >= 210
BLK_IO_DEV* ideBlkDevInstall(IDE_CTRL* pIdeCtrl, int drive, unsigned size,
    unsigned offset, char* name, int dma);
BLK_IO_DEV* _ideBlkDevInstall(IDE_CTRL* pIdeCtrl, int drive, unsigned size,
    unsigned offset, char* name, int dma, int usePart);
#endif	/* OS_VERSION >= 210 */

#endif	/* __INCideDrvh */
