#ifndef INTERFACE_ETHERNET_H
#define INTERFACE_ETHERNET_H

int             iSendDataRK(unsigned int, unsigned int, unsigned int, unsigned char*, unsigned short);   // Функция формирования сообщения С2 и отправки данных в РК
void            vThreadGetDataRK(void *arg);                                                             // Потоковая функция открытия сокета на прослушку, получение данных от РК и отправка данных
unsigned char   ucParsePackRK(InputDataMSPO *p_stInData, MuvMemoryData *p_stMUV);                        // Функция разбора сообщения С1, С3 от РК

#endif // INTERFACE_ETHERNET_H
