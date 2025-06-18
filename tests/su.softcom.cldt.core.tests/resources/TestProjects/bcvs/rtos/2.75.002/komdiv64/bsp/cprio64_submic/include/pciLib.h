/* pciLib.h - header for PCI library */

#ifndef __INCpciLibh
#define __INCpciLibh

#ifdef __ASSEMBLER__

#define CF_LE_W(x)	((((x) & 0x00ff) << 8) | (((x) >> 8) & 0x00ff))

#define CF_LE_L(x)	((((x) << 24) | (x) >> 24) | \
			(((x) & 0x0000ff00) << 8)  | \
			(((x) >> 8) & 0x0000ff00)) 

#define	CT_LE_W		CF_LE_W
#define	CT_LE_L		CF_LE_L

#else	/* __ASSEMBLER__ */

#include <board.h>

/* little endian to big endian convertation */

#define CF_LE_W(x)	((((unsigned short)(x) & 0x00ffU) << 8) |	 \
			 (((unsigned short)(x) >> 8) & 0x00ffU))

#define CF_LE_L(x)	(((unsigned)(x) << 24) | ((unsigned)(x) >> 24) | \
			 (((unsigned)(x) & 0x0000ff00U) << 8) | 	 \
			 (((unsigned)(x) >> 8) & 0x0000ff00U)) 

#define	CT_LE_W		CF_LE_W
#define	CT_LE_L		CF_LE_L

/* Device classes and subclasses */

#define PCI_CLASS_NOT_DEFINED			0x0000
#define PCI_CLASS_NOT_DEFINED_VGA		0x0001

#define PCI_BASE_CLASS_STORAGE			0x01
#define PCI_CLASS_STORAGE_SCSI			0x0100
#define PCI_CLASS_STORAGE_IDE			0x0101
#define PCI_CLASS_STORAGE_FLOPPY		0x0102
#define PCI_CLASS_STORAGE_IPI			0x0103
#define PCI_CLASS_STORAGE_RAID			0x0104
#define PCI_CLASS_STORAGE_OTHER			0x0180

#define PCI_BASE_CLASS_NETWORK			0x02
#define PCI_CLASS_NETWORK_ETHERNET		0x0200
#define PCI_CLASS_NETWORK_TOKEN_RING		0x0201
#define PCI_CLASS_NETWORK_FDDI			0x0202
#define PCI_CLASS_NETWORK_ATM			0x0203
#define PCI_CLASS_NETWORK_OTHER			0x0280

#define PCI_BASE_CLASS_DISPLAY			0x03
#define PCI_CLASS_DISPLAY_VGA			0x0300
#define PCI_CLASS_DISPLAY_XGA			0x0301
#define PCI_CLASS_DISPLAY_OTHER			0x0380

#define PCI_BASE_CLASS_MULTIMEDIA		0x04
#define PCI_CLASS_MULTIMEDIA_VIDEO		0x0400
#define PCI_CLASS_MULTIMEDIA_AUDIO		0x0401
#define PCI_CLASS_MULTIMEDIA_OTHER		0x0480

#define PCI_BASE_CLASS_MEMORY			0x05
#define PCI_CLASS_MEMORY_RAM			0x0500
#define PCI_CLASS_MEMORY_FLASH			0x0501
#define PCI_CLASS_MEMORY_OTHER			0x0580

#define PCI_BASE_CLASS_BRIDGE			0x06
#define PCI_CLASS_BRIDGE_HOST			0x0600
#define PCI_CLASS_BRIDGE_ISA			0x0601
#define PCI_CLASS_BRIDGE_EISA			0x0602
#define PCI_CLASS_BRIDGE_MC			0x0603
#define PCI_CLASS_BRIDGE_PCI			0x0604
#define PCI_CLASS_BRIDGE_PCMCIA			0x0605
#define PCI_CLASS_BRIDGE_NUBUS			0x0606
#define PCI_CLASS_BRIDGE_CARDBUS		0x0607
#define PCI_CLASS_BRIDGE_OTHER			0x0680

#define PCI_BASE_CLASS_COMMUNICATION		0x07
#define PCI_CLASS_COMMUNICATION_SERIAL		0x0700
#define PCI_CLASS_COMMUNICATION_PARALLEL	0x0701
#define PCI_CLASS_COMMUNICATION_OTHER		0x0780

#define PCI_BASE_CLASS_SYSTEM			0x08
#define PCI_CLASS_SYSTEM_PIC			0x0800
#define PCI_CLASS_SYSTEM_DMA			0x0801
#define PCI_CLASS_SYSTEM_TIMER			0x0802
#define PCI_CLASS_SYSTEM_RTC			0x0803
#define PCI_CLASS_SYSTEM_OTHER			0x0880

#define PCI_BASE_CLASS_INPUT			0x09
#define PCI_CLASS_INPUT_KEYBOARD		0x0900
#define PCI_CLASS_INPUT_PEN			0x0901
#define PCI_CLASS_INPUT_MOUSE			0x0902
#define PCI_CLASS_INPUT_OTHER			0x0980

#define PCI_BASE_CLASS_DOCKING			0x0a
#define PCI_CLASS_DOCKING_GENERIC		0x0a00
#define PCI_CLASS_DOCKING_OTHER			0x0a01

#define PCI_BASE_CLASS_PROCESSOR		0x0b
#define PCI_CLASS_PROCESSOR_386			0x0b00
#define PCI_CLASS_PROCESSOR_486			0x0b01
#define PCI_CLASS_PROCESSOR_PENTIUM		0x0b02
#define PCI_CLASS_PROCESSOR_ALPHA		0x0b10
#define PCI_CLASS_PROCESSOR_POWERPC		0x0b20
#define PCI_CLASS_PROCESSOR_CO			0x0b40

#define PCI_BASE_CLASS_SERIAL			0x0c
#define PCI_CLASS_SERIAL_FIREWIRE		0x0c00
#define PCI_CLASS_SERIAL_ACCESS			0x0c01
#define PCI_CLASS_SERIAL_SSA			0x0c02
#define PCI_CLASS_SERIAL_USB			0x0c03
#define PCI_CLASS_SERIAL_FIBER			0x0c04

#define PCI_CLASS_OTHERS			0xff


/* pci space descriptor */
#if OS_MAJOR > 3
  #define LONG_INT long
#else
  #define LONG_INT int
#endif
typedef struct {
    LONG_INT memStart;	/* ptr to current free pci memory space */
    LONG_INT memBound;	/* ptr to pci memory space  upper bound */
    LONG_INT ioStart;	/* ptr to current free pci I/O space */
    LONG_INT ioBound;	/* ptr to pci I/O space  upper bound */
} PCI_SPACE;

/* argument for pciScan() */
typedef struct pci_dev {
    long			cookie;	/* befor first scan should be (-1) */
    unsigned		id;	/* device and vendor id's */
    unsigned char	bus;	/* bus number */
    unsigned char	dev;	/* device number */
    unsigned char	fun;	/* function number */
} PCI_DEV;

/* function declarations */

void* pciBusMemAlloc(unsigned bus, unsigned nBytes);
void* pciBusIoAlloc(unsigned bus, unsigned nBytes);
int pciIntNum(unsigned bus, unsigned dev, unsigned fun);
STATUS pciIntEnable(unsigned pciLine);
STATUS pciIntDisable(unsigned pciLine);
STATUS pciIntConnect(unsigned pciLine, VOIDFUNCPTR handler, int arg);
STATUS pciIntDisconnect(unsigned pciLine, VOIDFUNCPTR handler, int arg);
void pciConfigOutLong(int bus, int dev, int fun, int reg, int value);
unsigned pciConfigInLong(int bus, int dev, int fun, int reg);
STATUS pciScan(PCI_DEV* d);
STATUS pciDeviceFind(unsigned id, int ix, int* pBus, int* pDev, int* pFun);
unsigned pciLocalToBus(void* adrs);
void* pciBusToLocal(unsigned adrs);
unsigned pciLocalToMem(unsigned adrs);
unsigned pciLocalToIO(unsigned adrs);
unsigned pciMemToLocal(unsigned adrs);
unsigned pciIOToLocal(unsigned adrs);
void pciDevs(int detail);
const char* pciClassNameGet(unsigned int class);
void pciDevsDisable(int skip_bridge);
void pciBusGetSpace(int bus, PCI_SPACE* space);
void pciBusSetSpace(int bus, PCI_SPACE* space);
int pciDevIxGet(unsigned bus, unsigned dev, unsigned fun,
    unsigned* id, unsigned class);
int pciBusBigMemAlloc(unsigned bus, unsigned nBytes, uint64_t* cpuBase, uint32_t* pciBase);

#endif	/* __ASSEMBLER__ */

#endif	/* __INCpciLibh */
