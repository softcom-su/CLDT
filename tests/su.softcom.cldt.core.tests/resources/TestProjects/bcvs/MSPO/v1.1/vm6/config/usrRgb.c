/* usrRgb.c - RGB console initialization */

/*
    28may08,laz - INVLUDE_RGB_SET_INTERRUPT
*/

#include <rgbDrv.h>
#include <bt502.h>
#include <drv/rgb/font.c>

/* flat panel mode index */

#define	FP_RS6		0
#define	FP_SHARP	1
#define	FP_RS1		2

/* globals */

void* sysRgbId;			/* parameter of sysRgbHook */

/* forward declaration */

int sysRgbHook(NS_CHANNEL* ns, int cmd, void* id, unsigned val);

/******************************************************************************/

/* typical CRT display modes: from Linux RedHat 6.2 XF86Config */

CRT_MODE sysCrtMode[]  = {
    /* 640x480 @ 60 Hz, 31.5 kHz hsync */
    {25175,640,656,752,800,480,490,492,525,'-','-'},
    /* 640x480 @ 72 Hz, 36.5 kHz hsync */
    {31500,640,680,720,864,480,488,491,521,'-','-'},
    /* 640x480 @ 75 Hz, 37.50 kHz hsync */
    {31500,640,656,720,840,480,481,484,500,'-','-'},
    /* 640x480 @ 85 Hz, 43.27 kHz hsync */
    {36000,640,696,752,832,480,481,484,509,'-','-'},
    /* 640x480 @ 100 Hz, 53.01 kHz hsync */
    {45800,640,672,768,864,480,488,494,530,'-','-'},
    /* 800x600 @ 56 Hz, 35.15 kHz hsync */
    {36000,800,824,896,1024,600,601,603,625,'-','-'},
    /* 800x600 @ 60 Hz, 37.8 kHz hsync */
    {40000,800,840,968,1056,600,601,605,628,'+','+'},
    /* 800x600 @ 72 Hz, 48.0 kHz hsync */
    {50000,800,856,976,1040,600,637,643,666,'+','+'},
    /* 800x600 @ 85 Hz, 55.84 kHz hsync */
    {60750,800,864,928,1088,600,616,621,657,'-','-'},
    /* 800x600 @ 100 Hz, 64.02 kHz hsync */
    {69650,800,864,928,1088,600,604,610,640,'-','-'},
    /* 1024x768 @ 60 Hz, 48.4 kHz hsync */
    {65000,1024,1032,1176,1344,768,771,777,806,'-','-'},
    /* 1024x768 @ 70 Hz, 56.5 kHz hsync */
    {75000,1024,1048,1184,1328,768,771,777,806,'-','-'},
    /* 1024x768 @ 76 Hz, 62.5 kHz hsync */
    {85000,1024,1032,1152,1360,768,784,787,823,'-','-'},
    /* 1024x768 @ 85 Hz, 70.24 kHz hsync */
    {98900,1024,1056,1216,1408,768,782,788,822,'-','-'},
    /* 1024x768 @ 100Hz, 80.21 kHz hsync */
    {115500,1024,1056,1248,1440,768,771,781,802,'-','-'},
    /* user defined */
    {0,0,0,0,0,0,0,0,0,'-','-'}
};

/******************************************************************************/

/* typical Flat Panel display modes */

BT502_FP_MODE sysBT502FpMode[]  = {
    /* RS6 Flat Panel: 800x600 @ 60Hz */
    {40000,800,0x46,0x7e,0x39e,0x3fe,255,600,0,0x17,0x26e,0x270,57,'-','-'},
    /* Sharp Flat Panel: 800x600 @ 60Hz */
    {40000,800,0x7f,0xd6,0x3f6,0x41e,287,600,0x3,0x16,0x26e,0x299,96,'-','-'},
    /* RS1 Flat Panel: 1024x768 @ 60Hz */
    {40000,1024,0x7f,0x13f,0x53f,0x540,349,768,0x3,0x25,0x325,0x327,71,'+','+'},
    /* user defined */
    {0,0,0,0,0,0,0,0,0,'-','-'}
};

/******************************************************************************/

/* RGB console initialization */

#if defined(RS6M) || defined(BT206_RS6)
static int fdconsole=0, nwritten=0;
#endif

int sysRgb502Unit=0;
STATUS sysRgbInit()
{
    NS_CHANNEL* ns; int flags = 0; void* mode;

#ifdef RGB_CURSOR_BLINK
    flags |= FL_CURSOR_BLINK;
#endif

#ifdef RGB_FLAT_PANEL
    flags |= FL_FLAT_PANEL;
    mode = &sysBT502FpMode[FP_MODE];
#else
    mode = &sysCrtMode[RGB_MODE];
#endif

#ifdef INCLUDE_RGB_SET_INTERRUPT
    flags |= RGB_SET_INTERRUPT
#endif

    if(!(sysRgbId = bt502DevInit(sysRgb502Unit,RGB_INHERIT_MODE,RGB_THREAD_PRI,
	mode,COLOR_BG,COLOR_FG,TAB_SIZE,
	font8x16,RGB_MQ_DEPTH,flags)))
    {
      // kprint ("bt502DevInit error !!!\n\r");
      return -1;
    }

    if(!(ns = nsChannelFind(CONSOLE_DEVICE)))
    {
	return -1;
    }

    ns->hookArg = (int)sysRgbId;
    ns->hookRtn = sysRgbHook;

#if defined(RS6M) || defined(BT206_RS6)
     // kprint ("\n\rDisabling console port output\n\r");
     fdconsole = open (CONSOLE_DEVICE,2);
     if (fdconsole >=0) ioctl (fdconsole, FIOTxNULL,1);
#endif
    return 0;
}

/******************************************************************************/

/* RGB console reinitialization (i.e. restore from X) */

STATUS sysRgbReset()
{
    if(bt502HwInit(sysRgbId) == -1)
	return -1;

    bt502TermReset(sysRgbId);
    return 0;
}

/******************************************************************************/

/* RGB console print character routine for tty_outstring() */

void sysRgbPrintChar(char c)
{
    if(sysRgbId)
	bt502PrintChar(sysRgbId,c);
}

/******************************************************************************/

/* hook for terminal driver */

int sysRgbHook(NS_CHANNEL* ns, int cmd, void* id, unsigned val)
{
    static int isGraphMode = FALSE;

    switch(cmd)
    {
	case NS_HOOK_TX:
	    if(!isGraphMode)
		bt502SendChar(id,val);
	    return 0;

#ifdef	_1B813
	case NS_HOOK_RX:
	    /* do hardware board reset on ^R */
	    if(val == 0x12)
		boardReset();
	    return 0;
#endif	/* _1B813 */

	case NS_HOOK_VMODE:
	{
	    int result = 0;
	    if(!val)
		result = sysRgbReset();
	    isGraphMode = val;
	    return result;
	}

	case NS_HOOK_OUT:
	    if(!isGraphMode)
		sysRgbPrintChar(val);
	    return 0;
    }

    return -1;
}

