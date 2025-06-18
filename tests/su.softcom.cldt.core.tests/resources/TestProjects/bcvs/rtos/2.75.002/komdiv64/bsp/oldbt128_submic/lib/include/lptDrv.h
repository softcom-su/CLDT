/* lptDrv.h - LPT driver header */

#ifndef	__INClptDrvh
#define	__INClptDrvh

#include <version.h>
#include <semaphore.h>
#include <private/ios.h>
#if OS_VERSION < 300
#include <private/stddrv.h>
#else
#include <private/queue.h>
#include <private/ios.h>
#endif

/* SPP registers offset from base0 */

#define	SPP_DR		0	/* Data Register */
#define	SPP_SR		1	/* Status Register */
#define	SPP_CR		2	/* Control Register */

/* EPP registers offset from base0 */

#define	EPP_AR		3	/* Address Register */
#define	EPP_DR		4	/* Data Register */
#define	EPP_EXT		5	/* Extended Register 0 */

/* ECP registers offset from base0 and base1 */

#define	ECP_AFIFO	0	/* Address FIFO (base0) */
#define	ECP_DFIFO	0	/* Data FIFO (base1) */
#define	ECP_CFIFO	0	/* Fast Centronics FIFO (base1) */
#define	ECP_TFIFO	0	/* Test FIFO (base1) */
#define	ECP_CNFGA	0	/* Configuration register A (base1) */
#define	ECP_CNFGB	1	/* Configuration register B (base1) */
#define	ECP_ECR		2	/* Extended Control Register (base1) */

/* controller modes enumeration */

#define	MODE_SPP	1
#define	MODE_EPP	2
#define	MODE_ECP	3

/* ECP modes enumeration */

#define	ECP_SPP		0	/* SPP */
#define	ECP_PS2		1	/* Bi-Directional PS/2 */
#define	ECP_PPF		2	/* Parallel Port FIFO: Fast Centronics */
#define	ECP_ECP		3	/* ECP */
#define	ECP_TEST	6	/* Test */
#define	ECP_CONF	7	/* Configuration */

/* SPP control register */

#define CR_STROBE	0x01	/* STROBE signal */
#define CR_AUTOFEED	0x02	/* AUTOFEED signal */
#define CR_NOINIT	0x04	/* INITIALIZE signal */
#define CR_SELECT	0x08	/* SELECT INPUT signal */
#define CR_ENABLE	0x10	/* ENABLE interrupt */
#define CR_INPUT	0x20	/* DATA DIRECTION control bit */

/* SPP status register */

#define SR_TOUT		0x01	/* EPP cycle timeout */
#define SR_NOERR	0x08	/* device error */
#define SR_SELECT	0x10	/* selected */
#define SR_PEND		0x20	/* paper end */
#define SR_NOACK	0x40	/* printer ready */
#define SR_NOBUSY	0x80	/* busy */
#define SR_MASK		0xb8	/* mask bits */

/* ECP extended control register */

#define	ECR_EMPTY	0x01	/* FIFO empty */
#define	ECR_FULL	0x02	/* FIFO full */
#define	ECR_INT_DS	0x04	/* service interrupt disable */
#define	ECR_DMA_EN	0x08	/* DMA enable */
#define	ECR_ERR_DS	0x10	/* nFAULT interrupt disable */

/* ioctl commands */

#define	DMA_OUT_BYTES	10001	/* write buffer via DMA (internal) */
#define	LPT_MODE_SET	10002	/* set LPT mode */
#define	LPT_CONFIG_GET	10003	/* get LPT configuration */
#define	LPT_CONFIG_SET	10004	/* set LPT configuration */
#define	LPT_RESET	10005	/* reset the printer */
#define	LPT_CTRL_RESET	10006	/* reset the controller */
#define	LPT_CTRL_SHOW	10007	/* show information about controller */

/* dma directions */

#define	PORT_TO_HOST	0	/* from port to host memory */
#define	HOST_TO_PORT	1	/* from host memory to port */

/* flags */

#define	LPT_QUIRK_BTM23405	1	/* BTM23-405 bug */
#define	LPT_PDIR_AUTO		2	/* EPP PDIR auto switched */
#define	LPT_HAS_EPP_TOUT	4	/* SR timeout bit present */

/* event enumeration */

#define	LPT_EVENT_PAPER_END	1	/* paper end detected */
#define	LPT_EVENT_NO_PAPER	2	/* wait paper timeout */
#define	LPT_EVENT_NO_SELECT	3	/* printer not set select */
#define	LPT_EVENT_ERROR		4	/* some printer error */
#define	LPT_EVENT_BUSY		5	/* printer not clear busy */
#define	LPT_EVENT_INTR		6	/* interrupt happen (called from interrupt context) */

/* user hook call */

#define	LPT_EVENT(name,arg)					\
    if(lpt->userRtn)					\
	lpt->userRtn(lpt->userArg,LPT_EVENT_##name,arg);

/* debug print */

#define	LPT_DEBUG(level,msg)	\
{				\
    if(lptDebug >= (level))	\
	logMsg msg;		\
}

/* mutex lock/unlock */

#define	LPT_LOCK()				\
    if(lpt->share)				\
	pthread_mutex_lock(&lpt->mutex)

#define	LPT_UNLOCK()				\
    if(lpt->share)				\
	pthread_mutex_unlock(&lpt->mutex)

/* default timeouts */

#define	DEF_PEND_TOUT		120	/* end of paper timeout in seconds */
#define	DEF_PEND_STEP		15	/* end of paper message in seconds */
#define	DEF_BUSY_TOUT		120	/* wait no busy in seconds */
#define	DEF_BUSY_COUNT		1000	/* wait no busy retries */
#define	DEF_SELECT_COUNT	100000	/* wait select retries */
#define	DEF_FIFO_COUNT		100000	/* wait FIFO not full or empty retries */
#define	DEF_DMA_TOUT		120	/* end of DMA timeout in seconds */

/* printer INIT time in us */

#define	LPT_RESET_DELAY		50

/* base0 and base1 registers access */

#define	LPT0_WR(offset,val)	lptWr(lpt->base0+(offset),(val))
#define	LPT0_RD(offset)		lptRd(lpt->base0+(offset))
#define	LPT1_WR(offset,val)	lptWr(lpt->base1+(offset),(val))
#define	LPT1_RD(offset)		lptRd(lpt->base1+(offset))

/* inlines */

static inline void lptWr(volatile unsigned char* reg, unsigned char val)
{
    *reg = val;
}

static inline unsigned char lptRd(volatile unsigned char* reg)
{
    return *reg;
}

#if	(OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL))

/* generic controller descriptor */

struct lpt_ctrl {
    ioDEV			dev_dev;	/* system device header */
    queHEAD			dev_que;	/* aiocb queue */
    pthread_mutex_t		dev_mut;	/* aiocb queue mutex */
    sem_t			dev_sem;	/* semaphore for thread resume */
    pthread_t			dev_thread;	/* aio thread id */
    volatile unsigned char*	base0;		/* SPP base address */
    volatile unsigned char*	base1;		/* ECP base address */
    pthread_mutex_t		mutex;		/* for shared mutual exclusion */
    FUNCPTR			ioctl;		/* low-level controller ioctl */
    VOIDFUNCPTR			userRtn;	/* user hook entry */
    long			userArg;	/* user hook argument */
    int				sppCR;		/* control register value for SPP */
    int				share;		/* TRUE if used both direct and via system access */
    int				lptMode;	/* current controller mode */
    int				ecpMode;	/* current ECP mode */
    int				pendTout;	/* wait paper timeout in seconds */
    int				pendStep;	/* paper end message in seconds or -1 */
    int				busyTout;	/* busy timeout in seconds */
    int				busyCount;	/* short busy timeout counter */
    int				selectCount;	/* select timeout counter */
    int				fifoCount;	/* FIFO is not full and FIFO empty counter */
    int				dmaTout;	/* dma transfer timeout in seconds */
    int				dmaThreshold;	/* minimum amount of data for dma */
    int				dmaEnable;	/* TRUE if dma enable */
    int				flags;		/* controller bugs */
};

#endif	/* (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL)) */


typedef struct lpt_ctrl LPT_CTRL;

/* LPT configuration */

typedef struct {

    /* read only configuration */

    volatile unsigned char*	base0;	/* SPP base address */
    volatile unsigned char*	base1;	/* ECP base address */
    pthread_mutex_t*		pMutex;	/* for shared mutual exclusion */

    /* read-write configuration */

    VOIDFUNCPTR	userRtn;	/* user hook entry */
    long	userArg;	/* user hook argument */
    int		sppCR;		/* control register value for SPP */
    int		lptMode;	/* current controller mode */
    int		ecpMode;	/* current ECP mode */
    int		pendTout;	/* wait paper timeout in seconds */
    int		pendStep;	/* paper end message in seconds or -1 */
    int		busyTout;	/* busy timeout in seconds */
    int		busyCount;	/* short busy timeout counter */
    int		selectCount;	/* select timeout counter */
    int		fifoCount;	/* FIFO is not full and FIFO empty counter */
    int		dmaTout;	/* dma transfer timeout in seconds */
    int		dmaThreshold;	/* minimum amount of data for dma */
    int		dmaEnable;	/* TRUE if dma enable */

} LPT_CONFIG;

/* globals */

extern LPT_CTRL* pSysLptCtrl;	/* implicite pointer to system controller */
extern int lptDebug;		/* debug level */
extern int lptFd; 		/* printer file descriptor */

/* function declarations */

STATUS lptWait(LPT_CTRL* lpt);
STATUS lptCtrlInit(LPT_CTRL* lpt, char* base0, char* base1, FUNCPTR ioctl,
    int share, int lptMode, int ecpMode, int dmaThreshold, int dmaEnable);
int sppOutBytes(LPT_CTRL* lpt, char* buf, int nBytes);
int ppfOutBytes(LPT_CTRL* lpt, char* buf, int nBytes);
void lptDefHook(LPT_CTRL* lpt, int event, unsigned sr);

int lptModeSet(LPT_CTRL* lpt, int lptMode, int ecpMode);
int lptConfigSet(LPT_CTRL* lpt, LPT_CONFIG* config);
int lptConfigGet(LPT_CTRL* lpt, LPT_CONFIG* config);
void lptReset(LPT_CTRL* lpt);
void lptCtrlReset(LPT_CTRL* lpt);
STATUS lptDevCreate(LPT_CTRL* lpt, char* name, int priority);
int lptDrvVersion(int silence);
STATUS lptShow(LPT_CTRL* lpt);
int eppOutBytes(LPT_CTRL* lpt, unsigned addr, unsigned char* buf, int nBytes,
    int check);
int eppInBytes(LPT_CTRL* lpt, unsigned addr, unsigned char* buf, int nBytes,
    int check);

#endif	/* __INClptDrvh */
