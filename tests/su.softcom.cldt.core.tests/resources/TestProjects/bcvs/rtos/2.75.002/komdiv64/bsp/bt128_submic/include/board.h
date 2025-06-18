/* board.h - K128 board header */

#ifndef	__INCboardh__
#define	__INCboardh__

#ifndef __ASSEMBLER__
  #include <system.h>
  #include <stdint.h>
#endif

#include <cpuLib.h>
#include <bsp.h>
#include <bspversion.h>
#include <arch/mips/idtcpu.h>

#define	BSP_VERSION		((BSP_VERSION_MAJOR << 16) | BSP_VERSION_MINOR)

/* PRIME's data handling - it 'sits' at the middle of bootstring - at  0x80000900 ! */

#define PRIMES_DATA              0x80000900
#define PRIMES_DATA_SAVE_ADRS    (0x80000700+BOOT_LINE_SIZE)
#define PRIMES_DATA_SIZE         256

/* NVRAM cell description */

#define	NV_CELL_BYTES	BOOT_LINE_SIZE
#define	NV_CELL_WORDS	(NV_CELL_BYTES / 2)
#define	NV_CELL_LONGS	(NV_CELL_BYTES / 4)
#define	NV_CELL_SUM	(NV_CELL_WORDS - 1)
#define	NV_CELL_END	(NV_CELL_WORDS - 2)

/* CPU definitions */

#define	IS_KSEG0(x)	((unsigned)(x) >= K0BASE && (unsigned)(x) < K1BASE)
#define	IS_KSEG1(x)	((unsigned)(x) >= K1BASE && (unsigned)(x) < K2BASE)
#define	IS_KUSEG(x)	((unsigned)(x) < K0BASE)

/* local ram base address: KSEG0 or KSEG1 */

#define LOCAL_MEM_LOCAL_ADRS	(RAM_LOW_ADRS & 0xa0000000)

/* clock defininitions */

#define UART_CLK_FREQ		24000000	/* 24 MHz clock for uart  */
#define TIMER_CLC_FREQ          24000000        /* 24 MHz external timer clock */

/* maximum size of boot line */

#define	BOOT_LINE_SIZE		512

/* offset OC2000 boot line in NVRAM */

#define	BOOT_LINE_OFFSET	0
#define	BOOT_LINE_NV_RAM_OFFSET	BOOT_LINE_OFFSET

/* exception and POST result address */

#define EXC_MSG_ADR		0xa0000800

#define FLASH_BASE		0

/* oc3000 memory map */

#define	BOARD_UNCACHED_MEM_BASE		0xcc000000
#define	BOARD_CACHED_MEM_BASE		0xce000000
#define	BOARD_UNCACHED_MEM_PHYS_BASE	0x0f800000     /* last 8M of first 256M */
#define	BOARD_UNCACHED_MEM_SIZE		0x00800000
#define	BOARD_REGS_BASE			0xd0000000

#define WARM_REBOOT	                0xbfc00000
#define WARM_REBOOT_ADDRESS             WARM_REBOOT

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

/* PCI map */

#define PCI_MEM_BASE            K1_TO_TLB(0xb0000000)
#define PCI_MEM_SIZE            0x06000000
#define PCI_IO_BASE             K1_TO_TLB(0xbe000000)
#define PCI_IO_SIZE             0x10000

/* for little endian 64-bit registers access */

#define CF_LE_LL(x)                                            \
    (((unsigned long long)(x) << 56ULL) |                      \
    ((unsigned long long)(x) >> 56ULL) |                       \
    (((unsigned long long)(x) >> 40ULL) & 0x0000ff00ULL) |     \
    (((unsigned long long)(x) & 0x0000ff00ULL) << 40ULL) |     \
    (((unsigned long long)(x) >> 24ULL) & 0x00ff0000ULL) |     \
    (((unsigned long long)(x) & 0x00ff0000ULL) << 24ULL) |     \
    (((unsigned long long)(x) >> 8ULL) & 0xff000000ULL) |      \
    (((unsigned long long)(x) & 0xff000000ULL) << 8ULL))

#define CT_LE_LL        CF_LE_LL

/* macros for address convertation */

#if OS_VERSION < 300
#define LOCAL_TO_PCI(adrs)      K0_TO_PHYS(adrs)
#define PCI_TO_LOCAL(adrs)      PHYS_TO_K1(adrs)
#else
#define LOCAL_TO_PCI(adrs)      pciLocalToBus(adrs)
#define PCI_TO_LOCAL(adrs)      pciBusToLocal(adrs)
#endif

#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron  
#define	LOCAL_TO_PCI_MEM(adrs)	((adrs) & ~0x70000000)
#define	PCI_MEM_TO_LOCAL(adrs)	((adrs) | PCI_MEM_BASE)
#else
#define LOCAL_TO_PCI_MEM(adrs)  (adrs)
#define PCI_MEM_TO_LOCAL(adrs)  (adrs)
#endif

#define LOCAL_TO_PCI_IO(adrs)   (adrs)
#define PCI_IO_TO_LOCAL(adrs)   (adrs)

/* SRAM */

#define SRAM_BASE               K1_TO_TLB(0xbfc00000)
#define SRAM_SIZE               0x8000

/*  ASIC/DEV address map */

#define DEV_BASE	        K1_TO_TLB(0xbff00000)

/* SPI controller regs */

#define SPI_BASE                DEV_BASE
#define SPI_MODE_REG            ((volatile unsigned *)(SPI_BASE+0x00))
#define SPI_TIME_REG            ((volatile unsigned *)(SPI_BASE+0x04))
#define SPI_CTL_REG             ((volatile unsigned *)(SPI_BASE+0x08))
#define SPI_STAT_REG            ((volatile unsigned *)(SPI_BASE+0x0c))
#define SPI_RXF_REG             ((volatile unsigned *)(SPI_BASE+0x10))
#define SPI_TXF_REG             ((volatile unsigned *)(SPI_BASE+0x14))
#define SPI_CFG_REG             ((volatile unsigned *)(SPI_BASE+0x18))
#define SPI_FSTAT_REG           ((volatile unsigned *)(SPI_BASE+0x1c))
#define SPI_IEN_REG             ((volatile unsigned *)(SPI_BASE+0x20))
#define SPI_IS_REG              ((volatile unsigned *)(SPI_BASE+0x24))

/* UART */

#define UART0_BASE              (DEV_BASE+0x103)
#define UART1_BASE              (DEV_BASE+0x803)

/* Timers */

#define TIMER0_BASE             (DEV_BASE+0x300)
#define TIMER1_BASE             (DEV_BASE+0x400)
#define TIMER2_BASE             (DEV_BASE+0x500)

#define TIMER_LOAD_LO_REG       0x00
#define TIMER_LOAD_HI_REG       0x04
#define TIMER_READ_LO_REG       0x08
#define TIMER_READ_HI_REG       0x0c
#define TIMER_STATUS_REG        0x10
#define TIMER_SCRATCH_LO_REG    0x14
#define TIMER_SCRATCH_HI_REG    0x18

#define AUX_CLOCK_TIMER		0
#define AUX2_CLOCK_TIMER	1
#define AUX3_CLOCK_TIMER	2

/* DDR2 */

#define DDR2_BASE               (DEV_BASE+0x600)
#define DDR2_INT_STAT_REG       ((volatile unsigned *)(DDR2_BASE+0x24))
#define DDR2_INT_ACK_REG        ((volatile unsigned *)(DDR2_BASE+0x28))
#define DDR2_OUT_OF_RANGE0_REG  ((volatile unsigned *)(DDR2_BASE+0x2c))
#define DDR2_OUT_OF_RANGE1_REG  ((volatile unsigned *)(DDR2_BASE+0x30))
#define DDR2_OUT_OF_RANGE2_REG  ((volatile unsigned *)(DDR2_BASE+0x34))

/* GPIO */

#define GPIO_BASE               (DEV_BASE+0x700)
#define GPIO_IN_REG             ((volatile unsigned *)(GPIO_BASE+0x00))
#define GPIO_OUT_REG            ((volatile unsigned *)(GPIO_BASE+0x04))
#define GPIO_OE_REG             ((volatile unsigned *)(GPIO_BASE+0x08))
#define GPIO_IE_REG             ((volatile unsigned *)(GPIO_BASE+0x0c))

/* I2C */

#define I2C_BASE                (DEV_BASE+0x900)
#define I2C_ADDR_REG            ((volatile unsigned *)(I2C_BASE+0x00))
#define I2C_DATA_REG            ((volatile unsigned *)(I2C_BASE+0x04))
#define I2C_CNTR_REG            ((volatile unsigned *)(I2C_BASE+0x08))
#define I2C_STAT_REG            ((volatile unsigned *)(I2C_BASE+0x0c))
#define I2C_CCR_REG             I2C_STAT_REG
#define I2C_XADDR_REG           ((volatile unsigned *)(I2C_BASE+0x10))
#define I2C_SRST_REG            ((volatile unsigned *)(I2C_BASE+0x1c))

/* PCI Master/Slave */

#define PCI_BASE                (DEV_BASE+0xa00)
#define PS_BAR_REG              ((volatile unsigned *)(PCI_BASE+0x00))
#define PS_SEGMENT_REG          ((volatile unsigned *)(PCI_BASE+0x04))
#define PM_DMA_RAM_ADDR_REG     ((volatile unsigned *)(PCI_BASE+0x08))
#define PM_DMA_PCI_ADDR_REG     ((volatile unsigned *)(PCI_BASE+0x0c))
#define PM_DMA_DATA_VOLUME_REG  ((volatile unsigned *)(PCI_BASE+0x10))
#define PM_CONFIG_ADDR_REG      ((volatile unsigned *)(PCI_BASE+0x14))
#define PM_CONFIG_DATA_REG      ((volatile unsigned *)(PCI_BASE+0x18))
#define PCI_ARBITER_REG         ((volatile unsigned *)(PCI_BASE+0x1c))
#define PM_DMA_STATUS_REG       ((volatile unsigned *)(PCI_BASE+0x20))
#define PCI_MEM_BAR_REG         ((volatile unsigned *)(PCI_BASE+0x24))
#define PCI_IO_BAR_REG          ((volatile unsigned *)(PCI_BASE+0x28))
#define PCI_MASTER_STATUS_REG   ((volatile unsigned *)(PCI_BASE+0x2c))
#define PCI_INT_MASK_REG        ((volatile unsigned *)(PCI_BASE+0x30))
#define PCI_INT_STATUS_REG      ((volatile unsigned *)(PCI_BASE+0x34))

/* PCI_INT_MASK/PCI_INT_STATUS regs bits */

#define PCI_INT_M_ABORT         0x200
#define PCI_INT_T_ABORT         0x100
#define PCI_INT_PERR            0x080
#define PCI_INT_SERR            0x040
#define PCI_INT_RETRY           0x020
#define PCI_INT_TRDY            0x010
#define PCI_INT_ERR             0x3f0
#define PCI_INT_A               0x008
#define PCI_INT_B               0x004
#define PCI_INT_C               0x002
#define PCI_INT_D               0x001

/* DMA */

#define DMA_BASE                (DEV_BASE+0x2000)
#define DMA_RF_CH_MR_REG        ((volatile unsigned *)(DMA_BASE+0x000))
#define DMA_IR_REG              ((volatile unsigned *)(DMA_BASE+0x400))
#define DMA_IE_REG              ((volatile unsigned *)(DMA_BASE+0x404))
#define DMA_CH0_CTRL_REG        ((volatile unsigned *)(DMA_BASE+0x440))
#define DMA_CH0_STAT_REG        ((volatile unsigned *)(DMA_BASE+0x444))
#define DMA_CH0_DADDR_REG       ((volatile unsigned *)(DMA_BASE+0x448))
#define DMA_CH0_IRQ_CAUSE_U_REG ((volatile unsigned *)(DMA_BASE+0x44c))
#define DMA_CH0_CURR_DESC_REG   ((volatile unsigned *)(DMA_BASE+0x450))

#define DMA_CH_MT_CTRL_REG      ((volatile unsigned *)(DMA_BASE+0x500))
#define DMA_CH_MT_STAT_REG      ((volatile unsigned *)(DMA_BASE+0x504))
#define DMA_CH_MT_DADDR_REG     ((volatile unsigned *)(DMA_BASE+0x508))
#define DMA_CH_MT_FUSERID       ((volatile unsigned *)(DMA_BASE+0x50c))

#define DMA_CH_MR_CTRL_REG      ((volatile unsigned *)(DMA_BASE+0x600))
#define DMA_CH_MR_STAT_REG      ((volatile unsigned *)(DMA_BASE+0x604))
#define DMA_CH_MR_IE_REG        ((volatile unsigned *)(DMA_BASE+0x608))
#define DMA_CH_MR_INT_REG       ((volatile unsigned *)(DMA_BASE+0x60c))
#define DMA_CH_MR_SID_REG       ((volatile unsigned *)(DMA_BASE+0x610))
#define DMA_CH_MR_MISADR0_REG   ((volatile unsigned *)(DMA_BASE+0x630))
#define DMA_CH_MR_MISCNT0_REG   ((volatile unsigned *)(DMA_BASE+0x634))
#define DMA_CH_MR_MISADR1_REG   ((volatile unsigned *)(DMA_BASE+0x638))
#define DMA_CH_MR_MISCNT1_REG   ((volatile unsigned *)(DMA_BASE+0x63c))
#define DMA_CH_MR_REALTIME_REG  ((volatile unsigned *)(DMA_BASE+0x640))
#define DMA_CH_MR_FIFO_REG      ((volatile unsigned *)(DMA_BASE+0x644))

#define DMA_CH_VSK_CTRL_REG     ((volatile unsigned *)(DMA_BASE+0x700))
#define DMA_CH_VSK_STAT_REG     ((volatile unsigned *)(DMA_BASE+0x704))
#define DMA_CH_VSK_IE_REG       ((volatile unsigned *)(DMA_BASE+0x708))
#define DMA_CH_VSK_INT_REG      ((volatile unsigned *)(DMA_BASE+0x70c))
#define DMA_CH_VSK_SID_REG      ((volatile unsigned *)(DMA_BASE+0x710))
#define DMA_CH_VSK_MISADR0_REG  ((volatile unsigned *)(DMA_BASE+0x730))
#define DMA_CH_VSK_MISCNT0_REG  ((volatile unsigned *)(DMA_BASE+0x734))
#define DMA_CH_VSK_MISADR1_REG  ((volatile unsigned *)(DMA_BASE+0x738))
#define DMA_CH_VSK_MISCNT1_REG  ((volatile unsigned *)(DMA_BASE+0x73c))
#define DMA_CH_VSK_REALTIME_REG ((volatile unsigned *)(DMA_BASE+0x740))
#define DMA_CH_VSK_WORD_CNT_REG ((volatile unsigned *)(DMA_BASE+0x744))
#define DMA_CH_VSK_FIFO_REG     ((volatile unsigned *)(DMA_BASE+0x748))

#define DMA_CH0_ADDR_LIM_REG    ((volatile unsigned *)(DMA_BASE+0x1000))
#define DMA_CH0_IRQ_MASK_REG    ((volatile unsigned *)(DMA_BASE+0x1004))
#define DMA_CH0_BASE_ADDR_REG   ((volatile unsigned *)(DMA_BASE+0x1008))
#define DMA_CH0_IRQ_CAUSE_REG   ((volatile unsigned *)(DMA_BASE+0x100c))
#define DMA_CH0_CONFIG_REG      ((volatile unsigned *)(DMA_BASE+0x1010))

/* RIO */

#define RIO_BASE                (DEV_BASE+0x4000)
#define RIO_GRP_INT_REQ_REG     ((volatile unsigned *)(RIO_BASE+0x000))
#define RIO_GRP_INT_EN_REG      ((volatile unsigned *)(RIO_BASE+0x004))
#define RIO_GRP_PLL_REG         ((volatile unsigned *)(RIO_BASE+0x020))
#define RIO_GRP_DLL_REG         ((volatile unsigned *)(RIO_BASE+0x024))
#define RIO_TCPU_HEAD_REG       ((volatile unsigned *)(RIO_BASE+0x400))
#define RIO_TCPU_ARIO_REG       ((volatile unsigned *)(RIO_BASE+0x404))
#define RIO_TCPU_RXA_REG        ((volatile unsigned *)(RIO_BASE+0x408))
#define RIO_TCPU_TXA_REG        ((volatile unsigned *)(RIO_BASE+0x40c))
#define RIO_TCPU_STATUS_REG     ((volatile unsigned *)(RIO_BASE+0x410))
#define RIO_TCPU_VSK_TXA_REG    ((volatile unsigned *)(RIO_BASE+0x414))
#define RIO_HM_M_CTRL_REG       ((volatile unsigned *)(RIO_BASE+0x800))
#define RIO_HM_M_STAT_REG       ((volatile unsigned *)(RIO_BASE+0x804))
#define RIO_HM_M_TX_HIGH_REG    ((volatile unsigned *)(RIO_BASE+0x810))
#define RIO_HM_M_TX_LOW_REG     ((volatile unsigned *)(RIO_BASE+0x814))
#define RIO_HM_M_TX_CTRL_REG    ((volatile unsigned *)(RIO_BASE+0x818))
#define RIO_HM_M_RX_HIGH_REG    ((volatile unsigned *)(RIO_BASE+0x820))
#define RIO_HM_M_RX_LOW_REG     ((volatile unsigned *)(RIO_BASE+0x824))
#define RIO_HM_M_RX_CTRL_REG    ((volatile unsigned *)(RIO_BASE+0x828))
#define RIO_CAR_DEV_ID_REG      ((volatile unsigned *)(RIO_BASE+0xc00))
#define RIO_CAR_DEV_INFO_REG    ((volatile unsigned *)(RIO_BASE+0xc04))
#define RIO_CAR_ASMBLY_ID_REG   ((volatile unsigned *)(RIO_BASE+0xc08))
#define RIO_CAR_ASMBLY_INFO_REG ((volatile unsigned *)(RIO_BASE+0xc0c))
#define RIO_CAR_PE_FEAT_REG     ((volatile unsigned *)(RIO_BASE+0xc10))
#define RIO_CAR_SOURCE_OP_REG   ((volatile unsigned *)(RIO_BASE+0xc18))
#define RIO_CAR_DEST_OP_REG     ((volatile unsigned *)(RIO_BASE+0xc1c))
#define RIO_CSR_PE_LL_CTL_REG   ((volatile unsigned *)(RIO_BASE+0xc4c))
#define RIO_CSR_CFG_LBAR_REG    ((vHostolatile unsigned *)(RIO_BASE+0xc5c))
#define RIO_CSR_BASE_DEV_ID_REG ((volatile unsigned *)(RIO_BASE+0xc60))
#define RIO_CSR_HB_DEV_ID_LOCK_REG ((volatile unsigned *)(RIO_BASE+0xc68))
#define RIO_CSR_COMP_TAG_REG    ((volatile unsigned *)(RIO_BASE+0xc6c))
#define RIO_PL_PORT_MBH0_REG    ((volatile unsigned *)(RIO_BASE+0xd00))
#define RIO_PL_PORT_LT_CTL_REG  ((volatile unsigned *)(RIO_BASE+0xd20))
#define RIO_PL_PORT_RT_CTL_REG  ((volatile unsigned *)(RIO_BASE+0xd24))
#define RIO_PL_PORT_GEN_CTL_REG ((volatile unsigned *)(RIO_BASE+0xd3c))
#define RIO_PL_PORT0_ERR_STAT_REG ((volatile unsigned *)(RIO_BASE+0xd58))
#define RIO_PL_PORT0_CTL_REG    ((volatile unsigned *)(RIO_BASE+0xd5c))
#define RIO_PKT_CNT0_REG        ((volatile unsigned *)(RIO_BASE+0xd60))
#define RIO_PKT_CNT1_REG        ((volatile unsigned *)(RIO_BASE+0xd64))
#define RIO_RDB_DB_REG          ((volatile unsigned *)(RIO_BASE+0x1400))
#define RIO_RDB_STATUS_REG      ((volatile unsigned *)(RIO_BASE+0x1404))
#define RIO_TDB_DB_REG          ((volatile unsigned *)(RIO_BASE+0x1800))
#define RIO_TDB_STATUS_REG      ((volatile unsigned *)(RIO_BASE+0x1804))
#define RIO_RBUFC_BUF_N_REG     ((volatile unsigned *)(RIO_BASE+0x1c00))
#define RIO_RBUFC_ALLOC_REG     ((volatile unsigned *)(RIO_BASE+0x1c04))
#define RIO_RBUFC_BUF_BASE_REG  ((volatile unsigned *)(RIO_BASE+0x1c08))
#define RIO_TBUFC_BUF_N_REG     ((volatile unsigned *)(RIO_BASE+0x1c80))
#define RIO_TBUFC_ALLOC_REG     ((volatile unsigned *)(RIO_BASE+0x1c84))
#define RIO_TBUFC_BUF_BASE_REG  ((volatile unsigned *)(RIO_BASE+0x1c88))

/* Interrupt controller regs */

#define INTR_BASE               (DEV_BASE+0x200)
#define SYS_INTMAP_REG		((volatile unsigned *)(INTR_BASE+0x00))
#define SYS_INTREQ_REG          ((volatile unsigned *)(INTR_BASE+0x80))
#define SYS_INTENABLE_REG       ((volatile unsigned *)(INTR_BASE+0x90))

/* interrupt requests */

#define INT_REQ_SPI             0
#define INT_REQ_UART0           1
#define INT_REQ_PCI_S           2
#define INT_REQ_UART1           3
#define INT_REQ_I2C             4
#define INT_REQ_TIMER0          5
#define INT_REQ_DMA             6
#define INT_REQ_CP2             7
#define INT_REQ_PCI_M           8
#define INT_REQ_TIMER1          9
#define INT_REQ_TIMER2          10
#define INT_REQ_RIO             11
#define INT_REQ_DDR2            12
#define INT_REQ_NMI             15

#define INT_REQ_EXT0            26
#define INT_REQ_EXT1            27
#define INT_REQ_EXT2            28
#define INT_REQ_EXT3            29
#define INT_REQ_EXT4            30
#define INT_REQ_EXT5            31

#define INT_REQ_MAX             32

/* SYS_INTMAPx register bits */

#define INT_SENSE_LVL           0x00
#define INT_SENSE_EDGE          0x20

#define INT_LVL_HIGH            0x00
#define INT_LVL_LOW             0x10

#define INT_EDGE_RISE           0x00
#define INT_EDGE_FALL           0x10

#define INT_MAP_DISABLE         0x0f

/* system controller vectors base */

#define IV_BASE                 0x40

#define IV_SPI                  (IV_BASE+INT_REQ_SPI)
#define IV_UART0                (IV_BASE+INT_REQ_UART0)
#define IV_PCI_S                (IV_BASE+INT_REQ_PCI_S)
#define IV_UART1                (IV_BASE+INT_REQ_UART1)
#define IV_I2C                  (IV_BASE+INT_REQ_I2C)
#define IV_TIMER0               (IV_BASE+INT_REQ_TIMER0)
#define IV_DMA                  (IV_BASE+INT_REQ_DMA)
#define IV_CP2                  (IV_BASE+INT_REQ_CP2)
#define IV_PCI_M                (IV_BASE+INT_REQ_PCI_M)
#define IV_TIMER1               (IV_BASE+INT_REQ_TIMER1)
#define IV_TIMER2               (IV_BASE+INT_REQ_TIMER2)
#define IV_RIO                  (IV_BASE+INT_REQ_RIO)
#define IV_DDR2                 (IV_BASE+INT_REQ_DDR2)
#define IV_NMI                  (IV_BASE+INT_REQ_NMI)

#define IV_EXT0                 (IV_BASE+INT_REQ_EXT0)
#define IV_EXT1                 (IV_BASE+INT_REQ_EXT1)
#define IV_EXT2                 (IV_BASE+INT_REQ_EXT2)
#define IV_EXT3                 (IV_BASE+INT_REQ_EXT3)
#define IV_EXT4                 (IV_BASE+INT_REQ_EXT4)
#define IV_EXT5                 (IV_BASE+INT_REQ_EXT5)

#define IV_SPURIOUS0            0x70
#define IV_SPURIOUS1            0x71
#define IV_SPURIOUS2            0x72
#define IV_SPURIOUS3            0x73
#define IV_SPURIOUS4            0x74

#define IV_USR_EXT_INT0         IV_EXT3 
#define IV_USR_EXT_INT1         IV_EXT4 

/* cpu internal vectors */

#define IV_TIMER                0x60

/* timer control register bits */

#define TIMER_ONE_SHOT		0x00000001
#define TIMER_RUN		0x00000002
#define TIMER_INT_ENABLE 	0x00000004
#define TIMER_INT_PENDED 	0x00000008
#define TIMER_EXTERNAL_FREQ	0x00000010
#define TIMER_RST 		0x00000020
#define TIMER_COUNT_LOAD 	0x00000040
#define TIMER_FEN 		0x00000080

#ifndef __ASSEMBLER__

int  rionetDevInit(unsigned rxSize, char* ifname);
void boardRioHWInit(int prio,int timeo,int rio_id_max, int rreg_prio, int rreg_timeo );
void  boardExtInt0Set(int set);
void boardExtInt1Set(int set);
int boardRioIdGet(void);
int boardTimerCountSet64(unsigned ix, uint64_t count);
uint64_t boardTimerCountGet64(unsigned ix);
int boardTimeModeSet(unsigned ix, uint32_t mode);
uint32_t boardTimeModeGet(unsigned ix);
#ifdef FOR_SUBMICRON
extern unsigned time_start_boardHWInit1;   //gmu: otl
extern unsigned time_clear_cp0count_hw1;       //gmu: otl
extern unsigned time_sync_VM6_VM7;       //gmu: otl


extern int rioInitAll(void); // gmu: для того чтобы запускать вне первоночальной инициализации
extern int netconfig();      // gmu: для того чтобы запускать вне первоночальной инициализации


//#define DEBUG_SAVE_INIT_TIME

#ifdef DEBUG_SAVE_INIT_TIME  //debug
// для того чтобы понять какие функции запускаются при инициализации и в какое время
// сохраняем информацию о выполнявшихся функциях в специальный буфер
extern char string_buf_for_init_info[4096];  // буфер для хранения информации о инициализации
extern unsigned counter_buf_for_init;  // указатель на область для записи в буфер инициализации

// TODO: частота процессора пока константа 200
#define WRITE_TOBUF_INIT(a) if(counter_buf_for_init<4096){ \
 counter_buf_for_init+=sprintf(string_buf_for_init_info+counter_buf_for_init,"Start %s time  %d mks",#a,(boardCountGet()*2)/125); \
 a; \
 counter_buf_for_init+=sprintf(string_buf_for_init_info+counter_buf_for_init," done time %d mks\n",(boardCountGet()*2)/125); }

#else
#define WRITE_TOBUF_INIT(a) a;
#endif //DEBUG_SAVE_INIT_TIME

#else
#define WRITE_TOBUF_INIT(a) a;
#endif //FOR_SUBMICRON

#endif /* __ASSEMBLER__ */

#endif	/* __INCboardh__ */


