#include "GlobalMSPO.h"		// Глобальные переменные

/*
Отправка данных в РК
Входные параметры:
	uiID_OutPacketBCVSM	- номер исходящего пакета БУВС-М
	uiID_InPacketRK 	- номер последнего входящего пакета от РК
	uiUSL				- управляющее слово
	*pM					- передаваемый массив М
	usSizeM				- размер передаваемого массива в байтах
Возвращаемые занчения:
	-1	- некорректная работа ф-ии sendto(...)
	-2	- некорректные входные данные
	-3	- неудачная попытка создания сокета
	0	- отправка не требуется
	>0	- объем переданных данных в байтах
*/
int iSendDataRK(unsigned int uiID_OutPacketBCVSM, unsigned int uiID_InPacketRK, unsigned int uiUSL, unsigned char *pM, unsigned short usSizeM)
{
	static int                  l_iID_SockSend = -1;    // Идентификатор сокета для отправки
	static struct sockaddr_in   AddrRK;
	static struct timeval       tWaitTime;		        // Данные временного ожидания


	if (l_iID_SockSend == -1)				// Если еще не открывали сокет для отправки
	{
		l_iID_SockSend = socket(AF_INET, SOCK_DGRAM, 0);	// Открытие сокета

		if (l_iID_SockSend == -1)
			return -3;

		AddrRK.sin_len = sizeof(AddrRK);
		AddrRK.sin_family = AF_INET;
		AddrRK.sin_addr.s_addr = inet_addr(IP_RK);
		AddrRK.sin_port = htons(PORT_RK);

		//***Задаем время ожидания для select***
        tWaitTime.tv_sec    = 0;	    // Ожидание в секундах
        tWaitTime.tv_usec   = 50000;    // Ожидание в микросекундах
        //**************************************
	}

	size_t 			stSizeData	= 2 + 4 + 4 + 4 + 2 + (size_t)(usSizeM);	// Объем отправляемых данных
	unsigned short	usTypeC2	= TYPE_C2;									// Тип сообщения С2

	//***Выравнивание на 4***
	if (stSizeData % 4 != 0)
		stSizeData = stSizeData - (stSizeData % 4) + 4;
	//***********************
	unsigned char	ucData[stSizeData];										// Массив с отправляемыми данными
	unsigned short	usPosC2	= 0;											// Позиция в сообщении С2

	memcpy(ucData + usPosC2, &usTypeC2, sizeof(unsigned short));			// Тип сообщения С2
	usPosC2 += sizeof(unsigned short);
	memcpy(ucData + usPosC2, &uiID_OutPacketBCVSM, sizeof(unsigned int));	// Порядковый номер исходящего пакета
	usPosC2 += sizeof(unsigned int);
	memcpy(ucData + usPosC2, &uiID_InPacketRK, sizeof(unsigned int));
	usPosC2 += sizeof(unsigned int);
	memcpy(ucData + usPosC2, &uiUSL, sizeof(unsigned int));
	usPosC2 += sizeof(unsigned int);
	memcpy(ucData + usPosC2, &usSizeM, sizeof(unsigned short));
	usPosC2 += sizeof(unsigned short);

	if (pM != NULL && usSizeM > 0)
		memcpy(ucData + usPosC2, pM, usSizeM);

    int iRetSend = -1;

    fd_set write_set;// = {1, l_iIDsocketGetData};
    FD_ZERO(&write_set);
    FD_SET(l_iID_SockSend, &write_set);

    int iResSelect = select(l_iID_SockSend + 1, NULL, &write_set, NULL, &tWaitTime);

    if (iResSelect > 0)
    {
        iRetSend = sendto(l_iID_SockSend, (void*)&ucData, stSizeData, 0, (struct sockaddr*)&AddrRK, sizeof(AddrRK));	// Отправка данных
    }
    else
        printf("***iSendDataRK ERROR: select = %i (%s)\n", iResSelect, strerror(errno));


	return iRetSend;
}

/*
Потоковая функция открытия сокета на прослушку, получение данных от РК и отправка данных
Входные данные:
	*arg	- указтель на входные параметры (не используется)
*/
void vThreadGetDataRK(void *arg)
{
    int                 l_iIDsocketGetData	= -1;	// Дескриптор сокета
	struct sockaddr_in  l_stAddrBCVSM;
	struct timeval      l_tWaitTime;		        // Данные временного ожидания

	//***Задаем время ожидания для select***
	l_tWaitTime.tv_sec    = 0;		// Ожидание в секундах
	l_tWaitTime.tv_usec   = 50000;  // Ожидание в микросекундах
	//**************************************

	l_iIDsocketGetData = socket(AF_INET, SOCK_DGRAM, 0);	// Создаем сокет (семейство протоколов TCP/IP, протокол UDP, ...) из которого будем вычитывать

	if (l_iIDsocketGetData == -1)		// Если не сомгли создать сокет
	{
		printf("###Thread_Ethernet ERROR[%i]: OpenSocket [%s]\n", l_iIDsocketGetData, strerror(errno));
		return;
	}

	/*
    int iRetSetSockOpt; // Результат функции setsockopt()

    if((iRetSetSockOpt = setsockopt(l_iIDsocketGetData, SOL_SOCKET, SO_RCVTIMEO, &l_tWaitTime, sizeof(l_tWaitTime))) == -1)	// Установка атрибутов сокета БЦ-61, передадим ожидание
    {
        printf("###Thread_Ethernet ERROR: setsockopt[SO_RCVTIMEO] = [%s]\n", strerror(errno));
        return;
    }
    //*/

    /*
    int iRecvBuf = 32 * 1024;   // 32 Кб
    if((iRetSetSockOpt = setsockopt(l_iIDsocketGetData, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBuf, sizeof(int))) == -1)	// Установка атрибутов сокета БЦ-61, установим размер входного буфера
    {
        printf("###Thread_Ethernet ERROR: setsockopt[SO_RCVBUF] = [%s]\n", strerror(errno));
    }
    //*/

	l_stAddrBCVSM.sin_len            = sizeof(l_stAddrBCVSM);	// Размер структуры
	l_stAddrBCVSM.sin_family         = AF_INET;         		// Семейство протоколов TCP/IP
	l_stAddrBCVSM.sin_addr.s_addr    = INADDR_ANY;
	l_stAddrBCVSM.sin_port           = htons(PORT_BCVSM);

    int iRetBind;       // Результат функции bind()

	if((iRetBind = bind(l_iIDsocketGetData, (struct sockaddr *)&l_stAddrBCVSM, sizeof(l_stAddrBCVSM))) == -1)		// Устанавливаем адрес сокета
	{
        g_MuvData.uInitialSPO.uiInitThreadEth = FALSE;

        if (shutdown(l_iIDsocketGetData, SHUT_RDWR) == 0)
            close(l_iIDsocketGetData);

        printf("###Thread_Ethernet ERROR: bind = [%i][%s]\n", iRetBind, strerror(errno));
        printf("###Thread_Ethernet ERROR: Exchange RK = FALSE\n");

		return;
	}
	else
	{
        g_MuvData.uInitialSPO.uiInitThreadEth = TRUE;
        printf("***Exchange RK = Ok [%s:%u]\n", inet_ntoa(l_stAddrBCVSM.sin_addr), ntohs(l_stAddrBCVSM.sin_port));
    }

	struct sockaddr_in  l_stAddrRK;
	int                 l_istAddrRKlen = sizeof(l_stAddrRK);

	while(1)	// Чтобы из потока не вываливаться вешаем его в бесконечность
	{
        fd_set read_set;// = {1, l_iIDsocketGetData};
        FD_ZERO(&read_set);
        FD_SET(l_iIDsocketGetData, &read_set);

        int iResSelect = select(l_iIDsocketGetData + 1, &read_set, NULL, NULL, &l_tWaitTime);

        if (iResSelect > 0)
        {
            int l_iResMutex = pthread_mutex_lock(&g_hMutex);

            if (l_iResMutex != 0)
                printf("Eth_lock: %i - [%s]\n", l_iResMutex, strerror(l_iResMutex));

            g_InputData.stSizeDataRK = recvfrom(l_iIDsocketGetData, g_InputData.p_ucDataRK, SIZE_UDP, 0, (struct sockaddr *)&l_stAddrRK, &l_istAddrRKlen);

            if (g_InputData.stSizeDataRK > 0)	// Как только получили данные
            {
                memcpy((void*)&g_InputData.usTypeC, g_InputData.p_ucDataRK, sizeof(unsigned short));					// Скопируем тип сообщения от РК
                //printf("g_InputData.usTypeC = 0x%X\n", g_InputData.usTypeC);

                if (g_InputData.usTypeC == TYPE_C1)
                {
                    memcpy((void*)&g_InputData.uiID_OutPacketRK_C1, g_InputData.p_ucDataRK + 2, sizeof(unsigned int));	    // Скопируем порядклвый номер исходящего пакета РК
                    memcpy((void*)&g_InputData.uiID_InPacketBCVSM, g_InputData.p_ucDataRK + 6, sizeof(unsigned int));	            // Скопируем порядклвый номер последнего принятого сообщения от БЦ-61 системой РК
                    memcpy((void*)&g_InputData.uiISL, g_InputData.p_ucDataRK + 10, sizeof(unsigned int));			        // Скопируем информационное слово РК
                    memcpy((void*)&g_InputData.uRegNormRK.uiRegNormRK, g_InputData.p_ucDataRK + 14, sizeof(unsigned int));	// Скопируем регистр норм РК
                    memcpy((void*)&g_InputData.usSizeVM, g_InputData.p_ucDataRK + 18, sizeof(unsigned short));			        // Скопируем размер ВМ
                }
                else
                {
                    if (g_InputData.usTypeC == TYPE_C3)
                    {
                        memcpy((void*)&g_InputData.uiID_OutPacketRK_C3, g_InputData.p_ucDataRK + 2, sizeof(unsigned int));// Скопируем порядклвый номер исходящего пакета РК C3
                        memcpy((void*)&g_InputData.uiISL, g_InputData.p_ucDataRK + 6, sizeof(unsigned int));			// Скопируем информационное слово РК
                        memcpy((void*)&g_InputData.usSizeVM, g_InputData.p_ucDataRK + 10, sizeof(unsigned short));			// Скопируем размер ВМ, т.е. РЛИ
                    }
                }

                //***Разбираем сообщение С1 или С3 и отправляем С1***
                g_InputData.bIsDataRK = ucParsePackRK((InputDataMSPO*)&g_InputData, (MuvMemoryData*)&g_MuvData);		// Разберем полученный пакет и выставим признак получения данных от РК

                if (g_InputData.usTypeC == TYPE_C1 &&
                    dGetTimeCNT(TYPE_SEC) > 1.0)
                {
                    //printf("TimeCNT = %f\n", l_dTimeCNT);
                    g_OutData.uiID_OutPacketBCVSM = g_OutData.uiNumPackBCVSM++;			// Порядковый номер исходящего пакета БЦ-61 в РК
                    g_OutData.uiID_InPacketRK = g_InputData.uiID_OutPacketRK_C1;	// Порядковый номер последнего принятого сообщения от РК

                    int iRetSend = iSendDataRK(g_OutData.uiID_OutPacketBCVSM, g_OutData.uiID_InPacketRK, g_OutData.uiUSL,
                                               g_OutData.p_ucOutBufRK, g_OutData.stSizeOutBufRK);
                }
                //***************************************************

                l_iResMutex = pthread_mutex_unlock(&g_hMutex);

                if (l_iResMutex != 0)
                    printf("Eth_unlock: %i - [%s]\n", l_iResMutex, strerror(l_iResMutex));
            }
        }
	}
}

/*
Функция разбора сообщения С1, С3 от РК
Входные параметры:
	*p_stInData	- указатель на входную структуру СПО
	*p_stMUV	- указатель на внутреннюю структуру СПО
Возвращаемые занчения:
	1 - данные получены корректно
	0 - данные получены некорректно
*/
unsigned char ucParsePackRK(InputDataMSPO *p_stInData, MuvMemoryData *p_stMUV)
{
    unsigned char l_ucCorrectData = FALSE;

    switch (p_stInData->uiISL)
	{
		case _ISL1:	// Выполнение сокращенного самоконтроля
		case _ISL2:	// Выполнение расширенного самоконтроля
		case _ISL3:	// Самоконтроль завершен
		case _ISL6:	// Ожидание команды
		{
            l_ucCorrectData = TRUE;
			break;
		}

		case _ISL14:	// Визирование закончилось, формуляр РЛИ
		{
			if (p_stInData->usSizeVM > 0 && p_stInData->usTypeC == TYPE_C1)
			{
                //***Разбираем ВМ3***
				int l_iPos = C1_POS_VM;
				unsigned short  l_usTmp = 0;
				int             l_iTmp = 0;

				memcpy(&p_stMUV->dataVM3.ucPrRLI, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned char));
				l_iPos += sizeof(unsigned char);
				memcpy(&p_stMUV->dataVM3.usNviz, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned short));
				l_iPos += sizeof(unsigned short);

                /*###Вывод ВМ3###
                printf("+++++++VM3+++++++++\n");
				printf("SizeVM3 = %i [%i]\n", p_stInData->usSizeVM, g_InputData.uiID_OutPacket3B222_C1);
                printf("p_stMUV->dataVM3.ucPrRLI = 0x%.2X\n",   p_stMUV->dataVM3.ucPrRLI);
                printf("p_stMUV->dataVM3.usNviz = %d\n",        p_stMUV->dataVM3.usNviz);
				//*/

                l_ucCorrectData = TRUE;
			}
			break;
		}

		case _ISL16:	// Передача элемента И (амп)
		{
			if (p_stInData->usSizeVM > 0 && p_stInData->usTypeC == TYPE_C3)
			{
				//***Разбираем ВМ4***
				int l_iPos = C3_POS_VM;

				memcpy(&p_stMUV->dataVM4.ucType, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned char));	      // Тип изображения
				l_iPos += sizeof(unsigned char);
                memcpy(&p_stMUV->dataVM4.ucNumCluster, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned char)); // Номер кластера изображения
				l_iPos += sizeof(unsigned char);

				switch (p_stMUV->dataVM4.ucType)
				{
                    case RLI_TYPE_AMP:
                    {
                        if (p_stMUV->sRLI.p_usRLI_Amp != NULL)
                        {
                            if (p_stMUV->sRLI.uStatusRLI[0].usCountRows < 256)
                            {
                                memcpy((void*)(p_stMUV->sRLI.p_usRLI_Amp + 256 * p_stMUV->sRLI.uStatusRLI[0].usCountRows),       // Копируем строки себе в ОЗУ
                                       (void*)p_stInData->p_ucDataRK + l_iPos, 256 * RLI_CNT_ROWS_CLUSTER * sizeof(unsigned short));

                                p_stMUV->sRLI.uStatusRLI[0].usCountRows += RLI_CNT_ROWS_CLUSTER;								// Инкрементируем кол-во принятых строк

                                if (p_stMUV->dataVM4.ucNumCluster != (p_stMUV->sRLI.uStatusRLI[0].usCountRows / 2 - 1))
                                    printf("***Error: Number cluster Amp = %d (%d)\n", p_stMUV->dataVM4.ucNumCluster, (p_stMUV->sRLI.uStatusRLI[0].usCountRows / 2 - 1));
                            }
                        }

                        break;
                    }

                    default:
                        printf("***Error type RLI...\n");
				};


                /*###Вывод ВМ4###
                printf("+++++++VM4+++++++++\n");
                printf("SizeVM4 = %d\n",                        p_stInData->usSizeVM);
                printf("p_stMUV->dataVM4.ucType = %d\n",        p_stMUV->dataVM4[ucBankOZU].ucType);
                printf("p_stMUV->dataVM4.ucNumCluster = %d\n",  p_stMUV->dataVM4[ucBankOZU].ucNumCluster);
                //*/

                l_ucCorrectData = TRUE;
			}

			break;
		}

		case _ISL17:	// Элемент И передан полностью
		{
			if (p_stInData->usSizeVM > 0 && p_stInData->usTypeC == TYPE_C1)
			{
                //***Разбираем ВМ5***
				int l_iPos = C1_POS_VM;

				memcpy(&p_stMUV->dataVM5.ucType, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned char)); // Тип переданного элемента
				l_iPos += sizeof(unsigned char);
                memcpy(&p_stMUV->dataVM5.usKS, p_stInData->p_ucDataRK + l_iPos, sizeof(unsigned short));  // Контрольная сумма переданного элемента
				l_iPos += sizeof(unsigned short);

                /*###Вывод ВМ5###
                printf("+++++++VM5+++++++++\n");
				printf("SizeVM5 = %i\n",                        p_stInData->usSizeVM);
                printf("p_stMUV->dataVM5.ucType = %i\n",        p_stMUV->dataVM5[ucBankOZU].ucType);
				printf("p_stMUV->dataVM5.usKS   = 0x%4.4X\n",   p_stMUV->dataVM5[ucBankOZU].usKS);
				//*/

				//printf("Time get RLI (%u) = %f\n", p_stMUV->dataVM5.ucType, l_dStopTime - l_dStartTime);

                l_ucCorrectData = TRUE;
 			}

			break;
		}
	}

	return l_ucCorrectData;
}
