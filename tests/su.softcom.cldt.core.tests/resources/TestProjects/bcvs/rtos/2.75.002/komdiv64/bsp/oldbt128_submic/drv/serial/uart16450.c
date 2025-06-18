/* uart16450.c - UART 16450/16550 driver for K128 */

#ifndef _INCLUDED_NS16450_
#define _INCLUDED_NS16450_

#include <sys/ioctl.h>
#include <private/bindmodule.h>
#include <private/terminal.h>
#include <ns16x50.h>

/* number of UART's in system controller */

unsigned k128AuxChannelMax = 2;

/******************************************************************************/

/* register access for k128 */

static void nsOut_k128(volatile unsigned char* base, unsigned reg, unsigned char val)
{
    base[reg*4] = val;
}

static unsigned nsIn_k128(volatile unsigned char* base, unsigned reg)
{
    return base[reg*4];
}

/******************************************************************************/

/* ioctl for k128 */

static int nsIoctl_k128(NS_CHANNEL* ns, int cmd, int arg)
{
    unsigned divisor;

    switch(cmd)
    {
	case NS_CMD_DIV_GET:
	{
	    if(arg < 50 || arg > 115200)
		return -1;

	    divisor = UART_CLK_FREQ / 16 / arg;

	    return (divisor < 3) ? (-1) : divisor;
	}

	case NS_CMD_FIFO_GET:
	    NS_OUT(2,FCR_TX_RST|FCR_RX_RST);
	    return 1;

	case TIOCSETA:
	    return 0;
    }

    return -1;
}

/******************************************************************************/

/* device descriptors */

static NS_CHANNEL uart[] = {
    [0] = {
        dev		: (void *)NULL,
        name		: (char *)NULL,
        base		: UART0_BASE,
	in		: (void*)nsIn_k128,
	out		: (void*)nsOut_k128,
	ioctl		: nsIoctl_k128,
	id		: IV_UART0,
    },
    [1] = {
        dev		: (void *)NULL,
        name		: (char *)NULL,
        base		: UART1_BASE,
	in		: (void*)nsIn_k128,
	out		: (void*)nsOut_k128,
	ioctl		: nsIoctl_k128,
	id		: IV_UART1,
    },
};

/******************************************************************************/

/* this routine initialized both UART's and called from boardHwInit() */

void k128AuxHwInit(void)
{
    int i;

    for(i = 0; i < k128AuxChannelMax; i++)
	nsHwInit(&uart[i],115200);
}

/******************************************************************************/

/* terminal device driver initialization routine */

void* _k128AuxDevInit(char* name, int channel, int rdSize, int wrSize,
    int speed, int cread)
{
    NS_CHANNEL* ns = &uart[channel];

    /* verify parameters */

    if(channel < 0 || channel >= k128AuxChannelMax)
    {
	errno = EINVAL; return NULL;
    }

    /* connect interrupt handler */
    
    intConnect(ns->id,(void*)nsIntr,(int)ns);

    /* initialize terminal device driver */

    if(_nsDevInit(ns,name,rdSize,wrSize,speed,cread) == -1)
	return NULL;

    /* enable ASIC UART interrupt */

    boardIvEnable(ns->id);

    return ns->dev;
}

/******************************************************************************/

/* terminal device driver initialization routine with rx irq on */

void* k128AuxDevInit(char* name, int channel, int rdSize, int wrSize, int speed)
{
    return _k128AuxDevInit(name,channel,rdSize,wrSize,speed,TRUE);
}

#endif
