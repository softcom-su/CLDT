
#ifndef inc_exptst_h
#define inc_exptst_h

#define AREA_FOR_EXPTST_INFO_ADR     0xA000EFC0 
#define AREA_FOR_EXPTST_INFO_SIZE    0x40 

typedef struct {	
unsigned gpio            /*значение, считанное с интерфейса GPIO при запуске БСПО*/;
unsigned startExpTstInfo;/* Адрес начала массива обобщенной информацией о неисправностях обнаруженных во время проведения экспресс тестирования*/
unsigned freeExpTstInfo; /* Адрес следующей свободной ячейки массива ExpTstInfo*/
unsigned err[2];  /*битовый вектор результатов экспресс-тестов*/
unsigned tstDone[2]; /*битовый вектор номеров выполненных экспресс-тестов */
unsigned reserv[11]; /*резерв*/
}State_t;

unsigned exptstGetResult(void);    //функция для получения результатов тестов
State_t * exptstGetState(void); //функция для получения обобщенной информации

#endif /*inc_exptst_h*/
