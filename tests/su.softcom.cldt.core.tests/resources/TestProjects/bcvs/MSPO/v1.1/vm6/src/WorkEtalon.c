#include "GlobalMSPO.h"

/*
Выбор эталона на МН
*/
int iSerchEtalonMN(MuvMemoryData *p_stMUV)
{
    //***Пересчет координат ОУМа в Земную СК и относительную СК***
    float l_fX0_oum = 0.f,
          l_fY0_oum = 0.f,
          l_fZ0_oum = 0.f;

    while ((unsigned short)p_stMUV->sEtalonTrue.ucI < p_stMUV->dataID.usNoum)
    {
        bFloat_Geo2Zem(p_stMUV->dataID.coordStart.fPhi, p_stMUV->dataID.coordStart.fLambda, p_stMUV->dataID.fP,
                       p_stMUV->dataID.p_stOUM[p_stMUV->sEtalonTrue.ucI].fPhi, p_stMUV->dataID.p_stOUM[p_stMUV->sEtalonTrue.ucI].fLambda, p_stMUV->dataID.p_stOUM[p_stMUV->sEtalonTrue.ucI].fH,
                       &l_fX0_oum, &l_fY0_oum, &l_fZ0_oum);

        if (l_fX0_oum - p_stMUV->fX0 > 0.0f)   // Проверка на наблюдаемость ОУМа
            break;
        else
            p_stMUV->sEtalonTrue.ucI++;
    }

    if ((unsigned short)p_stMUV->sEtalonTrue.ucI == p_stMUV->dataID.usNoum)
    {
        p_stMUV->sEtalonTrue.ucCanSee      = FALSE;
        p_stMUV->sEtalonTrue.ucIsSelect    = FALSE;

        return 0;
    }
    //*********************************

    p_stMUV->sEtalonTrue.fDnakl = sqrtf((l_fX0_oum - p_stMUV->fX0) * (l_fX0_oum - p_stMUV->fX0) +
                                         (l_fY0_oum - p_stMUV->fY0) * (l_fY0_oum - p_stMUV->fY0) +
                                         (l_fZ0_oum - p_stMUV->fZ0) * (l_fZ0_oum - p_stMUV->fZ0));

    if (p_stMUV->sEtalonTrue.fDnakl > 60000.0f)
    {
        p_stMUV->sEtalonTrue.ucCanSee      = FALSE;
        p_stMUV->sEtalonTrue.ucIsSelect    = FALSE;

        return 2;
    }

    p_stMUV->sEtalonTrue.ucIsSelect = TRUE;
    p_stMUV->sEtalonTrue.ucCanSee   = TRUE;
    p_stMUV->sEtalonTrue.ucI++;

    //***Сохраним координаты X0, Y0, Z0 ОУМа***
    p_stMUV->sEtalonTrue.coordOUM_XYZ.fX0  = l_fX0_oum;
    p_stMUV->sEtalonTrue.coordOUM_XYZ.fY0  = l_fY0_oum;
    p_stMUV->sEtalonTrue.coordOUM_XYZ.fZ0  = l_fZ0_oum;
    //*****************************************

    return 1;
}
