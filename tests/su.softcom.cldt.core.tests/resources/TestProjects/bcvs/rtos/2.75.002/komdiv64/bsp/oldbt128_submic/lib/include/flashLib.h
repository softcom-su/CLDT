/* flashLib.h - header file for flashLib */

#ifndef __INCflashLibh
#define __INCflashLibh

#define M_flashLib	(100 << 16)

#ifdef __cplusplus
extern "C" {
#endif

/* generic status codes */

#define S_flashLib_FILE_ERROR            ( M_flashLib | 0x01 )
#define S_flashLib_CLEAR_ERROR	         ( M_flashLib | 0x02 )
#define S_flashLib_WRITE_ERROR     	 ( M_flashLib | 0x03 )
#define S_flashLib_RANGE_ERROR	         ( M_flashLib | 0x04 )
#define S_flashLib_INVALID_FILE_FORMAT	 ( M_flashLib | 0x05 )
#define S_flashLib_MEM_NOT_ALLOCATED	 ( M_flashLib | 0x06 )
#define S_flashLib_NOT_EMPTY	         ( M_flashLib | 0x07 )
#define S_flashLib_PROTECTION	         ( M_flashLib | 0x08 )
#define S_flashLib_CHIP  	         ( M_flashLib | 0x09 )

/* function declarations */

#ifndef	__ASSEMBLER__
#if defined(__STDC__) || defined(__cplusplus)

extern unsigned *flash_base;
extern int flash_size, flash_nsec, flash_ssec;

extern void   flashShowInit ( void ) ;
extern void   flashInfo ( int *nsec, int *ssec, int **base );
extern int    flashEraseAll ( void ) ;
extern int    flashEraseSector ( int sec ) ;
extern int    flashWriteBlock ( char *dst, char *src, int len, int ovr, int chk ) ;
extern int    _flashWriteBlock(char *dst, char *src, int len, int ovr, int chk, int free_check);
extern int    flashReadBlock (char *dst, char *src, int len);
extern int    flashWriteFile  ( char *file, char *dst, int chk, int ovr ) ;
//extern int  flashWriteSRec ( char *file ) ;
extern int    flashFree ( char *adr, int len, char *scr ) ;
extern void   flashShow ( void ) ;

#else 	/* __STDC__ */

extern int flashInit () ;
extern void   flashShowInit () ;
extern void   flashInfo () ;
extern int    flashEraseAll () ;
extern int    flashEraseSector () ;
extern int    flashWriteBlock () ;
extern int    flashWriteFile () ;
//extern int  flashWriteSRec () ;
extern int    flashFree () ;
extern void   flashShow () ;

#endif	/* __STDC__ */

#endif	/* __ASSEMBLER__ */

#ifdef __cplusplus
}
#endif

#endif




