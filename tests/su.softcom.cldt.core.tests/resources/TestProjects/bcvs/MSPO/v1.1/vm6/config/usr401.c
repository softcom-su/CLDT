//#include "usr401a.h"
#include <version.h>
#if defined(INCLUDE_BTM401) || defined(INCLUDE_BT83401)
  extern void bt401Drv (void);
  extern int bt401Loglevel;
#endif  
#ifdef INCLUDE_BT401A
  extern void bt401DrvVME (void);
  extern int bt401LoglevelVME;
#endif  
/*-----------------------------------------*/
int usr401Init (void)
{
#if defined(INCLUDE_BTM401) || defined(INCLUDE_BT83401)
  #ifdef BT401LOGLEVEL
   bt401Loglevel = BT401LOGLEVEL ;
  #endif
    bt401Drv();
#endif
#ifdef INCLUDE_BT401A
  #ifdef BT401LOGLEVEL_VME
   bt401Loglevel = BT401LOGLEVEL_VME ;
  #endif
    bt401DrvVME();
#endif

#if 0  /* OS_VERSION > 300    */
  #ifdef  INCLUDE_BTM401 
    { int m;  
      if ((m = bt401Init(1,0,0,0,1)) == (-1))
        { if ((m = bt401Init(2,0,0,0,1)) == (-1))
            { printf("bt401Init: no BTM23-401 founded or initialization error\n");
              return -1;
            }
	}
      printf ("usr401: BTM23-401 initialized \n");
      if (bt401DevCreate ("/dev/mkio_0",m, 0, 33*10, 33*2) != 0)
      { printf ("bt401DevCreate: error %i\n\r", errno); return -1;
      }
      printf ("usr401: device  '/dev/mkio_0'  created\n");
  #endif
#endif
    
    return 0;
}
