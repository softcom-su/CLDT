#ifndef _TDESCSTRUCTS_H_
#define _TDESCSTRUCTS_H_

typedef char string[128];
typedef char version[16];
typedef char IPAddr[16];
typedef int RioId;
typedef int RioSwitchId;
typedef int RioEndpId;
typedef int RioPort;

/* все, что ниже, автоматически сгенерировано с помощью $(OSBASE)/bin/ps
   на основе схемы $(OSBASE)/lib/xml-tdesc.xsd */

struct HldFile { 
   string                                   hf_Path;
   version                                  hf_Version;
};
struct HldFiles { 
   struct {
      int size;
      int esize;
      const struct HldFile *HldFile;
   } hf_HldFile;
};
typedef enum { 
   TDESC_LM_SELF,
   TDESC_LM_PASSIVE,
   TDESC_LM_REQUEST,
} LoadMode;
struct Load { 
   LoadMode                                 l_Mode;
   string                                   l_Path;
   string                                   l_Address;
};
struct EthernetInterface { 
   string                                   ei_Name;
   string                                   ei_Addr;
   IPAddr                                   ei_Mask;
};
struct Ethernet { 
   struct {
      int size;
      int esize;
      const struct EthernetInterface *Interface;
   } e_Interface;
};
struct Route { 
   IPAddr                                   r_Destination;
   IPAddr                                   r_Mask;
   IPAddr                                   r_Gateway;
};
struct IpRouting { 
   IPAddr                                   ir_DefaultGateway;
   struct {
      int size;
      int esize;
      const struct Route *Route;
   } ir_Route;
};
typedef enum { 
   TDESC_MP_FTP,
   TDESC_MP_TFTP,
   TDESC_MP_NFS,
   TDESC_MP_VFAT,
   TDESC_MP_TAR,
} MountProto;
struct MountDir { 
   IPAddr                                   md_ServerIp;
   MountProto                               md_Protocol;
   string                                   md_Path;
   string                                   md_Address;
   int                                      md_Size;
   string                                   md_MountPoint;
   int                                      md_Uid;
   int                                      md_Gid;
   string                                   md_User;
   string                                   md_Password;
};
struct Mounts { 
   struct {
      int size;
      int esize;
      const struct MountDir *MountDir;
   } m_MountDir;
};
struct ReplFile { 
   string                                   rf_Name;
   string                                   rf_SourcePath;
   string                                   rf_SourceAddress;
   int                                      rf_Size;
   string                                   rf_TargetPath;
   string                                   rf_TargetAddress;
   string                                   rf_Type;
};
struct Reconf { 
   struct {
      int size;
      int esize;
      const struct ReplFile *ReplFile;
   } r_ReplFile;
};
struct Cpu { 
   int                                      c_GeoId;
   int                                      c_RioId;
   int                                      c_HostRioId;
   string                                   c_Name;
   unsigned                                 c_LoaderFlags;
   int                                      c_ReconfFlags;
   struct Load                            c_Load ;
   struct Ethernet                        c_Ethernet ;
   struct IpRouting                       c_IpRouting ;
   struct Mounts                          c_Mounts ;
   struct Reconf                          c_Reconf ;
};
typedef enum { 
   TDESC_SM_GENERAL,
   TDESC_SM_LOAD,
   TDESC_SM_RECONF,
   TDESC_SM_OS,
} StartupMode;
struct Startup { 
   StartupMode                              s_Mode;
   string                                   s_Submode;
   struct {
      int size;
      int esize;
      const struct Cpu *Cpu;
   } s_Cpu;
};
struct RTableEntry { 
   RioEndpId                                rte_Id;
   RioPort                                  rte_OutPort;
   int                                      rte_Span;
};
struct RoutingTable { 
   RioPort                                  rt_InPort;
   struct {
      int size;
      int esize;
      const struct RTableEntry *TableEntry;
   } rt_TableEntry;
};
typedef enum { 
   TDESC_RTT_SINGLE,
   TDESC_RTT_PERPORT,
} SwitchLutKind;
struct Switch { 
   RioSwitchId                              s_Id;
   string                                   s_Name;
   SwitchLutKind                            s_Type;
   RioEndpId                                s_Host;
   struct {
      int size;
      int esize;
      const struct RoutingTable *RTable;
   } s_RTable;
};
struct Switches { 
   struct {
      int size;
      int esize;
      const struct Switch *Switch;
   } s_Switch;
};
struct Link { 
   RioPort                                  l_LocalPort;
   RioId                                    l_PeerId;
   RioPort                                  l_PeerPort;
};
struct RioDevice { 
   RioId                                    rd_Id;
   struct {
      int size;
      int esize;
      const struct Link *Link;
   } rd_Link;
};
struct Topology { 
   struct {
      int size;
      int esize;
      const struct RioDevice *Device;
   } t_Device;
};
struct Description { 
   string                                   d_Name;
   version                                  d_Version;
   version                                  d_CompilerVersion;
   struct HldFiles                        d_HldFiles ;
   struct {
      int size;
      int esize;
      const struct Startup *Startup;
   } d_Startup;
   struct Switches                        d_Switches ;
   struct Topology                        d_Topology ;
};

#endif
