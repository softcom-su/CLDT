#ifndef CRC_BCVSM_H
#define CRC_BCVSM_H

unsigned short	CRC16(unsigned char *p_ucBlock, unsigned int uiLen);	    // Подсчет КС CRC16 по таблично заданному полиному
unsigned short  CRC16_twist(unsigned char *p_ucBlock, unsigned int uiLen);  // Подсчет КС CRC16 по таблично заданному полиному
unsigned short	CRC16_2(unsigned char *p_ucBlock, unsigned int uiLen);	    // Подсчет КС CRC16 (работает медленнее чем CRC16)
unsigned 		CRC32(unsigned char *ucBuf, unsigned uLen);				    // Подсчет КС CRC32
unsigned short	CRC401(unsigned short *usBuf, unsigned uLen);			    // Подсчет КС НИО 4-01

#endif	// CRC_BCVSM_H

