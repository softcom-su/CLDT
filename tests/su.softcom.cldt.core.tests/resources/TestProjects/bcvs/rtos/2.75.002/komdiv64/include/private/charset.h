#ifndef _PRIVATE_CHARSET_H
#define _PRIVATE_CHARSET_H

#include <wchar.h>
#include <sys/types.h>

#define WCHAR_NOTHING 0xFFFF
#define WEOF 0xFFFF

#define CHS_CHAR_HASH_SIZE  256

#define CHS_ELEMENT_WCSLEN(element) (((element)->size - (sizeof(struct chsElement)))/sizeof(wchar_t))

#define CHSERR_OK           0
#define CHSERR_NOMEM        1
#define CHSERR_INVCHAR      2
#define CHSERR_NOELEM       3
#define CHSERR_NOCHARSET    4
#define CHSERR_BADLOCALE    5
#define CHSERR_LOADERR      6

#define CHS_CHWEIGHT_IGNORE 0x0202
#define CHS_CHWEIGHT_EOS    0x0101

#define CHS_INITIALIZED(cs) (cs != NULL)

#ifndef NULL
#define NULL 0
#endif

typedef struct chsCharacter
{
    wchar_t  code;            /* кодовая позиция символа */
    uint_t   firstElementOff; /* смещение в байтах первого строкового элемента, начинающегося с
                                 данного символа, от начала блока строковых элементов */
    wctype_t ctype;           /* тип символа */
    wchar_t  lower;           /* кодовая позиция символа в нижнем регистре */
    wchar_t  upper;           /* кодовая позиция символа в верхнем регистре */
} CHS_CHAR_t;

typedef struct chsElement
{
    ushort  size;      /* размер описателя                             */
    ushort  weight[2]; /* первичный и вторичный вес                    */
    wchar_t chars[];   /* массив кодовых позиций, составляющих элемент */
} CHS_ELEM_t;

typedef struct chsCharset
{
    wchar_t maxWcharCode;                /* максимальное значение кодовой позиции */
    ushort  charsCnt;                    /* количество символов в региональном наборе символов */
    ushort  elementsCnt;                 /* количество элементов, заданных в LC_COLLATE */
    unchar  collOrder[2];                /* правила трактовки весов элементов */
    ushort  charHash[CHS_CHAR_HASH_SIZE];/* хэш-таблица смещений групп символов */
    CHS_CHAR_t *charTable;               /* массив описателей символов */
    CHS_ELEM_t *elemTable;               /* массив описателей строковых элементов */
} CHS_CSET_t;


int locLoadCharset(const char *locale);
int locSizeCharset(caddr_t addr, size_t *size);
int locCopyCharset(caddr_t dst, caddr_t src, size_t size);

int chsSetCharset(CHS_CSET_t *chs);
int chsGetCharsetDesc(CHS_CSET_t **charset);
int chsGetCharDesc(wchar_t wc, CHS_CHAR_t **charDesc);
int chsParseElement(const wchar_t *wcs, CHS_ELEM_t **elemDesc);
int chsMbParseElement(const char *s, CHS_ELEM_t **elemDesc, size_t *bytes);
int chsIsTypeof(wchar_t wc, wctype_t mask);

#endif /* _PRIVATE_CHARSET_H */
