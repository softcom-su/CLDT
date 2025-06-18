#ifndef _MIPS_LOP_H
#define _MIPS_LOP_H
#define MASK_COM	    0xFC000000  /* выделяет код команды */
#define MASK_SPEC    0x0000003f  /* выделяет код специальной команды */
#define SPECIAL		    0x00000000

#define BCOND		      0x04000000  /* условная передача управления */ 

#define MAX_TRACE  25 /* максимально возможное число уровней трассировки стека */
#define MAX_SCAN  4096 /* максимальный размер поля просмотра программного кода */

/* Операция для сравнения кодов команд */
#define	CCOM(addr,com,mask)	((*addr & mask) == com)

#define MASK_JADR	 0x03ffffff  /* адрес перадачи управления по команде J или JAL  */
#define MASK_BCOND	0x001f0000  /* Для условной передачи управления */
#define MASK_BASE	 0x03e00000  /* Для базового регистра */
#define MASK_BCF	  0x03e30000  /*  */
#define MASK_BCT	  0x03e30000  /*  */
#define MASK_BREAK 0x00ff0000  /* Для номера команды break */ 

#define RA		 31                 /* номер регистра для адреса возврата */
#define _SP	 29                 /* номер регистра для указателя стека */
#define K0   26                 /* номер регистра для выхода из прерываний */ 

#define POS_BASE	21            /* смещение в битах */
#define POS_RT		 16          	 
#define POS_RS		 21 
#define POS_BREAK 16           /* смещение для номера break */	    

#define MASK_RT		(0x1f<<POS_RT)  /*  */
#define MASK_RS		(0x1f<<POS_RS)  /* высечение номера регистра */

#define	COM_JAL		   0x0c000000   /* Коды некоторых команд */

#define COM_BLTZ 		 0x00000000
#define COM_BGEZ 		 0x00010000
#define COM_BLTZL 		0x00020000
#define COM_BGEZL 		0x00030000

#define COM_BLTZAL 	0x00100000
#define COM_BGEZAL 	0x00110000
#define COM_BLTZALL 0x00120000
#define COM_BGEZALL 0x00130000


#define COM_SW		    0xac000000
#define COM_LD		    0xdc000000
#define COM_SD		    0xfc000000
#define COM_ADDIU		 0x24000000

#define	COM_JR 		   0x00000008
#define	COM_JALR 		 0x00000009
#define	COM_J 			   0x08000000
#define	COM_JAL 		  0x0c000000
#define	COM_BREAK 		0x0000000d
#define COM_LW    		0x8c000000

#define COM_BEQ 	  	0x10000000
#define COM_BNE 		  0x14000000
#define COM_BLEZ 		 0x18000000
#define COM_BGTZ 		 0x1c000000
#define COM_BEQL 		 0x50000000
#define COM_BNEL 		 0x54000000
#define COM_BLEZL 		0x58000000
#define COM_BGTZL 		0x5c000000
#define	COM_BC1F 		 0x45000000
#define	COM_BC1T 		 0x45010000
#define	COM_BC1FL 		0x45020000
#define	COM_BC1TL 		0x45030000


#endif /* _MIPS_LOP_H */
