#ifndef	__INCbageth
#define	__INCbageth

/* typedefs */

typedef struct {
    unsigned pioChannel1;	/* channel 1 PIO mode timings */
    unsigned dmaChannel1;	/* channel 1 DMA mode timings */
    unsigned pioChannel2;	/* channel 2 PIO mode timings */
    unsigned dmaChannel2;	/* channel 2 DMA mode timings */
} BAGET_TIMINGS;

/* baget mio pci vendor ID and device ID */

#define BAGET_MIO_VENDOR	0x7777
#define BAGET_MIO_DEVICE	0x3333
#define	BAGET_PCI_ID		(BAGET_MIO_VENDOR | (BAGET_MIO_DEVICE << 16))

#define	BAGET_PCI_LPT_ID	0x33407777
#define	BAGET_PCI_IDE_ID	0x33507777

/* channel perfomance timings */

#define	BAGET_TIMINGS_DEF	(-1)	/* default timings (after reset) */
#define	BAGET_TIMINGS_PIO_SLOW	0xffff	/* slowest PIO timings */
#define	BAGET_TIMINGS_PIO_FAST	0x0000	/* fastest PIO timings */
#define	BAGET_TIMINGS_MDMA_SLOW	0xff	/* slowest multiword dma timings */
#define	BAGET_TIMINGS_MDMA_FAST	0x00	/* fastest multiword dma timings */

/* for 48MHZ clock */

#define	BAGET_48MHZ_PIO0	0xeefd	/* PIO mode 0 timings */
#define	BAGET_48MHZ_PIO1	0x5ec7	/* PIO mode 1 timings */
#define	BAGET_48MHZ_PIO2	0x0e66	/* PIO mode 2 timings */
#define	BAGET_48MHZ_PIO3	0x4545	/* PIO mode 3 timings */
#define	BAGET_48MHZ_PIO4	0x2424	/* PIO mode 4 timings */
#define	BAGET_48MHZ_MDMA0	0xcb	/* multiword dma mode 0 timings */
#define	BAGET_48MHZ_MDMA1	0x44	/* multiword dma mode 1 timings */
#define	BAGET_48MHZ_MDMA2	0x24	/* multiword dma mode 2 timings */

/* for PCI 33MHZ */

#define	BAGET_33MHZ_PIO0	0x88c4	/* PIO mode 0 timings */
#define	BAGET_33MHZ_PIO1	0x1863	/* PIO mode 1 timings */
#define	BAGET_33MHZ_PIO2	0x1822	/* PIO mode 2 timings */
#define	BAGET_33MHZ_PIO3	0x1111	/* PIO mode 3 timings */
#define	BAGET_33MHZ_PIO4	0x0000	/* PIO mode 4 timings */
#define	BAGET_33MHZ_MDMA0	0x47	/* multiword dma mode 0 timings */
#define	BAGET_33MHZ_MDMA1	0x02	/* multiword dma mode 1 timings */
#define	BAGET_33MHZ_MDMA2	0x01	/* multiword dma mode 2 timings */

/* for PCI 25MHZ */

#define	BAGET_25MHZ_PIO0	0x6693	/* PIO mode 0 timings */
#define	BAGET_25MHZ_PIO1	0x1642	/* PIO mode 1 timings */
#define	BAGET_25MHZ_PIO2	0x1611	/* PIO mode 2 timings */
#define	BAGET_25MHZ_PIO3	0x1111	/* PIO mode 3 timings */
#define	BAGET_25MHZ_PIO4	0x0000	/* PIO mode 4 timings */
#define	BAGET_25MHZ_MDMA0	0x25	/* multiword dma mode 0 timings */
#define	BAGET_25MHZ_MDMA1	0x02	/* multiword dma mode 1 timings */
#define	BAGET_25MHZ_MDMA2	0x01	/* multiword dma mode 2 timings */

/* for backward compatibility */

#define	BAGET_TIMINGS_PIO0	BAGET_25MHZ_PIO0
#define	BAGET_TIMINGS_PIO1	BAGET_25MHZ_PIO1
#define	BAGET_TIMINGS_PIO2	BAGET_25MHZ_PIO2
#define	BAGET_TIMINGS_PIO3	BAGET_25MHZ_PIO3
#define	BAGET_TIMINGS_PIO4	BAGET_25MHZ_PIO4
#define	BAGET_TIMINGS_MDMA0	BAGET_25MHZ_MDMA0
#define	BAGET_TIMINGS_MDMA1	BAGET_25MHZ_MDMA1
#define	BAGET_TIMINGS_MDMA2	BAGET_25MHZ_MDMA2

#define	BAGET_MAX_XFER		120	/* maximum transfer sectors */

#ifdef BT206_RS6
  #define	BAGET_MAX_CTRL		1	/* maximum baget mio controllers */
#else
  #define	BAGET_MAX_CTRL		4	/* maximum baget mio controllers */
#endif

/* globals */

extern BAGET_TIMINGS bagetTimings[BAGET_MAX_CTRL];

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

STATUS ideBagetCtrlInit(int reset, int priority);

#else

STATUS ideBagetCtrlInit();

#endif  /* __STDC__ */

#endif	/* __INCbageth */
