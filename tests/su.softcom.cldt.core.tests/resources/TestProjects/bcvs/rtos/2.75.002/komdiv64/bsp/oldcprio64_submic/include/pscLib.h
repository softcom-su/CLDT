#ifndef inc_pscLib_h
#define inc_pscLib_h

typedef union {
 unsigned wrd;
 struct { unsigned 
    r0           :12, //31…20 Резерв
    sVM6LIVEINC  : 4, //19…16 Интегрированный код годности 1890ВМ6Я
    r1           : 3, //15…13 Резерв
    sTLMBANK     : 1, //12 Флаг 2 ТМК
    sTLMREADY    : 1, //11 Флаг 3 ТМК
    sTLMSTARTPCI : 1, //10 Флаг 1 ТМК
    sA           :10; //9…0 Количество пакетов ОЗУ 1 ТМК
 };
} psc_1000h_t;


typedef union {
 unsigned wrd;
 struct { unsigned 
    r31          :12, //31…20 Резерв
    sVM7LIVEINC  : 4, //19…16 Интегрированный код годности 1890ВМ7Я
    r15          : 3, //15…13 Резерв
    sTLMBANK     : 1, //12 Флаг 2 ТМК
    sTLMREADY    : 1, //11 Флаг 3 ТМК
    sTLMSTARTPCI : 1, //10 Флаг 1 ТМК
    sB           :10; //9…0 Количество пакетов ОЗУ 2 ТМК
 };
} psc_1001h_t;



//Регистры управления MRAM
typedef union {
 unsigned wrd;
 struct { unsigned 
    START   : 1, //31    Флаг инициирования процесса однократной зп/чт
    LOOP    : 1, //30    Флаг инициирования процесса циклической зп/чт
    READY   : 1, //29    Флаг готовности контроллера к процессу зп/чт
    sMRAMNW : 9, //28…20 Количество слов
    sMRAMAW :20; //19…0  Смещение в адресном пространстве MRAM
 } fld;
} psc_mram_reg_t;


//Адресное пространство блоков TLM и RAM2 при доступе к ним через PCI
typedef struct {
 unsigned RAM2[4][512]; //Кэш стойкой памяти
 unsigned TLM[2][1024]; //Телеметрия

 psc_1000h_t _1000h;
 psc_1001h_t _1001h;

 psc_mram_reg_t _1002h;
 psc_mram_reg_t _1003h;
 psc_mram_reg_t _1004h;
 psc_mram_reg_t _1005h;

 //1006:
 unsigned  sTMIWRD; //31…0 Аппаратное состояние ТМИ
 //1007:
 unsigned  sTIMECNT;//31…1 Время с момента включения в 3,2 мкс 
                    //0    Признак отсутствия сигнала метки времени
 //1008:
 unsigned  sMOVECNT;//31…1 Время с момента начала движения в 3,2 мкс 
                    //0    Признак начала движения, сигнал sMOVE
 //1009:
 unsigned  sSSSSCNT;//31…1 Время с момента окончания воздействия ПФ в 3,2 мкс 
                    //0    Признак окончания воздействия ПФ, сигнал sENDVOZ
} pscAS_t;

//Описатель PSC(PCI Slave controller)
typedef struct {
 pscAS_t * cpuAdr; //базовый процессорный адрес
 unsigned  pciAdr; //соответствующий PCI-адрес
 int bus;
 int dev;
 int func;
} pscDesc_t;

extern pscDesc_t pscDesc;

int  pscLibInit(void);
void pscLibLiveInc(unsigned n);
void pscLibInitLiveInc(unsigned num_timer,unsigned timer_period,unsigned time_LiveInc);

#define PSC_READ(nWord)        CF_LE_L(*((unsigned*)pscDesc.cpuAdr + nWord))
#define PSC_WRITE(nWord,Word)  *((unsigned*)pscDesc.cpuAdr + nWord) = CF_LE_L(Word)


#endif /*inc_pscLib_h*/
