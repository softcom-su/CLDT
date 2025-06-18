#ifndef _DRV401A_H
#define _DRV401A_H

/************************/
/* header for drv401a      */
/*  history
   13apr10,laz -  oc3000 adaptation
   10nov04,laz - BT206 version
   20sep04,laz - localMemSize
   23feb04,laz - RT_RW_mask added to MPI_DEV
   01nov01,laz - (bus, dev, fun) for BTM401 and BT83401
   25oct01,laz - SETBUSY, CHAINSTOP
   04jun01,laz - BusErrorFlag added to MPI_DEV
   23.12.98,sha - first version
*/
/************************/

#include <private/ios.h>

#define NO_BUSY_IN_START

#ifndef	BOOL
  #define	BOOL	int
#endif

#ifdef BT83401
  #include "qbusLib.h"
#endif
#include "usr401a.h"

#define DRV401A_VERSION "6.4"

#define DRV_LOGLEVEL (0)

#define RT_SERVER_PRIORITY 235  /* may be changed by FIORTSERVERPRIOSET */
#define CHAIN_SEM_INT_WAIT 10   /* clock ticks to wait sem_int, when chain starts */
                                /* may be changed only by FIOCHAINTWAIT */
#define RT_SERVER_SEM_INT_WAIT 10 /* clock ticks to wait sem_int in RT_Server */
                                 /* may be changed only by FIORTSERVERTWAIT */

#if OS_VERSION == 206
  #include <inttypes.h>
#endif

#if OS_VERSION >= 300
 #define MAX_MES_NUM	1024	/* 32*32 */
 #define SEM_SHARING_401     1
 #include <sys/types.h>
#else
 #define SEM_SHARING_401     0
#endif

typedef struct {
   unsigned int 	open;

   unsigned int		mode_rd;      // single/double/circular
   unsigned int		mode_td;      // single/double/circular
   unsigned short	size_rd;      // size of local mem receive buf (R-buf)
   unsigned short	size_td;      // size of local mem transmit buf(T-buf)

   unsigned short 	spp_td;       // copy of subadr parameters word for tranceiver
   unsigned short	spp_rd;       // copy of subadr parameters word for receiver
   unsigned short	buf_rd;       // local mem R-buf adr
   unsigned short	buf_td;       // local mem T-buf adr
   
   unsigned short 	*ringbuf;        // pointer to ring T-buf
   unsigned short 	cur_pos_ringbuf; // ring T-buf index to store data (FIOSETBLOCK)
   unsigned short 	beg_pos_ringbuf; // ring T-buf index to transmit from
   unsigned short	size_ringbuf;    // size of ring T-buf
   unsigned short 	free_ringbuf;    // number of free words in ring T-buf
   unsigned short	pos_td;        // index in local mem T-buf
   unsigned short	busy_td;
   unsigned short	num_full_buf;  // for FIOSETBLOCK in DOUBLE_BUF mode
   unsigned             nOut, nIn, nSet, nGet, nErrs, nBusyIn, nBusyOut, nIll, nNospace, nNoaw;     // counters of messages and errors

#ifdef BT401ARINC
   QUEUING_PORT_ID_TYPE idr;	      // tx port ID
#endif

} bt401sa;

typedef struct
  {
#ifdef _FOR_OCPB
   DEV_HDR   devHdr;
#else
   ioDEV     devHdr;
#endif
   unsigned short    	*base_ram;
   uintptr_t     	base_reg;
   unsigned short     	channel;
   int        	        irq;
   int           	vec;
   int                  localMemSize;
   unsigned int		sw;
   unsigned short	reg_int;
   unsigned short	reg_mask;
   unsigned short	reg_config;
   unsigned short	reg_start;
   unsigned short 	reg_ptr;
   bt401mode     	mode;
   sem_t        	sem_int;
   int          	dev_open;
   bt401stats 	        stats;

/*  for bc */
   unsigned short	ck_A_ptr;
   unsigned short	ck_A_size;
   unsigned short	ck_A_init;
   unsigned short	ck_B_ptr;
   unsigned short	ck_B_size;
   unsigned short	ck_B_init;
   pthread_mutex_t      sem_A;
   pthread_mutex_t      sem_B;
   pthread_mutex_t      sem_sync;
   pthread_mutex_t     	sem_ini;

   unsigned short	beg_ptr_buf; /* 1st word after ring protocol buffer */
   unsigned short 	cur_ptr_buf;
   unsigned short	buf_ptr;  /* ring protocol buffer current pointer */
   unsigned short 	ch;
   int                  bcgetsss; /* StartChain saves here SSS on error */
   int                  bcSemWait; /* semTimedwait ticks */
   unsigned short       RT_RW_mask[32]; /* bit masks for answer words - bits to ignore */

/*  for rt  */ 
   bt401sa      	sa[32];
   int       	 	tID;
   pthread_mutex_t     	sem_rd;
   pthread_mutex_t     	sem_td;
   pthread_mutex_t     	sem_rd_data;
   pthread_cond_t       cond_rd_data;
   pthread_mutex_t     	sem_td_data;
   pthread_cond_t       cond_td_data;
   pthread_mutex_t     	sem_wai;
   pthread_cond_t       cond_wai;
   unsigned short       wait_com_ks;   // Command word for FIOWAITCOM
   unsigned short	sss;
   unsigned short 	ks;
   unsigned short	*ringbuf;
   unsigned short	cur_pos_ringbuf;
   unsigned short	beg_pos_ringbuf;
   unsigned short	size_ringbuf;
   unsigned short	free_ringbuf;
   unsigned short	busy_ringbuf;

   unsigned short	size_tdringbuf; //from bt401DevCreate T-buf size
   int			BusErrorFlag;
   int                  nberrs;
   int                  moduleType;    // BT23401A, BT83401, BTM23401
   int                  busySet;       // to set BYSY flag in START_REG or not
   int                  chainStop;     // to stop chain on each message or not
   int                  rtServerPrio;
   int                  rtServerTwait;
   int                  saveAllComs;   // flag of saving all commands in input ring buf */
   int                  nErrs;         // n of errors, which cannot be bind to a SA
         /*         for MT */
   unsigned short	cur_len_a;
   unsigned short	cur_len_b;
   unsigned short	cur_ptr_a;
   unsigned short	cur_ptr_b;
   unsigned short	busy_reg;

      //  unsigned int no_timeout; /* BC, RT: semWaitWithTimeout used (=0) or simple sem_wait (=1) */
   int                  busyPolicy;    /* =0 - old rules for RT BUSY setting, =1 - new rules (almost no BUSY) */
   int                  ringBufSize;   /* size of ring buffer for RT events */
#ifdef BT5890
   unsigned             userReadReg;   /* 5890VG1 - register of 16 input signals */
   unsigned             userWriteReg;  /* 5890VG1 - register of 16 output signals */
#endif
#ifdef BT83401
   QBUS_ID	qbusId;
#endif

#ifdef BT401ARINC
   int       	 	trArincID;
   QUEUING_PORT_ID_TYPE idr;	      // rx port ID
#endif
#if 0 /* OS_VERSION > 300 */
   paddr_t              base_ram_pa;
#endif

  } MPI_DEV;

typedef struct {
	uintptr_t base_ram;
	uintptr_t base_reg;
	unsigned  base_vec;
	unsigned int level;
	unsigned int max_num_ch;
        int          localMemSize;
#if defined (BTM401) || defined (BT83401)
        int          bus, dev, fun;  // PCI location
        unsigned     devVendorCode;
#endif
	MPI_DEV *mpi[8];
#ifdef BT83401
 	QBUS_ID	qbusId;
#endif
} MPI_MODULE;


typedef MPI_DEV *MPI_DEV_ID;

/**************** Standard Manchester definitions **********************/
#define COM_N_MASK            0x001F
#define COM_S_SHIFT           5       /* for subadr shift */
#define COM_S_MASK            0x03E0
#define COM_A_SHIFT           11      /* for adr shift */
#define COM_A_MASK            0xF800

#define COM_RT_BC             0x0400

#define TYPE_BT23401A 0
#define TYPE_BT83401  1
#define TYPE_BTM23401 2

#define USERREADREG          0xA0028  /* 5890VG1 - offset of register for 16 input signals */
#define USERWRITEREG         0xA002c  /* 5890VG1 - offset of register for 16 output signals */

/************** Common BC & RT & MT definitions & variables ******************/
/* Manchester chip registers definitions */
/* Registers offsets */


#undef  R_CONFIG

#ifdef BT23401A
  #define HOLE               	1
  #define SHIFT		        0
  #define R_CONFIG             0x00
  #define R_START              0x02
  #define R_MASK               0x04
  #define R_INT                0x06
  #define R_STATUS             0x08
  #define R_POINTER            0x0a
  #define R_CONTROL            0x0c
  #define R_TIMER              0x0e
  #define R_SUBSTAT            0x0e
  #define R_CODER              0x10
  #define R_DECODER            0x10
  #define R_WORD               0x12
  #undef  CT_LE_W
  #define CT_LE_W(x)           x
  #undef  CF_LE_W
  #define CF_LE_W(x)           x
#endif

#if (defined BT83401) || (defined BTM401)
#define HOLE   	                2
#define SHIFT		        1
#define R_CONFIG             0x02
#define R_START              0x06
#define R_MASK               0x0a
#define R_INT                0x0e
#define R_STATUS             0x12
#define R_POINTER            0x16
#define R_CONTROL            0x1a
#define R_TIMER              0x1e
#define R_SUBSTAT            0x1e
#define R_CODER              0x22
#define R_DECODER            0x22
#define R_WORD               0x26

#endif

/* Local Memory size (16 bit words) */
#define L_M_SIZE(x)          (x->localMemSize)   // 0x400

/* Each monitor buffer size in local memory */
#define BUF_MON_SIZE(x)         ((L_M_SIZE(x)-0x60/*0xA*/) / 2)


/* Control code */
#define CONTROL_CODE           0xFFFF



/************************ Bus Controller (BC) ****************************/
/* BC registers definitions */
/* R_CONFIG BC bits ---------  some bits are in usr401a.h  */
#define BC_CONFIG_RESERV        0xE000
#define BC_CONFIG_ONLST         0x1000
#define BC_CONFIG_OFLST         0x0800
#define BC_CONFIG_UP            0x0400
#define BC_CONFIG_TRM           0x0200
#define BC_CONFIG_TRL           0x0100
#define BC_CONFIG_IAC           0x0080
#define BC_CONFIG_ARE           0x0004
#define BC_CONFIG_BRM           0x0003

/* Answer word delay */
#define DELAY_40          0
#define DELAY_UNLIMITED   BC_CONFIG_TRL
#define DELAY_80          BC_CONFIG_TRM
#define DELAY_160         (BC_CONFIG_TRM+BC_CONFIG_TRL)



/* R_START BC bits */
#define BC_START_RESERV         0xFFC0
#define BC_START_AB             0x0020
#define BC_START_SM             0x0010
#define BC_START_SF             0x0008
#define BC_START_IR             0x0004
#define BC_START_ST             0x0002
#define BC_START_SR             0x0001

/* R_MASK BC bits */
#define BC_MASK_RESERV          0xFFE0
#define BC_MASK_ST              0x0010
#define BC_MASK_ME              0x0008
#define BC_MASK_SB              0x0004
#define BC_MASK_EF              0x0002
#define BC_MASK_EM              0x0001

/* R_INT BC bits */
#define BC_INT_MI               0x8000
#define BC_INT_RESERV           0x7FE0
#define BC_INT_ST               0x0010
#define BC_INT_ME               0x0008
#define BC_INT_SB               0x0004
#define BC_INT_EF               0x0002
#define BC_INT_EM               0x0001

/* R_STATUS BC bits */
#define BC_STATUS_ACT                  0x8000
/* Decoder bits */
#define BC_STATUS_D_SYNC_TYPE          0x4000
#define BC_STATUS_D_RECEIVE            0x2000
#define BC_STATUS_D_READY              0x1000
#define BC_STATUS_D_WAIT_RECEIVE       0x0800
#define BC_STATUS_D_DATA_VALID         0x0400
#define BC_STATUS_D_UNBREAK_RECEIVE    0x0200
/* Coder bits */
#define BC_STATUS_C_READY              0x0100
#define BC_STATUS_C_NOT_TRANSMIT       0x0080
#define BC_STATUS_C_TRANSMIT           0x0040
#define BC_STATUS_C_TIMEOUT            0x0020
/* Other */
#define BC_STATUS_REPLAY_WORD_WAIT     0x0010
#define BC_STATUS_NK                   0x0008  
#define BC_STATUS_NF                   0x0004  
#define BC_STATUS_FP                   0x0002
#define BC_STATUS_MP                   0x0001  

/* R_CONTROL BC bits */
#define BC_CONTROL_RESERV       0xC020
#define BC_CONTROL_ME           0x2000
#define BC_CONTROL_SRQ          0x1000
#define BC_CONTROL_BRF          0x0800
#define BC_CONTROL_BUSY         0x0400
#define BC_CONTROL_SSF          0x0200
#define BC_CONTROL_TF           0x0100
#define BC_CONTROL_AC           0x0080
#define BC_CONTROL_RE           0x0040
#define BC_CONTROL_IE           0x0010
#define BC_CONTROL_AB           0x0008
#define BC_CONTROL_MC           0x0004
#define BC_CONTROL_BR           0x0002
#define BC_CONTROL_RT           0x0001

/* Chain pointer indexes */
#define CURPTRA         0
#define CURCNTA         1
#define BEGPTRA         2
#define BEGCNTA         3
#define CURPTRB         4
#define CURCNTB         5
#define BEGPTRB         6
#define BEGCNTB         7
#define BC_BEG_FREE_MEM 8

/* Control word format & combinations */
#define CW_RESERV         0xC020
#define CW_ME             0x2000
#define CW_SRQ            0x1000
#define CW_BRF            0x0800
#define CW_BUSY           0x0400
#define CW_SSF            0x0200
#define CW_TF             0x0100
#define CW_AC             0x0080
#define CW_RE             0x0040
#define CW_IE             0x0010
#define CW_AB             0x0008
#define CW_MBR            0x0007
#define CW_MC             0x0004
#define CW_BR             0x0002
#define CW_RT             0x0001

#define CW_ALL_MASK  (CW_ME+CW_SRQ+CW_BRF+CW_BUSY+CW_SSF+CW_TF)

/* Status word format */
#define RT_SW_EM             0x8000
#define RT_SW_SM             0x4000
#define RT_SW_AB             0x2000
#define RT_SW_ERR            0x1000
#define RT_SW_RT             0x0800
#define RT_SW_FE             0x0400
#define RT_SW_NAW            0x0200
#define RT_SW_ST             0x0100
#define RT_SW_ILL            0x0080
#define RT_SW_BUSY           0x0040
#define RT_SW_BOF            0x0020
#define RT_SW_IC             0x0010
#define RT_SW_ISC            0x0008
#define RT_SW_IWC            0x0004
#define RT_SW_IS             0x0002
#define RT_SW_IW             0x0001



/* Chain definitions */
#define CH_A                    0
#define CH_B                    1



/************************ Remote Terminal (RT) ****************************/
/* RT registers definitions */
/* R_CONFIG RT bits */
#define RT_CONFIG_BCE           0x8000
#define RT_CONFIG_BDE           0x4000
#define RT_CONFIG_BE            0x2000
#define RT_CONFIG_STONL         0x1000
#define RT_CONFIG_STOFL         0x0800
#define RT_CONFIG_PAR           0x0400
#define RT_CONFIG_TR            0x0300
#define RT_CONFIG_IAC           0x0080
#define RT_CONFIG_ADR_MASK      0x007C
#define RT_CONFIG_ADR_SHIFT  	2
#define RT_CONFIG_BRM           0x0003

/* R_START RT bits */
#define RT_START_RESERV         0xFC00
#define RT_START_IFT            0x0200
#define RT_START_INB            0x0100
#define RT_START_INA            0x0080
#define RT_START_BUSY           0x0040
#define RT_START_SRQ            0x0020
#define RT_START_FS             0x0010
#define RT_START_FT             0x0008
#define RT_START_IR             0x0004
#define RT_START_ST             0x0002
#define RT_START_SR             0x0001

/* R_MASK RT bits */
#define RT_MASK_RESERV          0xFF80
#define RT_MASK_MMC             0x0040
#define RT_MASK_MA              0x0020
#define RT_MASK_MT              0x0010
#define RT_MASK_MF              0x0008
#define RT_MASK_MC              0x0004
#define RT_MASK_MP              0x0002
#define RT_MASK_ME              0x0001

/* R_INT RT bits */
#define RT_INT_INT              0x8000
#define RT_INT_RESERV           0x7F80
#define RT_INT_MC               0x0040
#define RT_INT_NA               0x0020
#define RT_INT_STERR            0x0010
#define RT_INT_FE               0x0008
#define RT_INT_CBOVF            0x0004
#define RT_INT_PBOVF            0x0002
#define RT_INT_EM               0x0001

/* R_STATUS RT bits */
#define RT_STATUS_ACT                  0x8000
/* Decoder bits */
#define RT_STATUS_D_SYNC_TYPE          0x4000
#define RT_STATUS_D_RECEIVE            0x2000
#define RT_STATUS_D_READY              0x1000
#define RT_STATUS_D_WAIT_RECEIVE       0x0800
#define RT_STATUS_D_DATA_VALID         0x0400
#define RT_STATUS_D_UNBREAK_RECEIVE    0x0200
/* Coder bits */
#define RT_STATUS_C_READY              0x0100
#define RT_STATUS_C_NOT_TRANSMIT       0x0080
#define RT_STATUS_C_TRANSMIT           0x0040
#define RT_STATUS_C_TIMEOUT            0x0020
/* Other */
#define RT_STATUS_WAW                  0x0010  
#define RT_STATUS_NK                   0x0008  
#define RT_STATUS_NF                   0x0004  
#define RT_STATUS_NA                   0x0002  
#define RT_STATUS_MP                   0x0001  

/* R_SUBSTAT RT bits */
#define RT_CONTROL_IE           0x8000
#define RT_CONTROL_BUSY         0x4000
#define RT_CONTROL_SRQ          0x2000
#define RT_CONTROL_SEL          0x1000
#define RT_CONTROL_BUF_H        0x0800
#define RT_CONTROL_BUF_L	0x0400
#define RT_CONTROL_MC_MASK      0x03E0
#define RT_CONTROL_MC_SHIFT	5
#define RT_CONTROL_BL_MASK      0x001F

#define BUF_SINGLE		0x0
#define BUF_CIRCULAR		RT_CONTROL_BUF_H
#define BUF_DOUBLE		RT_CONTROL_BUF_L
#define BUF_DISABLE		RT_CONTROL_BUF_L | RT_CONTROL_BUF_H

/* RT memory */

#define CURPTRBUF	0
#define BEGPTRBUF	2
#define CURCNTBUF	1
#define BEGCNTBUF	3

#define RT_BEG_FREE_MEM  	0x92
#define RT_BEG_FREE_MEM_GRP	0xCE

#define SIZE_BUF_PRT    10      /* size of ring buffer for RT events */
#define SPP_RD		0
#define ADR_RD		1
#define SPP_TD		2
#define ADR_TD		3
#define SPP_GRP		0x90
#define ADR_GRP		0x91

#define CTRL_EN1	0x7C
#define CTRL_INT1	0x7D
#define CTRL_EN2	0x7E
#define CTRL_INT2	0x7F
#define CTRL_ADR	0x80

#define BUF_SSS		0
#define BUF_TIMER	1
#define BUF_CW		2
#define BUF_ADR		3

/* Format Msg Status Word */
#define SSS_EM		0x8000
#define SSS_SM		0x4000
#define SSS_AB		0x2000
#define SSS_ERR		0x1000
#define SSS_RT		0x0800
#define SSS_FE		0x0400
#define SSS_NAW		0x0200
#define SSS_ST		0x0100
#define SSS_ILL		0x0080
#define SSS_BUSY	0x0040
#define SSS_BOF		0x0020
#define SSS_IC		0x0010
#define SSS_ISC		0x0008
#define SSS_IWC		0x0004
#define SSS_IS		0x0002
#define SSS_IW		0x0001

/* SPP RT bits */
#define SPP_IE           0x8000
#define SPP_BUSY         0x4000
#define SPP_SRQ          0x2000
#define SPP_SEL          0x1000
/*@@@@@@@ sveta begin @@@@@@@@*/
#define SPP_BUF_SHIFT	 10
/*@@@@@@@ sveta end @@@@@@@@*/
#define SPP_BUF_H        0x0800
#define SPP_BUF_L	 0x0400
#define SPP_MC_MASK      0x03E0
#define SPP_MC_SHIFT	 5
#define SPP_BL_MASK      0x001F

/************************ Monitor (MT) ****************************/
/* MT registers definitions */
/* R_CONFIG MT bits */
#define MT_CONFIG_RESERV        0xE77C
#define MT_CONFIG_ONLST         0x1000
#define MT_CONFIG_OFLST         0x0800
#define MT_CONFIG_IAC           0x0080
#define MT_CONFIG_BRM           0x0003

/* R_START MT bits */
#define MT_START_RESERV         0xFF00
#define MT_START_CMD            0x0080
#define MT_START_MON_MASK       0x0060
#define MT_START_MON_SHIFT      5
#define MT_START_STP            0x0010
#define MT_START_CONT           0x0008
#define MT_START_IR             0x0004
#define MT_START_ST             0x0002
#define MT_START_SR             0x0001

/* R_MASK MT bits */
#define MT_MASK_RESERV          0xFF80
#define MT_MASK_STOP            0x0040
#define MT_MASK_NA              0x0020
#define MT_MASK_START           0x0010
#define MT_MASK_ME              0x0008
#define MT_MASK_CMD             0x0004
#define MT_MASK_SB              0x0002
#define MT_MASK_WW              0x0001

/* R_INT MT bits */
#define MT_INT_MI               0x8000
#define MT_INT_RESERV           0x7F80
#define MT_INT_STOP             0x0040
#define MT_INT_NA               0x0020
#define MT_INT_START            0x0010
#define MT_INT_ME               0x0008
#define MT_INT_CMD              0x0004
#define MT_INT_SB               0x0002
#define MT_INT_WW               0x0001

/* R_STATUS MT bits */
#define MT_STATUS_ACT                  0x8000
/* Decoder bits */
#define MT_STATUS_D_SYNC_TYPE          0x4000
#define MT_STATUS_D_RECEIVE            0x2000
#define MT_STATUS_D_READY              0x1000
#define MT_STATUS_D_WAIT_RECEIVE       0x0800
#define MT_STATUS_D_DATA_VALID         0x0400
#define MT_STATUS_D_UNBREAK_RECEIVE    0x0200
/* Coder bits */
#define MT_STATUS_C_READY              0x0100
#define MT_STATUS_C_NOT_TRANSMIT       0x0080
#define MT_STATUS_C_TRANSMIT           0x0040
#define MT_STATUS_C_TIMEOUT            0x0020
/* Other */
#define MT_STATUS_WAW                  0x0010  
#define MT_STATUS_NK                   0x0008  
#define MT_STATUS_NA                   0x0002
#define MT_STATUS_BUF                  0x0004
#define MT_STATUS_BUF_SHIFT            2
//#define MT_STATUS_BUF                  0x0002
//#define MT_STATUS_NA                   0x0004  
#define MT_STATUS_MP                   0x0001  

/* MT memory */

#define CURPTRBUFMONA			0
#define CURCNTBUFMONA			1
#define BEGPTRBUFMONA			2
#define BEGCNTBUFMONA			3
#define CURPTRBUFMONB			4
#define CURCNTBUFMONB			5
#define BEGPTRBUFMONB			6
#define BEGCNTBUFMONB			7

/******************************************************************/

#endif	/* _DRV401A_H */

