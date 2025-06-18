#ifndef MUV_BCVSM_H
#define MUV_BCVSM_H

int iSetNULLtoMUV();            // Обнуляем регистр MRAM
int iOZUtoMUV(MuvMemoryData*);  // Сохранение данных в MRAM
int iMUVtoOZU(MuvMemoryData*);  // Загрузка данных из MRAM

#endif	// MUV_BCVSM_H
