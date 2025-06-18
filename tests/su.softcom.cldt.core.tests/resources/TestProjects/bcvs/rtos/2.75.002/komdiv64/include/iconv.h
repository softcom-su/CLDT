#ifndef ICONV_H_
#define ICONV_H_

#include <sys/types.h>

typedef int iconv_t;

void iconv_init();
iconv_t iconv_open(const char *encTo, const char *encFrom);
int     iconv_close(iconv_t cd);
size_t  iconv(iconv_t cd, char **inBuf, size_t *inLeft, char **outBuf, size_t *outLeft);

#endif /* ICONV_H_ */
