#ifndef _LIB401A_H
#define _LIB401A_H

/* Functions prototype */
int bt401InitModule(MPI_MODULE*,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int);

/* BT23-40A external registers defenitions */

/* Channels number */
#define BT23401A_CHN            4
#define BT83401_CHN             2
#define BTM401_CHN              2

/* VME controller registers offsets */
#define  REG_RAM_BASE           0x80
#define  REG_LEVEL_VECTOR_0     0x82
#define  REG_LEVEL_VECTOR_1     0x84
#define  REG_LEVEL_VECTOR_2     0x86
#define  REG_LEVEL_VECTOR_3     0x88

/* Registers default address A16 (0x100 bytes )*/
#define  REG_BASE_DEFAULT       0x8D00

/* RAM base register definitions (0x2000 bytes) */
#define  RAM_BASE_DEFAULT       0x860000
#define  RAM_BASE_SHIFT         13
#define  RAM_BASE_MASK          0x7FF

/* Int level & vector register defenitions */
#define  LEVEL_DEFAULT          3
#define  VECTOR_DEFAULT         0x60
#define  LEVEL_SHIFT            8



/* Useful defenitions */
/* Channels names */
#define CH0                      0
#define CH1                      1
#define CH2                      2
#define CH3                      3

/* PCI controller 32-bit registers indexes */
#define  DEV_VENDOR             0x00
#define  STATUS_COMMAND         0x01
#define  CLASS_REVISION         0x02
#define  BIST                   0x03
#define  BASE_ADDRESS           0x04
#define  INT_LINE               0x0F
#define  USER                   0x10

/* PCI controller registers bits */
#define  DEV_VENDOR_CODE        0x00021234
#define  DEV_VENDOR_CODE1       0x00081234
#define  CLASS_CODE             0xFF000001

#define  COM_MEM_ENABLE         0x00000002
#define  COM_PERR_ENABLE        0x00000040
#define  COM_SERR_ENABLE        0x00000100

#define  STAT_PERR              0x00008000
#define  STAT_SERR              0x00004000
#define  STAT_TARGET_ABORT      0x00000800

#define  INT_A                  0x00000000
#define  INT_B                  0x00000001
#define  INT_C                  0x00000002
#define  INT_D                  0x00000003

#define  USER_INT_QBUS          0x00000001
#define  USER_INTA              0x00000002
#define  USER_INTB              0x00000004

/* Subdevices offsets (bytes) */
#define  MEM_A                  0x00000000
#define  IO_A                   0x00020000
#define  MEM_B                  0x00040000
#define  IO_B                   0x00060000
#define  MEM_QBUS               0x00080000
#define  INT_QBUS               0x000C0000

#endif	/* _LIB401A_H */
