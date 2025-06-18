

#include "btpo.h"

int test0(void)
{
 unsigned ret=0;

 ret|=chk0_0();
 ret|=chk0_1();

 return ret;
}



int chk0_0(void)
{
 return 0;
}

int chk0_1(void)
{
 return 0;
}


