#include <pscLib.h>

pscDesc_t pscDesc; // описатель PSC  


inline unsigned _pscRead(unsigned nWord)
{
   register unsigned val;
   val = *((unsigned*)pscDesc.cpuAdr + nWord);
   return CF_LE_L(val);
}

/*
	инициализация PSC на шине PCI
        Функция находит устройство на шине PCI и заполняет структуры для доступа к PSC
        Если регистры BAR(0x10) или status(0x4)  для PSC не настроеын, то функция возвращает -1 ( в errno код не сохраняется)
 ВОЗВРАТ:
  0 - норма
 -1 - errno:
        ENODEV - устройство не обнаружено на шине PCI       
*/
int pscLibInit(void)
{
  int res;
  pscDesc_t * p=&pscDesc;
  


  if(p->cpuAdr!=NULL)return 0;  

// Устанавливаем признак что контроллер может быть мастером на шине PCI
// Если этот признак не будет установлен мы не сможем получить доступ 
// к конфигурационным регистрам PCI.
// TODO: разобраться почему во время инициализации он сбрасывается
  *PCI_ARBITER_REG = 0x20;
// После установки этого бита, возможность установки/сброса внутреннего мастера
// со стороны pci(регистр command(0x4)) теряется.

 //Найдём PSC по идентификатору (0xBAFC2000 - "Manual PSC-01 17.04.2013.doc")
 res=pciDeviceFind(0xBAFC2000, 0, &p->bus, &p->dev, &p->func);
 if(res == -1) {
    errno = ENODEV;  //Нет устройства
    return -1;
 }

// 1890ВМ7 настраивает регистры PSC, не анализируя, что на них установлено (1890ВМ6 - устанавливает в тоже значение)
 
//PSC требует 4МБ (заранее выделено в начале пространства PCI)
 p->cpuAdr=(pscAS_t*)PCI_MEM_BASE; //процессорный адрес
 //printf(" debug: p->cpuAdr = 0x%x \n",p->cpuAdr); // debug!!!!
 if(p->cpuAdr==NULL) {
    errno = ENOMEM; //Недостаточно памяти
    return -1;
 }
 p->pciAdr=pciLocalToMem((unsigned)p->cpuAdr); //адрес PCI который должен быть у PSC
  
 pciConfigOutLong(p->bus, p->dev, p->func, 0x10, p->pciAdr);
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

