#ifndef _LOCALES_H_
#define _LOCALES_H_

#include <sys/systm.h>
#include <limits.h>
#include <private/charset.h>
#include <locale.h>
#include <sys/time.h>

typedef struct locGetRequest
{
    const char *locale;
    int     cat;
    size_t  size;
    int   (*fn_locCopy)(caddr_t dst, caddr_t src, size_t size);
    caddr_t uaddr;
    caddr_t kaddr;
} LOC_GET_REQUEST;

#define LOC_GET_REQ_INIT(loc,cat,fnCopy) {loc,cat,0,fnCopy,0,0}

int locGetLocale(LOC_GET_REQUEST *req);


#ifdef KERNEL

typedef struct locDefinition
{
    char locale[LOCALE_NAME_MAX + 1];
    struct 
    {
        caddr_t addr;
        size_t size;
    } cats[_LC_LAST];
} LOC_DEFINITION;

#define DEFLOCALE(loc,var) \
    static const LOC_DEFINITION locd##var = {\
        .locale = loc,\
        .cats = {\
            [LC_ALL]      = {NULL,0},\
            [LC_COLLATE]  = {(caddr_t)&locale.etableBin,sizeof(locale.etableBin) + sizeof(locale.ctable) + sizeof(locale.charset)},\
            [LC_CTYPE]    = {(caddr_t)&locale.etableBin,sizeof(locale.etableBin) + sizeof(locale.ctable) + sizeof(locale.charset)},\
            [LC_MONETARY] = {(caddr_t)&locale.monStrings,sizeof(locale.monStrings)},\
            [LC_NUMERIC]  = {(caddr_t)&locale.numStrings,sizeof(locale.numStrings)},\
            [LC_TIME]     = {(caddr_t)&locale.timeStrings,sizeof(locale.timeStrings)},\
            [LC_MESSAGES] = {NULL,0}\
        }\
    };\
    DATA_SET(locales,locd##var);

#if __GNUC__ >= 3
#define DATA_SET(set, var) static const void *__##var  __attribute__ ((section ("."#set),unused,used)) = &var
#else
#define DATA_SET(set, var) static const void *__##var  __attribute__ ((section ("."#set),unused)) = &var
#endif


extern struct linker_set locales;

#define PLOCALES ((LOC_DEFINITION**)(locales.ls_items))

#endif /* KERNEL */

#endif /*_LOCALES_H_*/
