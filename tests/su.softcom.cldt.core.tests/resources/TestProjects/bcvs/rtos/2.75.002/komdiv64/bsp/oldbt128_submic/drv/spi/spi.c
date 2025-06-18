#include "spiLib.h"
/*
  Функция для чтения данных из ПЗУ по интрефейсу spi
  Параметры:
  adr_from - адрес начала области в ПЗУ
  adr_to - адрес размещения считанных данных
  size - количество байт, которые нужно считать 
*/
void spi_flash_read(unsigned adr_from,unsigned adr_to,unsigned size)
{
    int i;
    unsigned stat = 0;

    // printf(" debug: start spi_flash_read(0x%x,0x%x,0x%x)\n", adr_from,adr_to,size );

    // запретим прерывания
     *SPI_IEN_REG = 0;
    // установим значения
     *SPI_TIME_REG = (0x8 & SPI_TIME_CLOCK_DIV_MASK); // TODO: 8=> другой коэфициент, поменьше скорость выше 
    // завершим текущие передачи и очистим FIFO
     *SPI_CTL_REG = (SPI_CTRL_CTXF|SPI_CTRL_CRXF|SPI_CTRL_ETX|SPI_CTRL_ERX);

    // установим режим работы ( set up mode, and active chip select signal)
     *SPI_MODE_REG = (SPI_MODE_SOE|SPI_MODE_CPH|SPI_MODE_CPO|SPI_MODE_MST);
     *SPI_MODE_REG |= SPI_MODE_SOV;

    // модифицируем адрес ( 24 =>23, из-за изменний в приборах, и описанных адресных пространствах)  

      adr_from = (adr_from|((adr_from&0x1000000)>>1))&0xFEFFFFFF;      

     *SPI_CTL_REG = (SPI_CTRL_CTXF|SPI_CTRL_CRXF|SPI_CTRL_ETX|SPI_CTRL_ERX);			
  
     // сбросим признаки прерываний	
     *SPI_IS_REG = -1;	
      
     *SPI_IS_REG = SPI_IS_TXD;

      adr_from = CF_LE_L(adr_from);

      *SPI_TXF_REG = ((adr_from>>8)<<8)+3;

      *SPI_CTL_REG = SPI_CTRL_STX | (0x4 & SPI_CTRL_TRF_COUNT_MASK);

      // ждем завершения передачи
      do{
      stat = *SPI_IS_REG;
      }while((stat&SPI_IEN_TXD)==0);


      // читаем по 4 байта
      for(i=0;i<(size>>2);i++)
      {
        *SPI_CTL_REG = SPI_CTRL_TXDM | SPI_CTRL_STX | SPI_CTRL_SRX | (0x4 & SPI_CTRL_TRF_COUNT_MASK);
        do
        {
           stat = *SPI_FSTAT_REG;
        }while((stat&SPI_FSTAT_RXF_NUM_ENTRIES_MASK)==0);

        *((unsigned*)adr_to) = CF_LE_L(*SPI_RXF_REG);
         adr_to+=4;
      }

      *SPI_CTL_REG = (SPI_CTRL_ETX | SPI_CTRL_ERX);// end transmitting and receiving 
      stat = *SPI_MODE_REG;
      *SPI_MODE_REG = stat&(~(SPI_MODE_SOV)); // inactive chip select signal 
  // printf(" debug: end spi_flash_read \n");
}



