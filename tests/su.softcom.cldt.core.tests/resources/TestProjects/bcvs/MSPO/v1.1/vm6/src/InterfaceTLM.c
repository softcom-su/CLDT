#include "GlobalMSPO.h"		// Глобальные переменные

//***Статические переменные***
static short            s_sNextSendOum     = 0; // Номер отправляемого ОУМ
static unsigned char    s_ucTekData688  = 0; // 3 по 688 (3 * 688 = 2064)
//****************************

/*
Формирование ТМИ кадра 0x01010101
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stInData     - указатель на структуру входных данных МСПО
    *p_stOutData    - указатель на структуру выходных данных МСПО
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateKadr_Info_1(DataTLM *p_stDataTLM, InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    Kadr_1 l_stKadr_1;
    memset(&l_stKadr_1, 0, sizeof(Kadr_1));

    l_stKadr_1.uiMarkerKadrStart = MARKER_KADR_1;
    l_stKadr_1.fTime             = (float)p_stMUV->dTime;
    l_stKadr_1.uiIdKadr          = g_uiID_TLM++;
    l_stKadr_1.usRegNormBCVSM    = p_stMUV->uRegNorm.usRegNorm;
    l_stKadr_1.uiRegNormRK       = p_stInData->uRegNormRK.uiRegNormRK;
    l_stKadr_1.usUS              = p_stMUV->uUS.usUS;
    l_stKadr_1.usIS              = p_stMUV->uIS.usIS;
    l_stKadr_1.uiUSL             = p_stMUV->uiUSL;
    l_stKadr_1.uiISL             = p_stMUV->uiISL;

    l_stKadr_1.stDataBCVSM.uiMarkerParamBCVSM_1 = MARKER_DATA_BCVSM;

    memcpy(&l_stKadr_1.stDataBCVSM.stExpressTest, p_stMUV->pExpressTest, sizeof(State_t));
    l_stKadr_1.stDataBCVSM.uiInitialMSPO = p_stMUV->uInitialSPO.uiInitialSPO;

    //***Чтение информации о МСПО***
    memcpy(l_stKadr_1.stDataBCVSM.ucVersion, _VERSION_MSPO, 20);
    strcpy(l_stKadr_1.stDataBCVSM.ucDate,"01.01.2022");
    //***********************************

    strcpy(l_stKadr_1.stDataBCVSM.ucVersion_VM7, "ZERO");
    strcpy(l_stKadr_1.stDataBCVSM.ucDate_VM7,    "01.01.2001");
    l_stKadr_1.stDataBCVSM.ucXYp[0]             = 128;
    l_stKadr_1.stDataBCVSM.ucXYp[1]             = 128;

    l_stKadr_1.stDataBCVSM.uiID_OutPacketBCVSM    = p_stOutData->uiID_OutPacketBCVSM;
    l_stKadr_1.stDataBCVSM.uiID_InPacketRK        = p_stOutData->uiID_InPacketRK;
    l_stKadr_1.stDataBCVSM.usTypeC                = p_stInData->usTypeC;
    l_stKadr_1.stDataBCVSM.uiID_OutPacketRK_C1    = p_stInData->uiID_OutPacketRK_C1;
    l_stKadr_1.stDataBCVSM.uiID_InPacketBCVSM     = p_stInData->uiID_InPacketBCVSM;
    l_stKadr_1.stDataBCVSM.stSizeOutBufRK         = p_stOutData->stSizeOutBufRK;
    l_stKadr_1.stDataBCVSM.usSizeVM               = p_stInData->usSizeVM;
    l_stKadr_1.stDataBCVSM.stM0.uiMarkerM_1       = MARKER_M0;

    if (p_stOutData->stSizeOutBufRK > 0)
    {
        memcpy(l_stKadr_1.stDataBCVSM.stM0.ucData, p_stOutData->p_ucOutBufRK, 34 * sizeof(unsigned char));
        memcpy(&l_stKadr_1.stDataBCVSM.stM0.usKS, p_stOutData->p_ucOutBufRK + 34, sizeof(unsigned short));
    }

    l_stKadr_1.stDataBCVSM.stM0.uiMarkerM_2 = MARKER_M0;

    l_stKadr_1.stDataBCVSM.uiMarkerParamBCVSM_2 = MARKER_DATA_BCVSM;

    l_stKadr_1.uiMarkerKadrStop = MARKER_KADR_1;

    l_stKadr_1.usKSpack = CRC16((unsigned char*)(&l_stKadr_1) + 2, sizeof(Kadr_1) - 2); // Расчет КС сформированного пакета


    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU1 = sizeof(Kadr_1);                               // Размер сформированной ТМИ для ОЗУ1
    memcpy(p_stDataTLM->p_ucDataOZU1, &l_stKadr_1, g_TLMdata.stSizeDataOZU1);	// Копируем в буфер отправки данных
    //**************************************
}

/*
Формирование ТМИ кадра 0x02020202
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stInData     - указатель на структуру входных данных МСПО
    *p_stOutData    - указатель на структуру выходных данных МСПО
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateKadr_PP_2(DataTLM *p_stDataTLM, InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    Kadr_2 l_stKadr_2;
    memset(&l_stKadr_2, 0, sizeof(Kadr_2));

    l_stKadr_2.uiMarkerKadrStart = MARKER_KADR_2;
    l_stKadr_2.fTime             = (float)p_stMUV->dTime;
    l_stKadr_2.uiIdKadr          = g_uiID_TLM++;
    l_stKadr_2.usRegNormBCVSM    = p_stMUV->uRegNorm.usRegNorm;
    l_stKadr_2.uiRegNormRK       = p_stInData->uRegNormRK.uiRegNormRK;
    l_stKadr_2.usUS              = p_stMUV->uUS.usUS;
    l_stKadr_2.usIS              = p_stMUV->uIS.usIS;
    l_stKadr_2.uiUSL             = p_stMUV->uiUSL;
    l_stKadr_2.uiISL             = p_stMUV->uiISL;

    l_stKadr_2.stDataBCVSM.uiMarkerParamBCVSM_1 = MARKER_DATA_BCVSM;

    l_stKadr_2.stDataBCVSM.ucTypeID              = p_stMUV->dataID.ucTypeID;
    l_stKadr_2.stDataBCVSM.uiCountSD             = p_stMUV->dataID.uiCountSD;
    l_stKadr_2.stDataBCVSM.usStartSend           = p_stMUV->dataID.usStartSend;
    l_stKadr_2.stDataBCVSM.usStopSend            = p_stMUV->dataID.usStopSend;
    l_stKadr_2.stDataBCVSM.uiInfoCountSD         = p_stMUV->dataID.uiInfoCountSD;
    l_stKadr_2.stDataBCVSM.usKS_ID_M2            = p_stMUV->dataID.usKS_ID_M2;
    l_stKadr_2.stDataBCVSM.usKS_ID               = p_stMUV->dataID.usKS_ID;
    l_stKadr_2.stDataBCVSM.usStatusID            = p_stMUV->dataID.usStatusID;

    l_stKadr_2.stDataBCVSM.uiID_OutPacketBCVSM   = p_stOutData->uiID_OutPacketBCVSM;
    l_stKadr_2.stDataBCVSM.uiID_InPacketRK       = p_stOutData->uiID_InPacketRK;
    l_stKadr_2.stDataBCVSM.usTypeC               = p_stInData->usTypeC;
    l_stKadr_2.stDataBCVSM.uiID_OutPacketRK_C1   = p_stInData->uiID_OutPacketRK_C1;
    l_stKadr_2.stDataBCVSM.uiID_InPacketBCVSM    = p_stInData->uiID_InPacketBCVSM;
    l_stKadr_2.stDataBCVSM.stSizeOutBufRK        = p_stOutData->stSizeOutBufRK;
    l_stKadr_2.stDataBCVSM.usSizeVM              = p_stInData->usSizeVM;

    l_stKadr_2.stDataBCVSM.stM0.uiMarkerM_1 = MARKER_M0;
    if (p_stOutData->stSizeOutBufRK > 0)
    {
        memcpy(l_stKadr_2.stDataBCVSM.stM0.ucData, p_stOutData->p_ucOutBufRK, 34 * sizeof(unsigned char));
        memcpy(&l_stKadr_2.stDataBCVSM.stM0.usKS, p_stOutData->p_ucOutBufRK + 34, sizeof(unsigned short));
    }
    l_stKadr_2.stDataBCVSM.stM0.uiMarkerM_2 = MARKER_M0;

    l_stKadr_2.stDataBCVSM.stVM0.uiMarkerVM_1 = MARKER_VM0;

    l_stKadr_2.stDataBCVSM.stVM0.uiMarkerVM_2 = MARKER_VM0;

    l_stKadr_2.stDataBCVSM.uiMarkerParamBCVSM_2 = MARKER_DATA_BCVSM;

    l_stKadr_2.uiMarkerKadrStop = MARKER_KADR_2;

    l_stKadr_2.usKSpack = CRC16((unsigned char*)(&l_stKadr_2) + 2, sizeof(Kadr_2) - 2); // Расчет КС сформированного пакета

    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU1 = sizeof(Kadr_2);                               // Размер сформированной ТМИ для ОЗУ1
    memcpy(p_stDataTLM->p_ucDataOZU1, &l_stKadr_2, g_TLMdata.stSizeDataOZU1);	// Копируем в буфер отправки данных
    //**************************************
}

void vCreateKadr_OR_3(DataTLM *p_stDataTLM, InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    Kadr_3 l_stKadr_3;
    memset(&l_stKadr_3, 0, sizeof(Kadr_3));

    l_stKadr_3.uiMarkerKadrStart = MARKER_KADR_3;
    l_stKadr_3.fTime             = (float)p_stMUV->dTime;
    l_stKadr_3.uiIdKadr          = g_uiID_TLM++;
    l_stKadr_3.usRegNormBCVSM    = p_stMUV->uRegNorm.usRegNorm;
    l_stKadr_3.uiRegNormRK       = p_stInData->uRegNormRK.uiRegNormRK;
    l_stKadr_3.usUS              = p_stMUV->uUS.usUS;
    l_stKadr_3.usIS              = p_stMUV->uIS.usIS;
    l_stKadr_3.uiUSL             = p_stMUV->uiUSL;
    l_stKadr_3.uiISL             = p_stMUV->uiISL;

    l_stKadr_3.stDataBCVSM.uiMarkerParamBCVSM_1 = MARKER_DATA_BCVSM;
        l_stKadr_3.stDataBCVSM.ucTekState   = p_stMUV->bTekState;
        l_stKadr_3.stDataBCVSM.ucNextState  = p_stMUV->bNextState;
        l_stKadr_3.stDataBCVSM.usPrC10      = p_stMUV->usPrC10;
        l_stKadr_3.stDataBCVSM.fPsi         = p_stMUV->fPsi;
        l_stKadr_3.stDataBCVSM.fThetta      = p_stMUV->fThetta;
        l_stKadr_3.stDataBCVSM.fGamma       = p_stMUV->fGamma;
        l_stKadr_3.stDataBCVSM.fX0          = p_stMUV->fX0;
        l_stKadr_3.stDataBCVSM.fY0          = p_stMUV->fY0;
        l_stKadr_3.stDataBCVSM.fZ0          = p_stMUV->fZ0;
        l_stKadr_3.stDataBCVSM.usRegMSPO    = p_stMUV->sRegMSPO.usRegMSPO;

        l_stKadr_3.stDataBCVSM.ucI          = p_stMUV->sEtalonTrue.ucI;
        l_stKadr_3.stDataBCVSM.fXoum        = p_stMUV->sEtalonTrue.coordOUM_XYZ.fX0;
        l_stKadr_3.stDataBCVSM.fYoum        = p_stMUV->sEtalonTrue.coordOUM_XYZ.fY0;
        l_stKadr_3.stDataBCVSM.fZoum        = p_stMUV->sEtalonTrue.coordOUM_XYZ.fZ0;

        l_stKadr_3.stDataBCVSM.ucXp         = 128;
        l_stKadr_3.stDataBCVSM.ucYp         = 128;
        l_stKadr_3.stDataBCVSM.ucNz         = 1;
    l_stKadr_3.stDataBCVSM.uiMarkerParamBCVSM_2 = MARKER_DATA_BCVSM;

    l_stKadr_3.uiMarkerKadrStop = MARKER_KADR_3;

    l_stKadr_3.usKSpack = CRC16((unsigned char*)(&l_stKadr_3) + 2, sizeof(Kadr_3) - 2); // Расчет КС сформированного пакета

    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU1 = sizeof(Kadr_3);                               // Размер сформированной ТМИ для ОЗУ1
    memcpy(p_stDataTLM->p_ucDataOZU1, &l_stKadr_3, g_TLMdata.stSizeDataOZU1);	// Копируем в буфер отправки данных
    //**************************************
}


/*
Формирование ТМИ кадра ОЗУ №1 0xB1B1B1B1
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateKadr_1_OZU2(DataTLM *p_stDataTLM, MuvMemoryData *p_stMUV)
{
    Kadr_1_OZU2 l_stKadr_1_OZU2;
    memset(&l_stKadr_1_OZU2, 0, sizeof(Kadr_1_OZU2));

    l_stKadr_1_OZU2.uiMarkerKadrStart   = MARKER_KADR_OZU2_1;
    l_stKadr_1_OZU2.fTime               = (float)p_stMUV->dTime;
    l_stKadr_1_OZU2.uiIdKadr            = g_uiID_TLM;
    l_stKadr_1_OZU2.ucTypeID            = p_stMUV->dataID.ucTypeID;
    l_stKadr_1_OZU2.ucParsID            = p_stMUV->sRegMSPO.usParsID;
    l_stKadr_1_OZU2.uiInfoParseID       = p_stMUV->dataID.uiInfoParseID;
    l_stKadr_1_OZU2.usKS_ID             = p_stMUV->dataID.usKS_ID;

    if (p_stMUV->dataID.ucTypeID == ID_PR)
    {
        l_stKadr_1_OZU2.usKS_ID_M2      = p_stMUV->dataID.usKS_ID;
        l_stKadr_1_OZU2.usVer           = p_stMUV->dataID.usVer;
        l_stKadr_1_OZU2.usNoum          = p_stMUV->dataID.usNoum;

        if (p_stMUV->dataID.usNoum > 0 && p_stMUV->dataID.usNoum <= 10)
        {
            unsigned short l_usTekOUM = 0;
            for (; l_usTekOUM < p_stMUV->dataID.usNoum; l_usTekOUM++)
            {
                l_stKadr_1_OZU2.stOUM[l_usTekOUM].fPhi      = p_stMUV->dataID.p_stOUM[l_usTekOUM].fPhi;
                l_stKadr_1_OZU2.stOUM[l_usTekOUM].fLambda   = p_stMUV->dataID.p_stOUM[l_usTekOUM].fLambda;
                l_stKadr_1_OZU2.stOUM[l_usTekOUM].fH        = p_stMUV->dataID.p_stOUM[l_usTekOUM].fH;
            }
        }
    }

    l_stKadr_1_OZU2.uiMarkerKadrStop = MARKER_KADR_OZU2_1;

    l_stKadr_1_OZU2.usKSpack = CRC16((unsigned char*)(&l_stKadr_1_OZU2) + 2, sizeof(Kadr_1_OZU2) - 2); // Расчет КС сформированного пакета

    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU2 = sizeof(Kadr_1_OZU2);                              // Размер сформированной ТМИ для ОЗУ1
    memcpy(p_stDataTLM->p_ucDataOZU2, &l_stKadr_1_OZU2, g_TLMdata.stSizeDataOZU2);	// Копируем в буфер отправки данных
    //**************************************
}

/*
Формирование ТМИ кадра ОЗУ №2 0xB2B2B2B2
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateKadr_2_OZU2(DataTLM *p_stDataTLM, MuvMemoryData *p_stMUV)
{
    Kadr_2_OZU2 l_stKadr_2_OZU2;
    memset(&l_stKadr_2_OZU2, 0, sizeof(Kadr_2_OZU2));

    l_stKadr_2_OZU2.uiMarkerKadrStart   = MARKER_KADR_OZU2_2;
    l_stKadr_2_OZU2.fTime               = (float)p_stMUV->dTime;
    l_stKadr_2_OZU2.uiIdKadr            = g_uiID_TLM;

    short l_sTekSendOUM  = 0;

    if (s_sNextSendOum < p_stMUV->dataID.usNoum)   // Если еще не выдали все ОУМы
    {
        l_sTekSendOUM  = s_sNextSendOum;          // Порядковый номер первого ОУМ

        l_stKadr_2_OZU2.ucTekSendOUM    = (unsigned char)l_sTekSendOUM;

        if (s_ucTekData688 == 2)
        {
            memcpy(l_stKadr_2_OZU2.ucDataImg, p_stMUV->dataID.p_stOUM[l_sTekSendOUM].ucDataImg + (s_ucTekData688 * 688), 672);

            s_ucTekData688 = 0;
            l_sTekSendOUM++;
            s_sNextSendOum = l_sTekSendOUM;
        }
        else
        {
            memcpy(l_stKadr_2_OZU2.ucDataImg, p_stMUV->dataID.p_stOUM[l_sTekSendOUM].ucDataImg + (s_ucTekData688 * 688), 688);
            s_ucTekData688++;
        }

        l_stKadr_2_OZU2.ucTekData688 = s_ucTekData688;
    }

    l_stKadr_2_OZU2.uiMarkerKadrStop = MARKER_KADR_OZU2_2;

    l_stKadr_2_OZU2.usKSpack = CRC16((unsigned char*)(&l_stKadr_2_OZU2) + 2, sizeof(Kadr_2_OZU2) - 2); // Расчет КС сформированного пакета

    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU2 = sizeof(Kadr_2_OZU2);
    memcpy(p_stDataTLM->p_ucDataOZU2, &l_stKadr_2_OZU2, g_TLMdata.stSizeDataOZU2);
    //**************************************
}

/*
Формирование ТМИ кадра ОЗУ №2 0xB3B3B3B3
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateKadr_3_OZU2(DataTLM *p_stDataTLM, MuvMemoryData *p_stMUV)
{
    Kadr_3_OZU2 l_stKadr_3_OZU2;
    memset(&l_stKadr_3_OZU2, 0, sizeof(Kadr_3_OZU2));

    l_stKadr_3_OZU2.uiMarkerKadrStart   = MARKER_KADR_OZU2_3;
    l_stKadr_3_OZU2.fTime               = (float)p_stMUV->dTime;
    l_stKadr_3_OZU2.uiIdKadr            = g_uiID_TLM;

    static unsigned short   s_usNumStrRLI[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (g_TLMdata.cSendVision < 0)
        g_TLMdata.cSendVision = g_TLMdata.ucCntVision - 1;

    int l_iTekRLI = g_TLMdata.cSendVision;

    if (s_usNumStrRLI[l_iTekRLI * 3] < 256)
    {
        l_stKadr_3_OZU2.ucSendVision    = g_TLMdata.cSendVision;
        l_stKadr_3_OZU2.usKS            = g_TLMdata.stInfoRLI[l_iTekRLI * 3].uInfoRLI_TMI.usKS;
        l_stKadr_3_OZU2.usNumStrRLI     = s_usNumStrRLI[l_iTekRLI * 3];
        memcpy(l_stKadr_3_OZU2.usDataStrRLI, g_TLMdata.p_usRLI_Amp + l_iTekRLI * RLI_SIZE + s_usNumStrRLI[l_iTekRLI * 3] * 256, 256 * sizeof(short));   // 512

        s_usNumStrRLI[l_iTekRLI * 3]++;

        s_usNumStrRLI[l_iTekRLI * 3 + 1] = 256;
        s_usNumStrRLI[l_iTekRLI * 3 + 2] = 256;
        //*******************************************************************
    }

    if (s_usNumStrRLI[l_iTekRLI * 3]     == 256 &&  // Если выдали амплитудное и
        s_usNumStrRLI[l_iTekRLI * 3 + 1] == 256 &&  // матрицу
        s_usNumStrRLI[l_iTekRLI * 3 + 2] == 256)    // матрицу
    {
        s_usNumStrRLI[l_iTekRLI * 3]     = 0;   //  Занулим всех РЛИ выдаваемую строку
        s_usNumStrRLI[l_iTekRLI * 3 + 1] = 0;   //
        s_usNumStrRLI[l_iTekRLI * 3 + 2] = 0;   //

        g_TLMdata.cSendVision--;   // Выдали все РЛИ этого визирования

        /*
        printf("***TLM: Send vision - %i (send_%i, cnt_%i)\n", l_iTekRLI, g_TLMdata.ucSendVision - 1, g_TLMdata.ucCntVision);
        //*/
    }

    l_stKadr_3_OZU2.uiMarkerKadrStop = MARKER_KADR_OZU2_3;

    l_stKadr_3_OZU2.usKSpack = CRC16((unsigned char*)(&l_stKadr_3_OZU2) + 2, sizeof(Kadr_3_OZU2) - 2); // Расчет КС сформированного пакета

    //***Упаковываем на отправку***
    p_stDataTLM->stSizeDataOZU2 = sizeof(Kadr_3_OZU2);
    memcpy(p_stDataTLM->p_ucDataOZU2, &l_stKadr_3_OZU2, g_TLMdata.stSizeDataOZU2);
    //**************************************
}

/*
Формирование ТМИ кадров МСПО
Входные данные:
    *p_stDataTLM    - указатель на структуру ТМ-кадра
    *p_stInData     - указатель на структуру входных данных МСПО
    *p_stOutData    - указатель на структуру выходных данных МСПО
    *p_stMUV		- указатель на структуру МУВ МСПО
Возвращаемые значения:
    -
*/
void vCreateDataTMI(InputDataMSPO *p_stInData, OutputDataMSPO *p_stOutData, MuvMemoryData *p_stMUV)
{
    if (p_stMUV->uInitialSPO.uiInitTMI == TRUE) // Если инициализация ТМИ прошла успешно
    {
        g_TLMdata.stSizeDataOZU1 = 0;
        g_TLMdata.stSizeDataOZU2 = 0;

        //###Выбор кадра для формирования###
        switch (p_stMUV->uiTypeKadrTMI)
        {
            case MARKER_KADR_1:
            {
                vCreateKadr_Info_1((DataTLM*)&g_TLMdata, p_stInData, p_stOutData, p_stMUV);   // Будем формировать кадр 0x01010101

                if (p_stMUV->sRegMSPO.usWorkPP == TRUE)
                    p_stMUV->uiTypeKadrTMI = MARKER_KADR_2;
                else
                    p_stMUV->uiTypeKadrTMI = MARKER_KADR_3;

                break;
            }

            case MARKER_KADR_2:
            {
                vCreateKadr_PP_2((DataTLM*)&g_TLMdata, p_stInData, p_stOutData, p_stMUV);     // Будем формировать кадр 0x02020202
                break;
            }

            case MARKER_KADR_3:
            {
                vCreateKadr_OR_3((DataTLM*)&g_TLMdata, p_stInData, p_stOutData, p_stMUV);    // Будем формировать кадр 0x03030303 (М1)
                break;
            }

            default:
            {
                vCreateKadr_OR_3((DataTLM*)&g_TLMdata, p_stInData, p_stOutData, p_stMUV);    // будем формировать кадр 0x03030303
                printf("***ExchangeTLM = DEFAULT KADR_3 (USL = %i <-> ISL = %i)\n", p_stMUV->uiUSL, p_stMUV->uiISL);
            }

        }

        //***Формирование кадров ОЗУ №2***
        static unsigned short s_usStateCreateTMI_OZU2 = 1;

        switch (s_usStateCreateTMI_OZU2)
        {
            case 1:
            {
                if (p_stMUV->dataID.usStopSend == ID_STOP_SEND &&
                    p_stMUV->sRegMSPO.usParsID == TRUE)
                {
                    vCreateKadr_1_OZU2((DataTLM*)&g_TLMdata, p_stMUV);    // Будем формировать кадр 0xB1B1B1B1

                    if (p_stMUV->uRegNorm.usNormID == TRUE &&
                        p_stMUV->dataID.ucTypeID == ID_PR)
                    {
                        s_usStateCreateTMI_OZU2 = 2; // Общую информацию выдали, теперь передадим данные эталонов
                    }
                }

                break;
            }

            case 2:
            {
                vCreateKadr_2_OZU2((DataTLM*)&g_TLMdata, p_stMUV);

                if (s_sNextSendOum == p_stMUV->dataID.usNoum)
                {
                    s_usStateCreateTMI_OZU2 = 3;
                }

                break;
            }

            default:
            case 3:
            {
                if (g_TLMdata.ucCntVision > 0)
                {
                    vCreateKadr_3_OZU2((DataTLM*)&g_TLMdata, p_stMUV);    // Будем формировать кадр 0xB3B3B3B3
                }

                break;
            }
        }
        //****************************************

        // printf("***TMI: NumKadr = 0x%X (%i), UsBC60 = 0x%X, IsBC61 = 0x%X, Usl = %i, Isl = %i\n", l_uiNumKadrTMI, l_PackTLM_BC61_OZU1.uiTypeData, p_stMUV->usUS_BC60, p_stMUV->usIS_BC61, p_stMUV->uiUSL_3B222, p_stMUV->uiISL_3B222);
        // printf("***TMI: NumKadr = 0x%X (%i)\n", l_uiNumKadrTMI, l_PackTLM_BC61_OZU1.uiTypeData);
        //##################################

        //****Отправка ТМИ в ИТС-22Н***
        psi_1000h w1000h;
        psi_1001h w1001h;
        unsigned int *p_uiAdrOZU_VM6 = NULL;
        unsigned int *p_uiAdrOZU_VM7 = NULL;
        unsigned int *p_uiDataOZU1   = (unsigned int*)g_TLMdata.p_ucDataOZU1;
        unsigned int *p_uiDataOZU2   = (unsigned int*)g_TLMdata.p_ucDataOZU2;
        unsigned int uiSizeOZU1     = (unsigned)g_TLMdata.stSizeDataOZU1;
        unsigned int uiSizeOZU2     = g_TLMdata.stSizeDataOZU2 > 0 ? (unsigned)g_TLMdata.stSizeDataOZU2 : 4;

        w1000h.ui1000h = pscRead(0x1000);
        w1001h.ui1001h = pscRead(0x1001);

        w1000h.sA = (uiSizeOZU1 + 3) >> 2;
        w1001h.sB = (uiSizeOZU2 + 3) >> 2;

        pscWrite(0x1000, w1000h.ui1000h);
        pscWrite(0x1001, w1001h.ui1001h);

        p_uiAdrOZU_VM6 = &pscDesc.cpuAdr->TLM[0][0]; //Начало ОЗУ телеметрии VM6
        p_uiAdrOZU_VM7 = &pscDesc.cpuAdr->TLM[1][0]; //Начало ОЗУ телеметрии VM7

        int i;
        for (i = 0; i < ((uiSizeOZU1 + 3) >> 2); i++)
            p_uiAdrOZU_VM6[i] = CT_LE_L(p_uiDataOZU1[i]);

        if (g_TLMdata.stSizeDataOZU2 > 0)
        {
            for (i = 0; i < ((uiSizeOZU2 + 3) >> 2); i++)
                p_uiAdrOZU_VM7[i] = CT_LE_L(p_uiDataOZU2[i]);
        }

        //***Опрос готовности банков ТМИ к отправке***
        double  l_dTimeGetReadyBUF  = dGetTime(TYPE_MSEC); // Фиксируем время начала опроса банков ОЗУ в мс

        do
        {
            //*
            if (iIsTime(l_dTimeGetReadyBUF, 90.0) == 1)   // Если прошло больше 90.0 мс, выходим, нет времени ждать...
            {
                printf("***ExchangeTLM = ERROR TIME GET READY [%f]\n", dGetTime(TYPE_MSEC) - l_dTimeGetReadyBUF);

                break;
            }
            //*/

            w1000h.ui1000h = pscRead(0x1000);
        } while (w1000h.sTLMREADY == 0);
        //********************************************

        //***Инициируем отправку***
        w1000h.sTLMSTARTPCI |= 1; /*бит для запуска*/
        pscWrite(0x1000, w1000h.ui1000h);

        w1000h.ui1000h = pscRead(0x1000);
        w1000h.sTLMSTARTPCI = 0; /*бит для запуска*/
        pscWrite(0x1000, w1000h.ui1000h);
        //*************************
    }
    else
        printf("***ExchangeTLM = ERROR INIT THREAD TLM [%i]\n", p_stMUV->uInitialSPO.uiInitTMI);
}
