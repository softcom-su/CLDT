#include <hosts.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/domain.h>
#include <private/oslib.h>
#include <net/netutils.h>
#include <private/systime.h>
#include <private/symtbl.h>

#include <arch/board.h>

#define LAN_LOOPADDR "127.0.0.1"
#include <net/netutils.h>
void loopattach();

int setuid(uid_t uid);
int setgid(gid_t gid);
int ptyCreate(int n);
int netconfig()
{
    int error,total_error=0;
    static int netconfig_done=0;
    if(netconfig_done) return 0;
    netconfig_done=1;
    error=0;
    setuid (UID) ;
    setgid (GID) ;
    loopattach();
    netIPSet("lo0","127.0.0.1",0,0);
#if (MP_NETWORK) && MP_NETWORK_TYPE==MP_NETWORK_VITA
    {
        int busnetDevInit(int logical_addr);
        int boardBnConfig(int);
        PRINTF("Vita config ...");
        boardBnConfig(BN_MASTER_ADDR);

        /*=========================================================================== */
        bn_config.bn_packet_size =BN_PACKET_SIZE;
        bn_config.bn_p_probe_startdelay=BN_P_PROBE_STARTDELAY; /* microseconds */
        bn_config.bn_p_probe_mininterval=BN_P_PROBE_MININTERVAL;
        bn_config.bn_p_probe_maxcount=BN_P_PROBE_MAXCOUNT;
        bn_config.bn_p_probe_backoffcount=BN_P_PROBE_BACKOFFCOUNT;
        bn_config.bn_p_probe_maxinterval=BN_P_PROBE_MAXINTERVAL;
        if((error=bn_init(&bn_config))==0)
        {
            error=busnetDevInit(bn_config.bn_logical_addr);
            if(error) PRINTF("Fail\n");
            else      PRINTF("OK\n");
        } else
        {
            PRINTF("Fail\n");
        }
        total_error +=error;
    }
#endif /*MP_NETWORK_VITA */
#if INCLUDE_HOST_TABLE_INIT
    error=hostTableInit(HOST_TABLE_NODES_COUNT, HOST_TABLE_ALIAS_COUNT,
        HOST_TABLE_ADDRESS_COUNT, HOST_TABLE_NAME_LENGTH, HOST_TABLE_ADDRESS_LENGTH) ;
    PRINTF("hostTableInit %s \n",error?"Fail":"OK");
    total_error +=error;

    error=hostAdd("localhost", LAN_LOOPADDR) ;
    PRINTF("hostAdd(%s,%s) %s \n","localhost", LAN_LOOPADDR,error?"Fail":"OK");
    total_error +=error;
#if INCLUDE_ADDHOSTS0
    error=hostAdd(ADDHOSTS_NAME0,ADDHOSTS_ADDRESS0);
    PRINTF("hostAdd(%s,%s) %s \n",ADDHOSTS_NAME0,ADDHOSTS_ADDRESS0,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_ADDHOSTS1
    error=hostAdd(ADDHOSTS_NAME1,ADDHOSTS_ADDRESS1);
    PRINTF("hostAdd(%s,%s) %s \n",ADDHOSTS_NAME1,ADDHOSTS_ADDRESS1,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_ADDHOSTS2
    error=hostAdd(ADDHOSTS_NAME2,ADDHOSTS_ADDRESS2);
    PRINTF("hostAdd(%s,%s) %s \n",ADDHOSTS_NAME2,ADDHOSTS_ADDRESS2,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_ADDHOSTS3
    error=hostAdd(ADDHOSTS_NAME3,ADDHOSTS_ADDRESS3);
    PRINTF("hostAdd(%s,%s) %s \n",ADDHOSTS_NAME3,ADDHOSTS_ADDRESS3,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_ADDHOSTS4
    error=hostAdd(ADDHOSTS_NAME4,ADDHOSTS_ADDRESS4);
    PRINTF("hostAdd(%s,%s) %s \n",ADDHOSTS_NAME4,ADDHOSTS_ADDRESS4,error?"Fail":"OK");
    total_error +=error;
#endif
#endif /* INCLUDE_HOST_TABLE_INIT */

#if INCLUDE_SLIP

#if INCLUDE_SLIP0
    error=slopen(SLIP_DEVNAME0,SLIP_MTU0);
    PRINTF("slopen(%s,%d) %s \n",SLIP_DEVNAME0,SLIP_MTU0,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_SLIP1
    error=slopen(SLIP_DEVNAME1,SLIP_MTU1);
    PRINTF("slopen(%s) %s \n",SLIP_DEVNAME1,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_SLIP2
    error=slopen(SLIP_DEVNAME2,SLIP_MTU2);
    PRINTF("slopen(%s) %s \n",SLIP_DEVNAME2,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_SLIP3
    error=slopen(SLIP_DEVNAME3,SLIP_MTU3);
    PRINTF("slopen(%s) %s \n",SLIP_DEVNAME3,error?"Fail":"OK");
    total_error +=error;
#endif
#endif

#if INCLUDE_PPP
#if INCLUDE_PPP0
    error=pppopen(PPP_DEVNAME0,0);
    PRINTF("pppopen(%s) %s \n",PPP_DEVNAME0,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_PPP1
    error=pppopen(PPP_DEVNAME1,0);
    PRINTF("pppopen(%s) %s \n",PPP_DEVNAME1,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_PPP2
    error=pppopen(PPP_DEVNAME2,0);
    PRINTF("pppopen(%s) %s \n",PPP_DEVNAME2,error?"Fail":"OK");
    total_error +=error;
#endif
#if INCLUDE_PPP3
    error=pppopen(PPP_DEVNAME3,0);
    PRINTF("pppopen(%s) %s \n",PPP_DEVNAME3,error?"Fail":"OK");
    total_error +=error;
#endif
#endif
#define NET_IP_SET(i,a,n,p) \
    { PRINTF("iface %s addr %s nm %s p-t-p %s ...",i,\
    (a && a[0])?a:"n/a",\
    (n && n[0])?n:"n/a",\
    (p && p[0])?p:"n/a");\
    fflush(stdout); \
    error=netIPSet(i,a,n,p);\
    total_error +=error;   \
    if(error) PRINTF("error=%d\n",error); else PRINTF("OK\n"); }

#if CONFIG_NETADDRESSES
#if INCLUDE_NETADDRESS0
    NET_IP_SET(_IFNAME0,_ADDRESS0,_NETMASK0,_PTP0);
#endif
#if INCLUDE_NETADDRESS1
    NET_IP_SET(_IFNAME1,_ADDRESS1,_NETMASK1,_PTP1);
#endif
#if INCLUDE_NETADDRESS2
    NET_IP_SET(_IFNAME2,_ADDRESS2,_NETMASK2,_PTP2);
#endif
#if INCLUDE_NETADDRESS3
    NET_IP_SET(_IFNAME3,_ADDRESS3,_NETMASK3,_PTP3);
#endif
#if INCLUDE_NETADDRESS4
    NET_IP_SET(_IFNAME4,_ADDRESS4,_NETMASK4,_PTP4);
#endif

#if INCLUDE_DEFAULT_GATEWAY
    error=netDefgatewaySet(DEFAULT_GATEWAY_ADDRESS);
    PRINTF("netDefgatewaySet(%s) %s \n",DEFAULT_GATEWAY_ADDRESS,error?"Fail":"OK");
    total_error +=error;
#endif
#endif /* CONFIG_NETADDRESSES */

#if INCLUDE_NETGW
#define NET_GW_ADD(a) \
    { PRINTF("netRouteAdd Dst=%s Gw=%s Nm=%s ... ", GATEWAY_DST##a, GATEWAY_GW##a, GATEWAY_MASK##a); \
    fflush(stdout); error=netRouteAdd(GATEWAY_DST##a, GATEWAY_GW##a, GATEWAY_MASK##a);     \
    total_error += error; \
    if(error) PRINTF("error=%d\n",error); else PRINTF("OK\n"); }
#if INCLUDE_NETGW0
    NET_GW_ADD(0);
#endif
#if INCLUDE_NETGW1
    NET_GW_ADD(1);
#endif
#if INCLUDE_NETGW2
    NET_GW_ADD(2);
#endif
#if INCLUDE_NETGW3
    NET_GW_ADD(3);
#endif
#if INCLUDE_NETGW4
    NET_GW_ADD(4);
#endif
#if INCLUDE_NETGW5
    NET_GW_ADD(5);
#endif
#endif
#if INCLUDE_SNETGW
#define NET_SGW_ADD(a) \
    { PRINTF("netStaticRouteAdd Dst=%s Gw=%s ... ", GATEWAY_SDST##a, GATEWAY_SGW##a); \
    fflush(stdout); error=netStaticRouteAdd(GATEWAY_SDST##a, GATEWAY_SGW##a);     \
    total_error += error; \
    if(error) PRINTF("error=%d\n",error); else PRINTF("OK\n"); }
#if INCLUDE_SNETGW0
    NET_SGW_ADD(0);
#endif
#if INCLUDE_SNETGW1
    NET_SGW_ADD(1);
#endif
#if INCLUDE_SNETGW2
    NET_SGW_ADD(2);
#endif
#if INCLUDE_SNETGW3
    NET_SGW_ADD(3);
#endif
#if INCLUDE_SNETGW4
    NET_SGW_ADD(4);
#endif
#if INCLUDE_SNETGW5
    NET_SGW_ADD(5);
#endif
#endif

#ifdef BOOTER_INHERITANCE
    {
        struct netCONFIG nc;
        error=boardBootParamsGet (&nc);
        PRINTF("boardBootParamsGet %s \n",error?"Fail":"OK");
        if(error==0)
        {
            int i;
            for(i=0;i<4;i++)
            {
                if(nc.ifc[i].ifname[0])
                    NET_IP_SET(nc.ifc[i].ifname,nc.ifc[i].ip,nc.ifc[i].netmask,nc.ifc[i].ptp);
            }
            if(nc.default_gw[0]);
            error=netDefgatewaySet(nc.default_gw);
            PRINTF("netDefgatewaySet(%s) %s \n",nc.default_gw,error?"Fail":"OK");
            if(nc.host_name[0] && nc.host_IP[0])
            {
                error=hostAdd(nc.host_name,nc.host_IP);
                PRINTF("hostAdd(%s,%s) %s \n",nc.host_name,nc.host_IP,error?"Fail":"OK");
            }
            if(nc.host_IP[0])
            {
                if(nc.ftp_nfs && nc.nfs_export[0] )
                {
                    char buf[100];
                    setuid((short)nc.uid);
                    setgid((short)nc.gid);
                    sprintf(buf,"-t nfs %s:%s /nfs",nc.host_IP,nc.nfs_export);
                    error=_mount(buf);
                    PRINTF("%s %s \n",buf,error?"Fail":"OK");
                }
                if(nc.ftp_nfs==0 && nc.user[0] )
                {
                    char buf[200];
                    sprintf(buf,"-t ftp %s:%s@%s /ftp",nc.user,nc.password,nc.host_IP); // FIXME mountpoint
                    error=_mount(buf);
                    PRINTF("mount %s %s \n",buf,error?"Fail":"OK");
                }
            }
        }
        total_error +=error;
    }
#endif


#if (INCLUDE_FTP_FS) && (MOUNT_FTP) && ( MOUNT_FTP0 || MOUNT_FTP1 || MOUNT_FTP2 )
    {
        char *s;
#define FTP_MOUNT(a)\
    s = "-t ftp -o timeout=20 "FTP_PARSTRING##a " "FTP_MOUNTPOINT##a;\
        error=_mount(s);                                      \
        PRINTF("mount %s %s \n",s,error?"Fail":"OK");         \
        total_error +=error;
#if MOUNT_FTP0
        FTP_MOUNT(0);
#endif
#if MOUNT_FTP1
        FTP_MOUNT(1);
#endif
#if MOUNT_FTP2
        FTP_MOUNT(2);
#endif
    }
#endif /* INCLUDE_FTP_FS */

#if   INCLUDE_PING
    {
        int pingInit();
        pingInit();
        PRINTF("pingInit OK\r\n");
    }
#endif
#if   INCLUDE_TELNET
    {
        int telnetInit();
        telnetInit();
        PRINTF("telnetInit OK\r\n");
    }
#endif
#if   INCLUDE_TELNETD
    {
        void *telnetd(void *arg);
        pthread_attr_t attr;
        struct  sched_param sp;

        if( ptyCreate(15) == 0 )
        {
#if TELNETD_PRIORITY
            sp.sched_priority = TELNETD_PRIORITY;
#else
            sp.sched_priority = 150;
#endif
            pthread_attr_init(&attr);
            pthread_attr_setschedparam( &attr,&sp);
            pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
            pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
            pthread_attr_setbreakstate( &attr, PTHREAD_BREAK_DISABLE );
            pthread_attr_setsystem( &attr, PTHREAD_CREATE_SYSTEM );
//            traceSET_ATTR_SYSTHREAD( &attr );
            pthread_attr_setfpustate(&attr,PTHREAD_FPU_DISABLE);
#if TELNETD_LOGON
            error=pthread_create(NULL,&attr,telnetd,TELNETD_LOGON_SCRIPT);
#else
            error=pthread_create(NULL,&attr,telnetd,NULL);
#endif
            PRINTF("telnetdInit %s\n",error?"Fail":"OK");
            total_error +=error;
        }
    }
#endif

#if  INCLUDE_RPC
    {
        int rpcInclude( int portmapdPriority ) ;
        PRINTF("rpcInclude start ... ");
        error=rpcInclude( PORTMAPD_PRIORITY );
        PRINTF(" %s\n",error?"Fail":"OK");
        total_error +=error;
    }
#endif /* INCLUDE_RPC */

#if (INCLUDE_RDB) && (INCLUDE_LOADER)
    {
        int rdbInclude (int rdbPr, int rdbThrPr, size_t rdbThrStkSz);
        PRINTF("rdbInclude start ...");
        error=rdbInclude(RDBTHREAD_PRIORITY,RDBFUNCTION_PRIORITY,RDBFUNCTION_STACKSIZE) ;
        PRINTF(" %s\n",error?"Fail":"OK");
        total_error +=error;
    }
#endif /* INCLUDE_RDB */

#if INCLUDE_NETTIME
    {
        int netSetSystemTimeFromTime(char *host,int tmo,int attempts);
        PRINTF("netSetSystemTimeFromTime start...");
        error=netSetSystemTimeFromTime(NETTIME_HOST_ADDRESS,NETTIME_TIMEOUT,NETTIME_ATTEMPT_COUNT);
        PRINTF(" %s\n",error?"Fail":"OK");
        total_error +=error;
    }
#endif

#if  INCLUDE_NFS &&  MOUNT_NFS_AUTO && (MOUNT_NFS0 || MOUNT_NFS1 || MOUNT_NFS2 || MOUNT_NFS3 )
    {
        int error;
        char buf[100];
        const char *nfs_mnt = "-t nfs -o rsize=%d,wsize=%d%s %s:%s %s";
        PRINTF("nfs mount start\n");
#define NFS_MOUNT(a) \
        sprintf(buf,nfs_mnt, \
        NFS_RSIZE##a,NFS_WSIZE##a,NFS_FORCE##a?",force":"",\
        NFS_HOST_ADDRESS##a,NFS_EXPORT##a,NFS_MOUNTPOINT##a);\
        error=_mount(buf);                                                       \
        PRINTF("mount %s %s \n",buf,error?"Fail":"OK");                          \
        total_error +=error;
#if MOUNT_NFS0
        NFS_MOUNT(0);
#endif
#if MOUNT_NFS1
        NFS_MOUNT(1);
#endif
#if MOUNT_NFS2
        NFS_MOUNT(2);
#endif
#if MOUNT_NFS3
        NFS_MOUNT(3);
#endif
        PRINTF("nfs mount done\n");
    }
#endif /*MOUNT_NFS_AUTO*/

#if  INCLUDE_NFS &&  INCLUDE_NFSSERVER
    {
        void nfsrv_init(int term);
        void nfsrv_initcache();
        void *nfsd(void *);
        void *mountd(void *);
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
        pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
        pthread_attr_setbreakstate( &attr, PTHREAD_BREAK_DISABLE );
        pthread_attr_setsystem( &attr, PTHREAD_CREATE_SYSTEM );
#if INCLUDE_TRACE
#if traceTHREADS == traceTHREADS_ALL || traceTHREADS == traceTHREADS_SYS
        pthread_attr_settracestate( &attr, PTHREAD_TRACE_ENABLE );
#else
        pthread_attr_settracestate( &attr, PTHREAD_TRACE_DISABLE );
#endif
#endif
        attr.fpustate =  PTHREAD_FPU_DISABLE;
        nfsrv_init(0);        /* Init server data structures */
        nfsrv_initcache();    /* Init the server request cache */
        pthread_create(0,&attr,nfsd,(void *)NFSSERVER_DAEMON_COUNT);
        pthread_create(0,&attr,mountd,NFSSERVER_EXPORTS);
        PRINTF("nfsd start\n");
    }
#endif
#if INCLUDE_IP_FIREWALL
    {
        int ipfw_init();
        ipfw_init();
        {
            int fd=open(IP_FIREWALL_RULES,0);
            if(fd != -1)
            {
                struct shellStartParam ss;
                bzero(&ss,sizeof(ss));
                ss.ssp_fdin = fd;
                ss.ssp_flags = SHELL_SSP_FDIN ;
                osShell(&ss);
                close(fd);
            }
        }
    }
#endif
    PRINTF("Network init error=%d\n",total_error);
    return  total_error;
}
