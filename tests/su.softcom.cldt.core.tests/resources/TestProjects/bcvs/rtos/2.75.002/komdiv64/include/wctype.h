#ifndef _WCTYPE_H_
#define _WCTYPE_H_

#include <wchar.h>

typedef enum {
    WCT_TOUPPER = 1,
    WCT_TOLOWER
} wctrans_t;

int iswalnum(wint_t);
int iswalpha(wint_t);
int iswcntrl(wint_t);
int iswblank(wint_t);
//int iswctype(wint_t, wctype_t);
int iswdigit(wint_t);
int iswgraph(wint_t);
int iswlower(wint_t);
int iswprint(wint_t);
int iswpunct(wint_t);
int iswspace(wint_t);
int iswupper(wint_t);
int iswxdigit(wint_t);

wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);

wctrans_t wctrans(const char *charclass);
wint_t    towctrans(wint_t wc, wctrans_t desc);

#endif /*_WCTYPE_H_*/
