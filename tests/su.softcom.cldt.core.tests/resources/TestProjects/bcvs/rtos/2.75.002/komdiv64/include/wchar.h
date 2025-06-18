#ifndef _WCHAR_H_
#define _WCHAR_H_

#include <stddef.h> /* wchar_t */

#define WC_COLLORD_FORWARD  1
#define WC_COLLORD_BACKWARD 2
#define WC_COLLORD_POSITION 4

/* POSIX */
#define WCHAR_MAX   0xFFFF
#define WCHAR_MIN   0
#define WEOF        0xFFFF


#ifndef _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED  typedef unsigned short wctype_t
_WCTYPE_T_DEFINED;
#endif

#ifndef _WINT_T_DEFINED
#define _WINT_T_DEFINED    typedef unsigned short wint_t
_WINT_T_DEFINED;
#endif

typedef struct encConv
{
    int state;
} mbstate_t;

size_t   wcslen(const wchar_t *ws);
wchar_t *wcscat(wchar_t *dest, const wchar_t *src);
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc);
int      wcscmp(const wchar_t *ws1, const wchar_t *ws2);
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src);
size_t   wcscspn(const wchar_t *wcs, const wchar_t *reject);
int      wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wcspbrk(const wchar_t *wcs, const wchar_t *accept);
wchar_t *wcsrchr(const wchar_t *wcs, wchar_t wc);
size_t   wcsspn(const wchar_t *wcs, const wchar_t *accept);
wchar_t *wcsstr(const wchar_t *ws1, const wchar_t *ws2);
wchar_t *wcstok(wchar_t *wcs, const wchar_t *delim, wchar_t **ptr);

int      wcscoll(const wchar_t *wcs1, const wchar_t *wcs2);
size_t   wcsxfrm(wchar_t *dest, const wchar_t *src, size_t len);

wchar_t *wmemchr(const wchar_t *wcs, wchar_t c, size_t n);
int      wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n);

//int    wctomb(char *s, wchar_t wc);
size_t wcrtomb(char *s, wchar_t wc, mbstate_t *econv);
//size_t wcstombs(char *s, const wchar_t *wcs, size_t n);
size_t wcsrtombs(char *s, const wchar_t **pwcs, size_t n, mbstate_t *econv);

//int    mbtowc(wchar_t *pwc, const char *s, size_t n);
size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *econv);
//size_t mbstowcs(wchar_t *wcs, const char *s, size_t n);
size_t mbsrtowcs(wchar_t *wcs, const char **s, size_t n, mbstate_t *econv);

//int    mblen(const char *s, size_t n);
size_t mbrlen(const char *s, size_t n, mbstate_t *econv);

int    mbsinit(const mbstate_t *ps);
int    wctob(wint_t c);
wint_t btowc(int c);


#endif /*_WCHAR_H_*/
