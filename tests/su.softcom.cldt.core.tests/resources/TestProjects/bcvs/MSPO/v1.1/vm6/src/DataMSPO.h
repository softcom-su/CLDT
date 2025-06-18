#ifndef DATA_MSPO_H
#define DATA_MSPO_H

#define PACK	__attribute__((__packed__))		// Для выравнивания структур

//***Структура sTMIWRD***
typedef union                           // sTMIWRD
{
    unsigned int    uiTMIWRD;

    struct
    {
        unsigned short sSRV              :1;	// [31]     Режим работы: 0 - технологический; 1 - штатный
        unsigned short sVM6_VM7_RESOPN   :1;    // [30]     Признак реакции МУВ на правильное изменение кодов годности
        unsigned short sSWITCHINF        :2;    // [29-28]  Признак переключения: 0 - по команде Х2; 1 - в случае неисправности ВМ; 2 - в случчае срабатывания ДПФ; 3 - не было переключения
        unsigned short sTLV              :1;	// [27]     Признак наличия перемычек на Х2
        unsigned short sMBMKO            :1;	// [26]     Режим технологического управления мультиплексорами
        unsigned short sMBWR             :1;	// [25]     Режим записи
        unsigned short sSWITCINF_42      :3;	// [24-22]  Признак переключения [4..2]
        unsigned short sVM67RST          :1;	// [21]     Режим технологической подачи сигнала сброс на процессоры ВМ6 и ВМ7
        unsigned short FLASHSTREAM       :1;	// [20]     Режим потокового программирования
        unsigned short sVMLIVE           :1;	// [19]     Признак годности аппаратуры горячего ВМ
        unsigned short sVM1LCHBAD        :1;	// [18]     Признак частота PLL1 ВМ неисправна
        unsigned short sVM2LCHBAD        :1;	// [17]     Признак частота PLL2 ВМ неисправна
        unsigned short sMUV1LCHBAD       :1;	// [16]     Признак частота PLL1 МУВ неисправна
        unsigned short sMUV2LCHBAD       :1;	// [15]     Признак частота PLL2 МУВ-ВМ-МУВ неисправна
        unsigned short sSWITCHINF_85     :4;	// [14-11]  Признак переключения
        unsigned short sVM6LIVE          :1;	// [10]     Признак годности узла 6 ВМ
        unsigned short sVM7LIVE          :1;	// [9]      Признак годности узла 7 ВМ
        unsigned short sMOVE             :1;	// [8]      Признак начала движения
        unsigned short tSWITCH           :1;	// [7]      Признак наличия воздействия
        unsigned short DINC6_5us         :1;	// [6]      Признак двойной инкрементации ИКГ за время < 5 мкс
        unsigned short DINC7_5us         :1;	// [5]      Признак двойной инкрементации ИКГ за время < 5 мкс
        unsigned short sVM3LCHBAD        :1;	// [4]      Признак частота PLL3 ВМ неисправна
        unsigned short sPOWST            :4;	// [3-0]    Состояние автомата переключения резервов МУВ
    };
} sTMIWRD;
//***********************

//***Ус***
typedef union                           // sUS
{
    unsigned short	usUS;			// Ук или Ус
    struct
    {
        unsigned short   usRez   :14;    // Резерв                  - 2-15 биты

        unsigned short   usUS2   :1;    // Ус2 - Работа            - 1 бит
        unsigned short   usUS1   :1;    // Уc1 - Проверка связи	  - 0 бит
    };
} sUS;
//**************

//***Ис***
typedef union                           // sIS
{
    unsigned short	usIS;			// Ис прибора БЦВС-М
    struct
    {
        unsigned short   usRez   :14;	//

        unsigned short   usIS1   :1;	// Ис1  - Координаты распознавания готовы						- 1 бит
        unsigned short   usIS0   :1;	// Ис0  - Готовность к работе									- 0 бит
    };
} sIS;
//**************

//***Структура регистра норм***
typedef union                           // sREG
{
    unsigned short	usRegNorm;			// Регистр норм
    struct
    {
        unsigned short   usNormM3        :1; // Норма М3						- 15 бит
        unsigned short   usNormBCVSM     :1; // Норма БЦВС-М					- 14 бит
        unsigned short   usNormRK        :1; // Норма РК 						- 13 бит
        unsigned short   usNormID        :1; // Норма загрузки ИД            	- 12 бит

        unsigned short   usRezerv        :12;// Резерв                          - 9-8 биты
    };
} sREG;
//********************************

//***Структура регистра норм РК***
typedef union                           // sREG_RK
{
    unsigned int	uiRegNormRK;			// Регистр норм
    struct
    {
        unsigned int   uiRezerv       :29; // Резерв
        unsigned int   uiNormRK       :1;  // РК неисправна								- 2 бит
        unsigned int   uiNormM3       :1;  // Норма обмена по каналу МКИО				- 1 бит
        unsigned int   uiNormEth      :1;  // Норма обмена с БЦ							- 0 бит
    };
} sREG_RK;
//********************************

//***Структура инициализации МСПО***
typedef union                           // sINITIALSPO
{
    unsigned int	uiInitialSPO;			// Результаты инициализации МСПО
    struct
    {
        unsigned int   uiRezerv        :21; // Резерв										   - 11...31 бит
        unsigned int   uiLoadReboot    :1;  // Восстанавление после перезагрузки               - 10 бит
        unsigned int   uiInitRapidIO   :1;  // Инициализация RapidIO                           - 9 бит
        unsigned int   uiInitTMI       :1;	// Инициализация ТМИ                               - 8 бит
        unsigned int   uiInitEthernet  :1;  // Инициализация Ethernet                          - 7 бит
        unsigned int   uiInitData      :1;  // Корректная инициализация параметров vInitData() - 6 бит
        unsigned int   uiGoodExpTest   :1;  // Успешное прохождение экспресс-тестов            - 5 бит
        unsigned int   uiInitMRAM      :1;  // Инициализация MRAM                              - 4 бит
        unsigned int   uiInitMKIO_M3   :1;	// Инициализация МКИО по М3                        - 3 бит
        unsigned int   uiInitMKIO_M2   :1;	// Инициализация МКИО по М2                        - 2 бит
        unsigned int   uiInitTimer     :1;	// Инициализация таймера                           - 1 бит
        unsigned int   uiInitThreadEth :1;	// Инициализация потока Ethernet                   - 0 бит
    };
} sINITIALSPO;
//***************************************

//***Структура регистра состояния МСПО***
typedef union                           // sREGSPO
{
    unsigned short	usRegMSPO;			// Регистр состояния МСПО
    struct
    {
        unsigned short   usRezerv        :11; // Пока резерв                                          (5-15 бит)

        unsigned short   usIsID_MRAM     :1; // ИД в MRAM записаны                                   (4 бит)
        unsigned short   usIsReboot      :1; // Признак переключения БЦ-61 для ОР                    (3 бит)

        unsigned short   usWorkOR        :1;	// Выполнение ОР                                        (2 бит)
        unsigned short   usWorkPP        :1;	// Выполнение ПП                                        (1 бит)
        unsigned short   usParsID        :1;	// Попытка разбора БНИ                                  (0 бит)
    };
} sREGMSPO;
//********************************************

//***Вспомогательные структуры***
typedef struct		                    // CoordPLH
{
	float fPhi;		// Широта
	float fLambda;	// Долгота
    float fH;         // Высота
} CoordPLH;

typedef struct		                    // CoordPLint
{
	int iPhi;		// Широта
	int iLambda;	// Долгота
} CoordPLint;

typedef struct		                    // CoordPL
{
	float fPhi;		// Широта
	float fLambda;	// Долгота
} CoordPL;

typedef struct		                    // CoordXYZ
{
	float fX0;		// X
	float fY0;		// Y
	float fZ0;		// Z
} CoordXYZ;

typedef struct                          // Data_AS
{
    unsigned short  usNviz;
	unsigned int    uiXp;
	unsigned int    uiYp;
	unsigned int    uiNz;
} PACK DATA_AS;

typedef struct							// SerchEtalon
{
    unsigned char	ucI;  	            // Номер найденного ОУМ	                    - ИЗНАЧАЛЬНО ДОЛЖЕН БЫТЬ 0
    unsigned char	ucCanSee;			// Флаг, показывающий соответствие ОУМ условиям визирования
    unsigned char	ucIsSelect;			// Флаг, показывающий выбора эталона
    int             iRetSerch;          // Значение по результату работы функции iSerchEtalonMN()

    float			fDnakl; 			// Наклонная дальность
    CoordXYZ		coordOUM_XYZ;		// Координаты
} PACK SerchEtalon;
//*******************************

//***Структура хранения полученных изображений***
typedef union                           // sStatusRLI
{
    unsigned short	usStatusRLI;			// Регистр норм 3А222
    struct
    {
        unsigned short  usIsOK      :1;     // Признак получения 256 строк РЛИ  - 15 бит
        unsigned short  usCountRows :15;    // Кол-во принятых строк            - 14-0 биты
    };
} sStatusRLI;

typedef struct                          // sRLI_MUV
{
    sStatusRLI  	uStatusRLI[3];  // Информация по полученных РЛИ
    unsigned  short	usKS[3];		// Контрольная сумма

    unsigned short  *p_usRLI_Amp;       // Амплитудное изображение суммарного канала
} PACK sRLI_MUV;
//************************************************

//*** Структура хранения БНИ/НИБ ***
typedef struct                          // DataOUM_KN
{
    float           fPhi;               // Широта ОУМ/точки прицеливания (репера), рад
    float           fLambda;            // Долгота ОУМ/точки прицеливания (репера), рад
    float           fH;                 // Высота ОУМ/точки прицеливания (репера), м
    unsigned char   ucDataImg[2048];    // Изображение
} PACK DataOUM;

typedef struct                          // ID
{
    unsigned char	ucTypeID;			// Тип ИД
    unsigned int	uiCountSD;			// Размер массива p_usSD_ID (кол-во СД)

    union
    {
        unsigned int    uiInfoParseID; // Информация о разборе ИД

        struct
        {
            unsigned int  uiRezerv        :25;  // Резерв
            unsigned int  uiZeroEtalOUM   :1;   // Кол-во эталонов ОУМ = 0       = 7 бит
            unsigned int  uiZeroOUM       :1;   // Кол-во ОУМ = 0                = 6 бит
            unsigned int  uiCntEtalMN     :1;   // Кол-во эталонов >0 и <=45     = 4 бит
            unsigned int  uiCntOUM        :1;   // Кол-во ОУМ >0 и <=10          = 3 бит
            unsigned int  uiIsVer         :1;   // Признак поддерживаемой версии = 2 бит
            unsigned int  uiKS            :1;   // Признак совпадения КС         = 1 бит
            unsigned int  uiGoodRead      :1;   // Признак успешного чтения      = 0 бит
        } stInfoID;
    };

    //***Параметры ввода***
    unsigned short  usStartSend;        // Значение начала выдачи ИД
    unsigned short  usStopSend;         // Значение конца выдачи ИД
    unsigned int    uiInfoCountSD;      // Кол-во передаваемых СД ИД
    unsigned short  usKS_ID_M2;         // КС ИД посчитанная кем-то
    unsigned short  usKS_ID;            // КС ИД посчитанная МСПО
    unsigned short  usStatusID;         // Статус ввода ИД
    //*********************

    //***Параметры БНИ***
    unsigned short  usVer;              // Версия формата
    CoordPLH 		coordStart;		    // Местоположение изделия ПЗ-90.02
    CoordPLH 		coordEnd;		    // Местоположение изделия ПЗ-90.02
    CoordXYZ        coordEndXYZ;        // Местоположение объекта в Земной СК
    float           fP;                 // Пеленг
    unsigned short  usNoum;             // Количество ОУМов
    DataOUM         *p_stOUM;           // Данные ОУМов
    //*********************

    unsigned short	*p_usSD_ID;	// СД с ИД от СУС
} ID_DATA;
//**********************************

//***Структуры данных для передачи БЦВС-М -> РК***
typedef struct						    // M4
{
    unsigned int	uiPrM4;			// Признаки М4
	unsigned short  usNviz;         // Порядковый номер визирования
	int				iX0v;			// Координаты центра визируемой области в Земной СК, ЦМР = 10^-2
    int				iY0v;
    int				iZ0v;
	unsigned char	ucRezerv[23];	// Резерв
} PACK M4;
//**************************************************

//***Структуры данных для передачи РК -> БЦВС-М***
typedef struct                          // VM3
{
    unsigned char   ucPrRLI;        // 1-ошибка подготовки РЛИ, 0-РЛИ подготовлены
    unsigned short  usNviz;         // Порядковый номер визирования
} PACK VM3;

typedef struct  					    // VM4
{
    unsigned char   ucType;         // Обозначение типа изображения
    unsigned char   ucNumCluster;	// Номер кластера изображения
    short           *p_sClusterImg;	// Кластер изображения (массив кластера по 2-байта на пиксель)
} PACK VM4;

typedef struct 					        // VM5
{
    unsigned char   ucType;		// Тип передаваемого элемента
    unsigned short  usKS;		// Контрольная сумма переданного элемента CRC16
} PACK VM5;
//*************************************************

//*** Структура входных параметров МСПО***
typedef struct                          // InputData
{
	unsigned short usNenormaC10;			// Количество неудачных получений сообщений С10

	//***Буферы СД входных данных по МКИО***
	unsigned short usPrizC10;                   // Последний принятый Приз сообщения С10
	unsigned short usInBufMKO_C10_pr1[32];		// СД в пришедшем КС по МКИО в C10 Приз = 1
	//*******************************************

	//***Входные данные от РК***
	unsigned char	bIsDataRK;			    // Признак получения данных от РК (true/false)

    size_t			stSizeDataRK;			// Размер входного пакета от РК в байтах
	unsigned short	usTypeC;				// Тип сообщения (С1 = 0х00С1, С3 = 0х00С3)
	unsigned int	uiID_OutPacketRK_C1;	// Порядклвый номер исходящего пакета РК C1
	unsigned int	uiID_OutPacketRK_C3;	// Порядклвый номер исходящего пакета РК C3
	unsigned int	uiID_InPacketBCVSM;		// Порядклвый номер последнего принятого сообщения
	unsigned int	uiISL;      			// Информационное слово
	sREG_RK         uRegNormRK; 			// Регистр норм
	unsigned short	usSizeVM;				// Размер входного массива ВМ
	unsigned char	*p_ucDataRK;       		// Данные входного пакета от РК
	//*****************************
} InputDataMSPO;
//**********************************************

//*** Структура выходных параметров МСПО ***
typedef struct                          // OutputData
{
	//***Буферы СД выходных сообщений по МКИО***
    unsigned short usBufX2_1;           // СД (1 - подадреса) для отправки по МКИО в СУС
    unsigned short usBufX2_2[2];        // СД (2 - подадреса) для отправки по МКИО в СУС

    unsigned short usBuf_OC07_10;		// СД (10 - подадреса) для отправки по МКИО ОС07
    unsigned short usBuf1_OC11_8[32];	// СД (8 - подадреса) для отправки по МКИО ОС11
    unsigned short usBuf2_OC11_8[32];	// СД (8 - подадреса) для отправки по МКИО ОС11

    unsigned short usBufDataMKIO[64];   // СД которые лежат в драйвере МКИО
	//******************************************

	//***Данные для отправки в РК***
    size_t          stSizeOutBufRK;	        // Размер отправляемого буфера в байтах
    unsigned int    uiID_OutPacketBCVSM;	// Порядклвый номер исходящего пакета
    unsigned int    uiID_InPacketRK;	    // Порядклвый номер последнего принятого сообщения от РК
    unsigned int    uiUSL;		            // Управляющее слово для РК
    unsigned int    uiNumPackBCVSM;		    // Номер исходящего пакета в РК
    unsigned char   *p_ucOutBufRK;          // Выходной буфер для РК
	//*********************************
} OutputDataMSPO;
//***********************************************

//*** Структура внутренних параметров МСПО ***
typedef struct				            // MuvMemoryData
{
    //***Тип выдаваемого кадра ТМИ***
    unsigned int    uiTypeKadrTMI;
    //*******************************

    //***Результаты экспресс-тестов БЦВС-М, инициализации МСПО***
    State_t         *pExpressTest;          // Результаты экспресс-тестов
    sINITIALSPO     uInitialSPO;            // Результаты инициализации МСПО
    sREGMSPO        sRegMSPO;				// Регистр состояния МСПО
    //*************************************************

	//+++Секция вектора состояния+++
    union
    {
        unsigned short  usPrC10;    // Признак данных в С10
        struct
        {
            unsigned short usNotC10 :1;     // Признак отсутствия полученного сообщения С10
            unsigned short usRezerv :13;    // Резерв
            unsigned short usPr     :2;
        } stPrC10;
    };
	float 			fPsi;		// Курс, рад (С10)
	float 			fThetta;	// Тангаж, рад (С10)
	float 			fGamma;		// Крен, рад (С10)
	float 			fX0;		// Координаты в земной системе координат
	float 			fY0;		//
	float 			fZ0;		//
	//++++++++++++++++++++++++++++++

	//***Переменные под время***
	double			dTime;				// Текущее время МСПО
	double 			dTimeStartTackt;	// Время МСПО на начало такта, мс
	double 			dTimeStopTackt;		// Время МСПО на конец такта, мс
	//**************************

	//***Переменные под управляющие, информационные слова***
	sUS             uUS;			// Текущее Ук или Ус
	sIS             uIS;			// Текущее информационное слово
	unsigned int	uiUSL;		    // Текущее управляющее слово для РК
	unsigned int	uiISL;		    // Текущее информационное слово РК
	//******************************************************

	//***Регистр норм***
	sREG           uRegNorm;		// Регистр норм
	//******************

	//***Переменные под состояния ЛВД***
    unsigned char	bTekState;			// Текущее состояние
	unsigned char	bNextState;			// Слудеющее состояние
	//**********************************

    //***Параметры для РЛИ***
    sRLI_MUV        sRLI;               // Структура для полученных РЛИ
	SerchEtalon		sEtalonTrue;		// Структура с данными выбранного эталона
	//***********************

	DATA_AS		    dataAS; 			// Структура данных коррекции
	ID_DATA			dataID; 			// ИД

    M4              dataM4;

    VM3				dataVM3;			// Данные ВМ3 (формуляр И) по результату визирования РК
    VM4				dataVM4;			// Данные ВМ4 (элемент И) по результату визирования РК
    VM5				dataVM5;			// Данные ВМ5 (заверешение передачи И) по результату визирования РК
} /*PACK*/ MuvMemoryData;
//*************************************************

typedef struct                          // InfoRLI_TMI
{
    float fCMR;                         // ЦМР

    union
    {
        unsigned int uiInfoRLI_TMI;			// Харктеристика передаваемого И
        struct
        {
            unsigned short  usRezerv:7;     // Резерв
            unsigned short  usPrRLI :8;
            unsigned short  usPrSK  :1;
            unsigned short  usKS    :16;    // Контрольная сумма И  - 15-0 биты
        } uInfoRLI_TMI;
    };
} InfoRLI_TMI;

typedef struct                          // DataTLM
{
    size_t stSizeDataOZU1;			// Размер отправляемых данных ОЗУ1
    size_t stSizeDataOZU2;			// Размер отправляемых данных ОЗУ2
    unsigned char *p_ucDataOZU1;	// Данные для кадра ТЛМ из ОЗУ1
    unsigned char *p_ucDataOZU2;	// Данные для кадра ТЛМ из ОЗУ1

	//***Выдаваемые РЛИ в банке 2***
	unsigned char   ucCntVision;        // Кол-во выполненных съемок
	char            cSendVision;        // Номер отправленных РЛИ съемок
	InfoRLI_TMI 	stInfoRLI[30];
	unsigned short  *p_usRLI_Amp;       // Амплитудное изображение (1 изображение * 10 визирований)
	//******************************
} PACK DataTLM;
#endif	// DATA_MSPO_H
