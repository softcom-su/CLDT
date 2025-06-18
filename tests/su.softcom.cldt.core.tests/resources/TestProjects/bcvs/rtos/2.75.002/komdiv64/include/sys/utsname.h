#ifndef _SYS_UTSNAME_H
#include <version.h>
#include <limits.h>
#define _UTSNAME_LENGTH 65
struct utsname 
{
    char  sysname[_UTSNAME_LENGTH];  /* Name of this implementation of the operating system. */
    char  nodename[HOST_NAME_MAX];   /* Name of this node within an implementation-defined communications network. */
    char  release[_UTSNAME_LENGTH];  /* Current release level of this implementation. */
    char  version[_UTSNAME_LENGTH];  /* Current version level of this release. */
    char  machine[_UTSNAME_LENGTH];  /* Name of the hardware type on which the system is running. */
};

int uname(struct utsname *);


#endif /* !_SYS_UTSNAME_H */
