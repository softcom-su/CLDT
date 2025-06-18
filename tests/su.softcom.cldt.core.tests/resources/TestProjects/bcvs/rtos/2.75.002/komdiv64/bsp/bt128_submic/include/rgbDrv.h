/* rgbDrv.h - common RGB terminal emulation driver header */

#ifndef	__INCrgbDrvh
#define	__INCrgbDrvh

/* index in sysCrtMode[] */

#define	CRT_640x480x60		0
#define	CRT_640x480x72		1
#define	CRT_640x480x75		2
#define	CRT_640x480x85		3
#define	CRT_640x480x100		4
#define	CRT_800x600x56		5
#define	CRT_800x600x60		6
#define	CRT_800x600x72		7
#define	CRT_800x600x85		8
#define	CRT_800x600x100		9
#define	CRT_1024x768x60		10
#define	CRT_1024x768x70		11
#define	CRT_1024x768x76		12
#define	CRT_1024x768x85		13
#define	CRT_1024x768x100	14

/* palette colors */

#define	BLACK		0
#define	LIGHTRED	1
#define	LIGHTGREEN	2
#define	LIGHTBLUE	3
#define	YELLOW		4
#define	LIGHTMAGENTA	5
#define	LIGHTCYAN	6
#define	LIGHTGREY	7
#define	LIGHTBROWN	8
#define	BROWN		9
#define	GREEN		10
#define	BLUE		11
#define	MARSH		12
#define	MAGENTA		13
#define	CYAN		14
#define	DARKGREY	15
#define	WHITE		255

/* CRT display mode descriptor: like Linux XF86Config */

typedef struct {
    unsigned	dotClock;	/* hTotal*vTotal*Hz */
    unsigned	hDispSize;	/* X resolution: 640, 800, 1024 and etc. */
    unsigned	hSyncStart;
    unsigned	hSyncEnd;
    unsigned	hTotal;
    unsigned	vDispSize;	/* Y resolution: 480, 600, 768 and etc. */
    unsigned	vSyncStart;
    unsigned	vSyncEnd;
    unsigned	vTotal;
    int		hSyncPol;	/* '+' or '-' ('-' is default in XF86Config) */
    int		vSyncPol;	/* '+' or '-' ('-' is default in XF86Config) */
} CRT_MODE;

/* function declarations */

STATUS sysRgbInit();
STATUS sysRgbReset();
void sysRgbPrintChar(char c);

#endif	/* __INCrgbDrvh */
