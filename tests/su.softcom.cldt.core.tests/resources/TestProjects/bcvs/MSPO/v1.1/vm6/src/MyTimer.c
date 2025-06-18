#include "GlobalMSPO.h"			// Глобальные переменные и инклуды

volatile long   g_lTick		    = 0;
int             g_iHz		    = 0;
float           g_fInitMSec     = 0;
unsigned char   g_bIsInitTimer	= FALSE;

/*
Обработчик срабатывания таймера
Входные параметры:
	arg	- не используется, необходим параметр для функции обработчика таймера
*/
void vTickNow(int arg)
{
	g_lTick++;
}

/*
Инициализация таймера 5-02-03
Входные параметры:
	uiMSec		- частота работы таймера в мсекундах
	bIsStart	- признак включать таймер или нет
Возвращаемые занчения:
	1	- инициализация уже была выполнена, требуется остановить и заново проинициализировать
	2	- не удалось подключить функцию обработчика таймера
	3	- данная частота срабатывания таймера не поддерживается (1 < Гц < 2.5Мгц)
	4	- не удалось установить частоту срабатывания таймера
	0	- таймер проинициализирован
*/
int iInitTimer(float fMSec, unsigned char bIsStart, long lTick)
{
    g_lTick = lTick;

    if (g_bIsInitTimer == TRUE)			// Если уже была инициализация, то выходим, либо нужно остановить и проинициализировать заново
		return 1;

    if (boardAux2ClkConnect((FUNCPTR)vTickNow, 1) != 0)	// Подключение обработчика срабатывания таймера 5-02-03
		return 2;

	g_fInitMSec = fMSec;					// Сохраним себе в глобальную переменную инициализированную частоту срабатывания в мс

	g_iHz = 1.0f / (fMSec / 1000.0f);		// Высчитаем частоту срабатывания в Гц

	if (g_iHz < 1 || g_iHz > 2500000)		// Если частота не укладывается в способности таймера, то валим
		return 3;

	if (boardAux2ClkRateSet(g_iHz) != 0)	// Инициализируем частоту работы таймера 5-02-03
		return 4;

    g_bIsInitTimer = TRUE;					// Выставим признак инициализации таймера

    if (bIsStart == TRUE)					// Если требуется включить таймер
		boardAux2ClkEnable();

	return 0;
}

/*
Получение инициализированной частоты таймера 5-02-03 в Гц
Возвращаемые занчения:
	0	- таймер не был проинициализирован
	>0	- частота таймера в Гц
*/
int iGetHzTimer()
{
    if (g_bIsInitTimer == TRUE)
		return g_iHz;
	else
		return 0;
}

/*
Получение инициализированной частоты таймера 5-02-03 в мс
Возвращаемые занчения:
	0	- таймер не был проинициализирован
	>0	- частота таймера в мс
*/
double dGetMSecTimer()
{
    if (g_bIsInitTimer == TRUE)
		return 1.0 / g_iHz * 1000.0;
	else
        return 0.0;
}

/*
Запуск таймера 5-02-03
Возвращаемые занчения:
	0	- таймер запущен
	1	- таймер не был проинициализирован
*/
int iStartTimer()
{
    if (g_bIsInitTimer == TRUE)
	{
		boardAux2ClkEnable();
		return 0;
	}
	else
		return 1;
}

/*
Остановка таймера 5-02-03
Возвращаемые занчения:
	0	- таймер остановлен
	1	- таймер не был проинициализирован
*/
int iStopTimer()
{
    if (g_bIsInitTimer == TRUE)
	{
		boardAux2ClkDisable();

		g_lTick			= 0;
		g_iHz			= 0;
		g_fInitMSec	    = 0;
        g_bIsInitTimer	= FALSE;

		return 0;
	}
	else
		return 1;
}

/*
Поучение текущего значения таймера 5-02-03
Входные параметры:
	iTypeTime		- тип выдаваемого времени:
		TYPE_SEC	- в секундах
		TYPE_MSEC	- в мсекундах
		TYPE_MKSEC	- в мксекундах
		TYPE_NSEC	- в нсекундах
Возвращаемые занчения:
	-1.0	- таймер не был проинициализирован
	>=0.0	- текущее время таймера 5-02-03
*/
double dGetTime(int iTypeTime)
{
	//double l_dSec = g_lTick * 1.0 / g_iHz;

    if (g_bIsInitTimer == FALSE)
		return -1.0;

//    printf("%d\n", g_lTick);

    switch (iTypeTime)
	{
        case TYPE_SEC:
			return (g_lTick * 1.0 / g_iHz);					// Вернет в секндах
		default:
		case TYPE_MSEC:
			return (g_lTick * 1.0 / g_iHz) * 1000.;			// Вернет в мсекндах
		case TYPE_MKSEC:
			return (g_lTick * 1.0 / g_iHz) * 1000000.;		// Вернет в мксекндах
		case TYPE_NSEC:
			return (g_lTick * 1.0 / g_iHz) * 1000000000.;	// Вернет в нсекндах
	}
}

/*
Поучение текущего времени от подачи питания
Входные параметры:
	iTypeTime		- тип выдаваемого времени:
		TYPE_SEC	- в секундах
		TYPE_MSEC	- в мсекундах
		TYPE_MKSEC	- в мксекундах
		TYPE_NSEC	- в нсекундах
Возвращаемые занчения:
	>=0.0	- время от подачи питания
*/
double  dGetTimeCNT(int iTypeTime)
{
    unsigned int ui1007h;

    ui1007h = pscRead(0x1007);

    double dTimeCNT_MKS = (ui1007h >> 20) * 1000000.0 + ((ui1007h >> 1) & 0x0007FFFF) * 3.2;

    switch (iTypeTime)
	{
        case TYPE_SEC:
			return dTimeCNT_MKS / 1000000.;			// Вернет в секндах
		default:
		case TYPE_MSEC:
			return dTimeCNT_MKS / 1000.;			// Вернет в мсекндах
		case TYPE_MKSEC:
			return dTimeCNT_MKS;		            // Вернет в мксекндах
		case TYPE_NSEC:
			return dTimeCNT_MKS * 1000.;        	// Вернет в нсекндах
    }
}

/*
Выработка задержки в мс
Входные параметры:
	uiMSec		- задержка в мс
Примечание. До вызова данной функции необходимо выполнить инициализацю таймера 5-02-03 с частотой в 1 мс
*/
void vSleepMSec(double dMSec)
{
    unsigned char l_bIsTime = FALSE;
    double l_dStartMSec = dGetTime(TYPE_MSEC);

    while (l_bIsTime == FALSE)
	{
        if (dGetTime(TYPE_MSEC) - l_dStartMSec >= dMSec)
            l_bIsTime = TRUE;
	}
}

/*
Определение прохождения заданного времени
Входные параметры:
   dStartTime    - начальное время в мс от которого необходимо засеч интервал
   dFixTime      - сколько необходимо ждать в мс
Выходные данные:
    1   - время прошло
    0   - время не прошло
    -1  - таймер не запущен
*/
int iIsTime(double dStartTime, double dFixTime)
{
    if (g_bIsInitTimer == FALSE)
        return -1;

    double        l_dTimeNow    = 0.0;

    l_dTimeNow = dGetTime(TYPE_MSEC);

    if ((l_dTimeNow - dStartTime) >= dFixTime)
    {
        //printf("%f - %f\n", s_dTimeStart, l_dTimeNow);
        return 1;
    }
    else
        return 0;
}

/*
Функция ожидания прихода сообщения С10 по М3 либо истечение указанного интервала времени
Входные данные:
	p_Sem	    - указатель на семафор на получение сообщения С10
	dTimeOver   - вермя в мс которое необходимо ждать в случае неполучения сообщения С10
Возвращаемые значения:
	-
*/
void vWaitC10orTimeOver(sem_t *p_Sem, double dTimeOver)
{
    int     l_iRetSemC10        = -1;
    int     l_iIsTime           = 0;
    double  l_dStartTimeWaitC10 = dGetTime(TYPE_MSEC);

    do
    {
        l_iRetSemC10 = sem_trywait(p_Sem);
        l_iIsTime = iIsTime(l_dStartTimeWaitC10, dTimeOver);
    }
    while (l_iRetSemC10 != 0 && l_iIsTime == 0);

    if (l_iRetSemC10 == 0)
        g_bC10 = TRUE;
    else
        g_bC10 = FALSE;
}
