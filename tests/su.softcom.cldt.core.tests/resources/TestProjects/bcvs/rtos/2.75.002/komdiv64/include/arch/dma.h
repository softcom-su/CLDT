/*

*/
#ifndef _ARCH_DMA_H
#define _ARCH_DMA_H

/* Возможно эти константы зависят от архитектуры ?*/

#define arcDMA_MODE_READ   0x44    /* I/O to memory, no autoinit, increment, single mode */
#define arcDMA_MODE_WRITE  0x48    /* memory to I/O, no autoinit, increment, single mode */
#define arcDMA_MODE_CASCADE 0xC0   /* pass thru DREQ->HRQ, DACK<-HLDA only */

void dmaDisable(int dmanr);
void dmaClearFF(int dmanr);
void dmaSetMode(int dmanr,char mode);
void dmaSetAddr(int dmanr,unsigned long addr);
void dmaSetCount(int dmanr,int count);
void dmaEnable(int dmanr);
#endif                                  /* !_ARCH_DMA_H */
