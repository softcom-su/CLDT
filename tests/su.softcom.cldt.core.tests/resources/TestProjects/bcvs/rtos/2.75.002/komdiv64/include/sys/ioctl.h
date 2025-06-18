#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H


#define	TIOCMGET	_IOR('t', 3, int)	/* get modem control state */
#define	TIOCMSET	_IOW('t', 4, int)	/* set modem control state */
#define		TIOCM_LE	0001		/* line enable */
#define		TIOCM_DTR	0002		/* data terminal ready */
#define		TIOCM_RTS	0004		/* request to send */
#define		TIOCM_ST	0010		/* secondary transmit */
#define		TIOCM_SR	0020		/* secondary receive */
#define		TIOCM_CTS	0040		/* clear to send */
#define		TIOCM_CAR	0100		/* carrier detect */
#define		TIOCM_CD	TIOCM_CAR
#define		TIOCM_RNG	0200		/* ring */
#define		TIOCM_RI	TIOCM_RNG
#define		TIOCM_DSR	0400		/* data set ready */

#define	TIOCGETA	_IOR('t', 19, struct termios) /* get termios struct */
#define	TIOCSETA	_IOW('t', 20, struct termios) /* set termios struct */
#define	TIOCSETAF	_IOW('t', 21, struct termios) /* set termios struct */
#define	TIOCSETAW	_IOW('t', 22, struct termios) /* set termios struct */


#define TIOCSETFONT _IOW('t', 31, int)
#define TIOCFLUSH	_IOR('t', 32, int)
#define TIOCSBRK	_IOR('t', 33, int)
#define TIOCCBRK	_IOR('t', 34, int)
#define TIOCDRAIN	_IOR('t', 35, int)
#define TIOCSTI     _IOR('t', 36, char *)



#define TIOCSTOP	_IOR('t', 41, int)
#define TIOCSTART	_IOR('t', 42, int)
#define TIOCPKT		_IOR('t', 43, int)
#define TIOCUCNTL	_IOR('t', 44, int)
#define TIOCREMOTE	_IOR('t', 45, int)
#define TIOCMAXCAN  _IOR('t', 46, int)
#define TIOCMAXINP  _IOR('t', 47, int)

/* Для x-сервера */
#define K_RAW        0x00
#define K_XLATE      0x01
#define K_MEDIUMRAW  0x02
#define K_UNICODE    0x03

#define KDGKBMODE       0x4b44
#define KDSKBMODE       0x4b45
#define KDSETLED        0x4b46
#define KDGETLED        0x4b47
#define KDSETTVTEXTMODE 0x4b48

#define  LED_CAP     0x04
#define  LED_NUM     0x02
#define  LED_SCR     0x01
#include <sys/sockio.h>
#include <sys/ioccom.h>
#endif

