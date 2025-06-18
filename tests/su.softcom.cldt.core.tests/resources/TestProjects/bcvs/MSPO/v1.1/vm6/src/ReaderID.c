#include "GlobalMSPO.h"		// Глобальные переменные

/*
Читалка ИД
Входные данные:
    *p_stMUV	- указатель на внутренню структуру МСПО
Возвращаемые значения:
    unsigned int - результат чтения ИД: stInfoID
*/
unsigned int uiReaderIDfromM2(MuvMemoryData *p_stMUV)
{
    if (p_stMUV->uRegNorm.usNormID == TRUE &&
        p_stMUV->dataID.ucTypeID == ID_PR)
    {
        p_stMUV->dataID.uiInfoParseID = 0;

        unsigned char   *p_ucID = (unsigned char*)p_stMUV->dataID.p_usSD_ID; // Откастуем к байтовому массиву
        int             l_iPos  = 0;	                                     // Позиция в массиве ИД

        memcpy(&p_stMUV->dataID.usVer, p_ucID + l_iPos, sizeof(unsigned short));
        l_iPos += sizeof(unsigned short);

        p_stMUV->dataID.stInfoID.uiKS = TRUE;

        printf("***ID: Ver = %d [", p_stMUV->dataID.usVer);
        printf("%u]\n", p_stMUV->dataID.uiCountSD);
        switch (p_stMUV->dataID.usVer)
        {
            case 69:
            {
                p_stMUV->dataID.stInfoID.uiIsVer = TRUE;    // Выставляем признак поддерживаемой версии
                vReaderID_v69(p_stMUV);                     // Читаем v69 ИД

                break;
            }

            default:
            {
                p_stMUV->dataID.stInfoID.uiIsVer = FALSE;   // Снимаем признак поддерживаемой версии

                printf("***ID_v%d: ERROR VERSION ID[", p_stMUV->dataID.usVer);
                printf("%u]\n", p_stMUV->dataID.uiCountSD);
            }
        }

        p_stMUV->sRegMSPO.usParsID = TRUE;   // Выставим бит "Попытка разбора ИД"
        //******************************************
    }

    return p_stMUV->dataID.uiInfoParseID;
}

/*
Чтение ИД версии 69
Входные данные:
    *p_stMUV	- указатель на внутренню структуру
Возвращаемые значения:
    -
*/
void vReaderID_v69(MuvMemoryData *p_stMUV)
{
    unsigned char   *p_ucID = (unsigned char*)p_stMUV->dataID.p_usSD_ID; // Откастуем к байтовому массиву
    int             l_iPos       = 2;	                                 // Позиция в массиве ИД
    int             l_iTmp       = 0;
    unsigned short  l_usTmp      = 0;


    memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
    p_stMUV->dataID.coordStart.fPhi = (float)(l_iTmp * CMR_RAD);
    l_iPos += sizeof(int);
    memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
    p_stMUV->dataID.coordStart.fLambda = (float)(l_iTmp * CMR_RAD);
    l_iPos += sizeof(int);
    memcpy(&l_usTmp, p_ucID + l_iPos, sizeof(unsigned short));
    p_stMUV->dataID.coordStart.fH = (float)l_usTmp * 0.1f;
    l_iPos += sizeof(unsigned short);
    memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
    p_stMUV->dataID.coordEnd.fPhi = (float)(l_iTmp * CMR_RAD);
    l_iPos += sizeof(int);
    memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
    p_stMUV->dataID.coordEnd.fLambda = (float)(l_iTmp * CMR_RAD);
    l_iPos += sizeof(int);
    memcpy(&l_usTmp, p_ucID + l_iPos, sizeof(unsigned short));
    p_stMUV->dataID.coordEnd.fH = (float)l_usTmp * 0.1f;
    l_iPos += sizeof(unsigned short);

    //***Расчет пеленга***
    p_stMUV->dataID.fP = 0.0f;

    bFloat_GetP(p_stMUV->dataID.coordStart.fPhi, p_stMUV->dataID.coordStart.fLambda,
                p_stMUV->dataID.coordEnd.fPhi, p_stMUV->dataID.coordEnd.fLambda,
                &p_stMUV->dataID.fP);
    //****************************

    //***Пересчет из широты, долготы в X0, Y0, Z0***
    float l_fX0 = 0.f,
          l_fY0 = 0.f,
          l_fZ0 = 0.f;

    bFloat_Geo2Zem(p_stMUV->dataID.coordStart.fPhi, p_stMUV->dataID.coordStart.fLambda, p_stMUV->dataID.fP,
                   p_stMUV->dataID.coordEnd.fPhi, p_stMUV->dataID.coordEnd.fLambda,
                   p_stMUV->dataID.coordEnd.fH,
                   &l_fX0, &l_fY0, &l_fZ0);

    p_stMUV->dataID.coordEndXYZ.fX0 = l_fX0;
    p_stMUV->dataID.coordEndXYZ.fY0 = l_fY0;
    p_stMUV->dataID.coordEndXYZ.fZ0 = l_fZ0;
    //**********************************************

    memcpy(&p_stMUV->dataID.usNoum, p_ucID + l_iPos, sizeof(unsigned short));
    l_iPos += sizeof(unsigned short);

    /*
    printf("***ID: StartPhi     = %f [%f]\n",       p_stMUV->dataID.coordStart.fPhi, RAD2GRAD(p_stMUV->dataID.coordStart.fPhi));
    printf("***ID: StartLambda  = %f [%f]\n",       p_stMUV->dataID.coordStart.fLambda, RAD2GRAD(p_stMUV->dataID.coordStart.fLambda));
    printf("***ID: StartH       = %f\n",            p_stMUV->dataID.coordStart.fH);
    printf("***ID: EndPhi       = %f [%f]\n",       p_stMUV->dataID.coordEnd.fPhi, RAD2GRAD(p_stMUV->dataID.coordEnd.fPhi));
    printf("***ID: EndLambda    = %f [%f]\n",       p_stMUV->dataID.coordEnd.fLambda, RAD2GRAD(p_stMUV->dataID.coordEnd.fLambda));
    printf("***ID: EndH         = %f\n",            p_stMUV->dataID.coordEnd.fH);
    printf("***ID: X0 = %f, Y0 = %f, Z0 = %f]\n",   p_stMUV->dataID.coordEndXYZ.fX0, p_stMUV->dataID.coordEndXYZ.fY0, p_stMUV->dataID.coordEndXYZ.fZ0);

    printf("***ID: usNoum = %d\n", p_stMUV->dataID.usNoum);
    //*/

    if (p_stMUV->dataID.usNoum > 0 && p_stMUV->dataID.usNoum <= 10)
    {
        p_stMUV->dataID.stInfoID.uiCntOUM      = TRUE;
        p_stMUV->dataID.stInfoID.uiCntEtalMN   = TRUE;

        p_stMUV->dataID.p_stOUM = NULL;
        p_stMUV->dataID.p_stOUM = (DataOUM*)malloc(p_stMUV->dataID.usNoum * sizeof(DataOUM));

        if (p_stMUV->dataID.p_stOUM != NULL)
        {
            unsigned short l_usTekOUM = 0;
            for (; l_usTekOUM < p_stMUV->dataID.usNoum; l_usTekOUM++)
            {
                memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
                p_stMUV->dataID.p_stOUM[l_usTekOUM].fPhi = (float)(l_iTmp * CMR_RAD);
                l_iPos += sizeof(int);
                memcpy(&l_iTmp, p_ucID + l_iPos, sizeof(int));
                p_stMUV->dataID.p_stOUM[l_usTekOUM].fLambda = (float)(l_iTmp * CMR_RAD);
                l_iPos += sizeof(int);
                memcpy(&l_usTmp, p_ucID + l_iPos, sizeof(unsigned short));
                p_stMUV->dataID.p_stOUM[l_usTekOUM].fH = (float)(l_usTmp) * 0.1f;
                l_iPos += sizeof(unsigned short);

                /*
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                printf("p_stMUV->dataID.p_stOUM[%d].fPhi = %f [%f]\n", l_usTekOUM, p_stMUV->dataID.p_stOUM[l_usTekOUM].fPhi, RAD2GRAD(p_stMUV->dataID.p_stOUM[l_usTekOUM].fPhi));
                printf("p_stMUV->dataID.p_stOUM[%d].fLambda = %f [%f]\n", l_usTekOUM, p_stMUV->dataID.p_stOUM[l_usTekOUM].fLambda, RAD2GRAD(p_stMUV->dataID.p_stOUM[l_usTekOUM].fLambda));
                printf("p_stMUV->dataID.p_stOUM[%d].fH = %f\n", l_usTekOUM, p_stMUV->dataID.p_stOUM[l_usTekOUM].fH);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                //*/

                memcpy(p_stMUV->dataID.p_stOUM[l_usTekOUM].ucDataImg, p_ucID + l_iPos, 2048 * sizeof(unsigned char));
                l_iPos += 2048 * sizeof(unsigned char);
            }
        }
    }
    else
        if (p_stMUV->dataID.usNoum == 0)
        {
            p_stMUV->dataID.stInfoID.uiZeroOUM = TRUE;
            p_stMUV->dataID.stInfoID.uiZeroEtalOUM = TRUE;
        }

    if ((p_stMUV->dataID.stInfoID.uiCntOUM == TRUE) &&
        (p_stMUV->dataID.stInfoID.uiCntEtalMN == TRUE))
    {
        p_stMUV->dataID.stInfoID.uiGoodRead = TRUE;
    }

    printf("***ID_v69: ResultRead = 0x%X [%d]\n",  p_stMUV->dataID.uiInfoParseID, p_stMUV->dataID.uiCountSD);
}
