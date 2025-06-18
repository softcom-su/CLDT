#ifndef _CTYPE_H_
#define _CTYPE_H_

#include <sys/types.h>

enum
{
    CT_NONE   = 0x0000,
    CT_UPPER  = 0x0001,
    CT_LOWER  = 0x0002,
    CT_ALPHA  = 0x0004,
    CT_DIGIT  = 0x0008,
    CT_SPACE  = 0x0010,
    CT_CNTRL  = 0x0020,
    CT_PUNCT  = 0x0040,
    CT_XDIGIT = 0x0080,
    CT_BLANK  = 0x0100,
    CT_GRAPH  = 0x0200,
    CT_ALNUM  = 0x0400,
    CT_PRINT  = 0x0800
};

int isalnum(int wc);
int isalpha(int wc);
int isascii(int wc);
int isblank(int wc);
int iscntrl(int wc);
int isdigit(int wc);
int isgraph(int wc);
int islower(int wc);
int isprint(int wc);
int ispunct(int wc);
int isspace(int wc);
int isupper(int wc);
int isxdigit(int wc);

int toascii(int);
int tolower(int wc);
int toupper(int wc);

#endif /* !_CTYPE_H_ */
