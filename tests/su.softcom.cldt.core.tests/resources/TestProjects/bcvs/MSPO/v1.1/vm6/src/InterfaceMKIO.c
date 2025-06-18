#include "GlobalMSPO.h"		// Глобальные переменные

/*
Функция обратного вызова, срабатывает на пришедшее КС по МКИО X2 от КК
Входные данные:
	usCMD       - КС
	*p_usBuf    - указатель на СД
	ulLength	- количество СД в КС
Возвращаемые значения:
	1 - функция отработана
*/

unsigned uGetDataMKIO_X2(unsigned short usCMD, unsigned short *p_usBuf, unsigned long ulLength)
{
    if (ulLength != 0)
    {
        printf("***uGetDataMKIO: (cmd = %i", usCMD);
        printf(", SubAdr = %i", usCMD & 0x3E0);
        printf(", length = %d)\r\n", (unsigned)ulLength);
    }
    //***Прием КС с данными БНИ/НИБ***
    if (((usCMD >> 11) & 0x1F) == 3 &&
        (((usCMD >> 5) & 0x1F) >= 2 && ((usCMD >> 5) & 0x1F) <= 30) &&
        (g_MuvData.dataID.ucTypeID == ID_PR))
	{
        static unsigned int s_uiCntSD_SubAdr = 0;       // Для сохранения предыдущего подадреса и кол-ва СД
        if ((s_uiCntSD_SubAdr & 0xFFFF) == ((usCMD >> 5) & 0x1F))   // Если второй раз шлют в тот же подадрес
            g_MuvData.dataID.uiCountSD -= ((s_uiCntSD_SubAdr >> 16) & 0xFFFF);   // Смещаемся на шаг назад, т.е. смещение будет равно на предыдущий шаг
        else
        {
            s_uiCntSD_SubAdr = (usCMD >> 5 & 0x1F); // Сохраним в какой подадрес прислали
            s_uiCntSD_SubAdr |= (ulLength << 16);   // Сохраним кол-во СД сколько прислали
        }

        if (g_MuvData.dataID.p_usSD_ID != NULL &&
            g_MuvData.dataID.uiInfoCountSD >= g_MuvData.dataID.uiCountSD + (unsigned int)ulLength)
        {
            memcpy(g_MuvData.dataID.p_usSD_ID + g_MuvData.dataID.uiCountSD, p_usBuf, ulLength * sizeof(unsigned short));
            g_MuvData.dataID.uiCountSD += (unsigned int)ulLength;
        }

        /*
        printf("g_MuvData.dataID.uiCountSD = %u\n", g_MuvData.dataID.uiCountSD);
        //*/
	}
    //********************************

	switch (usCMD)
	{
        case 7201:  // CK (ОУ = 3, Формат 2, ПодАДР = 1, СД = 1)
        {
            /*
            printf("***uGetDataMKIO_KSUS: C01 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);
            //*/

            break;
        }

        case 7234:  // CK (ОУ = 3, Формат 2, ПодАДР = 2, СД = 2)
        {
            /*
            printf("***uGetDataMKIO_KSUS: C02 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);
            //*/

            break;
        }

        case 6273:	// CK01 (ОУ = 3, ПодАДР = 4, СД = 1)
        {
            //g_bCK01 = TRUE;
            g_MuvData.dataID.ucTypeID = p_usBuf[0];
            break;
        }

        case 6179:	// CK02 (ОУ = 3, ПодАДР = 1, СД = 3)
		{
            //g_bCK02 = TRUE;

            g_MuvData.dataID.usStartSend     = p_usBuf[0];
            g_MuvData.dataID.uiInfoCountSD   = (p_usBuf[2] << 16) | p_usBuf[1];

            if (p_usBuf[0] == ID_START_SEND)	// Признак начала выдачи ИД 3А222
            {
                if (g_MuvData.dataID.p_usSD_ID != NULL)
                {
                    free(g_MuvData.dataID.p_usSD_ID);
                    g_MuvData.dataID.p_usSD_ID = NULL;
                }

                if (g_MuvData.dataID.uiInfoCountSD > 0 && g_MuvData.dataID.uiInfoCountSD * 2 <= ID_MAX_SIZE) // Если кол-во СД с ИД больше 0 и не превышает 1 Мб, то
                {
                    g_MuvData.dataID.p_usSD_ID = (unsigned short*)malloc(g_MuvData.dataID.uiInfoCountSD * sizeof(unsigned short));   // выделим память под получение ИД и
                    g_MuvData.dataID.uiCountSD = 0;                                                                                          // обнулим кол-во принятых СД

                    if (g_MuvData.dataID.p_usSD_ID == NULL)
                    {
                        g_MuvData.dataID.ucTypeID = (unsigned char)ID_MAK;    // Сбросим признак БНИ/НИБ, не смогли зарезервировать память
                        printf("***LVD_PP: ERRO malloc data BNI/NIB\n");
                    }
                }
                else    // иначе объем ИД превышает 1 Мб, значит
                {
                    g_MuvData.dataID.ucTypeID = (unsigned char)ID_MAK;    // Сбросим признак БНИ/НИБ, чтобы не вылезди за пределы массива
                    printf("***LVD_PP: WARNING BNI/NIB = %u, Max = %u\n", g_MuvData.dataID.uiInfoCountSD * 2, ID_MAX_SIZE);
                }

                printf("***LVD_PP_%.1f: Start get ID_BCVSM [0x", dGetTime(TYPE_MSEC));
                printf("%X][", g_MuvData.dataID.ucTypeID);
                printf("%u]...\n", g_MuvData.dataID.uiInfoCountSD);
            }

            break;
        }

        case 6178: // СК03 (ОУ = 3, ПодАДР = 1, СД = 2)
        {
            //g_bCK03 = TRUE;
            g_MuvData.dataID.usStopSend   = p_usBuf[0];
            g_MuvData.dataID.usKS_ID_M2   = p_usBuf[1];
            if (p_usBuf[0] == ID_STOP_SEND)	// Признак конца выдачи
            {
                //***Установим бит занятости***
                //ioctl(mkioDrvDesc.Module[1].IUDesc[1].fd, FIOCLRWRTBUF, 2);
                //manch_write_buf(g_iID_X2, 2, 0, (unsigned short*)&g_OutData.usBufX2_2, 2);
                //ioctl(mkioDrvDesc.Module[1].IUDesc[1].fd, FIOBUSYSET, 1);
                //*****************************
                g_MuvData.dataID.usKS_ID    = 0;
                g_MuvData.dataID.usKS_ID_M2 = p_usBuf[1];

                if (g_MuvData.dataID.ucTypeID == ID_PR){
                    g_MuvData.dataID.usKS_ID = CRC16_twist((unsigned char*)g_MuvData.dataID.p_usSD_ID, g_MuvData.dataID.uiCountSD * 2);
                }
                if (g_MuvData.dataID.usKS_ID == p_usBuf[1] &&  // Если КС совпала и
                    (g_MuvData.dataID.ucTypeID == ID_PR))     // тип = НИБ
                {
                    g_MuvData.uRegNorm.usNormID = TRUE;	// Выставляем "Норма загрузки БНИ/НИБ"
                    g_MuvData.dataID.usStatusID      = ID_OK;
                    g_OutData.usBufX2_2[0]           = ID_OK;
                    g_OutData.usBufX2_2[1]           = g_MuvData.dataID.usKS_ID;
                }
                else
                {
                    g_MuvData.uRegNorm.usNormID = FALSE;	// Снимаем "Норма загрузки БНИ/НИБ"
                    g_MuvData.dataID.usStatusID      = ID_ERROR;
                    g_OutData.usBufX2_2[0]           = ID_ERROR;
                    g_OutData.usBufX2_2[1]           = g_MuvData.dataID.usKS_ID;
                }

                //***Обновим данные по 2 подадресу на М2***
                vMkioBruteWrite((unsigned short*)&g_OutData.usBufX2_2, 2, 2, MKIO_CANAL_M2, NULL);
                //*****************************************

                printf("***LVD_PP_%.1f: Stop get ID_BCVSM [0x", dGetTime(TYPE_MSEC));
                printf("%X][", g_MuvData.dataID.ucTypeID);
                printf("%u]: KS = 0x", g_MuvData.dataID.uiInfoCountSD);
                printf("%.4X, KS KSUS = 0x", g_MuvData.dataID.usKS_ID);
                printf("%.4X\n", p_usBuf[1]);
            }

			break;
		}

		case 6177: // СК03 (ОУ = 3, ПодАДР = 1, СД = 1)
        {
            if (p_usBuf[0] == 0x0005)
            {
                g_OutData.uiUSL = _USL0_;
            }

            break;
        }
	}

    return 1;
}

/*
Функция обратного вызова, срабатывает на пришедшее КС по МКИО X3 от КК
Входные данные:
	usCMD       - КС
	*p_usBuf    - указатель на СД
	ulLength	- количество СД в КС
Возвращаемые значения:
	1 - функция отработана
*/
unsigned uGetDataMKIO_X3(unsigned short usCMD, unsigned short *p_usBuf, unsigned long ulLength)
{
    switch(usCMD)
	{
        case 5441:  // C07 (ОУ = 2, Формат 2, ПодАДР = 10, СД = 1)
        {
            /*
            printf("***uGetDataMKIO: C07 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);
            //*/

            break;
        }

        case 5376:  // C11 (ОУ = 2, Формат 2, ПодАДР = 8, СД = 32)
        {
            if (g_ucBankC11 == 0)
                vMkioBruteWrite((unsigned short*)&g_OutData.usBuf1_OC11_8, 32, 8, MKIO_CANAL_M3, g_OutData.usBufDataMKIO);
            else
                vMkioBruteWrite((unsigned short*)&g_OutData.usBuf2_OC11_8, 32, 8, MKIO_CANAL_M3, g_OutData.usBufDataMKIO);

            /*
            printf("***uGetDataMKIO: C11 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);
            //*/

            break;
        }

		case 4545:	// C01 (ОУ = 2, ПодАДР = 14, СД = 1)
		{
			g_MuvData.uUS.usUS = p_usBuf[0];

            //printf("***uGetDataMKIO: C01 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);
			break;
		}

        case 4385:	// C02 (ОУ = 2, ПодАДР = 9, СД = 1)
		{
			g_MuvData.uUS.usUS = p_usBuf[0];

            printf("***uGetDataMKIO: C02  (cmd = %i", usCMD);
			printf(", length = %d", (unsigned)ulLength);
            printf(") = [US1 = %i, US2 = ", g_MuvData.uUS.usUS1);
            printf("%i]\n", g_MuvData.uUS.usUS2);
            break;
		}

        case 4352:	// C10 (ОУ = 2, ПодАДР = 8, СД = 32)
		{
            if (p_usBuf[1] == 0x0001)      // Приз = 1
                memcpy(&g_InputData.usInBufMKO_C10_pr1, p_usBuf, 32 * sizeof(unsigned short));

			if (g_MuvData.sRegMSPO.usWorkOR == TRUE &&
                g_MuvData.sRegMSPO.usIsReboot == FALSE &&
                g_InputData.usPrizC10 == 0xDEAD)
            {
                g_InputData.usPrizC10 = p_usBuf[1];
                sem_post((sem_t*)g_hSemGetC10);         // Отпускаем семафор для работы ЛВД
            }

            //printf("***uGetDataMKIO: C10 (cmd = %i, length = %d)\n", usCMD, (unsigned)ulLength);

			break;
		}
	}

	return 1;
}

/*
Функция записи данных в буфер МКИО для 2-ого формата
Входные данные:
    *p_usInBuf    - указатель на записываемые данные
    iCntSD        - кол-во слов данных
    iSubadr		  - подадрес в который укладываем данные
    iMkioCanal    - магистраль на БЦВС
    *p_usDataMKIO - указатель на данных которые лежат внутри буферов контроллера МКИО
Возвращаемое значение:
    -
*/
void vMkioBruteWrite(unsigned short *p_usInBuf, int iCntSD, int iSubadr, int iMkioCanal, unsigned short *p_usDataMKIO)
{
    int iDevice = -1;	// Номер платы
    int iUnit   = -1;	// 0 - ИУМК A, 1 - ИУМК B

    switch (iMkioCanal)
    {
        case X1: iDevice = 1; iUnit = 0; break;
        case X2: iDevice = 1; iUnit = 1; break;
        case X3: iDevice = 0; iUnit = 1; break;
        case X4: iDevice = 0; iUnit = 0; break;
        default: iDevice = 0; iUnit = 0; break;
    }

    //Базовый адрес контроллера МКИО считывается из конфигурационного пространства PCI со смещением 0x10
    //Таблица 3 ЮКСУ 431295.006Д4
    unsigned uBaseAdress = pciConfigInLong (0, 3 + iDevice, 0, 0x10);

    //Слово состояния подадреса
    //Формат слова описан в таблице 30 ЮКСУ 431295.006Д4
    //Адрес слова в памяти контроллера описан в таблице 33 ЮКСУ 431295.006Д4
    unsigned short *p_usWPS = (unsigned short*)pciMemToLocal(uBaseAdress + 0x00040000 * iUnit + 4 * (iSubadr * 4 + 2));
    unsigned short usValueWPS = SWAP_US(*p_usWPS);

    //Адрес буфера передачи
    //Адрес в памяти контроллера описан в таблице 33 ЮКСУ 431295.006Д4
    unsigned short *p_usBufAdr = (unsigned short*)pciMemToLocal(uBaseAdress + 0x00040000 * iUnit + 4 * (iSubadr * 4 + 3));
    unsigned short usValueBA = SWAP_US(*p_usBufAdr);


    //Размер буфера передачи
    //Вычисляется из слова состояния подадреса согласно таблице 30 ЮКСУ 431295.006Д4
    unsigned short usBufferSize = usValueWPS & 0x1F;

    if (usBufferSize == 0)
        usBufferSize = 32;

    //Признак неактивного буфера передачи, доступного для записи
    //Вычисляется как инвертированный бит 5 (активный двойной буфер)
    //из слова состояния подадреса согласно таблице 30 ЮКСУ 431295.006Д4
    unsigned short usWriteBuffer = ((usValueWPS & 0x20) >> 5) ^ 1;

    //Адрес буфера передачи для записи в процессорном пространстве памяти
    unsigned short *p_usBuffer = (unsigned short*)pciMemToLocal(uBaseAdress + 0x00040000 * iUnit + 4 * (usValueBA + usBufferSize * usWriteBuffer));
    unsigned short *p_usBuffer2 = (unsigned short*)pciMemToLocal(uBaseAdress + 0x00040000 * iUnit + 4 * (usValueBA + usBufferSize * (usWriteBuffer ^ 1)));

    int i;
    for(i = 0; i < iCntSD; i++)
        p_usBuffer[2 * i] = SWAP_US(p_usInBuf[i]);

    *p_usWPS ^= 0x2000; // Поменяем активный буфер

    for(i = 0; i < iCntSD; i++)
        p_usBuffer2[2 * i] = SWAP_US(p_usInBuf[i]);

    if (p_usDataMKIO != NULL)
    {
        for(i = 0; i < iCntSD; i++)
        {
            p_usDataMKIO[2 * i]     = p_usBuffer[2 * i];
            p_usDataMKIO[2 * i + 1] = p_usBuffer2[2 * i];
        }
    }
}
