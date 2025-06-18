/* usrSerial.c - user serial driver initialization */

#include <bagetSerial.h>

#include <drv/serial/uart16x50.c>

#ifndef	INCLUDE_CONSOLE
#include <nullConsole.h>
#endif

#ifndef MPON
#ifndef BT83M
  #include "kbd_mouse.h"

  #define MOUSE_NAME	"/dev/mouse"
  #define MOUSE_TYPE	MS_MOUSE
#endif
#endif

#define XCONSOLE_NAME	"/dev/xconsole"
#define KBD_TYPE	KL85_MODE_AT

/* system controller descriptor */

BAGET_SERIAL_CTRL* pSysBsCtrl;

#define	PCI_AUX_INIT(x)					\
{							\
    _bagetAuxDevInit(pSysBsCtrl,PCI_AUX_NAME##x,x,	\
	PCI_AUX_RBUF##x,PCI_AUX_WBUF##x,		\
	PCI_AUX_SPEED##x,PCI_AUX_INT_ENABLE##x);	\
}


/******************************************************************************/

/* serial driver initialization */

STATUS usrSerialConfig()
{
    NS_CHANNEL* ns; int fd;

    TRACE(TR_START,0xcc0000);

#ifdef	AUX_CONFIG
  #ifdef AUX_CONFIG0
    _sc64AuxDevInit(AUX_NAME0,0,AUX_RBUF0, AUX_WBUF0,AUX_SPEED0,AUX_INT_ENABLE0);
  #endif
  #ifdef AUX_CONFIG1
    _sc64AuxDevInit(AUX_NAME1,1,AUX_RBUF1, AUX_WBUF1,AUX_SPEED1,AUX_INT_ENABLE1);
  #endif
    if((fd = open("/dev/console",O_RDWR,0)) >= 0)
    {
        kprint ("SETTING FIFO !!!!!!!!!!!!!!!\n\r");
	ioctl(fd,FIOFIFOLVLSET,FCR_RX_L2);
	close(fd);
    }
#endif

#ifdef BT211
    /* init PCI 1890VG8T ports */ 

    if(!(pSysBsCtrl = bagetSerialInit(0, BS_MODE_48MHZ)))
    {
	return -1;
    }
    TRACE(TR_START,0xcc0020);

#ifdef PCI_AUX_CONFIG0
    PCI_AUX_INIT(0);
    TRACE(TR_START,0xcc0021);
#endif

#ifdef PCI_AUX_CONFIG1
    PCI_AUX_INIT(1);
    TRACE(TR_START,0xcc0022);
#endif

#ifdef PCI_AUX_CONFIG2
    PCI_AUX_INIT(2);
    TRACE(TR_START,0xcc0023);
#endif

#ifdef PCI_AUX_CONFIG3
    PCI_AUX_INIT(3);
    TRACE(TR_START,0xcc0024);
#endif

#ifdef PCI_AUX_CONFIG4
    PCI_AUX_INIT(4);
    TRACE(TR_START,0xcc0025);
#endif

#endif	// BT211

#ifdef	INCLUDE_CONSOLE

    /* initialize kprint routine and attach default hook routine */

    ns = nsChannelFind(CONSOLE_DEVICE);

    if(ns)
    {
	boardOutStringArg = (long)ns;
	boardOutStringRtn = (FUNCPTR)nsOutString;
	TRACE(TR_START,0xcc0034);
    }
    else
    {
	boardOutStringArg = 0;
	boardOutStringRtn = 0;
	TRACE(TR_START,0xcc0038);
    }

    TRACE(TR_START,0xcc0040);

#else	/* INCLUDE_CONSOLE */

    nullConsoleInit("/dev/null");
    boardOutStringRtn = NULL;
    TRACE(TR_START,0xcc0050);

#endif	/* INCLUDE_CONSOLE */

#ifdef BT211

    if(nsChannelFind(MOUSE_NAME))
    {
	TRACE(TR_START,0xcc0060);
	if((fd = open(MOUSE_NAME,O_RDWR,0)) >= 0)
	{
	    TRACE(TR_START,0xcc0064);
	    ioctl(fd,FIOFIFOLVLSET,0);
	    TRACE(TR_START,0xcc0065);
	    close(fd);
	    TRACE(TR_START,0xcc0066);
	}

	TRACE(TR_START,0xcc0068);
	serial_port_mouse_init(MOUSE_NAME,MOUSE_TYPE);
	TRACE(TR_START,0xcc006c);
    }

    if(nsChannelFind(XCONSOLE_NAME))
    {
	TRACE(TR_START,0xcc0070);
	if((fd = open(XCONSOLE_NAME,O_RDWR,0)) >= 0)
	{
	    TRACE(TR_START,0xcc0074);
	    ioctl(fd,FIOFIFOLVLSET,0);
	    TRACE(TR_START,0xcc0075);
	    close(fd);
	    TRACE(TR_START,0xcc0076);
	}

	TRACE(TR_START,0xcc0078);
	serial_port_kl85_init(XCONSOLE_NAME,KBD_TYPE);
	TRACE(TR_START,0xcc007c);
    }

#endif

    return 0;
}

/******************************************************************************/

#if PS2MOUSE==1
#warning PS2MOUSE !!!!!!!!!!!!!!!!!!!!!
#include "mouseDrv.h"

int ps2mouseInit(void)
{
   /* channel, mode and type - don't change */
   return mouseDevInit(PS2MOUSE_NAME,PS2MOUSE_BUF_SIZE,0,1,0);
}

#endif


