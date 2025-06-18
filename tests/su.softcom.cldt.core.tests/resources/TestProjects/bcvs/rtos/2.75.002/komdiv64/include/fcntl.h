/*
Copyright  1998
FCNTL.H
*/

#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/types.h>
#include <sys/fcntl.h>


#define FD_CLOEXEC      1       /* actually anything with low bit set goes */

/*
 *  generic c-lib function
 */

int creat(const char *path,mode_t mode);
int open(const char *path,int flags,.../*mode_t mode*/);
int fcntl(int filedes,int cmd,...);

#endif  /* _FCNTL_H */

