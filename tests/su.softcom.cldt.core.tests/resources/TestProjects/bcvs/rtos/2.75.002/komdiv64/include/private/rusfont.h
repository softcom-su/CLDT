#ifndef _RUSFONT_H
#define _RUSFONT_H

extern const unsigned char rusTransTbl_alt2koi[128];
extern const unsigned char rusTransTbl_alt2win[128];

extern unsigned char reverseCurTbl[128]; 
extern const unsigned char *pRusKeyTrans;

#define rusTransTbl_alt2alt  (const unsigned char *)0
/*трансляция русского текста в заданную кодировку*/
void TranslateText
(
   char * const *ppStrTbl,
   int nStr
);
/*инициализация перекодирования русского текста*/
void InitRusTranslateText( int codepage );

#endif
