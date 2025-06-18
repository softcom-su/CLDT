#ifdef FOR_SUBMICRON
   // min init RIO for bcvs if get_mode(gp_b[2] == 1)
   // no PMON initialization 

#define	BASE_RIO_REG	0xBA400000
#define	PORT0_RATE_EN	0xBA401044

// биты PORT0_ERR_STAT


#define OUTPUT_FAIL         (1<<25)
#define OUTPUT_DEG          (1<<24)
#define OUTPUT_ERR_ENCTR    (1<<17)
#define OUTPUT_ERR_STOP     (1<<16)
#define INPUT_ERR           (1<<9)
#define INPUT_ERR_STOP      (1<<8)
#define PORT_PST            (1<<3)
#define PORT_ERR            (1<<2)
#define PORT_OK             (1<<1)
#define PORT_UNIT           (1<<0)



/*
  Функция для инициализации rio
  Возвращаемое значение:
  0 - норма
  -1 - ошибка
  Примечание. Возвращает ошибку в случае если 
  TODO: переделать значения на универсальные через define определенные
*/
int rio_init_submic(void)
{
   char mode = 0;
   unsigned port_err_stat = 0;

   PRINTF("rio_init_submic: start ...  "); 
   rioRegs = (void*)RIO_REGS_BASE;

  // Проверяем завершил порт инициализацию и не выставил ли ошибки,
  // если инициализация завершена и ошибок нет, то продолжаем,
  // если есть, то завершаем ислнение функции и возвращаем код ошибки
   port_err_stat = CT_LE_L(rioRegs->ext.port[0].errStat);
   
//   printf(" debug: port_err_stat = 0x%x \n", port_err_stat);  // debug => del

   if((port_err_stat&PORT_UNIT)==PORT_UNIT)
   {
     return -1;
   }
   else
   {
     if((port_err_stat&(OUTPUT_ERR_STOP|INPUT_ERR_STOP|PORT_ERR))!=0)   //OUTPUT_FAIL|OUTPUT_DEG!!! Надо или нет
     {
        return -1;
     }
     // Сбросим установлиные биты (некоторые биты могут быть установлены после включения)   
     rioRegs->ext.port[0].errStat = CT_LE_L(port_err_stat); 

     port_err_stat = CT_LE_L(rioRegs->ext.port[0].errStat); // debug => del
//     printf(" debug: clear port_err_stat = 0x%x \n", port_err_stat);  // debug => del

     if((port_err_stat&(PORT_PST|PORT_OK))!= (PORT_PST|PORT_OK))
     {
       return -1;
     }
   }

  // check MODE
   mode = *IO_PIN_B; //*((char*)0xbb400258);  

   if((mode&0x4)!=2) //gmu: if no technical mode
   {
      // из описания      
      // установить аварийные таймауты PORT_LT_CTL,PORT_RT_CTL
      rioRegs->ext.ltCtl = CT_LE_L(0x080000); // значение взято из PMON
      rioRegs->ext.rtCtl = CT_LE_L(0x177000); // значение взято из PMON      
      // разрешить управление ошибками
      *((unsigned*)PORT0_RATE_EN) = CT_LE_L(0x80FE003F); // значение взято из PMON (разрешает формирование ошибок) 
      
      // разрешение фиксации ошибок логического и транспортного уровней  ( взято из PMON)
      rioRegs->errCtrl.logErrDetEn = CT_LE_L(0xFFC00000);
      
      // установить биты OUTPUT_EN,INPUT_EN  в регистре PORT0_CTL
      rioRegs->ext.port[0].ctl = CT_LE_L(0x44000000);

      // установить DEV_ID и DEV_ID_LOCK
      rioRegs->csr.baseDevID = CT_LE_L(0x00010001);
      rioRegs->csr.hbDevIDLock = CT_LE_L(0x00000001);
     
      // запрограммировать коммутатор:
      // PORT_ROUT_CFG_DSTID
      rioRegs->csr.routeDstID = CT_LE_L(2);
      // PORT_ROUTE_CFG_PORT
      rioRegs->csr.routePort = CT_LE_L(0); 

      PRINTF(" done \n");
   }   

  return 0;
}   
#endif
