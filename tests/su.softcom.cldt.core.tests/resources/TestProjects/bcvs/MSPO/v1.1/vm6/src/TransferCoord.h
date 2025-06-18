#ifndef NAV_TRSNSFORM
#define NAV_TRSNSFORM

#define FCONST_E	1.0f / 298.25784f
#define DCONST_E	1.0 / 298.25784
#define FCONST_R	6378136.0f
#define DCONST_R	6378136.0

typedef float	FloatM[3][3];
typedef double	DoubleM[3][3];

int iMultiplyMatrix(float *pA, float *pB, float *pC, int i_x1, int i_y1, int i_x2, int i_y2);

unsigned char bFloat_GetM_Sop2Zem(float fX0, float fY0, float fZ0, FloatM pM);
unsigned char bFloat_Geo2Zem(float fPhiST, float fLambdaST, float fP, float fPhi, float fLambda, float fH, float *pX0, float *pY0, float *pZ0);
unsigned char bFloat_Geo2Zem_2(float fPhiST, float fLambdaST, float fPhi, float fLambda, float fH, float *pX0, float *pY0, float *pZ0);
unsigned char bFloat_Zem2Geo(float fPhiST, float fLambdaST, float fP, float fX0, float fY0, float fZ0, float *pPhi, float *pLambda, float *pH);
unsigned char bFloat_GetP(float fPhiST, float fLambdaST, float fPhiT, float fLambdaT, float *pP);

unsigned char bDouble_GetM_Sop2Zem(double dX0, double dY0, double dZ0, DoubleM pM);
unsigned char bDouble_Geo2Zem(double dPhiST, double dLambdaST, double dP, double dPhi, double dLambda, double dH, double *pX0, double *pY0, double *pZ0);
unsigned char bDouble_Geo2Zem_2(double dPhiST, double dLambdaST, double dPhi, double dLambda, double dH, double *pX0, double *pY0, double *pZ0);
unsigned char bDouble_Zem2Geo(double dPhiST, double dLambdaST, double dP, double dX0, double dY0, double dZ0, double *pPhi, double *pLambda, double *pH);
unsigned char bDouble_GetP(double dPhiST, double dLambdaST, double dPhiT, double dLambdaT, double *pP);

#endif	// NAV_TRSNSFORM
