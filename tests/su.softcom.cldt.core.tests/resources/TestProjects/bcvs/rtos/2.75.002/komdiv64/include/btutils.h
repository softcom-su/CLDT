#ifndef BTUTILS_H
#define BTUTILS_H
int   BTfdisk(char *devname,int sizes[], int n);
void  BTcp(const char *from,const char *to);
int   BTnewfs(char *devname,int fat,int res);
#endif
