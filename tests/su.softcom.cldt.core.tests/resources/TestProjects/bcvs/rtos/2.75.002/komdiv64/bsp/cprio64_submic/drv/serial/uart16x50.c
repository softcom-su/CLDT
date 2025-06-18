/* uart16x50.c - UART 16450/16550 driver for SC64 controller */

#include <sys/ioctl.h>
#include <private/bindmodule.h>
#include <private/terminal.h>
#include <ns16x50.h>

/******************************************************************************/

/* register access for SC64 */

static void nsOut_sc64(volatile unsigned char* base, unsigned reg,
    unsigned char val)
{
    base[reg] = val;
}

static unsigned nsIn_sc64(volatile unsigned char* base, unsigned reg)
{
    return base[reg];
}

/******************************************************************************/

/* ioctl for sc64 */

static long nsIoctl_sc64(NS_CHANNEL* ns, int cmd, int arg)
{
    switch(cmd)
    {
	case NS_CMD_DIV_GET:
	    if(arg < 50 || arg > 115200)
		return -1;
	    return 115200 / arg;

	case NS_CMD_FIFO_GET:
	    NS_OUT(2,0);
	    return 1;

	case TIOCSETA:
	    return 0;
    }

    return -1;
}

/******************************************************************************/

/* device descriptors */

static NS_CHANNEL uart[2] = {
    [0] = {
	base		: UART0_BASE,
	in		: (void*)nsIn_sc64,
	out		: (void*)nsOut_sc64,
	ioctl		: nsIoctl_sc64,
	id		: IV_UART0,
    },
    [1] = {
	base		: UART1_BASE,
	in		: (void*)nsIn_sc64,
	out		: (void*)nsOut_sc64,
	ioctl		: nsIoctl_sc64,
	id		: IV_UART1,
    }
};

/******************************************************************************/

/* this routine initialized both UART's and called from boardHwInit() */

void sc64AuxHwInit(void)
{
    nsHwInit(&uart[0],9600);
    nsHwInit(&uart[1],9600);
}

/******************************************************************************/

/* terminal device driver initialization routine */
//static void checkTx (NS_CHANNEl *);

void* _sc64AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed, int cread)
{
    NS_CHANNEL* ns = &uart[channel];

    /* verify parameters */

    if(channel < 0 || channel >= 2)
    {
	errno = EINVAL; return NULL;
    }

    // kprint("_sc64AuxDevInit(%s,%i,..), IER=%x\n\r", name, channel, NS_IN(1));
    /* connect interrupt handler */
    
    intConnect(ns->id,(void*)nsIntr,(int)ns);

    /* initialize terminal device driver */

    if(_nsDevInit(ns,name,rdSize,wrSize,speed, cread) == -1)
	return NULL;

    if(boardFreqRef == 25000000)
    {
	termDEV* t = ns->dev;
	t->te_ti.c_cflag |= CSTOPB;
	NS_OUT(3,NS_IN(3)|LCR_STOP2);
    }

    /* enable the UART[channel]  in interrupt controller */

    boardIvEnable(ns->id);

    return ns->dev;
}
#if 0
static int nout = 0;
static void checkTx (NS_CHANNEl *ns)
{ unsigned char c; 
  termDEV* term = ns->dev;
  
  if (ns->nOut == nout)    /* No progress in Tx ??? */
    { if ((termDev *)ns-dev)->te_getchar ((termDev *)ns-dev, &c) == TERM_OK)
       NS_OUT (0, c); 
       ns->nout++; nout = ns->nOut;
    }
}
#endif
/*--------------------------------------------------------------------------------------------*/
void* sc64AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed)
{
  return _sc64AuxDevInit(name, channel, rdSize, wrSize, speed, 1);
}
