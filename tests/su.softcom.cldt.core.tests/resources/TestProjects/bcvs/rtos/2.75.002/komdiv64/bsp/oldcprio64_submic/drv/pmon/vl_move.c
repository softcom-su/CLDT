#include "drv/pmon/vl.h"

/*----------------------------------------------------*/

void move_pmons_vars (void)
{ 
#ifndef FOR_SUBMICRON
   int i; unsigned char c, *pfrom=VL_BASE_ADDR, *pto=VL_SAVE_ADDR;
   
    for (i=0, c = *pfrom; i < PMONS_VARS_MAX_LEN && c != 0; i++) 
        { c = *pfrom++; *pto++ = c;
	}
#endif
}
