#include "GlobalMSPO.h"			// Глобальные переменные и инклуды

/*
Обнуление регистра MRAM
Возвращаемые значения:
	int - рузельтат записи данных в MRAM
*/
int iSetNULLtoMUV()
{
    int iRet;

    unsigned int    uiRegMUV = 0x00000000;  // Считанная информация из регистра МУВ

    iRet = mramWrite(0, &uiRegMUV, MUV_ADR_REG, 4);
    printf("***MRAM NULL: Write = %i: adr = 0x%05X, size = %i, Reg = 0x%08X\n", iRet, MUV_ADR_REG, 4, uiRegMUV);

    return iRet;
}

/*
Сохранение данных в память MRAM
Возвращаемые значения:
	int - рузельтат записи данных в MRAM
*/
int iOZUtoMUV(MuvMemoryData *p_stMUV)
{
    int             iRet;
    unsigned int    uiRegMUV = 0x00000000;  // Считанная информация из регистра МУВ

    //***Прочитаем регистр МУВ***
    iRet = mramRead(0, MUV_ADR_REG, &uiRegMUV, 4);
    //printf("mramRead = %i: uiRegMUV = 0x%08X, ucOblMUV = %d\n", iRet, uiRegMUV, BIT_ISON(uiRegMUV, 0));
    //***************************

    //***Запись данных о состоянии СПО БЦ-61***
    unsigned char *p_ucDataWrite = NULL;
    p_ucDataWrite = (unsigned char*)malloc(64 * sizeof(unsigned int));

    if (p_ucDataWrite != NULL)
    {
        int l_iPos = 0;

        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fPsi, sizeof(float));
        l_iPos += sizeof(float);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fThetta, sizeof(float));
        l_iPos += sizeof(float);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fGamma, sizeof(float));
        l_iPos += sizeof(float);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fX0, sizeof(float));
        l_iPos += sizeof(float);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fY0, sizeof(float));
        l_iPos += sizeof(float);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->fZ0, sizeof(float));
        l_iPos += sizeof(float);

        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->uUS.usUS, sizeof(unsigned short));
        l_iPos += sizeof(unsigned short);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->uIS.usIS, sizeof(unsigned short));
        l_iPos += sizeof(unsigned short);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->uiUSL, sizeof(unsigned int));
        l_iPos += sizeof(unsigned int);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->uiISL, sizeof(unsigned int));
        l_iPos += sizeof(unsigned int);

        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->bTekState, sizeof(unsigned char));
        l_iPos += sizeof(unsigned char);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->bNextState, sizeof(unsigned char));
        l_iPos += sizeof(unsigned char);

        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->sRegMSPO.usRegMSPO, sizeof(unsigned short));
        l_iPos += sizeof(unsigned short);
        memcpy(p_ucDataWrite + l_iPos, &p_stMUV->uRegNorm.usRegNorm, sizeof(unsigned short));
        l_iPos += sizeof(unsigned short);

        //printf("l_iPos = %i\n", l_iPos);

        iRet = mramWrite(0, p_ucDataWrite, MUV_ADR_STATE, 64 * sizeof(unsigned int));
//            printf("STATE: Write%i = %i: adr = 0x%05X, size = %i\n", ucOblMUV, iRet, MUV_ADR_STATE_2, 64 * sizeof(unsigned int));

        if (iRet == 0)
        {
            uiRegMUV = BIT_ON(uiRegMUV, REGMUV_STATE);

            iRet = mramWrite(0, &uiRegMUV, MUV_ADR_REG, 4);
        }
    }

    if (!BIT_ISON(uiRegMUV, REGMUV_ID))    // Если ИД не записаны
    {
        if (p_stMUV->uRegNorm.usNormID == TRUE &&
            p_stMUV->sRegMSPO.usParsID == TRUE)
        {
            if (p_ucDataWrite != NULL)
            {
                free(p_ucDataWrite);
                p_ucDataWrite = NULL;
            }

            size_t stSizeID = sizeof(unsigned int) + sizeof(unsigned char) + sizeof(unsigned int) + p_stMUV->dataID.uiCountSD * 2;

            //***Выравнивание на 4***
            if (stSizeID % 4 != 0)
                stSizeID = stSizeID - (stSizeID % 4) + 4;
            //***********************

            static unsigned char ucIsWriteFullObl = 0;
            static unsigned char *p_ucDataWriteID = NULL;

            if (p_ucDataWriteID == NULL)
            {
                p_ucDataWriteID = (unsigned char*)malloc(stSizeID);

                if (p_ucDataWriteID != NULL)
                {
                    memset(p_ucDataWriteID, 0, stSizeID);    // Обнулим

                    memcpy(p_ucDataWriteID, &stSizeID, 4);
                    memcpy(p_ucDataWriteID + 4, &p_stMUV->dataID.ucTypeID, sizeof(unsigned char));
                    memcpy(p_ucDataWriteID + 5, &p_stMUV->dataID.uiCountSD, sizeof(unsigned int));
                    memcpy(p_ucDataWriteID + 9, p_stMUV->dataID.p_usSD_ID, p_stMUV->dataID.uiCountSD * 2);
                }
            }

            if (p_ucDataWriteID != NULL)
            {
                static int s_iSmeshObl = 0;

                if (ucIsWriteFullObl == 0)
                {
                    size_t stSizeID_block = stSizeID > 256 ? 256 : stSizeID;

                    if (s_iSmeshObl + stSizeID_block > stSizeID)
                        stSizeID_block = stSizeID - s_iSmeshObl;

                    iRet = mramWrite(0, p_ucDataWriteID + s_iSmeshObl, MUV_ADR_ID + s_iSmeshObl / 4, stSizeID_block);
                    s_iSmeshObl += stSizeID_block;

                    if (s_iSmeshObl == stSizeID)
                    {
                        ucIsWriteFullObl = 1;

                        if (p_ucDataWriteID != NULL)
                        {
                            free(p_ucDataWriteID);
                            p_ucDataWriteID = NULL;
                        }

                        printf("***MRAM ID_%.1f: Write = %i: adr = 0x%05X, size = %i\n", dGetTime(TYPE_MSEC), iRet, MUV_ADR_ID, stSizeID);
                    }
                }

                if (iRet == 0 && ucIsWriteFullObl == 1)
                {
                    uiRegMUV = BIT_ON(uiRegMUV, REGMUV_ID);

                    iRet = mramWrite(0, &uiRegMUV, MUV_ADR_REG, 4);
                    printf("***MRAM REG: Write = %i: adr = 0x%05X, size = %i, Reg = 0x%08X\n", iRet, MUV_ADR_REG, 4, uiRegMUV);

                    //***Сохранение информации о записи ИД в MRAM***
                    if (iRet == 0)
                        p_stMUV->sRegMSPO.usIsID_MRAM = TRUE;
                    else
                        p_stMUV->sRegMSPO.usIsID_MRAM = FALSE;
                    //**********************************************
                }
            }
        }
    }

    if (p_ucDataWrite != NULL)
    {
        free(p_ucDataWrite);
        p_ucDataWrite = NULL;
    }

    return iRet;
}

/*
Загрузка данных из памяти MRAM
Возвращаемые значения:
	int - рузельтат чтения данных из MRAM
*/
int iMUVtoOZU(MuvMemoryData *p_stMUV)
{
    int             iRet            = 0;
    unsigned int    uiRegMUV        = 0x00000000;  // Считанная информация из регистра МУВ
    unsigned char   *p_ucDataRead   = NULL;

    //***Прочитаем регистр МУВ***
    iRet = mramRead(0, MUV_ADR_REG, &uiRegMUV, 4);
    printf("***MRAM REG: mramRead = %i", iRet);
    printf(": uiRegMUV = 0x%08X\n", uiRegMUV);
    //***************************

    if (iRet == 0)
    {
        if (uiRegMUV == 0)  // Если нет никакой информации в МУВе
            return 502;

        if (BIT_ISON(uiRegMUV, REGMUV_STATE))    // Если есть данные о состоянии
        {
            p_ucDataRead = (unsigned char*)malloc(64 * sizeof(unsigned int));

            if (p_ucDataRead != NULL)
            {
                if (BIT_ISON(uiRegMUV, REGMUV_STATE))         // там есть данные, значит считаем актуальные на момент переключения
                {
                    iRet = mramRead(0, MUV_ADR_STATE, p_ucDataRead, 64 * sizeof(unsigned int));

                    printf("***MRAM STATE: mramRead = %i\n", iRet);
                }

                if (iRet == 0)  // Успешно считали данные о состоянии МСПО
                {
                    int l_iPos = 0;

                    memcpy(&p_stMUV->fPsi, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);
                    memcpy(&p_stMUV->fThetta, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);
                    memcpy(&p_stMUV->fGamma, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);
                    memcpy(&p_stMUV->fX0, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);
                    memcpy(&p_stMUV->fY0, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);
                    memcpy(&p_stMUV->fZ0, p_ucDataRead + l_iPos, sizeof(float));
                    l_iPos += sizeof(float);

                    memcpy(&p_stMUV->uUS.usUS, p_ucDataRead + l_iPos, sizeof(unsigned short));
                    l_iPos += sizeof(unsigned short);
                    memcpy(&p_stMUV->uIS.usIS, p_ucDataRead + l_iPos, sizeof(unsigned short));
                    l_iPos += sizeof(unsigned short);
                    memcpy(&p_stMUV->uiUSL, p_ucDataRead + l_iPos, sizeof(unsigned int));
                    l_iPos += sizeof(unsigned int);
                    memcpy(&p_stMUV->uiISL, p_ucDataRead + l_iPos, sizeof(unsigned int));
                    l_iPos += sizeof(unsigned int);

                    memcpy(&p_stMUV->bTekState, p_ucDataRead + l_iPos, sizeof(unsigned char));
                    l_iPos += sizeof(unsigned char);
                    memcpy(&p_stMUV->bNextState, p_ucDataRead + l_iPos, sizeof(unsigned char));
                    l_iPos += sizeof(unsigned char);

                    memcpy(&p_stMUV->sRegMSPO.usRegMSPO, p_ucDataRead + l_iPos, sizeof(unsigned short));
                    l_iPos += sizeof(unsigned short);
                    memcpy(&p_stMUV->uRegNorm.usRegNorm, p_ucDataRead + l_iPos, sizeof(unsigned short));
                    l_iPos += sizeof(unsigned short);

                    /*
                    printf("l_iPos = %i\n", l_iPos);
                    printf("p_stMUV->fPsi = %f\n", p_stMUV->fPsi);
                    printf("p_stMUV->fThetta = %f\n", p_stMUV->fThetta);
                    printf("p_stMUV->fGamma = %f\n", p_stMUV->fGamma);
                    printf("p_stMUV->fX0 = %f\n", p_stMUV->fX0);
                    printf("p_stMUV->fY0 = %f\n", p_stMUV->fY0);
                    printf("p_stMUV->fZ0 = %f\n", p_stMUV->fZ0);
                    printf("p_stMUV->usUS = %d\n", p_stMUV->uUS.usUS);
                    printf("p_stMUV->usIS = %d\n", p_stMUV->uIS.usIS);
                    printf("p_stMUV->uiUSL = %d\n", p_stMUV->uiUSL);
                    printf("p_stMUV->uiISL = %d\n", p_stMUV->uiISL);
                    printf("p_stMUV->usRegNorm = %d\n", p_stMUV->uRegNorm.usRegNorm);
                    printf("p_stMUV->usRegMSPO = %d\n", p_stMUV->sRegMSPO.usRegMSPO);
                    //*/
                }
            }
        }

        if (BIT_ISON(uiRegMUV, REGMUV_ID))       // Если есть данные БНИ/НИБ в области
        {
            static unsigned int l_uiSizeID     = 0;    // Считанный размер ИД из МУВ
            static unsigned char ucIsReadFullObl    = 0;    // Признак прочитанных всех ИД из МУВ

            if (p_ucDataRead != NULL && l_uiSizeID == 0)
            {
                free(p_ucDataRead);
                p_ucDataRead = NULL;
            }

            iRet = mramRead(0, MUV_ADR_ID, &l_uiSizeID, 4);   // Считаем размер ИД в MRAM

            if (iRet == 0)
            {
                if (p_ucDataRead == NULL)
                    p_ucDataRead = (unsigned char*)malloc(l_uiSizeID);

                if (p_ucDataRead != NULL)
                {
                    static int s_iSmeshObl = 0;

                    while (ucIsReadFullObl == 0)
                    {
                        size_t stSizeID_block = l_uiSizeID > 12288 ? 12288 : l_uiSizeID;

                        if (s_iSmeshObl + stSizeID_block > l_uiSizeID)
                            stSizeID_block = l_uiSizeID - s_iSmeshObl;

                        iRet = mramRead(0, MUV_ADR_ID + s_iSmeshObl / 4, p_ucDataRead + s_iSmeshObl, stSizeID_block);
                        s_iSmeshObl += stSizeID_block;

                        if (s_iSmeshObl == l_uiSizeID)
                        {
                            ucIsReadFullObl = 1;
                            printf("***MRAM ID: mramRead = %i, size = %i\n", iRet, l_uiSizeID);
                        }

                        pscLiveInc(0);  // Инкремент интегрированного кода годности для ВМ6
                        pscLiveInc(1);  // Инкремент интегрированного кода годности для ВМ7
                    }
                }
            }

            if (iRet == 0 && ucIsReadFullObl == 1)
            {
                if (p_stMUV->dataID.p_usSD_ID != NULL)
                {
                    free(p_stMUV->dataID.p_usSD_ID);
                    p_stMUV->dataID.p_usSD_ID = NULL;
                }

                memcpy(&p_stMUV->dataID.ucTypeID, p_ucDataRead + 4, sizeof(unsigned char));  // Считываем тип ИД
                memcpy(&p_stMUV->dataID.uiCountSD, p_ucDataRead + 5, sizeof(unsigned int));    // Считываем значимый размер данных ИД

                if (p_stMUV->dataID.ucTypeID == ID_PR)
                {
                    p_stMUV->dataID.p_usSD_ID   = (unsigned short*)malloc(p_stMUV->dataID.uiCountSD * sizeof(unsigned short)); // Резервируем память под ИД
                    p_stMUV->dataID.uiInfoCountSD    = p_stMUV->dataID.uiCountSD;                                                   // Устанавливаем кол-во СД ИД

                    if (p_stMUV->dataID.p_usSD_ID != NULL)
                        memset(p_stMUV->dataID.p_usSD_ID, 0, p_stMUV->dataID.uiCountSD * sizeof(unsigned short));
                }

                if (p_stMUV->dataID.p_usSD_ID != NULL)
                {
                    memcpy(p_stMUV->dataID.p_usSD_ID, p_ucDataRead + 9, p_stMUV->dataID.uiCountSD * sizeof(unsigned short));    // Читаем данные ИД в ОЗУ
                    uiReaderIDfromM2(p_stMUV);
                }
            }
        }
    }
    //***************************

    return iRet;
}
