/* bt502.h - BT23-502 terminal driver header */

#ifndef	__INCbt502h
#define	__INCbt502h

typedef struct {
    unsigned	dotClock;	/* hTotal*vTotal*Hz */
    unsigned	hDispSize;	/* X resolution: 640, 800, 1024 and etc. */
    unsigned	hSyncWidth;
    unsigned	hDataStart;
    unsigned	hDataEnd;
    unsigned	hTotal;
    unsigned	xSync;
    unsigned	vDispSize;	/* Y resolution: 480, 600, 768 and etc. */
    unsigned	vSyncWidth;
    unsigned	vDataStart;
    unsigned	vDataEnd;
    unsigned	vTotal;
    unsigned	yBlank;
    int		hSyncPol;	/* '+' or '-' */
    int		vSyncPol;	/* '+' or '-' */
} BT502_FP_MODE;

/* flags */

#define	FL_CURSOR_BLINK		1
#define	FL_FLAT_PANEL		2
#define RGB_SET_INTERRUPT       4       /* use interrupt for mem bug fix on bt502 */

/* typedefs */

typedef struct bt502_ctrl BT502_CTRL;

/* function declarations */

BT502_CTRL* bt502DevInit(int unit, int inherit, int priority, void* mode,
    int bg, int fg, int tab, unsigned char* font, int depth, int flags);
STATUS bt502HwInit(BT502_CTRL* rgb);
void bt502PutChar(BT502_CTRL* rgb, int ch, int x, int y);
void bt502ClearScreen(BT502_CTRL* rgb);
void bt502ScrollUp(BT502_CTRL* rgb);
void bt502MoveCursor(BT502_CTRL* rgb, int x, int y);
void bt502TermReset(BT502_CTRL* rgb);
STATUS bt502SendChar(BT502_CTRL* rgb, char ch);
int bt502PrintChar(BT502_CTRL* rgb, unsigned char c);
int bt502DrvVersion(int silence);

#endif	/* __INCbt502h */
