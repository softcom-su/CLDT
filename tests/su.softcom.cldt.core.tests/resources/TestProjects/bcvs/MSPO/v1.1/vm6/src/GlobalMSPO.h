#ifndef GLOBAL_MSPO_H
#define GLOBAL_MSPO_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "psc.h"
#include "mram.h"
#include <math.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <public/mp/mp.h>

#include "manchester.h"			// Для работы по МКИО
#include "exptst.h"         	// Для доступа к MRAM
#include "tlmBuf.h"             // Для доступа к TLM
#include "versionMSPO.h"	    // Версия МСПО
#include "DefineMSPO.h"			// Дефайны МСПО
#include "DataMSPO.h"			// Структуры данных МСПО
#include "MyTimer.h"			// Работа с таймером 5-02-03
#include "CRC.h"				// Для расчета CRC16
#include "TransferCoord.h"  	// Для пересчета координат
#include "WorkEtalon.h"         // Для поиска эталонов
#include "InterfaceMRAM.h"      // Для работы с MRAM
#include "InterfaceMKIO.h"      // Для работы с МКИО
#include "InterfaceTLM.h"       // Для формирования и отправки ТМИ
#include "ReaderID.h"           // Чтение ИД
#include "InterfaceEthernet.h"  // Для работы с РК

#ifdef ONCE_GLOBALS
#define GLOBALS_DEF volatile
#else
#define GLOBALS_DEF extern volatile
#endif //ONCE_GLOBALS

//***Для МКИО***
GLOBALS_DEF int				g_iID_X2;	    // Идентификатор связи МКИО Х2
GLOBALS_DEF int				g_iID_X3;	    // Идентификатор связи МКИО Х3
GLOBALS_DEF unsigned char 	g_bC10;		    // Признак получения сообщения С10
//**************

GLOBALS_DEF float           g_arfStartTime[5];  // Для фиксации времени

//***Семафоры***
GLOBALS_DEF sem_t			*g_hSemGoWork;	// Семафор для выполнения режима ПП и ОР в ТЧ-80
GLOBALS_DEF sem_t			*g_hSemGetC10;	// Семафор для события по получению С10
GLOBALS_DEF pthread_mutex_t g_hMutex;
//**************

//***Глобальные переменные***
GLOBALS_DEF unsigned char	g_ucBankC11;    // Номер банка ОЗУ для C11 (0, 1)
GLOBALS_DEF unsigned int	g_uiID_TLM;		// Идентификатор кадра ТМИ
//***************************

//***Глобальные структуры данных***
GLOBALS_DEF MuvMemoryData	g_MuvData;	// Структура МУВ
GLOBALS_DEF InputDataMSPO	g_InputData;// Входные данные МСПО
GLOBALS_DEF OutputDataMSPO	g_OutData;	// Выходные данные МСПО
GLOBALS_DEF DataTLM			g_TLMdata;	// Данные для выброса в ТЛМ
//*********************************

//***Прототипы функций***
int             iLVD_PP(InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);   // ЛВД MСПО в режиме ПП
int             iLVD_OR(InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);   // ЛВД MСПО в режиме ОР
void            vThreadGetDataRK(void*);                                    // Функция потока прослушки Ethernet
void            vThreadSendTLM(void*);                                      // Функция отправки ТЛМ
extern int      netconfig();                                                // Инициализация Ethernet
extern int      rioInitAll();                                               // Инициализация RIO
extern unsigned boardCountGet();                                            // Точный таймер
//***********************
#endif	// GLOBAL_MSPO_H
