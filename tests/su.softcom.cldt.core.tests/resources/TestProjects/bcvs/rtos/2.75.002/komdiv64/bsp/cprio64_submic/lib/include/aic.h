/* aic.h - sound codec header */

#ifndef __aic
#define __aic

#ifdef OS_VXWORKS
#include <semLib.h>
#else
#include <version.h>
#include <private/ios.h>
#include <semaphore.h>
#include <logLib.h>
#endif

#define AIC_DEBUG \
    if (aicDebug) logMsg

#define AIC_BUF_BYTES		256
#define AICQUEUEBYTES		2560

#define AIC_BUF_WORDS		64

#define MAX_CODEC_NUM		1

#ifdef BT43
#define MAX_CHN_NUM		1
#else
#define MAX_CHN_NUM		2
#endif

#ifdef BT43
#define MAX_CHAN		1
#else
#define MAX_CHAN		2
#endif

#define AIC_ENABLE_NONE		0
#define AIC_ENABLE_OUTPUT	1
#define AIC_ENABLE_INPUT	2

#ifndef BTMPC4_402_PCI_SDRAM_ID
#define BTMPC4_402_PCI_SDRAM_ID 0x7778
#endif
#define BTMPC4_402_PCI_SND_ID	0x7779

/* Modes */

#define AIC_DATA_WORD			0x01
#define AIC_DATA_A_LAW			0x02
#define AIC_DATA_U_LAW			0x04
#define AIC_EXCH_INQUIRY		0x08
#define AIC_EXCH_EACHFRAME		0x10
#define AIC_WITHOUT_DATA_VALID_FLAG	0x20
#define AIC_CHN1			0
#define AIC_CHN2			0x40

/* Bufs offsets */

#define AIC_BUF_offs			0x800
#define AIC_BUF_offs_chn2		0xc00

#define AIC_VER_CTRL			0x00015678

#define AIC_CTR_RESET			0x00000001
#define AIC_CTR_PWRDN			0x00000002
#define AIC_CTR_DATASZ			0x00010000
#define AIC_CTR_ROUTINE1		0x00020000
#define AIC_CTR_TURNONCH1		0x00040000
#define AIC_CTR_STATECH1		0x00080000
#define AIC_CTR_DATASZ2			0x00100000
#define AIC_CTR_ROUTINE2		0x00200000
#define AIC_CTR_TURNONCH2		0x00400000

#define AIC_REG_CODECDATA		0x00000001
#define AIC_REG_CODECDATA_MASK		0x000000ff
#define AIC_REG_CODECREGADDR		0x00010000
#define AIC_REG_CODECREGADDR_MASK	0x00ff0000
#define AIC_REG_ALLOWINTREG		0x04000000
#define AIC_REG_CODECREGEXCHEND		0x10000000
#define AIC_REG_OPERTYPE		0x40000000
#define AIC_REG_CODECREGEXCH		0x80000000

#define AIC_EX_ALLOWINTBUF		0x00000001
#define AIC_EX_CODECBUFEXCHEND		0x00000002
#define AIC_EX_BUFNUM			0x00000004
#define AIC_EX_ALLOWINTBUF2		0x00000010
#define AIC_EX_CODECBUFEXCHEND2		0x00000020
#define AIC_EX_BUFNUM2			0x00000040
#define AIC_EX_INTBUFEXCH		0x20000000
#define AIC_EX_INTBUFEXCH2		0x40000000
#define AIC_EX_INTREGEXCH		0x80000000

typedef struct {
    int		chn;			/* channel number */
    int		data_type;		/* data type */
    int		trans_mode;		/* transmitting mode */
    int		valid_flag;		/* data word valid flag */
} AIC_MODE;

typedef struct {
    unsigned char	addr;		/* codec register address */
    unsigned char	data;		/* codec register value */
} CODEC_REG;

/* Registers */

typedef struct {
    volatile unsigned aic_ver;		/* 0x000 */
    volatile unsigned aic_ctr;		/* 0x004 */
    volatile unsigned aic_reg;		/* 0x008 */
    volatile unsigned aic_ex;		/* 0x00c */
} AIC_REGs;

typedef struct {
    volatile unsigned chn1_brx[2][AIC_BUF_WORDS];	/* 0x800 - 0x8ff, 0x900 - 0x9ff */
    volatile unsigned chn1_btx[2][AIC_BUF_WORDS];	/* 0xa00 - 0xaff, 0xb00 - 0xbff */
    volatile unsigned chn2_brx[2][AIC_BUF_WORDS];	/* 0xc00 - 0xcff, 0xd00 - 0xdff */
    volatile unsigned chn2_btx[2][AIC_BUF_WORDS];	/* 0xe00 - 0xeff, 0xf00 - 0xfff */
} AIC_BUFs;

/* Queue */

typedef struct {
    unsigned	*queueBeg, *queueEnd;
    unsigned	*queueFirst, *queueLast;
    int		freeSize;
} AIC_QUEUE;

typedef struct {
    AIC_QUEUE	*pTxQueue;		/* transmit queue */
    AIC_QUEUE	*pRxQueue;		/* receive queue */
    int		duplex;			/* tx, rx, fullduplex */
    int		data_type;		/* data type */
    int		transmit_en;		/* transmission enable flag */
    int		chn_open;		/* channel opened */
} AIC_CHN_DESC;

typedef struct {
    AIC_REGs	*aic_regs;		/* ptr to registers space */
    AIC_BUFs	*aic_bufs;		/* ptr to buffers space */
    int		ctrl_num;		/* controller number */
    int		trans_mode;		/* transmitting mode */
    int		valid_flag;		/* get data words with/without valid flag */

    int		dev_open;		/* device opened */
#ifdef OS_VXWORKS
    SEM_ID	sem;			/* binary semaphore */
    SEM_ID	semb;			/* binary semaphore */
#else
    sem_t	sem;			/* semaphore for register exchange interrupt*/
    sem_t	semb;			/* semaphore for buffer exchange interrupt */
#endif /*OS_VXWORKS*/
    int		created;		/* device exist */
    
    unsigned	aic_ex;			/* ex register current value */
    
    AIC_CHN_DESC aicChnDesc[MAX_CHN_NUM];/* channel descriptor */
} AIC_DESC;

#if (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL))

/* OC device descriptor */

typedef struct {
#ifdef OS_VXWORKS
    DEV_HDR	devHdr;			/* VX device header */
#else
    ioDEV	devHdr;			/* OC device header */
#endif /*OS_VXWORKS*/
    AIC_DESC	*aicDesc;		/* controller descriptor */
    int		type_desc;		/* 0 - controller; 1,2 - channels */
} OS_AIC_DEV;

#endif /* (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL)) */

#define FIOAICSETMODE	5001
#define FIOAICGETMODE	5002
#define FIOAICSTART	5003
#define FIOAICSTOP	5004
#define FIOAICREGWRITE	5005
#define FIOAICREGREAD	5006
#define FIOAICRESET	5007

#ifdef BT43
STATUS aicInit(int);
#else
STATUS aicInit();
#endif
void aicIntr(int);

int aicUserIn(int);

#define CODEC_CTRL1	1
#define CODEC_CTRL2	2
#define CODEC_CTRL3	3
#define CODEC_CTRL4	4
#define CODEC_CTRL5	5
#define CODEC_CTRL6	6
#define CODEC_CTRL7	7
#define CODEC_CTRL8	8
#define CODEC_CTRL9	9
#define CODEC_CTRL10	10
#define CODEC_CTRL11	11
#define CODEC_CTRL12	12
#define CODEC_CTRL13	13
#define CODEC_CTRL14	14
#define CODEC_CTRL15	15
#define CODEC_CTRL16	16
#define CODEC_CTRL17	17

#define CTRL2_RESET	0x01
#define CTRL7_RESET	0x01

#endif /* __aic */
