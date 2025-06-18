#ifndef PUBLIC_SYS_ARINC653_H
#define PUBLIC_SYS_ARINC653_H

#include <version.h>
#include <arinc/partition.h>

#if OS_MAJOR >= 3
#  include <health_monitor.h>
#  include <sys/xsd_convertion.h>
#else
#  include <public/xsd_convertion.h>
#endif
#if OS_MAJOR >= 3
struct Error_ID_LevelType {
   CERROR_CODE_TYPE                         eidlt_ErrorIdentifier;
   NAME_TYPE                                eidlt_Description;
   ERROR_LEVEL_TYPE                         eidlt_ErrorLevel;
   ERROR_CODE_TYPE                          eidlt_ErrorCode;
};
struct Memory_RequirementsType {
   NAME_TYPE                                mrt_RegionName;
   MEMORY_REQ_TYPE_TYPE                     mrt_Type;
   APEX_INTEGER                             mrt_SizeBytes;
};
struct Partition_MemoryType {
   PARTITION_REF_TYPE                       pmt_PartitionIdentifier;
   REF_NAME_TYPE                            pmt_PartitionName;
   struct {
      int size;
      const struct Memory_RequirementsType *Memory_Requirements;
   } pmt_Memory_Requirements;
};
struct System_State_EntryType {
   SYSTEM_STATE_TYPE                        sset_SystemState;
   NAME_TYPE                                sset_Description;
   struct {
      int size;
      const struct Error_ID_LevelType *Error_ID_Level;
   } sset_Error_ID_Level;
};
struct System_HM_TableType {
   struct {
      int size;
      const struct System_State_EntryType *System_State_Entry;
   } shmtt_System_State_Entry;
};
struct Module_HM_Type {
   NAME_TYPE                                mhmt_ModuleCallback;
   struct {
      int size;
      const struct ModuleSystem_State_EntryType *System_State_Entry;
   } mhmt_System_State_Entry;
};

struct PartitionLogbookType {
   int plt_MaxNBInProgressMessages;
   int plt_MaxNBLoggedMessages;
   int plt_MaxMessageSize;
   NAME_TYPE plt_OwnerPartitionName;
   NAME_TYPE plt_LogbookName;
   int plt_PartitionIdentifier;
};
struct LogicalDevice {
   NAME_TYPE   ld_DeviceName;
   NAME_TYPE   ld_Name;
   struct {
      int size;
      const struct PartitionLogbookType *LogBook;
   } ld_LogBook;
};

struct ModuleConfig {
   string                                   mc_noNamespaceSchemaLocation;
   NAME_TYPE                                mc_ModuleName;
   NAME_TYPE                                mc_ModuleVersion;
   APEX_INTEGER                             mc_ModuleId;
   struct System_HM_TableType             mc_System_HM_Table ;
   struct Module_HM_Type                  mc_Module_HM_Table ;
   struct {
      int size;
      const struct PartitionType *Partition;
   } mc_Partition;
   struct {
      int size;
      const struct Partition_MemoryType *Partition_Memory;
   } mc_Partition_Memory;
   struct {
      int size;
      const struct Partition_HM_Type *Partition_HM_Table;
   } mc_Partition_HM_Table;
   struct Connection_Table {
      struct {
         int size;
         const struct ChannelType *Channel;
      } ct_Channel;
   } mc_Connection_Table;
   struct ModExt {
      struct {
         int size;
         const struct ModuleScheduleType *ModuleSchedule;
      } me_ModuleSchedule;
   } mc_ModExt;
   struct LogbookSystem {
      struct {
        int size;
        const struct LogicalDevice *LogicalDevice;
      } ls_LogicalDevice;
   } mc_LogbookSystem;
};

struct ModuleError_ID_ActionType {
   CERROR_CODE_TYPE                         meidat_ErrorIdentifier;
   NAME_TYPE                                meidat_Description;
   MODULE_ERROR_ACTION_TYPE                 meidat_Action;
};
struct PartitionError_ID_ActionType {
   CERROR_CODE_TYPE                         peidat_ErrorIdentifier;
   NAME_TYPE                                peidat_Description;
   PARTITION_ERROR_ACTION_TYPE              peidat_Action;
};

struct PartitionScheduleType {
   PARTITION_REF_TYPE                       pst_PartitionIdentifier;
   REF_NAME_TYPE                            pst_PartitionName;
   boolean                                  pst_SetModuleSchedule;
   PARTITION_ERROR_ACTION_TYPE              pst_ScheduleChangeAction;
};
struct Partition_HM_Type {
   PARTITION_REF_TYPE                       phmt_PartitionIdentifier;
   REF_NAME_TYPE                            phmt_PartitionName;
   NAME_TYPE                                phmt_PartitionCallback;
   struct {
      int size;
      const struct PartitionSystem_State_EntryType *System_State_Entry;
   } phmt_System_State_Entry;
};
struct ModuleSystem_State_EntryType {
   SYSTEM_STATE_TYPE                        msset_SystemState;
   NAME_TYPE                                msset_Description;
   struct {
      int size;
      const struct ModuleError_ID_ActionType *Error_ID_Action;
   } msset_Error_ID_Action;
};
struct PartitionSystem_State_EntryType {
   SYSTEM_STATE_TYPE                        psset_SystemState;
   NAME_TYPE                                psset_Description;
   struct {
      int size;
      const struct PartitionError_ID_ActionType *Error_ID_Action;
   } psset_Error_ID_Action;
};
struct WindowPartition {
   PARTITION_LIST_ITEM_TYPE                 wp_PartitionIdentifier;
   PARTITION_NAME_TYPE                      wp_PartitionName;
   boolean                                  wp_PartitionPeriodStart;
   int                                      wp_PartitionMinPrio;
   APEX_INTEGER                             wp_Core;
};
struct WindowConfig {
   APEX_INTEGER                             wc_WindowIdentifier;
   system_time_t                            wc_WindowDurationSeconds;
   struct {
      int size;
      const struct WindowPartition *Partition;
   } wc_Partition;
};
struct ModuleScheduleType {
   system_time_t                            mst_MajorFrameSeconds;
   NAME_TYPE                                mst_ScheduleName;
   APEX_INTEGER                             mst_ScheduleIdentifier;
   boolean                                  mst_InitialModuleSchedule;
   struct {
      int size;
      const struct PartitionScheduleType *Partition_Schedule;
   } mst_Partition_Schedule;
   struct {
      int size;
      const struct WindowConfig *Window;
   } mst_Window;
};

#else   /* OS_MAJOR >= 3 */

/* OS_MAJOR == 2 */
struct PartitionType;
struct ChannelType;
struct ModuleConfig {
   struct {
      int size;
      const struct PartitionType *Partition;
   } mc_Partition;
   struct Connection_Table {
      struct {
         int size;
         const struct ChannelType *Channel;
      } ct_Channel;
   } mc_Connection_Table;
};
#endif   /* OS_MAJOR == 2 */

enum {
   Standard_Partition_t = 1,
   Pseudo_Partition_t
};

union PortMappingType {
   int   type;
   struct Standard_Partition {
      PARTITION_REF_TYPE                    sp_PartitionIdentifier;
      REF_NAME_TYPE                         sp_PartitionName;
      NAME_TYPE                             sp_PortName;
      APEX_INTEGER                          sp_PhysicalAddress;
   }     pmt_Standard_Partition;
   struct Pseudo_Partition {
      NAME_TYPE                             pp_Name;
      APEX_INTEGER                          pp_PhysicalAddress;
      BOOLEAN                               pp_NoHeader;
      APEX_INTEGER                          pp_Link;
      NAME_TYPE                             pp_Partner;
   }     pmt_Pseudo_Partition;
};
struct ChannelType {
   APEX_INTEGER                             ct_ChannelIdentifier;
   NAME_TYPE                                ct_ChannelName;
   union PortMappingType                    ct_Source ;
   struct {
      int size;
      const union PortMappingType *Destination;
   } ct_Destination;
};
struct SamplingPortType {
   APEX_INTEGER                             spt_ref;
   APEX_INTEGER                             spt_MaxMessageSize;
   system_time_t                            spt_RefreshRateSeconds;
   PORT_DIRECTION_TYPE                      spt_Direction;
   NAME_TYPE                                spt_Name;
};
struct QueuingPortType {
   APEX_INTEGER                             qpt_ref;
   APEX_INTEGER                             qpt_MaxNbMessages;
   APEX_INTEGER                             qpt_MaxMessageSize;
   PORT_DIRECTION_TYPE                      qpt_Direction;
   NAME_TYPE                                qpt_Name;
};
struct SAPPortType {
   APEX_INTEGER                             sappt_ref;
   integer                                  sappt_MaxNbMessages;
   APEX_INTEGER                             sappt_MaxMessageSize;
   PORT_DIRECTION_TYPE                      sappt_Direction;
#define UDP_PROTOCOL 1
   int                                      sappt_ProtocolType;
   int                                      sappt_ExtendedPort;
   NAME_TYPE                                sappt_Name;
};

struct ProcessType {
   NAME_TYPE                                pt_Name;
   APEX_INTEGER                             pt_StackSize;
};
struct PartitionExtType {
   APEX_INTEGER                             pet_KernelStackSize;
};

typedef enum {
    READ_WRITE,
    READ_ONLY
} MEMORY_ACCESS_TYPE;

struct MemoryRegionType {
   NAME_TYPE               mrt_Name;
   NAME_TYPE               mrt_Type;
   APEX_INTEGER            mrt_Size;
   APEX_INTEGER            mrt_Address;
   MEMORY_ACCESS_TYPE      mrt_AccessRights;
};
struct PartitionType {
   APEX_INTEGER                             pt_PartitionIdentifier;
   REF_NAME_TYPE                            pt_PartitionName;
   CRITICALITY_TYPE                         pt_Criticality;
   boolean                                  pt_SystemPartition;
   NAME_TYPE                                pt_EntryPoint;
   struct {
      int size;
      const struct MemoryRegionType *MemoryRegion;
   } pt_MemoryRegion;
   struct {
      int size;
      const struct SamplingPortType *Sampling_Port;
   } pt_Sampling_Port;
   struct {
      int size;
      const struct QueuingPortType *Queuing_Port;
   } pt_Queuing_Port;
   struct {
      int size;
      const struct SAPPortType *SAP_Port;
   } pt_SAP_Port;
   struct {
      int size;
      const struct ProcessType *Process;
   } pt_Process;
   struct {
      int size;
      const struct PartitionExtType *PartitionExt;
   } pt_PartitionExt;
};
#endif /* PUBLIC_SYS_ARINC653_H */
