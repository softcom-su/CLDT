#include "GlobalMSPO.h"

//***Реализация на Float***
unsigned char Float_multiplMatrix(FloatM mA, FloatM mB, FloatM mC)
{
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
        {
            int k;
            mC[i][j] = 0.0f;
            for (k = 0; k < 3; k++)
                mC[i][j] += mA[i][k] * mB[k][j];
        }

    return 1;
}

int iMultiplyMatrix(float *pA, float *pB, float *pC, int i_x1, int i_y1, int i_x2, int i_y2)
{
    int i, k;

    if(i_x1 != i_y2)
        return 0;

    for (i = 0; i < i_y1; i++)
        for (k = 0; k < i_x2; k++)
        {
            pC[i * i_x2 + k] = 0;
            int j;

            for (j = 0; j < i_y2; j++)
                pC[i * i_x2 + k] += pA[i * i_x1 + j] * pB[j * i_x2 + k];
        }

    return 1 ;
}

unsigned char Float_transMatrix(FloatM mA, FloatM mB)
{
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            mB[j][i] = mA[i][j];

    return 1;
}

unsigned char bFloat_GetM_Sop2Zem(float fX0, float fY0, float fZ0, FloatM pM)
{
    float l_fPhiD = asin(fX0 / sqrtf(fX0 * fX0 + fY0 * fY0));
    float l_fPhiZ = atan(fZ0 / sqrtf(fX0 * fX0 + fY0 * fY0));

    FloatM mPhiD = {
                    {cosf(l_fPhiD), sinf(l_fPhiD), 0.0f},
                    {-sinf(l_fPhiD), cosf(l_fPhiD), 0.0f},
                    {0.0f, 0.0f, 1.0f}
                   };

    FloatM mPhiZ = {
                    {1.0f, 0.0f, 0.0f},
                    {0.0f, cosf(l_fPhiZ), -sinf(l_fPhiZ)},
                    {0.0f, sinf(l_fPhiZ), cosf(l_fPhiZ)}
                   };

    Float_multiplMatrix(mPhiD, mPhiZ, pM);

    return 1;
}

unsigned char bFloat_Geo2Zem(float fPhiST, float fLambdaST, float fP, float fPhi, float fLambda, float fH, float *pX0, float *pY0, float *pZ0)
{
    float fE = 1.0f - FCONST_E;
    float fE2 = fE * fE;
    float fR = FCONST_R;
    float fR2 = fR * fR;

    float l_fPhiC = atan(tan(fPhi) * fE2);		// 2.2.2

    FloatM mLambda = {{cosf(fLambda), sinf(fLambda), 0.0f},		// 2.2.3
                      {-sinf(fLambda), cosf(fLambda), 0.0f},
                      {0.0f, 0.0f, 1.0f}};

    FloatM mPhi = {{cosf(l_fPhiC), 0.0f, sinf(l_fPhiC)},		// 2.2.4
                   {0.0f, 1.0f, 0.0f},
                   {-sinf(l_fPhiC), 0.0f, cosf(l_fPhiC)}};

    FloatM mA1, mA1_T;

    Float_multiplMatrix(mPhi, mLambda, mA1);					// 2.2.5
    Float_transMatrix(mA1, mA1_T);

    float l_fRz = fR2 * fE / sqrtf(fR2 * fE2 * cosf(l_fPhiC) * cosf(l_fPhiC) + fR2 * sinf(l_fPhiC) * sinf(l_fPhiC));	// 2.2.6

    float l_fXc = mA1_T[0][0] * (l_fRz + fH);			// 2.2.7
    float l_fYc = mA1_T[1][0] * (l_fRz + fH);
    float l_fZc = mA1_T[2][0] * (l_fRz + fH);

    float l_fPhi_stc = atan(tan(fPhiST) * fE2);		// 2.2.8

    FloatM mLambdaST = {{cosf(fLambdaST), sinf(fLambdaST), 0.0f},	// 2.2.9
                        {-sinf(fLambdaST), cosf(fLambdaST), 0.0f},
                        {0.0f, 0.0f, 1.0f}};

    FloatM mPhiST = {{cosf(l_fPhi_stc), 0.0f, sinf(l_fPhi_stc)},	// 2.2.10
                     {0.0f, 1.0f, 0.0f},
                     {-sinf(l_fPhi_stc), 0.0f, cosf(l_fPhi_stc)}};

    FloatM mP = {{1.0f, 0.0f, 0.0f},								// 2.2.11
                 {0.0f, cosf(fP), -sinf(fP)},
                 {0.0f, sinf(fP), cosf(fP)}};

    Float_multiplMatrix(mP, mPhiST, mA1);					// 2.2.12
    FloatM mA1_PPL;
    Float_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.2.12

    //FloatM m0 = {{0.0f, 0.0f, -1.0f},
    //             {1.0f, 0.0f, 0.0f},
    //             {0.0f, -1.0f, 0.0f}};
    FloatM m0 = {{0.0f, 0.0f, 1.0f},
                 {1.0f, 0.0f, 0.0f},
                 {0.0f, 1.0f, 0.0f}};

    FloatM mXYZ;

    Float_multiplMatrix(m0, mA1_PPL, mXYZ);

    *pX0 = mXYZ[0][0] * l_fXc + mXYZ[0][1] * l_fYc + mXYZ[0][2] * l_fZc;
    *pY0 = mXYZ[1][0] * l_fXc + mXYZ[1][1] * l_fYc + mXYZ[1][2] * l_fZc;
    *pZ0 = mXYZ[2][0] * l_fXc + mXYZ[2][1] * l_fYc + mXYZ[2][2] * l_fZc;

    return 1;
}

unsigned char bFloat_Geo2Zem_2(float fPhiST, float fLambdaST, float fPhi, float fLambda, float fH, float *pX0, float *pY0, float *pZ0)
{
    float fP = 0.0f;
    bFloat_GetP(fPhiST, fLambdaST, fPhi, fLambda, &fP);

    float fE = 1.0f - FCONST_E;
    float fE2 = fE * fE;
    float fR = FCONST_R;
    float fR2 = fR * fR;

    float l_fPhiC = atan(tan(fPhi) * fE2);		// 2.2.2

    FloatM mLambda = {{cosf(fLambda), sinf(fLambda), 0.0f},		// 2.2.3
                      {-sinf(fLambda), cosf(fLambda), 0.0f},
                      {0.0f, 0.0f, 1.0f}};

    FloatM mPhi = {{cosf(l_fPhiC), 0.0f, sinf(l_fPhiC)},		// 2.2.4
                   {0.0f, 1.0f, 0.0f},
                   {-sinf(l_fPhiC), 0.0f, cosf(l_fPhiC)}};

    FloatM mA1, mA1_T;

    Float_multiplMatrix(mPhi, mLambda, mA1);					// 2.2.5
    Float_transMatrix(mA1, mA1_T);

    float l_fRz = fR2 * fE / sqrtf(fR2 * fE2 * cosf(l_fPhiC) * cosf(l_fPhiC) + fR2 * sinf(l_fPhiC) * sinf(l_fPhiC));	// 2.2.6

    float l_fXc = mA1_T[0][0] * (l_fRz + fH);			// 2.2.7
    float l_fYc = mA1_T[1][0] * (l_fRz + fH);
    float l_fZc = mA1_T[2][0] * (l_fRz + fH);

    float l_fPhi_stc = atan(tan(fPhiST) * fE2);		// 2.2.8

    FloatM mLambdaST = {{cosf(fLambdaST), sinf(fLambdaST), 0.0f},	// 2.2.9
                        {-sinf(fLambdaST), cosf(fLambdaST), 0.0f},
                        {0.0f, 0.0f, 1.0f}};

    FloatM mPhiST = {{cosf(l_fPhi_stc), 0.0f, sinf(l_fPhi_stc)},	// 2.2.10
                     {0.0f, 1.0f, 0.0f},
                     {-sinf(l_fPhi_stc), 0.0f, cosf(l_fPhi_stc)}};

    FloatM mP = {{1.0f, 0.0f, 0.0f},								// 2.2.11
                 {0.0f, cosf(fP), -sinf(fP)},
                 {0.0f, sinf(fP), cosf(fP)}};

    Float_multiplMatrix(mP, mPhiST, mA1);					// 2.2.12
    FloatM mA1_PPL;
    Float_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.2.12

    //FloatM m0 = {{0.0f, 0.0f, -1.0f},
    //             {1.0f, 0.0f, 0.0f},
    //             {0.0f, -1.0f, 0.0f}};
    FloatM m0 = {{0.0f, 0.0f, 1.0f},
                 {1.0f, 0.0f, 0.0f},
                 {0.0f, 1.0f, 0.0f}};

    FloatM mXYZ;

    Float_multiplMatrix(m0, mA1_PPL, mXYZ);

    *pX0 = mXYZ[0][0] * l_fXc + mXYZ[0][1] * l_fYc + mXYZ[0][2] * l_fZc;
    *pY0 = mXYZ[1][0] * l_fXc + mXYZ[1][1] * l_fYc + mXYZ[1][2] * l_fZc;
    *pZ0 = mXYZ[2][0] * l_fXc + mXYZ[2][1] * l_fYc + mXYZ[2][2] * l_fZc;

    return 1;
}

unsigned char bFloat_Zem2Geo(float fPhiST, float fLambdaST, float fP, float fX0, float fY0, float fZ0, float *pPhi, float *pLambda, float *pH)
{
    float fE = 1.0f - FCONST_E;
    float fE2 = fE * fE;
    float fR = FCONST_R;
    float fR2 = fR * fR;

    float l_fPhi_stc = atan(tan(fPhiST) * fE2);		// 2.3.1

    FloatM mLambdaST = {{cosf(fLambdaST), sinf(fLambdaST), 0.0f},	// 2.3.2
                        {-sinf(fLambdaST), cosf(fLambdaST), 0.0f},
                        {0.0f, 0.0f, 1.0f}};

    FloatM mPhiST = {{cosf(l_fPhi_stc), 0.0f, sinf(l_fPhi_stc)},	// 2.3.3
                     {0.0f, 1.0f, 0.0f},
                     {-sinf(l_fPhi_stc), 0.0f, cosf(l_fPhi_stc)}};

    FloatM mP = {{1.0f, 0.0f, 0.0f},								// 2.3.4
                 {0.0f, cosf(fP), -sinf(fP)},
                 {0.0f, sinf(fP), cosf(fP)}};

    FloatM mA1;
    FloatM mA1_PPL, mA1_PPL_T;

    Float_multiplMatrix(mP, mPhiST, mA1);					// 2.3.5
    Float_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.3.5
    Float_transMatrix(mA1_PPL, mA1_PPL_T);

    //FloatM m0 = {{0.0f, 1.0f, 0.0f},
    //             {0.0f, 0.0f, -1.0f},
    //             {-1.0f, 0.0f, 0.0f}};
    FloatM m0 = {{0.0f, 1.0f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
                 {1.0f, 0.0f, 0.0f}};

    FloatM mXcYcZc;

    Float_multiplMatrix(mA1_PPL_T, m0, mXcYcZc);		// 2.3.6

    float l_fXc = mXcYcZc[0][0] * fX0 + mXcYcZc[0][1] * fY0 + mXcYcZc[0][2] * fZ0;	// 2.3.6
    float l_fYc = mXcYcZc[1][0] * fX0 + mXcYcZc[1][1] * fY0 + mXcYcZc[1][2] * fZ0;
    float l_fZc = mXcYcZc[2][0] * fX0 + mXcYcZc[2][1] * fY0 + mXcYcZc[2][2] * fZ0;

    float l_fPhiC = atan2f(l_fZc, sqrtf(l_fXc * l_fXc + l_fYc * l_fYc));		// 2.3.7
    *pLambda = atan2f(l_fYc, l_fXc);											// 2.3.8
    *pPhi = atan(tan(l_fPhiC) / fE2);										// 2.3.11

    float l_fRz = fR2 * fE / sqrtf(fR2 * fE2 * cosf(l_fPhiC) * cosf(l_fPhiC) + fR2 * sinf(l_fPhiC) * sinf(l_fPhiC));	// 2.3.12

    *pH = sqrtf(fX0 * fX0 + fY0 * fY0 + fZ0 * fZ0) - l_fRz;	// 2.3.13

    return 1;
}

unsigned char bFloat_GetP(float fPhiST, float fLambdaST, float fPhiT, float fLambdaT, float *pP)
{
    float fE = 1.0f - FCONST_E;
    float fE2 = fE * fE;
    float fR = FCONST_R;
    float fR2 = fR * fR;

    float l_fPhiC = atan(tan(fPhiT) * fE2);

    float l_fRz = fR2 * fE / sqrtf(fR2 * fE2 * cosf(l_fPhiC) * cosf(l_fPhiC) + fR2 * sinf(l_fPhiC) * sinf(l_fPhiC));

    FloatM mLambda = {{cosf(fLambdaT), sinf(fLambdaT), 0.0f},
                      {-sinf(fLambdaT), cosf(fLambdaT), 0.0f},
                      {0.0f, 0.0f, 1.0f}};

    FloatM mPhi = {{cosf(l_fPhiC), 0.0f, sinf(l_fPhiC)},
                   {0.0f, 1.0f, 0.0f},
                   {-sinf(l_fPhiC), 0.0f, cosf(l_fPhiC)}};

    FloatM mA1, mA1_T;

    Float_multiplMatrix(mPhi, mLambda, mA1);
    Float_transMatrix(mA1, mA1_T);

    float l_fXc = mA1_T[0][0] * l_fRz;
    float l_fYc = mA1_T[1][0] * l_fRz;
    float l_fZc = mA1_T[2][0] * l_fRz;

    float l_fPhi_stc = atan(tan(fPhiST) * fE2);

    float l_fRst = fR2 * fE / sqrtf(fR2 * fE2 * cosf(l_fPhi_stc) * cosf(l_fPhi_stc) + fR2 * sinf(l_fPhi_stc) * sinf(l_fPhi_stc));

    FloatM mLambdaST = {{cosf(fLambdaST), sinf(fLambdaST), 0.0f},
                        {-sinf(fLambdaST), cosf(fLambdaST), 0.0f},
                        {0.0f, 0.0f, 1.0f}};

    FloatM mPhiST = {{cosf(l_fPhi_stc), 0.0f, sinf(l_fPhi_stc)},
                     {0.0f, 1.0f, 0.0f},
                     {-sinf(l_fPhi_stc), 0.0f, cosf(l_fPhi_stc)}};

    Float_multiplMatrix(mPhiST, mLambdaST, mA1);
    Float_transMatrix(mA1, mA1_T);

    float l_fXs = mA1_T[0][0] * l_fRst;
    float l_fYs = mA1_T[1][0] * l_fRst;
    float l_fZs = mA1_T[2][0] * l_fRst;

    float l_fm = (l_fYs * l_fZc - l_fZs * l_fYc) * l_fYs - (l_fZs * l_fXc - l_fXs * l_fZc) * l_fXs;	// 1.13
    float l_fn = sqrtf((l_fYs * l_fZc - l_fZs * l_fYc) * (l_fYs * l_fZc - l_fZs * l_fYc) +
                       (l_fZs * l_fXc - l_fXs * l_fZc) * (l_fZs * l_fXc - l_fXs * l_fZc) +
                       (l_fXs * l_fYc - l_fYs * l_fXc) * (l_fXs * l_fYc - l_fYs * l_fXc)
                       ) *
                        sqrtf(l_fYs * l_fYs + l_fXs * l_fXs);

    if (l_fn == 0.0f)
        *pP = 0.0f;		// 1.15
    else
        if (fabs(l_fm / l_fn) >= 1.0f)
            *pP = acos((l_fm / l_fn) > 0 ? 1.0f : -1.0f);		// 1.16
        else
            if(((fLambdaT - fLambdaST) >= 0.0f && (fLambdaT - fLambdaST) < M_PI) || (fLambdaT - fLambdaST) < -M_PI)
                *pP = acos(l_fm / l_fn);					// 1.17.1
            else
                *pP = 2.0f * (float)M_PI - acos(l_fm / l_fn);		// 1.17.2

    return 1;
}

//***Релаизация на Double***
unsigned char Double_multiplMatrix(DoubleM mA, DoubleM mB, DoubleM mC)
{
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
        {
            int k;
            mC[i][j] = 0.0;
            for (k = 0; k < 3; k++)
                mC[i][j] += mA[i][k] * mB[k][j];
        }

        return 1;
}

unsigned char Double_transMatrix(DoubleM mA, DoubleM mB)
{
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            mB[j][i] = mA[i][j];

    return 1;
}

unsigned char bDouble_GetM_Sop2Zem(double dX0, double dY0, double dZ0, DoubleM pM)
{
    double l_dPhiD = asin(dX0 / sqrt(dX0 * dX0 + dY0 * dY0));
    double l_dPhiZ = atan(dZ0 / sqrt(dX0 * dX0 + dY0 * dY0));

    DoubleM mPhiD = {{cos(l_dPhiD), sin(l_dPhiD), 0.0},
                     {-sin(l_dPhiD), cos(l_dPhiD), 0.0},
                     {0.0, 0.0, 1.0}};

    DoubleM mPhiZ = {{1.0, 0.0, 0.0},
                     {0.0, cos(l_dPhiZ), -sin(l_dPhiZ)},
                     {0.0, sin(l_dPhiZ), cos(l_dPhiZ)}};

    Double_multiplMatrix(mPhiD, mPhiZ, pM);

    return 1;
}

unsigned char bDouble_Geo2Zem(double dPhiST, double dLambdaST, double dP, double dPhi, double dLambda, double dH, double *pX0, double *pY0, double *pZ0)
{
    double dE = 1.0 - DCONST_E;
    double dE2 = dE * dE;
    double dR = DCONST_R;
    double dR2 = dR * dR;

    double l_dPhiC = atan(tan(dPhi) * dE2);		// 2.2.2

    DoubleM mLambda = {{cos(dLambda), sin(dLambda), 0.0},		// 2.2.3
                       {-sin(dLambda), cos(dLambda), 0.0},
                       {0.0, 0.0, 1.0}};

    DoubleM mPhi = {{cos(l_dPhiC), 0.0, sin(l_dPhiC)},		// 2.2.4
                    {0.0, 1.0, 0.0},
                    {-sin(l_dPhiC), 0.0, cos(l_dPhiC)}};

    DoubleM mA1, mA1_T;

    Double_multiplMatrix(mPhi, mLambda, mA1);					// 2.2.5
    Double_transMatrix(mA1, mA1_T);

    double l_dRz = dR2 * dE / sqrt(dR2 * dE2 * cos(l_dPhiC) * cos(l_dPhiC) + dR2 * sin(l_dPhiC) * sin(l_dPhiC));	// 2.2.6

    double l_dXc = mA1_T[0][0] * (l_dRz + dH);			// 2.2.7
    double l_dYc = mA1_T[1][0] * (l_dRz + dH);
    double l_dZc = mA1_T[2][0] * (l_dRz + dH);

    double l_dPhi_stc = atan(tan(dPhiST) * dE2);		// 2.2.8

    DoubleM mLambdaST = {{cos(dLambdaST), sin(dLambdaST), 0.0},	// 2.2.9
                         {-sin(dLambdaST), cos(dLambdaST), 0.0},
                         {0.0, 0.0, 1.0}};

    DoubleM mPhiST = {{cos(l_dPhi_stc), 0.0, sin(l_dPhi_stc)},	// 2.2.10
                      {0.0, 1.0, 0.0},
                      {-sin(l_dPhi_stc), 0.0, cos(l_dPhi_stc)}};

    DoubleM mP = {{1.0, 0.0, 0.0},								// 2.2.11
                  {0.0, cos(dP), -sin(dP)},
                  {0.0, sin(dP), cos(dP)}};

    Double_multiplMatrix(mP, mPhiST, mA1);					// 2.2.12
    DoubleM mA1_PPL;
    Double_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.2.12

    //DoubleM m0 = {{0.0, 0.0, -1.0},
    //              {1.0, 0.0, 0.0},
    //              {0.0, -1.0, 0.0}};
    DoubleM m0 = {{0.0, 0.0, 1.0},
                  {1.0, 0.0, 0.0},
                  {0.0, 1.0, 0.0}};

    DoubleM mXYZ;

    Double_multiplMatrix(m0, mA1_PPL, mXYZ);

    *pX0 = mXYZ[0][0] * l_dXc + mXYZ[0][1] * l_dYc + mXYZ[0][2] * l_dZc;
    *pY0 = mXYZ[1][0] * l_dXc + mXYZ[1][1] * l_dYc + mXYZ[1][2] * l_dZc;
    *pZ0 = mXYZ[2][0] * l_dXc + mXYZ[2][1] * l_dYc + mXYZ[2][2] * l_dZc;

    return 1;
}

unsigned char bDouble_Geo2Zem_2(double dPhiST, double dLambdaST, double dPhi, double dLambda, double dH, double *pX0, double *pY0, double *pZ0)
{
    double dP = 0.0;
    bDouble_GetP(dPhiST, dLambdaST, dPhi, dLambda, &dP);

    double dE = 1.0 - DCONST_E;
    double dE2 = dE * dE;
    double dR = DCONST_R;
    double dR2 = dR * dR;

    double l_dPhiC = atan(tan(dPhi) * dE2);		// 2.2.2

    DoubleM mLambda = {{cos(dLambda), sin(dLambda), 0.0},		// 2.2.3
                       {-sin(dLambda), cos(dLambda), 0.0},
                       {0.0, 0.0, 1.0}};

    DoubleM mPhi = {{cos(l_dPhiC), 0.0, sin(l_dPhiC)},		// 2.2.4
                    {0.0, 1.0, 0.0},
                    {-sin(l_dPhiC), 0.0, cos(l_dPhiC)}};

    DoubleM mA1, mA1_T;

    Double_multiplMatrix(mPhi, mLambda, mA1);					// 2.2.5
    Double_transMatrix(mA1, mA1_T);

    double l_dRz = dR2 * dE / sqrt(dR2 * dE2 * cos(l_dPhiC) * cos(l_dPhiC) + dR2 * sin(l_dPhiC) * sin(l_dPhiC));	// 2.2.6

    double l_dXc = mA1_T[0][0] * (l_dRz + dH);			// 2.2.7
    double l_dYc = mA1_T[1][0] * (l_dRz + dH);
    double l_dZc = mA1_T[2][0] * (l_dRz + dH);

    double l_dPhi_stc = atan(tan(dPhiST) * dE2);		// 2.2.8

    DoubleM mLambdaST = {{cos(dLambdaST), sin(dLambdaST), 0.0},	// 2.2.9
                         {-sin(dLambdaST), cos(dLambdaST), 0.0},
                         {0.0, 0.0, 1.0}};

    DoubleM mPhiST = {{cos(l_dPhi_stc), 0.0, sin(l_dPhi_stc)},	// 2.2.10
                      {0.0, 1.0, 0.0},
                      {-sin(l_dPhi_stc), 0.0, cos(l_dPhi_stc)}};

    DoubleM mP = {{1.0, 0.0, 0.0},								// 2.2.11
                  {0.0, cos(dP), -sin(dP)},
                  {0.0, sin(dP), cos(dP)}};

    Double_multiplMatrix(mP, mPhiST, mA1);					// 2.2.12
    DoubleM mA1_PPL;
    Double_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.2.12

    //DoubleM m0 = {{0.0, 0.0, -1.0},
    //              {1.0, 0.0, 0.0},
    //              {0.0, -1.0, 0.0}};
    DoubleM m0 = {{0.0, 0.0, 1.0},
                  {1.0, 0.0, 0.0},
                  {0.0, 1.0, 0.0}};

    DoubleM mXYZ;

    Double_multiplMatrix(m0, mA1_PPL, mXYZ);

    *pX0 = mXYZ[0][0] * l_dXc + mXYZ[0][1] * l_dYc + mXYZ[0][2] * l_dZc;
    *pY0 = mXYZ[1][0] * l_dXc + mXYZ[1][1] * l_dYc + mXYZ[1][2] * l_dZc;
    *pZ0 = mXYZ[2][0] * l_dXc + mXYZ[2][1] * l_dYc + mXYZ[2][2] * l_dZc;

    return 1;
}

unsigned char bDouble_Zem2Geo(double dPhiST, double dLambdaST, double dP, double dX0, double dY0, double dZ0, double *pPhi, double *pLambda, double *pH)
{
    double dE = 1.0 - DCONST_E;
    double dE2 = dE * dE;
    double dR = DCONST_R;
    double dR2 = dR * dR;

    double l_dPhi_stc = atan(tan(dPhiST) * dE2);		// 2.3.1

    DoubleM mLambdaST = {{cos(dLambdaST), sin(dLambdaST), 0.0},	// 2.3.2
                         {-sin(dLambdaST), cos(dLambdaST), 0.0},
                         {0.0, 0.0, 1.0}};

    DoubleM mPhiST = {{cos(l_dPhi_stc), 0.0, sin(l_dPhi_stc)},	// 2.3.3
                      {0.0, 1.0, 0.0},
                      {-sin(l_dPhi_stc), 0.0, cos(l_dPhi_stc)}};

    DoubleM mP = {{1.0, 0.0, 0.0},								// 2.3.4
                  {0.0, cos(dP), -sin(dP)},
                  {0.0, sin(dP), cos(dP)}};

    DoubleM mA1;
    DoubleM mA1_PPL, mA1_PPL_T;

    Double_multiplMatrix(mP, mPhiST, mA1);					// 2.3.5
    Double_multiplMatrix(mA1, mLambdaST, mA1_PPL);			// 2.3.5
    Double_transMatrix(mA1_PPL, mA1_PPL_T);

    //DoubleM m0 = {{0.0, 1.0, 0.0},
    //              {0.0, 0.0, -1.0},
    //              {-1.0, 0.0, 0.0}};
    DoubleM m0 = {{0.0, 1.0, 0.0},
                  {0.0, 0.0, 1.0},
                  {1.0, 0.0, 0.0}};

    DoubleM mXcYcZc;

    Double_multiplMatrix(mA1_PPL_T, m0, mXcYcZc);		// 2.3.6

    double l_dXc = mXcYcZc[0][0] * dX0 + mXcYcZc[0][1] * dY0 + mXcYcZc[0][2] * dZ0;	// 2.3.6
    double l_dYc = mXcYcZc[1][0] * dX0 + mXcYcZc[1][1] * dY0 + mXcYcZc[1][2] * dZ0;
    double l_dZc = mXcYcZc[2][0] * dX0 + mXcYcZc[2][1] * dY0 + mXcYcZc[2][2] * dZ0;

    double l_fPhiC = atan2(l_dZc, sqrt(l_dXc * l_dXc + l_dYc * l_dYc));		// 2.3.7
    *pLambda = atan2(l_dYc, l_dXc);											// 2.3.8
    *pPhi = atan(tan(l_fPhiC) / dE2);										// 2.3.11

    double l_dRz = dR2 * dE / sqrt(dR2 * dE2 * cos(l_fPhiC) * cos(l_fPhiC) + dR2 * sin(l_fPhiC) * sin(l_fPhiC));	// 2.3.12

    *pH = sqrt(dX0 * dX0 + dY0 * dY0 + dZ0 * dZ0) - l_dRz;	// 2.3.13

    return 1;
}

unsigned char bDouble_GetP(double dPhiST, double dLambdaST, double dPhiT, double dLambdaT, double *pP)
{
    double dE = 1.0 - DCONST_E;
    double dE2 = dE * dE;
    double dR = DCONST_R;
    double dR2 = dR * dR;

    double l_dPhiC = atan(tan(dPhiT) * dE2);

    double l_dRz = dR2 * dE / sqrt(dR2 * dE2 * cos(l_dPhiC) * cos(l_dPhiC) + dR2 * sin(l_dPhiC) * sin(l_dPhiC));

    DoubleM mLambda = {{cos(dLambdaT), sin(dLambdaT), 0.0},
                       {-sin(dLambdaT), cos(dLambdaT), 0.0},
                       {0.0, 0.0, 1.0}};

    DoubleM mPhi = {{cos(l_dPhiC), 0.0, sin(l_dPhiC)},
                    {0.0, 1.0, 0.0},
                    {-sin(l_dPhiC), 0.0f, cos(l_dPhiC)}};

    DoubleM mA1, mA1_T;

    Double_multiplMatrix(mPhi, mLambda, mA1);
    Double_transMatrix(mA1, mA1_T);

    double l_dXc = mA1_T[0][0] * l_dRz;
    double l_dYc = mA1_T[1][0] * l_dRz;
    double l_dZc = mA1_T[2][0] * l_dRz;

    double l_dPhi_stc = atan(tan(dPhiST) * dE2);

    double l_dRst = dR2 * dE / sqrt(dR2 * dE2 * cos(l_dPhi_stc) * cos(l_dPhi_stc) + dR2 * sin(l_dPhi_stc) * sin(l_dPhi_stc));

    DoubleM mLambdaST = {{cos(dLambdaST), sin(dLambdaST), 0.0},
                         {-sin(dLambdaST), cos(dLambdaST), 0.0},
                         {0.0, 0.0, 1.0}};

    DoubleM mPhiST = {{cos(l_dPhi_stc), 0.0, sin(l_dPhi_stc)},
                      {0.0, 1.0, 0.0},
                      {-sin(l_dPhi_stc), 0.0, cos(l_dPhi_stc)}};

    Double_multiplMatrix(mPhiST, mLambdaST, mA1);
    Double_transMatrix(mA1, mA1_T);

    double l_dXs = mA1_T[0][0] * l_dRst;
    double l_dYs = mA1_T[1][0] * l_dRst;
    double l_dZs = mA1_T[2][0] * l_dRst;

    double l_dm = (l_dYs * l_dZc - l_dZs * l_dYc) * l_dYs - (l_dZs * l_dXc - l_dXs * l_dZc) * l_dXs;	// 1.13
    double l_dn = sqrt(																					// 1.14
                       (l_dYs * l_dZc - l_dZs * l_dYc) * (l_dYs * l_dZc - l_dZs * l_dYc) +
                       (l_dZs * l_dXc - l_dXs * l_dZc) * (l_dZs * l_dXc - l_dXs * l_dZc) +
                       (l_dXs * l_dYc - l_dYs * l_dXc) * (l_dXs * l_dYc - l_dYs * l_dXc)
                       ) *
                       sqrt(l_dYs * l_dYs + l_dXs * l_dXs);

    if (l_dn == 0.0)
        *pP = 0.0;		// 1.15
    else
        if (fabs(l_dm / l_dn) >= 1.0)
            *pP = acos(((l_dm / l_dn) > 0 ? 1.0 : -1.0));		// 1.16
        else
            if(((dLambdaT - dLambdaST) >= 0.0 && (dLambdaT - dLambdaST) < M_PI) || (dLambdaT - dLambdaST) < -M_PI)
                *pP = acos(l_dm / l_dn);					// 1.17.1
            else
                *pP = 2.0 * M_PI - acos(l_dm / l_dn);		// 1.17.2

    return 1;
}
