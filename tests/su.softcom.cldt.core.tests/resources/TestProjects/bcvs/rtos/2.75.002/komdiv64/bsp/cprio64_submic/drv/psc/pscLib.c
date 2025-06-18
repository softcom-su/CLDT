#include <pscLib.h>

/* Выставляем готовность из ВМ6 как для ВМ6 так и для ВМ7 изходя из тех соображений что в случае*/
pscDesc_t pscDesc; // описатель PSC

  
// глобальные переменные 
unsigned counter_timer_pscLiveInt_start;  // счетчик запусков обработчика прерываний от таймера
unsigned count_timer_pscLiveInt;  //количество отсчитываемых запусков обработчика
unsigned time_start_timer_pscLiveInt;  //время запуска таймера

///////////////////////////////////////////////////////////////////////////////
//TODO: перенести в drv\timer\sc64Timer.c
//nn=0-4 - natural numbering
int  boardAuxNNClkConnect(int nn,FUNCPTR rtn, intptr_t arg) { return tmrConnect(nn,(VOIDFUNCPTR)rtn,arg); }
void boardAuxNNClkDisable(int nn) { tmrDisable(nn); }
void boardAuxNNClkEnable(int  nn) { tmrEnable(nn);  }
int  boardAuxNNClkRateGet(int nn) { return timer[nn].rate; }
int  boardAuxNNClkRateSet(int nn,int _hz) { return sc64TimerRateSet(nn,1,_hz,TIMER_MODE_CYCLIC); }
///////////////////////////////////////////////////////////////////////////////


int init_timer_pscLive(int num_timer,int freq);

/*
   Функция для инициализации механизма выставления признака годности в течении заданного времени
   Функция инициализирует регистры PSC для дотупа по PCI, настраивает и запускает указанный таймер с периодом,
   указанным в параметрах. Обработчик прерывания будет обновлять признак годности в течении указанного времени.
   По истечении времени таймер будет отключен.
   Примечание. Параметр time_LiveInc должен быть кратен timer_period, если будет указан не кратный, то он будет выровнен.
               Переод  срабатывания таймера переводится в Гц ( при переводе возможно возникновение погрешности, 
               это нужно учитывать при задании интервала в мс)
*/
void pscLibInitLiveInc(
   unsigned num_timer,    /*=1-5 - номер используемого таймера*/
   unsigned timer_period, /* период срабатывания таймера в мс */
   unsigned time_LiveInc  /* период выставления признак годности в мс*/
 )
{
//ВНИМАНИЕ! В этом файле принята естественная нумерация таймеров 0-4;
//А в этой функции - противоестественная 1-5, чтобы совпадала c противными
//нименованиями функций ОС2000  boardAux5...
    unsigned freq;    
    unsigned res;

    if((timer_period!=0)||(time_LiveInc!=0))
    {
        // инициализация PSC на шине PCI
        //printf(" PSC: PCI Slave controller\n");
        pscLibInit();
        //printf(" PSC: bus=%d  dev=%d  func=%d  adr=0x%08x\n", p->bus, p->dev, p->func, (int)p->cpuAdr);

        // установим начальное значение 
        // !!!!!! пока устанавливаем как для ВМ6, так и для ВМ7 из ВМ6 TODO: продумать  
        //для ВМ6      
        pscLibLiveInc(0);//вместо PSC_WRITE(0x1000,0);
        //для ВМ7     
        pscLibLiveInc(1); //вместо PSC_WRITE(0x1001,0);
 
        time_start_timer_pscLiveInt = boardCountGet(); //gmu:debug
        // инициализация таймера
        // инициализируем глобальные переменные используемые обработчиком прерываний
        counter_timer_pscLiveInt_start = 0;
        count_timer_pscLiveInt = time_LiveInc/timer_period;
        // вычислим частоту срабатывания таймера в герцах 
        freq = 1000/timer_period;
        res=init_timer_pscLive(num_timer-1,freq);
        assert(res==0); //TODO: как проверять?        
    }    
}

/*
	инициализация PSC на шине PCI
 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        ENODEV - устройство не обнаружено на шине PCI
        ENOMEM - недостаточно памяти в области PCI
*/
int pscLibInit(void)
{
  int res;
  pscDesc_t * p=&pscDesc;

 if(p->cpuAdr!=NULL)return 0;  

 //Найдём PSC по идентификатору (0xBAFC2000 - "Manual PSC-01 17.04.2013.doc")
 res=pciDeviceFind(0xBAFC2000, 0, &p->bus, &p->dev, &p->func);
 if(res == -1) {
    errno = ENODEV;  //Нет устройства
    return -1;
 }

 //PSC требует 4МБ ( заранее выделено в начале пространства PCI)
 p->cpuAdr=(pscAS_t*)PCI_MEM0_BASE; //процессорный адрес
 if(p->cpuAdr==NULL) {
    errno = ENOMEM; //Недостаточно памяти
    return -1;
 }

 //Заполним BAR
 p->pciAdr=pciLocalToMem((unsigned)p->cpuAdr); //адрес PCI
 pciConfigOutLong(p->bus, p->dev, p->func, 0x10, p->pciAdr);

 //Разрешаем выполнение Memory Read, Memory Write в режиме Target
 pciConfigOutLong(p->bus, p->dev, p->func, 0x4, 0x00000002);
 
 return 0;
}

/*
     инкрементирует интегрированный код годности 
*/
void pscLibLiveInc(unsigned n) /* n 0 - для 1890ВМ6Я, 1 - для 1890ВМ7Я*/
{
 psc_1000h_t w0;
 psc_1001h_t w1;

 n&=1;

 if(n==0) {
    w0.wrd=PSC_READ(0x1000);
    w0.sVM6LIVEINC++;
    PSC_WRITE(0x1000,w0.wrd);
 }else{
    w1.wrd=PSC_READ(0x1001);
    w1.sVM7LIVEINC++;
    PSC_WRITE(0x1001,w1.wrd);
 }
}

/*
   Обработчик прерываний от таймера
*/
void timer_pscLiveInt(int arg)
{   
   counter_timer_pscLiveInt_start++;
   if(counter_timer_pscLiveInt_start<=count_timer_pscLiveInt)
   { 
      // утанавливаем признак годности
      // !!!!!! пока устанавливаем как для ВМ6, так и для ВМ7 из ВМ6 TODO: продумать
      pscLibLiveInc(0);
      pscLibLiveInc(1);
      //logMsg(" !!! pscLiveInc (count = 0x%x) \n",boardCountGet(),0,0,0,0,0);
   }
   else
   {
      // отключаем таймер
      boardAuxNNClkDisable(arg);
   }
}
/*
   Функция для инициализации и запуска дополнительного таймера
   В качестве параметра freq передается частота срабатываения таймера
   должна быть создана функция  обработчик прерываний от таймера
   Возвращаемое значение:
   0 -  если все хорошо
   -1 - при инициализации были ошибки
*/
int init_timer_pscLive(
   int n, /*=0-4      номер используемого таймера*/
   int f  /*=1-1024 - частота срабатывания в Гц */
 )
{
 int res=-1;
 int lock;   

 if( !((n>=0) && (n<=   4)) ||
     !((f>=1) && (f<=1024)) ){
    errno=-EINVAL;
    return res;
 }

 lock = intLock();
 // отключаем таймер
 boardAuxNNClkDisable(n);
 // подключаем обработчик прерываний для таймера
 if( boardAuxNNClkConnect(n,(void*)timer_pscLiveInt, n) == 0 ) {
    // задаём частоту срабатывания таймера    
    if( boardAuxNNClkRateSet(n,f) == 0 ) {
       // включаем таймер
       boardAuxNNClkEnable(n);
       res=0;
    }
 }
 intUnlock(lock);

 return res;
}
