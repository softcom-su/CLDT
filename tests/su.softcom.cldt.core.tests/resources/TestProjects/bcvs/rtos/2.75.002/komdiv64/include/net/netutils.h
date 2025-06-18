#ifndef _NET_NETUTILS_H
#define _NET_NETUTILS_H
#include <netinet/in.h>
void netsock();
void nettcp();
void netif();
void nethelpInit();
int  netIPSet(const char *ifname,const char *ip,const char *netmask,const char *ptp);
int  netIPAdd(const char *ifname,const char *ip,const char *netmask,const char *ptp);
int __netIPSet(const char *ifname, struct in_addr __ip, struct in_addr __netmask, struct in_addr __ptp,int delete);
int  netDhclient(const char *ifname, int verbose);
const u_char *netHWGet(const char *ifname);
int  netDefgatewaySet(const char *gw);
int  netRouteAdd(const char *cdst,const char *gw,const char *nm);
int  netStaticRouteAdd(const char *cdst,const char *gw);
int  slopen(char *devname,int mtu);
int  pppopen(char *devname,int mtu);
#endif
