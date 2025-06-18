#ifndef MY_TIMER
#define MY_TIMER

#define TYPE_SEC	0	// Тип возвращаемого времени в секундах
#define TYPE_MSEC	1	// Тип возвращаемого времени в мсекундах
#define TYPE_MKSEC	2	// Тип возвращаемого времени в мксекундах
#define TYPE_NSEC	3	// Тип возвращаемого времени в нсекундах

int     iInitTimer(float fMSec, unsigned char bIsStart, long lTick);    // Инициализация таймера
int     iGetHzTimer();                                                  // Получить частоту срабатывания таймера
double  dGetMSecTimer();                                                // Получить точность срабатывания таймера в мс
int     iStartTimer();                                                  // Запуск таймера
int     iStopTimer();                                                   // Остановка таймера
double  dGetTime(int iTypeTime);                                        // Получить время
double  dGetTimeCNT(int iTypeTime);                                     // Получить время от подачи питания
void    vSleepMSec(double dMSec);                                       // Задержка
int     iIsTime(double dStartTime, double dFixTime);                    // Определение прохождения заданного времени
void    vWaitC10orTimeOver(sem_t *p_Sem, double dTimeOver);             // Ожидание сообщения С10

#endif	// MY_TIMER
