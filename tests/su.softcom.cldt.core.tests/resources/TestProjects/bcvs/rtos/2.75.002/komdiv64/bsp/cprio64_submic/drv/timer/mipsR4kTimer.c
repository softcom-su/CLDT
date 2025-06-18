/* mipsR4kTimer.c - MIPS R4000 CPU internal timer library */

/*
modification history
--------------------
    04sep12,sok   add fine adjust.
    04may12,sok	  fix boardClockAdjust and boardClockGetTime.
    16jan12,sok   add boardClockAdjust.
*/

#ifdef	K64SMP
#define	CPU_TIMER_DIV	1
#else
#define	CPU_TIMER_DIV	2
#endif

/* locals */

static VOIDFUNCPTR boardClockRtn;	/* attached user ISR */
static int boardClockArg;		/* user ISR argument */
static unsigned boardTicks;		/* interrupt count */
static int boardClkRate;		/* clock rate in HZ */
#if OS_VERSION > 271 || (OS_VERSION == 271 && OS_RELEASE >= 29)
static long long boardFineRatio, boardFineZero, boardFineOne, boardCountPerSec, _boardCountPerSec, boardCountPerSecNom;
#endif
/* globals */

unsigned boardCompareVal = 0xffffffff;	/* CP0 Compare register value */

/******************************************************************************/

/* get system clock rate in Hz */

int boardClkRateGet(void)
{
    return boardClkRate;
}

/******************************************************************************/

/* get number of ticks between interrupts */

unsigned boardGetTimerFreq()
{
    return boardCompareVal;
}

/******************************************************************************/

/* set system clock rate */

int boardClockInit(int _hz)
{
    if(_hz <= 0 || _hz > boardCpuClockRateGet() / CPU_TIMER_DIV)
        return -1;

    boardCompareVal = (boardCpuClockRateGet() / CPU_TIMER_DIV) / _hz;
#if OS_VERSION > 271 || (OS_VERSION == 271 && OS_RELEASE >= 29)
    boardCountPerSecNom = boardCpuClockRateGet() / CPU_TIMER_DIV;
    boardCountPerSec = boardCountPerSecNom;
    _boardCountPerSec = boardCountPerSecNom;
#endif
    KPRINT("\n\rboardCompareVal=%i (_hz=%i boardCpuClockRateGet=%i CPU_TIMER_DIV=%i)\n\r", boardCompareVal, _hz, boardCpuClockRateGet(), CPU_TIMER_DIV);
    boardTimerHZ = boardClkRate = _hz ;
    return 0;
}

int boardClkRateSet(int _hz)
{
    return boardClockInit(_hz);
}

/******************************************************************************/

/* interrupt handler */

void boardClockInt(int arg)
{
    unsigned compare, count, tune = 0;

    TRACE(TR_INTR, (IV_TIMER<<16) | (boardTicks & 0xffff));
    boardTicks++;

    compare = boardCompareGet();

    /* we lost some ticks: adjust new compare value */

    if((count = boardCountGet()) - compare > boardCompareVal)
    {
#ifdef	BAD_INTS_LOGGING
      if (count < compare)
        { logMsg ("boardClockInt: count (%x) < compare (%x) !!!\n\r", count, compare,0,0,0,0);
           // cpuReboot();     /* !!!!!!!!!!!!!!!!!!!!!!!!! */
        }
      else
	logMsg("boardClockInt: count=%#x compare=%#x boardCompareVal=%#x\n\r",
	    count,compare,boardCompareVal,0,0,0);
#endif
        compare = count;
    }

#if OS_VERSION > 271 || (OS_VERSION == 271 && OS_RELEASE >= 29)

    if(boardFineRatio)
    {
        if(boardFineOne)
        {
            long long ratio = (boardFineZero << 16) / boardFineOne;

            if(ratio < boardFineRatio)
            {
                boardFineZero++;
            }
            else
            {
                boardFineOne++;
                tune = 1;
            }
        }
        else
        {
            boardFineOne = 1;
            tune = 1;
        }
    }

    boardCountPerSec = _boardCountPerSec;
#endif

    /* acknowledge interrupt and set new compare value */

    boardCompareSet(compare + boardCompareVal + tune);

    if(boardClockRtn)
        boardClockRtn(boardClockArg);

    TRACE(TR_INTR, (IV_TIMER<<16) | 0xffff);
}

/******************************************************************************/

/* connect a routine to the clock interrupt */

int boardClockConnect(int (rtn)(), int arg)
{
    boardClockArg = arg;
    boardClockRtn = (void*)rtn;
    return 0;
}

/******************************************************************************/

/* get clock interrupt handler */

int boardClockHandlerGet(FUNCPTR* rtn, int* arg)
{
    if(!rtn || !arg)
	return -1;
    *arg = boardClockArg;
    *rtn = (void*)boardClockRtn;
    return 0;
}

/******************************************************************************/

/* disable system clock */

void boardClockDisable(void)
{
    int mask = intLock();
#if defined(_MIPS_TUNE_RM7000) || defined(BT206) || defined(BT206RUS)
    int ic = boardIntControlGet();

    if(ic & IC_TE)
    {
	boardIntControlSet(ic & ~IC_IM_TIMER);
    }
    else
    {
	mask &= ~SR_IBIT8;
    }
#elif	defined(K64SMP)
    boardCauseSet(boardCauseGet() | 0x08000000);
#else
    boardConfigSet(boardConfigGet() & ~0x40000000);
#endif

    intUnlock(mask);
}

/******************************************************************************/

/* disable system clock */

void boardClockEnable(void)
{
    int mask = intLock();

#if defined(_MIPS_TUNE_RM7000) || defined(BT206) || defined(BT206RUS)

    int ic = boardIntControlGet();

    if(ic & IC_TE)
    {
	boardIntControlSet(ic | IC_IM_TIMER);
    }
    else
    {
	mask |= SR_IBIT8;
    }
#elif	defined(K64SMP)
    boardCauseSet(boardCauseGet() & ~0x08000000);
#else
    boardConfigSet(boardConfigGet() | 0x40000000);
#endif

    boardCountSet(0);
    boardCompareSet(boardCompareVal);
    boardCountSet(0);

    intUnlock(mask);
}

/******************************************************************************/

/* select timer interrupt source */

#if defined(_MIPS_TUNE_RM7000) || defined(BT206) || defined(BT206RUS)

void boardClockAltSource(int flag)
{
    int mask = intLock();

    if(flag)
    {
	boardIntControlSet(boardIntControlGet() | IC_TE);
    }
    else
    {
	boardIntControlSet(boardIntControlGet() & ~IC_TE);
    }

    intUnlock(mask);
}

#endif	/* defined(_MIPS_TUNE_RM7000) || defined(BT206) */

/******************************************************************************/

/* get interrupt count */

unsigned boardTickGet(void)
{
    return boardTicks;
}

/******************************************************************************/

/* set interrupt count */

void boardTickSet(unsigned val)
{
    boardTicks = val;
}

/******************************************************************************/

/* get number of ticks between interrupts */

int boardClkCntMaxGet(void)
{
    return boardCompareVal;
}

/******************************************************************************/

/* get number of ticks until next interrupts */

int boardClkCntGet(void)
{
    unsigned compare = boardCompareGet(), count = boardCountGet(), delta;

    delta = compare - count;

    if(delta <= boardCompareVal)
	return delta;

    if(delta + boardCompareVal <= boardCompareVal)
	return -delta;

    return boardCompareVal;
}
/*****************************************************************************************/
#if OS_VERSION > 271 || (OS_VERSION == 271 && OS_RELEASE >= 29)
/*
 * СРЕДСТВА СИНХРОНИЗАЦИИ СИСТЕМНЫХ ЧАСОВ
 *
 * Для синхронизации эталонные часы тем или иным образом подают сигнал,
 * который вызывает прерывание, через фиксированный интервал времени D (период).
 * (например, раз в секунду). При этом значение эталонных часов не сообщается.
 *
 * В результате работы функций синхронизации показания системных часов
 * постепенно изменяются с тем, чтобы их показания были кратны D
 * в момент прерывания от системных часов.
 *
 * Пользователь разрабатывает функцию обработки прерываний от эталонных часов.
 * Эта функция вызывает функцию корректировки системных
 * часов clockAdjust(), которая входит в состав ОС.
 * Аргументы функции clockAdjust() указывают (в наносекундах):
 *  - период эталонных часов D (время между двумя
 *    последовательными сигналами точного времени);
 *  - максимальная величина, на которую показания часов можно уменьшить
 *    за секунду;
 *  - максимальная величину, на которую показания часов можно увеличить
 *    за секунду;
 *  - пороговое значение при котором коррекция не производится
 *    (допустимая погрешность ).
 *
 * Функция clockAdjust() использует две функции ППМ:
 *   - функцию корректировки часов boardClockAdjust();
 *   - функцию опроса точного времени boardClockGetTime().
 * Функция boardClockAdjust() принимает два аргумента:
 *   - время, за которое погрешность должна быть устранена
 *     (обычно длина периода эталонных часов);
 *   - погрешность часов (положительная, если часы спешат,
 *     и отрицательная, если часы отстают).
 * Функция boardClockAdjust() корректирует показания часов путем изменения
 * длины тика в counts. Если часы спешат, то функция увеличивает
 * число единиц счетчика (counts) в тике (часы будут идти медленнее).
 * Если часы отстают, то то функция уменьшает число единиц счетчика в тике
 * (часы будут идти быстрее).
 * Такой способ корректировки показаний часов гарантирует гарантирует непрерывное
 * и монотонное изменение показаний часов при корректировке, что может оказаться
 * важным, например, при трассировке.
 *
 * Функция boardClockGetTime() позволяет опросить часы с максимальной точностью
 * (с точностью до 1 count).
 */

/* ------- boardClockAdjust - функция коррекции системных часов, ППМ, ос2000 -------
 * Аргументы:
 *   D    - время, за которое погрешность должна быть устранена
 *          (обычно длина периода, с каким эталонный таймер посылает прерывания),
 *          в нсек;
 *   diff - погрешность часов (положительная, если часы спешат,
 *          и отрицательная, если часы отстают), в нсек.
 * Функция вычисляет новое значение длины тика (в count) с тем, чтобы
 * чтобы погрешность была устранена в течении D нсек.
 * Функция может использоваться только для основных (системных) часов.
 */
	/*
	 * Для ликвидации погрешности diff за D наносекунд длина секунды
	 * должна быть установлена равной (в нсек)
	 * (т.е увеличена если diff > 0 и уменьшена в противном случае)
	 *
	 *    ((D + diff)/D)*1000000000LL
	 *                             = 1000000000LL + (diff*1000000000LL)/D.
	 *
	 * Если count1sec - номинальное (при отсутствии погрешности)
	 * число count в 1 секунде, то для устанения погрешности за период D длина
	 * секунды должна быть установлена равной (в count)
	 *
	 *    count1sec + (count1sec*diff)/D.
	 *
	 * Если часы посылают прерывания процессору с частотой boardTimerHZ,
	 * то длина тика (в count) равна
	 *
	 *    (count1sec + (count1sec*diff)/D)/boardTimerHZ.
	 *
	 * Новая (расчетная) длина тика будет использоваться
	 * после очередного прерывания от часов
	 * (для программирования часов и вычисления точного времени).
	 * До этого момента используется старая (текущая) длина тика
	 * (для вычисления точного времени).
	 * Таким образом следует хранить текущую и расчетную длину тика.
	 * При очередном прерывании от часов текущая длина тика замещается расчетной.
     *
     * Для реализации данной функции следуется разработать функцию ППМ,
     * которая возвращает число count в секунду.
     */

#if (OS_VERSION == 271 && OS_RELEASE == 32) || OS_VERSION > 272
void boardClockAdjust(system_time_t D, int diff)
#else
system_time_t boardClockAdjust(system_time_t D, int diff)
#endif
{
    long long count1sec;
    unsigned err;

    count1sec = boardCountPerSecNom + (boardCountPerSecNom*diff)/(long long)D;
    boardCompareVal = (unsigned)(count1sec / boardClkRate);
    err = (unsigned)(count1sec % boardClkRate);

    _boardCountPerSec = count1sec;

    boardFineRatio = err ? (((boardClkRate - err) << 16) / err) : 0;
    boardFineZero = 0;
    boardFineOne = 0;
    
#if (OS_VERSION == 271 && OS_RELEASE == 32) || OS_VERSION > 272
    return;
#else
    return (system_time_t) boardCompareVal;
#endif
}

unsigned long long boardClockCountPerSecGet(int nom)
{
    return nom ? boardCountPerSecNom : boardCountPerSec;
}

/*---------------------------------------------------------------------------*/
/*----- boardClockGetTime - функция опроса точного времени, ППМ, ос2000 -----
 * Функция вычисляет значение текущего времени исходя из числа,
 * прошедших прерываний от системных часов (тиков), и
 * числа count, прошедших с момента последнего прерывания от системных часов
 * (от начала последнего тика).
 *
 * Функция может использоваться только для основных (системных) часов.
 */
   // Искомое время
   //   t = t1 + t2, где
   //   t1 - время, прошедшее до предыдущего прерывания от часов (в нсек);
   //   t2 - время, прошедшее от предыдущего прерывания от часов
   //   до настоящего момента (в нсек).
   //
   //   t1 = tickNumber * 1000000000LL / boardTimerHZ, где
   //   tickNumber - число тиков (прерываний от часов),
   //   произошедших к настоящему моменту.
   //   boardTimerHZ - частота, с которой часы посылают прерывания
   //   (количество тиков в секунду).
   //
   //        currcount   1000000000LL
   //   t2 = --------- * ------------ =
   //        tickcount   boardTimerHZ
   //
   //   ((currcount * 1000000000LL)/tickcount)/boardTimerHZ, где
   //   currcount - время, прошедшее с момента предыдущего тика в count;
   //   tickcount - длина текущего тика в count.
   //
   //    Для реализации данной функции следуется разработать функцию ППМ,
   //    которая возвращает число count, прошедших с начала текущего тика.
   //    Длина следующего тика в count вычисляется при вызове
   //    функции boardClockAdjust, а длина текущего тика в count определяется
   //    при обработке прерывания от таймера.


int boardClockOverflow = FALSE;

system_time_t boardClockGetTime(void)
{
    unsigned mask, count, compare, ticks;
    int delta;
    long long t1, t2;

    mask = intLock();
    ticks = boardTicks;
    compare = boardCompareGet();
    count = boardCountGet();
    delta = compare - count;
    if(delta < 0)
    {
	delta = - delta;
	ticks++;
    }
    else
    {
	if(delta > boardCompareVal)
	    goto fail;
	delta = boardCompareVal - delta;
    }
    if(delta > boardCompareVal)
    {
fail:
	boardClockOverflow = TRUE;
//	kprint("%s: count=%d compare=%d boardCompareVal=%d delta=%d\n\r",__FUNCTION__,count,compare,boardCompareVal,delta);
	delta = boardCompareVal;
    }
    intUnlock(mask);
    t1 = (long long)ticks*1000000000LL/boardClkRate;
    t2 = (long long)delta*1000000000LL/((boardCpuClockRateGet()/CPU_TIMER_DIV));
    return t1 + t2;
}

#endif
