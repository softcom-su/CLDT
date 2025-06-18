#define RIO_SUCCESS                     0x0    // Success status code

#define RIO_WARN_INCONSISTENT           0x1    // Used by rioRouteGetEntry?indicates that
                                               // the routeportno returned is not the same for
                                               // all ports

#define RIO_ERR_SLAVE                   0x1001 // Another host has a higher priority

#define RIO_ERR_INVALID_PARAMETER       0x1002 // One or more input parameters had an invalid
                                               // value
#define RIO_ERR_RIO                     0x1003 // The RapidIO fabric returned a Response
                                               // Packet with ERROR status reported
#define RIO_ERR_ACCESS                  0x1004 // A device-specific hardware interface was
                                               // unable to generate a maintenance
                                               // transaction and reported an error
#define RIO_ERR_LOCK                    0x1005 // Another host already acquired the specified
                                               // processor element
#define RIO_ERR_NO_DEVICE_SUPPORT       0x1006 // Device Access Routine does not provide
                                               // services for this device
#define RIO_ERR_INSUFFICIENT_RESOURCES  0x1007 // Insufficient storage available in
                                               // Device Access Routine private storage area
#define RIO_ERR_ROUTE_ERROR             0x1008 // Switch cannot support requested routing
#define RIO_ERR_NO_SWITCH               0x1009 // Target device is not a switch
#define RIO_ERR_FEATURE_NOT_SUPPORTED   0x100A // Target device is not capable of
                                               // per-input-port routing

/* The HOST_REGS value below is a destination ID used to specify that the registers of
* the processor/platform on which the code is running are to be accessed.
*/

#define HOST_REGS			0xFFFFFFFF

typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint8_t UINT8;

extern int rioMaintInOut(unsigned hopCount, unsigned addr, unsigned dstID, unsigned data,
        unsigned* pData, const char* fname, unsigned* err);

/******************************************************************************/

/* we have only one port to RIO */

INT32 rioGetNumLocalPorts(void)
{
    return 1;
}

/******************************************************************************/

/* maintenance cycle in/out routine  */

static STATUS rioConfigurationInOut(UINT8 localport, UINT32 destid, UINT8 hopcount,
     UINT32 offset, UINT32* readdata, UINT32 writedata, const char* fname)
{
    unsigned err;
    if(localport != 0)
        return RIO_ERR_INVALID_PARAMETER;
    if(rioMaintInOut(hopcount,offset,destid,writedata,readdata,fname,&err) == -1)
    {
        switch(err)
        {
            case EINVAL:
                return RIO_ERR_INVALID_PARAMETER;
            case EIO:
                return RIO_ERR_RIO;
        }
        return RIO_ERR_ACCESS;
    }
    return RIO_SUCCESS;
}

STATUS rioConfigurationRead(UINT8 localport, UINT32 destid, UINT8 hopcount,
    UINT32 offset, UINT32 *readdata)
{
    if(!readdata)
        return RIO_ERR_INVALID_PARAMETER;
    return rioConfigurationInOut(localport,destid,hopcount,offset,readdata,0,__FUNCTION__);
}

STATUS rioConfigurationWrite(UINT8 localport, UINT32 destid, UINT8 hopcount,
    UINT32 offset, UINT32 *writedata)
{
    if(!writedata)
        return RIO_ERR_INVALID_PARAMETER;
    return rioConfigurationInOut(localport,destid,hopcount,offset,NULL,*writedata,__FUNCTION__);
}

/******************************************************************************/

/* write routine table for TSI500 switch */

static STATUS tundraWriteRoutingTable(UINT32 dstId, UINT8 hopCnt, UINT8 port,
    UINT8 *entries, UINT32 entCnt)
{
    unsigned val, i, j, p, res;

    if(port != 0xff && port > 3)
        return RIO_ERR_INVALID_PARAMETER;

    for(i = 0; i < entCnt; i += 8)
    {
        val = 0x88888888;

        for(j = 0; j < 8 && i + j < entCnt; j++)
        {
            if(entries[i+j] == 0xff)
                continue;

            p = entries[i+j];

            if(p > 3)
                return RIO_ERR_INVALID_PARAMETER;

            val = (val & ~(0xf << j*4)) | (p << j*4);
        }

        if(port == 0xff)
        {
            for(p = 0; p < 4; p++)
                if((res = rioConfigurationWrite(0,dstId,hopCnt,0x10a00+i/2+p*0x20000,&val)))
                    return res;
        }
        else
        {
            if((res = rioConfigurationWrite(0,dstId,hopCnt,0x10a00+i/2+port*0x20000,&val)))
                return res;
        }
    }
    return RIO_SUCCESS;
}

/******************************************************************************/

/* write routine table for NIISI switch */

static STATUS niisiWriteRoutingTable(UINT32 devId, UINT32 dstId, UINT8 hopCnt,
    UINT8 port, UINT8 *entries, UINT32 entCnt)
{
    unsigned i, offset = 0, maxPort, p, res;

    switch(devId)
    {
        case 0x87170074:
            if(port != 0xff)
                return RIO_ERR_INVALID_PARAMETER;
            maxPort = 1;
            break;
        case 0x00010074:
            if(port != 0xff && port > 7)
                return RIO_ERR_INVALID_PARAMETER;
            maxPort = 7;
            break;
        default:
            return RIO_ERR_NO_DEVICE_SUPPORT;
    }

    if(port != 0xff)
        offset = 0x10000 + port*0x400;

    for(i = 0; i < entCnt; i++)
    {
        p = entries[i];

        if(p != 0xff && p > maxPort)
            return RIO_ERR_INVALID_PARAMETER;

        if((res = rioConfigurationWrite(0,dstId,hopCnt,offset+CSR_ROUTE_CFG_DESTID,&i)))
            return res;

        if((res = rioConfigurationWrite(0,dstId,hopCnt,offset+CSR_ROUTE_CFG_PORT,&p)))
            return res;
    }

    return RIO_SUCCESS;
}

/******************************************************************************/

/* common write routine table routine */

STATUS rioSwitchWriteRoutingTable(UINT32 devId, UINT8 localport, UINT32 dstId,
    UINT8 hopCnt, UINT8 port, UINT8 *entries, UINT32 entCnt)
{
    if(localport != 0 || !entCnt || !entries)
        return RIO_ERR_INVALID_PARAMETER;

    if(devId == 0x0500000d)
        return tundraWriteRoutingTable(dstId,hopCnt,port,entries,entCnt);

    if((devId & 0xffff) == 0x0074)
        return niisiWriteRoutingTable(devId,dstId,hopCnt,port,entries,entCnt);

    return RIO_ERR_NO_DEVICE_SUPPORT;
}

/******************************************************************************/

/* additional configuration for TSI500 switch */

static STATUS tundraConfigure(UINT32 dstId, UINT8 hopCnt)
{
    unsigned i, res, val;

    for(i = 0, val = TUNDRA_FAB_TEA_CTL_VAL; i < 4; i++)
    {
        if((res = rioConfigurationWrite(0,dstId,hopCnt,FAB_TEA_CTL0+i*4,&val)))
            return res;
    }

    val = TUNDRA_LT_CTL_VAL;
    if((res = rioConfigurationWrite(0,dstId,hopCnt,PORT_LT_CTL,&val)))
        return res;

    return RIO_SUCCESS;
}

/******************************************************************************/

/* additional configuration for NIISI switch */

static STATUS niisiConfigure(UINT32 devId, UINT32 dstId, UINT8 hopCnt)
{
    unsigned res, val;

    /* set default port to 0xff */

    val = 0xff;
    if((res = rioConfigurationWrite(0,dstId,hopCnt,CSR_LUT_ATTR,&val)))
        return res;

    val = NIISI_PKT_TTL_VAL;
    if((res = rioConfigurationWrite(0,dstId,hopCnt,PKT_TTL,&val)))
        return res;

    return RIO_SUCCESS;
}

/******************************************************************************/

/* additional switch configuration routine */

STATUS rioSwitchConfigure(UINT32 devId, UINT8 localport, UINT32 dstId, UINT8 hopCnt)
{
    if(localport != 0)
        return RIO_ERR_INVALID_PARAMETER;

    if(devId == 0x0500000d)
        return tundraConfigure(dstId,hopCnt);
    
    if((devId & 0xffff) == 0x0074)
        return niisiConfigure(devId,dstId,hopCnt);
    
    return RIO_ERR_NO_DEVICE_SUPPORT;
}

