/* usrNetwork.c - network initialization */

/* history
 *19apr13,laz  default_gw when gw is empty fixed
  17jan07,laz  de1 inheritance fixed
*/
#if OS_VERSION >= 210
  #include <sys/mbuf.h>
  #include <sys/ioctl.h>
#else	/* OS_VERSION >= 210 */
  #include <net/bsd.h>
  #include <net/mbuf.h>
#endif	/* OS_VERSION >= 210 */
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <hosts.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <net/netutils.h>
#include <private/systime.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#if OS_VERSION >= 240
  #include <arch/board.h>
#endif
//-------------------------------------------------------------------------------------		
#include "config/usrBootLine.h"
//-------------------------------------------------------------------------------------		
            //extern int netIPSet(char *,char *,char *,char *);
#if OS_VERSION >= 240 && OS_RELEASE > 44 || OS_VERSION >= 249
  extern int slopen(char *devname, int mtu);
#else
  extern int slopen(char *devname);
#endif
//-------------------------------------------------------------------------------------	
#if OS_VERSION >= 240
static struct netCONFIG netConfig;
static int netInitDone = FALSE;
#endif
//-------------------------------------------------------------------------------------	
// 		check for valid inet address in boot field
//-------------------------------------------------------------------------------------	
STATUS checkInetAddrField(char* s, int subnetMaskOK)
{
    char buf[30]; int netmask;

    if(!s[0])
	return 0;

    strncpy(buf,s,sizeof(buf));

    if(subnetMaskOK)
    {
	if(bootNetmaskExtract(buf,&netmask) == -1)
	{
	    printf("Error: invalid netmask in boot field \"%s\".\n",buf);
	    return -1;
	}
    }

    if(inet_addr(buf) == -1)
    {
	printf("Error: invalid inet address in boot field \"%s\".\n",buf);
	return -1;
    }

    return 0;
}
//-----------------------------------------------------------------------------	
//		usrNetInit
//-----------------------------------------------------------------------------	
extern int setuid(uid_t uid);
extern int setgid(gid_t gid);

STATUS usrNetInit(char *bootString)
{
    boot_params params;
    char* bootIF;
    char* bootIP;
     int i=0;
//-----------------------------------------------------------------------------
    if(!bootString)	bootString = (char *)BOOT_LINE_ADRS;	// ???

    bootParamsCopy(&params);

#if defined(MP_NETWORK) || defined(INCLUDE_RMSG_NET)
  if(params.smDev == TRUE)
    {
  #if MP_NETWORK_TYPE==MP_NETWORK_BUSNET || defined(INCLUDE_RMSG_NET)
       strcpy (netConfig.ifc[i].ifname, "sm0");
   #else
       strcpy (netConfig.ifc[i].ifname, "bn0");
   #endif
       strcpy (netConfig.ifc[i].ip, params.bad);
       strcpy (netConfig.ifc[i].netmask, "255.255.255.0");
       netConfig.ifc[i].ptp[0] =0;
       i++;
    }
#endif
//============================================================================	        
    bootIF = params.netDev;
    bootIP = params.ead;
//-----------------------------------------------------------------------------
if((bootIF[0] != EOS) && strncmp(bootIF,"not",3) && (bootIP[0] != EOS))
  {
   if (strcmp (bootIF, "de1") && strcmp (bootIF, "de9")) sprintf(bootIF,"%s0",bootIF);
   if((strncmp(bootIF,"sl",2) == NULL) && (params.had[0] != EOS))
     {  
      char sl_name[15];
      bzero(sl_name,sizeof(sl_name));
      sprintf(sl_name,"/dev/aux%1d",params.net_arg0);
#if OS_VERSION >= 243 && OS_RELEASE > 44 || OS_VERSION >= 249
      slopen(sl_name,296);    /* fixed MTU ??? */
#else
      slopen(sl_name);
#endif
      strcpy (netConfig.ifc[i].ptp, params.had);
     }
   else
     {          
      netConfig.ifc[i].ptp[0] =0;
     }
   strcpy (netConfig.ifc[i].netmask, params.ead_mask);
   strcpy (netConfig.ifc[i].ifname, bootIF);
   strcpy (netConfig.ifc[i].ip, bootIP);
   i++;

  }
//-----------------------------------------------------------------------------	     
    strcpy(netConfig.user,params.usr);
    strcpy(netConfig.password,params.passwd);
    strcpy(netConfig.host_IP,params.had);
    strcpy(netConfig.host_name,params.hostName);
    if (strlen(params.gad) == 0)
       strcpy(netConfig.default_gw, params.had);
    else 
       strcpy(netConfig.default_gw,params.gad);

    if(params.bootFile[0] == '/')
      {
       char *ps;
       sprintf(netConfig.nfs_export,"%s",params.bootFile);                           
       ps = strrchr(netConfig.nfs_export + 1,(int)'/');
       if(ps == 0)
         {
	  sprintf(netConfig.nfs_export,"/home/%s",params.usr);		// ?????
	 } 
       else
         {
	  *ps = 0;
	 } 
      }
    else
      {
       sprintf(netConfig.nfs_export,"/home/%s",params.usr);                 
      }

    netConfig.ftp_nfs = (params.flags & 0x10) != 0;
#if 0
printf ("ifc[0].nm=%s ip=%s mask=%s booter: %s %s\n\r",
        netConfig.ifc[0].ifname, netConfig.ifc[0].ip,netConfig.ifc[0].netmask, 
        params.bad, params.bad_mask
       );
printf ("ifc[1].nm=%s ip=%s mask=%s booter: %s %s\n\r",
        netConfig.ifc[1].ifname, netConfig.ifc[1].ip,netConfig.ifc[1].netmask,
        params.ead, params.ead_mask
       );
printf ("gw %s booter: %s\n\r", netConfig.default_gw, params.gad);
#endif
    netInitDone = TRUE; return 0;
//-----------------------------------------------------------------------------	   
return 0;
}
//-----------------------------------------------------------------------------	   
//		boardBootParamsGet
//-----------------------------------------------------------------------------	   
#if OS_VERSION >= 240
int boardBootParamsGet(struct netCONFIG* nc)
{
    if(!netInitDone)
	return -1;

    *nc = netConfig; return 0;
}
#endif
//-----------------------------------------------------------------------------	
//-----------------------------------------------------------------------------	
