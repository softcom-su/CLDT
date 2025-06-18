#define ONCE_GLOBALS
#include "GlobalMSPO.h"			// Глобальные переменные и инклуды



u_short buf1[64];
u_short buf2[64];
short ks1 = 0;
short ks2 = 0;
long l1 = 0;
long l2 = 0;

void __bp1__(){

}


/*
Обработчик срабатывания таймера
Входные параметры:
	arg	- не используется, необходим параметр для функции обработчика таймера
*/
void TimeNowPP(int arg)
{
    sem_post((sem_t*)g_hSemGoWork);
}

/*
Обработчик срабатывания таймера
Входные параметры:
    arg	- не используется, необходим параметр для функции обработчика таймера
*/
void TimeNowOR(int arg)
{
    sem_post((sem_t*)g_hSemGoWork);
}

/*
Функция инициализации параметров MСПО
*/
void vInitData()		// Инициализация начальных параметров
{
    //***По умолчанию выставляем норму инициализации начальных параметров***
    g_MuvData.uInitialSPO.uiInitialSPO  = 0;        // Обнуляем все в ненорму
    g_MuvData.uInitialSPO.uiInitData    = TRUE;     // Выставляем "Корректная инициализация параметров vInitData()"
    g_MuvData.uInitialSPO.uiInitRapidIO = FALSE;    // Выставляем некорректную инициализацию RapidIO
    //**********************************************************************
    //printf("%s", "bcvsm.c:34");
    //***Инициализируем Ethernet***
    int iRetNet = netconfig();

    if (iRetNet)	// Если >0, значит ошибка
    {
		g_MuvData.uInitialSPO.uiInitEthernet = FALSE;
        printf("***ERROR init: netconfig = %i\n", iRetNet);
    }
	else
		g_MuvData.uInitialSPO.uiInitEthernet = TRUE;
    //*****************************

    //***Инициализация переменных для МКИО***
	g_iID_X2	= -1;
	g_iID_X3	= -1;

    g_bC10		= FALSE;
	//**************

	//***Семафоры***
	g_hSemGoWork	= NULL;
	g_hSemGetC10    = NULL;
	//**************

	//***Глобальные переменные***
    g_ucBankC11		= 0;
	g_uiID_TLM		= 1;
    //***************************

	//***Глобальные структуры данных***
	g_MuvData.uiTypeKadrTMI     = MARKER_KADR_1;
    g_MuvData.pExpressTest      = NULL;

    g_MuvData.sRegMSPO.usRegMSPO  = 0;
    g_MuvData.sRegMSPO.usWorkPP  = TRUE;

    g_MuvData.fPsi              = 0.0f;
    g_MuvData.fThetta           = 0.0f;
    g_MuvData.fGamma            = 0.0f;
    g_MuvData.fX0               = 0.0f;
    g_MuvData.fY0               = 0.0f;
    g_MuvData.fZ0               = 0.0f;
    g_MuvData.dTime             = 0.0;
	g_MuvData.dTimeStartTackt	= 0.0;
	g_MuvData.dTimeStopTackt	= 0.0;
	g_MuvData.uUS.usUS          = 0;
	g_MuvData.uIS.usIS          = 0;
	g_MuvData.uiUSL     		= 0;
	g_MuvData.uiISL     		= 0;
    g_MuvData.bTekState 		= 0;
    g_MuvData.bNextState 		= 0;
	g_MuvData.uRegNorm.usRegNorm	    = 0;
    g_MuvData.uRegNorm.usNormBCVSM      = TRUE;

    g_MuvData.dataID.ucTypeID            = 0;
    g_MuvData.dataID.uiCountSD           = 0;
    g_MuvData.dataID.usStartSend         = 0;
    g_MuvData.dataID.usStopSend          = 0;
    g_MuvData.dataID.uiInfoCountSD       = 0;
    g_MuvData.dataID.usKS_ID_M2          = 0;
    g_MuvData.dataID.usKS_ID             = 0;
    g_MuvData.dataID.usStatusID          = 0;
    g_MuvData.dataID.p_stOUM             = NULL;
    g_MuvData.dataID.p_usSD_ID           = NULL;

    //***Инициализация массивов М***
    memset((void*)&g_MuvData.dataM4, 0, sizeof(M4));
    //******************************

    //***Инициализация данных под изображения***
    g_MuvData.sRLI.p_usRLI_Amp = NULL;
    g_MuvData.sRLI.p_usRLI_Amp = (unsigned short*)malloc(RLI_SIZE * sizeof(unsigned short));    // Инициализируем память под Амп изображение (256 * 256 * 2 байта)

    if (g_MuvData.sRLI.p_usRLI_Amp == NULL)
    {
        g_MuvData.uInitialSPO.uiInitData = FALSE;   // Не смогли выделить память под элементы РЛИ
    }

    g_MuvData.sRLI.uStatusRLI[0].usStatusRLI = 0;    // Обнуляем информацию о полученном Амп изображении
    g_MuvData.sRLI.uStatusRLI[1].usStatusRLI = 0;
    g_MuvData.sRLI.uStatusRLI[2].usStatusRLI = 0;
    //******************************************

    g_MuvData.sEtalonTrue.ucI = 0;
    g_MuvData.dataVM4.p_sClusterImg = NULL;

	g_InputData.usNenormaC10	= 0;
	g_InputData.usPrizC10       = 0xDEAD;

    g_InputData.bIsDataRK   = FALSE;

    g_InputData.p_ucDataRK  = NULL;
    g_InputData.p_ucDataRK  = (unsigned char*)malloc(SIZE_UDP * sizeof(unsigned char));

    if (g_InputData.p_ucDataRK == NULL)
        g_MuvData.uInitialSPO.uiInitData = FALSE;

    memset(g_InputData.p_ucDataRK, 0, SIZE_UDP * sizeof(unsigned char));

    g_InputData.stSizeDataRK                    = 0;
    g_InputData.usTypeC                         = 0;
	g_InputData.uiID_OutPacketRK_C1	            = 0;
	g_InputData.uiID_OutPacketRK_C3	            = 0;
	g_InputData.uiID_InPacketBCVSM		        = 0;
	g_InputData.uiISL   				        = 0;
	g_InputData.uRegNormRK.uiRegNormRK          = 0;
	g_InputData.usSizeVM				        = 0;

    g_OutData.usBufX2_1					= ID_GET_ON;
    memset((void*)g_OutData.usBufX2_2, 0, 2 * sizeof(unsigned short));

    g_OutData.usBuf_OC07_10				= g_MuvData.uRegNorm.usRegNorm;
    memset((void*)g_OutData.usBuf1_OC11_8, 0, 32 * sizeof(unsigned short));
    memset((void*)g_OutData.usBuf2_OC11_8, 0, 32 * sizeof(unsigned short));

    g_OutData.p_ucOutBufRK  = NULL;
    g_OutData.p_ucOutBufRK  = (unsigned char*)malloc(SIZE_UDP * sizeof(unsigned char));

    if (g_OutData.p_ucOutBufRK == NULL)
        g_MuvData.uInitialSPO.uiInitData = FALSE;

    
    (g_OutData.p_ucOutBufRK, 0, SIZE_UDP * sizeof(unsigned char));

	//***Формируем массив М0, который будем отправлять как только в нас кинет пакет РК***
    g_OutData.p_ucOutBufRK[0]  = 0;      g_OutData.p_ucOutBufRK[1]  = 2;      g_OutData.p_ucOutBufRK[2]  = 4;      g_OutData.p_ucOutBufRK[3] = 8;
    g_OutData.p_ucOutBufRK[4]  = 16;     g_OutData.p_ucOutBufRK[5]  = 32;     g_OutData.p_ucOutBufRK[6]  = 64;     g_OutData.p_ucOutBufRK[7] = 128;
    g_OutData.p_ucOutBufRK[8]  = 255;	 g_OutData.p_ucOutBufRK[9]  = 0;      g_OutData.p_ucOutBufRK[10] = 2;      g_OutData.p_ucOutBufRK[11] = 4;
    g_OutData.p_ucOutBufRK[12] = 8;      g_OutData.p_ucOutBufRK[13] = 16;     g_OutData.p_ucOutBufRK[14] = 32;     g_OutData.p_ucOutBufRK[15] = 64;
    g_OutData.p_ucOutBufRK[16] = 128;	 g_OutData.p_ucOutBufRK[17] = 255;	  g_OutData.p_ucOutBufRK[18] = 0;      g_OutData.p_ucOutBufRK[19] = 2;
    g_OutData.p_ucOutBufRK[20] = 4;      g_OutData.p_ucOutBufRK[21] = 8;      g_OutData.p_ucOutBufRK[22] = 16;     g_OutData.p_ucOutBufRK[23] = 32;
    g_OutData.p_ucOutBufRK[24] = 64;     g_OutData.p_ucOutBufRK[25] = 128;	  g_OutData.p_ucOutBufRK[26] = 255;	   g_OutData.p_ucOutBufRK[27] = 0;
    g_OutData.p_ucOutBufRK[28] = 2;      g_OutData.p_ucOutBufRK[29] = 4;      g_OutData.p_ucOutBufRK[30] = 8;      g_OutData.p_ucOutBufRK[31] = 16;
    g_OutData.p_ucOutBufRK[32] = 32;     g_OutData.p_ucOutBufRK[33] = 64;

	unsigned short l_usKS_M0 = 0;	// Контрольная сумма значений М0 с 0 по 31 байт
	int iNumElem;					// Переменная для цикла суммирования

    for (iNumElem = 0; iNumElem < 34; iNumElem++)
        l_usKS_M0 += (unsigned short)g_OutData.p_ucOutBufRK[iNumElem];

    memcpy(g_OutData.p_ucOutBufRK + 34, &l_usKS_M0, sizeof(unsigned short));		// Укладываем КС в 33-34 байты
	//**************************************************************************************

	g_OutData.stSizeOutBufRK		= 36;
	g_OutData.uiID_OutPacketBCVSM	= 0;
	g_OutData.uiID_InPacketRK   	= 0;
	g_OutData.uiUSL     			= _USL0_;
	g_OutData.uiNumPackBCVSM		= 1;

	//***Инициализация памяти под ТМИ***
    g_TLMdata.stSizeDataOZU1	= 0;
    g_TLMdata.stSizeDataOZU2	= 0;
    g_TLMdata.p_ucDataOZU1      = NULL;
    g_TLMdata.p_ucDataOZU2      = NULL;
    g_TLMdata.p_ucDataOZU1      = (unsigned char*)malloc(SIZE_TLM_OZU1 * sizeof(unsigned char));
    g_TLMdata.p_ucDataOZU2      = (unsigned char*)malloc(SIZE_TLM_OZU2 * sizeof(unsigned char));

    if (g_TLMdata.p_ucDataOZU1 == NULL || g_TLMdata.p_ucDataOZU2 == NULL)
        g_MuvData.uInitialSPO.uiInitData = FALSE;

    memset(g_TLMdata.p_ucDataOZU1, 0, SIZE_TLM_OZU1 * sizeof(unsigned char));
    memset(g_TLMdata.p_ucDataOZU2, 0, SIZE_TLM_OZU2 * sizeof(unsigned char));

    g_TLMdata.ucCntVision       = 0;
    g_TLMdata.cSendVision       = 0;
    g_TLMdata.p_usRLI_Amp       = NULL;    // Амплитудное изображение суммарного канала

    g_TLMdata.p_usRLI_Amp       = (unsigned short*)malloc(10 * RLI_SIZE * sizeof(unsigned short));    // Амплитудное изображение суммарного канала

    if (g_TLMdata.p_usRLI_Amp == NULL)
        g_MuvData.uInitialSPO.uiInitData = FALSE;
    //**********************************

    //***Инициализация MRAM***
    int iRetMRAM = pscInit();		// Инициализация PSC (настраивает регистры PCI PSC)

    if (iRetMRAM == 0)
        g_MuvData.uInitialSPO.uiInitMRAM = TRUE;
    else
        printf("***ERROR init: pscInit = %i\n", iRetMRAM);
    //************************
}

/*
Основная функция СПО БЦ-61, срабатывает при загрузки образа ОС с СПО БЦ-61
*/
void* osThreadMSPO(void)
{
    vInitData();

    pscLiveInc(0);	// Инкремент интегрированного кода годности для ВМ6
    pscLiveInc(1);	// Инкремент интегрированного кода годности для ВМ7

    printf("\n##########################################################\n");
	char _VERSION_[] = "##########################################################";
	strncpy(_VERSION_ + 18, _VERSION_MSPO, strlen(_VERSION_MSPO));
	printf("%s\n", _VERSION_);
    printf("##########################################################\n");
    //printf("***TacktTimePP = %.6f s (%i Hz)\n", (float)(1.0 / TACT_PP_HZ), TACT_PP_HZ);
    //printf("***TacktTimeOR = %.6f s (%i Hz)\n", (float)(1.0 / TACT_OR_HZ), TACT_OR_HZ);

    g_MuvData.pExpressTest          = exptstGetState();     // Считаем результаты экспресс-тестов
    g_MuvData.pExpressTest->gpio    |= boardUserLatch();    // Считыем выходные защелки, там как раз и есть признак прохождения экспресс-тестов

    sTMIWRD l_stTMIWRD;
    l_stTMIWRD.uiTMIWRD = pscRead(0x1006);   // Считаем аппаратное слово состояния

    printf("***Initial BCVSM:\n");
    printf("  -g_MuvData.pExpressTest.err[0-1] = %i");
    printf("-%i\n", g_MuvData.pExpressTest->err[0], g_MuvData.pExpressTest->err[1]);
    //printf("  -sTMIWRD = 0x%8.8X (29_28 = %i, 27 = %i, 0-3 = %i)\n", l_stTMIWRD.uiTMIWRD, l_stTMIWRD.sSWITCHINF, l_stTMIWRD.sTLV, l_stTMIWRD.sPOWST);

    if (exptstGetResult() == 0) // Экспресс-тесты прошли успешно
        g_MuvData.uInitialSPO.uiGoodExpTest = TRUE;

    if (l_stTMIWRD.sSWITCHINF == 3 &&  												// Переключения ВМ не было
        (l_stTMIWRD.sPOWST == 2 || l_stTMIWRD.sPOWST == 10))						// Работает ВМ1 или ВМ2
    {
        iSetNULLtoMUV();
//        iMUVtoOZU(&g_MuvData);
    }
    else    // Иначе было переключение...
        if (l_stTMIWRD.sSWITCHINF == 0 ||   // Если переключиличь по команде из М2 или
			l_stTMIWRD.sSWITCHINF == 1 ||   // если переключиличь по неисправности СПО БЦ-61 или
            l_stTMIWRD.sSWITCHINF == 2)     // сработал датчик ДПФ
        {
            int iRetMUVtoOZU = -1;

            do
            { //Тут видимо надо загужать полетное задание (?)
                iRetMUVtoOZU = iMUVtoOZU((MuvMemoryData*)&g_MuvData);
                printf("***Load MRAM data = %i\n", iRetMUVtoOZU);
            }
            while (iRetMUVtoOZU != 0 && iRetMUVtoOZU != 502);     // Если удачно считали из МРАМ или там нет ничего, то идем дальше

            g_MuvData.uInitialSPO.uiLoadReboot  = TRUE;
            g_MuvData.sRegMSPO.usIsReboot       = TRUE;
            g_OutData.uiUSL                     = g_MuvData.uiUSL;
        }

    //***Инициализация потока Ethernet***
	pthread_t ThreadID_Ethernet;
	pthread_attr_t Attr;
  	struct sched_param Param;

    Param.sched_priority = 101;	// Приоритет потока Ethernet
	pthread_attr_init(&Attr);
	pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setschedparam(&Attr, &Param);

	//***Создаем поток для прослушки по Ethernet***
    if (pthread_create(&ThreadID_Ethernet, &Attr, (void*)&vThreadGetDataRK, NULL) != 0)
        printf("***Exchange 3B222 = FALSE\n");
    //************************************

    //***Создаем семафор***
    g_hSemGoWork = sem_open("SemGoWork", O_CREAT, 0, 0);

    if (errno != 0)
        printf("***ERROR: SEM_FAILED - %i");
        printf("[%s]\n", errno, strerror(errno));

    g_hSemGetC10 = sem_open("SemGetC10", O_CREAT, 0, 0);

    if (errno != 0)
        printf("***ERROR: SEMC10_FAILED - %i");
        printf("[%s]\n", errno, strerror(errno));
    //*********************/

    //***Инициализация мютекса по умолчанию***
    pthread_mutex_init(&g_hMutex, NULL);
    //*********************************************

    //***Инициализация ТЛМ***
    int iRetInitTMI = 1;

    iRetInitTMI = tlmBufInit(0);
    if (iRetInitTMI == 0 &&                 // Если инициализация ТЛМ прошла успешно и
        g_TLMdata.p_ucDataOZU1 != NULL &&   // инициализирована память под выходной буфер ОЗУ1 и
        g_TLMdata.p_ucDataOZU2 != NULL)     // инициализирована память под выходной буфер ОЗУ2
    {
        g_MuvData.uInitialSPO.uiInitTMI = TRUE;

        size_t  l_stSizeTLMpacketOZU1   = SIZE_TLM_OZU1;     // Рубеж отправки ТЛМ по ОЗУ1
        size_t  l_stSizeTLMpacketOZU2   = SIZE_TLM_OZU2; // Рубеж отправки ТЛМ по ОЗУ2
        //***Устанавливаем кол-во пакетов для ОЗУ1, ОЗУ2***
        size_t stCountPackOZU1 = l_stSizeTLMpacketOZU1 % 4 ? l_stSizeTLMpacketOZU1 / 4.0f + 0.5f : l_stSizeTLMpacketOZU1 / 4;
        size_t stCountPackOZU2 = l_stSizeTLMpacketOZU2 % 4 ? l_stSizeTLMpacketOZU2 / 4.0f + 0.5f : l_stSizeTLMpacketOZU2 / 4;
        tlmSetCount(0, stCountPackOZU1);   // Кол-во пакетов в ОЗУ1
        tlmSetCount(1, stCountPackOZU2);   // Кол-во пакетов в ОЗУ2
        //*************************************************
        printf("***ExchangeTLM (%i, ", stCountPackOZU1);
        printf("%i)= Ok (", stCountPackOZU2);
        printf("%u, ", SIZE_TLM_OZU1);
        printf("%u)\n", SIZE_TLM_OZU2);
    }
    else
    {
        g_MuvData.uInitialSPO.uiInitTMI = FALSE;
        printf("***ExchangeTLM = FALSE\n");
    }
	//***********************
    boardAuxClkConnect((FUNCPTR)TimeNowPP, 1); 			// Подключаем обработчик на срабатывание таймера
    boardAuxClkRateSet(TACT_PP_HZ);						// Задаем частоту срабатывания таймера (частотат описана в файле DefineBC61.h)

	//***Инициализируем таймер***
    int l_iRetTimer = -1;

    l_iRetTimer = iInitTimer(0.1f, TRUE, 0);				// Инициализация таймера с частотой в 0.1 мс

	if (l_iRetTimer == 0)
		g_MuvData.uInitialSPO.uiInitTimer = TRUE;

    printf("***Timer = %s, ", l_iRetTimer == 0 ? "Ok" : "FALSE"); // Тут по умолчанию false
    printf("%i Hz\n", iGetHzTimer());
	//***********************************

	int iDevice = -1;	// Номер платы
	int iChanel = -1;	// Номер канала

	switch (MKIO_CANAL_M2)
	{
        case X1: iDevice = 1; iChanel = 0; break;
        case X2: iDevice = 1; iChanel = 1; break;
		case X3: iDevice = 0; iChanel = 1; break;
		case X4: iDevice = 0; iChanel = 0; break;
		default: iDevice = 0; iChanel = 0; break;
	}

    g_iID_X2 = manch_open(iDevice, iChanel, MANCH_RT03);
    printf("***MKIO X%i = ", MKIO_CANAL_M2);	// Выведем состояние инициализации МКИО
    printf("%s (g_iID_X2 = ", g_iID_X2 < 0 ? "ERROR" : "Ok");
    printf("%i, Device = ", g_iID_X2);
    printf("%i, Chanel = ", iDevice);
    printf("%i)\n", iChanel);
	switch (MKIO_CANAL_M3)
	{
		case X1: iDevice = 1; iChanel = 0; break;
		case X2: iDevice = 1; iChanel = 1; break;
		case X3: iDevice = 0; iChanel = 1; break;
		case X4: iDevice = 0; iChanel = 0; break;
		default: iDevice = 0; iChanel = 0; break;
	}

    g_iID_X3 = manch_open(iDevice, iChanel, MANCH_RT02);
    printf("***MKIO X%i = ", MKIO_CANAL_M3);	// Выведем состояние инициализации МКИО
    printf("%s (g_iID_X3 = ", g_iID_X3 < 0 ? "ERROR" : "Ok");
    printf("%i, Device = ", g_iID_X3);
    printf("%i, Chanel = ", iDevice);
    printf("%i)\n", iChanel);
    manch_hint_attach(g_iID_X2, uGetDataMKIO_X2, 0);
    manch_hint_attach(g_iID_X3, uGetDataMKIO_X3, 0);

	//***Инициализируем 1-подадрес, чтобы мог СУС забрать с него данные***
    int l_iRetMWB_1 = manch_write_buf(g_iID_X2, 1, 0, (unsigned short*)&g_OutData.usBufX2_1, 1);
    vMkioBruteWrite((unsigned short*)&g_OutData.usBufX2_1, 1, 1, MKIO_CANAL_M2, NULL);
	//*********************************************************************************

	//***Инициализируем 2-подадрес, чтобы мог СУС забрать с него данные***
    int l_iRetMWB_2 = manch_write_buf(g_iID_X2, 2, 0, (unsigned short*)&g_OutData.usBufX2_2, 2);
    vMkioBruteWrite((unsigned short*)&g_OutData.usBufX2_2, 2, 2, MKIO_CANAL_M2, NULL);
    //ioctl(mkioDrvDesc.Module[1].IUDesc[1].fd, FIOBUSYSET, 1);
	//*********************************************************************************

	//***Инициализируем 10-подадрес, чтобы могли забрать с него данные сообщением С07***
    int l_iRetMWB_10 = manch_write_buf(g_iID_X3, 10, 0, (unsigned short*)&g_OutData.usBuf_OC07_10, 1);
    vMkioBruteWrite((unsigned short*)&g_OutData.usBuf_OC07_10, 1, 10, MKIO_CANAL_M3, NULL);
	//*********************************************************************************

	//***Инициализируем 8-подадрес, чтобы могли забрать с него данные сообщением С11***
    int l_iRetMWB_8 = manch_write_buf(g_iID_X3, 8, 0, (unsigned short*)&g_OutData.usBuf1_OC11_8, 32);
    vMkioBruteWrite((unsigned short*)&g_OutData.usBuf1_OC11_8, 32, 8, MKIO_CANAL_M3, NULL);
	//*********************************************************************************

    if (l_iRetMWB_1 != 0 || l_iRetMWB_2 != 0)
	{
        g_MuvData.uRegNorm.usNormBCVSM = FALSE;
        printf("***ERROR MKIO M2: MWB_1 = %i, MWB_2 = %i\n", l_iRetMWB_1, l_iRetMWB_2);
	}
    else
    {
		g_MuvData.uInitialSPO.uiInitMKIO_M2 = TRUE;
		printf("***MKIO M2 1, 2 SubAddr = Ok\n");
    }

    if (l_iRetMWB_10 != 0 || l_iRetMWB_8 != 0)
	{
        g_MuvData.uRegNorm.usNormBCVSM = FALSE;
        printf("***ERROR MKIO M3: MWB_10 = %i, MWB_8 = %i\n", l_iRetMWB_10, l_iRetMWB_8);
	}
    else
    {
		g_MuvData.uInitialSPO.uiInitMKIO_M3 = TRUE;
		printf("***MKIO M3 8, 10 SubAddr = Ok\n");
    }
	//**********************

	//###Запускаем режим ПП, т.е. работа ЛВД MСПО в режиме ПП###
    printf("++++++++++++++++++++++++++++++LVD_PP-%d = START++++++++++++++++++++++++++++++\n", boardAuxClkRateGet());

    int l_iStatusLVD_PP = LVD_PP_WORK;
    boardAuxClkEnable();
    
    while (1 && g_MuvData.sRegMSPO.usWorkPP == TRUE)	// Пока ЛВД в ПП работает, крутимся
	{
        __bp1__();
        //uGetDataMKIO_X3(ks1, buf1, l1);
        uGetDataMKIO_X2(ks2, buf2, l2);
        sem_wait((sem_t*)g_hSemGoWork);

        l_iStatusLVD_PP = iLVD_PP((InputDataMSPO*)&g_InputData, (OutputDataMSPO*)&g_OutData, (MuvMemoryData*)&g_MuvData);
        if (l_iStatusLVD_PP != LVD_PP_WORK)
            break;
	}

    boardAuxClkDisable();			// Выключаем периодический таймер
	//*
    printf("***LVD_PP_STATUS = %i\n", l_iStatusLVD_PP);
    printf("++++++++++++++++++++++++++++++LVD_PP-%d = STOP+++++++++++++++++++++++++++++++\n\n", boardAuxClkRateGet());
	//*/
	//###############################################################

    boardAuxClkConnect((FUNCPTR)TimeNowOR, 1);
    boardAuxClkRateSet(TACT_OR_HZ);

    //###Запускаем работу ЛВД МСПО в ОР###
    printf("++++++++++++++++++++++++++++++LVD_OR-%d = START++++++++++++++++++++++++++++++\n", boardAuxClkRateGet());
    printf("############case %i########################\n", g_MuvData.bNextState);

    int l_iStatusLVD_OR = LVD_OR_WORK;
    boardAuxClkEnable();						// включаем периодический таймер

    while(1)					// Бесконечная работа БЦ-61 в ТЧ-80
	{
        __bp1__();
        uGetDataMKIO_X3(ks1, buf1, 0);
        uGetDataMKIO_X2(ks2, buf2, 0);
        sem_wait((sem_t*)g_hSemGoWork);

        l_iStatusLVD_OR = iLVD_OR((InputDataMSPO*)&g_InputData, (OutputDataMSPO*)&g_OutData, (MuvMemoryData*)&g_MuvData);

		if (l_iStatusLVD_OR == LVD_OR_STOP)
			break;
	}

    boardAuxClkDisable();
    sem_close((sem_t*)g_hSemGoWork);
    printf("++++++++++++++++++++++++++++++LVD_OR-%d = STOP+++++++++++++++++++++++++++++++\n", boardAuxClkRateGet());

	return NULL;
}
