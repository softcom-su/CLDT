/* pciLib.c - K128 pci bus support library */

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
int pciHostDev = 0;			/* host bridge device number */

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
    unsigned mask;

    if(bus > 0)
	return;

    value = CF_LE_L(value);
    mask = intLock();
    *PM_CONFIG_ADDR_REG = 0x80000000 | (dev << 11) | (fun << 8) | reg;
    *PM_CONFIG_DATA_REG = value;
    intUnlock(mask);
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
    unsigned mask, value;

    if(bus > 0)
	return 0xffffffff;

    mask = intLock();
    *PM_CONFIG_ADDR_REG = 0x80000000 | (dev << 11) | (fun << 8) | reg;
    value = *PM_CONFIG_DATA_REG;
    intUnlock(mask);
    
    return CT_LE_L(value);
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

/******************************************************************************/

/* interrupt handler */

void pciIntr(int unused)
{
    unsigned stat = *PCI_INT_STATUS_REG, pstat;

    if(stat & PCI_INT_ERR)
    {
        pstat = pciConfigInLong(0,pciHostDev,0,4);
        pciConfigOutLong(0,pciHostDev,0,4,pstat);
        *PCI_INT_STATUS_REG = 0;

        if(pciUserRtn)
            pciUserRtn(pciUserArg,stat,pstat);
        else
            logMsg("%s: stat=%x pstat=%x\n",(int)__FUNCTION__,stat,pstat,0,0,0);
    }

    if(stat & PCI_INT_A)
        pciInt(0);
    if(stat & PCI_INT_B)
        pciInt(1);
    if(stat & PCI_INT_C)
        pciInt(2);
    if(stat & PCI_INT_D)
        pciInt(3);
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
        case PCI_INT_A: demux = 0; break;
        case PCI_INT_B: demux = 1; break;
        case PCI_INT_C: demux = 2; break;
        case PCI_INT_D: demux = 3; break;
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
        case PCI_INT_A: demux = 0; break;
        case PCI_INT_B: demux = 1; break;
        case PCI_INT_C: demux = 2; break;
        case PCI_INT_D: demux = 3; break;
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
    int mask;

    switch(iv)
    {
        case PCI_INT_A:
        case PCI_INT_B:
        case PCI_INT_C:
        case PCI_INT_D:
            break;
        default:
            return -1;
    }
    mask = intLock();
    *PCI_INT_MASK_REG &= ~iv;
    intUnlock(mask);
    return 0;
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
    int mask;
    
    switch(iv)
    {
        case PCI_INT_A:
        case PCI_INT_B:
        case PCI_INT_C:
        case PCI_INT_D:
            break;
        default:
            return -1;
    }
    mask = intLock();
    *PCI_INT_MASK_REG |= iv;
    intUnlock(mask);
    return 0;
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

    switch(dev)
    {
        case 7:
        case 6:
        case 5:
        case 2:
        case 1:
            return PCI_INT_A;
    }
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

    while(pciScan(&d) != -1)
	pciConfigOutLong(d.bus,d.dev,d.fun,0x4,0);
}

/******************************************************************************/

/* add device function to pciDevList */

static void pciFunAdd(int bus, int dev, int fun, unsigned id)
{
    unsigned sr = (dev == pciHostDev) ? 0xffff0004 : 0xffff0000;
    PCI_NODE* pciNode;

#ifndef FOR_SUBMICRON // if no bcvs 
    pciConfigOutLong(bus,dev,fun,0x04,sr);
#endif

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
    unsigned val;

    /* clear master abort */

    *PCI_INT_STATUS_REG = 0;

    val = pciConfigInLong(bus,dev,fun,0);

    /* check master abort */

    if(*PCI_INT_STATUS_REG & PCI_INT_M_ABORT)
	return 0xffffffff;

    return val;
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

#ifdef FOR_SUBMICRON // gmu: for bcvs from submicron  

    /* [31]=1 enable external access, [30:24]=0 mask 16MB, [0]=1 enable byte order rotation */
    //  *PS_BAR_REG = 0x80000001;
    *PS_BAR_REG = 1;  //gmu: доступ к пространству ВМ7 со стороны pci не разрешаем //TODO: обсудить

    /* map on PCI memory address 0 of RAM */
    *PS_SEGMENT_REG = 0;

    /* [5]=1 enable internal master, [4]=0 disable arbiter */
    *PCI_ARBITER_REG = 0x20;

    /* PCI address == CPU virtual address */
    *PCI_MEM_BAR_REG = 0x80000000; //PCI_MEM_BASE;//TODO: адрес может меняться, надо это учесть
    *PCI_IO_BAR_REG = PCI_IO_BASE;

    /* [30]=1 disable bus error on master abort, [5:0]=0 clear errors */
    *PCI_MASTER_STATUS_REG = 0x40000000;

    /* clear and mask all interrupts */
    *PCI_INT_MASK_REG = 0x3ff;
    *PCI_INT_STATUS_REG = 0;

    /*clear interrupt*/
    *PCI_INT_STATUS_REG = 0;

    pciDevAdd(0,4);     /* MKO1 */
    pciDevAdd(0,3);     /* MKO0 */
    pciDevAdd(0,2);     /* PSC */
    pciDevAdd(0,1);     /* 1890VM7  */
    pciDevAdd(0,0);     /* 1890VM6 */

// gmu: сразу отсупаем место под окно для PSC, оно по умолчанию будет вначеле области
    pciSpace[0].memStart = PCI_MEM_BASE + 4194304; // окно для PSC 4 Мб
    pciSpace[0].memBound = PCI_MEM_BASE + PCI_MEM_SIZE;
    pciSpace[0].ioStart = PCI_IO_BASE;
    pciSpace[0].ioBound = PCI_IO_BASE + PCI_IO_SIZE;

    return 0;

#endif /* FOR_SUBMICRON*/

    /* check host mode */
    if(!(*PCI_ARBITER_REG & 0x10))
	return -1;

    pciSpace[0].memStart = PCI_MEM_BASE;
    pciSpace[0].memBound = PCI_MEM_BASE + PCI_MEM_SIZE;
    pciSpace[0].ioStart = PCI_IO_BASE;
    pciSpace[0].ioBound = PCI_IO_BASE + PCI_IO_SIZE;

    /* [31]=1 enable external access, [30:24]=0f mask 256MB, [0]=1 enable byte order rotation */

    *PS_BAR_REG = 0x8f000001;

    /* map on PCI memory address 0 of RAM */

    *PS_SEGMENT_REG = 0;

    /* [5]=1 enable internal master, [4]=1 enable arbiter, [3]=0 enable round-robin arbiter mode */

    *PCI_ARBITER_REG = 0x30;

    /* PCI address == CPU virtual address */

    *PCI_MEM_BAR_REG = PCI_MEM_BASE;
    *PCI_IO_BAR_REG = PCI_IO_BASE;

    /* [30]=1 disable bus error on master abort, [5:0]=0 clear errors */

    *PCI_MASTER_STATUS_REG = 0x40000000;

    /* clear and mask all interrupts */

    *PCI_INT_MASK_REG = 0x3ff;
    *PCI_INT_STATUS_REG = 0;

    if(pciMemProbe(0,8,0) == 0x1002191e)
    {
	pciHostDev = 8;
	pciDevAdd(0,6);     /* PMC2 */
	pciDevAdd(0,7);     /* PMC1 */
	pciDevAdd(0,5);	    /* 1890VG18 */
	pciDevAdd(0,8);     /* Host Bridge */
    }
    else if(pciMemProbe(0,4,0) == 0x1002191e)
    {
	pciHostDev = 4;
	pciDevAdd(0,6);     /* PMC2 */
	pciDevAdd(0,5);     /* PMC2 ? */
	pciDevAdd(0,7);     /* PMC1 */
	pciDevAdd(0,4);     /* Host Bridge */
    }
    else if(pciMemProbe(0,0,0) == 0x1002191e)
    {
	pciHostDev = 0;
	pciDevAdd(0,2);     /* PMC */
	pciDevAdd(0,1);     /* ETH */
	pciDevAdd(0,0);     /* Host Bridge */
    }

    pciConfigOutLong(0,pciHostDev,0,0x10,0);
    pciConfigOutLong(0,pciHostDev,0,0x48,0xffff);        // [15:8]=ff Trdy counter, [7:0]=ff Retry counter
    pciConfigOutLong(0,pciHostDev,0,0x04,0xffff0146);

    *PCI_INT_STATUS_REG = 0;

    /* connect common interrupt handlers */

    intConnect(IV_PCI_S,(void*)pciInt,0);
#ifndef	DISABLE_PCI_INT
    boardIvEnable(IV_PCI_S);
#endif

    /* enable error interrupts */
//    *PCI_INT_MASK_REG = ~PCI_INT_ERR;
    return 0;
}
