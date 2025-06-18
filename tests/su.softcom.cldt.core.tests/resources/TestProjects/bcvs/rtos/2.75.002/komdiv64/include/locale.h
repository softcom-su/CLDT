#ifndef _LOCALE_H_
#define _LOCALE_H_

#define LANG_ID_STRING_RU  "ru_SU"
#define LANG_ID_STRING_EN  "en_US"   

#define RU_ISO8859  "ru_SU.ISO_8859-5"
#define RU_KOI8R    "ru_SU.KOI8-R"
#define RU_CP866    "ru_SU.CP866"
#define RU_CP1251   "ru_SU.CP1251"

#include <limits.h>
#include <sys/types.h>

#ifndef NULL
#define	NULL	0
#endif

/* региональные настроки категорий LC_NUMERIC и LC_MONETARY */
struct lconv
{
    /* LC_NUMERIC */
    
    char *decimal_point; /* разделитель целой и дробной части */
    char *thousands_sep; /* разделитель групп разрядов */
      /* Каждый байт хранит количество цифр в соответствующей группе разрядов,
       * младшим адресам соответствуют группы, лежащие ближе к дробной части.
       * Байт со значением CHAR_MAX означает, что дальнейшая группировка не
       * производится; байт с нулевым значением означает, при дальнейшей
       * группировке для всех остальных групп используется значение количества
       * цифр из предыдущего байта.*/
    char *grouping;      
    
    /* LC_MONETARY */
    
      /* символ (строка) обозначения национальной валюты;
       * может содержать символы в региональной кодировке */
    char *currency_symbol;
      /* число цифр в дробной части суммы в случае записи с местным символом 
       * валюты */
    char  frac_digits;
      /* Первые 3 символа - обозначение международной валюты (по ISO 4217),
       * четвертый символ - разделитель. Пятый символ нулевой. */
    char *int_curr_symbol;
      /* число цифр в дробной части суммы в случае записи с международным 
       * символом валюты */
    char  int_frac_digits;
    
      /* 1, если символ валюты предшествует записи суммы, иначе 0           */
    char  n_cs_precedes;     /* местная валюта, отрицательная сумма         */
    char  p_cs_precedes;     /* местная валюта, неотрицательная сумма       */
    char  int_n_cs_precedes; /* международная валюта, отрицательная сумма   */
    char  int_p_cs_precedes; /* международная валюта, неотрицательная сумма */
    
      /* разделение записи суммы:
       *   0 - пробелы не используются;
       *   1 - если символ валюты и строка знака смежны, они отделяются 
       *       пробелом от суммы, в противном случае отделяется пробелом 
       *       символ валюты;
       *   2 - если символ валюты и строка знака смежны, они отделяются 
       *       пробелом от суммы, в противном случае отделяется пробелом 
       *       строка знака. */
    char  n_sep_by_space;     /* местная валюта, отрицательная сумма         */
    char  p_sep_by_space;     /* местная валюта, неотрицательная сумма       */
    char  int_n_sep_by_space; /* международная валюта, отрицательная сумма   */
    char  int_p_sep_by_space; /* международная валюта, неотрицательная сумма */

      /* определеяет взаимное раположение строки знака, символа валюты 
       * и значения суммы:
       *   0 - символ валюты и сумма заключаются в скобки;
       *   1 - строка знака предшествует сумме и символу валюты;
       *   2 - строка знака следует за суммой и символом валюты;
       *   3 - строка знака предшествует символу валюты;
       *   4 - строка знака следует за символом валюты. */
    char  n_sign_posn;     /* местная валюта, отрицательная сумма           */
    char  p_sign_posn;     /* местная валюта, неотрицательная сумма         */
    char  int_n_sign_posn; /* международная валюта, отрицательная сумма     */
    char  int_p_sign_posn; /* международная валюта, неотрицательная сумма   */
    
    char *mon_decimal_point; /* разделитель целой и дробной части           */
    char *mon_grouping;      /* разделение на группы разрядов, см. grouping */
    char *mon_thousands_sep; /* разделитель групп разрядов                  */
    char *negative_sign;     /* символ (строка) знака отрицательной суммы   */
    char *positive_sign;     /* символ (строка) знака положительной суммы   */
};

struct numLconv
{
    char *decimal_point;
    char *thousands_sep;
    char *grouping; 
};

struct monLconv
{
    char *currency_symbol;
    char  frac_digits;
    char *int_curr_symbol;
    char  int_frac_digits;
    char  n_cs_precedes;
    char  p_cs_precedes;
    char  int_n_cs_precedes;
    char  int_p_cs_precedes;
    char  n_sep_by_space;
    char  p_sep_by_space;
    char  int_n_sep_by_space;
    char  int_p_sep_by_space;
    char  n_sign_posn;
    char  p_sign_posn;
    char  int_n_sign_posn;
    char  int_p_sign_posn;
    char *mon_decimal_point;
    char *mon_grouping;
    char *mon_thousands_sep;
    char *negative_sign;
    char *positive_sign;
};

/* категории региональных настроек */
enum {
  LC_ALL = 0,
  LC_COLLATE,
  LC_CTYPE,
  LC_MONETARY,
  LC_NUMERIC,
  LC_TIME,
  LC_MESSAGES,
  _LC_LAST		/* не категория, просто обозначает конец */
};

typedef struct locLocaleNameParts
{
    char *language;
    char *country;
    char *encoding;
    char *extension;
} LOC_NAME_PARTS;

#define LOC_IS_POSIX(n) (!strncmp(n,"C",LOCALE_NAME_MAX) || !strncmp(n,"POSIX",LOCALE_NAME_MAX))
#define LOC_IS_EXT_POSIX(n) (((strstr(n,"C") == n) || (strstr(n,"POSIX"))) && (strchr(n,'@') != 0))

#define locCategoryExists(cat) ((cat) >= LC_ALL && (cat) < _LC_LAST)


void          localeInit();
int locParseLocaleName(int category, LOC_NAME_PARTS *locParts);

struct lconv* localeconv (void);
char*         setlocale(int category, const char *locale);


#endif /* _LOCALE_H_ */
