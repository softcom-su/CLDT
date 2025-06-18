/* usrRam.c - example of block device on ram initialization */

#include <system.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <btutils.h>
#include <ramDrv.h>

#define	N_RAM_BYTES	0x1000000	/* size of block device in bytes */
#define	RAM_BLK_SIZE	512		/* block size in bytes */
#define	RAM_DEV_NAME	"/dev/ram"	/* device name */
#define	RAM_MOUNT_POINT	"/dos"		/* mount point */

int msdosfsMount (char *, char *);
/******************************************************************************/

/* file sytem initialization on RAM drive, called from boardHwInit3() */

STATUS usrRamInit(int fmt)
{
    BLK_IO_DEV* io; int res;

    /* create block device */

    if(!(io = ramDevInstall(NULL,RAM_BLK_SIZE,8,N_RAM_BYTES/RAM_BLK_SIZE,
	0,RAM_DEV_NAME)))
    {
	printf("usrRamInit: ramDevInstall failed\n\r"); return -1;
    }

    
    if (BTnewfs(RAM_DEV_NAME,0,fmt) != 0)
      { printf ("Error BTnewfs \n\r");  return -1;
      }

#if OS_VERSION <= 218
    res = msdosfsMount(RAM_MOUNT_POINT,RAM_DEV_NAME);
#else
    { char s[80];
      sprintf (s,"-t msdos %s %s", RAM_DEV_NAME, RAM_MOUNT_POINT);
      printf ("_mount ""%s"" \n\r", s);
      res = _mount(s);
    }
#endif
    if (res !=0) printf ("Error mount vfat on /dev/ram (errno=%i)\n\r", errno);
    return res;
}

STATUS usrRamConfig()
{
    return usrRamInit(0);
}
