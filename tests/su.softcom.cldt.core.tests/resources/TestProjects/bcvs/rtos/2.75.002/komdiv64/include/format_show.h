/**
 * \file  format_show.h
 *
 * \brief Унифицированный вывод.
 *
 * Унификация вывода таблиц, времени, объёма памяти.
 *
 */

#include <sys/types.h>

#define MAX_TABLE_WIDTH 120
#define TABLE_SIZE(a) sizeof(a)/sizeof(tableCOLUMN)

#define BYTES_PER_KB 1024
#define BYTES_PER_MB 1048576

/** Описание столбца таблицы. 
 *
 *  Полностью таблица описывается массивом описаний столбцов.
*/
typedef struct {  
  char *tc_name;              /**< Имя столбца (которое будет отображено в 'шапке'. */
  unsigned char  tc_size;     /**< Ширина столбца.                                  */
  unsigned char  tc_type;     /**< Тип столбца (s-строка, d-целое, f-дробное, c-символ, x-16ричное, p-указатель). */
  unsigned char  tc_accuracy; /**< Точность (для типа f - количество знаков после запятой). */
} tableCOLUMN;    

/* Проверка корректности таблицы (ширина). */
extern int tableCheck(tableCOLUMN* table, int colNum);
/* Вывод 'шапки' таблицы. */
extern int tableShowHead( int (printf)(const char *fmt, ...), const tableCOLUMN* table, int size);
/* Вывод строки таблицы. */
extern int tableShowLine( int (printf)(const char *fmt, ...), const tableCOLUMN* table, int size, ...);

/* Формирование строки времени. */
extern void formatTimeStr( system_time_t time, char* buf, int size);

/* Форматирование количества памяти. */
extern void formatMemory(unsigned long long bytes, unsigned long long *amount, char *suffix); 
extern void formatMemoryStr(unsigned long long bytes, char* buf, int size);

