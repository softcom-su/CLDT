/* board.h - K64SMP and K64RIO board header */

#ifndef	__INCboardh__
#define	__INCboardh__

#include <sys/types.h>
#include <arch/cpu.h>

#ifndef __ASSEMBLER__ 
#include <system.h>
#include <pciLib.h>
#include <bsp.h>
#endif

#include <cpuLib.h>

#if !defined(MPON) && !defined(K64RIO) && !defined(BT83M) && !defined(CPRIO64)
  #include <pcivme.h>
#endif

#ifdef BT83M                      /* for compatibility with bt83b */
  #define IV_MINOR_EXT_INT_0 0
  #define IV_MINOR_EXT_INT_1 1
  #define IV_MINOR_EXT_INT_2 2
  #define IV_MINOR_EXT_INT_3 3
  #define IV_MINOR_EXT_INT_4 4
  #define IV_MINOR_EXT_INT_5 5
#endif

#ifndef CPU
#error CPU not defined!!!
#endif

#ifndef	BOOL
#define	BOOL int
#endif

/* Constants for figuring out CPU clock frequency */

#ifndef	CPU_CLK_RATE
#define CPU_CLK_RATE		24000000	/* CPU bus clock rate in Hz */
#endif	/* CPU_CLK_RATE */

#ifndef	BOARD_FREQ_REF
#define	BOARD_FREQ_REF		24000000	/* reference frequency in Hz */
#endif

/* Interrupt vectors */

#define	IV_BASE		0x40
#define	LINE_TO_IV(l)	((l)+IV_BASE)
#define	IV_TO_LINE(v)	((v)-IV_BASE)

#define	IV_PCI_INTA	LINE_TO_IV(0)
#define	IV_PCI_INTB	LINE_TO_IV(1)
#define	IV_PCI_INTC	LINE_TO_IV(2)
#define	IV_PCI_INTD	LINE_TO_IV(3)
#define	IV_TIMER4	LINE_TO_IV(4)
#define	IV_I2C		LINE_TO_IV(5)
#define	IV_UART0	LINE_TO_IV(6)
#define	IV_PCI		LINE_TO_IV(7)
#define	IV_NAND		LINE_TO_IV(8)
#define	IV_TIMER3	LINE_TO_IV(9)
#define	IV_TIMER2	LINE_TO_IV(10)
#define	IV_TIMER1	LINE_TO_IV(11)
#define	IV_TIMER0	LINE_TO_IV(12)
#define	IV_GPIO2	LINE_TO_IV(13)
#define	IV_GPIO3	LINE_TO_IV(14)
#define	IV_SC		LINE_TO_IV(15)
#define	IV_UART1	LINE_TO_IV(16)      /* 0x50 */
#define	IV_IRQ0		LINE_TO_IV(17)
#define	IV_GPIO10	LINE_TO_IV(18)
#define	IV_GPIO11	LINE_TO_IV(19)
#define IV_RIO          LINE_TO_IV(21)
#define	IV_IRQ1		LINE_TO_IV(22)
#define	IV_ETH		LINE_TO_IV(23)
#define	IV_HW_SPURIOUS	IV_ETH

/* cpu internal vectors */

#define	IV_TIMER	(IV_ETH+1)          /* 0x58 */
#define	IV_SPURIOUS	(IV_ETH+2)

/* default vector for VME demultiplexor */

#ifdef	K64SMP
#define	IV_VME_DEFAULT	IV_PCI_INTC
#else
#define	IV_VME_DEFAULT	IV_PCI_INTB
#endif

#ifdef VG12_PMC
  #warning !!!!!!!!! VME test code
  #define	IV_VME_DEFAULT	IV_PCI_INTA    /* PMCs of BT211 */
  
#endif
/* PRIME's data handling - it 'sits' at the middle of bootstring - at  0x80000900 ! */

#define PRIMES_DATA              0x80000900
#define PRIMES_DATA_SAVE_ADRS   (0x80000700+BOOT_LINE_SIZE)  
#define PRIMES_DATA_SIZE         256

/* local ram base address: KSEG0 or KSEG1 */

#define LOCAL_MEM_LOCAL_ADRS	(STARTPOINT & 0xa0000000)

/* uncached memory layout */

#define	BOARD_UNCACHED_MEM_BASE		0xcc000000
#define	BOARD_CACHED_MEM_BASE		0xce000000
#if defined(MPON)
  #define	BOARD_UNCACHED_MEM_PHYS_BASE	0x0e000000
  #define	BOARD_UNCACHED_MEM_SIZE		0x02000000
#else
  #define	BOARD_UNCACHED_MEM_PHYS_BASE	0x0f800000
  #define	BOARD_UNCACHED_MEM_SIZE		0x00800000
#endif
#define	BOARD_REGS_BASE			0xd0000000
#define	VME_SLAVE_BASE			(BOARD_UNCACHED_MEM_BASE+BOARD_UNCACHED_MEM_SIZE-VME_MEM_TOTAL_SIZE)

/* convert KSEG1 address to TLB translated virual address */

#if OS_VERSION >= 300
#ifdef __ASSEMBLER__
#define	K1_TO_TLB(x)	((x) + 0x20000000)
#else
#define	K1_TO_TLB(x)	((unsigned)(x) + 0x20000000)
#endif
#define	TLB_TO_K1(x)	((unsigned)(x) - 0x20000000)
#define	TLB_TO_PHYS(x)	((unsigned)(x) - 0xc0000000)
#define	PHYS_TO_TLB(x)	((unsigned)(x) + 0xc0000000)
#else
#define	K1_TO_TLB(x)	x
#endif

/* ^X reboot entry */

#define WARM_REBOOT		0xbfc00000
#define WARM_REBOOT_ADDRESS	WARM_REBOOT

/* SC64 device map */

#define	PCI_MEM0_BASE		K1_TO_TLB(0xb0000000)
#define	PCI_MEM0_SIZE		0x0a000000
#define	PCI_CONF		K1_TO_TLB(0xbe000000)
#define	PCI_IO_BASE		K1_TO_TLB(0xba000000)
#define	PCI_IO_SIZE		0x00400000
#define	PCI0_REGS		K1_TO_TLB(0xba800000)
#define	SYSTEM_REGS		K1_TO_TLB(0xbb000000)
#define	SC64_REGS		K1_TO_TLB(0xbb400000)
#define	SC64_ETH_REGS		K1_TO_TLB(0xbb800000)
#define	FLASH_BASE		K1_TO_TLB(0xbc000000)
#define	UART0_BASE		K1_TO_TLB(0xbff70000)
#define	UART1_BASE		K1_TO_TLB(0xbff70008)

/* SC64 regs */

#define	INTR_CTRL_BASE		(SC64_REGS+0x20)
#define	TIMER_BASE		(SC64_REGS+0x80)
#define	I2C_A_BASE		(SC64_REGS+0x110)
#define	I2C_B_BASE		(SC64_REGS+0x120)
#define	IRQ_BASE		(SC64_REGS+0x130)
#define	RTC_BASE		(SC64_REGS+0x240)
#define	SCIO_BASE		(SC64_REGS+0x250)
#define	SCRATCH_BASE		(SC64_REGS+0x270)

/* for little endian 64-bit registers access */

#define CF_LE_LL(x)						\
    (((unsigned long long)(x) << 56ULL) |			\
     ((unsigned long long)(x) >> 56ULL) |			\
     (((unsigned long long)(x) >> 40ULL) & 0x0000ff00ULL) |	\
     (((unsigned long long)(x) & 0x0000ff00ULL) << 40ULL) |	\
     (((unsigned long long)(x) >> 24ULL) & 0x00ff0000ULL) |	\
     (((unsigned long long)(x) & 0x00ff0000ULL) << 24ULL) |	\
     (((unsigned long long)(x) >> 8ULL) & 0xff000000ULL) |	\
     (((unsigned long long)(x) & 0xff000000ULL) << 8ULL))

#define	CT_LE_LL	CF_LE_LL

/* macros for address convertation */

#if OS_VERSION < 300
  #define LOCAL_TO_PCI(adrs)	K0_TO_PHYS(adrs)
  #define PCI_TO_LOCAL(adrs)	PHYS_TO_K1(adrs)
#else
  #define LOCAL_TO_PCI(adrs)	pciLocalToBus(adrs)
  #define PCI_TO_LOCAL(adrs)	pciBusToLocal(adrs)
#endif

#ifdef	BT211
  #define	LOCAL_TO_PCI_MEM(adrs)	(adrs)
  #define	PCI_MEM_TO_LOCAL(adrs)	(adrs)
#else
  #define	LOCAL_TO_PCI_MEM(adrs)	((adrs) & ~0x70000000)
  #define	PCI_MEM_TO_LOCAL(adrs)	((adrs) | PCI_MEM0_BASE)
#endif
#define	LOCAL_TO_PCI_IO(adrs)	(adrs)
#define	PCI_IO_TO_LOCAL(adrs)	(adrs)

/* control/status register bits */

/* timer modes */

#define	TIMER_MODE_ONESHOT	0
#define	TIMER_MODE_CYCLIC	4

#ifdef K64RIO
  #define IO_INPUT_MASK 0xf0
#endif
#if defined(BT211) || defined(MPON)  || defined(BT83M)     /* ???? laz */
  #define BOARD_USEROUT_0   8
  #define BOARD_USEROUT_1   9
  #define BOARD_USEROUT_2   10
  #define BOARD_USEROUT_3   11
  #if defined(BT83M)
    #define BOARD_USEROUT_4  12
    #define BOARD_USEROUT_5  13
    #define BOARD_USEROUT_6  14
  #endif
#endif
#ifdef CPRIO64
  #define BOARD_USEROUT_0   11
  #define BOARD_USEROUT_1   12
  #define BOARD_USEROUT_2   13
  #define BOARD_USEROUT_3   14
#endif
/* external interrupt mode */

#define	EXT_INT_MODE_LEVEL_1		0
#define	EXT_INT_MODE_LEVEL_0		1
#define	EXT_INT_MODE_EDGE_RISE		2
#define	EXT_INT_MODE_EDGE_FALL		3

#ifndef	SDRAM_SIZE
#define	SDRAM_SIZE		0x10000000
#endif

/* fixed memory location of PCI-VME */

#define	VME_FIXED_BASE
#define	VME_REGS_BASE		(PCI_MEM0_BASE+PCI_MEM0_SIZE-0x40000)
#define	VME_MWIN_BASE		PCI_MEM0_BASE
#define VME_VEC_BASE		(PCI_MEM0_BASE+PCI_MEM0_SIZE-0x30000)

/* fixed memory location of PCI-RIO */

#if OS_VERSION >= 300
  #define	RIO_SLAVE_MEM_BASE		(BOARD_UNCACHED_MEM_BASE+0x1000000)
  #define	RIO_SLAVE_CACHED_MEM_BASE	(RIO_SLAVE_MEM_BASE-BOARD_UNCACHED_MEM_BASE+BOARD_CACHED_MEM_BASE)
#else
  #define	RIO_SLAVE_MEM_BASE	(K1BASE+SDRAM_SIZE-0x1000000)
#endif

#ifdef	K64RIO
  #define K64RIO_MEM_BASE         K1_TO_TLB(0xb1800000)
  #define K64RIO_REGS_BASE        K1_TO_TLB(0xba400000)
  #define	RIO_MEM_BASE		K64RIO_MEM_BASE
  #define	RIO_REGS_BASE		K64RIO_REGS_BASE
#else
  #if !defined(BT211) && !defined(BT83M)
    #define	RIO_MEM_BASE		PCI_MEM0_BASE          /* MPON */
    #define	RIO_REGS_BASE		(PCI_MEM0_BASE+PCI_MEM0_SIZE-0x200000)
  #endif

#endif

/* fixed memory location for DPRAM of BT55-211 */

#ifdef BT55211
  #define DPRAM_FIXED_BASE
  #define DPRAM_FAST_WIN_BASE    (PCI_MEM0_BASE+PCI_MEM0_SIZE-0x200000)
  #define DPRAM_SLOW_WIN_BASE    (PCI_MEM0_BASE+PCI_MEM0_SIZE-0x100000)
  #define DPRAM_REGS_BASE        (PCI_MEM0_BASE+PCI_MEM0_SIZE-0x80000)
#endif
/* rioMemCpy() routine direction param value */

#define	RIO_TO_HOST	0
#define	HOST_TO_RIO	1

/* NAND flash controller regs */

#define	NAND_BASE		K1_TO_TLB(0xbb600000)
#define	NAND_DATA_BYTE		(volatile unsigned char*)(NAND_BASE)
#define	NAND_DATA_WORD		(volatile unsigned*)(NAND_BASE)
#define	NAND_DATA_DWORD		(volatile unsigned long long*)(NAND_BASE)
#define	NAND_ADDR_REG		(volatile unsigned char*)(NAND_BASE+0x8)
#define	NAND_CMD_REG		(volatile unsigned char*)(NAND_BASE+0x9)
#define	NAND_CTRL_REG		(volatile unsigned*)(NAND_BASE+0xc)

#if defined(MPON)
#define	NAND_N_CHIPS			2
#define	NAND_PAGES_PER_BLOCK		64
#define	NAND_PAGES_PER_CHIP		(NAND_PAGES_PER_BLOCK*nandBlocksPerChip)
#define	NAND_BLOCKS_PER_CHIP		8192
#elif defined(CPRIO64)
#define	NAND_N_CHIPS			1
#define	NAND_PAGES_PER_BLOCK		64
#define	NAND_PAGES_PER_CHIP		(NAND_PAGES_PER_BLOCK*nandBlocksPerChip)
#define	NAND_BLOCKS_PER_CHIP		8192
#else
#define	NAND_N_CHIPS			1
#define	NAND_PAGES_PER_BLOCK		64
#define	NAND_PAGES_PER_CHIP		(NAND_PAGES_PER_BLOCK*nandBlocksPerChip)
#define	NAND_BLOCKS_PER_CHIP		1024
#endif

/* NAND control register bits */

#define	NAND_CTRL_CS			0x02
#define	NAND_CTRL_FLASH_READY		0x04
#define	NAND_CTRL_IRQ_MASK		0x08
#define	NAND_CTRL_IRQ			0x10

/* maximum size of boot line */

#define	BOOT_LINE_SIZE		1024

/* offset OC2000 boot line in NVRAM */

#define	BOOT_LINE_OFFSET	0
#define	BOOT_LINE_NV_RAM_OFFSET	BOOT_LINE_OFFSET

/* NVRAM cell description */

#define	NV_CELL_BYTES	BOOT_LINE_SIZE
#define	NV_CELL_WORDS	(NV_CELL_BYTES / 2)
#define	NV_CELL_LONGS	(NV_CELL_BYTES / 4)
#define	NV_CELL_SUM	(NV_CELL_WORDS - 1)
#define	NV_CELL_END	(NV_CELL_WORDS - 2)

/* exception and POST result address */

#define EXC_MSG_ADR		0x80000800

#ifdef FOR_SUBMICRON

// PCI init value
#define PCI_REG_BAR_val         0
#define PCI_REG_COMSTAT_val     0xffff0146
#define PCI_REG_TRANS_val       0x80000000
#define PCI_REG_MASK_TRANS_val  0
#define PCI_REG_ENARET_val      6
#define PCI_REG_INT_val         0xf0f00

#define PCI_PSC_REG_BAR_val         0x80000000
#define PCI_PSC_REG_COMSTAT_val     0x2

#endif  //FOR_SUBMICRON

#ifndef __ASSEMBLER__ 

/* globals */

extern int nandDebug;			/* debug level */
extern int nandWritePoll;		/* enable write poll busy mode instead IRQ */
extern int nandErasePoll;		/* enable erase poll busy mode instead IRQ */
extern int nandInitDone;		/* initialization done flag */
extern int nandBlocksPerChip;		/* blocks per chip */
extern unsigned sc64TimerFreq;		/* timers frequency in HZ */
extern VOIDFUNCPTR boardECCHadler;	/* ECC handler */

#ifdef FOR_SUBMICRON
extern unsigned time_start_boardHWInit1;
extern unsigned time_clear_cp0count_hw1;       //gmu:otl
extern unsigned trace_submic[1024];
extern unsigned counter_trace_submic;
extern unsigned time_sync_VM6_VM7;       //gmu: otl

extern int rioInitAll(void); // gmu: для того чтобы запускать вне первоночальной инициализации
extern int netconfig();      // gmu: для того чтобы запускать вне первоночальной инициализации

//#define       DEBUG_SAVE_INIT_TIME

#ifdef DEBUG_SAVE_INIT_TIME  //debug
// для того чтобы понять какие функции запускаются при инициализации и в какое время
// сохраняем информацию о выполнявшихся функциях в специальный буфер
extern char string_buf_for_init_info[4096];  // буфер для хранения информации о инициализации
extern unsigned counter_buf_for_init;  // указатель на область для записи в буфер инициализации

// TODO: частота процессора пока константа 200
#define WRITE_TOBUF_INIT(a) if(counter_buf_for_init<4096){ \
 counter_buf_for_init+=sprintf(string_buf_for_init_info+counter_buf_for_init,"Start %s time  %d mks",#a,boardCountGet()/200); \
 a; \
 counter_buf_for_init+=sprintf(string_buf_for_init_info+counter_buf_for_init," done time %d mks\n",boardCountGet()/200); }

#else
#define WRITE_TOBUF_INIT(a) a;
#endif //DEBUG_SAVE_INIT_TIME

#else
#define WRITE_TOBUF_INIT(a) a;
#endif //FOR_SUBMICRON

/* function declarations */

void pciErrInt(int ctrl);
void intConnectPciErr(VOIDFUNCPTR rtn, int arg);
int pciErrIntEnable(int ctrl, int errmask);
void nandIntr(long arg);
int nandReset(void);
int nandRead(unsigned chunk, void* data, void* oob);
int nandIsFree(unsigned chunk, int* res);
int nandWrite(unsigned chunk, void* data, void* oob);
int nandErase(unsigned chunk);
int nandInit(void);
int nandIsBad(unsigned chunk, int* res);
void* sc64AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed);
void* _sc64AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed, int cread);
int sc64TimerCountStart(unsigned ix);
int sc64TimerCountStop(unsigned ix);
int sc64TimerCountSet(unsigned ix, uint64_t count);
int sc64TimerCountGet(unsigned ix, uint64_t* count);
int sc64TimerRateSet(unsigned ix, unsigned mul, unsigned div, unsigned mode);
int sc64TimerCSRSet(unsigned ix, unsigned char val);
int sc64TimerCSRGet(unsigned ix, unsigned char* val);
int boardClockHandlerGet(FUNCPTR* rtn, int* arg);
int boardClkCntGet(void);
void* boardUncachedToCached(void* addr);
void* boardCachedToUncached(void* addr);
int boardUserDir(unsigned n, unsigned dir);
void boardTimerExtClkEnable(void);
void boardTimerExtClkDisable(void);
void boardTimer0OutEnable(void);
void boardTimer0OutDisable(void);
int boardExtInt0Enable(unsigned mode);
int boardExtInt1Enable(unsigned mode);
int boardExtInt2Enable(unsigned mode);
int boardExtInt3Enable(unsigned mode);
void boardExtInt0Disable(void);
void boardExtInt1Disable(void);
void boardExtInt2Disable(void);
void boardExtInt3Disable(void);
int  boardExtInt0Get(void);
int  boardExtInt1Get(void);
int  boardExtInt2Get(void);
int  boardExtInt3Get(void);
int  boardExtInt4Get(void);
int  boardExtInt5Get(void);
int  boardExtInt6Get(void);
void boardExtInt0Connect(VOIDFUNCPTR rtn, int arg);
void boardExtInt1Connect(VOIDFUNCPTR rtn, int arg);
void boardExtInt2Connect(VOIDFUNCPTR rtn, int arg);
void boardExtInt3Connect(VOIDFUNCPTR rtn, int arg);
int boardExtDevInt0Enable(unsigned mode);
int boardExtDevInt1Enable(unsigned mode);
void boardExtDevInt0Disable(void);
void boardExtDevInt1Disable(void);
void boardExtDevInt0Connect(VOIDFUNCPTR rtn, int arg);
void boardExtDevInt1Connect(VOIDFUNCPTR rtn, int arg);
unsigned short boardUserLatch(void);
void boardUserOutAll(unsigned short val);
void boardECCDisable(void);
void boardECCIntEnable(void);
void boardBPEnable(void);
void boardBPDisable(void);
int boardRioIdGet(void);

#endif /*__ASSEMBLER__*/

#endif	/* __INCboardh__ */
