//---------------------------------------------------------------------------------------------	
//			mouseDrv.h
//---------------------------------------------------------------------------------------------	
#ifndef	_MOUSE_H
#define	_MOUSE_H

typedef struct  
{
	uintptr_t		base[2];
	VOIDFUNCPTR		out[2];
	FUNCPTR			in[2];	
	unsigned		id;	
} PS2_CHANNEL;

extern int mouseDevInit (char *, int, int, int, int);
#endif		// _MOUSE_H
