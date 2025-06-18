/* pciLib.c - K64SMP pci bus support library */

/*
   28feb12,isok -BT83M support added
   23jan12,sok - use skip_bridge param in pciDevsDisable().
   18jul11,sok - added K64RIO in pciIntNum.
   18mar11,laz - added BT55-211 in pciIntNum.
*/

#include <try.h>
#include <pciLib.h>

#if OS_VERSION >= 300
#include <private/longcopy.h>
#endif


/* structure to store pci interrupt handler description */

typedef struct pci_action {
    struct pci_action*	next;		/* single link list  */
    VOIDFUNCPTR		handler;	/* interrupt handler */
    int			arg;		/* handler argument  */
} PCI_ACTION;

/* pci device list node */

typedef struct pci_node {
    struct pci_node*	next;	/* next in list */
    unsigned		id;	/* pci device and vendor id */
    unsigned		bus;	/* bus number */
    unsigned		dev;	/* device number */
    unsigned		fun;	/* function number */
} PCI_NODE;

/* locals */

static PCI_SPACE pciSpace[1];		/* pci space for bus 0 */
static PCI_NODE* pciDevList;		/* list of pci devices */
static PCI_ACTION* pciIntLists[4];	/* lists of PCI_ACTION */
static VOIDFUNCPTR pciUserRtn;		/* user error handler routine */
static int pciUserArg;			/* user error handler argument */
#ifdef BT83M
int pciHostDev = 5;			/* host bridge device number */
#else
int pciHostDev = 0;			/* host bridge device number */
#endif
int pciBigMem = FALSE;			/* big memory support*/

/*
  We look upper 2GB PCI memory space (from 0x80000000 on the bus)
  at 64-bit CPU physical address 0xd00000000. But we also have old
  160MB window (from 0x80000000 on the bus) at 64-bit CPU physical
  address 0x010000000.
*/

#define	PCI_BIG_MEM_SIZE	0x80000000
#define	PCI_BIG_MEM_CPU_BASE	0xd00000000ULL
#define	PCI_BIG_MEM_BUS_BASE	0x80000000

static uint32_t pciBigMemOffset = 0x10000000;	/* skip 256MB old window space */

/*******************************************************************************
*
* alignAlloc - allocate aligned nBytes space from pool
*    started at *start and ended at bound.
*
* RETURNS: ptr to allocated space and modified *start, 
* or NULL if space intersect bound
*
*/

static inline void* alignAlloc(unsigned nBytes, unsigned* start, unsigned bound)
{
    unsigned result, end, module = *start % nBytes;
    
    result = module ? (*start + nBytes - module) : *start;

    end = (unsigned)result + nBytes;
    
    if(end > bound)
	return NULL;
    *start = end;

    return (void*)result;
}

/*******************************************************************************
*
* pciBusBigMemAlloc - allocate aligned nBytes space from pci memory space
*
* Allocate aligned nBytes space from big pci memory space
*
* RETURNS: 0 on success or -1 on error
*
*/

int pciBusBigMemAlloc(unsigned bus, unsigned nBytes, uint64_t* cpuBase, uint32_t* pciBase)
{
    uint32_t offset;

    if(bus > 0 || !pciBigMem || !cpuBase || !pciBase)
	return -1;

    if(!(offset = (uint32_t)alignAlloc(nBytes,&pciBigMemOffset,PCI_BIG_MEM_SIZE)))
	return -1;

    *cpuBase = PCI_BIG_MEM_CPU_BASE + offset;
    *pciBase = PCI_BIG_MEM_BUS_BASE + offset;
    return 0;
}

/*******************************************************************************
*
* pciBusMemAlloc - allocate aligned nBytes space from pci memory space
*
* Allocate aligned nBytes space from pci memory space
*
* RETURNS: ptr to allocated space, or NULL if space intersect pciMemBound
*
*/

void* pciBusMemAlloc(unsigned bus, unsigned nBytes)
{
    if(bus > 0)
	return NULL;
    return alignAlloc(nBytes,&pciSpace[bus].memStart,pciSpace[bus].memBound);
}

/*******************************************************************************
*
* pciBusIoAlloc - allocate aligned nBytes space from pci I/O space
*
* Allocate aligned nBytes space from pci I/O space
*
* RETURNS: ptr to allocated space, or NULL if space intersect pciIoBound
*
*/

void* pciBusIoAlloc(unsigned bus, unsigned nBytes)
{
    if(bus > 0)
	return NULL;
    return alignAlloc(nBytes,&pciSpace[bus].ioStart,pciSpace[bus].ioBound);
}

/*******************************************************************************
*
* pciLocalToBus - convert local memory address to pci bus address.
*
* RETURNS: pci bus address.
*
*/

unsigned pciLocalToBus(void* adrs)
{
#if OS_VERSION < 300
    return LOCAL_TO_PCI(adrs);
#else
    paddr_t pa;
    MODCALL(pa = cpuV2P(getpid(),adrs));
    return pa;
#endif
}

/*******************************************************************************
*
* pciBusToLocal - convert pci bus address to local memory KSEG1 address.
*
* RETURNS: local memory KSEG1 address.
*
*/

void* pciBusToLocal(unsigned adrs)
{
#if OS_VERSION < 300
    return (void*)PCI_TO_LOCAL(adrs);
#else
#if defined(INCLUDE_VME) && defined(MP_NETWORK) && MP_NETWORK_TYPE==MP_NETWORK_BUSNET && BSM_DRIVER_TYPE==OCPB_LINUX
    if(!adrs)
	return NULL;
#endif

    if(adrs < BOARD_UNCACHED_MEM_PHYS_BASE || adrs >= BOARD_UNCACHED_MEM_PHYS_BASE + BOARD_UNCACHED_MEM_SIZE)
    {
	kprint("%s: cannot map address=%p\n\r",__FUNCTION__,adrs);
	return (void*)(-1);
    }
    
    return (void*)(BOARD_UNCACHED_MEM_BASE + adrs - BOARD_UNCACHED_MEM_PHYS_BASE);
#endif
}

/*******************************************************************************
*
* pciLocalToMem - convert local pci memory address to pci bus memory address.
*
* RETURNS: pci bus memory address.
*
*/

unsigned pciLocalToMem(unsigned adrs)
{
    return LOCAL_TO_PCI_MEM(adrs);
}

/*******************************************************************************
*
* pciMemToLocal - convert pci bus memory address to local pci memory address.
*
* RETURNS: local pci memory address.
*
*/

unsigned pciMemToLocal(unsigned adrs)
{
    return PCI_MEM_TO_LOCAL(adrs);
}

/*******************************************************************************
*
* pciLocalToIO - convert local pci I/O address to pci bus I/O address.
*
* RETURNS: pci bus I/O address.
*
*/

unsigned pciLocalToIO(unsigned adrs)
{
    return LOCAL_TO_PCI_IO(adrs);
}

/*******************************************************************************
*
* pciIOToLocal - convert pci bus I/O address to local pci I/O address.
*
* RETURNS: local pci I/O address.
*
*/

unsigned pciIOToLocal(unsigned adrs)
{
    return PCI_IO_TO_LOCAL(adrs);
}

/******************************************************************************/

/* get address of configuration register */

static inline volatile unsigned* pciConfigReg(int bus, int dev, int fun,
    int reg)
{
    return (void*)(PCI_CONF | (dev << 11) | (fun << 8) | reg);
}

/*******************************************************************************
*
* pciConfigOutLong - write 32-bit value to pci configuration space
*
* NOTE: Value must be in cpu byte order.
*
* RETURNS: N/A
*
*/

void po(int bus, int dev, int fun, int reg, int val)
{
    pciConfigOutLong(bus,dev,fun,reg,val);
}

void pciConfigOutLong(int bus, int dev, int fun, int reg, int value)
{ 
    if(bus > 0)
	return;

    *pciConfigReg(bus,dev,fun,reg) = CT_LE_L(value);
}

/*******************************************************************************
*
* pciConfigInLong - read 32-bit value from pci configuration space
*
* RETURNS: converted to cpu byte order value.
*
*/

unsigned pi(int bus, int dev, int fun, int reg)
{
    return pciConfigInLong(bus,dev,fun,reg);
}

unsigned pciConfigInLong(int bus, int dev, int fun, int reg)
{ 
    unsigned value;

    if(bus > 0)
	return 0xffffffff;

    value = *pciConfigReg(bus,dev,fun,reg);
    return CT_LE_L(value);
}

/******************************************************************************/

/* error interrupt handler */

void pciErrInt(int unused)
{
    unsigned stat = pciConfigInLong(0,pciHostDev,0,0x7c);
    pciConfigOutLong(0,pciHostDev,0,0x7c,stat);
    
    BAD_LOG("%s: stat=%08x %s%s%s%s\n",(int)__FUNCTION__,stat,
	(int)((stat & 0x800) ? "WRITE_DEV_TO_CTRL " : ""),
	(int)((stat & 0x400) ? "WRITE_CTRL_TO_DEV " : ""),
	(int)((stat & 0x200) ? "PERR " : ""),
	(int)((stat & 0x100) ? "SERR " : ""));

    if(pciUserRtn)
	pciUserRtn(pciUserArg,stat);
}

/*******************************************************************************
*
* pciInt - interrupt handler for shared PCI interrupt.
*
* This routine executes multiple interrupt handlers for a PCI interrupt.
* Each interrupt handler must check the device dependent interrupt status bit
* to determine the source of the interrupt and must return TRUE if it his irq, 
* else return FALSE. 
*
* RETURNS: N/A
*
*/

static void pciInt
    (
    int demux		/* demultiplexor index */
    )
{
    register PCI_ACTION *act;

    for(act = pciIntLists[demux]; act; act = act->next)
	act->handler(act->arg);
}

/*******************************************************************************
*
* pciIntConnect - connect the interrupt handler to the PCI interrupt line(A - D).
*
* RETURNS:
* 0, or -1 if the interrupt handler cannot be built.
*
*/

int pciIntConnect
    (
    unsigned	iv,		/* line to attach to */
    VOIDFUNCPTR	handler,	/* routine to be called */
    int		arg		/* parameter to be passed to routine */
    )
{
    PCI_ACTION* act; int lock, demux;

    switch(iv)
    {
	case IV_PCI_INTA: demux = 0; break;
	case IV_PCI_INTB: demux = 1; break;
	case IV_PCI_INTC: demux = 2; break;
	case IV_PCI_INTD: demux = 3; break;
	default: return -1;
    }

    if(!(act = (PCI_ACTION*)malloc(sizeof(PCI_ACTION))))
	return -1;

    act->handler = handler;
    act->arg	 = arg;

    lock = intLock();

    /* insert new handler in head of list */

    act->next = pciIntLists[demux];
    pciIntLists[demux] = act;

    intUnlock(lock);

    return 0;
}

/*******************************************************************************
*
* pciIntDisconnect - disconnect the interrupt handler from the PCI interrupt.
*
* RETURNS:
* 0, or -1 if the interrupt handler cannot be removed.
*
*/

int pciIntDisconnect
    (
    unsigned	iv,		/* line to detach to */
    VOIDFUNCPTR	handler,	/* routine to be called */
    int		arg		/* parameter to be passed to routine */
    )
{
    PCI_ACTION* act;
    PCI_ACTION* old;
    int lock, demux;

    switch(iv)
    {
	case IV_PCI_INTA: demux = 0; break;
	case IV_PCI_INTB: demux = 1; break;
	case IV_PCI_INTC: demux = 2; break;
	case IV_PCI_INTD: demux = 3; break;
	default: return -1;
    }

    for(act = pciIntLists[demux], old = NULL; act && act->handler != handler;
        old = act, act = act->next);

    if(!act)
	return -1;

    lock = intLock();

    if(old)
	old->next = act->next;
    else
	pciIntLists[demux] = act->next;

    intUnlock(lock);

    free((void*)act); return 0;
}

/*******************************************************************************
*
* pciIntEnable - enable pci interrupts for specified interrupt vector
*
* RETURNS: -1 if line is invalid, else 0
*
*/

int pciIntEnable(unsigned iv)
{
    return boardIvEnable(iv);
}

/*******************************************************************************
*
* pciIntDisable - disable pci interrupts for specified vector
*
* RETURNS: -1 if line is invalid, else 0
*
*/

int pciIntDisable(unsigned iv)
{
    return boardIvDisable(iv);
}

/*******************************************************************************
*
* pciIntNum - get pci interrupt vector of specified device
*
* RETURNS: interrupt line number or -1.
*
*/

int pciIntNum(unsigned bus, unsigned dev, unsigned fun)
{
    if(bus > 0)
	return -1;

#if defined(K64RIO)
    switch(dev)
    {
	case 2: return IV_PCI_INTC;		/* PMC2 on non-standard board */
	case 4: return IV_PCI_INTC;		/* PMC2 */
	case 3: return IV_PCI_INTB;		/* PMC1 */
    }
#elif defined(MPON)
    switch(dev)
    {
	case 1:			/* RIO */
	    return IV_PCI_INTB;
	case 2:			/* PMC1 */
	case 3:			/* PMC2 */
	    return IV_PCI_INTA;
    }
#elif defined(BT55211)
    switch(dev)
    {
	case 4: return IV_PCI_INTB;		/* PMC2 */
	case 3: return IV_PCI_INTA;		/* PMC1 */
	case 1: return IV_PCI_INTD;		/* PCI-RAM */
	case 2: return IV_PCI_INTC;		/* VME */
    }
#elif defined(BT83M)
    switch(dev)
    {
	case 6: return PCI_DEV1_IRQ;
	case 7: return PCI_DEV2_IRQ;
	case 8: return PCI_DEV3_IRQ;
	case 9: return PCI_DEV4_IRQ;
    }
#else
    if(pciHostDev == 0)
    {
	switch(dev)
	{
	    case 4: return IV_PCI_INTA;		/* PMC */
	    case 3: return IV_PCI_INTA;		/* VGA */
	    case 1: return IV_PCI_INTB;		/* MIO */
	    case 2: return IV_PCI_INTC;		/* VME */
	}
    }
    else
    {
	switch(dev)
	{
	    case 18: return IV_PCI_INTA;
	    case 19: return IV_PCI_INTC;
	}
    }
#endif
    return -1;
}


/******************************************************************************/

/* find index device with specified pci device id */

int pciDeviceFind(unsigned id, int ix, int* pBus, int* pDev, int* pFun)
{
    PCI_DEV d = { cookie : -1 };

    while(pciScan(&d) != -1)
    {
	if(d.id == id && ix-- == 0)
	{
	    *pBus = d.bus;
	    *pDev = d.dev;
	    *pFun = d.fun;
	    return 0;
	}
    }
    return -1;
}

/******************************************************************************/

/* scan pci devices */

int pciScan(PCI_DEV* d)
{
    PCI_NODE* pciNode = (d->cookie == -1) ? pciDevList :
	((PCI_NODE*)d->cookie)->next;
    
    if(!pciNode)
	return -1;

    d->id     = pciNode->id;
    d->bus    = pciNode->bus;
    d->dev    = pciNode->dev;
    d->fun    = pciNode->fun;
    d->cookie = (int)pciNode;

    return 0;
}

/******************************************************************************/

/* get class name by configuration register 0x08 */

const char* pciClassNameGet(unsigned int class)
{
    switch (class >> 8) {
	case PCI_CLASS_NOT_DEFINED:		return "Non-VGA device";
	case PCI_CLASS_NOT_DEFINED_VGA:		return "VGA compatible device";

	case PCI_CLASS_STORAGE_SCSI:		return "SCSI storage controller";
	case PCI_CLASS_STORAGE_IDE:		return "IDE interface";
	case PCI_CLASS_STORAGE_FLOPPY:		return "Floppy disk controller";
	case PCI_CLASS_STORAGE_IPI:		return "IPI storage controller";
	case PCI_CLASS_STORAGE_RAID:		return "RAID storage controller";
	case PCI_CLASS_STORAGE_OTHER:		return "Unknown mass storage controller";

	case PCI_CLASS_NETWORK_ETHERNET:	return "Ethernet controller";
	case PCI_CLASS_NETWORK_TOKEN_RING:	return "Token ring network controller";
	case PCI_CLASS_NETWORK_FDDI:		return "FDDI network controller";
	case PCI_CLASS_NETWORK_ATM:		return "ATM network controller";
	case PCI_CLASS_NETWORK_OTHER:		return "Network controller";

	case PCI_CLASS_DISPLAY_VGA:		return "VGA compatible controller";
	case PCI_CLASS_DISPLAY_XGA:		return "XGA compatible controller";
	case PCI_CLASS_DISPLAY_OTHER:		return "Display controller";

	case PCI_CLASS_MULTIMEDIA_VIDEO:	return "Multimedia video controller";
	case PCI_CLASS_MULTIMEDIA_AUDIO:	return "Multimedia audio controller";
	case PCI_CLASS_MULTIMEDIA_OTHER:	return "Multimedia controller";

	case PCI_CLASS_MEMORY_RAM:		return "RAM memory";
	case PCI_CLASS_MEMORY_FLASH:		return "FLASH memory";
	case PCI_CLASS_MEMORY_OTHER:		return "Memory";

	case PCI_CLASS_BRIDGE_HOST:		return "Host bridge";
	case PCI_CLASS_BRIDGE_ISA:		return "ISA bridge";
	case PCI_CLASS_BRIDGE_EISA:		return "EISA bridge";
	case PCI_CLASS_BRIDGE_MC:		return "MicroChannel bridge";
	case PCI_CLASS_BRIDGE_PCI:		return "PCI bridge";
	case PCI_CLASS_BRIDGE_PCMCIA:		return "PCMCIA bridge";
	case PCI_CLASS_BRIDGE_NUBUS:		return "NuBus bridge";
	case PCI_CLASS_BRIDGE_CARDBUS:		return "CardBus bridge";
	case PCI_CLASS_BRIDGE_OTHER:		return "Bridge";

	case PCI_CLASS_COMMUNICATION_SERIAL:	return "Serial controller";
	case PCI_CLASS_COMMUNICATION_PARALLEL:	return "Parallel controller";
	case PCI_CLASS_COMMUNICATION_OTHER:	return "Communication controller";

	case PCI_CLASS_SYSTEM_PIC:		return "PIC";
	case PCI_CLASS_SYSTEM_DMA:		return "DMA controller";
	case PCI_CLASS_SYSTEM_TIMER:		return "Timer";
	case PCI_CLASS_SYSTEM_RTC:		return "RTC";
	case PCI_CLASS_SYSTEM_OTHER:		return "System peripheral";

	case PCI_CLASS_INPUT_KEYBOARD:		return "Keyboard controller";
	case PCI_CLASS_INPUT_PEN:		return "Digitizer Pen";
	case PCI_CLASS_INPUT_MOUSE:		return "Mouse controller";
	case PCI_CLASS_INPUT_OTHER:		return "Input device controller";

	case PCI_CLASS_DOCKING_GENERIC:		return "Generic Docking Station";
	case PCI_CLASS_DOCKING_OTHER:		return "Docking Station";

	case PCI_CLASS_PROCESSOR_386:		return "386";
	case PCI_CLASS_PROCESSOR_486:		return "486";
	case PCI_CLASS_PROCESSOR_PENTIUM:	return "Pentium";
	case PCI_CLASS_PROCESSOR_ALPHA:		return "Alpha";
	case PCI_CLASS_PROCESSOR_POWERPC:	return "Power PC";
	case PCI_CLASS_PROCESSOR_CO:		return "Co-processor";

	case PCI_CLASS_SERIAL_FIREWIRE:		return "FireWire (IEEE 1394)";
	case PCI_CLASS_SERIAL_ACCESS:		return "ACCESS Bus";
	case PCI_CLASS_SERIAL_SSA:		return "SSA";
	case PCI_CLASS_SERIAL_USB:		return "USB Controller";
	case PCI_CLASS_SERIAL_FIBER:		return "Fiber Channel";

	default:				return "Unknown class";
    }
}

/******************************************************************************/

/* get device pciScan index */

int pciDevIxGet(unsigned bus, unsigned dev, unsigned fun,
    unsigned* id, unsigned class)
{
    PCI_DEV d = { cookie : -1 }; unsigned ix = 0, i;

    while(pciScan(&d) != -1)
    {
	if(class != -1 && (pciConfigInLong(d.bus,d.dev,d.fun,0x08) >> 16) != class)
	    continue;

    	for(i = 0; id[i] != -1; i++)
	{
	    if(id[i] == d.id)
	    {
		if(d.bus == bus && d.dev == dev && d.fun == fun)
		    return ix;

		ix++; break;
	    }
	}
    }

    return -1;
}

/******************************************************************************/

/* display available pci devices */

void pciDevs(int detail)
{
    PCI_DEV d = { cookie : -1 }; int i, j;

    if(!detail)
	printf("    Bus   Dev   Fun       id         status                 class\n"
	       "   ----- ----- ----- ------------ ------------ -------------------------------\n");

    while(pciScan(&d) != -1)
    {
	if(detail)
	{
	    printf("\nBus %i Device %i Function %i:\n\n",d.bus,d.dev,d.fun);
	    for(i = 0; i < 0x100; i += 0x10)
	    {
		printf("0x%02x",i);
		for(j = 0; j < 0x10; j += 4)
		    printf(" 0x%08x",pciConfigInLong(d.bus,d.dev,d.fun,i+j));
		printf("\n");
	    }
	    printf("\n\n\n\nType <CR> to continue, Q<CR> to stop: "); fflush(stdout);

	    switch(getchar())
	    {
		case '\n':
		    break;

		case 'q': case 'Q':
		    while(getchar() != '\n');
		    return;

		default:
		    while(getchar() != '\n');
		    break;
	    }
	}
	else
	{
	    printf("     %i     %i     %i    0x%08x   0x%08x   %s\n",
		d.bus,d.dev,d.fun,d.id,pciConfigInLong(d.bus,d.dev,d.fun,4),
		pciClassNameGet(pciConfigInLong(d.bus,d.dev,d.fun,8) >> 8));
	}
    }
    fflush(stdout);
}

/******************************************************************************/

/* disable all pci devices */

void pciDevsDisable(int skip_bridge)
{
    PCI_DEV d = { cookie : -1 };

    /* do ethernet soft reset */

    *(volatile int*)(SC64_ETH_REGS) = CT_LE_L(1);

    while(pciScan(&d) != -1)
    {
	if(skip_bridge && d.dev == pciHostDev)
	    continue;
	pciConfigOutLong(d.bus,d.dev,d.fun,0x4,0);
    }
}

/******************************************************************************/

/* add device function to pciDevList */

static void pciFunAdd(int bus, int dev, int fun, unsigned id)
{
    unsigned sr = (dev == pciHostDev) ? 0xffff0004 : 0xffff0000;
    PCI_NODE* pciNode;

    pciConfigOutLong(bus,dev,fun,0x04,sr);

    if(!(pciNode = malloc(sizeof(PCI_NODE))))
	return;

    pciNode->id  = id;
    pciNode->bus = bus;
    pciNode->dev = dev;
    pciNode->fun = fun;

    pciNode->next = pciDevList; pciDevList = pciNode;
}

/******************************************************************************/

/* try read specified register */

static unsigned pciMemProbe(int bus, int dev, int fun)
{
    volatile unsigned val; register unsigned res;

    /* clear master abort, keep master enable */

    pciConfigOutLong(0,pciHostDev,0,4,0x20000004);

    tryBegin()
	val = *pciConfigReg(bus,dev,fun,0);
    tryCatch()
	val = 0xffffffff;
    tryEnd()

    /* check master abort */
    
    if(pciConfigInLong(0,pciHostDev,0,4) & 0x20000000)
	return 0xffffffff;

    res = val;

    return CF_LE_L(res);
}

/******************************************************************************/

/* add pci device functions to pciDevList */

static void pciDevAdd(int bus, int dev)
{
    unsigned id[8]; int fun;

    id[0] = pciMemProbe(bus,dev,0);

    if(id[0] == 0xffffffff)
	return;

    /* for not multifunction device add only function 0 */

    if(!(pciConfigInLong(bus,dev,0,0xc) & 0x00800000))
    {
	pciFunAdd(bus,dev,0,id[0]);
	return;
    }

    /* read device & vendor id from device functions */

    for(fun = 1; fun < 8; fun++)
    {
	id[fun] = pciMemProbe(bus,dev,fun);

	if(id[fun] == 0xffffffff)
	    break;
    }

    /* add device functions to list */

    for(fun = fun - 1; fun >= 0; fun--)
	pciFunAdd(bus,dev,fun,id[fun]);
}

/******************************************************************************/

/* initialize the pciLib (work only once) */

int pciLibInit()
{
#ifdef	K64RIO
    if(*(volatile unsigned*)(SYSTEM_REGS + 0x17c) & 0x20000)
	return -1;
#endif

#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron  
    pciSpace[0].memStart = PCI_MEM0_BASE + 0x400000; // окно для PSC 4 Мб
    pciSpace[0].memBound = PCI_MEM0_BASE + PCI_MEM0_SIZE;
    pciSpace[0].ioStart = PCI_IO_BASE;
    pciSpace[0].ioBound = PCI_IO_BASE + PCI_IO_SIZE;
#else

#ifdef	INCLUDE_VME
    pciSpace[0].memStart = PCI_MEM0_BASE + VME_MWIN_SIZE*8;           /* VME master wins */
  #ifdef  BT55211
    pciSpace[0].memBound = PCI_MEM0_BASE + PCI_MEM0_SIZE - 0x200000;  /* DPRAM's registers (1024) + slow (512*1024) and fast (1024*1024) wins + VME regs*/
  #else
    pciSpace[0].memBound = PCI_MEM0_BASE + PCI_MEM0_SIZE - 0x40000;   /* VME regs */
  #endif
#elif defined(MPON)
    pciSpace[0].memStart = PCI_MEM0_BASE + 0x4000000;
    pciSpace[0].memBound = PCI_MEM0_BASE + PCI_MEM0_SIZE - 0x400000;
#else
    pciSpace[0].memStart = PCI_MEM0_BASE;
    pciSpace[0].memBound = PCI_MEM0_BASE + PCI_MEM0_SIZE;
#endif
    pciSpace[0].ioStart = PCI_IO_BASE;
    pciSpace[0].ioBound = PCI_IO_BASE + PCI_IO_SIZE;

#endif //FOR_SUBMICRON

    /* do ethernet soft reset */

    *(volatile int*)(SC64_ETH_REGS) = CT_LE_L(1);


#ifdef BT83M
    pciDevAdd(0,9);
    pciDevAdd(0,8);
    pciDevAdd(0,7);
    pciDevAdd(0,6);
#else
    if(pciConfigInLong(0,0,0,0) != 0xc0fe191e)
    {
	pciHostDev = 20;
	pciDevAdd(0,18);		/* PMC */
	pciDevAdd(0,19);		/* Ethernet */
    }
    else
    {
	pciDevAdd(0,4);			/* PMC */
	pciDevAdd(0,3);			/* VGA or PMC !!! */
	pciDevAdd(0,2);			/* VME */
	pciDevAdd(0,1);			/* MIO or DPRAM for bt55-211PD */
    }
#endif
    pciDevAdd(0,pciHostDev);	/* Host Bridge */

#ifdef FOR_SUBMICRON
 pciConfigOutLong(0,pciHostDev,0,0x10,PCI_REG_BAR_val);
 pciConfigOutLong(0,pciHostDev,0,0x58,PCI_REG_MASK_TRANS_val);		
 pciConfigOutLong(0,pciHostDev,0,0x40,PCI_REG_TRANS_val);	
 pciConfigOutLong(0,pciHostDev,0,0x44,PCI_IO_BASE);
 pciConfigOutLong(0,pciHostDev,0,0x04,PCI_REG_COMSTAT_val);
 pciConfigOutLong(0,pciHostDev,0,0xe4,PCI_REG_ENARET_val);		// return 0xff on master and target abort
 pciConfigOutLong(0,pciHostDev,0,0x7c,PCI_REG_INT_val);	// clear and enable all error interrupts
#else

    pciConfigOutLong(0,pciHostDev,0,0x10,0);
#ifdef BT211
    pciConfigOutLong(0,pciHostDev,0,0x58,0x70000000);	// do compatible with old version
    pciConfigOutLong(0,pciHostDev,0,0x40,PCI_MEM0_BASE);// do compatible with old version
#else
    pciConfigOutLong(0,pciHostDev,0,0x58,0);		// only new version: enable 2GB PCI memory space 
    pciConfigOutLong(0,pciHostDev,0,0x40,0x80000000);	// old version replace: [31:28], new version replace: [31]
#endif
    pciConfigOutLong(0,pciHostDev,0,0x44,PCI_IO_BASE);
#if defined(MPON)
    pciConfigOutLong(0,pciHostDev,0,0x04,0xffff0006);  /* MPON create SERR and PERR from nowhere ! skip it */
#else
    pciConfigOutLong(0,pciHostDev,0,0x04,0xffff0146);
#endif    
//    pciConfigOutLong(0,pciHostDev,0,0xe0,0x20);		// burst size
    pciConfigOutLong(0,pciHostDev,0,0xe4,0x6);		// return 0xff on master and target abort
    pciConfigOutLong(0,pciHostDev,0,0x7c,0xf0f00);	// clear and enable all error interrupts

#endif //FOR_SUBMICRON

    /* check big memory support */
    
    if((pciConfigInLong(0,pciHostDev,0,0x58) >> 12) == 0)
	pciBigMem = TRUE;

    /* connect error interrupt handlers */

    intConnect(IV_PCI,(void*)pciErrInt,0);
    boardIvEnable(IV_PCI);

    return 0;
}
