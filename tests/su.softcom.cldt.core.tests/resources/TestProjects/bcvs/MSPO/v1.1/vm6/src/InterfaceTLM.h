#ifndef INTERFACE_TLM
#define INTERFACE_TLM

typedef union                       // Побитовая структура ui1000h
{
    unsigned int ui1000h;

    struct
    {
        unsigned int    TempSupport     :1; // Поддержка аппаратурой датчиков температуры                           [31]
        unsigned int    sVM6RESOPN      :1; // Признак реакции МУВ на правильное изменение кода годности 1890ВМ6Я   [30]
        unsigned int    sVM6_VM7_RESOPN :1; // Признак открытия доступа к ресурсам телеметрии, MRAM                 [29]
        unsigned int    REZERV_1        :9; // Резерв                                                               [28...20]
        unsigned int    sVM6LIVEINC     :4; // Интегрированный код годности 1890ВМ6Я                                [19...16]
        unsigned int    REZERV_2        :3; // Резерв                                                               [15...13]
        unsigned int    sTLMBANK        :1; // Флаг BANK ТМК 1890ВМ6Я                                               [12]
        unsigned int    sTLMREADY       :1; // Флаг READY ТМК 1890ВМ6Я                                              [11]
        unsigned int    sTLMSTARTPCI    :1; // Флаг START ТМК 1890ВМ6Я                                              [10]
        unsigned int    sA              :10;// Количество пакетов ОЗУ 1 ТМК                                         [9...0]
    };
} psi_1000h;

typedef union                       // Побитовая структура ui1001h
{
    unsigned int ui1001h;

    struct
    {
        unsigned int    TempSupport     :1; // Поддержка аппаратурой датчиков температуры                           [31]
        unsigned int    sVM7RESOPN      :1; // Признак реакции МУВ на правильное изменение кода годности 1890ВМ7Я   [30]
        unsigned int    sVM6_VM7_RESOPN :1; // Признак открытия доступа к ресурсам телеметрии, MRAM                 [29]
        unsigned int    REZERV_1        :9; // Резерв                                                               [28...20]
        unsigned int    sVM7LIVEINC     :4; // Интегрированный код годности 1890ВМ7Я                                [19...16]
        unsigned int    REZERV_2        :3; // Резерв                                                               [15...13]
        unsigned int    sTLMBANK        :1; // Флаг BANK ТМК 1890ВМ7Я                                               [12]
        unsigned int    sTLMREADY       :1; // Флаг READY ТМК 1890ВМ7Я                                              [11]
        unsigned int    sTLMSTARTPCI    :1; // Флаг START ТМК 1890ВМ7Я                                              [10]
        unsigned int    sB              :10;// Количество пакетов ОЗУ 2 ТМК                                         [9...0]
    };
} psi_1001h;

typedef struct                      // Кадр №1 0x01010101
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;  // Маркер текущего кадра 0x01010101
    float           fTime;              // Метка времени
    unsigned int    uiIdKadr;           // Идентификатор кадра
    unsigned short  usRegNormBCVSM;     // Регистр норм
    unsigned int    uiRegNormRK;	    // Регистр норм РК
    unsigned short  usUS;
    unsigned short  usIS;
    unsigned int    uiUSL;
    unsigned int    uiISL;

    struct DataBCVSM
    {
        unsigned int    uiMarkerParamBCVSM_1;
        State_t         stExpressTest;
        unsigned int    uiInitialMSPO;
        unsigned char   ucVersion[20];
        unsigned char   ucDate[12];
        State_t         stExpressTest_VM7;
        unsigned char   ucVersion_VM7[20];
        unsigned char   ucDate_VM7[12];
        unsigned char   ucXYp[2];
        unsigned int    uiID_OutPacketBCVSM;
        unsigned int    uiID_InPacketRK;
        unsigned short  usTypeC;
        unsigned int    uiID_OutPacketRK_C1;
        unsigned int    uiID_InPacketBCVSM;
        size_t          stSizeOutBufRK;
        unsigned short  usSizeVM;

        struct M0
        {
            unsigned int    uiMarkerM_1;
            unsigned char   ucData[34];
            unsigned short  usKS;
            unsigned int    uiMarkerM_2;
        } PACK stM0;

        unsigned int    uiMarkerParamBCVSM_2;
        unsigned char   ucRezerv[122];
    } PACK stDataBCVSM;

    unsigned int    uiMarkerKadrStop;  // Маркер текущего кадра 0x01010101
} PACK Kadr_1;

typedef struct                      // Кадр №2 0x02020202
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;
    float           fTime;
    unsigned int    uiIdKadr;
    unsigned short  usRegNormBCVSM;
    unsigned int    uiRegNormRK;
    unsigned short  usUS;
    unsigned short  usIS;
    unsigned int    uiUSL;
    unsigned int    uiISL;

    struct DataBCVSM_02
    {
        unsigned int    uiMarkerParamBCVSM_1;
        unsigned char   ucTypeID;           // Тип ИД
        unsigned int    uiCountSD;          // Кол-во СД
        unsigned short  usStartSend;        // Команда на начало отправки ИД
        unsigned short  usStopSend;         // Команда на конец отправки ИД
        unsigned int    uiInfoCountSD;      // Прислали кол-во СД ИД
        unsigned short  usKS_ID_M2;
        unsigned short  usKS_ID;
        unsigned short  usStatusID;
        CoordPLH        coordStart;
        CoordPLH        coordEnd;
        CoordXYZ        coordEndXYZ;

        unsigned int    uiID_OutPacketBCVSM;
        unsigned int    uiID_InPacketRK;
        unsigned short  usTypeC;
        unsigned int    uiID_OutPacketRK_C1;
        unsigned int    uiID_InPacketBCVSM;
        size_t          stSizeOutBufRK;
        unsigned short  usSizeVM;

        struct M0_02
        {
            unsigned int    uiMarkerM_1;
            unsigned char   ucData[34];
            unsigned short  usKS;
            unsigned int    uiMarkerM_2;
        } PACK stM0;

        struct VM0_02
        {
            unsigned int    uiMarkerVM_1;
            unsigned short	usSizeMDI;
            unsigned short  usVersionVM0;
            unsigned char	ucDataMDI[16];
            unsigned int    uiMarkerVM_2;
        } PACK stVM0;

        unsigned int    uiMarkerParamBCVSM_2;
        unsigned char   ucRezerv[253];
    } PACK stDataBCVSM;

    unsigned int    uiMarkerKadrStop;
} PACK Kadr_2;

typedef struct                      // Кадр №3 0x03030303
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;
    float           fTime;
    unsigned int    uiIdKadr;
    unsigned short  usRegNormBCVSM;
    unsigned int    uiRegNormRK;
    unsigned short  usUS;
    unsigned short  usIS;
    unsigned int    uiUSL;
    unsigned int    uiISL;

    struct DataBCVSM_03
    {
        unsigned int    uiMarkerParamBCVSM_1;

        unsigned char   ucTekState;
        unsigned char   ucNextState;
        unsigned short  usPrC10;
        float           fPsi;
        float           fThetta;
        float           fGamma;
        float           fX0;
        float           fY0;
        float           fZ0;
        unsigned short  usRegMSPO;

        unsigned char	ucI;
        float           fXoum;
        float           fYoum;
        float           fZoum;

        unsigned char   ucXp;
        unsigned char   ucYp;
        unsigned char   ucNz;

        unsigned int    uiID_OutPacketBCVSM;
        unsigned int    uiID_InPacketRK;
        unsigned short  usTypeC;
        unsigned int    uiID_OutPacketRK_C1;
        unsigned int    uiID_InPacketBCVSM;
        size_t          stSizeOutBufRK;
        unsigned short  usSizeVM;

        unsigned int    uiMarkerParamBCVSM_2;
        unsigned char   ucRezerv[334];
    } PACK stDataBCVSM;

    unsigned int    uiMarkerKadrStop;
} PACK Kadr_3;

typedef struct
{
    float           fPhi;
    float           fLambda;
    float           fH;
} PACK DataOUM_TLM;

typedef struct                      // Кадр №1 ОЗУ 2 0xB1B1B1B1
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;
    float           fTime;
    unsigned int    uiIdKadr;

    unsigned char   ucTypeID;
    unsigned char   ucParsID;
    unsigned int    uiInfoParseID;
    unsigned short  usKS_ID;
    unsigned short  usKS_ID_M2;
    unsigned short  usVer;
    unsigned int    uiL_inf;
    unsigned short  usNoum;
    DataOUM_TLM     stOUM[10];

    unsigned char   ucRezerv[556];      // Резерв в этом кадре

    unsigned int    uiMarkerKadrStop;
} PACK Kadr_1_OZU2;

typedef struct                      // Кадр №2 ОЗУ 2 0xB2B2B2B2
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;
    float           fTime;
    unsigned int    uiIdKadr;

    unsigned char   ucTekSendOUM;
    unsigned char   ucTekData688;
    unsigned char   ucDataImg[688];

    unsigned char   ucRezerv[4];

    unsigned int    uiMarkerKadrStop;  // Маркер текущего кадра
} PACK Kadr_2_OZU2;

typedef struct                      // Кадр №3 ОЗУ 2 0xB3B3B3B3
{
    unsigned short  usKSpack;           // Контрольная сумма всего пакета

    unsigned int    uiMarkerKadrStart;  // Маркер текущего кадра
    float           fTime;
    unsigned int    uiIdKadr;

    unsigned char   ucSendVision;
    unsigned short  usKS;
    unsigned short  usNumStrRLI;
    unsigned short  usDataStrRLI[256];

    unsigned char   ucRezerv[177];

    unsigned int    uiMarkerKadrStop;
} PACK Kadr_3_OZU2;

//***Функции формирования ТМ-кадров ОЗУ №1***
void vCreateKadr_Info_1(DataTLM*, InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);
void vCreateKadr_PP_2(DataTLM*, InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);
void vCreateKadr_OR_3(DataTLM*, InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);
//*******************************************

//***Функции формирования ТМ-кадров ОЗУ №2***
void vCreateKadr_1_OZU2(DataTLM*, MuvMemoryData*);
void vCreateKadr_2_OZU2(DataTLM*, MuvMemoryData*);
void vCreateKadr_3_OZU2(DataTLM*, MuvMemoryData*);
//*******************************************

//***Менеджер по формированию кадров МСПО***
void vCreateDataTMI(InputDataMSPO*, OutputDataMSPO*, MuvMemoryData*);
//******************************************

#endif // INTERFACE_TLM
