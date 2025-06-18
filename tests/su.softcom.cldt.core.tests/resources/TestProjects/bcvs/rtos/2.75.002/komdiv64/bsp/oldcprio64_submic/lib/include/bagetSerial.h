/* bagetSerial.h - 1890VG8T multi controller serial ports driver header */

#ifndef INCbagetSerialh
#define INCbagetSerialh

#include <ns16x50.h>

#define	BS_MODE_48MHZ		0x10
#define	BS_MODE_24MHZ		0x00

/* maximum high speed baud rate */

#define	BS_MAX_BAUD_24MHZ	(BS_MAX_BAUD_48MHZ/2)
#define	BS_MAX_BAUD_48MHZ	2995200

/* typedefs */

typedef struct baget_serial_ctrl BAGET_SERIAL_CTRL;

/* globals */

extern int bsDebug;		/* debug level */

/* function declarations */

int bsDrvVersion(int silence);
void bagetSerialIntr(BAGET_SERIAL_CTRL* bs);
BAGET_SERIAL_CTRL* bagetSerialInit(unsigned unit, unsigned mode);
NS_CHANNEL* bagetSerialChannelGet(BAGET_SERIAL_CTRL* bs, unsigned channel);
void* bagetAuxDevInit(BAGET_SERIAL_CTRL* bs, char* name, int channel,
    int rdSize, int wrSize, int speed);
void* _bagetAuxDevInit(BAGET_SERIAL_CTRL* bs, char* name, int channel,
    int rdSize, int wrSize, int speed, int cread);

#endif /* INCbagetSerialh */
