//-------------------------------------------------------------------------------------	
// 		usrBootLine.c
//-------------------------------------------------------------------------------------	

#if OS_VERSION >= 210
#include <sys/mbuf.h>
#include <sys/ioctl.h>
  #if OS_VERSION > 300
    #include <sys/socket.h>
  #endif
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

//-------------------------------------------------------------------------------------	
#include	"config/usrBootLine.h"
//-------------------------------------------------------------------------------------	
static int bootParamsArg(char *);
static int bootDeviceArg();
static boot_params params;
static char notDev[] = "not";
//-------------------------------------------------------------------------------------	
//		bootParamsCopy
//-------------------------------------------------------------------------------------	
int bootParamsCopy(boot_params *bp)
{
memcpy(bp,&params,sizeof(boot_params));

//    strcpy(params.netDev,"sl");
//    params.net_arg0 = 1;
//    params.net_arg1 = 115200;

return 0;
}
//-------------------------------------------------------------------------------------	
//		bootStringToStruct
//-------------------------------------------------------------------------------------	
int bootStringToStruct(char* pBootLine,boot_params* bp)
{
char *p1;
char *p2;
//------------------------------------------------------------------------------------	
bzero((char *)&params,sizeof(boot_params));
//------------------------------------------------------------------------------------	
if((p2 = strstr(pBootLine,"(")) == NULL) 		goto err_bsts;
else
  {
   if((p2 - pBootLine) >= BOOT_DEV_LEN)			goto err_bsts;
   strncpy(params.bootDev,pBootLine,(int)(p2 - pBootLine)); 
  }

//-------------------------------------------------------------------------------------	  		
if((p1 = strstr(pBootLine,")")) == NULL) 		goto err_bsts;
if((p2 = strstr(pBootLine,":")) == NULL) 		goto err_bsts;

if((p2 - p1) >= BOOT_HOST_LEN)				goto err_bsts; 
p1 ++;
strncpy(params.hostName,p1,(int)(p2 - p1)); 
//-------------------------------------------------------------------------------------	  		  
p1 = p2;
if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;
p1 ++;
if((p2 - p1) > BOOT_FILE_LEN)				goto err_bsts;  
strncpy(params.bootFile,p1,(p2 - p1));
//-------------------------------------------------------------------------------------	  		  
if((p1 = strstr(pBootLine," e=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 2;
   if((p2 - p1) > BOOT_ADDR_LEN)			goto err_bsts;
   strncpy(params.ead,p1,(int)(p2 - p1));
  } 
else	params.ead[0] = (char)0;
//-------------------------------------------------------------------------------------	  		  
if((p1 = strstr(pBootLine," b=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 2;
   if((p2 - p1) > BOOT_ADDR_LEN)			goto err_bsts;
   strncpy(params.bad,p1,(int)(p2 - p1));
  }
else	params.bad[0] = (char)0;  
//-------------------------------------------------------------------------------------	  		  
if((p1 = strstr(pBootLine," h=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 2;
   if((p2 - p1) > BOOT_ADDR_LEN)			goto err_bsts;
   strncpy(params.had,p1,(int)(p2 - p1));
  }
else	params.had[0] = (char)0;  
//-------------------------------------------------------------------------------------	  		  
if((p1 = strstr(pBootLine," g=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 2;
   if((p2 - p1) > BOOT_ADDR_LEN)			goto err_bsts;
   strncpy(params.gad,p1,(int)(p2 - p1));
  }
else	params.gad[0] = (char)0;   
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," u=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 2;
   if((p2 - p1) > BOOT_USR_LEN)				goto err_bsts;
   strncpy(params.usr,p1,(int)(p2 - p1));
  }
else	strcpy(params.usr,"vxuser");   
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," pw=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)			goto err_bsts;  
   p1 += 3;
   if((p2 - p1) > BOOT_PASSWORD_LEN)			goto err_bsts;
   strncpy(params.passwd,p1,(int)(p2 - p1));
  }
else	strcpy(params.passwd,"vxpasswd"); 
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," f=")) != NULL)
  {
   char m1[15];
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)	p2 = pBootLine + 1 + strlen(pBootLine);
   p1 += 2;
   if((p2 - p1) > 10)					goto err_bsts;
   bzero(m1,sizeof(m1));
   strncpy(m1,p1,(p2 - p1));
   sscanf(m1,"%x",&(params.flags));
  }
else	params.flags = 0x4; 
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," tn=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)	p2 = pBootLine + 1 + strlen(pBootLine);
   p1 += 3;
   if((p2 - p1) > BOOT_HOST_LEN)			goto err_bsts;
   strncpy(params.targetName,p1,(int)(p2 - p1));
  }
else	strcpy(params.targetName,"t"); 
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," s=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)	p2 = pBootLine + 1 + strlen(pBootLine);
   p1 += 2;
   if((p2 - p1) > BOOT_OTHER_LEN)			goto err_bsts;
   strncpy(params.startupScript,p1,(int)(p2 - p1));
  }
else	params.startupScript[0] = (char)0; 
//------------------------------------------------------------------------------------- 
if((p1 = strstr(pBootLine," o=")) != NULL)
  {
   p1 ++;
   if((p2 = strstr(p1," ")) == NULL)	p2 = pBootLine + 1 + strlen(pBootLine);
   p1 += 2;
   if((p2 - p1) > BOOT_OTHER_LEN)			goto err_bsts;
   strncpy(params.other,p1,(int)(p2 - p1));
  }
else	params.other[0] = (char)0; 
//------------------------------------------------------------------------------------- 
if((p1 = strstr(params.ead,":")) != NULL)
  {
   strcpy(params.ead_mask,p1 + 1);
   *p1 = (char)0;
  }
if((p1 = strstr(params.gad,":")) != NULL)
  {
   strcpy(params.gad_mask,p1 + 1);
   *p1 = (char)0;   
  }
if((p1 = strstr(params.had,":")) != NULL)
  {
   strcpy(params.had_mask,p1 + 1);
   *p1 = (char)0;     
  }
if((p1 = strstr(params.bad,":")) != NULL)
  {
   strcpy(params.bad_mask,p1 + 1);
   *p1 = (char)0;       
  }
//-------------------------------------------------------------------------------------    
if(strncmp(params.bootDev,"go=",3) == NULL)
  {
   if(((p1 = strstr(params.bootDev,",")) != NULL) ||  /* ??? !!! */
      ((p1 = strstr(params.bootDev," ")) != NULL)
     )
     {
      p1 ++;
//      strncpy(params.bootDev,p1,(int)(p1 - params.bootDev));
      strcpy(params.bootDev,p1);
     }
  }     
//-------------------------------------------------------------------------------------    
if(bootParamsArg(params.bootDev) != NULL)
  {
   params.boot_arg0 = params.arg0;
   params.boot_arg1 = params.arg1;   
   params.boot_arg2 = params.arg2;   
  }

if(bootParamsArg(params.other) != NULL)
  {
   params.other_arg0 = params.arg0;
   params.other_arg1 = params.arg1;   
   params.other_arg2 = params.arg2;   
  }
//-------------------------------------------------------------------------------------    
strcpy(params.nfsMount,"not");      
if((params.flags & 0x10) != NULL)	// NFS
  { 
   int i;
   char *p1;     
   char dm[80];
   int uid,gid;
   char m_other[80];   

   bzero(dm,sizeof(dm));

   if(strncmp(params.bootFile,"/",1) == NULL)  
     {
      for(i = strlen(params.bootFile);i >= 0;i --)
        {
         if(*((params.bootFile) + i) == '/')
           {
            p1 = (params.bootFile) + i;
            *p1 = 0x00;
            strcpy(dm,params.bootFile);
            *p1 = '/';	
            break;
           }
         else	dm[0] = '/';
        }
     } 
   else
     {        
      sprintf(dm,"/home/%s",params.usr);
     }
   strcpy(params.nfsMount,dm);      
//-------------------------------------------------------------------------------------         
   if((params.other[0] == EOS) || (params.other[0] != 'U') ||
     (strstr(params.other,",") == NULL) ||
     (strstr(params.other,"G") == NULL))	    
     {
      uid = 500; 	gid = 500;
     }
   else
     {
      bzero(m_other,sizeof(m_other));
      strcpy(m_other,params.other);
      p1 = strstr(m_other,",");
      *p1 = (char)0;
      sscanf(&(m_other[1]),"%d",&uid);
      *p1 = ',';
      p1 = strstr(m_other,"G");
      p1 ++;
      sscanf(p1,"%d",&gid);
     }
   params.uid = uid;
   params.gid = gid;    
  }
//-------------------------------------------------------------------------------------         
bootDeviceArg();
//-------------------------------------------------------------------------------------    
if(bp != NULL)	bootParamsCopy(bp);
//-------------------------------------------------------------------------------------    
return 0;
   
err_bsts:
kprint("bootStringToStruct: error in boot string !\n\r");
bzero((char *)&params,sizeof(boot_params));
strcpy(params.bootDev,"not");
strcpy(params.hostName,"h");    
strcpy(params.bootFile,"oc2000");
strcpy(params.usr,"user");
strcpy(params.passwd,"passwd");
params.flags 	= 0x4;

return -1;
}
//------------------------------------------------------------------------------------- 
//		bootStringDefault()
//------------------------------------------------------------------------------------- 
static char bpd[] = "not(0,0)h:oc2000 e= h= u=osuser pw=vxpasswd f=0x4 tn=t";
void bootStringDefault()
{
strcpy((void *)BOOT_LINE_ADRS,bpd);
return;
}
//------------------------------------------------------------------------------------- 
//		bootNetMaskExtract()
//------------------------------------------------------------------------------------- 
int bootNetmaskExtract(char *pa,int *netmask)
{
*netmask = 0;
return 0; 
}
//------------------------------------------------------------------------------------- 
//		bootParamsArg
//------------------------------------------------------------------------------------- 
static int bootParamsArg(char * ps)
{
char *p1;
if((p1 = strstr(ps,"=")) != NULL)
  {
   char *p;
   *p1 = (char)NULL; 
   p1 ++;
   if((p = strstr(p1,",")) != NULL)
     {
      *p = (char)NULL;
      sscanf(p1,"%d",&(params.arg0));
      p1 = p + 1;
      if((p = strstr(p1,",")) != NULL)      
        {
         *p = (char)NULL;
         sscanf(p1,"%d",&(params.arg1));
         p1 = p + 1;
         if((p = strstr(p1,",")) != NULL)      	 
           {
            *p = (char)NULL;
            sscanf(p1,"%d",&(params.arg2));
	    return 3;
	   }
	 else
	   {
	    sscanf(p1,"%d",&(params.arg2));    
	    return 3;
	   }    
	}   
      else
        {
	 sscanf(p1,"%d",&(params.arg1));    	
	 return 2;
	} 
     }
   else
     {
      sscanf(p1,"%d",&(params.arg0));    
      return  1;
     } 
  }
return 0;  
}  
//-------------------------------------------------------------------------------------	
//		bootDeviceArg
//-------------------------------------------------------------------------------------	
static int bootDeviceArg()
{
params.netDev = notDev;
if(!strncmp(params.other,"de",2) ||
   !strncmp(params.other,"tu",2) ||
   !strncmp(params.other,"ed",2) ||
// !strncmp(params.other,"sm",2) ||  
   !strncmp(params.other,"sl",2))
  {
    params.netDev   = params.other;
    params.net_arg0 = params.other_arg0;	
    params.net_arg1 = params.other_arg1;	
    params.net_arg2 = params.other_arg2;	
  }
else
  {
   if(!strncmp(params.bootDev,"de",2) ||
      !strncmp(params.bootDev,"tu",2) ||
      !strncmp(params.bootDev,"ed",2) ||
//    !strncmp(params.bootDev,"sm",2) ||        
      !strncmp(params.bootDev,"sl",2))      
     {
      params.netDev   = params.bootDev;
      params.net_arg0 = params.boot_arg0;	
      params.net_arg1 = params.boot_arg1;	
      params.net_arg2 = params.boot_arg2;		 
     } 
  }
//-------------------------------------------------------------------------------------   
params.smDev = FALSE;
#if defined(MP_NETWORK) || defined(INCLUDE_RMSG_NET)
  #if MP_NETWORK_TYPE==MP_NETWORK_BUSNET || defined(INCLUDE_RMSG_NET)
    if(!strncmp(params.bootDev,"sm",2) || !strncmp(params.other,"sm",2) || params.bad[0] != EOS)
  #else
    if(!strncmp(params.bootDev,"bn",2) || !strncmp(params.other,"bn",2) || params.bad[0] != EOS)
  #endif
      {
        if(params.bad[0] == EOS)
          {
            sprintf(params.bad,"7.0.0.%d",boardProcNumGet() + 1);
          }
        params.smDev = TRUE;
      }
#endif
//------------------------------------------------------------------------------------- 
params.dev = notDev;
if(!strncmp(params.other,"ide",3) ||
   !strncmp(params.other,"scsi",4) ||
   !strncmp(params.other,"go",2))
  {
    params.dev   = params.other;
    params.arg0 = params.other_arg0;	
    params.arg1 = params.other_arg1;	
    params.arg2 = params.other_arg2;	
  }
else
  {
   if(!strncmp(params.bootDev,"ide",3) ||
      !strncmp(params.bootDev,"scsi",4) ||
      !strncmp(params.bootDev,"go",2))      
     {
      params.dev   = params.bootDev;
      params.arg0 = params.boot_arg0;	
      params.arg1 = params.boot_arg1;	
      params.arg2 = params.boot_arg2;		 
     } 
  }
return 0;
}
//------------------------------------------------------------------------------------- 
//		bpShow()
//------------------------------------------------------------------------------------- 
void bpShow()
{
  boot_params bp;
  bootParamsCopy(&bp);
  printf("\n");
  printf("--------------------------------------------\n");
  printf("bootDevice      -> <%s>\n",bp.bootDev);
  printf("hostName        -> <%s>\n",bp.hostName);
  printf("bootFile        -> <%s>\n",bp.bootFile);

if(bp.ead_mask[0] == (char)0)
  printf("ead             -> <%s>\n",bp.ead);
else
  printf("ead:mask        -> <%s:%s>\n",bp.ead,bp.ead_mask);

if(bp.bad_mask[0] == (char)0)
  printf("bad             -> <%s>\n",bp.bad);
else
  printf("bad:mask        -> <%s:%s>\n",bp.bad,bp.bad_mask);

if(bp.had_mask[0] == (char)0)
  printf("had             -> <%s>\n",bp.had);
else
  printf("had:mask        -> <%s:%s>\n",bp.had,bp.had_mask); 

if(bp.gad_mask[0] == (char)0)
  printf("gad             -> <%s>\n",bp.gad);
else
  printf("gad:mask        -> <%s:%s>\n",bp.gad,bp.gad_mask);  

  printf("user            -> <%s>\n",bp.usr);
  printf("passwd          -> <%s>\n",bp.passwd);
  printf("flags           -> 0x%x\n",bp.flags);
  printf("targetName      -> <%s>\n",bp.targetName);
  printf("startupScript   -> <%s>\n",bp.startupScript);
  printf("other           -> <%s>\n",bp.other);
  printf("--------------------------------------------\n");  
  printf("boot     (arg0) -> %d (0x%x)\n",bp.boot_arg0,bp.boot_arg0);
  printf("boot     (arg1) -> %d (0x%x)\n",bp.boot_arg1,bp.boot_arg1);
  printf("boot     (arg2) -> %d (0x%x)\n",bp.boot_arg2,bp.boot_arg2);  
  printf("--------------------------------------------\n");
  printf("other    (arg0) -> %d (0x%x)\n",bp.other_arg0,bp.other_arg0);
  printf("other    (arg1) -> %d (0x%x)\n",bp.other_arg1,bp.other_arg1);
  printf("other    (arg2) -> %d (0x%x)\n",bp.other_arg2,bp.other_arg2);  
  printf("--------------------------------------------\n");  
  printf("nfsMount        -> <%s>\n",bp.nfsMount);
  printf("uid             -> %d\n",bp.uid);
  printf("gid             -> %d\n",bp.gid);  
  printf("--------------------------------------------\n");
  printf("netDev          -> <%s>\n",bp.netDev);  
  printf("dev             -> <%s>\n",bp.dev);  
  printf("smDev           -> %1d\n",bp.smDev);  
  printf("--------------------------------------------\n");  
return;
}

//------------------------------------------------------------------------------------- 
//------------------------------------------------------------------------------------- 

