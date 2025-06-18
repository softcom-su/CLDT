#ifndef INTERFACEMKIO_H
#define INTERFACEMKIO_H

unsigned    uGetDataMKIO_X2(unsigned short usCMD, unsigned short *p_usBuf, unsigned long ulLength);                             // Функция обратного вызова, срабатывает на пришедшее КС по МКИО X2 от КК
unsigned    uGetDataMKIO_X3(unsigned short usCMD, unsigned short *p_usBuf, unsigned long ulLength);                             // Функция обратного вызова, срабатывает на пришедшее КС по МКИО X3 от КК
void        vMkioBruteWrite(unsigned short *p_usInBuf, int iCntSD, int iSubadr, int iMkioCanal, unsigned short *p_usDataMKIO);  // Функция записи данных в буфер МКИО для 2-ого формата

#endif // INTERFACEMKIO_H
