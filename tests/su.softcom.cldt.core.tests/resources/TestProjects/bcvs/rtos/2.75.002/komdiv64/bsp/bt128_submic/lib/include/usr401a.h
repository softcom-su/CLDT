#ifndef _USR401A_H
#define _USR401A_H

#ifdef RAM_LOW_ADRS     /* for examples/bt401 in SOURCE_BROWSE */
  #include "configbrd.h"
  #ifdef INCLUDE_BTM401
    #define BTM401
  #endif
  #ifdef INCLUDE_BT401a
    #define BT23401A
  #endif
  #ifdef INCLUDE_BT83401
    #define BT83401
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _FOR_OCPB 
  #define STAT_OK  OK
  #define STAT_ER  ERROR
#endif

#ifdef BTM401
  #define MAX_MPI_DEVICE 4
  #define MAX_NUM_MODULE 2
#else
  #ifdef BT83401
    #define MAX_MPI_DEVICE 8
    #define MAX_NUM_MODULE 4
  #else
    #define MAX_MPI_DEVICE 31
    #define MAX_NUM_MODULE 8
  #endif
#endif
#define MAX_CK_LEN     40

typedef struct{
  unsigned int mode      : 2;
  unsigned int wait      : 2; 
/*  BC bits  */
  unsigned int rep       : 1;
/*  RT bits  */    
  unsigned int dev_n     : 5; /* RT adr */
  unsigned int grp_en    : 1;
  unsigned int grp_dif   : 1;
  unsigned int mode_wait : 1; /* MOD_WAIT for 401 compatible*/
} bt401mode;

typedef struct {
   int com;   
   int sa1;    /* subadr */
   int rt1;    /* RT adr */
   int rt2;
   int sa2;
   int size;
   short aw1;
   short aw2;
   short res;
   short *buf; 
} bt401com;

typedef struct {
   short ck_len;
   bt401com ck[MAX_CK_LEN]; 
} bt401chain;

typedef struct {
   int sa;        /* subaddress (1..30) */
   int mode_rd;   /* SA_MODE_SINGLE/ SA_MODE_DOUBLE/SA_MODE_CIRCULAR */
   int mode_td;   /* SA_MODE_SINGLE/ SA_MODE_DOUBLE/SA_MODE_CIRCULAR */
   int size_rd;   /* space in local mem for read (1..32) */
   int size_td;   /* space in local mem for write (1..32) */

} bt401open; /* for function OPENSA */

#define SA_MODE_SINGLE		0
#define SA_MODE_DOUBLE		1
#define SA_MODE_CIRCULAR	2
#define SA_MODE_DISABLE		3

typedef struct {
   unsigned short *buf;
   unsigned int   sa;
   unsigned int   size;

} bt401block; /* analog sBlock for 401*/


/****** bt401 mode *******/
#define BC     		0
#define RT   		1
#define MT		2 
#define NON		3 

/****** time of the wait answer word  *****/
#define WAIT_40     0
#define WAIT_80     1
#define WAIT_160    2
#define WAIT_UNLIM  3

/* Standard manchester answer word bits */
#ifndef RW_A_SHIFT
  #define RW_A_SHIFT            11  /* right shift count to get RT adr */
  #define RW_A_MASK         0xF800  /* RT adr mask */

  #define RW_MES_ERR        0x0400  /* error in message */
  #define RW_REQ            0x0100  /* request for service */
  #define RW_GROUP          0x0010  /* group command accepted */
  #define RW_BUSY           0x0008  /* abonent is busy */
  #define RW_ABERR          0x0004  /* abonent in failure */
  #define RW_GET_CONTROL    0x0002  /* RT became BC */
  #define RW_RTERR          0x0001  /* RT in failure */
#endif

/* SSS (message status word) bits */
  #define SW_RESERV         0x0030
  #define SW_EM             0x8000
  #define SW_SM             0x4000
  #define SW_AB             0x2000
  #define SW_ERR            0x1000
  #define SW_STS            0x0800
  #define SW_FE             0x0400
  #define SW_NAW            0x0200
  #define SW_ST             0x0100
  #define SW_MSS            0x0080
  #define SW_RET            0x0040
  #define SW_IA             0x0008
  #define SW_IWC            0x0004
  #define SW_IS             0x0002
  #define SW_IW             0x0001

/* Some BC config register bits */
#define BC_CONFIG_SME       0x0040  /* Stop after message on error */
#define BC_CONFIG_SFE       0x0020  /* Stop after all chain on error */
#define BC_CONFIG_SMS       0x0010  /* Stop after message when unmasked bit in answer word */
#define BC_CONFIG_SFS       0x0008  /* Stop after all chain when unmasked bit in answer word */

typedef struct{
   unsigned short *buf;
   unsigned int   rt1   : 5;
   unsigned int   sa1   : 5; 
   unsigned int   rt2   : 5;
   unsigned int   sa2   : 5;
   unsigned short size;
   unsigned short delay;
 
} bt401transf;


typedef struct {
   int     com;
   int     dev;
   short   data ;
} bt401ctrl;

/*@@@@@ sveta begin @@@@@*/
typedef struct {
   int		dev;
   int		sdev;
   int		sn_rv;
   short	data;
} bt401nmbmsgs;
/*@@@@@ sveta end @@@@@*/

typedef struct {
   int send;
   int s_error;
   int s_busy;
   int s_words;
   int receive;
   int r_error;
   int r_busy;
   int r_words;
   int com;
   int c_error;
   int param_error;
   int interrupt;
   int send12;
   int s12_error;
   int s12_busy;
   int transf_error;
   int nDroppedIn;    /* number of dropped input msgs and control commands */
} bt401stats;

typedef struct
  { short rt;
    short mask;
  } bt401awmask;

/*---command for SendCom ----*/

#define CHANGECONTROL  0x00
#define SYNCRO         0x01
#define GIVEOS         0x02
#define BEGINCONTROL   0x03
#define LOCK           0x04
#define UNLOCK         0x05
#define LOCKFAULT      0x06
#define LOCKERROR      0x06
#define UNLOCKFAULT    0x07
#define UNLOCKERROR    0x07
#define RESET          0x08
#define GIVEVECWORD    0x10
#define SYNCROWORD     0x11
#define SYNCWITHWORD   0x11
#define GIVELASTCOM    0x12
#define GIVEVSK        0x13
#define LOCKi          0x14
#define UNLOCKi        0x15

/*!!!!!!!!!!!!!!!! FOR SUBMICRON */  //gmu
/*---command for SendCom ----*/ /* cod control 0x1f */ 

#define CHANGECONTROL_31  (0x20|0x00)
#define SYNCRO_31         (0x20|0x01)
#define GIVEOS_31         (0x20|0x02)
#define BEGINCONTROL_31   (0x20|0x03)
#define LOCK_31           (0x20|0x04)
#define UNLOCK_31         (0x20|0x05)
#define LOCKFAULT_31      (0x20|0x06)
#define LOCKERROR_31      (0x20|0x06)
#define UNLOCKFAULT_31    (0x20|0x07)
#define UNLOCKERROR_31    (0x20|0x07)
#define RESET_31          (0x20|0x08)
#define GIVEVECWORD_31    (0x20|0x10)
#define SYNCROWORD_31     (0x20|0x11)
#define SYNCWITHWORD_31   (0x20|0x11)
#define GIVELASTCOM_31    (0x20|0x12)
#define GIVEVSK_31        (0x20|0x13)
#define LOCKi_31          (0x20|0x14)
#define UNLOCKi_31        (0x20|0x15)
/*!!!!!!!!!!!!!!!! FOR SUBMICRON */

/*--- Commands for ioctl ---*/

#define FIOSETMODE         1005
#define FIOGETMODE         1006
#define FIOGETSTATUSIO     1011
#define FIORESET	   1024

/*new*/
#define FIOSETMODE401A	   1101
#define FIOGETMODE401A     1102

#define FIOMEMORYMAP       1202

/* 10may2011 addition */

#define FIONOSEMWAITTIMEOUT 1201

/*  for controller */
#define FIOSEND            1001
#define FIORECEIVE         1002
#define FIORESEAVE         1002
#define FIOSENDCOM         1018
#define FIOSETCHANEL       1014
#define FIOGETCHANEL       1015

#define FIOSEND1to2        1023

/*new*/
#define FIOINITCHAINA      1103
#define FIOINITCHAINB      1104
#define FIOSTARTCHAINA     1105
#define FIOSTARTCHAINB     1106 
#define FIOCLRCHAINA	   1107
#define FIOCLRCHAINB	   1108
#define FIOCHAINSTOP       1113
#define FIOBCGETSSS        1116
#define FIOCHAINTWAIT      1119

#define FIORTRWMASKSET     1121
#define FIOBCCONFIGSET     1122
#define FIOBCCONFIGGET     1123

/*@@@@@ sveta begin @@@@@*/
#define FIOSENDNMBMSGS     1115
/*@@@@@ sveta end @@@@@*/

/***  for RT   ***/
#define FIOGETBLOCK        1003 
#define FIOSETBLOCK        1004
#define FIOOPENSUBBUF      1008
#define FIOCLOSESUBBUF     1009
#define FIOCLRRDBUF        1012
#define FIOCLRWRTBUF       1013
#define FIOGETFREERDBUF    1016
#define FIOGETFREEWRTBUF   1017
#define FIOSETDEVNUMBER    1007
#define FIOGETINFO1BLOCK   1010
#define FIODROPVECWORD     1019
#define FIODROPVSK         1020
#define FIOGETSYNCROW	   1025
#define FIOWAITCOM	   1027

#define FIOSETGETCTR       1021
#define FIOCLRGETCTR       1022
#define FIOSETREQUEST	   1026

/*new*/
#define FIOCLRREQUEST	   1109
#define FIOGETFREEMEM      1110
#define FIOOPENSA          1111
#define FIOCLOSESA         1112
#define FIOBUSYSET         1114

#define FIORTSERVERPRIOSET 1117
#define FIORTSERVERTWAIT   1118

#define FIORTSAVEALLCOM    1120
#define FIOBUSYPOLICY      1124

/* not aviable */

#define FIOGETSTATS	   1028
#define FIOCLEARSTATS	   1029

/*****  for MT   *****/
#define FIOSTARTMON	   1150
#define FIOSTOPMON	   1151
#define FIOSETETALON	   1152
#define FIOGETETALON	   1153
#define FIOGETPROTOCOL	   1154
#define FIOSETETALONMASK   1155
#define FIOGETETALONMASK   1156
#define FIOGETSTARTREGS    1157

#define ERROR_BUSY          0x0080
#define ERROR_IO            0x0001 
#define ERROR_NO_OS         0x0040
#define ERROR_CONTROL       0x0002
#define ERROR_MSG           0x0100
#define ERROR_FAULT         0x0400
#define ERROR_FAULT_SUBDEV  0x0200 
#define IO_OK               0x0004
#define IO_CONTROL	    0x0008
#define IO_KU		    0x0020
#define IO_REQUEST	    0x0020

/*========*/
extern int  bt401Drv();
extern int  bt401DrvVME();

extern int  bt401DevCreate(
		     char *name,      		/* name device*/
		     int  m,
		     int  ch,			/* channel */
		     int  size_rdbuf,
		     int  size_tdbuf
		    );
extern int  bt401DevCreateVME(
		     char *name,      		/* name device*/
		     int  m,
		     int  ch,			/* channel */
		     int  size_rdbuf,
		     int  size_tdbuf
		    );

#ifdef BT83M
extern int bt401Init( int dev, 
                     int unused,
                     int offset, /* Offset from PCI window start
		                  with 1MB alignment. !!! Not used now !!! */
		     int arg,
		     int max_ch);
#else
extern int  bt401Init(
		     int  arg1,		/* 83-40x - device number on PCI, BTM401/421 - PMC slot (1,2) */
		     int  arg2,		/* interrupt line for 83-40x, 0 - for PMC */
		     int  arg3,		/* 83-401 - offset in PCI memory, 0 - otherwise */
		     int  arg4,		/* 0 */
		     int  arg5		/* max number of channel */
		    );
#endif
extern int  bt401InitVME(
		     int  arg1,		/* REGISTERS BASE ADDRESS in A16 */
		     int  arg2,		/* RAM BASE ADDRESS in A24 */
		     int  arg3,		/* VME interrupts line */
		     int  arg4,		/* interrupts vector (base) */
		     int  arg5		/* max number of channel */
		    );

extern int userReadWriteRegsGet5890(    /* Get addresses of userRead and userWrite regs */ 
                     int modNum,        /* module number - result of bt401Init */
                     unsigned *pRead,   /* pointer to variable for address of userRead register */
                     unsigned *pWrite   /* pointer to variable for address of userWrite register */
                    );
extern unsigned short boardUserIn5890 (      /* read userRead reg to left 16 bits of result */
                     int modNum        /* module number - result of bt401Init */
                    );
extern int      boardUserOut5890 (     /* write userWrite reg from left 16 bits of v */
                     int modNum,       /* module number - result of bt401Init */
                     unsigned short v
                    );
/* ====================*/

/* commands for chain */
#define BC_RT        0
#define RT_BC        1
#define DELAY        2
#define CTRL         3
#define RT_RT        4
#define BC_RT_CHAN_A 5                    /* 22nov06 */
#define BC_RT_CHAN_B 6                    /* 22nov06 */
#define RT_BC_CHAN_A 7                    /* 22nov06 */
#define RT_BC_CHAN_B 8                    /* 22nov06 */
#define RT_RT_CHAN_A 9                    /* 22nov06 */
#define RT_RT_CHAN_B 10                   /* 22nov06 */
#define CTRL_CHAN_A  11                   /* 22nov06 */
#define CTRL_CHAN_B  12                   /* 22nov06 */

#define MOD_CTRL 0x0001
#define MOD_DEV  0x0002
#define MOD_MON  0x0004
#define MOD_1111 0x0008
#define MOD_WAIT 0x0010
#define MOD_ssss 0x0020
#define CONTROLLER 33

#define MOD_STOP    0x0040
#define MOD_CMD     0x0080
#define MOD_CHNBUF  0x0100
#define MOD_WW      0x0200

#define MT_ST		0x0002
#define MT_CONT		0x0008
#define MT_CMD		0x0080
#define MT_COND1	0x0020
#define MT_COND2	0x0040
#define MT_COND3	0x0060

#define MT_READ_RGBUF	1
#define MT_READ_MTBUF	2
#define MT_WRITE_RGBUF	4
#define MT_WRITE_LNBUF	8

#define MONITOR 34

#ifdef __cplusplus
}
#endif

#endif	/* _USR401A_H */
