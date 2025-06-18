#ifdef FOR_SUBMICRON

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

   // min init RIO for bcvs if get_mode(GPIO[2] == 1)
   // no PMON initialization
 
int rio_init_submic(void)
{
   unsigned mode = 0;
   unsigned port_err_stat = 0;

   PRINTF(" rio_init_submic: start ... ");

  // Проверяем завершил порт инициализацию и не выставил ли ошибки,
  // если инициализация завершена и ошибок нет, то продолжаем,
  // если есть, то завершаем ислнение функции и возвращаем код ошибки
   port_err_stat = *RIO_PL_PORT0_ERR_STAT_REG;  

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
     *RIO_PL_PORT0_ERR_STAT_REG = port_err_stat; 
    
     port_err_stat = *RIO_PL_PORT0_ERR_STAT_REG; // debug => del
//     printf(" debug: clear port_err_stat = 0x%x \n", port_err_stat);  // debug => del

     if((port_err_stat&(PORT_PST|PORT_OK))!= (PORT_PST|PORT_OK))
     {
       return -1;
     }
   }

  // check MODE
   mode = *((unsigned *)0xbff00700);  // gpio  
 
   if((mode&0x7)!=2) //gmu: if no technical mode
   {
      // установим  RIO_PL_PORT0_CTL_REG
      *RIO_PL_PORT0_CTL_REG = 0x44000800; 

      // установить DEV_ID и DEV_ID_LOCK
      *RIO_CSR_BASE_DEV_ID_REG = 0x00020000;  
      *RIO_CSR_HB_DEV_ID_LOCK_REG = 0x2; 
      
      // установим PKT_CNT0 т PKT_CNT1
      *RIO_PKT_CNT0_REG = 0x14;
      *RIO_PKT_CNT1_REG = 0x14;

      //(аварийные таймауты и прочее настриваются настраиваются в rioInit)
      PRINTF(" done \n");
   }  
  return 0; 
}   
#endif
