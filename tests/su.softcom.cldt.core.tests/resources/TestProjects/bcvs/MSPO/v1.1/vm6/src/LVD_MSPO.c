#include "GlobalMSPO.h"		// Глобальные переменные и инклуды

//***Прототипы функций***
void    vParseC10(InputDataMSPO*, MuvMemoryData*);
int     iCreateOC11(unsigned short*, MuvMemoryData*);
int     iSendDataRK(unsigned int, unsigned int, unsigned int, unsigned char*, unsigned short);
//***********************
/*
ЛВД MСПО режима ПП
Входные данные:
	p_stInData	- структура входных данных MСПО
	p_stOutData - структура выходных данных MСПО
	p_stMUV		- структура МУВ MСПО
Возвращаемые значения:
	LVD_PP_WORK			- ЛВД MСПО в режиме ПП работает
	LVD_PP_FINISH		- ЛВД MСПО в режиме ПП завершилась
*/
int iLVD_PP(InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    //***Заблокируем мютекс***
    int l_iResMutex = pthread_mutex_lock(&g_hMutex);
    if (l_iResMutex != 0){
        printf("LVD_PP_lock: %i - [", l_iResMutex);
        printf("%s]\n", strerror(l_iResMutex));
    }
        
    //************************

    p_stMUV->dTimeStartTackt	= dGetTime(TYPE_MSEC);  // Время начала выполнения такта
    p_stMUV->dTime 				= dGetTime(TYPE_MSEC);  // Время работы МСПО
    pscLiveInc(0);                                      // Инкремент интегрированного кода годности для ВМ6
    pscLiveInc(1);                                      // Инкремент интегрированного кода годности для ВМ7

    int                 iRetLVD_PP      = LVD_PP_WORK;              // По дефолту работаем
    static int          iCountRK_OFF    = 0;
    static unsigned int uiNumC1         = 0;

    //***Проверка работоспособности РК***
    if (p_stInData->uiID_OutPacketRK_C1 != uiNumC1)
    {
        p_stMUV->uiISL  = p_stInData->uiISL;
        uiNumC1         = p_stInData->uiID_OutPacketRK_C1;
        iCountRK_OFF    = 0;
    }
    else
    {
        if (uiNumC1 > 0)    // Если получали уже сообщения С1
        {
            p_stInData->uiISL = 0;
            p_stMUV->uiISL    = p_stInData->uiISL;
            iCountRK_OFF++;
        }

        if (iCountRK_OFF > 10 &&
            p_stOutData->uiUSL == _USL0_)		// P.S. Проверяем только если находимся в самоконтроле (и только если подряд)
        {
            p_stMUV->uRegNorm.usNormRK = FALSE;	// Выставляем ненорма РК по обмену

            printf("***Error exchange ARK (iCountRK_OFF = %i) %.1f\n", iCountRK_OFF, dGetTime(TYPE_MSEC));
        }
    }
    //**********************************************

    //***Проверка инициализации БЦВС-М***
    if (p_stMUV->uRegNorm.usNormBCVSM == FALSE)
    {
        if ((p_stMUV->uInitialSPO.uiInitialSPO & INITIALSPO_OK) == INITIALSPO_OK)
            p_stMUV->uRegNorm.usNormBCVSM = TRUE;
        else{
            printf("***ERROR InitialSPO = 0x%.8X (0x", (p_stMUV->uInitialSPO.uiInitialSPO & INITIALSPO_OK));
            printf("%.8X) <-> 0x", p_stMUV->uInitialSPO.uiInitialSPO);
            printf("%.8X\n", INITIALSPO_OK);
        }
            
    }
    //*********************************

    if (p_stMUV->uUS.usUS1 == TRUE)		// Если пришел Уc1
    {
        p_stMUV->uRegNorm.usNormM3 = TRUE;	// Выставляем "норму М3"
        //printf("***LVD_PP_%.1f C01: Uk1\n", dGetTime(TYPE_MSEC));
    }

    switch (p_stMUV->uiISL)
    {
        case _ISL3:   // Если сам. завершен
        {
            if (p_stInData->uRegNormRK.uiNormRK == FALSE &&
                p_stInData->uRegNormRK.uiNormEth == FALSE)
            {
                p_stMUV->uRegNorm.usNormRK = TRUE;

                if (p_stMUV->uRegNorm.usNormM3 == TRUE)
                {
                    //***Переводим в готовность к работе***
                    p_stOutData->uiUSL          = _USL3_;
                    p_stOutData->stSizeOutBufRK = 0;
                    memset(p_stOutData->p_ucOutBufRK, 0, SIZE_UDP);
                    //*************************************
                }
            }
            else
            {
                p_stMUV->uRegNorm.usNormRK = FALSE;
            }

            break;
        }
    }

    if (p_stMUV->uRegNorm.usNormID == TRUE &&
        p_stMUV->dataID.ucTypeID == ID_PR &&
        p_stMUV->sRegMSPO.usParsID == FALSE)
    {
        unsigned int uiRetID3A222 = uiReaderIDfromM2(p_stMUV);

        if ((uiRetID3A222 & 1) == 0)
            p_stMUV->uRegNorm.usNormID = FALSE;
    }

    if (p_stMUV->uUS.usUS2 == TRUE)		// Если приходила Уc2
    {
        if (p_stMUV->uRegNorm.usNormID == TRUE &&
            p_stMUV->uRegNorm.usNormRK == TRUE &&
            p_stMUV->uiISL == _ISL6)
        {
            p_stMUV->uIS.usIS0 = TRUE;
            p_stMUV->bTekState = p_stMUV->bNextState  = 1;

            iRetLVD_PP = LVD_PP_FINISH;    // Завершаем ПП

            printf("***LVD_PP_%.1f: US2\n", dGetTime(TYPE_MSEC));
        }
        else
        {
            p_stMUV->uRegNorm.usNormRK = FALSE;
        }
    }

    if (p_stMUV->uiUSL != p_stOutData->uiUSL)
		p_stMUV->uiUSL = p_stOutData->uiUSL;

    //***Обновим данные по 10 подадресу на М3***
    p_stOutData->usBuf_OC07_10 = p_stMUV->uRegNorm.usRegNorm;	// Обновим регистр норм
    vMkioBruteWrite((unsigned short*)&p_stOutData->usBuf_OC07_10, 1, 10, MKIO_CANAL_M3, p_stOutData->usBufDataMKIO);
    //******************************************

    //###Запись данных в MRAM###
    iOZUtoMUV(p_stMUV);
    //##########################

    //###Формирование и отправка ТМИ###
    vCreateDataTMI(p_stInData, p_stOutData, p_stMUV);
    //#################################

    if (g_MuvData.sRegMSPO.usIsReboot == TRUE)   // Если переключились, то снимаем
        g_MuvData.sRegMSPO.usIsReboot = FALSE;

    p_stMUV->dTimeStopTackt = dGetTime(TYPE_MSEC);       // Время завершения такта

    //***Разблокируем мютекс***
    l_iResMutex = pthread_mutex_unlock(&g_hMutex);

    if (l_iResMutex != 0){
        printf("LVD_PP_unlock: %i - [", l_iResMutex);
        printf("%s]\n", strerror(l_iResMutex));
    }
    //************************

    return iRetLVD_PP;
}

/*
ЛВД МСПО режима ОР
Входные данные:
    p_stInData	- структура входных данных МСПО
    p_stOutData - структура выходных данных МСПО
    p_stMUV		- структура МУВ МСПО
Возвращаемые значения:
    LVD_OR_WORK      - ЛВД MСПО в режиме ОР работает
    LVD_OR_FINISH    - ЛВД MСПО в режиме ОР завершилась
*/
int iLVD_OR(InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    p_stMUV->dTimeStartTackt  = dGetTime(TYPE_MSEC);  // Зафиксируем время начала выполнения такта
    p_stMUV->dTime 			  = dGetTime(TYPE_MSEC);  // Время работы MСПО

    if (p_stMUV->sRegMSPO.usWorkOR == FALSE)           // Если заходим первый раз после ПП
    {
        p_stMUV->sRegMSPO.usWorkPP = FALSE;	          // Скидываем режим ПП
        p_stMUV->sRegMSPO.usWorkOR = TRUE;	          // Устанавливаем режим ОР
        p_stMUV->uiTypeKadrTMI     = MARKER_KADR_3;    // Устанавливаем выдачу основного кадра ОР
    }

    vWaitC10orTimeOver(g_hSemGetC10, 95.0);  // Подождем приход сообщения С10 либо 95 мс

    //***Заблокируем мютекс***
    int l_iResMutex = pthread_mutex_lock(&g_hMutex);

    if (l_iResMutex != 0){
        printf("LVD_lock: %i - [", l_iResMutex);
        printf("%s]\n", strerror(l_iResMutex));
    }
        
    //************************

    pscLiveInc(0);                                   // Инкремент интегрированного кода годности для ВМ6
    pscLiveInc(1);                                   // Инкремент интегрированного кода годности для ВМ7

    int                     iRetLVD_OR          = LVD_OR_WORK;   // По дефолту работаем
    static int              iCountRK_OFF        = 0;
    static unsigned int     uiNumC1             = 0;
    static unsigned char    s_ucUnSetUs         = 0;

    //***Проверка работоспособности РК***
    if (p_stInData->uiID_OutPacketRK_C1 != uiNumC1)
    {
        p_stMUV->uiISL    = p_stInData->uiISL;
        uiNumC1           = p_stInData->uiID_OutPacketRK_C1;
        iCountRK_OFF      = 0;

        p_stMUV->uRegNorm.usNormRK = TRUE;
    }
    else
    {
        p_stMUV->uRegNorm.usNormRK = FALSE;

        p_stInData->uiISL = 0;
        iCountRK_OFF++;

        if (iCountRK_OFF > 10)
        {
            printf("***Error exchange RK (iCountRK_OFF = %i)  %.1f\n", iCountRK_OFF, dGetTime(TYPE_MSEC));
            iCountRK_OFF = 0;
        }
    }
    //**********************************************

    if (g_bC10 == TRUE)
    {
        vParseC10(p_stInData, p_stMUV); // Разбираем С10
    }
    else
    {
        p_stMUV->uUS.usUS           = 0;        // Для обновления Ус
        p_stMUV->stPrC10.usNotC10   = TRUE;     // Устанавливаем признак неполучения сообщения С10
    }

    switch (p_stMUV->bTekState)	// Перейдем в состояние МСПО
	{
        case 1: // Основная ветка
        {
            if (p_stInData->uiISL == _ISL6)	// Если РК в ожидании команды
            {
                p_stMUV->sEtalonTrue.iRetSerch = iSerchEtalonMN(p_stMUV);

                if (p_stMUV->sEtalonTrue.iRetSerch == 1)
                {
                    printf("**LVD_%.1f: Find OUM[%i] - %i\n", dGetTime(TYPE_MSEC), p_stMUV->sEtalonTrue.ucI, p_stMUV->sEtalonTrue.ucCanSee);

                    //***Формируем данные для РК на получение И ОУМ***
                    p_stOutData->uiUSL = _USL9_;
                    p_stOutData->stSizeOutBufRK = 0;
                    memset(p_stOutData->p_ucOutBufRK, 0, SIZE_UDP);

                    memset(&p_stMUV->dataM4, 0, sizeof(M4));
                    p_stMUV->dataM4.uiPrM4 = 0;

                    p_stMUV->dataM4.usNviz = (unsigned short)p_stMUV->sEtalonTrue.ucI;
                    p_stMUV->dataM4.iX0v   = (int)(p_stMUV->sEtalonTrue.coordOUM_XYZ.fX0 / CMR_XYZ);
                    p_stMUV->dataM4.iY0v   = (int)(p_stMUV->sEtalonTrue.coordOUM_XYZ.fY0 / CMR_XYZ);
                    p_stMUV->dataM4.iZ0v   = (int)(p_stMUV->sEtalonTrue.coordOUM_XYZ.fZ0 / CMR_XYZ);

                    memcpy(p_stOutData->p_ucOutBufRK, &p_stMUV->dataM4, sizeof(M4)); // Заложим М4 на выход
                    p_stOutData->stSizeOutBufRK = sizeof(M4);			            // Размер отправляемого M4
                    //*************************************************************

                    //***Обнуление данных под изображения***
                    if (p_stMUV->sRLI.p_usRLI_Amp != NULL)
                        memset(p_stMUV->sRLI.p_usRLI_Amp, 0, RLI_SIZE * sizeof(unsigned short));

                    p_stMUV->sRLI.uStatusRLI[0].usStatusRLI = 0;    // Обнуляем информацию о полученном Амп изображении
                    p_stMUV->sRLI.uStatusRLI[1].usStatusRLI = 0;    // Обнуляем информацию о полученной матрице Thetta
                    p_stMUV->sRLI.uStatusRLI[2].usStatusRLI = 0;    // Обнуляем информацию о полученной матрице Psi
                    //******************************************

                    p_stMUV->bNextState = 2;	            // Перейдем в состояние ожидания ответа от системы 3Б222

                    //***Фиксируем время выполнения режима съемки***
                    g_arfStartTime[TIME_PUT_CMD_RLI] = dGetTime(TYPE_MSEC) + 100.0; // Время в мс
                    //**************************************************
                }

                break;
            }
            else    // Еще не вышла в ожидание команды после завершения режима, надо бы перевести в Готовность к работе
            {
                p_stOutData->uiUSL          = _USL3_;
                p_stOutData->stSizeOutBufRK = 0;
                p_stMUV->bNextState         = 1;

                break;
            }

            break;
        }

		case 2: // Проверка полученных И
        {
            if (p_stMUV->sRegMSPO.usIsReboot == FALSE)	// Если БЦВС-М не перключался
            {
                switch (p_stInData->uiISL)		// Смотрим что шлет нам
                {
                    case _ISL14:  // И готовы
                    {
                        if (p_stMUV->dataVM3.ucPrRLI == 0) // Если изображения сформированы и нам готовы их выдать
                        {
                            p_stOutData->uiUSL              = _USL11_;			// Усл на прием И
                            p_stOutData->stSizeOutBufRK     = 0;				// Отправим Усл, без М
                            memset(p_stOutData->p_ucOutBufRK, 0, SIZE_UDP);  // Обнулим
                        }
                        else
                        {
                            p_stOutData->uiUSL              = _USL3_;	// Усл на готовность к работе
                            p_stOutData->stSizeOutBufRK     = 0;        // На всякий случай, отправим Усл, без М

                            p_stMUV->bNextState = 1;
                        }

                        break;
                    }

                    case _ISL16:  // Передача элемента И
                    {
                        p_stOutData->uiUSL = _USL11_;       // Усл на прием И
                        break;
                    }

                    case _ISL17:  // Элемент И передан полностью
                    {
                        if (p_stMUV->dataVM5.ucType == RLI_TYPE_AMP)
                        {
                            if (p_stMUV->sRLI.uStatusRLI[0].usIsOK == FALSE)
                            {
                                //***Расчет КС принятого изображения (лежит у нас в ОЗУ)***
                                unsigned short l_usKS_RLI = 0xFFFF;

                                if (p_stMUV->sRLI.p_usRLI_Amp != NULL)
                                    l_usKS_RLI = CRC16((unsigned char*)p_stMUV->sRLI.p_usRLI_Amp, RLI_SIZE * sizeof(unsigned short));
                                //***************************

                                if (l_usKS_RLI == p_stMUV->dataVM5.usKS)	// Если КС совпала, то все Ок
                                {
                                    memcpy(g_TLMdata.p_usRLI_Amp + g_TLMdata.ucCntVision * RLI_SIZE, p_stMUV->sRLI.p_usRLI_Amp, RLI_SIZE * sizeof(unsigned short));

                                    g_TLMdata.stInfoRLI[g_TLMdata.ucCntVision * 3].uInfoRLI_TMI.usKS     = l_usKS_RLI;
                                    g_TLMdata.stInfoRLI[g_TLMdata.ucCntVision * 3].uInfoRLI_TMI.usPrRLI  = RLI_TYPE_AMP;

                                    p_stMUV->sRLI.uStatusRLI[0].usIsOK  = TRUE;         // Выставим корректное получение изображения
                                    p_stMUV->sRLI.usKS[0]               = l_usKS_RLI;   // Контрольная сумма
                                    p_stOutData->uiUSL                  = _USL13_;      // Усл о корректном получении И
                                }
                                else
                                {
                                    printf("***RLI[%i]: KS_RLI = 0x%4.4X <-> KS_RK = 0x%4.4X\n", p_stMUV->sRLI.uStatusRLI[0].usCountRows, l_usKS_RLI, p_stMUV->dataVM5.usKS);

                                    p_stMUV->sRLI.uStatusRLI[0].usIsOK      = FALSE;
                                    p_stMUV->sRLI.usKS[0]                   = l_usKS_RLI;
                                    p_stMUV->sRLI.uStatusRLI[0].usCountRows = 0;
                                    p_stOutData->uiUSL                      = _USL12_;	    // Выставляем: передай еще разок, не получили корректно

                                    p_stMUV->dataAS.usNviz  = p_stMUV->dataM4.usNviz + 1;
                                    p_stMUV->dataAS.uiXp    = 0;
                                    p_stMUV->dataAS.uiYp    = 0;
                                    p_stMUV->dataAS.uiNz    = 0;
                                }
                            }
                        }

                        if (p_stMUV->sRLI.uStatusRLI[0].usIsOK == TRUE) // Если получили изображение, то
                        {
                            p_stOutData->uiUSL          = _USL3_;	// Усл на готовность к работе
                            p_stOutData->stSizeOutBufRK = 0;        // На всякий случай, отправим Усл, без М

                            //*
                            printf("**LVD_%.1f: RLI Get...\n", dGetTime(TYPE_MSEC));
                            //*/

                            if (g_TLMdata.ucCntVision < 10)  // Из-за ограничения в 10 И
                                g_TLMdata.ucCntVision++;

                            p_stMUV->dataAS.usNviz  = p_stMUV->dataM4.usNviz;
                            p_stMUV->dataAS.uiXp    = 128;
                            p_stMUV->dataAS.uiYp    = 128;
                            p_stMUV->dataAS.uiNz    = 1;

                            p_stMUV->uIS.usIS1 = TRUE;     // Устанавливаем готовность данных для M3

                            p_stMUV->bNextState = 1;	     // Перейдем в основное состояние НЦ
                        }

                        break;
                    }
                }
            }
            else
            {
                p_stOutData->uiUSL          = _USL3_;	// Усл на завершение режима
                p_stOutData->stSizeOutBufRK = 0;		// На всякий случай, отправим Усл, без М

                p_stMUV->bNextState = 1;	    // Перейдем в основное состояние НЦ
            }

            break;
        }
    }

    //###Формирование СД для ОС11###
    if (g_ucBankC11 == 0)
    {
        iCreateOC11(p_stOutData->usBuf2_OC11_8, p_stMUV);
        g_ucBankC11 = 1;
    }
    else
    {
        iCreateOC11(p_stOutData->usBuf1_OC11_8, p_stMUV);
        g_ucBankC11 = 0;
    }
    //##############################

    if (p_stMUV->uiUSL != p_stOutData->uiUSL)
        p_stMUV->uiUSL = p_stOutData->uiUSL;

    //###Запись данных в MRAM###
    iOZUtoMUV(p_stMUV);
    //##########################

    //###Формирование и отправка ТМИ###
    vCreateDataTMI(p_stInData, p_stOutData, p_stMUV);
    //#################################

    //###Вывод состояния ЛВД MСПО###
    if (p_stMUV->bTekState != p_stMUV->bNextState ||
        p_stMUV->sRegMSPO.usIsReboot == TRUE)
	{
        printf("**LVD_%.1f: case %i->%i [sRegMSPO = 0x%.4X]\n", dGetTime(TYPE_MSEC), p_stMUV->bTekState, p_stMUV->bNextState, p_stMUV->sRegMSPO.usRegMSPO);
		printf("############case %i########################\n", p_stMUV->bNextState);
	}

    if (p_stMUV->sRegMSPO.usIsReboot == TRUE)   // Если переключились, то снимаем
        p_stMUV->sRegMSPO.usIsReboot = FALSE;

    p_stMUV->dTimeStopTackt = dGetTime(TYPE_MSEC);      // Зафиксируем время завершения текущего такта
	p_stMUV->bTekState		= p_stMUV->bNextState;		// Установим следующее состояние ЛВД MСПО на следующий такт

    //***Разблокируем мютекс***
    l_iResMutex = pthread_mutex_unlock(&g_hMutex);

    if (l_iResMutex != 0)
        printf("LVD_unlock: %i - [%s]\n", l_iResMutex, strerror(l_iResMutex));
    //*************************

    return iRetLVD_OR;
}

/*
Функция разбора сообщения С10 от M3
Входные параметры:
	*p_stInData	- указатель на входную структуру MСПО
	*p_stMUV	- указатель на внутреннюю структуру MСПО
Возвращаемые занчения:
	-
*/
void vParseC10(InputDataMSPO *p_stInData, MuvMemoryData *p_stMUV)
{
    switch (p_stInData->usPrizC10)
    {
        case 0x0001:    // Идут...
        {
            p_stMUV->uUS.usUS = p_stInData->usInBufMKO_C10_pr1[0];	// СД01 - Ус
            p_stMUV->usPrC10  = p_stInData->usInBufMKO_C10_pr1[1];	// СД02 - Признак состава сообщения

            int l_iPos = 2;     // Позиция СД в С10 (с СД03)
            int l_iTmp;         // Для копирования значения без ЦМР

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД03-СД04
            p_stMUV->fPsi = l_iTmp * CMR_RAD;
            l_iPos += 2;

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД05-СД06
            p_stMUV->fThetta = l_iTmp * CMR_RAD;
            l_iPos += 2;

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД07-СД08
            p_stMUV->fGamma = l_iTmp * CMR_RAD;
            l_iPos += 2;

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД09-СД10
            p_stMUV->fX0 = l_iTmp * CMR_XYZ;
            l_iPos += 2;

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД11-СД12
            p_stMUV->fY0 = l_iTmp * CMR_XYZ;
            l_iPos += 2;

            memcpy(&l_iTmp, p_stInData->usInBufMKO_C10_pr1 + l_iPos, sizeof(int));                      // СД13-СД14
            p_stMUV->fZ0 = l_iTmp * CMR_XYZ;
            l_iPos += 2;

            break;
        }

        default:
        {
            printf("***ERROR: usPrizC10 = 0x%4.4X\n", p_stInData->usPrizC10);
            break;
        }
    }

    p_stInData->usPrizC10 = 0xDEAD;

    /*
	//###Вывод расшифрованного сообщения С10###
    printf("C10: p_stMUV->usUS = %i\n", p_stMUV->uUS.usUS);
    printf("C10: p_stMUV->usPrC10 = %i\n", p_stMUV->usPrC10);
    printf("C10: p_stMUV->fPsi = %f\n", p_stMUV->fPsi);
    printf("C10: p_stMUV->fThetta = %f\n", p_stMUV->fThetta);
    printf("C10: p_stMUV->fGamma = %f\n", p_stMUV->fGamma);
    printf("C10: p_stMUV->fX0 = %f\n", p_stMUV->fX0);
    printf("C10: p_stMUV->fY0 = %f\n", p_stMUV->fY0);
    printf("C10: p_stMUV->fZ0 = %f\n", p_stMUV->fZ0);
	//##########################################
	//*/
}

/*
Функция формирования СД для сообщения ОС11
Входные данные:
	*usBuf_OC11_8 - указатель на массив СД для ОС11 для M3
	*p_stMUV	  - указатель на внутреннюю структура MСПО
Возвращаемое значение:
    CREATEOC11_TRUE		- Ответное сообщение ОС11 сфорировано
*/
int iCreateOC11(unsigned short *usBuf_OC11_8, MuvMemoryData *p_stMUV)
{
	unsigned short	l_usSD[2];	// 2 слова данных

	if (p_stMUV->uIS.usIS1 == TRUE)	// Если данные коррекции готовы
	{
		memcpy(&l_usSD, &p_stMUV->dataAS.uiXp, sizeof(unsigned int));
        usBuf_OC11_8[1] = l_usSD[0];											// СД02
        usBuf_OC11_8[2] = l_usSD[1];											// СД03
        memcpy(&l_usSD, &p_stMUV->dataAS.uiYp, sizeof(unsigned int));
        usBuf_OC11_8[3] = l_usSD[0];											// СД04
        usBuf_OC11_8[4] = l_usSD[1];											// СД05
        memcpy(&l_usSD, &p_stMUV->dataAS.uiNz, sizeof(unsigned int));
        usBuf_OC11_8[5] = l_usSD[0];											// СД06
        usBuf_OC11_8[6] = l_usSD[1];											// СД07
        usBuf_OC11_8[7] = p_stMUV->dataAS.usNviz;								// СД08

        //printf("Create OC11 done...\n");
	}
	//*****************************

    usBuf_OC11_8[0] = p_stMUV->uIS.usIS;		// СД01

    return CREATEOC11_TRUE;
}
