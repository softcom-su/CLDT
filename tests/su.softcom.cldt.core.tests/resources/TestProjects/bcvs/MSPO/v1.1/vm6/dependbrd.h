/* dependbrd.h - resolve configbrd.h defines */

#ifdef	__BOARD_CONFIG

#ifndef	DE0_10HD
 #define DE0_10HD	0
#else
 #undef	DE0_10HD
 #define DE0_10HD	MII_10HD
#endif
#ifndef	DE0_10FD
 #define DE0_10FD	0
#else
 #undef	DE0_10FD
 #define DE0_10FD	MII_10FD
#endif
#ifndef	DE0_100HD
 #define DE0_100HD	0
#else
 #undef	DE0_100HD
 #define DE0_100HD	MII_100HD
#endif
#ifndef	DE0_100FD
 #define DE0_100FD	0
#else
 #undef	 DE0_100FD
 #define DE0_100FD	MII_100FD
#endif
#ifndef	DE0_RESET
 #define DE0_RESET	0
#else
 #undef	DE0_RESET
 #define DE0_RESET	MII_RESET
#endif
#define	DE0_MEDIA	(DE0_10HD|DE0_10FD|DE0_100HD|DE0_100FD|DE0_RESET)

#ifndef	DE1_10HD
#define	DE1_10HD	0
#else
#undef	DE1_10HD
#define	DE1_10HD	MII_10HD
#endif
#ifndef	DE1_10FD
#define	DE1_10FD	0
#else
#undef	DE1_10FD
#define	DE1_10FD	MII_10FD
#endif
#ifndef	DE1_100HD
#define	DE1_100HD	0
#else
#undef	DE1_100HD
#define	DE1_100HD	MII_100HD
#endif
#ifndef	DE1_100FD
#define	DE1_100FD	0
#else
#undef	DE1_100FD
#define	DE1_100FD	MII_100FD
#endif
#ifndef	DE1_RESET
#define	DE1_RESET	0
#else
#undef	DE1_RESET
#define	DE1_RESET	MII_RESET
#endif
#define	DE1_MEDIA	(DE1_10HD|DE1_10FD|DE1_100HD|DE1_100FD|DE1_RESET)

#define KL85_DEVICE     "/dev/xconsole"
#define MOUSE_DEVICE    "/dev/mouse"
#define LOW_WATER       20
#define HIGH_WATER      30

#ifndef	BSM_CRC_FLAG
#define	BSM_CRC_FLAG		0
#endif

#ifndef	SCSI_AUTO_LUNS
#define	SCSI_AUTO_LUNS		0
#endif
#ifndef SCSI_AUTO_MBR
#define SCSI_AUTO_MBR 0
#endif
#ifndef	SCSI_AUTO_DISCONNECT
#define	SCSI_AUTO_DISCONNECT	0
#endif
#ifndef	SCSI_AUTO_SYNC
#define	SCSI_AUTO_SYNC		0
#endif
#ifndef	SCSI_AUTO_WIDE
#define	SCSI_AUTO_WIDE		0
#endif
#ifndef	SCSI_AUTO_TAGGED
#define	SCSI_AUTO_TAGGED	0
#endif

#ifndef	SCSI_MAN_DISCONNECT0
#define	SCSI_MAN_DISCONNECT0	0
#endif
#ifndef	SCSI_MAN_SYNC0
#define	SCSI_MAN_SYNC0		0
#endif
#ifndef	SCSI_MAN_WIDE0
#define	SCSI_MAN_WIDE0		0
#endif
#ifndef	SCSI_MAN_TAGGED0
#define	SCSI_MAN_TAGGED0	0
#endif
#ifndef	SCSI_MAN_MBR0
#define	SCSI_MAN_MBR0	0
#endif
#ifndef	SCSI_MAN_DISCONNECT1
#define	SCSI_MAN_DISCONNECT1	0
#endif
#ifndef	SCSI_MAN_SYNC1
#define	SCSI_MAN_SYNC1		0
#endif
#ifndef	SCSI_MAN_WIDE1
#define	SCSI_MAN_WIDE1		0
#endif
#ifndef	SCSI_MAN_TAGGED1
#define	SCSI_MAN_TAGGED1	0
#endif
#ifndef	SCSI_MAN_MBR1
#define	SCSI_MAN_MBR1	0
#endif
#ifndef	SCSI_MAN_DISCONNECT2
#define	SCSI_MAN_DISCONNECT2	0
#endif
#ifndef	SCSI_MAN_SYNC2
#define	SCSI_MAN_SYNC2		0
#endif
#ifndef	SCSI_MAN_WIDE2
#define	SCSI_MAN_WIDE2		0
#endif
#ifndef	SCSI_MAN_TAGGED2
#define	SCSI_MAN_TAGGED2	0
#endif
#ifndef	SCSI_MAN_MBR2
#define	SCSI_MAN_MBR2	0
#endif
#ifndef	SCSI_MAN_DISCONNECT3
#define	SCSI_MAN_DISCONNECT3	0
#endif
#ifndef	SCSI_MAN_SYNC3
#define	SCSI_MAN_SYNC3		0
#endif
#ifndef	SCSI_MAN_WIDE3
#define	SCSI_MAN_WIDE3		0
#endif
#ifndef	SCSI_MAN_TAGGED3
#define	SCSI_MAN_TAGGED3	0
#endif
#ifndef	SCSI_MAN_MBR3
#define	SCSI_MAN_MBR3	0
#endif
#ifndef	SCSI_MAN_DISCONNECT4
#define	SCSI_MAN_DISCONNECT4	0
#endif
#ifndef	SCSI_MAN_SYNC4
#define	SCSI_MAN_SYNC4		0
#endif
#ifndef	SCSI_MAN_WIDE4
#define	SCSI_MAN_WIDE4		0
#endif
#ifndef	SCSI_MAN_TAGGED4
#define	SCSI_MAN_TAGGED4	0
#endif
#ifndef	SCSI_MAN_MBR4
#define	SCSI_MAN_MBR4	0
#endif
#ifndef	SCSI_MAN_DISCONNECT5
#define	SCSI_MAN_DISCONNECT5	0
#endif
#ifndef	SCSI_MAN_SYNC5
#define	SCSI_MAN_SYNC5		0
#endif
#ifndef	SCSI_MAN_WIDE5
#define	SCSI_MAN_WIDE5		0
#endif
#ifndef	SCSI_MAN_TAGGED5
#define	SCSI_MAN_TAGGED5	0
#endif
#ifndef	SCSI_MAN_MBR5
#define	SCSI_MAN_MBR5	0
#endif
#ifndef	SCSI_MAN_DISCONNECT6
#define	SCSI_MAN_DISCONNECT6	0
#endif
#ifndef	SCSI_MAN_SYNC6
#define	SCSI_MAN_SYNC6		0
#endif
#ifndef	SCSI_MAN_WIDE6
#define	SCSI_MAN_WIDE6		0
#endif
#ifndef	SCSI_MAN_TAGGED6
#define	SCSI_MAN_TAGGED6	0
#endif
#ifndef	SCSI_MAN_MBR6
#define	SCSI_MAN_MBR6	0
#endif

#ifndef	PRN_CR
#define	PRN_CR			0
#endif
#ifndef	PRN_DMA
#define	PRN_DMA			0
#endif
#ifndef	PF_VERBOSE
#define	PF_VERBOSE		0
#endif
#ifndef	BTM405_LPT_SHARED
#define	BTM405_LPT_SHARED	0
#endif
#ifdef	BTM405_LPT_PCI
#undef	BTM405_LPT_PCI
#define	BTM405_LPT_PCI		C_PCI_TOUT
#else
#define	BTM405_LPT_PCI		0
#endif
#ifdef	BTM405_LPT_EPP_17
#undef	BTM405_LPT_EPP_17
#define	BTM405_LPT_EPP_17	C_EPP_17
#else
#define	BTM405_LPT_EPP_17	0
#endif
/*
#ifndef	IDE_SLOW_IO
#define	IDE_SLOW_IO	0
#endif
*/
#ifndef	IDE_RESET
#define	IDE_RESET	0
#endif

#ifndef	IDE_MBR0
#define	IDE_MBR0	0
#endif
#ifndef	IDE_MBR1
#define	IDE_MBR1	0
#endif
#ifndef	IDE_MBR2
#define	IDE_MBR2	0
#endif
#ifndef	IDE_MBR3
#define	IDE_MBR3	0
#endif

#ifndef	INCLUDE_LOGGING
  #undef	BAD_INTS_LOGGING
#endif

#ifndef RGB_INHERIT_MODE
  #define	RGB_INHERIT_MODE	0
#endif

#ifndef	UART_ERR_SHOW
  #define	UART_ERR_SHOW	0
#endif

/* cannot access flash in resident mode */

#if !defined(BT_UZPI) && !(defined(BOOTER) && defined(BT205)) && !(defined(BOOTER) && defined(BT419)) && !(defined(BOOTER) && defined(BT83RUS))
  #ifdef	FLASH_RESIDENT
    #undef	INCLUDE_FTL
    #undef	INCLUDE_FLASH
    #undef	INCLUDE_FLASH_OLD
    #undef	FLASH_NVRAM_SECTOR
    #ifndef	INCLUDE_PRIME_NVRAM
      #undef	INCLUDE_NVRAM
    #endif	/* !INCLUDE_PRIME_NVRAM */
  #endif	/* FLASH_RESIDENT */
#endif	/* !BT_UZPI */

/* FTL configure */

#ifdef	INCLUDE_FTL
  #undef	INCLUDE_FLASH
  #define	INCLUDE_FLASH
#endif	/* INCLUDE_FTL */

#define INCLUDE_VMESHOW		/* vmeShow() routine */

#ifndef	INCLUDE_VME
  #undef	INCLUDE_VMESHOW
  #undef	INCLUDE_MP_OBJS
  #undef	MP_NETWORK
  #undef        INCLUDE_MULTIPROCESSING
  #define       VME_MEM_TOTAL_SIZE     0
#endif	/* INCLUDE_VME */

#ifndef MP_NETWORK
  #undef	VME_MEM_NET_SIZE
  #define	VME_MEM_NET_SIZE	0
#endif

#if !defined(INCLUDE_MP_OBJS) && !defined(INCLUDE_CHANNELS) && !defined(VME_MODULES)  /* OS2000 and OS3000 */
  #undef	VME_MEM_OBJECTS_SIZE
  #define	VME_MEM_OBJECTS_SIZE	0
#endif

#if defined(INCLUDE_CHANNELS)
  #undef	VME_MEM_OBJECTS_SIZE
  #define	VME_MEM_OBJECTS_SIZE	VME_MEM_CHANNELS_SIZE
#endif

#if CPU==R3000 //-------------------------------
  #ifdef CACHE_SIZE
  #if	CACHE_SIZE == I8D8
    #define	DCACHE_SIZE		0x2000		/* 8KB */
    #define	ICACHE_SIZE		0x2000		/* 8KB */
  #endif
  #if	CACHE_SIZE == I16D4
    #define	DCACHE_SIZE		0x1000		/* 4KB */
    #define	ICACHE_SIZE		0x4000		/* 16KB */
  #endif
  #else
    #define	DCACHE_SIZE		0x1000		/* 4KB */
    #define	ICACHE_SIZE		0x1000		/* 4KB */
  #endif

  #if	BOARD_BUS_REQ == -1
    #undef	BOARD_BUS_REQ
  #endif
#endif        //-------------------------------

#ifndef	INCLUDE_NVRAM
  #undef	FLASH_NVRAM_SECTOR
  #undef	INCLUDE_PRIME_NVRAM
#endif	/* INCLUDE_NVRAM */

#ifndef INCLUDE_FLASH
  #undef	FLASH_NVRAM_SECTOR
#endif	/* INCLUDE_FLASH */


#ifndef BT202 //-----------------------------------
#ifdef	INCLUDE_VME
  #define INCLUDE_SM
  /* BSM Ethernet on VME Configuration */

  #if   MODULE_VME_STEP == 0x400000
    #define	VME_MEM_TOTAL_SIZE	0x400000
    #define BSM_MAX_NODE		4
  #elif MODULE_VME_STEP == 0x100000 || MODULE_VME_STEP == 0x200000
    #define	VME_MEM_TOTAL_SIZE	0x100000
    #define BSM_MAX_NODE		8
  #else
    #error invalid MODULE_VME_STEP
  #endif

/*
  #if defined(BT419) && defined(BOOTER)
#warning AAAAAAAAAAAAAAAAA
      #undef VME_MEM_TOTAL_SIZE
      #undef VME_MEM_FIRMWARE_SIZE
      #undef VME_MEM_RESERVED_SIZE
      #undef VME_MEM_OBJECTS_SIZE
      #undef VME_MEM_NET_SIZE

      #define VME_MEM_TOTAL_SIZE      0
      #define VME_MEM_FIRMWARE_SIZE   0
      #define VME_MEM_RESERVED_SIZE   0
      #define VME_MEM_OBJECTS_SIZE    0
      #define VME_MEM_NET_SIZE        0
  #endif
*/

  #if (VME_MEM_FIRMWARE_SIZE + VME_MEM_RESERVED_SIZE + VME_MEM_OBJECTS_SIZE + VME_MEM_NET_SIZE) > VME_MEM_TOTAL_SIZE
    #error INVALID VME AREA SIZES
  #endif

  #define BSM_OFFSET		(VME_MEM_TOTAL_SIZE - BSM_MEM_SIZE)
  #define BSM_REGION_SIZE		VME_MEM_TOTAL_SIZE
  #define BSM_MEM_SIZE		VME_MEM_NET_SIZE
  #define BSM_MTU			1514
  #define	BSM_RCV_FIFO_SIZE	64

#endif        /* INCLUDE_VME */
#endif         // ifndef BT202 -----------------------------

//------------------------------------------------------
#ifdef _1B813
#if ((1<<PRI_SW1)|(1<<PRI_SW2)|(1<<PRI_WATCHDOG)|(1<<PRI_FPA)|(1<<PRI_ASIC0)|(1<<PRI_ASIC1)|(1<<PRI_ASIC2)|(1<<PRI_ASIC3)) != 0xff
#error	IRQ PRIORITY MUST BE DIFFERENT
#endif

#ifndef ENABLE_SW1
#define ENABLE_SW1	0
#endif
#ifndef ENABLE_SW2
#define ENABLE_SW2	0
#endif
#ifndef ENABLE_WATCHDOG
#define ENABLE_WATCHDOG	0
#endif
#ifndef ENABLE_FPA
#define ENABLE_FPA	0
#endif
#ifndef ENABLE_ASIC0
#define ENABLE_ASIC0	0
#endif
#ifndef ENABLE_ASIC1
#define ENABLE_ASIC1	0
#endif
#ifndef ENABLE_ASIC2
#define ENABLE_ASIC2	0
#endif
#ifndef ENABLE_ASIC3
#define ENABLE_ASIC3	0
#endif

#define TASK_SR_DEFAULT		(0x70000001|(ENABLE_SW1<<8)|(ENABLE_SW2<<9)|(ENABLE_WATCHDOG<<10)|(ENABLE_ASIC0<<11)|(ENABLE_ASIC1<<12)|(ENABLE_FPA<<13)|(ENABLE_ASIC2<<14)|(ENABLE_ASIC3<<15))

#define	IV_BASE(pin)	(IV_BASE_VECTOR0 + (pin)*0x10)

#define IV_UART_3	(IV_BASE(PIN_UART_3) | IV_MINOR_Uart3)
#define IV_UART_2	(IV_BASE(PIN_UART_2) | IV_MINOR_Uart2)
#define IV_UART_1	(IV_BASE(PIN_UART_1) | IV_MINOR_Uart1)
#define IV_EXT_INT_4	(IV_BASE(PIN_EXT_INT_4) | IV_MINOR_ExtInt4)
#define IV_EXT_INT_3	(IV_BASE(PIN_EXT_INT_3) | IV_MINOR_ExtInt3)
#define IV_EXT_INT_2	(IV_BASE(PIN_EXT_INT_2) | IV_MINOR_ExtInt2)
#define IV_EXT_INT_1	(IV_BASE(PIN_EXT_INT_1) | IV_MINOR_ExtInt1)
#define IV_EXT_INT_0	(IV_BASE(PIN_EXT_INT_0) | IV_MINOR_ExtInt0)
#define IV_TIMER_3	(IV_BASE(PIN_TIMER_3) | IV_MINOR_Timer3)
#define IV_TIMER_2	(IV_BASE(PIN_TIMER_2) | IV_MINOR_Timer2)
#define IV_TIMER_1	(IV_BASE(PIN_TIMER_1) | IV_MINOR_Timer1)
#define IV_PCI		(IV_BASE(PIN_PCI) | IV_MINOR_PCI)
#define IV_DRAM_UE	(IV_BASE(PIN_DRAM_UE) | IV_MINOR_DramUE)
#define IV_ERROR	(IV_BASE(PIN_ERROR) | IV_MINOR_WriteErr)
#endif        /* ifdef _1B813 */

//----------------------------------------------------
#ifdef BOOTER_INHERITANCE
  #define INCLUDE_BOOTLINE
  #ifndef INCLUDE_SLIP
   #define INCLUDE_SLIP 1
  #endif

  #ifdef INCLUDE_SLIP0
   #undef INCLUDE_SLIP0
  #endif
  #ifdef INCLUDE_SLIP1
    #undef INCLUDE_SLIP1
  #endif
  #ifdef INCLUDE_SLIP2
    #undef INCLUDE_SLIP2
  #endif
  #ifdef INCLUDE_SLIP3
    #undef INCLUDE_SLIP3
  #endif
  #ifdef INCLUDE_PPP
   #undef INCLUDE_PPP
  #endif
  #ifdef CONFIG_NETADDRESSES
    #undef CONFIG_NETADDRESSES
  #endif
  #ifdef INCLUDE_DEFAULT_GATEWAY
    #undef INCLUDE_DEFAULT_GATEWAY
  #endif
  #ifdef INCLUDE_NETGW
    #undef INCLUDE_NETGW
  #endif
  #ifdef INCLUDE_NETGWA0
    #undef INCLUDE_NETGWA0
  #endif
  #ifdef INCLUDE_NETGWA1
    #undef INCLUDE_NETGWA1
  #endif
  #ifdef INCLUDE_NETGWA2
    #undef INCLUDE_NETGWA2
  #endif
  #ifdef INCLUDE_NETGWA3
    #undef INCLUDE_NETGWA3
  #endif
  #ifdef INCLUDE_NETGWA4
    #undef INCLUDE_NETGWA4
  #endif
  #ifdef INCLUDE_NETGWA5
    #undef INCLUDE_NETGWA5
  #endif
  #ifdef INCLUDE_SNETGW
    #undef INCLUDE_SNETGW
  #endif
  #ifdef INCLUDE_SNETGWA0
    #undef INCLUDE_SNETGWA0
  #endif
  #ifdef INCLUDE_SNETGWA1
    #undef INCLUDE_SNETGWA1
  #endif
  #ifdef INCLUDE_SNETGWA2
    #undef INCLUDE_SNETGWA2
  #endif
  #ifdef INCLUDE_SNETGWA3
    #undef INCLUDE_SNETGWA3
  #endif
  #ifdef INCLUDE_SNETGWA4
    #undef INCLUDE_SNETGWA4
  #endif
  #ifdef INCLUDE_SNETGWA5
    #undef INCLUDE_SNETGWA5
  #endif

  #ifdef        MOUNT_FTP
    #undef MOUNT_FTP
  #endif
  #ifdef	MOUNT_FTP0
    #undef MOUNT_FTP0
  #endif
  #ifdef	MOUNT_FTP1
    #undef MOUNT_FTP1
  #endif
  #ifdef	MOUNT_FTP2
    #undef MOUNT_FTP2
  #endif
  #ifdef	MOUNT_FTP3
    #undef MOUNT_FTP3
  #endif
  #ifdef MOUNT_NFS_AUTO
    #undef MOUNT_NFS_AUTO
  #endif
  #ifdef	MOUNT_NFS0
    #undef MOUNT_NFS0
  #endif
  #ifdef	MOUNT_NFS1
    #undef MOUNT_NFS1
  #endif
  #ifdef	MOUNT_NFS2
    #undef MOUNT_NFS2
  #endif
  #ifdef	MOUNT_NFS3
    #undef MOUNT_NFS3
  #endif
#endif    /* ifdef BOOTER_INHERITANCE */

#if 0  // (defined(INCLUDE_BTM401) || defined(INCLUDE_BT401A) || defined(INCLUDE_BT83401)) && !defined(INCLUDE_POSIX_CND)
   #error !!!!! MKIO requires INCLUDE_POSIX_CND  !!!!!
#endif

#endif	/* BOARD_CONFIG */

