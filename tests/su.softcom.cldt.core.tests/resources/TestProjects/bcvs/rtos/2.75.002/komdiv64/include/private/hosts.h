#ifndef HOSTS_H_
#define HOSTS_H_
struct host_find_struct {
   const char *hfs_Name;
   unsigned long hfs_Addr;
   struct hostent *hfs_Hostent;
   int            hfs_Size;
};
int hostFind(struct host_find_struct *hfs);
#define LEN_AF_INET  4

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

#endif /*HOSTS_H_*/
