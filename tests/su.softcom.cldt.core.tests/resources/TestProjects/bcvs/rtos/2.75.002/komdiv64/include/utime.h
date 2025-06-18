#ifndef _UTIME_H
#define _UTIME_H

struct utimbuf
{
    time_t    actime;   /*  Access time.        */
    time_t    modtime;   /*  Modification time.  */
};

int utime(const char *, const struct utimbuf *);

#endif
