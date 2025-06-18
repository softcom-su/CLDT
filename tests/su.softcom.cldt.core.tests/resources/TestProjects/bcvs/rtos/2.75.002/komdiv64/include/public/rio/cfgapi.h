#ifndef _CFGAPI_H_
#define _CFGAPI_H_

#include <inttypes.h>
#include <sys/types.h>

typedef enum {
    RIO_SUCCESS                    = 0x0,    // Success status code
    RIO_WARN_INCONSISTENT          = 0x1,    // Used by rioRouteGetEntry?indicates that
                                             // the routeportno returned is not the same for
                                             // all ports
    RIO_ERR_SLAVE                  = 0x1001, // Another host has a higher priority
    RIO_ERR_INVALID_PARAMETER      = 0x1002, // One or more input parameters had an invalid
                                             // value
    RIO_ERR_RIO                    = 0x1003, // The RapidIO fabric returned a Response
                                             // Packet with ERROR status reported
    RIO_ERR_ACCESS                 = 0x1004, // A device-specific hardware interface was
                                             // unable to generate a maintenance
                                             // transaction and reported an error
    RIO_ERR_LOCK                   = 0x1005, // Another host already acquired the specified
                                             // processor element
    RIO_ERR_NO_DEVICE_SUPPORT      = 0x1006, // Device Access Routine does not provide
                                             // services for this device
    RIO_ERR_INSUFFICIENT_RESOURCES = 0x1007, // Insufficient storage available in
                                             // Device Access Routine private storage area
    RIO_ERR_ROUTE_ERROR            = 0x1008, // Switch cannot support requested routing
    RIO_ERR_NO_SWITCH              = 0x1009, // Target device is not a switch
    RIO_ERR_FEATURE_NOT_SUPPORTED  = 0x100A, // Target device is not capable of
                                             // per-input-port routing
} STATUS;

#define HOST_REGS 0xFFFFFFFF
//
//#define PORT_EP  254
//#define PORT_ALL 255

typedef u_int32_t UINT32;
typedef int32_t   INT32;
typedef u_int8_t  UINT8;
typedef u_int16_t UINT16;
typedef u_int16_t ADDR_MODE;

typedef struct
{
    UINT8  lport;
    UINT32 id;
    UINT8  hops;
} rioDest;

#define RIO_LOCAL_DEST ((rioDest){.lport = 0, .id = HOST_REGS, .hops = 0})

#define RIO_SWITCH_ID(id) ((id) >= 65536)
#define RIO_EP_ID(id) (0 <= (id) && (id) <= 65535)

/* === HAL API === */
INT32 rioGetNumLocalPorts(void);
STATUS rioConfigurationRead(UINT8 locport, UINT32 dstid, UINT8 hopcnt,
                            UINT32 offset, UINT32 *readdata);
STATUS rioConfigurationWrite(UINT8 locport, UINT32 dstid, UINT8 hopcnt,
                             UINT32 offset, UINT32 *writedata);
typedef STATUS(*rioHalInterceptor)(UINT8,UINT32,UINT8,UINT32,UINT32*);
void rioSetHalInterceptors(rioHalInterceptor readInter, rioHalInterceptor writeInter);
void rioClearHalInterceptors();

/* === Bring up API === */
STATUS rioInitLib(void);
STATUS rioGetFeatures(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *features);
STATUS rioGetSwitchPortInfo(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *portinfo);
STATUS rioGetPortsInfo(UINT8 lport, UINT32 dstId, UINT8 hops,
                       UINT8 *nPorts, UINT8 *srcPort);
STATUS rioGetExtFeaturesPtr(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *extfptr);
STATUS rioGetNextExtFeaturesPtr(UINT8 locport, UINT32 dstid, UINT8 hopcnt,
                                UINT32 curfptr, UINT32 *extfptr);
STATUS rioGetSourceOps(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *srcops);
STATUS rioGetDestOps(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *dstops);
STATUS rioGetAddressMode(UINT8 locport, UINT32 dstid, UINT8 hopcnt, ADDR_MODE *amode);
STATUS rioGetBaseDeviceId(UINT8 locport, UINT32 *deviceid);
STATUS rioSetBaseDeviceId(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 newdeviceid);
STATUS rioAquireDeviceLock(UINT8 locport, UINT32 dstid, UINT8 hopcnt,
                           UINT16 hostdeviceid, UINT16 *hostlockid);
STATUS rioReleaseDeviceLock(UINT8 locport, UINT32 dstid, UINT8 hopcnt,
                            UINT16 hostdeviceid, UINT16 *hostlockid);
STATUS rioGetComponentTag(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *comptag);
STATUS rioSetComponentTag(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 comptag);
STATUS rioGetPortErrStatus(UINT8 lport, UINT32 dstId, UINT8 hops, UINT16 extFOff,
                           UINT8 port, UINT32 *portErrStat);
STATUS rioSetPortErrStatus(UINT8 lport, UINT32 dstId, UINT8 hops, UINT16 extFOff,
                           UINT8 port, UINT32 portErrStat);
STATUS rioGetPortCtrl(UINT8 lport, UINT32 dstid, UINT8 hops, UINT16 extfoffset,
                      UINT8 portnum, UINT32 *portCtrl);
STATUS rioSetPortCtrl(UINT8 lport, UINT32 dstId, UINT8 hops, UINT16 extFOffset,
                        UINT8 port, UINT32 portCtrl);

/* non-standard */
STATUS rioGetDeviceIdentity(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT32 *devident);

/* === Routing API (not implemented due to identical functionality in DAR) === */
STATUS rioRouteAddEntry(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT8 tableidx,
                        UINT16 routedstid, UINT8 routeportno);
STATUS rioRouteGetEntry(UINT8 locport, UINT32 dstid, UINT8 hopcnt, UINT8 tableidx,
                        UINT16 routedstid, UINT8 *routeportno);

/* DAR API (Non-standard at all) */
struct rioDarDriver_tag;
typedef struct rioDarDriver_tag rioDarDriver;

typedef enum {
    RDC_ENDPOINT = 1,
    RDC_SWITCH = 2,
    RDC_SINGLE_LUT = 4,
    RDC_PERPORT_LUT = 8
} rioDevCaps;

STATUS rioDarGetDriver(UINT32 devId, const rioDarDriver **pdrv);
STATUS rioDarGetDriverForDevice(UINT8 lport, UINT32 dstId, UINT8 hopCnt,
                                const rioDarDriver **pdrv);
STATUS rioDarConfigure(const rioDarDriver *drv, UINT8 locport, UINT32 dstId, UINT8 hops);
STATUS rioDarWriteRoutingTable(const rioDarDriver *drv, UINT8 locport, UINT32 dstId,
                               UINT8 hops, UINT8 port, UINT8 *table, UINT16 entCnt);
STATUS rioDarRouteAddEntry(const rioDarDriver *drv, UINT8 locport, UINT32 dstId,
                           UINT8 hops, UINT8 inPort, UINT16 routDst, UINT8 routOutPort);
STATUS rioDarRouteGetEntry(const rioDarDriver *drv, UINT8 locport, UINT32 dstId,
                           UINT8 hops, UINT8 inPort, UINT16 routDst, UINT8 *routOutPort);
STATUS rioDarGetCaps(const rioDarDriver *drv, rioDevCaps *pcaps);
STATUS rioDarGetGeoAddr(const rioDarDriver *drv, UINT8 locport, UINT32 dstId,
                        UINT8 hops, UINT32 *geoAddr);
STATUS rioDarParseGeoAddr(const rioDarDriver *drv, UINT32 geoAddr,
                          UINT32 *machine, UINT32 *module, UINT32 *proc);
STATUS rioDarGetModel(const rioDarDriver *drv, char *buf, size_t len);
STATUS rioDarGetIdentity(const rioDarDriver *drv, UINT32 *identity);


#endif /* _CFGAPI_H_ */
